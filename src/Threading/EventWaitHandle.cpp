#include "EventWaitHandle.h"
#include "Win32Exception.h"
#include "Util.h"

using namespace WCL::Threading;

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
    wil::EventOptions options = wil::EventOptions::None;

    if (eventType == EventType::Manual)
    {
        options |= wil::EventOptions::ManualReset;
    }

    if (initialState == EventInitialState::Signaled)
    {
        options |= wil::EventOptions::Signaled;
    }

    if (!try_create(options, name))
    {
        throw Win32Exception(HResultFromLastError(), "CreateEventEx failed");
    }
}
