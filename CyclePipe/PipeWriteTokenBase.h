#pragma once

#include <memory>
#include "PipeWriteTokenInfo.h"
#include "Exception.h"

namespace CyclePipe
{
	class PipeWriteTokenBase
	{
	public:

		PipeWriteTokenBase() = delete;

		PipeWriteTokenBase(std::shared_ptr<PipeWriteTokenInfo> pTokenInfo);

		PipeWriteTokenBase(PipeWriteTokenBase&& right) noexcept;

		PipeWriteTokenBase(const PipeWriteTokenBase& right);

		PipeWriteTokenBase& operator = (PipeWriteTokenBase&& right) noexcept;

		PipeWriteTokenBase& operator = (const PipeWriteTokenBase& right);

		virtual ~PipeWriteTokenBase();

		size_t GetStartIndex() const { return info->StartIndex; }

		size_t GetSize() const { return info->Size; }

		bool IsShotStart() const { return info->IsShotStart; }

		bool IsShotEnd() const { return info->IsShotEnd; }

		void Close();

	private:

		std::shared_ptr<const PipeWriteTokenInfo> info;

	};

}