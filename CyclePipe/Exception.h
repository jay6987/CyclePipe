#pragma once
#include <iostream>

namespace CyclePipe
{
    typedef  std::exception Exception;
    inline void ThrowExceptionAndLog(std::string&& msg) {
        std::cout<<msg;
        std::__throw_logic_error(msg.c_str());
    }
    inline void ThrowException(std::string&& msg) {
        std::__throw_logic_error(msg.c_str());
    }
}