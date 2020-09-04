
#pragma once

#include <string>

namespace WFx {
namespace IO {

class Path
{
public:
    static std::wstring Combine(
        _In_ const std::wstring& path1,
        _In_ const std::wstring& path2
        );

    static std::wstring Combine(
        _In_ PCWSTR path1,
        _In_ PCWSTR path2
        );
};

}} // namespace WFx::IO
