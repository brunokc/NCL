
#pragma once

#include <vector>
#include <memory>
#include <string>

#include <windows.h>

namespace WCL::Text {

class Encoding
{
public:
    virtual std::vector<uint8_t> GetBytes(
        const std::wstring& str
        ) = 0;

    virtual std::vector<uint8_t> GetBytes(
        const wchar_t* str
        ) = 0;

    virtual std::wstring GetString(
        const BYTE* bytes,
        int byteCount
        ) = 0;

public:
    static const std::shared_ptr<Encoding> ASCII;
    static const std::shared_ptr<Encoding> Unicode;
    static const std::shared_ptr<Encoding> UTF8;

protected:
    static
    std::wstring
    MultiByteToWideString(
        uint32_t codePage,
        const std::string& str
        );

    static
    std::wstring
    MultiByteToWideString(
        uint32_t codePage,
        const char* str,
        int strCount
        );

    static
    std::vector<uint8_t>
    WideStringToMultiByteBuffer(
        uint32_t codePage,
        const std::wstring& str
        );

    static
    std::vector<uint8_t>
    WideStringToMultiByteBuffer(
        uint32_t codePage,
        const wchar_t* str,
        int strCount
        );
};

} // namespace WCL::Text
