
// Windows SDK
#include <windows.h>
#include <pathcch.h>

// STL
#include <string>
#include <stdexcept>

// WRL
#include <wrl/wrappers/corewrappers.h>


#ifndef E_TIMEOUT
#define E_TIMEOUT       __HRESULT_FROM_WIN32(ERROR_TIMEOUT)
#endif

inline
HRESULT
HRESULTFromLastError()
{
    return HRESULT_FROM_WIN32(::GetLastError());
}
