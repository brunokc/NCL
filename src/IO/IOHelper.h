#pragma once

#include <string>
#include <wil/resource.h>

#include "File.h"

namespace WCL::IO {

class IOHelper {
public:
    static wil::unique_hfile Open(
        const std::wstring& path,
        FileMode fileMode,
        FileAccess fileAccess,
        FileShare fileShare
        );

    static wil::unique_hfile Open(
        const std::wstring& path,
        DWORD desiredAccess,
        DWORD shareMode,
        DWORD creationDisposition,
        DWORD flagsAndAttributes
        );

    static wil::unique_handle CreateFileMapping(
        HANDLE fileHandle,
        LPSECURITY_ATTRIBUTES fileMappingAttributes,
        DWORD pageProtection,
        int64_t maxSize,
        const wchar_t* name
        );
};

} // namespace WCL::IO
