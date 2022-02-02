#include <windows.h>
#include <pathcch.h>

#include "Path.h"
#include "Win32Exception.h"
#include "Util.h"

using namespace WCL::IO;

const wchar_t Path::DirectorySeparatorChar = L'\\';

// static 
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

// static 
std::wstring Path::Combine(
    const std::wstring& path1,
    const std::wstring& path2
    )
{
    return Combine(path1.c_str(), path2.c_str());
}

// static
std::wstring_view Path::GetFileName(
    const std::wstring& path
    )
{
    size_t index = path.find_last_of(Path::DirectorySeparatorChar);
    if (index != std::wstring::npos && index + 1 < path.length())
    {
        return { &path[index + 1] };
    }
    else
    {
        return { path.data() };
    }
}

// static 
std::wstring Path::GetFullPath(
    const std::wstring& path
    )
{
    DWORD result = ::GetFullPathName(path.c_str(), 0, nullptr, nullptr);
    if (result == 0)
    {
        throw Win32Exception(HResultFromLastError(), "failed to get file full path");
    }

    std::wstring fullPath(result, L'\0');
    result = ::GetFullPathName(path.c_str(), result, &fullPath[0], nullptr);
    if (result == 0)
    {
        throw Win32Exception(HResultFromLastError(), "failed to get file full path");
    }

    return fullPath;
}
