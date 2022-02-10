#pragma once

#include <optional>
#include <string>
#include <memory>
#include <vector>

#include "Stream.h"
#include "TextReader.h"

namespace WCL::IO {

class StreamReader : public TextReader
{
public:
    StreamReader(std::shared_ptr<Stream>& stream);
    StreamReader(const std::wstring& fileName);
    virtual ~StreamReader();

    std::shared_ptr<Stream> BaseStream();
    bool EndOfStream();

    virtual void Close() override;

    virtual int Peek() override;

    virtual int Read() override;
    virtual int Read(
        BYTE* buffer,
        int count
        ) override;

    virtual std::optional<std::wstring> ReadLine() override;

    virtual std::wstring ReadToEnd() override;

private:
    void LoadBuffer();

private:
    std::shared_ptr<Stream> _stream;
    std::vector<BYTE> _buffer;
    int _dataSize = 0;
    int _index = 0;
    bool _eof = false;
};

}