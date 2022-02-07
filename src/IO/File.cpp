
#include "pch.h"
#include "File.h"
#include "FileStream.h"
#include "StreamReader.h"
#include "StreamWriter.h"
#include "IOException.h"
#include "IOHelper.h"

using namespace WCL::IO;

// static 
std::shared_ptr<StreamWriter> File::AppendText(
    const std::wstring& path
    )
{
    return std::make_shared<StreamWriter>(path, /* append */ true);
}

// static 
std::shared_ptr<StreamWriter> File::CreateText(
    const std::wstring& path
)
{
    return std::make_shared<StreamWriter>(path, /* append */ false);
}

// static 
void File::Delete(
    const std::wstring& path
    )
{
    if (!::DeleteFile(path.c_str()))
    {
        throw IOException("unable to delete file");
    }
}

// static 
bool File::Exists(
    const std::wstring& path
    )
{
    WIN32_FILE_ATTRIBUTE_DATA attributeData{ };
    return ::GetFileAttributesEx(path.c_str(), GetFileExInfoStandard, &attributeData) != FALSE &&
        !(attributeData.dwFileAttributes & (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_DEVICE));
}

// static 
FileAttributes File::GetAttributes(
    const std::wstring& path
    )
{
    DWORD attributes = ::GetFileAttributes(path.c_str());
    if (attributes == INVALID_FILE_ATTRIBUTES)
    {
        throw IOException("failed to set attributes");
    }

    return static_cast<FileAttributes>(attributes);
}

// static 
FILETIME File::GetCreationTime(
    const std::wstring& path
    )
{
    wil::unique_hfile file = IOHelper::Open(
        path,
        GENERIC_READ,
        0,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL
        );

    FILETIME ft{ };
    BOOL success = ::GetFileTime(file.get(), &ft, nullptr, nullptr);
    if (!success)
    {
        throw IOException("failed to get creation time");
    }

    return ft;
}

// static 
FILETIME File::GetLastWriteTime(
    const std::wstring& path
    )
{
    wil::unique_hfile file = IOHelper::Open(
        path,
        GENERIC_READ,
        0,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL
        );

    FILETIME ft{ };
    BOOL success = ::GetFileTime(file.get(), nullptr, nullptr, &ft);
    if (!success)
    {
        throw IOException("failed to get creation time");
    }

    return ft;
}

// static
std::shared_ptr<FileStream> File::Open(
    const std::wstring& path,
    FileMode fileMode
    )
{
    return Open(
        path,
        fileMode,
        FileAccess::ReadWrite,
        FileShare::None
        );
}

// static
std::shared_ptr<FileStream> File::Open(
    const std::wstring& path,
    FileMode fileMode,
    FileAccess fileAccess
    )
{
    return Open(
        path,
        fileMode,
        fileAccess,
        FileShare::None
        );
}

// static
std::shared_ptr<FileStream> File::Open(
    const std::wstring& path,
    FileMode fileMode,
    FileAccess fileAccess,
    FileShare fileShare
    )
{
    auto handle = IOHelper::Open(
        path,
        static_cast<DWORD>(fileAccess),
        static_cast<DWORD>(fileShare),
        static_cast<DWORD>(fileMode),
        FILE_ATTRIBUTE_NORMAL
        );

    return FileStream::Make(std::move(handle));
}

// static 
std::shared_ptr<FileStream> File::OpenRead(
    const std::wstring& path
)
{
    return Open(
        path,
        FileMode::Open,
        FileAccess::Read,
        FileShare::Read
    );
}

// static 
std::shared_ptr<StreamReader> File::OpenText(
    const std::wstring& path
)
{
    return std::make_shared<StreamReader>(path);
}

// static 
std::shared_ptr<FileStream> File::OpenWrite(
    const std::wstring& path
)
{
    return Open(
        path,
        FileMode::OpenOrCreate,
        FileAccess::Write,
        FileShare::None
    );
}

// static 
std::vector<std::wstring> File::ReadAllLines(
    const std::wstring& path
    )
{
    std::vector<std::wstring> lines;

    auto streamReader = std::make_shared<StreamReader>(path);
    std::optional<std::wstring> line;
    while ((line = streamReader->ReadLine()).has_value())
    {
        lines.push_back(line.value());
    }

    return lines;    
}

// static 
void File::SetAttributes(
    const std::wstring& path,
    FileAttributes attributes
    )
{
    BOOL success = ::SetFileAttributes(
        path.c_str(),
        static_cast<DWORD>(attributes)
        );
    if (!success)
    {
        throw IOException("failed to set attributes");
    }
}

// static 
void File::SetCreationTime(
    const std::wstring& path,
    FILETIME ft
    )
{
    wil::unique_hfile file = IOHelper::Open(
        path,
        FILE_WRITE_ATTRIBUTES,
        0,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL
        );
    BOOL success = ::SetFileTime(file.get(), &ft, nullptr, nullptr);
    if (!success)
    {
        throw IOException("failed to set creation time");
    }
}

// static 
void File::SetLastWriteTime(
    const std::wstring& path,
    FILETIME ft
    )
{
    wil::unique_hfile file = IOHelper::Open(
        path,
        FILE_WRITE_ATTRIBUTES,
        0,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL
        );
    BOOL success = ::SetFileTime(file.get(), nullptr, nullptr, &ft);
    if (!success)
    {
        throw IOException("failed to set creation time");
    }
}
