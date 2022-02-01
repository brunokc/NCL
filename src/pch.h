
// Windows SDK
#include <windows.h>
#include <pathcch.h>
#include <stdint.h>

// STL
#include <string>
#include <stdexcept>

// WIL
#include <wil/resource.h>

#ifndef E_TIMEOUT
#define E_TIMEOUT       __HRESULT_FROM_WIN32(ERROR_TIMEOUT)
#endif

