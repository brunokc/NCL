
#include "pch.h"
#include "Directory.h"
#include "Path.h"
#include "Win32Exception.h"

using namespace WFx::IO;

Directory::DirectoryIterator::DirectoryIterator(
    _In_ const std::wstring& path,
    _In_ IteratorType iteratorType
    ) :
    _iteratorType(iteratorType)
{
    ZeroMemory(&_findData, sizeof(_findData));

    Details::FindFileHandle handle(::FindFirstFileEx(
        path.c_str(),
        FindExInfoBasic,
        &_findData,
        FindExSearchLimitToDirectories,
        nullptr,
        FIND_FIRST_EX_LARGE_FETCH
        ));
    if (!handle.IsValid())
    {
        throw Win32Exception(HRESULTFromLastError(), "Failed to enumerate path");
    }

    _handle = std::move(handle);

    // Skip entries of the wrong type
    while (!IsEntryOfRightType())
    {
        MoveNext();
    }
}

// Special case to support the end of the enumeration (end())
Directory::DirectoryIterator::DirectoryIterator(
    _In_ std::nullptr_t,
    _In_ Directory::IteratorType iteratorType
    ) :
    _iteratorType(iteratorType)
{
    ZeroMemory(&_findData, sizeof(_findData));
}

Directory::DirectoryIterator::DirectoryIterator(
    _In_ Directory::DirectoryIterator&& other
    ) :
    _handle(std::move(other._handle))
{
}

bool Directory::DirectoryIterator::operator==(
    _In_ const Directory::DirectoryIterator& other
    )
{
    return wcscmp(_findData.cFileName, other._findData.cFileName) == 0;
}

bool Directory::DirectoryIterator::operator!=(
    _In_ const Directory::DirectoryIterator& other
    )
{
    return !(*this == other);
}

Directory Directory::DirectoryIterator::operator*()
{
    return Directory(_findData.cFileName);
}

// prefix ++ operator
Directory::DirectoryIterator& Directory::DirectoryIterator::operator++()
{
    MoveNext();
    return *this;
}

bool Directory::DirectoryIterator::IsEntryOfRightType()
{
    switch (_iteratorType)
    {
    case IteratorType::Files:
        return (_findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0;

    case IteratorType::Directories:
        return (_findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

    default:
        _ASSERTE(false);
        return false;
        break;
    }
}

void Directory::DirectoryIterator::MoveNext()
{
    do
    {
        ZeroMemory(&_findData, sizeof(_findData));
        if (!::FindNextFile(_handle.Get(), &_findData))
        {
            DWORD error = ::GetLastError();
            if (error == ERROR_NO_MORE_FILES)
            {
                // Mark the end of the enumeration somehow
                ZeroMemory(&_findData, sizeof(_findData));
                break;
            }
            else
            {
                throw Win32Exception(HRESULT_FROM_WIN32(error), "Error while numerating directories (FindNextFile)");
            }
        }
    } while (!IsEntryOfRightType());
}

Directory::DirectoryEnumerator::DirectoryEnumerator(
    _In_ std::wstring&& path
    ) :
    _path(std::move(path))
{
}

Directory::DirectoryIterator Directory::DirectoryEnumerator::begin()
{
    return DirectoryIterator(_path, IteratorType::Directories);
}

Directory::DirectoryIterator Directory::DirectoryEnumerator::end()
{
    return DirectoryIterator(nullptr, IteratorType::Directories);
}

Directory::FileEnumerator::FileEnumerator(
    _In_ std::wstring&& path
    ) :
    _path(std::move(path))
{
}

Directory::DirectoryIterator Directory::FileEnumerator::begin()
{
    return DirectoryIterator(_path, IteratorType::Files);
}

Directory::DirectoryIterator Directory::FileEnumerator::end()
{
    return DirectoryIterator(nullptr, IteratorType::Files);
}

Directory::Directory(
    _In_ const std::wstring& path
    ):
    _path(path)
{
}

bool Directory::Exists(
    _In_ const std::wstring& path
    )
{
    WIN32_FILE_ATTRIBUTE_DATA attributeData = { };
    return ::GetFileAttributesEx(path.c_str(), GetFileExInfoStandard, &attributeData) != FALSE &&
        (attributeData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
}

Directory::DirectoryEnumerator Directory::EnumerateDirectories(
    _In_ const std::wstring& path
    )
{
    auto globPath = Path::Combine(path, L"*");
    return DirectoryEnumerator(std::move(globPath));
}

Directory::FileEnumerator Directory::EnumerateFiles(
    _In_ const std::wstring& path
    )
{
    auto globPath = Path::Combine(path, L"*");
    return FileEnumerator(std::move(globPath));
}
