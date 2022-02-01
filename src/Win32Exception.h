
#pragma once

#include "Exception.h"

namespace WCL {

class Win32Exception :
    public Exception
{
public:
    explicit Win32Exception(HRESULT hr, const char* msg) :
        Exception(msg),
        _hr(hr)
    {
    }

private:
    HRESULT _hr;
};

} // namespace WCL
