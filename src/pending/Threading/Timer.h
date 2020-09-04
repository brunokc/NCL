/////////////////////////////////////////////////////////////////////
//
// Copyright (C) Microsoft Corporation. All rights reserved.
//
/////////////////////////////////////////////////////////////////////

#pragma once

#ifdef MODERN_SDK
#include "Timer.Win8.h"
#else
#include "Timer.Downlevel.h"

//
// Added by ShipAssertFileTagger.exe
// If you copy this file, delete this section
// For full mapping list, see %sdxroot%\wectools\bin\shipAssertMappings.txt
//
#undef FILE_TAG
#define FILE_TAG 0x5CA
#endif

template<class T>
class CTimerT;

#ifdef MODERN_SDK
typedef CTimerT<CTimerWin8>      CTimer;
#else
typedef CTimerT<CTimerDownlevel> CTimer;
#endif


template<class T>
class CTimerT :
    public T
{
public:
    CTimerT(
        )
    {
    }

    virtual ~CTimerT(
        )
    {
    }

    HRESULT
    Initialize(
        __in HRESULT (CALLBACK *pCallback)()
        )
    {
        if ( pCallback == nullptr )
        {
            return E_POINTER;
        }
        return InitializeTimer( pCallback );
    }

    template<class TObj>
    HRESULT
    Initialize(
        __in TObj* pObject,
        __in HRESULT (CALLBACK TObj::*pCallback)()
        )
    {
        if ( pObject == nullptr || pCallback == nullptr )
        {
            return E_POINTER;
        }

        return InitializeTimer( pObject, pCallback );
    }

    template<class TObj, typename TArg1>
    HRESULT
    Initialize(
        __in TObj* pObject,
        __in HRESULT (CALLBACK TObj::*pCallback)( TArg1 ),
        __in const TArg1& arg1
        )
    {
        if ( pObject == nullptr || pCallback == nullptr )
        {
            return E_POINTER;
        }

        return InitializeTimer( pObject, pCallback, arg1 );
    }

    HRESULT
    Schedule(
        __in DWORD dwDueTime
        )
    {
        return ScheduleTimer( dwDueTime, false );
    }

    HRESULT
    SchedulePeriodic(
        __in DWORD dwPeriod
        )
    {
        return ScheduleTimer( dwPeriod, true );
    }

    HRESULT
    Cancel(
        )
    {
        return CancelTimer();
    }

private:
    // Disallow copy and assignment
    CTimerT( const CTimerT& );
    CTimerT& operator=( const CTimerT& );

    HRESULT
    ScheduleTimer(
        __in DWORD dwDueTime,
        __in bool fPeriodic
        )
    {
        HRESULT hr = S_OK;

        CHECKEXP_EXIT( IsTimerCallbackValid(), HRESULT_FROM_WIN32( ERROR_NOT_READY ) );

        hr = CreateTimer( dwDueTime, fPeriodic );
        CHECKHR_EXIT( hr );

    exit:
        return hr;
    }
};

