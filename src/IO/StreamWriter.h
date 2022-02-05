#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Stream.h"
#include "TextWriter.h"

namespace WCL::IO {

class StreamWriter : public TextWriter
{
public:
    StreamWriter(std::shared_ptr<Stream>& stream);
    StreamWriter(const std::wstring& path);
    StreamWriter(const std::wstring& path, bool append);
    virtual ~StreamWriter();

    virtual void Close() override;

    virtual void Flush() override;

    virtual void Write(
        const std::wstring& line
        ) override;

    virtual void Write(
        const wchar_t* format,
        ...
        ) override;

    virtual void Write(
        const wchar_t* format,
        va_list args
        ) override;

    virtual void Write(
        const BYTE* buffer,
        int bufferSize
        ) override;

    virtual void WriteLine(
        const std::wstring& line
        ) override;

    virtual void WriteLine(
        const wchar_t* format,
        ...
        ) override;

    virtual void WriteLine(
        const wchar_t* format,
        va_list args
        ) override;

private:
    void FlushBuffer();

    void vWriteBuffer(const wchar_t* format, va_list args, bool appendNewLine);

    void WriteBuffer(const BYTE* buffer, int bufferSize, bool appendNewLine);

private:
    std::shared_ptr<Stream> _stream;
    std::vector<BYTE> _buffer;
    int _bufferPosition = 0;
};

} // namespace WCL::IO
