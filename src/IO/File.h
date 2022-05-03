
#pragma once

#include <string>
#include <vector>
#include <memory>

#include <Windows.h>

#include <collections/IEnumerable.h>

#include "StreamReader.h"
#include "StreamWriter.h"

namespace WCL::IO {

namespace details {

class FileContentIterator :
    public WCL::Collections::IEnumerator<std::wstring>
{
public:
    FileContentIterator() = default;

    FileContentIterator(const std::wstring& path)
    {
        if (!path.empty())
        {
            _streamReader = std::make_shared<StreamReader>(path);
            _currentLine = _streamReader->ReadLine();
        }
    }

    bool operator!=(const IEnumerator<std::wstring>&) override
    {
        // TODO: this is not the proper way to check if we've reached the end() iterator, 
        // but it will work as long we assume end() points to the end of the file.
        //const auto& o = static_cast<const FileContentIterator&>(other);
        return !_streamReader->EndOfStream();
    }

    std::wstring& operator*() override
    {
        return _currentLine.value();
    }

    FileContentIterator& operator++() override
    {
        _currentLine = _streamReader->ReadLine();
        return *this;
    }

private:
    std::shared_ptr<StreamReader> _streamReader;
    std::optional<std::wstring> _currentLine;
};

class FileContentEnumerator :
    public WCL::Collections::IEnumerable<std::wstring>
{
public:
    FileContentEnumerator(const std::wstring& path) :
        _begin(path),
        _end()
    {
    }

    FileContentIterator& begin()
    {
        return _begin;
    }

    FileContentIterator& end()
    {
        return _end;
    }

private:
    FileContentIterator _begin;
    FileContentIterator _end;
};

} // namespace details

enum class FileMode 
{
    // Creates a new file, always.
    // If the specified file exist sand is writable, the function overwrites the file.
    CreateNew = 1,

    // Creates a new file, only if it does not already exist.
    // If the specified file exists, the function fails.
    Create = 2,

    // Opens a file or device, only if it exists.
    // If the specified file or device does not exist, the function fails.
    Open = 3,

    // Opens a file, always.
    // If the specified file exists, the function succeed.
    // If the specified file does not exist and is a valid path to a writable location, 
    // a file is created.
    OpenOrCreate = 4,

    // Opens a fileand truncates it so that its size is zero bytes, only if it exists.
    // If the specified file does not exist, the function fails.
    Truncate = 5,

    // (No equivalent in CreateFile(), so don't use it for now)
    Append = 6,
};

enum class FileAccess 
{
    Read = 1,
    Write = 2,
    ReadWrite = Read | Write,
};

enum class FileShare 
{
    None = 0,
    Read = 1,
    Write = 2,
    ReadWrite = Read | Write,
    Delete = 4,
    Inheritable = 16,
};

enum class FileOptions : uint32_t
{
    None = 0,
    Encrypted = 0x4000,
    DeleteOnClose = 0x4000000,
    SequentialScan = 0x8000000,
    RandomAccess = 0x10000000,
    Asynchronous = 0x40000000,
    WriteThrough = 0x80000000,
};

enum class FileAttributes 
{
    ReadOnly = 0x00000001,
    Hidden = 0x00000002,
    System = 0x00000004,  
    Directory = 0x00000010,  
    Archive = 0x00000020,  
    Device = 0x00000040,
    Normal = 0x00000080,
    Temporary = 0x00000100,
    SparseFile = 0x00000200,  
    ReparsePoint = 0x00000400,  
    Compressed = 0x00000800,  
    Offline = 0x00001000,  
    NotContentIndexed = 0x00002000,  
    Encrypted =0x00004000,  
    IntegrityStream = 0x00008000,  
    NoScrubData = 0x00020000,  
};

class FileStream;

class File final
{
public:
    static std::shared_ptr<StreamWriter> AppendText(
        const std::wstring& path
        );

    static std::shared_ptr<StreamWriter> CreateText(
        const std::wstring& path
        );

    static void Delete(
        const std::wstring& path
        );

    static bool Exists(
        const std::wstring& path
        );

    static FileAttributes GetAttributes(
        const std::wstring& path
        );

    static FILETIME GetCreationTime(
        const std::wstring& path
        );

    static FILETIME GetLastWriteTime(
        const std::wstring& path
        );

    static std::shared_ptr<FileStream> Open(
        const std::wstring& path,
        FileMode fileMode
        );

    static std::shared_ptr<FileStream> Open(
        const std::wstring& path,
        FileMode fileMode,
        FileAccess fileAccess
        );

    static std::shared_ptr<FileStream> Open(
        const std::wstring& path,
        FileMode fileMode,
        FileAccess fileAccess,
        FileShare fileShare
        );

    static std::shared_ptr<FileStream> OpenRead(
        const std::wstring& path
        );

    static std::shared_ptr<StreamReader> OpenText(
        const std::wstring& path
        );

    static std::shared_ptr<FileStream> OpenWrite(
        const std::wstring& path
        );

    static std::vector<std::wstring> ReadAllLines(
        const std::wstring& path
        );

    static details::FileContentEnumerator ReadLines(
        const std::wstring& path
        );

    static void SetAttributes(
        const std::wstring& path,
        FileAttributes attributes
        );

    static void SetCreationTime(
        const std::wstring& path,
        FILETIME ft
        );

    static void SetLastWriteTime(
        const std::wstring& path,
        FILETIME ft
        );
};

} // namespace WCL::IO
