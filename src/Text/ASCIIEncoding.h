
#pragma once

#include "Encoding.h"

namespace WCL::Text {

class ASCIIEncoding :
    public Encoding
{
public:
    virtual std::vector<uint8_t> GetBytes(
        const std::wstring& str
        ) override;

    virtual std::vector<uint8_t> GetBytes(
        const wchar_t* str
        ) override;

    virtual std::wstring GetString(
        const BYTE* bytes,
        int byteCount
        ) override;
};

} // namespace WCL::Text
