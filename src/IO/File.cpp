
#include "pch.h"
#include "File.h"
#include "FileStream.h"
#include "StreamReader.h"
#include "IOException.h"
#include "IOHelper.h"

using namespace WCL::IO;

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
std::shared_ptr<FileStream> File::OpenWrite(
    const std::wstring& path
    )
{
    return Open(
        path,
        FileMode::OpenOrCreate,
        FileAccess::ReadWrite,
        FileShare::None
        );
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
