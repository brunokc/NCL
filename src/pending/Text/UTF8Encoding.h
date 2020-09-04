
#pragma once

#include "Encoding.h"

namespace WFx {
namespace Text {

class UTF8Encoding :
    public Encoding
{
public:
    virtual std::vector<uint8_t> GetBytes(
        _In_ const std::wstring& str
        );

    virtual std::wstring GetString(
        _In_bcount_(byteCount) const BYTE* bytes,
        _In_ size_t byteCount
        );
};

}} // namespace WFx::Text
