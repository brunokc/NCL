
#include <memory>

#include <wil/resource.h>

#include <IO/File.h>
#include <IO/IOHelper.h>
#include <IO/IOException.h>
#include <Enum.h>

#include "MemoryMappedFile.h"

using namespace WCL::IO; 
using namespace WCL::IO::MemoryMappedFiles;

inline FileAccess GetFileAccess(MemoryMappedFileAccess access)
{
    switch (access)
    {
    case MemoryMappedFileAccess::Read:
    case MemoryMappedFileAccess::ReadExecute:
        return FileAccess::Read;

    case MemoryMappedFileAccess::ReadWrite:
    case MemoryMappedFileAccess::CopyOnWrite:
    case MemoryMappedFileAccess::ReadWriteExecute:
        return FileAccess::ReadWrite;

    case MemoryMappedFileAccess::Write:
        return FileAccess::Write;

    default:
        throw std::invalid_argument("cannot map mapped file access to file access");
    }
}

inline DWORD GetPageAccess(MemoryMappedFileAccess access)
{
    switch (access)
    {
    case MemoryMappedFileAccess::Read:
        return PAGE_READONLY;
        
    case MemoryMappedFileAccess::ReadWrite:
        return PAGE_READWRITE;

    case MemoryMappedFileAccess::CopyOnWrite:
        return PAGE_WRITECOPY;

    case MemoryMappedFileAccess::ReadExecute:
        return PAGE_EXECUTE_READ;

    case MemoryMappedFileAccess::ReadWriteExecute:
        return PAGE_EXECUTE_READWRITE;

    case MemoryMappedFileAccess::Write:
        throw std::invalid_argument("write only access invalid");

    default:
        throw std::invalid_argument("unsupported access value");
    }
}

inline DWORD GetFileMapAccess(MemoryMappedFileAccess access)
{
    switch (access)
    {
    case MemoryMappedFileAccess::Read:
        return FILE_MAP_READ;

    case MemoryMappedFileAccess::Write:
        return FILE_MAP_WRITE;

    case MemoryMappedFileAccess::ReadWrite:
        return FILE_MAP_READ | FILE_MAP_WRITE;

    case MemoryMappedFileAccess::CopyOnWrite:
        return FILE_MAP_COPY;

    case MemoryMappedFileAccess::ReadExecute:
        return FILE_MAP_READ | FILE_MAP_EXECUTE;

    case MemoryMappedFileAccess::ReadWriteExecute:
        return FILE_MAP_READ | FILE_MAP_WRITE | FILE_MAP_EXECUTE;

    default:
        throw std::invalid_argument("unsupported access value");
    }
}

MemoryMappedFile::MemoryMappedFile(
    const private_marker&,
    wil::unique_handle&& handle,
    MemoryMappedFileAccess access,
    wil::unique_hfile&& file
    ) :
    _fileMapping(std::move(handle)),
    _access(access),
    _file(std::move(file))
{
}

// static
std::shared_ptr<MemoryMappedFile> MemoryMappedFile::CreateFromFile(
    const std::wstring& path,
    FileMode fileMode,
    const std::wstring& mapName,
    int64_t capacity,
    MemoryMappedFileAccess access
    )
{
    auto file = IOHelper::Open(
        path,
        fileMode,
        GetFileAccess(access),
        FileShare::None
        );

    auto fileMapping = IOHelper::CreateFileMapping(
        file.get(),
        nullptr,
        GetPageAccess(access),
        capacity,
        (mapName.empty() ? nullptr : mapName.c_str())
        );

    return std::make_shared<MemoryMappedFile>(
        private_marker{}, 
        std::move(fileMapping), 
        access,
        std::move(file)
        );
}

//// static
//std::shared_ptr<MemoryMappedFile> MemoryMappedFile::CreateFromFile(
//    const std::shared_ptr<FileStream>& stream,
//    const std::wstring& mapName,
//    int64_t capacity,
//    MemoryMappedFileAccess access,
//    HandleInheritability inheritability,
//    bool leaveOpen
//    )
//{
//    SECURITY_ATTRIBUTES secAttributes{ };
//    if (inheritability & HandleInheritability::Inheritable)
//    {
//        secAttributes.nLength = sizeof(secAttributes);
//        secAttributes.bInheritHandle = TRUE;
//    }
//
//    const DWORD capacityHigh = (capacity >> 32);
//    const DWORD capacityLow = (capacity & 0xffffffff);
//
//    auto file = IOHelper::Open(
//        L"",
//        static_cast<DWORD>(access), // desiredAccess
//        0,      // shareMode
//        0,      // createDisposition
//        FILE_ATTRIBUTE_NORMAL   // flagsAndAttributes
//        );
//
//    return nullptr;
//}

// static
std::shared_ptr<MemoryMappedFile> MemoryMappedFile::CreateNew(
    const std::wstring& mapName,
    int64_t capacity,
    MemoryMappedFileAccess access,
    MemoryMappedFileOptions options,
    HandleInheritability inheritability
    )
{
    if (options != MemoryMappedFileOptions::None &&
        options != MemoryMappedFileOptions::DelayAllocatePages)
    {
        throw std::out_of_range("options");
    }

    SECURITY_ATTRIBUTES secAttributes{ };
    if (Enum::FlagSet(inheritability, HandleInheritability::Inheritable))
    {
        secAttributes.nLength = sizeof(secAttributes);
        secAttributes.bInheritHandle = TRUE;
    }

    auto fileMapping = IOHelper::CreateFileMapping(
        INVALID_HANDLE_VALUE,
        &secAttributes,
        GetFileMapAccess(access) | static_cast<DWORD>(options),
        capacity,
        (mapName.empty() ? nullptr : mapName.c_str())
        );

    return std::make_shared<MemoryMappedFile>(
        private_marker{}, 
        std::move(fileMapping), 
        access,
        wil::unique_hfile{}
        );
}

// static
std::shared_ptr<MemoryMappedFile> MemoryMappedFile::CreateOrOpen()
{
    return nullptr;
}

// static
std::shared_ptr<MemoryMappedFile> MemoryMappedFile::OpenExisting()
{
    return nullptr;
}

std::shared_ptr<MemoryMappedViewAccessor> MemoryMappedFile::CreateViewAccessor()
{
    void* mappedMemory = ::MapViewOfFile(_fileMapping.get(), GetFileMapAccess(_access), 0, 0, 0);
    return MemoryMappedViewAccessor::Make(std::move(mappedMemory));
}
