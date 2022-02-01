
#pragma once

#include <string>
#include <vector>
#include <memory>

#include <collections/IEnumerable.h>

namespace WCL::IO {

class FileStream;

enum class FileMode 
{
    CreateNew = 1,
    Create = 2,
    Open = 3,
    OpenOrCreate = 4,
    Truncate = 5,
    //Append = 6,
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
    DeleteOnClose  = 0x4000000,
    SequentialScan = 0x8000000,
    RandomAccess   = 0x10000000,
    Asynchronous   = 0x40000000,
    WriteThrough   = 0x80000000,
};

class File
{
public:
    static std::shared_ptr<FileStream> Create(
        const std::wstring& path
        );

    static void Delete(
        const std::wstring& path
        );

    static bool Exists(
        const std::wstring& path
        );

    //static WCL::Collections::IEnumerable<File, std::wstring> ReadLines();

    static std::vector<std::wstring> ReadAllLines(
        const std::wstring& path
        );

    static std::shared_ptr<FileStream> OpenRead(
        const std::wstring& path
        );

    static std::shared_ptr<FileStream> OpenWrite(
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
};

} // namespace WCL::IO
