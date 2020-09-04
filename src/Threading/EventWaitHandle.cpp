///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) Microsoft Corporation. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "pch.h"
#include "EventWaitHandle.h"
#include "Win32Exception.h"

using namespace WFx::Threading;

EventWaitHandle::EventWaitHandle()
{
}

// EventWaitHandle::EventWaitHandle(
//     _In_ Traits::Type&& h
//     ) :
//     WaitHandle(std::move(h))
// {
// }

EventWaitHandle::EventWaitHandle(
    _Inout_ EventWaitHandle&& h
    ) :
    WaitHandle(std::move(h))
{
}

EventWaitHandle& EventWaitHandle::operator=(
    _Inout_ EventWaitHandle&& h
    )
{
    *static_cast<WaitHandle*>(this) = std::move(h);
    return *this;
}

void
EventWaitHandle::OpenExisting(
    _In_ PCWSTR name,
    _In_ bool modifiable
    )
{
    DWORD desiredAccess = SYNCHRONIZE | STANDARD_RIGHTS_READ;
    EventWaitHandle handle;

    if (modifiable)
    {
        desiredAccess |= EVENT_MODIFY_STATE;
    }

    handle.Attach(::OpenEvent(desiredAccess, FALSE, name));
    if (!handle.IsValid())
    {
        throw Win32Exception(HRESULTFromLastError(), "Failed to open existing wait handle");
    }

    *this = std::move(handle);
}

void
EventWaitHandle::OpenExisting(
    _In_ PCWSTR name
    )
{
    OpenExisting(name, false);
}

void
EventWaitHandle::Set() const
{
    if (!IsValid())
    {
        throw Win32Exception(E_UNEXPECTED, "Invalid handle");
    }

    if (!::SetEvent(Get()))
    {
        throw Win32Exception(HRESULTFromLastError(), "Failed to set event");
    }
}

void
EventWaitHandle::Reset() const
{
    if (!IsValid())
    {
        throw Win32Exception(E_UNEXPECTED, "Invalid handle");
    }

    if (!::ResetEvent(Get()))
    {
        throw Win32Exception(HRESULTFromLastError(), "Failed to reset event");
    }
}

void
EventWaitHandle::Initialize(
    _In_ EventType eventType,
    _In_ EventInitialState initialState,
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

    handle.Attach(::CreateEventEx(
        nullptr,
        name,
        flags,
        EVENT_ALL_ACCESS
        ));
    if (!handle.IsValid())
    {
        throw Win32Exception(HRESULTFromLastError(), "CreateEventEx failed");
    }

    *this = std::move(handle);
}
