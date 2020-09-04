///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) Microsoft Corporation. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <windows.h>
#include <wrl/wrappers/corewrappers.h>

namespace WFx {
namespace Threading {

//
// WaitHandle
//
// Base class for all helper classes that wrap a HANDLE. It takes care of closing the
// handle when the object is deleted. It also handles (no pun intended) generic HANDLE
// operations like copying/conversion operations, attach/detach and wait operations.
//
class WaitHandle :
    public Microsoft::WRL::Wrappers::Event
{
protected:
    explicit WaitHandle();

    // explicit WaitHandle(
    //     _In_ Traits::Type&& h
    //     );

public:
    WaitHandle(
        _Inout_ WaitHandle&& h
        );

    WaitHandle& operator=(
        _Inout_ WaitHandle&& h
        );

public:
    bool Wait(
        _In_ DWORD timeout
        ) const;

    bool Wait() const;

    //
    // WaitAll variants
    //

    template<typename... Args>
    static
    bool WaitAll(
        _In_ DWORD timeout,
        _In_ const WaitHandle& handle,
        _In_ const Args&... args
        )
    {
        const HANDLE handles[] = { handle.Get(), args.Get()... };
        int signaledHandleIndex = Wait(handles, _countof(handles), true, timeout);
        return (signaledHandleIndex >= 0);
    }

    template<typename T, typename... Args>
    static
    bool WaitAll(
        _In_ const WaitHandle& handle,
        _In_ Args&&... args
        )
    {
        return WaitAll(INFINITE, handle, std::forward<Args>(args)...);
    }

    static
    bool WaitAll(
        _In_ DWORD timeout,
        _In_reads_(handleCount) const HANDLE* handles,
        _In_ DWORD handleCount
        );

    static
    bool WaitAll(
        _In_reads_(handleCount) const HANDLE* handles,
        _In_ DWORD handleCount
        );

    //
    // WaitAny variants
    //

    template<typename... Args>
    static
    int WaitAny(
        _In_ DWORD timeout,
        _In_ const WaitHandle& handle,
        _In_ const Args&... args
        )
    {
        const HANDLE handles[] = { handle.Get(), args.Get()... };
        return Wait(handles, _countof(handles), false, timeout);
    }

    template<typename... Args>
    static
    int WaitAny(
        _In_ const WaitHandle& handle,
        _In_ Args&&... args
        )
    {
        return WaitAny(INFINITE, handle, std::forward<Args>(args)...);
    }

    static
    int WaitAny(
        _In_ DWORD timeout,
        _In_reads_(handleCount) const HANDLE* handles,
        _In_ DWORD handleCount
        );

    static
    int WaitAny(
        _In_reads_(handleCount) const HANDLE* handles,
        _In_ DWORD handleCount
        );

private:
    // Copy construction is not allowed
    WaitHandle(
        _In_ const WaitHandle&
        ) = delete;

private:
    static
    int
    WaitHandle::Wait(
        _In_reads_(handleCount) const HANDLE* pHandles,
        _In_ DWORD handleCount,
        _In_ bool waitForAll,
        _In_ DWORD timeOut
        );
};

}} // namespace WFx::Threading
