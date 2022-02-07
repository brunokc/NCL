
#pragma once

#include "Exception.h"

namespace WCL {

class Win32Exception : public Exception
{
public:
    explicit Win32Exception(HRESULT hr, const char* msg) :
        Exception(msg),
        hr(hr)
    {
    }

    HRESULT hr;
};

} // namespace WCL
