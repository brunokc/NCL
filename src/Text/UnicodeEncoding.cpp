
#pragma once

#include <vector>

#include "UnicodeEncoding.h"

using namespace WCL::Text;

std::vector<uint8_t>
UnicodeEncoding::GetBytes(
    const std::wstring& str
    )
{
    const uint8_t* p = reinterpret_cast<const uint8_t*>(str.data());
    size_t strBytes = str.size() * sizeof(str[0]);
    return { p, p + strBytes };
}

std::wstring
UnicodeEncoding::GetString(
    const BYTE* bytes,
    int byteCount
    )
{
    return std::wstring(reinterpret_cast<const wchar_t*>(bytes), byteCount);
}
