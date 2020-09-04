////////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) Microsoft Corporation. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <utility>

//
// Added by ShipAssertFileTagger.exe
// If you copy this file, delete this section
// For full mapping list, see %sdxroot%\wectools\bin\shipAssertMappings.txt
//
#undef FILE_TAG
#define FILE_TAG 0x5B7


template<class I>
class CAdviseHelper
{
private:
    struct MemberFunction
    {
        typedef HRESULT (CALLBACK I::*CallbackProc)();
    };

    template<typename TArg1>
    struct MemberFunction1
    {
        typedef HRESULT (CALLBACK I::*CallbackProc)( TArg1 );
    };

    template<typename TArg1, typename TArg2>
    struct MemberFunction2
    {
        typedef HRESULT (CALLBACK I::*CallbackProc)( TArg1, TArg2 );
    };

    template<typename TArg1, typename TArg2, typename TArg3>
    struct MemberFunction3
    {
        typedef HRESULT (CALLBACK I::*CallbackProc)( TArg1, TArg2, TArg3 );
    };

    template<typename TArg1, typename TArg2, typename TArg3, typename TArg4>
    struct MemberFunction4
    {
        typedef HRESULT (CALLBACK I::*CallbackProc)( TArg1, TArg2, TArg3, TArg4 );
    };

    template<typename TArg1, typename TArg2, typename TArg3, typename TArg4, typename TArg5>
    struct MemberFunction5
    {
        typedef HRESULT (CALLBACK I::*CallbackProc)( TArg1, TArg2, TArg3, TArg4, TArg5 );
    };

public:
    HRESULT
    Advise(
        __in I* pI
        );

    HRESULT
    Unadvise(
        __in I* pI
        );

    HRESULT
    NotifyAll(
        __in typename MemberFunction::CallbackProc pfnMethod
        );

    template<typename TArg1>
    HRESULT
    NotifyAll(
        __in typename MemberFunction1<TArg1>::CallbackProc pfnMethod,
        __in const TArg1& arg1
        );

    template<typename TArg1, typename TArg2>
    HRESULT
    NotifyAll(
        __in typename MemberFunction2<TArg1, TArg2>::CallbackProc pfnMethod,
        __in const TArg1& arg1,
        __in const TArg2& arg2
        );

    template<typename TArg1, typename TArg2, typename TArg3>
    HRESULT
    NotifyAll(
        __in typename MemberFunction3<TArg1, TArg2, TArg3>::CallbackProc pfnMethod,
        __in const TArg1& arg1,
        __in const TArg2& arg2,
        __in const TArg3& arg3
        );

    template<typename TArg1, typename TArg2, typename TArg3, typename TArg4>
    HRESULT
    NotifyAll(
        __in typename MemberFunction4<TArg1, TArg2, TArg3, TArg4>::CallbackProc pfnMethod,
        __in const TArg1& arg1,
        __in const TArg2& arg2,
        __in const TArg3& arg3,
        __in const TArg4& arg4
        );

    template<typename TArg1, typename TArg2, typename TArg3, typename TArg4, typename TArg5>
    HRESULT
    NotifyAll(
        __in typename MemberFunction5<TArg1, TArg2, TArg3, TArg4, TArg5>::CallbackProc pfnMethod,
        __in const TArg1& arg1,
        __in const TArg2& arg2,
        __in const TArg3& arg3,
        __in const TArg4& arg4,
        __in const TArg5& arg5
        );


private:
    CInterfaceArray<I> m_rgspCallbacks;
};


template<class I>
HRESULT
CAdviseHelper<I>::Advise(
    __in I* pI
    )
{
    HRESULT hr = S_OK;

    CHECKNULL_EXIT( pI, E_POINTER );

    try
    {
        for( SIZE_T i = 0; i < m_rgspCallbacks.GetCount(); i++ )
        {
            if ( m_rgspCallbacks[i] == pI )
            {
                SETHR_EXIT( HRESULT_FROM_WIN32( ERROR_ALREADY_ASSIGNED ) );
            }
        }

        //
        // Add callback to array
        //
        m_rgspCallbacks.Add( pI );
    }
    catch ( BaseException& e )
    {
        hr = e;
    }

exit:
    return hr;
}


template<class I>
HRESULT
CAdviseHelper<I>::Unadvise(
    __in I* pI
    )
{
    HRESULT hr = S_OK;

    CHECKNULL_EXIT( pI, E_POINTER );

    try
    {
        for( SIZE_T i = 0; i < m_rgspCallbacks.GetCount(); i++ )
        {
            if( m_rgspCallbacks[i] == pI )
            {
                m_rgspCallbacks.RemoveAt( i );
                SETHR_EXIT( S_OK );
            }
        }
    }
    catch ( BaseException& e )
    {
        hr = e;
    }

    //
    // Callback was not found in array returning S_FALSE to indicate the callback
    // will no longer recieve a notification but there was not a callback
    // registered at time of call
    //
    SETHR_EXIT( S_FALSE );

exit:
    return hr;
}


