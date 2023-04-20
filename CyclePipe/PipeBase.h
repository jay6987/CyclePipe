#pragma once

#include <queue>
#include <fstream>
#include <filesystem>
#include <memory>
#include <map>
#include <optional>
#include <any>

#include "SyncBase.h"

#include "PipeReadTokenInfo.h"
#include "PipeWriteTokenInfo.h"

namespace CyclePipe
{
	struct PipeClosedAndEmptySignal : SyncObjectClosedAndEmptySignal {};

	class PipeBase : public SyncBase
	{
	protected:

		typedef std::shared_ptr<PipeReadTokenInfo> ReadTokenInfoPtr;
		typedef std::shared_ptr<PipeWriteTokenInfo> WriteTokenInfoPtr;

		class PauseScopeImpl
		{
		public:
			PauseScopeImpl(PipeBase* pOwner);
			~PauseScopeImpl();
		private:
			PipeBase* pOwner;
		};

	public:

		typedef std::shared_ptr<PauseScopeImpl> PauseScope;
		PauseScope GetPauseScope();

		PipeBase(const std::string& pipeName);

		virtual ~PipeBase();

		void SetConsumer(
			const std::string& name,
			const size_t number,
			const size_t sizeIncludingOverlap,
			const size_t overlapSize);

		void SetProducer(
			const std::string& name,
			const size_t number,
			const size_t size);

		// If this function is NOT called, the default size of 
		// buffer is set to a minimum size that all readers
		// can work simultaneously at at their default sizes.
		// e.g.
		// 3 writers and 2 readers, each thread operate on 1 buffer,
		// the total buffer size will be set to 2.
		void SetBufferSize(const size_t size);

		// If this function is called, the default size of 
		// buffer is set to a minimum size that all readers AND all writers
		// can work simultaneously at at their default sizes.
		// e.g.
		// 2 writers and 3 readers, each thread operate on 1 buffer,
		// the total buffer size will be set to 5.
		void SetBufferSizeForMaxConcurrency();

		const std::vector<size_t>& GetElementShape() const;

		struct ExtPropertyNotExist {};
		template<typename T>
		T GetExtProperty(const std::string& key)
		{
			if (extendedProperties.find(key) == extendedProperties.end())
				throw ExtPropertyNotExist();
			return std::any_cast<T>(extendedProperties.at(key));
		}

		// if the property already exists, the value will be replaced.
		template<typename T>
		void SetExtProperty(const std::string& key, const T& value)
		{
			extendedProperties[key] = value;
		}
		virtual void SetDump(const std::filesystem::path& fileName) = 0;

		virtual void SetLoad(const std::filesystem::path& fileName) = 0;

		virtual std::string GetElementType() const = 0;
		const std::string& GetName() const { return pipeName; }
		const std::string& GetConsumer() const { return consumerName; }
		const std::string& GetProducer() const { return producerName; }
		size_t GetNumReaders() const { return numReaders; }
		size_t GetReadSize() const { return readSize; }
		size_t GetOverlapSize() const { return overlapSize; }
		size_t GetNumWriters() const { return numWriters; }
		size_t GetWriteSize() const { return writeSize; }
		size_t GetBufferSize() const { return bufferSize; }

		size_t GetWritingPos();
		size_t GetWriteDonePos();
		size_t GetReadingPos();
		size_t GetReadDonePos();

		void ClearFinishedWriteTokens();
		void ClearFinishedReadTokens();

	protected:

		void Pause();
		void Resume();

		void SetElementShape(const std::vector<size_t>& dimentions);

		WriteTokenInfoPtr GetWriteTokenInfo(size_t tokenWriteSize, bool isShotEnd);
		ReadTokenInfoPtr GetReadTokenInfo();

		size_t numWriters;
		size_t writeSize;
		size_t numReaders;
		size_t readSize;
		size_t overlapSize;
		size_t bufferSize;

		std::optional<std::ofstream> pDumpFileStream;
		std::optional<std::ifstream> pLoadFileStream;

		void CalculateBufferSize();

	private:

		// return buffer size that ensure all writers and reader can work
		// in the same time.
		size_t MaximumConcurrencyBufferSize() const;

		// return buffer size that ensure all readers can work in the same time.
		// and the pipe will never be blocked.
		size_t MinimumBufferSize() const;

		virtual void InitBufferIfParamsAreReady() = 0;
		virtual void DumpWriteToken(const size_t startIndex, const size_t endIndex) = 0;
		virtual void LoadReadToken(const size_t startIndex, const size_t endIndex) = 0;

		bool IsBufferReadyToWrite(size_t currentWriteSize);

		bool IsDataReadyToRead();
		size_t CalcReadEndPos();
		size_t CalcReadStartPos();

		std::shared_ptr<std::lock_guard<std::shared_mutex>> pauseLock;
#ifdef _DEBUG
		std::mutex seqGetWriteTokenMutex;
		std::mutex seqGetReadTokenMutex;
#endif

		std::queue<WriteTokenInfoPtr> writtingTokens;
		std::queue<ReadTokenInfoPtr> readingTokens;

		const std::string pipeName;
		std::string consumerName;
		std::string producerName;

		std::vector<size_t> elementShape;

		struct ShotInfo
		{
			ShotInfo(size_t nStartIndex)
				: StartIndex(nStartIndex), EndIndex(SIZE_MAX) {}
			const size_t StartIndex;
			size_t EndIndex;    // EndPos == SIZE_MAX indicates a shot is not finished
		};

		std::queue<ShotInfo> shotsToRead;

		// elements before this position are written
		size_t writeDonePos;

		// next token is writting from this position
		size_t writingPos;

		// elements before this position are already read,
		// if overlap == 0, these buffers are no longer used
		size_t readDonePos;

		// next token is reading from this position (if overlap == 0)
		size_t readingPos;

		std::map<std::string, std::any> extendedProperties;
	};

}
