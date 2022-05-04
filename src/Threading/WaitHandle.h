#pragma once

#include <windows.h>
#include <wil/resource.h>

namespace WCL::Threading {

//
// WaitHandle
//
// Base class for all helper classes that wrap a HANDLE. It takes care of closing the
// handle when the object is deleted. It also handles (no pun intended) generic HANDLE
// operations like copying/conversion operations, attach/detach and wait operations.
//
class WaitHandle :
    public wil::unique_event
{
protected:
    explicit WaitHandle() = default;

public:
    WaitHandle(
        WaitHandle&& h
        ) noexcept;

    WaitHandle& operator=(
        WaitHandle&& h
        ) noexcept;

    //HANDLE Get() const;

    bool Wait(
        DWORD timeout
        ) const;

    bool Wait() const;

    //
    // WaitAll variants
    //

    template<typename... Args>
    static
    bool WaitAll(
        DWORD timeout,
        const WaitHandle& handle,
        const Args&... args
        )
    {
        const HANDLE handles[] = { handle.get(), args.get()... };
        int signaledHandleIndex = Wait(handles, _countof(handles), true, timeout);
        return (signaledHandleIndex >= 0);
    }

    template<typename T, typename... Args>
    static
    bool WaitAll(
        const WaitHandle& handle,
        Args&&... args
        )
    {
        return WaitAll(INFINITE, handle, std::forward<Args>(args)...);
    }

    static
    bool WaitAll(
        DWORD timeout,
        _In_reads_(handleCount) const HANDLE* handles,
        DWORD handleCount
        );

    static
    bool WaitAll(
        _In_reads_(handleCount) const HANDLE* handles,
        DWORD handleCount
        );

    //
    // WaitAny variants
    //

    template<typename... Args>
    static
    int WaitAny(
        DWORD timeout,
        const WaitHandle& handle,
        const Args&... args
        )
    {
        const HANDLE handles[] = { handle.get(), args.get()... };
        return Wait(handles, _countof(handles), false, timeout);
    }

    template<typename... Args>
    static
    int WaitAny(
        const WaitHandle& handle,
        Args&&... args
        )
    {
        return WaitAny(INFINITE, handle, std::forward<Args>(args)...);
    }

    static
    int WaitAny(
        DWORD timeout,
        _In_reads_(handleCount) const HANDLE* handles,
        DWORD handleCount
        );

    static
    int WaitAny(
        _In_reads_(handleCount) const HANDLE* handles,
        DWORD handleCount
        );

private:
    // Copy construction is not allowed
    WaitHandle(
        const WaitHandle&
        ) = delete;

private:
    static
    int
    Wait(
        _In_reads_(handleCount) const HANDLE* pHandles,
        DWORD handleCount,
        bool waitForAll,
        DWORD timeOut
        );
};

} // namespace WCL::Threading
