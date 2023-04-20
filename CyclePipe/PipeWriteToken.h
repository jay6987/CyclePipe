// Description:
//   Pipe token is a token that claims a pack of elements is occupied
//   Pipe token should include information such as position, size, and 
//   whether the pack is the begining or ending of a shot.
#pragma once

#include <memory>
#include "PipeWriteTokenBase.h"

namespace CyclePipe
{
	template<typename T>
	class PipeWriteToken: public PipeWriteTokenBase
	{
	public:
		PipeWriteToken()
			:PipeWriteTokenBase(nullptr)
		{}

		PipeWriteToken(
			std::shared_ptr<PipeWriteTokenInfo> info,
			T* const* const pBufferPtrs)
			: PipeWriteTokenBase(info)
			, pBufferPtrs(pBufferPtrs)
		{
		}

		T* const* const GetBufferPtrs() const
		{
			return pBufferPtrs;
		}

		T& GetBuffer(size_t i) const
		{
			return *pBufferPtrs[i];
		}

	private:

		T* const*  pBufferPtrs;

	};
}