
#include "IOHelper.h"
#include "IOException.h"

using namespace WCL::IO;

// static
wil::unique_hfile IOHelper::Open(
    const std::wstring& path,
    DWORD desiredAccess,
    DWORD shareMode,
    DWORD creationDisposition,
    DWORD flagsAndAttributes
)
{
    wil::unique_hfile file{ ::CreateFile(
        path.c_str(),
        desiredAccess,
        shareMode,
        nullptr,
        creationDisposition,
        flagsAndAttributes,
        nullptr
        ) };
    if (!file)
    {
        throw IOException("Failed to open file");
    }

    return file;
}