template<class I>
HRESULT
CAdviseHelper<I>::NotifyAll(
    __in typename MemberFunction::CallbackProc pfnMethod
    )
{
    HRESULT hr = S_OK;

    CHECKNULL_EXIT( pfnMethod, E_POINTER );

    for( SIZE_T i = 0; i < m_rgspCallbacks.GetCount(); i++ )
    {
        CComPtr<I> spI = m_rgspCallbacks[i];
        if ( spI != nullptr )
        {
            HRESULT hrNotify = (spI->*pfnMethod)();
            hr = ( hr == S_OK && hrNotify == S_OK ? S_OK : S_FALSE );
        }
    }

exit:
    return hr;
}


template<class I>
template<typename TArg1>
HRESULT
CAdviseHelper<I>::NotifyAll(
    __in typename MemberFunction1<TArg1>::CallbackProc pfnMethod,
    __in const TArg1& arg1
    )
{
    HRESULT hr = S_OK;

    CHECKNULL_EXIT( pfnMethod, E_POINTER );

    for( SIZE_T i = 0; i < m_rgspCallbacks.GetCount(); i++ )
    {
        CComPtr<I> spI = m_rgspCallbacks[i];
        if ( spI != nullptr )
        {
            HRESULT hrNotify = (spI->*pfnMethod)( arg1 );
            hr = ( hr == S_OK && hrNotify == S_OK ? S_OK : S_FALSE );
        }
    }

exit:
    return hr;
}


template<class I>
template<typename TArg1, typename TArg2>
HRESULT
CAdviseHelper<I>::NotifyAll(
    __in typename MemberFunction2<TArg1, TArg2>::CallbackProc pfnMethod,
    __in const TArg1& arg1,
    __in const TArg2& arg2
    )
{
    HRESULT hr = S_OK;

    CHECKNULL_EXIT( pfnMethod, E_POINTER );

    for( SIZE_T i = 0; i < m_rgspCallbacks.GetCount(); i++ )
    {
        CComPtr<I> spI = m_rgspCallbacks[i];
        if ( spI != nullptr )
        {
            HRESULT hrNotify = (spI->*pfnMethod)( arg1, arg2 );
            hr = ( hr == S_OK && hrNotify == S_OK ? S_OK : S_FALSE );
        }
    }

exit:
    return hr;
}


template<class I>
template<typename TArg1, typename TArg2, typename TArg3>
HRESULT
CAdviseHelper<I>::NotifyAll(
    __in typename MemberFunction3<TArg1, TArg2, TArg3>::CallbackProc pfnMethod,
    __in const TArg1& arg1,
    __in const TArg2& arg2,
    __in const TArg3& arg3
    )
{
    HRESULT hr = S_OK;

    CHECKNULL_EXIT( pfnMethod, E_POINTER );

    for( SIZE_T i = 0; i < m_rgspCallbacks.GetCount(); i++ )
    {
        CComPtr<I> spI = m_rgspCallbacks[i];
        if ( spI != nullptr )
        {
            HRESULT hrNotify = (spI->*pfnMethod)( arg1, arg2, arg3 );
            hr = ( hr == S_OK && hrNotify == S_OK ? S_OK : S_FALSE );
        }
    }

exit:
    return hr;
}


template<class I>
template<typename TArg1, typename TArg2, typename TArg3, typename TArg4>
HRESULT
CAdviseHelper<I>::NotifyAll(
    __in typename MemberFunction4<TArg1, TArg2, TArg3, TArg4>::CallbackProc pfnMethod,
    __in const TArg1& arg1,
    __in const TArg2& arg2,
    __in const TArg3& arg3,
    __in const TArg4& arg4
    )
{
    HRESULT hr = S_OK;

    CHECKNULL_EXIT( pfnMethod, E_POINTER );

    for( SIZE_T i = 0; i < m_rgspCallbacks.GetCount(); i++ )
    {
        CComPtr<I> spI = m_rgspCallbacks[i];
        if ( spI != nullptr )
        {
            HRESULT hrNotify = (spI->*pfnMethod)( arg1, arg2, arg3, arg4 );
            hr = ( hr == S_OK && hrNotify == S_OK ? S_OK : S_FALSE );
        }
    }

exit:
    return hr;
}


template<class I>
template<typename TArg1, typename TArg2, typename TArg3, typename TArg4, typename TArg5>
HRESULT
CAdviseHelper<I>::NotifyAll(
    __in typename MemberFunction5<TArg1, TArg2, TArg3, TArg4, TArg5>::CallbackProc pfnMethod,
    __in const TArg1& arg1,
    __in const TArg2& arg2,
    __in const TArg3& arg3,
    __in const TArg4& arg4,
    __in const TArg5& arg5
    )
{
    HRESULT hr = S_OK;

    CHECKNULL_EXIT( pfnMethod, E_POINTER );

    for( SIZE_T i = 0; i < m_rgspCallbacks.GetCount(); i++ )
    {
        CComPtr<I> spI = m_rgspCallbacks[i];
        if ( spI != nullptr )
        {
            HRESULT hrNotify = (spI->*pfnMethod)( arg1, arg2, arg3, arg4, arg5 );
            hr = ( hr == S_OK && hrNotify == S_OK ? S_OK : S_FALSE );
        }
    }

exit:
    return hr;
}

