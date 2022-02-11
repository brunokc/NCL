
#include <algorithm>
#include <stdexcept>

#include "Environment.h"
#include "text/Encoding.h"
#include "text/UTF8Encoding.h"

#include "FileStream.h"
#include "StreamReader.h"

using namespace WCL::IO;
using namespace WCL::Text;

const int BufferSize = 4 * 1024;

StreamReader::StreamReader(std::shared_ptr<Stream>& stream) :
    _stream(stream),
    _buffer(BufferSize)
{
    if (!stream->CanRead())
    {
        throw std::invalid_argument("stream not readable");
    }

    _encoding = std::make_shared<UTF8Encoding>();
}

StreamReader::StreamReader(const std::wstring& fileName) :
    _buffer(BufferSize)
{
    _stream = std::make_shared<FileStream>(
        fileName,
        FileMode::Open,
        FileAccess::Read,
        FileShare::Read,
        0,
        FileOptions::None);

    _encoding = std::make_shared<UTF8Encoding>();
}

StreamReader::~StreamReader()
{
    Close();
}

std::shared_ptr<Stream> StreamReader::BaseStream()
{
    return _stream;
}

bool StreamReader::EndOfStream()
{
    return (_index >= _dataSize && _eof);
}

void StreamReader::Close()
{
    _stream->Close();
}

int StreamReader::Peek()
{
    if (_index >= _buffer.size())
    {
        LoadBuffer();
    }

    if (_eof)
    {
        return -1;
    }

    return _buffer[_index];
}

int StreamReader::Read()
{
    int nextChar = Peek();
    if (!_eof)
    {
        _index++;
    }
    return nextChar;
}

int StreamReader::Read(
    BYTE* buffer,
    int count
    )
{
    int bytesCopied = 0;
    while (bytesCopied < count && !_eof)
    {
        // Use any remaining buffer first
        int bufferRemaining = static_cast<int>(_buffer.size()) - _index;
        if (bufferRemaining > 0)
        {
            CopyMemory(buffer, &_buffer[_index], bufferRemaining);
            bytesCopied += bufferRemaining;
        }

        LoadBuffer();
    }

    return bytesCopied;
}

std::optional<std::wstring> StreamReader::ReadLine()
{
    if (_index >= _dataSize && _eof)
    {
        return { };
    }

    // Default enconding is UTF-8
    std::vector<BYTE> newLine = _encoding->GetBytes(Environment::NewLine);

    std::wstring line;
    while (true)
    {
        if (EndOfStream())
        {
            break;
        }

        auto it = std::search(_buffer.begin() + _index, _buffer.end(), newLine.begin(), newLine.end());
        if (it != _buffer.end())
        {
            int newIndex = static_cast<int>(it - _buffer.begin());
            if (newIndex > _index)
            {
                auto encodedText = _encoding->GetString(&_buffer[_index], newIndex - _index);
                line.append(encodedText);
            }

            // Advance index to new position, right after the new line
            _index = newIndex + static_cast<int>(newLine.size());

            break;
        }
        else
        {
            if (_dataSize > 0)
            {
                line.append(&_buffer[_index], &_buffer[_dataSize - 1]);
            }
            LoadBuffer();
        }
    }

    return line;
}

std::wstring StreamReader::ReadToEnd()
{
    // Read buffer into string
    size_t streamBytesLeft = _stream->Length() - _stream->Position();
    size_t bufferBytesLeft = _buffer.size() - _index;
    size_t stringLength = streamBytesLeft + bufferBytesLeft;
    std::wstring content(stringLength, L'\0');
    content.append(_buffer.begin() + _index, _buffer.end());
    stringLength -= bufferBytesLeft;

    // Read the rest of the content into the string
    _stream->Read(reinterpret_cast<BYTE*>(content.data() + bufferBytesLeft), static_cast<int>(stringLength));

    return content;
}

void StreamReader::LoadBuffer()
{
    _index = 0;
    _dataSize = _stream->Read(_buffer.data(), static_cast<int>(_buffer.size()));
    if (_dataSize < _buffer.size())
    {
        _eof = true;
    }
}
