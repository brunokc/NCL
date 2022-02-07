
#include "File.h"
#include "Path.h"
#include "FileSystemInfo.h"

using namespace WCL::IO;

FileSystemInfo::FileSystemInfo(const std::wstring& path)
{
    _fullPath = Path::GetFullPath(path);
}

FileAttributes FileSystemInfo::Attributes()
{
    return File::GetAttributes(_fullPath);
}

void FileSystemInfo::Attributes(FileAttributes attributes)
{
    File::SetAttributes(_fullPath, attributes);
}

FILETIME FileSystemInfo::CreationTime()
{
    return File::GetCreationTime(_fullPath);
}

void FileSystemInfo::CreationTime(const FILETIME& ft)
{
    File::SetCreationTime(_fullPath, ft);
}

std::wstring_view FileSystemInfo::Extension()
{
    return Path::GetExtension(_fullPath);
}

std::wstring_view FileSystemInfo::FullName()
{
    return _fullPath;
}

FILETIME FileSystemInfo::LastWriteTime()
{
    return File::GetLastWriteTime(_fullPath);
}

void FileSystemInfo::LastWriteTime(const FILETIME& ft)
{
    File::SetLastWriteTime(_fullPath, ft);
}

void FileSystemInfo::Refresh()
{
    throw std::exception("FileSystemInfo::Refresh() called");
}
