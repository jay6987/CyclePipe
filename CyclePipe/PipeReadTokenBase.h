#pragma once

#include <memory>
#include "PipeReadTokenInfo.h"
#include "Exception.h"

namespace CyclePipe
{
	class PipeReadTokenBase
	{
	public:

		PipeReadTokenBase() = delete;

		PipeReadTokenBase(std::shared_ptr<PipeReadTokenInfo> pTokenInfo);

		PipeReadTokenBase(PipeReadTokenBase&& right) noexcept;

		PipeReadTokenBase(const PipeReadTokenBase& right);

		PipeReadTokenBase& operator = (PipeReadTokenBase&& right) noexcept;

		PipeReadTokenBase& operator = (const PipeReadTokenBase& right);

		virtual ~PipeReadTokenBase();

		size_t GetStartIndex() const { return info->StartIndex; }

		size_t GetSize() const { return info->Size; }

		size_t GetOverlapSize() const { return info->OverlapSize; }

		bool IsShotStart() const { return info->IsShotStart; }

		bool IsShotEnd() const { return info->IsShotEnd; }

		void Close();

	private:

		std::shared_ptr<const PipeReadTokenInfo> info;

	};

}