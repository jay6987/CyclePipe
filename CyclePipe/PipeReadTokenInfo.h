// Description:
//   Pipe token is a token that claims a pack of elements is occupied
//   Pipe token should include information such as position, size, and 
//   whether the pack is the begining or ending of a shot.
//   This is a base class of PipeReadToekn, which contains all informations
//   about a PipeReadToken except the type-related pointer to buffer

#pragma once

#include "PipeTokenInfo.h"

namespace CyclePipe
{
	struct PipeReadTokenInfo : public PipeTokenInfo
	{
		PipeReadTokenInfo(
			PipeBase* const pOwner,
			const size_t nStartPos,
			const size_t nTokenSize,
			const size_t nOverlapSize,
			const bool bShotStart,
			const bool bShotEnd
		)
			: PipeTokenInfo(
				pOwner,
				nStartPos,
				nTokenSize,
				bShotStart,
				bShotEnd)
			, OverlapSize(nOverlapSize)
		{}

		const size_t OverlapSize;
	};
}