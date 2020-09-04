///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) Microsoft Corporation. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "WaitHandle.h"

namespace WFx {
namespace Threading {

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
    explicit EventWaitHandle();

    // explicit EventWaitHandle(
    //     _In_ Traits::Type&& h
    //     );

public:
    EventWaitHandle(
        _Inout_ EventWaitHandle&& h
        );

    EventWaitHandle& operator=(
        _Inout_ EventWaitHandle&& h
        );

    void OpenExisting(
        _In_ PCWSTR name,
        _In_ bool modifiable
        );

    void OpenExisting(
        _In_ PCWSTR name
        );

    void Set() const;

    void Reset() const;

protected:
    void
    Initialize(
        _In_ EventType eventType,
        _In_ EventInitialState initialState,
        _In_opt_ PCWSTR name
        );

private:
    // Copy construction is not allowed
    EventWaitHandle(
        _In_ const EventWaitHandle&
        ) = delete;
};

}} // namespace WFx::Threading
