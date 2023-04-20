// Description:
//   This is a base class of Pipe including all members not related to specific
//   pipe element type
//   This class is used for declaring a pointer to the template class Pipe.

#include "Exception.h"
#include "GLog.h"

#include "PipeBase.h"
#include "PipeReadTokenInfo.h"
#include "PipeWriteTokenInfo.h"
#include <iostream>

namespace CyclePipe
{
	PipeBase::PauseScope PipeBase::GetPauseScope()
	{
		return PauseScope(std::make_shared<PauseScopeImpl>(this));
	}

	PipeBase::PipeBase(const std::string& pipeName)
		: pipeName(pipeName)
		, numWriters(0)
		, writeSize(0)
		, numReaders(0)
		, readSize(0)
		, overlapSize(0)
		, bufferSize(0)
		, writeDonePos(0)
		, writingPos(0)
		, readDonePos(0)
		, readingPos(0)
		, consumerName("")
		, producerName("")
	{
	}

	PipeBase::~PipeBase()
	{
		std::unique_lock lk(mutex);
		while (!writtingTokens.empty() || !readingTokens.empty())
		{
			condition.wait(lk);
		}
		if (pDumpFileStream.has_value())
			pDumpFileStream->close();
		if (pLoadFileStream.has_value())
			pLoadFileStream->close();
	}

	void PipeBase::SetConsumer(
		const std::string& name,
		const size_t number,
		const size_t sizeIncludingOverlap,
		const size_t numFramesOverlap)
	{
		std::lock_guard lk(mutex);
		if (numReaders * readSize != 0)
			ThrowExceptionAndLog("consumer is already set.");
		if (number == 0)
			ThrowExceptionAndLog("number of reader can not be 0.");
		if (sizeIncludingOverlap == 0)
			ThrowExceptionAndLog("read size can not be 0.");
		consumerName = name;
		numReaders = number;
		readSize = sizeIncludingOverlap;
		overlapSize = numFramesOverlap;
		InitBufferIfParamsAreReady();
		condition.notify_all();
	}

	void PipeBase::SetProducer(
		const std::string& name,
		const size_t number,
		const size_t size)
	{
		std::lock_guard lk(mutex);
		if (numWriters * writeSize != 0)
			ThrowExceptionAndLog("consumer is already set.");
		if (number == 0)
			ThrowExceptionAndLog("number of writers can not be 0.");
		if (size == 0)
			ThrowExceptionAndLog("write size can not be 0.");
		producerName = name;
		numWriters = number;
		writeSize = size;
		InitBufferIfParamsAreReady();
		condition.notify_all();
	}

	void PipeBase::SetBufferSize(const size_t size)
	{
		if (bufferSize != 0)
			ThrowExceptionAndLog("pipe size is already set.");
		bufferSize = size;
		InitBufferIfParamsAreReady();
	}

	void PipeBase::SetElementShape(const std::vector<size_t>& dimentions)
	{
		elementShape = dimentions;
	}

	void PipeBase::SetBufferSizeForMaxConcurrency()
	{
		if (bufferSize != 0 && bufferSize != SIZE_MAX)
			ThrowExceptionAndLog("pipe size is already set.");
		bufferSize = SIZE_MAX;
		InitBufferIfParamsAreReady();
	}

	const std::vector<size_t>& PipeBase::GetElementShape() const
	{
		return elementShape;
	}

	size_t PipeBase::GetWritingPos()
	{
		std::shared_lock lg(mutex);
		return writingPos;
	}

	size_t PipeBase::GetWriteDonePos()
	{
		std::shared_lock lg(mutex);
		return writeDonePos;
	}

	size_t PipeBase::GetReadingPos()
	{
		std::shared_lock lg(mutex);
		return readingPos;
	}

	size_t PipeBase::GetReadDonePos()
	{
		std::shared_lock lg(mutex);
		return readDonePos;
	}

	void PipeBase::Pause()
	{
		pauseLock = std::make_shared< std::lock_guard<std::shared_mutex>>(mutex);
	}

	void PipeBase::Resume()
	{
		pauseLock.reset();
	}

