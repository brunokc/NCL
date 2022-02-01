
#pragma once

#include <stdexcept>

#include "Encoding.h"
#include "ASCIIEncoding.h"
#include "UnicodeEncoding.h"
#include "UTF8Encoding.h"
#include "Win32Exception.h"
#include "Util.h"

using namespace WCL::Text;

const std::shared_ptr<Encoding> Encoding::ASCII = std::make_shared<ASCIIEncoding>();
const std::shared_ptr<Encoding> Encoding::Unicode = std::make_shared<UnicodeEncoding>();
const std::shared_ptr<Encoding> Encoding::UTF8 = std::make_shared<UTF8Encoding>();

//
// Unicode -> UTF8
// UTF8 -> Unicode
// Ansi -> Unicode
// Unicode -> Ansi
//

std::wstring
Encoding::MultiByteToWideString(
    uint32_t codePage,
    const std::string& str
    )
{
    return MultiByteToWideString(codePage, str.c_str(), -1);
}

std::wstring
Encoding::MultiByteToWideString(
    uint32_t codePage,
    const char* str,
    int strCount
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
    if (sizeConverted <= 0)
    {
        throw Win32Exception(HResultFromLastError(), "MultiByteToWideChar failed");
    }

    return result;
}

std::vector<uint8_t>
Encoding::WideStringToMultiByteBuffer(
    uint32_t codePage,
    const std::wstring& str
    )
{
    return WideStringToMultiByteBuffer(codePage, str.c_str(), static_cast<int>(str.size()));
}

std::vector<uint8_t>
Encoding::WideStringToMultiByteBuffer(
    uint32_t codePage,
    const wchar_t* str,
    int strCount
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
        reinterpret_cast<char*>(&result[0]),
        sizeRequired,
        nullptr,
        nullptr
        );
    if (sizeConverted <= 0)
    {
        throw Win32Exception(HResultFromLastError(), "WideCharToMultiByte failed");
    }

    return result;
}
