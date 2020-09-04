
#include "pch.h"
#include "Path.h"
#include "Win32Exception.h"

using namespace WFx::IO;

std::wstring Path::Combine(
    _In_ PCWSTR path1,
    _In_ PCWSTR path2
    )
{
    WCHAR combinedPath[MAX_PATH + 1] = { };
    HRESULT hr = ::PathCchCombine(combinedPath, _countof(combinedPath), path1, path2);
    if (FAILED(hr))
    {
        throw Win32Exception(hr, "Failed to combine file paths");
    }

    return combinedPath;
}

std::wstring Path::Combine(
    _In_ const std::wstring& path1,
    _In_ const std::wstring& path2
    )
{
    return Combine(path1.c_str(), path2.c_str());
}
