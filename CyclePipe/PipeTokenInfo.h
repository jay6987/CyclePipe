#pragma once

#include "Noncopyable.h"

namespace CyclePipe
{
	class PipeBase;

	// PipeTokenInfo should not be copy,
	// always use shared pointer to contain a PipeTokenInfo
	struct PipeTokenInfo : public Noncopyable
	{

		PipeTokenInfo(
			PipeBase* const pOwner,
			const size_t nStartPos,
			const size_t nTokenSize,
			const bool bShotStart,
			const bool bShotEnd
		)
			: Owner(pOwner)
			, StartIndex(nStartPos)
			, Size(nTokenSize)
			, IsShotStart(bShotStart)
			, IsShotEnd(bShotEnd)
		{}

		virtual ~PipeTokenInfo() {}

		const size_t StartIndex;
		const size_t Size;
		const bool IsShotStart;
		const bool IsShotEnd;
		PipeBase* const Owner;
	};
}