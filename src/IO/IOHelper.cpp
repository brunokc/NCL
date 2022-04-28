
#include "IOHelper.h"
#include "IOException.h"
#include "Enum.h"

using namespace WCL::IO;

// static
wil::unique_hfile IOHelper::Open(
    const std::wstring& path,
    FileMode fileMode,
    FileAccess fileAccess,
    FileShare fileShare
    )
{
    DWORD desiredAccess = (Enum::FlagSet(fileAccess, FileAccess::Read) ? GENERIC_READ : 0) |
        (Enum::FlagSet(fileAccess, FileAccess::Write) ? GENERIC_WRITE : 0);

    return Open(
        path,
        desiredAccess,
        static_cast<DWORD>(fileShare),
        static_cast<DWORD>(fileMode),
        FILE_ATTRIBUTE_NORMAL
        );
}

// static
wil::unique_hfile IOHelper::Open(
    const std::wstring& path,
    DWORD desiredAccess,
    DWORD shareMode,
    DWORD creationDisposition,
    DWORD flagsAndAttributes
    )
{
    wil::unique_hfile file{ ::CreateFile(
        path.c_str(),
        desiredAccess,
        shareMode,
        nullptr,
        creationDisposition,
        flagsAndAttributes,
        nullptr
        ) };
    if (!file)
    {
        throw IOException("Failed to open file");
    }

    return file;
}

// static 
wil::unique_handle IOHelper::CreateFileMapping(
    HANDLE fileHandle,
    LPSECURITY_ATTRIBUTES fileMappingAttributes,
    DWORD pageProtection,
    int64_t maxSize,
    const wchar_t* name
    )
{
    const DWORD maxSizeHigh = (maxSize >> 32);
    const DWORD maxSizeLow = (maxSize & 0xffffffff);

    wil::unique_handle fileMapping{ ::CreateFileMapping(
        fileHandle,
        fileMappingAttributes,
        pageProtection,
        maxSizeHigh,
        maxSizeLow,
        name
        ) };
    if (!fileMapping)
    {
        throw IOException(HResultFromLastError(), "failed to create file mapping");
    }

    return fileMapping;
}
