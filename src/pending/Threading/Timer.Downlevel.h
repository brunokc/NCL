/////////////////////////////////////////////////////////////////////
//
// Copyright (C) Microsoft Corporation. All rights reserved.
//
/////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>
#include "Callback.h"

//
// Added by ShipAssertFileTagger.exe
// If you copy this file, delete this section
// For full mapping list, see %sdxroot%\wectools\bin\shipAssertMappings.txt
//
#undef FILE_TAG
#define FILE_TAG 0x5C9

#ifdef MODERN_SDK
#error This header file is meant to be used only in downlevel (pre-Win8) code
#endif


class CTimerDownlevel
{
protected:
    CTimerDownlevel(
        );

    virtual ~CTimerDownlevel(
        );

private:
    // Disallow copy and assignment
    CTimerDownlevel( const CTimerDownlevel& );
    CTimerDownlevel& operator=( const CTimerDownlevel& );

protected:
    HRESULT
    InitializeTimer(
        __in HRESULT (CALLBACK *pCallback)()
        );

    template<class T>
    HRESULT
    InitializeTimer(
        __in T* pObject,
        __in HRESULT (CALLBACK T::*pCallback)()
        );

    template<class T, typename TArg1>
    HRESULT
    InitializeTimer(
        __in T* pObject,
        __in HRESULT (CALLBACK T::*pCallback)( TArg1 ),
        __in const TArg1& arg1
        );

    HRESULT
    CreateTimer(
        __in DWORD dwDueTime,
        __in bool fPeriodic
        );

    bool
    IsTimerCallbackValid(
        );

    HRESULT
    CancelTimer(
        );

private:
    static
    VOID
    CALLBACK
    TimerProc(
        __in PVOID pParameter,
        __in BOOLEAN TimerOrWaitFired
        );

private:
    HANDLE m_hTimer;
    std::unique_ptr<Threading::Details::WorkDispatcherBase> m_pDispatcher;
    CComAutoCriticalSection m_csTimer;
};

template<class T>
HRESULT
CTimerDownlevel::InitializeTimer(
    __in T* pObject,
    __in HRESULT (CALLBACK T::*pCallback)()
    )
{
    m_pDispatcher.reset(
        new (std::nothrow) Threading::Details::MemberFunctionWorkDispatcher<T>(
            pObject,
            pCallback
            ) );
    if ( m_pDispatcher == nullptr )
    {
        return E_OUTOFMEMORY;
    }
    return S_OK;
}


template<class T, typename TArg1>
HRESULT
CTimerDownlevel::InitializeTimer(
    __in T* pObject,
    __in HRESULT (CALLBACK T::*pCallback)( TArg1 ),
    __in const TArg1& arg1
    )
{
    m_pDispatcher.reset(
        new (std::nothrow) Threading::Details::MemberFunctionWorkDispatcher1<T, TArg1>(
            pObject,
            pCallback,
            arg1
            ) );
    if ( m_pDispatcher == nullptr )
    {
        return E_OUTOFMEMORY;
    }
    return S_OK;
}

