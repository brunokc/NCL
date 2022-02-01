///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) Microsoft Corporation. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "EventWaitHandle.h"
#include "Win32Exception.h"
#include "Util.h"

using namespace WCL::Threading;

// EventWaitHandle::EventWaitHandle(
//     Traits::Type&& h
//     ) :
//     WaitHandle(std::move(h))
// {
// }

EventWaitHandle::EventWaitHandle(
    EventWaitHandle&& h
    ) noexcept :
    WaitHandle(std::move(h))
{
}

EventWaitHandle& EventWaitHandle::operator=(
    EventWaitHandle&& h
    ) noexcept
{
    *static_cast<WaitHandle*>(this) = std::move(h);
    return *this;
}

void
EventWaitHandle::OpenExisting(
    PCWSTR name,
    bool modifiable
    )
{
    DWORD desiredAccess = SYNCHRONIZE | STANDARD_RIGHTS_READ;
    EventWaitHandle handle;

    if (modifiable)
    {
        desiredAccess |= EVENT_MODIFY_STATE;
    }

    handle.reset(::OpenEvent(desiredAccess, FALSE, name));
    if (!handle)
    {
        throw Win32Exception(HResultFromLastError(), "Failed to open existing wait handle");
    }

    *this = std::move(handle);
}

void
EventWaitHandle::OpenExisting(
    PCWSTR name
    )
{
    OpenExisting(name, false);
}

void
EventWaitHandle::Set() const
{
    if (!is_valid())
    {
        throw Win32Exception(HRESULT_FROM_WIN32(ERROR_INVALID_HANDLE), "Invalid handle");
    }

    if (!::SetEvent(get()))
    {
        throw Win32Exception(HResultFromLastError(), "Failed to set event");
    }
}

void
EventWaitHandle::Reset() const
{
    if (!is_valid())
    {
        throw Win32Exception(HRESULT_FROM_WIN32(ERROR_INVALID_HANDLE), "Invalid handle");
    }

    if (!::ResetEvent(get()))
    {
        throw Win32Exception(HResultFromLastError(), "Failed to reset event");
    }
}

void
EventWaitHandle::Initialize(
    EventType eventType,
    EventInitialState initialState,
    _In_opt_ PCWSTR name
    )
{
    DWORD flags = 0;
    EventWaitHandle handle;

    if (eventType == EventType::Manual)
    {
        flags |= CREATE_EVENT_MANUAL_RESET;
    }

    if (initialState == EventInitialState::Signaled)
    {
        flags |= CREATE_EVENT_INITIAL_SET;
    }

    handle.reset(::CreateEventEx(
        nullptr,
        name,
        flags,
        EVENT_ALL_ACCESS
        ));
    if (!handle)
    {
        throw Win32Exception(HResultFromLastError(), "CreateEventEx failed");
    }

    *this = std::move(handle);
}
