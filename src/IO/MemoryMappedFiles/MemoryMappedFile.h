#pragma once

#include <memory>

#include <IO/File.h>
#include <IO/FileStream.h>

#include "MemoryMappedViewAccessor.h"

using namespace WCL::IO;

namespace WCL::IO::MemoryMappedFiles {

enum class MemoryMappedFileAccess
{
    ReadWrite = 0,
    Read = 1,
    Write = 2,
    CopyOnWrite = 3,
    ReadExecute = 4,
    ReadWriteExecute = 5,
};

enum class MemoryMappedFileOptions
{
    None = 0,
    DelayAllocatePages = 0x4000000,
};

enum class HandleInheritability
{
    None = 0,
    Inheritable = 1,
};

class MemoryMappedFile
{
private:
    struct private_marker { };

public:
    MemoryMappedFile(
        const private_marker&, 
        wil::unique_handle&& handle, 
        MemoryMappedFileAccess access, 
        wil::unique_hfile&& file
        );

    //static
    //std::shared_ptr<MemoryMappedFile> Make(wil::unique_handle&& handle);

    static
    std::shared_ptr<MemoryMappedFile> CreateFromFile(
        const std::wstring& path,
        FileMode fileMode,
        const std::wstring& mapName,
        int64_t capacity,
        MemoryMappedFileAccess access
        );

    //static
    //std::shared_ptr<MemoryMappedFile> CreateFromFile(
    //    const std::shared_ptr<FileStream>& stream,
    //    const std::wstring& mapName,
    //    int64_t capacity,
    //    MemoryMappedFileAccess access,
    //    HandleInheritability inheritability,
    //    bool leaveOpen
    //    );

    static
    std::shared_ptr<MemoryMappedFile> CreateNew(
        const std::wstring& mapName,
        int64_t capacity,
        MemoryMappedFileAccess access,
        MemoryMappedFileOptions options,
        HandleInheritability inheritability
        );

    static
    std::shared_ptr<MemoryMappedFile> CreateOrOpen();

    static
    std::shared_ptr<MemoryMappedFile> OpenExisting();

    std::shared_ptr<MemoryMappedViewAccessor> CreateViewAccessor();

private:
    wil::unique_handle _fileMapping;
    MemoryMappedFileAccess _access;
    wil::unique_hfile _file;
};

}