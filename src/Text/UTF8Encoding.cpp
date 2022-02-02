
#pragma once

#include "UTF8Encoding.h"

using namespace WCL::Text;

std::vector<uint8_t>
UTF8Encoding::GetBytes(
    const std::wstring& str
    )
{
    return WideStringToMultiByteBuffer(CP_UTF8, str);
}

std::vector<uint8_t>
UTF8Encoding::GetBytes(
    const wchar_t* str
    )
{
    return WideStringToMultiByteBuffer(CP_UTF8, str);
}

std::wstring
UTF8Encoding::GetString(
    const BYTE* bytes,
    int byteCount
    )
{
    return MultiByteToWideString(CP_UTF8, reinterpret_cast<const char*>(bytes), byteCount);
}