	std::shared_ptr<PipeWriteTokenInfo> PipeBase::GetWriteTokenInfo(
		size_t tokenWriteSize,
		bool isShotEnd)
	{
#ifdef _DEBUG
		std::unique_lock<std::mutex> lk(seqGetWriteTokenMutex, std::defer_lock);
		if (!lk.try_lock())
			ThrowExceptionAndLog("GetWriteToken() should not be concurrent!");
#endif
		if (tokenWriteSize > bufferSize)
		{
			ThrowExceptionAndLog(
				"Pipe::GetWriteToken(): write size is larger than buffer size!");
		}
		std::unique_lock lock(mutex);

		while (writingPos + tokenWriteSize > writeDonePos + bufferSize)
		{
			condition.wait(lock);
		}

		while (!closed && !IsBufferReadyToWrite(tokenWriteSize))
		{
			condition.wait(lock);
		}

		if (closed)
		{
			ThrowExceptionAndLog(
				"Pipe: trying to get writoken when pipe is closed!");
		}

		const bool isShotStart = (shotsToRead.empty()) ||
			(writingPos == shotsToRead.back().EndIndex);
		if (isShotStart)
		{
			shotsToRead.emplace(ShotInfo(writingPos));
		}

		WriteTokenInfoPtr pWriteTokenInfo = std::make_shared<PipeWriteTokenInfo>(
			this,
			writingPos,
			tokenWriteSize,
			isShotStart, isShotEnd);
		writtingTokens.emplace(pWriteTokenInfo);

		writingPos += tokenWriteSize;
		if (isShotEnd)
		{
			shotsToRead.back().EndIndex = writingPos;
			if (shotsToRead.back().EndIndex - shotsToRead.back().StartIndex < overlapSize + 1)
				ThrowExceptionAndLog("Shot size is smaller than or equal to overlap size.");
		}

		return pWriteTokenInfo;
	}

	void PipeBase::CalculateBufferSize()
	{
		if (bufferSize == 0)
		{
			bufferSize = MinimumBufferSize();
		}
		else if (bufferSize == SIZE_MAX)
		{
			bufferSize = MaximumConcurrencyBufferSize();
		}
		if (writeSize * numWriters > bufferSize)
			GDisplay("Pipe " + pipeName + " (pipeSize = " + std::to_string(bufferSize) + ") dose not allow "
				+ producerName + " with " + std::to_string(numWriters) + " writers and "
				+ std::to_string(writeSize) + " frames each write work concurrently.\n");
	}

	size_t PipeBase::MaximumConcurrencyBufferSize() const
	{
		size_t sizeForRead = numReaders * (readSize - overlapSize) +
			overlapSize;
		size_t sizeForWrite = numWriters * writeSize;
		return  sizeForRead + sizeForWrite;

	}

	size_t PipeBase::MinimumBufferSize() const
	{
		size_t minimumSize = std::max(writeSize, (numReaders - 1) * (readSize - overlapSize) + readSize);
		// not so simple! now the pipe may be blocked!!

		size_t tempWritingPos = writeSize;
		size_t tempReadingPos = 0;
		size_t tempWriteDonePos = 0;
		size_t tempReadDonePos = 0;
		size_t tempReaders = 0;
		std::vector<std::vector<size_t>> states;
		while (true)
		{
			size_t curBufSize = tempWritingPos - tempReadDonePos;

			states.emplace_back(std::vector < size_t>({ curBufSize,
				tempWriteDonePos - tempReadDonePos,
				tempReadingPos - tempReadDonePos }));

			minimumSize = std::max(minimumSize, curBufSize);
			if (states.size() > 5
				&&
				std::find(
					states.data(), states.data() + states.size() - 1,
					states.back())
				!= states.data() + states.size() - 1)
			{
				break;
			}
			if (tempWritingPos != tempWriteDonePos)
			{
				tempWriteDonePos += writeSize;
				continue;
			}
			else if (tempReadingPos + ((tempReadingPos == 0) ? readSize : (readSize - overlapSize)) <= tempWriteDonePos)
			{
				tempReadingPos += (tempReadingPos == 0) ? readSize : (readSize - overlapSize);
				++tempReaders;
				continue;
			}
			else if (tempReaders > 0)
			{
				tempReadDonePos += (readSize - overlapSize);
				--tempReaders;
				continue;
			}
			else
			{
				tempWritingPos += writeSize;
				continue;
			}
		}
		return minimumSize;
	}

