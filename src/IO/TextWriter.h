#pragma once

#include <string>

namespace WCL::IO {

class TextWriter
{
public:
    virtual ~TextWriter() { }

    virtual void Close() = 0;

    virtual void Flush() = 0;

    virtual void Write(
        const std::wstring& line
        ) = 0;

    virtual void Write(
        const wchar_t* format,
        ...
        ) = 0;

    virtual void Write(
        const wchar_t* format,
        va_list args
        ) = 0;

    virtual void Write(
        const BYTE* buffer,
        int count
        ) = 0;

    virtual void WriteLine(
        const std::wstring& line
        ) = 0;

    virtual void WriteLine(
        const wchar_t* format,
        ...
        ) = 0;

    virtual void WriteLine(
        const wchar_t* format,
        va_list args
        ) = 0;
};

} // namespace WCL::IO
