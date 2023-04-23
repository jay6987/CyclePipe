#pragma once
#include <iostream>

namespace CyclePipe
{
    typedef  std::exception Exception;
    inline void ThrowExceptionAndLog(std::string&& msg) {
        std::cout<<msg;
        throw std::logic_error(msg.c_str());
    }
    inline void ThrowException(std::string&& msg) {
        throw std::logic_error(msg.c_str());
    }
}