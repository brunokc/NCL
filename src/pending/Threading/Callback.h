///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) Microsoft Corporation. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#ifdef MODERN_SDK
#include "wrl/client.h"
#include "wrl/implements.h"
#include "wrl/ftm.h"
#include <windows.system.threading.h>

//
// Added by ShipAssertFileTagger.exe
// If you copy this file, delete this section
// For full mapping list, see %sdxroot%\wectools\bin\shipAssertMappings.txt
//
#undef FILE_TAG
#define FILE_TAG 0x5B9
#endif

//
// Callback support for Threading and Timer
//

namespace Threading
{
    namespace Details
    {
#ifdef MODERN_SDK

        struct WorkDispatcherBase :
            public Microsoft::WRL::RuntimeClass<
                Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::Delegate>,
                ABI::Windows::System::Threading::IWorkItemHandler,
                ABI::Windows::System::Threading::ITimerElapsedHandler,
                ::Microsoft::WRL::FtmBase>
        {
            IFACEMETHOD( Invoke )(
                __in_opt ABI::Windows::Foundation::IAsyncAction* sender
                )
            {
                UNREFERENCED_PARAMETER( sender );
                return Invoke();
            }

            IFACEMETHOD( Invoke )(
                __in_opt ABI::Windows::System::Threading::IThreadPoolTimer* timer
                )
            {
                UNREFERENCED_PARAMETER( timer );
                return Invoke();
            }

            STDMETHOD( Invoke )(
                ) = 0;
        };

#else

        struct WorkDispatcherBase
        {
            virtual ~WorkDispatcherBase() { }

            STDMETHOD( Invoke )(
                ) = 0;
        };

#endif

        //
        // Dispatcher classes for static function callbacks
        //

        struct FunctionWorkDispatcher :
            public WorkDispatcherBase
        {
            typedef HRESULT (CALLBACK *CallbackProc)();

            FunctionWorkDispatcher(
                __in CallbackProc pCallback
                ) :
                m_pCallback( pCallback )
            {
                _ASSERTE( pCallback != nullptr );
            }

            IFACEMETHOD( Invoke )(
                )
            {
                return m_pCallback();
            }

            CallbackProc m_pCallback;
        };

        template<typename P1>
        struct FunctionWorkDispatcher1 :
            public WorkDispatcherBase
        {
            typedef HRESULT (CALLBACK *CallbackProc)( P1 );

            FunctionWorkDispatcher1(
                __in CallbackProc pCallback,
                __in P1 param1
                ) :
                m_pCallback( pCallback ),
                m_param1( param1 )
            {
                _ASSERTE( pCallback != nullptr );
            }

            IFACEMETHOD( Invoke )(
                )
            {
                return m_pCallback( m_param1 );
            }

            CallbackProc m_pCallback;
            P1 m_param1;
        };

        template<typename P1, typename P2>
        struct FunctionWorkDispatcher2 :
            public WorkDispatcherBase
        {
            typedef HRESULT (CALLBACK *CallbackProc)( P1, P2 );

            FunctionWorkDispatcher2(
                __in CallbackProc pCallback,
                __in P1 param1,
                __in P2 param2
                ) :
                m_pCallback( pCallback ),
                m_param1( param1 ),
                m_param2( param2 )
            {
                _ASSERTE( pCallback != nullptr );
            }

            IFACEMETHOD( Invoke )(
                )
            {
                return m_pCallback( m_param1, m_param2 );
            }

            CallbackProc m_pCallback;
            P1 m_param1;
            P2 m_param2;
        };

