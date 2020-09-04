
#pragma once

#include "ASCIIEncoding.h"

using namespace WFx::Text;

std::vector<uint8_t>
ASCIIEncoding::GetBytes(
    _In_ const std::wstring& str
    )
{
    return Encoding::WideStringToMultiByteBuffer(CP_ACP, str);
}

std::wstring
ASCIIEncoding::GetString(
    _In_bcount_(byteCount) const BYTE* bytes,
    _In_ size_t byteCount
    )
{
    return Encoding::MultiByteToWideString(CP_ACP, static_cast<PCSTR>(bytes), byteCount);
}
