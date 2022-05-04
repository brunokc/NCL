#pragma once

#include "WaitHandle.h"

namespace WCL::Threading {

//
// Events
//

enum class EventType
{
    Manual,
    Auto
};

enum class EventInitialState
{
    Signaled,
    Unsignaled
};

class EventWaitHandle :
    public WaitHandle
{
protected:
    explicit EventWaitHandle() = default;

public:
    EventWaitHandle(
        EventWaitHandle&& h
        ) noexcept;

    EventWaitHandle& operator=(
        EventWaitHandle&& h
        ) noexcept;

    void OpenExisting(
        PCWSTR name,
        bool modifiable
        );

    void OpenExisting(
        PCWSTR name
        );

    void Set() const;

    void Reset() const;

protected:
    void
    Initialize(
        EventType eventType,
        EventInitialState initialState,
        _In_opt_ PCWSTR name
        );

private:
    // Copy construction is not allowed
    EventWaitHandle(
        const EventWaitHandle&
        ) = delete;
};

} // namespace WCL::Threading