        template<typename P1, typename P2, typename P3>
        struct FunctionWorkDispatcher3 :
            public WorkDispatcherBase
        {
            typedef HRESULT (CALLBACK *CallbackProc)( P1, P2, P3 );

            FunctionWorkDispatcher3(
                __in CallbackProc pCallback,
                __in P1 param1,
                __in P2 param2,
                __in P3 param3
                ) :
                m_pCallback( pCallback ),
                m_param1( param1 ),
                m_param2( param2 ),
                m_param3( param3 )
            {
                _ASSERTE( pCallback != nullptr );
            }

            IFACEMETHOD( Invoke )(
                )
            {
                return m_pCallback( m_param1, m_param2, m_param3 );
            }

            CallbackProc m_pCallback;
            P1 m_param1;
            P2 m_param2;
            P3 m_param3;
        };

        //
        // Dispatcher classes for member function callbacks
        //

        template<class T>
        struct MemberFunctionWorkDispatcher :
            public WorkDispatcherBase
        {
            typedef HRESULT (CALLBACK T::*CallbackProc)();

            MemberFunctionWorkDispatcher(
                __in T* pThis,
                __in CallbackProc pCallback
                ) :
                m_pThis( pThis ),
                m_pCallback( pCallback )
            {
                _ASSERTE( pThis != nullptr );
                _ASSERTE( pCallback != nullptr );
            }

            IFACEMETHOD( Invoke )(
                )
            {
                return ( m_pThis->*m_pCallback )();
            }

            T* m_pThis;
            CallbackProc m_pCallback;
        };

        template<class T, typename P1>
        struct MemberFunctionWorkDispatcher1 :
            public WorkDispatcherBase
        {
            typedef HRESULT (CALLBACK T::*CallbackProc)( P1 );

            MemberFunctionWorkDispatcher1(
                __in T* pThis,
                __in CallbackProc pCallback,
                __in P1 param1
                ) :
                m_pThis( pThis ),
                m_pCallback( pCallback ),
                m_param1( param1 )
            {
                _ASSERTE( pThis != nullptr );
                _ASSERTE( pCallback != nullptr );
            }

            IFACEMETHOD( Invoke )(
                )
            {
                return ( m_pThis->*m_pCallback )( m_param1 );
            }

            T* m_pThis;
            CallbackProc m_pCallback;
            P1 m_param1;
        };

        template<class T, typename P1, typename P2>
        struct MemberFunctionWorkDispatcher2 :
            public WorkDispatcherBase
        {
            typedef HRESULT (CALLBACK T::*CallbackProc)( P1, P2 );

            MemberFunctionWorkDispatcher2(
                __in T* pThis,
                __in CallbackProc pCallback,
                __in P1 param1,
                __in P2 param2
                ) :
                m_pThis( pThis ),
                m_pCallback( pCallback ),
                m_param1( param1 ),
                m_param2( param2 )
            {
                _ASSERTE( pThis != nullptr );
                _ASSERTE( pCallback != nullptr );
            }

            IFACEMETHOD( Invoke )(
                )
            {
                return ( m_pThis->*m_pCallback )( m_param1, m_param2 );
            }

            T* m_pThis;
            CallbackProc m_pCallback;
            P1 m_param1;
            P2 m_param2;
        };


        template<class T, typename P1, typename P2, typename P3>
        struct MemberFunctionWorkDispatcher3 :
            public WorkDispatcherBase
        {
            typedef HRESULT (CALLBACK T::*CallbackProc)( P1, P2, P3 );

            MemberFunctionWorkDispatcher3(
                __in T* pThis,
                __in CallbackProc pCallback,
                __in P1 param1,
                __in P2 param2,
                __in P3 param3
                ) :
                m_pThis( pThis ),
                m_pCallback( pCallback ),
                m_param1( param1 ),
                m_param2( param2 ),
                m_param3( param3 ),
            {
                _ASSERTE( pThis != nullptr );
                _ASSERTE( pCallback != nullptr );
            }

            IFACEMETHOD( Invoke )(
                )
            {
                return ( m_pThis->*m_pCallback )( m_param1, m_param2, m_param3 );
            }

            T* m_pThis;
            CallbackProc m_pCallback;
            P1 m_param1;
            P2 m_param2;
            P3 m_param3;
        };

    } // namespace Details
} // namespace Threading

