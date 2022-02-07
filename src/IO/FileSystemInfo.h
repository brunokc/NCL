#pragma once

#include <string>

#include "File.h"

namespace WCL::IO {

class FileSystemInfo
{
protected:
    FileSystemInfo(const std::wstring& path);

public:
    FileAttributes Attributes();
    void Attributes(FileAttributes attributes);

    FILETIME CreationTime();
    void CreationTime(const FILETIME& ft);

    virtual bool Exists() = 0;

    std::wstring_view Extension();

    virtual std::wstring_view FullName();

    FILETIME LastWriteTime();
    void LastWriteTime(const FILETIME& ft);

    virtual std::wstring_view Name() = 0;

    virtual void Delete() = 0;
    void Refresh();

protected:
    std::wstring _fullPath;
};

}
