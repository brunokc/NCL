
#pragma once

#include <string>

namespace WCL::IO {

class Path
{
public:
    const static wchar_t DirectorySeparatorChar;

    static std::wstring Combine(
        const std::wstring& path1,
        const std::wstring& path2
        );

    static std::wstring Combine(
        PCWSTR path1,
        PCWSTR path2
        );

    static std::wstring_view GetFileName(
        const std::wstring& path
        );

    static std::wstring GetFullPath(
        const std::wstring& path
        );
};

} // namespace WCL::IO
