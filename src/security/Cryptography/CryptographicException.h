#pragma once

#include <Win32Exception.h>

namespace WCL::Security::Cryptography {

class CryptographicException : public WCL::Win32Exception
{
public:
    explicit CryptographicException(HRESULT hr, const char* message) :
        Win32Exception(hr, message)
    {
    }
};

}
