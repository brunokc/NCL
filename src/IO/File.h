
#pragma once

#include <string>
#include <vector>
#include <memory>

#include <Windows.h>

namespace WCL::IO {

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

    static FileAttributes GetAttributes(
        const std::wstring& path
        );

    static FILETIME GetCreationTime(
        const std::wstring& path
        );

    static FILETIME GetLastWriteTime(
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

    //static WCL::Collections::IEnumerable<File, std::wstring> ReadLines();

    static std::vector<std::wstring> ReadAllLines(
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
