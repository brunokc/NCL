#pragma once

#include <memory>
#include <string>

#include "FileSystemInfo.h"
#include "DirectoryInfo.h"

namespace WCL::IO {

class FileInfo final : public FileSystemInfo
{
public:
    FileInfo(const std::wstring& path);

    std::shared_ptr<DirectoryInfo> Directory();
    std::wstring DirectoryName();
    virtual bool Exists() override;
    bool IsReadOnly();
    int64_t Length();
    virtual std::wstring_view Name() override;

    virtual void Delete() override;

private:
    void EnsureInitialized();

private:
    WIN32_FILE_ATTRIBUTE_DATA _data{ };
    bool _initialized = false;
};

}