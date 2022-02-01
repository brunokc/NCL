
#pragma once

#include <stdexcept>

namespace WCL {

class Exception :
    public std::exception
{
public:
    explicit Exception(const char* msg) :
        exception(msg)
    {
    }
};

}
