///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) Microsoft Corporation. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "EventWaitHandle.h"

namespace WFx {
namespace Threading {

namespace Details {

    template<EventType eventType>
    class Event :
        public EventWaitHandle
    {
    public:
        explicit Event()
        {
            Initialize(eventType, EventInitialState::Unsignaled, nullptr);
        }

        explicit Event(
            _In_ PCWSTR name
            )
        {
            Initialize(eventType, EventInitialState::Unsignaled, name);
        }

        explicit Event(
            _In_ const std::wstring& name
            )
        {
            Initialize(eventType, EventInitialState::Unsignaled, name.c_str());
        }

        explicit Event(
            _In_ EventInitialState initialState,
            _In_ PCWSTR name
            )
        {
            Initialize(eventType, initialState, name);
        }

    private:
        Event(const Event&) = delete;
    };

} // namespace WFx::Threading::Details

using ManualResetEvent = Details::Event<EventType::Manual>;
using AutoResetEvent = Details::Event<EventType::Auto>;

// class ManualResetEvent :
//     public EventWaitHandle
// {
// public:
//     explicit ManualResetEvent()
//     {
//         EventWaitHandle::Initialize(EventType::Manual, EventInitialState::Unsignaled, nullptr);
//     }

//     explicit AutoResetEvent(
//         _In_ PCWSTR name
//         )
//     {
//         EventWaitHandle::Initialize(EventType::Auto, EventInitialState::Unsignaled, name);
//     }

//     explicit AutoResetEvent(
//         _In_ EventInitialState initialState,
//         _In_ PCWSTR name
//         )
//     {
//         EventWaitHandle::Initialize(EventType::Auto, initialState, name);
//     }

// private:
//     ManualResetEvent(
//         const ManualResetEvent&
//         ) = delete;
// };


// class AutoResetEvent :
//     public EventWaitHandle
// {
// public:
//     explicit AutoResetEvent()
//     {
//         EventWaitHandle::Initialize(EventType::Auto, EventInitialState::Unsignaled, nullptr);
//     }

//     explicit AutoResetEvent(
//         _In_ PCWSTR name
//         )
//     {
//         EventWaitHandle::Initialize(EventType::Auto, EventInitialState::Unsignaled, name);
//     }

//     explicit AutoResetEvent(
//         _In_ EventInitialState initialState,
//         _In_ PCWSTR name
//         )
//     {
//         EventWaitHandle::Initialize(EventType::Auto, initialState, name);
//     }

// private:
//     AutoResetEvent(
//         const AutoResetEvent&
//         ) = delete;
// };

}} // namespace WFx::Threading
