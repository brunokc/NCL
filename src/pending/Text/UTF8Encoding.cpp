
#pragma once

#include "UTF8Encoding.h"

using namespace WFx::Text;

std::vector<uint8_t>
UTF8Encoding::GetBytes(
    _In_ const std::wstring& str
    )
{
    return Encoding::WideStringToMultiByteBuffer(CP_UTF8, str);
}

std::wstring
UTF8Encoding::GetString(
    _In_bcount_(byteCount) const BYTE* bytes,
    _In_ size_t byteCount
    )
{
    return Encoding::MultiByteToWideString(CP_UTF8, static_cast<PCSTR>(bytes), byteCount);
}
