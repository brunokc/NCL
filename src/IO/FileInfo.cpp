
#include "File.h"
#include "Path.h"
#include "IOException.h"

#include "FileInfo.h"

using namespace WCL::IO;

FileInfo::FileInfo(const std::wstring& path) :
    FileSystemInfo(path)
{
}

std::shared_ptr<DirectoryInfo> FileInfo::Directory()
{
    return std::make_shared<DirectoryInfo>(DirectoryName());
}

std::wstring FileInfo::DirectoryName()
{
    return std::wstring(Path::GetDirectoryName(_fullPath));
}

bool FileInfo::Exists()
{
    return File::Exists(_fullPath);
}

bool FileInfo::IsReadOnly()
{
    EnsureInitialized();
    return (_data.dwFileAttributes != INVALID_FILE_ATTRIBUTES &&
        (_data.dwFileAttributes & FILE_ATTRIBUTE_READONLY));
}

int64_t FileInfo::Length()
{
    EnsureInitialized();
    return (int64_t)_data.nFileSizeHigh << 32 | _data.nFileSizeLow;
}

std::wstring_view FileInfo::Name()
{
    return Path::GetFileName(_fullPath);
}

void FileInfo::Delete()
{
    File::Delete(_fullPath);
}

void FileInfo::EnsureInitialized()
{
    if (!_initialized)
    {
        BOOL success = ::GetFileAttributesEx(_fullPath.c_str(), GetFileExInfoStandard, &_data);
        if (!success)
        {
            throw IOException("unable to read file attributes");
        }

        _initialized = true;
    }
}
