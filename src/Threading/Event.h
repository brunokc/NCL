#pragma once

#include <string>

#include "EventWaitHandle.h"

namespace WCL::Threading {

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
            PCWSTR name
            )
        {
            Initialize(eventType, EventInitialState::Unsignaled, name);
        }

        explicit Event(
            const std::wstring& name
            )
        {
            Initialize(eventType, EventInitialState::Unsignaled, name.c_str());
        }

        explicit Event(
            EventInitialState initialState,
            PCWSTR name
            )
        {
            Initialize(eventType, initialState, name);
        }

        // Override private operator& in unique_event
        Event<eventType>* operator&()
        {
            return this;
        }
    private:
        Event(const Event&) = delete;
    };

} // namespace WCL::Threading::Details

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
//         PCWSTR name
//         )
//     {
//         EventWaitHandle::Initialize(EventType::Auto, EventInitialState::Unsignaled, name);
//     }

//     explicit AutoResetEvent(
//         EventInitialState initialState,
//         PCWSTR name
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
//         PCWSTR name
//         )
//     {
//         EventWaitHandle::Initialize(EventType::Auto, EventInitialState::Unsignaled, name);
//     }

//     explicit AutoResetEvent(
//         EventInitialState initialState,
//         PCWSTR name
//         )
//     {
//         EventWaitHandle::Initialize(EventType::Auto, initialState, name);
//     }

// private:
//     AutoResetEvent(
//         const AutoResetEvent&
//         ) = delete;
// };

} // namespace WCL::Threading
