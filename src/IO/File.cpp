
#include "pch.h"
#include "File.h"
#include "Win32Exception.h"

using namespace WFx::IO;

bool File::Exists(
    _In_ const std::wstring& path
    )
{
    WIN32_FILE_ATTRIBUTE_DATA attributeData = { };
    return ::GetFileAttributesEx(path.c_str(), GetFileExInfoStandard, &attributeData) != FALSE &&
        !(attributeData.dwFileAttributes & (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_DEVICE));
}
