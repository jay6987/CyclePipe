// Description:
//   This is a special implement of 
//   - Pipe<T>::SetDump
//   - Pipe<T>::SetLoad
//   - Pipe<T>::DumpWriteTokenImpl
//   - Pipe<T>::LoadReadTokenImpl
//   with common types

#include <vector>
#include "Pipe.h"
#include "Exception.h"


namespace CyclePipe
{
	template<>
	std::string Pipe<std::vector<float>>::GetElementType() const
	{
		return "std::vector<float>";
	}

	template<>
	std::string Pipe<std::vector<unsigned short>>::GetElementType() const
	{
		return "std::vector<unsigned short>";
	}

	template<>
	std::string Pipe<std::vector<signed short>>::GetElementType() const
	{
		return "std::vector<signed short>";
	}

	template<>
	std::string Pipe<std::vector<unsigned char>>::GetElementType() const
	{
		return "std::vector<unsigned char>";
	}

	template<>
	void Pipe<std::vector<float>>::SetDump(const std::filesystem::path& fileName)
	{
		if (pDumpFileStream.has_value())
			ThrowExceptionAndLog("Dump file is already set.");
		pDumpFileStream.emplace(fileName, std::ios::binary);
	}

	template<>
	void Pipe<std::vector<float>>::SetLoad(const std::filesystem::path& fileName)
	{
		if (pLoadFileStream.has_value())
			ThrowExceptionAndLog("Load file is already set.");
		if (!std::filesystem::exists(fileName))
			ThrowExceptionAndLog("Can not open file: " + fileName.string());
		pLoadFileStream.emplace(fileName, std::ios::binary);
	}

	template<>
	void Pipe<std::vector<float>>::DumpWriteToken(size_t nStart, const size_t nEnd)
	{
		while (nStart != nEnd)
		{
			std::vector<float>* pElement = bufferPtrs[nStart++ % bufferSize];
			pDumpFileStream->write(
				(char*)pElement->data(),
				pElement->size() * sizeof(float));
		}
	}

	template<>
	void Pipe<std::vector<float>>::LoadReadToken(size_t nStart, const size_t nEnd)
	{
		while (nStart != nEnd)
		{
			std::vector<float>* pElement = bufferPtrs[nStart++ % bufferSize];
			pLoadFileStream->read(
				(char*)pElement->data(),
				pElement->size() * sizeof(float));
		}
	}


	template<>
	void Pipe<std::vector<unsigned short>>::SetDump(const std::filesystem::path& fileName)
	{
		if (pDumpFileStream.has_value())
			ThrowException("Dump file is already set.");
		pDumpFileStream.emplace(fileName, std::ios::binary);
	}

	template<>
	void Pipe<std::vector<unsigned short>>::SetLoad(const std::filesystem::path& fileName)
	{
		if (pLoadFileStream.has_value())
			ThrowExceptionAndLog("Load file is already set.");
		if (!std::filesystem::exists(fileName))
			ThrowExceptionAndLog("Can not open file: " + fileName.string());
		pLoadFileStream.emplace(fileName, std::ios::binary);
	}

	template<>
	void Pipe<std::vector<unsigned short>>::DumpWriteToken(size_t nStart, const size_t nEnd)
	{
		while (nStart != nEnd)
		{
			std::vector<unsigned short>* pElement = bufferPtrs[nStart++ % bufferSize];
			pDumpFileStream->write(
				(char*)pElement->data(),
				pElement->size() * sizeof(unsigned short));
		}
	}

	template<>
	void Pipe<std::vector<unsigned short>>::LoadReadToken(size_t nStart, const size_t nEnd)
	{
		while (nStart != nEnd)
		{
			std::vector<unsigned short>* pElement = bufferPtrs[nStart++ % bufferSize];
			pLoadFileStream->read(
				(char*)pElement->data(),
				pElement->size() * sizeof(unsigned short));
		}
	}


	template<>
	void Pipe<std::vector<signed short>>::SetDump(const std::filesystem::path& fileName)
	{
		if (pDumpFileStream.has_value())
			ThrowExceptionAndLog("Dump file is already set.");
		pDumpFileStream.emplace(fileName, std::ios::binary);
	}

	template<>
	void Pipe<std::vector<signed short>>::SetLoad(const std::filesystem::path& fileName)
	{
		if (pLoadFileStream.has_value())
			ThrowExceptionAndLog("Load file is already set.");
		if (!std::filesystem::exists(fileName))
			ThrowExceptionAndLog("Can not open file: " + fileName.string());
		pLoadFileStream.emplace(fileName, std::ios::binary);
	}

	template<>
	void Pipe<std::vector<signed short>>::DumpWriteToken(size_t nStart, const size_t nEnd)
	{
		while (nStart != nEnd)
		{
			std::vector<signed short>* pElement = bufferPtrs[nStart++ % bufferSize];
			pDumpFileStream->write(
				(char*)pElement->data(),
				pElement->size() * sizeof(signed short));
		}
	}

	template<>
	void Pipe<std::vector<signed short>>::LoadReadToken(size_t nStart, const size_t nEnd)
	{
		while (nStart != nEnd)
		{
			std::vector<signed short>* pElement = bufferPtrs[nStart++ % bufferSize];
			pLoadFileStream->read(
				(char*)pElement->data(),
				pElement->size() * sizeof(signed short));
		}
	}

	template<>
	void Pipe<std::vector<unsigned char>>::SetDump(const std::filesystem::path& fileName)
	{
		if (pDumpFileStream.has_value())
			ThrowExceptionAndLog("Dump file is already set.");
		pDumpFileStream.emplace(fileName, std::ios::binary);
	}

	template<>
	void Pipe<std::vector<unsigned char>>::SetLoad(const std::filesystem::path& fileName)
	{
		if (pLoadFileStream.has_value())
			ThrowExceptionAndLog("Load file is already set.");
		if (!std::filesystem::exists(fileName))
			ThrowExceptionAndLog("Can not open file: " + fileName.string());
		pLoadFileStream.emplace(fileName, std::ios::binary);
	}

	template<>
	void Pipe<std::vector<unsigned char>>::DumpWriteToken(size_t nStart, const size_t nEnd)
	{
		while (nStart != nEnd)
		{
			std::vector<unsigned char>* pElement = bufferPtrs[nStart++ % bufferSize];
			pDumpFileStream->write(
				(char*)pElement->data(),
				pElement->size() * sizeof(unsigned char));
		}
	}

	template<>
	void Pipe<std::vector<unsigned char>>::LoadReadToken(size_t nStart, const size_t nEnd)
	{
		while (nStart != nEnd)
		{
			std::vector<unsigned char>* pElement = bufferPtrs[nStart++ % bufferSize];
			pLoadFileStream->read(
				(char*)pElement->data(),
				pElement->size() * sizeof(unsigned char));
		}
	}


	template<>
	std::string Pipe<size_t>::GetElementType() const
	{
		return "size_t";
	}

	template<>
	void Pipe<size_t>::SetDump(const std::filesystem::path& /*fileName*/)
	{
	}

	template<>
	void Pipe<size_t>::SetLoad(const std::filesystem::path& /*fileName*/)
	{
	}

	template<>
	void Pipe<size_t>::DumpWriteToken(const size_t /*nStart*/, const size_t /*nEnd*/)
	{
	}

	template<>
	void Pipe<size_t>::LoadReadToken(const size_t /*nStart*/, const size_t /*nEnd*/)
	{
	}
}