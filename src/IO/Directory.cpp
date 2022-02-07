
#include "Directory.h"
#include "Path.h"
#include "IOException.h"

using namespace WCL::IO;

Directory::DirectoryContentIterator::DirectoryContentIterator(
    const std::wstring& path,
    IteratorType iteratorType
    ) :
    _iteratorType(iteratorType)
{
    _findData = { };

    wil::unique_hfind handle{ ::FindFirstFileEx(
        path.c_str(),
        FindExInfoBasic,
        &_findData,
        FindExSearchLimitToDirectories,
        nullptr,
        FIND_FIRST_EX_LARGE_FETCH
        ) };
    if (!handle)
    {
        throw IOException("Failed to enumerate path");
    }

    _handle = std::move(handle);

    // Skip entries of the wrong type
    while (!IsEntryOfRightType())
    {
        MoveNext();
    }
}

// Special case to support the end of the enumeration (end())
Directory::DirectoryContentIterator::DirectoryContentIterator(
    std::nullptr_t,
    Directory::IteratorType iteratorType
    ) :
    _iteratorType(iteratorType)
{
    _findData = { };
}

Directory::DirectoryContentIterator::DirectoryContentIterator(
    Directory::DirectoryContentIterator&& other
    ) noexcept :
    _handle(std::move(other._handle))
{
}

bool Directory::DirectoryContentIterator::operator!=(
    const IEnumerator<std::wstring>& other
    )
{
    const auto& o = static_cast<const Directory::DirectoryContentIterator&>(other);
    return wcscmp(_findData.cFileName, o._findData.cFileName) != 0;
}

std::wstring& Directory::DirectoryContentIterator::operator*()
{
    _name = _findData.cFileName;
    return _name;
}

// prefix ++ operator
Directory::DirectoryContentIterator& Directory::DirectoryContentIterator::operator++()
{
    MoveNext();
    return *this;
}

bool Directory::DirectoryContentIterator::IsEntryOfRightType()
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

void Directory::DirectoryContentIterator::MoveNext()
{
    do
    {
        _findData = { };
        if (!::FindNextFile(_handle.get(), &_findData))
        {
            DWORD error = ::GetLastError();
            if (error == ERROR_NO_MORE_FILES)
            {
                // Mark the end of the enumeration somehow
                _findData = { };
                break;
            }
            else
            {
                throw IOException(HRESULT_FROM_WIN32(error), "Error while numerating directories (FindNextFile)");
            }
        }
    } while (!IsEntryOfRightType());
}

Directory::DirectoryEnumerator::DirectoryEnumerator(
    std::wstring&& path
    ) :
    _begin(path, IteratorType::Directories),
    _end(nullptr, IteratorType::Directories)
{
}

Directory::DirectoryContentIterator& Directory::DirectoryEnumerator::begin()
{
    return _begin;
}

Directory::DirectoryContentIterator& Directory::DirectoryEnumerator::end()
{
    return _end;
}

Directory::FileEnumerator::FileEnumerator(
    std::wstring&& path
    ) :
    _begin(path, IteratorType::Files),
    _end(nullptr, IteratorType::Files)
{
}

Directory::DirectoryContentIterator& Directory::FileEnumerator::begin()
{
    return _begin;
}

Directory::DirectoryContentIterator& Directory::FileEnumerator::end()
{
    return _end;
}

Directory::Directory(
    const std::wstring& path
    ):
    _path(path)
{
}

bool Directory::Exists(
    const std::wstring& path
    )
{
    WIN32_FILE_ATTRIBUTE_DATA attributeData = { };
    return ::GetFileAttributesEx(path.c_str(), GetFileExInfoStandard, &attributeData) != FALSE &&
        (attributeData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
}

Directory::DirectoryEnumerator Directory::EnumerateDirectories(
    const std::wstring& path
    )
{
    auto globPath = Path::Combine(path, L"*");
    return DirectoryEnumerator(std::move(globPath));
}

Directory::FileEnumerator Directory::EnumerateFiles(
    const std::wstring& path
    )
{
    return EnumerateFiles(path, L"*");
}

Directory::FileEnumerator Directory::EnumerateFiles(
    const std::wstring& path,
    const std::wstring& searchPattern
    )
{
    auto globPath = Path::Combine(path, searchPattern);
    return FileEnumerator(std::move(globPath));
}
