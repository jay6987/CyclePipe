// Description:
//   Pipe token is a token that claims a pack of elements is occupied
//   Pipe token should include information such as position, size, and 
//   whether the pack is the begining or ending of a shot.

#pragma once

#include <memory>
#include "PipeReadTokenBase.h"

namespace CyclePipe
{
	template<typename T>
	class PipeReadToken: public PipeReadTokenBase
	{
	public:
		PipeReadToken()
			: PipeReadTokenBase(nullptr)
		{}

		PipeReadToken<T>(
			std::shared_ptr<PipeReadTokenInfo> info,
			T* const* const pBufferPtrs
			)
			: PipeReadTokenBase(info)
			, pBufferPtrs(pBufferPtrs)
		{}
		
		T* const* const GetBufferPtrs() const
		{
			return pBufferPtrs;
		}

		T& GetBuffer(size_t i) const
		{
			return *pBufferPtrs[i];
		}

		T* const* const GetMutableBufferPtrs() const
		{
			return pBufferPtrs;
		}

		T& GetMutableBuffer(size_t i) const
		{
			return *pBufferPtrs[i];
		}

	private:

		T* const* pBufferPtrs;

	};
}
