#include "Directory.h"
#include "Path.h"
#include "IOException.h"

#include "DirectoryInfo.h"

using namespace WCL::IO;

DirectoryInfo::DirectoryInfo(const std::wstring& path) :
    FileSystemInfo(path)
{
}

bool DirectoryInfo::Exists()
{
    return Directory::Exists(_fullPath);
}

std::wstring_view DirectoryInfo::Name()
{
    return Path::GetFileName(_fullPath);
}

std::shared_ptr<DirectoryInfo> DirectoryInfo::Parent()
{
    std::wstring path{ Path::GetDirectoryName(_fullPath) };
    return std::make_shared<DirectoryInfo>(path);
}

std::shared_ptr<DirectoryInfo> DirectoryInfo::Root()
{
    std::wstring root{ Path::GetPathRoot(_fullPath) };
    return std::make_shared<DirectoryInfo>(root);
}

void DirectoryInfo::Delete()
{
    File::Delete(_fullPath);
}

void DirectoryInfo::EnsureInitialized()
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
