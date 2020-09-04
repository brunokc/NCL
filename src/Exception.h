
#pragma once

namespace WFx {

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
