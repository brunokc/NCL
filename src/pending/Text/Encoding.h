
#pragma once

#include "Property.h"

using namespace WFx::Base;

namespace WFx {
namespace Text {

class Encoding
{
public:
    virtual std::vector<uint8_t> GetBytes(
        _In_ const std::wstring& str
        ) = 0;

    virtual std::wstring GetString(
        _In_bcount_(byteCount) const BYTE* bytes,
        _In_ size_t byteCount
        ) = 0;

public:
    static const Property<std::shared_ptr<Encoding>> ASCII;
    static const Property<std::shared_ptr<Encoding>> Unicode;
    static const Property<std::shared_ptr<Encoding>> UTF8;

protected:
    static
    std::wstring
    MultiByteToWideString(
        _In_ uint32_t codePage,
        _In_ const std::string& str
        );

    static
    std::wstring
    MultiByteToWideString(
        _In_ uint32_t codePage,
        _In_bcount_(strCount) PCSTR str,
        _In_ size_t strCount
        );

    static
    std::vector<uint8_t>
    WideStringToMultiByteBuffer(
        _In_ uint32_t codePage,
        _In_ const std::wstring& str
        );

    static
    std::vector<uint8_t>
    WideStringToMultiByteBuffer(
        _In_ uint32_t codePage,
        _In_bcount_(strCount) PCWSTR str,
        _In_ size_t strCount
        );
};

}} // namespace WFx::Text
