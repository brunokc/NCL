#pragma once

#include <string>
#include <memory>

#include <windows.h>

#include "FileSystemInfo.h"

namespace WCL::IO {

class DirectoryInfo final : public FileSystemInfo
{
public:
    DirectoryInfo(const std::wstring& path);

    virtual bool Exists() override;
    virtual std::wstring_view Name() override;
    std::shared_ptr<DirectoryInfo> Parent();
    std::shared_ptr<DirectoryInfo> Root();

    virtual void Delete() override;

private:
    void EnsureInitialized();

private:
    WIN32_FILE_ATTRIBUTE_DATA _data{ };
    bool _initialized = false;
};

}
