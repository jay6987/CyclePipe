#include "PipeReadTokenBase.h"
#include "PipeBase.h"

namespace CyclePipe
{
	PipeReadTokenBase::PipeReadTokenBase(
		std::shared_ptr<PipeReadTokenInfo> pTokenInfo)
		: info(pTokenInfo)
	{
	}

	PipeReadTokenBase::PipeReadTokenBase(
		PipeReadTokenBase&& right) noexcept
		: info(right.info)
	{
		right.info.reset(); // to avoid calling ClearFinishedReadTokens();
	}

	PipeReadTokenBase::PipeReadTokenBase(
		const PipeReadTokenBase& right)
		: info(right.info)
	{
	}

	PipeReadTokenBase& PipeReadTokenBase::operator=(
		PipeReadTokenBase&& right) noexcept
	{
		info = right.info;
		right.info.reset(); // to avoid calling ClearFinishedReadTokens();
		return *this;
	}

	PipeReadTokenBase& PipeReadTokenBase::operator=(
		const PipeReadTokenBase& right)
	{
		info = right.info;
		return *this;
	}

	PipeReadTokenBase::~PipeReadTokenBase()
	{
		Close();
	}

	void PipeReadTokenBase::Close()
	{
		if (info.get())
		{
			PipeBase* const pPipeBase = info->Owner;
			info.reset();
			pPipeBase->ClearFinishedReadTokens();
		}
	}
}
