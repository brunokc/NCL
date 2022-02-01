
#pragma once

#include "ASCIIEncoding.h"

using namespace WCL::Text;

std::vector<uint8_t>
ASCIIEncoding::GetBytes(
    const std::wstring& str
    )
{
    return WideStringToMultiByteBuffer(CP_ACP, str);
}

std::wstring
ASCIIEncoding::GetString(
    const BYTE* bytes,
    int byteCount
    )
{
    return MultiByteToWideString(CP_ACP, reinterpret_cast<const char*>(bytes), byteCount);
}
