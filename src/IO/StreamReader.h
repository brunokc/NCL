#pragma once

#include <optional>
#include <string>
#include <memory>
#include <vector>

#include "Stream.h"
#include "TextReader.h"

namespace WCL::IO {

class StreamReader :
    public TextReader
{
public:
    StreamReader(std::shared_ptr<Stream>& stream);
    StreamReader(const std::wstring& fileName);

    virtual void Close();

    virtual int Peek();

    virtual int Read();
    virtual int Read(
        BYTE* buffer,
        int count
        );

    virtual std::optional<std::wstring> ReadLine();

    virtual std::wstring ReadToEnd();

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