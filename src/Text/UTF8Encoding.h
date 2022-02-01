
#pragma once

#include "Encoding.h"

namespace WCL::Text {

class UTF8Encoding :
    public Encoding
{
public:
    virtual std::vector<uint8_t> GetBytes(
        const std::wstring& str
        );

    virtual std::wstring GetString(
        const BYTE* bytes,
        int byteCount
        );
};

} // namespace WCL::Text
