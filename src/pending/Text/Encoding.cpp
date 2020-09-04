
#pragma once

#include "Encoding.h"
#include "ASCIIEncoding.h"
#include "UnicodeEncoding.h"
#include "UTF8Encoding.h"

using namespace WFx::Text;

const Property<std::shared_ptr<Encoding>> Encoding::ASCII = std::make_shared<ASCIIEncoding>();
const Property<std::shared_ptr<Encoding>> Encoding::Unicode = std::make_shared<UnicodeEncoding>();
const Property<std::shared_ptr<Encoding>> Encoding::UTF8 = std::make_shared<UTF8Encoding>();

//
// Unicode -> UTF8
// UTF8 -> Unicode
// Ansi -> Unicode
// Unicode -> Ansi
//

std::wstring
Encoding::MultiByteToWideString(
    _In_ uint32_t codePage,
    _In_ const std::string& str
    )
{
    return MultiByteToWideString(codePage, str.c_str(), -1);
}

std::wstring
Encoding::MultiByteToWideString(
    _In_ uint32_t codePage,
    _In_bcount_(strCount) PCSTR str,
    _In_ size_t strCount
    )
{
    if (strCount == 0)
    {
        throw std::invalid_argument("strCount");
    }

    int sizeRequired = ::MultiByteToWideChar(
        codePage,
        0,
        str,
        strCount,
        nullptr,
        0
        );
    if (sizeRequired <= 0)
    {
        throw Win32Exception(HResultFromLastError(), "MultiByteToWideChar failed");
    }

    // This does two things: allocates enough memory for the result and initializes with some value
    // so that the internal length of the string is set. This is important because we'll pass
    // &result[0] to a call later and we want the internal length to be set to the correct value
    std::wstring result(sizeRequired, L' ');

    int sizeConverted = ::MultiByteToWideChar(
        codePage,
        0,
        str,
        strCount,
        &result[0],
        sizeRequired
        );
    if (sizeRequired <= 0)
    {
        throw Win32Exception(HResultFromLastError(), "MultiByteToWideChar failed");
    }

    return result;
}

std::vector<uint8_t>
Encoding::WideStringToMultiByteBuffer(
    _In_ uint32_t codePage,
    _In_ const std::wstring& str
    )
{
    return WideToMultiByteString(codePage, str.c_str(), -1);
}

std::vector<uint8_t>
Encoding::WideStringToMultiByteBuffer(
    _In_ uint32_t codePage,
    _In_bcount_(strCount) PCWSTR str,
    _In_ size_t strCount
    )
{
    if (strCount == 0)
    {
        throw std::invalid_argument("strCount");
    }

    int sizeRequired = ::WideCharToMultiByte(
        codePage,
        0,
        str,
        strCount,
        nullptr,
        0,
        nullptr,
        nullptr
        );
    if (sizeRequired <= 0)
    {
        throw Win32Exception(HResultFromLastError(), "WideCharToMultiByte failed");
    }

    std::vector<uint8_t> result(sizeRequired);

    int sizeConverted = ::WideCharToMultiByte(
        codePage,
        0,
        str,
        strCount,
        &result[0],
        sizeRequired,
        nullptr,
        nullptr
        );
    if (sizeRequired <= 0)
    {
        throw Win32Exception(HResultFromLastError(), "WideCharToMultiByte failed");
    }

    return result;
}
