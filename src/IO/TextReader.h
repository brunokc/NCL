#pragma once

#include <windows.h>

#include <optional>
#include <string>

namespace WCL::IO {

class TextReader 
{
public:
    virtual ~TextReader() { }

    virtual void Close() = 0;

    virtual int Peek() = 0;

    virtual int Read() = 0;
    virtual int Read(
        BYTE* buffer,
        int count
        ) = 0;

    virtual std::optional<std::wstring> ReadLine() = 0;

    virtual std::wstring ReadToEnd() = 0;
};

}