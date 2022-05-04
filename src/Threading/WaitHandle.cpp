#include "WaitHandle.h"
#include "Win32Exception.h"
#include "Util.h"

using namespace WCL::Threading;

WaitHandle::WaitHandle(
    WaitHandle&& h
    ) noexcept :
    wil::unique_event(std::move(h))
{
}

WaitHandle& WaitHandle::operator=(
    WaitHandle&& h
    ) noexcept
{
    *static_cast<wil::unique_event*>(this) = std::move(h);
    return *this;
}

//HANDLE WaitHandle::Get() const
//{
//    return get();
//}

bool WaitHandle::Wait(
    DWORD timeout
    ) const
{
    if (!is_valid())
    {
        throw Win32Exception(HRESULT_FROM_WIN32(ERROR_INVALID_HANDLE), "Invalid handle");
    }

    DWORD waitResult = ::WaitForSingleObjectEx(get(), timeout, FALSE);
    if (waitResult == WAIT_FAILED)
    {
        throw Win32Exception(HResultFromLastError(), "Wait failed");
    }
    else if (waitResult == WAIT_TIMEOUT)
    {
        return false;
    }

    return true;
}

bool WaitHandle::Wait() const
{
    return Wait(INFINITE);
}

//
// WaitAll variants
//

// static
bool WaitHandle::WaitAll(
    DWORD timeout,
    _In_reads_(handleCount) const HANDLE* handles,
    DWORD handleCount
    )
{
    int signaledHandleIndex = Wait(handles, handleCount, true, timeout);
    return (signaledHandleIndex >= 0);
}

// static
bool WaitHandle::WaitAll(
    _In_reads_(handleCount) const HANDLE* handles,
    DWORD handleCount
    )
{
    return WaitAll(INFINITE, handles, handleCount);
}

//
// WaitAny variants
//

// static
int WaitHandle::WaitAny(
    DWORD timeout,
    _In_reads_(handleCount) const HANDLE* handles,
    DWORD handleCount
    )
{
    return Wait(handles, handleCount, false, timeout);
}

// static
int WaitHandle::WaitAny(
    _In_reads_(handleCount) const HANDLE* handles,
    DWORD handleCount
    )
{
    return WaitAny(INFINITE, handles, handleCount);
}

// static
int
WaitHandle::Wait(
    _In_reads_(handleCount) const HANDLE* pHandles,
    DWORD handleCount,
    bool waitForAll,
    DWORD timeOut
    )
{
    if (pHandles == nullptr)
    {
        throw std::invalid_argument("pHandles");
    }

    if (handleCount == 0)
    {
        throw std::invalid_argument("handleCount");
    }

    DWORD waitResult = ::WaitForMultipleObjectsEx(
        handleCount,
        pHandles,
        waitForAll,
        timeOut,
        FALSE
        );

    int signaledHandleIndex = -1;
    if (waitResult == WAIT_FAILED)
    {
        throw Win32Exception(HResultFromLastError(), "Wait failed");
    }
    else if (waitResult == WAIT_TIMEOUT)
    {
        signaledHandleIndex = -1;
    }
    else
    {
        signaledHandleIndex = waitResult - WAIT_OBJECT_0;
    }

    return signaledHandleIndex;
}
