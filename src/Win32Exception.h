
#pragma once

#include <string>
#include "Exception.h"

namespace WFx {

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

} // namespace WFx
