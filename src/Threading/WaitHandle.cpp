///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) Microsoft Corporation. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "pch.h"
#include "WaitHandle.h"
#include "Win32Exception.h"

using namespace WFx::Threading;

WaitHandle::WaitHandle()
{
}

// WaitHandle::WaitHandle(
//     _In_ Traits::Type&& h
//     ) :
//     Event(std::move(h))
// {
// }

WaitHandle::WaitHandle(
    _Inout_ WaitHandle&& h
    ) :
    Event(std::move(h))
{
}

WaitHandle& WaitHandle::operator=(
    _Inout_ WaitHandle&& h
    )
{
    *static_cast<Event*>(this) = std::move(h);
    return *this;
}

bool WaitHandle::Wait(
    _In_ DWORD timeout
    ) const
{
    if (!IsValid())
    {
        throw Win32Exception(E_UNEXPECTED, "Invalid handle");
    }

    DWORD waitResult = ::WaitForSingleObjectEx(Get(), timeout, FALSE);
    if (waitResult == WAIT_FAILED)
    {
        throw Win32Exception(HRESULTFromLastError(), "Wait failed");
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
    _In_ DWORD timeout,
    _In_reads_(handleCount) const HANDLE* handles,
    _In_ DWORD handleCount
    )
{
    int signaledHandleIndex = Wait(handles, handleCount, true, timeout);
    return (signaledHandleIndex >= 0);
}

// static
bool WaitHandle::WaitAll(
    _In_reads_(handleCount) const HANDLE* handles,
    _In_ DWORD handleCount
    )
{
    return WaitAll(INFINITE, handles, handleCount);
}

//
// WaitAny variants
//

// static
int WaitHandle::WaitAny(
    _In_ DWORD timeout,
    _In_reads_(handleCount) const HANDLE* handles,
    _In_ DWORD handleCount
    )
{
    return Wait(handles, handleCount, false, timeout);
}

// static
int WaitHandle::WaitAny(
    _In_reads_(handleCount) const HANDLE* handles,
    _In_ DWORD handleCount
    )
{
    return WaitAny(INFINITE, handles, handleCount);
}

// static
int
WaitHandle::Wait(
    _In_reads_(handleCount) const HANDLE* pHandles,
    _In_ DWORD handleCount,
    _In_ bool waitForAll,
    _In_ DWORD timeOut
    )
{
    if (pHandles == nullptr || handleCount == 0)
    {
        throw std::invalid_argument("pHandles");
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
        throw Win32Exception(HRESULTFromLastError(), "Wait failed");
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
