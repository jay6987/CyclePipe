// Description:
//   Pipe token is a token that claims a pack of elements is occupied
//   Pipe token should include information such as position, size, and 
//   whether the pack is the begining or ending of a shot.
//   This is a base class of PipeWriteToken, which contains all informations
//   about a PipeReadToken except the type-related pointer to buffer

#pragma once

#include "PipeTokenInfo.h"

namespace CyclePipe
{
	typedef PipeTokenInfo PipeWriteTokenInfo;
}