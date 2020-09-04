
#pragma once

#include <string>
#include <wrl/wrappers/corewrappers.h>

namespace WFx {
namespace IO {

class File
{
public:
    static bool Exists(
        _In_ const std::wstring& path
        );
};

}} // namespace WFx::IO
