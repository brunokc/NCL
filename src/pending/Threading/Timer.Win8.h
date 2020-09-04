/////////////////////////////////////////////////////////////////////
//
// Copyright (C) Microsoft Corporation. All rights reserved.
//
/////////////////////////////////////////////////////////////////////

#pragma once

#ifndef MODERN_SDK
#error This header file is meant to be used only in Modern (Win8) code
#endif

#include "wrl/client.h"
#include <windows.system.threading.h>
#include "Callback.h"

//
// Added by ShipAssertFileTagger.exe
// If you copy this file, delete this section
// For full mapping list, see %sdxroot%\wectools\bin\shipAssertMappings.txt
//
#undef FILE_TAG
#define FILE_TAG 0x5CB

using namespace ::Microsoft::WRL;
using namespace ::ABI::Windows::System::Threading;


class CTimerWin8
{
protected:
    CTimerWin8(
        );

    virtual ~CTimerWin8(
        );

private:
    // Disallow copy and assignment
    CTimerWin8( const CTimerWin8& );
    CTimerWin8& operator=( const CTimerWin8& );

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
    ComPtr<ITimerElapsedHandler> m_spHandler;
    CComPtr<IThreadPoolTimer> m_spTimer;
    CComAutoCriticalSection m_csTimer;
};


template<class T>
HRESULT
CTimerWin8::InitializeTimer(
    __in T* pObject,
    __in HRESULT (CALLBACK T::*pCallback)()
    )
{
    m_spHandler = Make<Threading::Details::MemberFunctionWorkDispatcher<T>>(
        pObject,
        pCallback
        );
    if ( m_spHandler == nullptr )
    {
        return E_OUTOFMEMORY;
    }

    return S_OK;
}


template<class T, typename TArg1>
HRESULT
CTimerWin8::InitializeTimer(
    __in T* pObject,
    __in HRESULT (CALLBACK T::*pCallback)( TArg1 ),
    __in const TArg1& arg1
    )
{
    m_spHandler = Make<Threading::Details::MemberFunctionWorkDispatcher1<T, TArg1>>(
        pObject,
        pCallback,
        arg1
        );
    if ( m_spHandler == nullptr )
    {
        return E_OUTOFMEMORY;
    }

    return S_OK;
}

