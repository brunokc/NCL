#pragma once

#include <string>
#include <wil/resource.h>

namespace WCL::IO {

class IOHelper {
public:
    static wil::unique_hfile Open(
        const std::wstring& path,
        DWORD desiredAccess,
        DWORD shareMode,
        DWORD creationDisposition,
        DWORD flagsAndAttributes
        );
};

} // namespace WCL::IO