	bool PipeBase::IsBufferReadyToWrite(size_t currentWriteSize)
	{
		const size_t possibleReadingPos = readingTokens.empty() ?
			(shotsToRead.empty() ? readingPos : CalcReadStartPos())
			:
			readingTokens.front()->StartIndex;
		const bool isBufferReadyToWrite = writingPos + currentWriteSize <=
			possibleReadingPos + bufferSize;
		if (!isBufferReadyToWrite &&
			readingTokens.empty() &&
			writtingTokens.empty() &&
			!IsDataReadyToRead())
		{
			closed = true;
			condition.notify_all();
			ThrowExceptionAndLog("Pipe deadlock");
		}
		return isBufferReadyToWrite;
	}

	std::shared_ptr<PipeReadTokenInfo> PipeBase::GetReadTokenInfo()
	{
#ifdef _DEBUG
		std::unique_lock<std::mutex> lk(seqGetReadTokenMutex, std::defer_lock);
		if (!lk.try_lock())
			ThrowExceptionAndLog("GetReadToken() should not be concurrent!");
#endif

		std::unique_lock lock(mutex);

		while (!IsDataReadyToRead())
		{
			if (closed && writtingTokens.empty())
			{
				throw PipeClosedAndEmptySignal();
			}
			condition.wait(lock);
		}

		const size_t actualReadingPos(CalcReadStartPos());
		const size_t actualOverlapSize = readingPos - actualReadingPos;
		const size_t actualReadSize = CalcReadEndPos() - actualReadingPos;
		const bool isShotStart =
			actualReadingPos == shotsToRead.front().StartIndex;
		const bool isShotEnd =
			(actualReadingPos + actualReadSize) == shotsToRead.front().EndIndex;

		if (isShotEnd)
		{
			shotsToRead.pop();
		}

		ReadTokenInfoPtr pReadTokenInfo =
			std::make_shared<PipeReadTokenInfo>(
				this,
				actualReadingPos,
				actualReadSize, actualOverlapSize,
				isShotStart, isShotEnd);

		readingTokens.push(pReadTokenInfo);
		readingPos += actualReadSize - actualOverlapSize;

		if (pLoadFileStream.has_value())
		{
			LoadReadToken(
				pReadTokenInfo->StartIndex + pReadTokenInfo->OverlapSize,
				pReadTokenInfo->StartIndex + pReadTokenInfo->Size);
		}

		return pReadTokenInfo;
	}

	bool PipeBase::IsDataReadyToRead()
	{
		return !shotsToRead.empty() &&
			(CalcReadEndPos() <=
				std::min(writeDonePos, readDonePos + bufferSize));
	}

	size_t PipeBase::CalcReadEndPos()
	{
		return std::min(
			CalcReadStartPos() +
			readSize,
			shotsToRead.front().EndIndex
		);
	}

	size_t PipeBase::CalcReadStartPos()
	{
		return std::max(
			shotsToRead.front().StartIndex,
			readingPos > overlapSize ?
			readingPos - overlapSize : 0
		);
	}

	void PipeBase::ClearFinishedWriteTokens()
	{
		std::lock_guard lk(mutex);

		while (!writtingTokens.empty() && writtingTokens.front().use_count() == 1)
		{
			writeDonePos += writtingTokens.front()->Size;
			if (pDumpFileStream.has_value())
			{
				DumpWriteToken(writtingTokens.front()->StartIndex, writeDonePos);
			}
			writtingTokens.pop();
			condition.notify_all();
		}

	}

	void PipeBase::ClearFinishedReadTokens()
	{
		std::lock_guard lk(mutex);

		while (!readingTokens.empty() && readingTokens.front().use_count() == 1)
		{
			readDonePos =
				readingTokens.front()->StartIndex +
				readingTokens.front()->Size;
			readingTokens.pop();
			condition.notify_all();
		}

	}

	PipeBase::PauseScopeImpl::PauseScopeImpl(PipeBase* pOwner)
		:pOwner(pOwner)
	{
		pOwner->Pause();
	}

	PipeBase::PauseScopeImpl::~PauseScopeImpl()
	{
		pOwner->Resume();
	}

}
