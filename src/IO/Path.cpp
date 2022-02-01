
#include "pch.h"
#include "Path.h"
#include "Win32Exception.h"

using namespace WCL::IO;

std::wstring Path::Combine(
    PCWSTR path1,
    PCWSTR path2
    )
{
    WCHAR combinedPath[MAX_PATH + 1] = { };
    HRESULT hr = ::PathCchCombine(combinedPath, std::size(combinedPath), path1, path2);
    if (FAILED(hr))
    {
        throw Win32Exception(hr, "Failed to combine file paths");
    }

    return combinedPath;
}

std::wstring Path::Combine(
    const std::wstring& path1,
    const std::wstring& path2
    )
{
    return Combine(path1.c_str(), path2.c_str());
}
