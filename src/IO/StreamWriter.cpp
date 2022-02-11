#include <stdexcept>
#include <windows.h>

#include "Environment.h"
#include "text/Encoding.h"
#include "text/UTF8Encoding.h"

#include "File.h"
#include "FileStream.h"
#include "StreamWriter.h"

using namespace WCL;
using namespace WCL::IO;
using namespace WCL::Text;

const int BufferSize = 4 * 1024;

StreamWriter::StreamWriter(std::shared_ptr<Stream>& stream) :
    _stream(stream),
    _buffer(BufferSize)
{
    if (!stream->CanWrite())
    {
        throw std::invalid_argument("stream not writable");
    }

    _encoding = std::make_shared<UTF8Encoding>();
}

StreamWriter::StreamWriter(const std::wstring& path) :
    StreamWriter(path, false)
{
}

StreamWriter::StreamWriter(const std::wstring& path, bool append) :
    _buffer(BufferSize)
{
    _stream = std::make_shared<FileStream>(
        path,
        append ? FileMode::Append : FileMode::Create,
        FileAccess::ReadWrite,
        FileShare::Read,
        0,
        FileOptions::None);

    _encoding = std::make_shared<UTF8Encoding>();
}

StreamWriter::~StreamWriter()
{
    Close();
}

void StreamWriter::Close()
{
    FlushBuffer();
    _stream->Close();
}

void StreamWriter::Flush()
{
    FlushBuffer();
    _stream->Flush();
}

void StreamWriter::Write(
    const std::wstring& line
    )
{
    Write(line.c_str());
}

void StreamWriter::Write(
    const wchar_t* format,
    ...
    )
{
    va_list args;
    va_start(args, format);
    vWriteBuffer(format, args, false);
    va_end(args);
}

void StreamWriter::Write(
    const wchar_t* format,
    va_list args
    )
{
    vWriteBuffer(format, args, false);
}

void StreamWriter::Write(
    const BYTE* buffer,
    int bufferSize
    )
{
    WriteBuffer(buffer, bufferSize, false);
}

void StreamWriter::WriteLine(
    const std::wstring& line
    )
{
    WriteLine(line.c_str());
}

void StreamWriter::WriteLine(
    const wchar_t* format,
    ...
    )
{
    va_list args;
    va_start(args, format);
    vWriteBuffer(format, args, true);
    va_end(args);
}

void StreamWriter::WriteLine(
    const wchar_t* format,
    va_list args
    )
{
    vWriteBuffer(format, args, true);
}

void StreamWriter::FlushBuffer()
{
    if (_bufferPosition > 0)
    {
        _stream->Write(_buffer.data(), _bufferPosition);
        _bufferPosition = 0;
    }
}

void StreamWriter::vWriteBuffer(
    const wchar_t* format,
    va_list args,
    bool appendNewLine
    )
{
    wchar_t buffer[1024] = { };

    vswprintf_s(buffer, std::size(buffer), format, args);

    auto bytes = _encoding->GetBytes(buffer);
    WriteBuffer(bytes.data(), static_cast<int>(bytes.size()), appendNewLine);
}

void StreamWriter::WriteBuffer(
    const BYTE* buffer,
    int bufferSize,
    bool appendNewLine
    )
{
    while (bufferSize > 0)
    {
        if (_bufferPosition >= _buffer.size())
        {
            FlushBuffer();
        }

        int bytesToCopy = std::min(static_cast<int>(_buffer.size()) - _bufferPosition, bufferSize);
        CopyMemory(&_buffer[_bufferPosition], buffer, bytesToCopy);

        _bufferPosition += bytesToCopy;
        bufferSize -= bytesToCopy;
        buffer += bytesToCopy;
    }

    if (appendNewLine)
    {
        auto newLineBytes = _encoding->GetBytes(Environment::NewLine);
        for (int i = 0; i < newLineBytes.size(); ++i)
        {
            if (_bufferPosition >= _buffer.size())
            {
                FlushBuffer();
            }

            _buffer[_bufferPosition] = newLineBytes[i];
            _bufferPosition++;
        }
    }
}
