#include "PipeWriteTokenBase.h"
#include "PipeBase.h"

namespace CyclePipe
{
	PipeWriteTokenBase::PipeWriteTokenBase(
		std::shared_ptr<PipeWriteTokenInfo> pTokenInfo)
		: info(pTokenInfo)
	{
	}

	PipeWriteTokenBase::PipeWriteTokenBase(
		PipeWriteTokenBase&& right) noexcept
		: info(right.info)
	{
		right.info.reset(); // to avoid calling ClearFinishedWriteTokens();
	}

	PipeWriteTokenBase::PipeWriteTokenBase(
		const PipeWriteTokenBase& right)
		: info(right.info)
	{
	}

	PipeWriteTokenBase& PipeWriteTokenBase::operator=(
		PipeWriteTokenBase&& right) noexcept
	{
		info = right.info;
		right.info.reset(); // to avoid calling ClearFinishedWriteTokens();
		return *this;
	}

	PipeWriteTokenBase& PipeWriteTokenBase::operator=(
		const PipeWriteTokenBase& right)
	{
		info = right.info;
		return *this;
	}

	PipeWriteTokenBase::~PipeWriteTokenBase()
	{
		Close();
	}

	void PipeWriteTokenBase::Close()
	{
		if (info.get())
		{
			PipeBase* const pPipeBase = info->Owner;
			info.reset();
			pPipeBase->ClearFinishedWriteTokens();
		}
	}
}
