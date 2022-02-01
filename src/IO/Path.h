
#pragma once

#include <string>

namespace WCL::IO {

class Path
{
public:
    static std::wstring Combine(
        const std::wstring& path1,
        const std::wstring& path2
        );

    static std::wstring Combine(
        PCWSTR path1,
        PCWSTR path2
        );
};

} // namespace WCL::IO
