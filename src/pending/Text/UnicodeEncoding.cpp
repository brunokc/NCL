
#pragma once

#include "UnicodeEncoding.h"

using namespace WFx::Text;

std::vector<uint8_t>
UnicodeEncoding::GetBytes(
    _In_ const std::wstring& str
    )
{
    return std::vector<uint8_t>(str.c_str());
}

std::wstring
UnicodeEncoding::GetString(
    _In_bcount_(byteCount) const BYTE* bytes,
    _In_ size_t byteCount
    )
{
    return std::wstring(static_cast<PCWSTR>(bytes), byteCount);
}
