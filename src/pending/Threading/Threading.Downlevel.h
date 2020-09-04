///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) Microsoft Corporation. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <winbase.h>
#include <utility>

#include "inlines.h"
#include "SafeUtil.h"       // SAFE_GETLASTERROR()
#include "Callback.h"

//
// Added by ShipAssertFileTagger.exe
// If you copy this file, delete this section
// For full mapping list, see %sdxroot%\wectools\bin\shipAssertMappings.txt
//
#undef FILE_TAG
#define FILE_TAG 0x5C5

enum class WorkItemPri
{
    Low,
    Normal,
    High,
};


namespace Threading
{
    namespace Details
    {
        //
        // Handlers for static function callbacks
        //

        inline
        WorkDispatcherBase*
        MakeFunctionHandler(
            __in HRESULT ( CALLBACK *pCallback )()
            )
        {
            return new (std::nothrow) FunctionWorkDispatcher( pCallback );
        }

        template<typename P1>
        inline
        WorkDispatcherBase*
        MakeFunctionHandler(
            __in HRESULT ( CALLBACK *pCallback )( P1 ),
            __in const P1& param1
            )
        {
            return new (std::nothrow) FunctionWorkDispatcher1<P1>(
                pCallback,
                param1
                );
        }

        template<typename P1, typename P2>
        inline
        WorkDispatcherBase*
        MakeFunctionHandler(
            __in HRESULT ( CALLBACK *pCallback )( P1, P2 ),
            __in const P1& param1,
            __in const P2& param2
            )
        {
            return new (std::nothrow) FunctionWorkDispatcher2<P1, P2>(
                pCallback,
                param1,
                param2
                );
        }

        template<typename P1, typename P2, typename P3>
        inline
        WorkDispatcherBase*
        MakeFunctionHandler(
            __in HRESULT ( CALLBACK *pCallback )( P1, P2, P3 ),
            __in const P1& param1,
            __in const P2& param2,
            __in const P3& param3
            )
        {
            return new (std::nothrow) FunctionWorkDispatcher3<P1, P2, P3>(
                pCallback,
                param1,
                param2,
                param3
                );
        }

        //
        // Handlers for member function callbacks
        //

        template<class T>
        inline
        WorkDispatcherBase*
        MakeHandler(
            __in T* pThis,
            __in HRESULT ( CALLBACK T::*pCallback )()
            )
        {
            return new (std::nothrow) MemberFunctionWorkDispatcher<T>( pThis, pCallback );
        }

        template<class T, typename P1>
        inline
        WorkDispatcherBase*
        MakeHandler(
            __in T* pThis,
            __in HRESULT ( CALLBACK T::*pCallback )( P1 ),
            __in const P1& param1
            )
        {
            return new (std::nothrow) MemberFunctionWorkDispatcher1<T, P1>(
                pThis,
                pCallback,
                param1
                );
        }

        template<class T, typename P1, typename P2>
        inline
        WorkDispatcherBase*
        MakeHandler(
            __in T* pThis,
            __in HRESULT ( CALLBACK T::*pCallback )( P1, P2 ),
            __in const P1& param1,
            __in const P2& param2
            )
        {
            return new (std::nothrow) MemberFunctionWorkDispatcher2<T, P1, P2>(
                pThis,
                pCallback,
                param1,
                param2
                );
        }

        template<class T, typename P1, typename P2, typename P3>
        inline
        WorkDispatcherBase*
        MakeHandler(
            __in T* pThis,
            __in HRESULT ( CALLBACK T::*pCallback )( P1, P2, P3 ),
            __in const P1& param1,
            __in const P2& param2,
            __in const P3& param3
            )
        {
            return new (std::nothrow) MemberFunctionWorkDispatcher3<T, P1, P2, P3>(
                pThis,
                pCallback,
                param1,
                param2,
                param3
                );
        }

        //
        // Helpers to queue up async work
        //

        struct WorkItem
        {
            WorkItem(
                __in WorkDispatcherBase* pDispatcher,
                __in ::WorkItemPri priority
                ) :
                m_pDispatcher( pDispatcher ),
                m_priority( priority )
            {
            }

            ~WorkItem()
            {
                SafeDelete( m_pDispatcher );
            }

            WorkDispatcherBase* m_pDispatcher;
            ::WorkItemPri m_priority;
        };


        static
        DWORD
        WINAPI
        ThreadProc(
            __in PVOID pParameter
            )
        {
            HRESULT hr = S_OK;
            BOOL fSuccess = FALSE;
            int nSavedPriority = THREAD_PRIORITY_ERROR_RETURN;
            int nPriority = THREAD_PRIORITY_NORMAL;
            WorkItem* pWorkItem = static_cast<WorkItem*>( pParameter );

            switch( pWorkItem->m_priority )
            {
                case WorkItemPri::Low:
                    nPriority = THREAD_PRIORITY_BELOW_NORMAL;
                    break;

                case WorkItemPri::High:
                    nPriority = THREAD_PRIORITY_ABOVE_NORMAL;
                    break;

                case WorkItemPri::Normal:
                default:
                    nPriority = THREAD_PRIORITY_NORMAL;
                    break;
            }

            nSavedPriority = GetThreadPriority( GetCurrentThread() );
            if ( nSavedPriority == THREAD_PRIORITY_ERROR_RETURN )
            {
                hr = HRESULT_FROM_WIN32( SAFE_GETLASTERROR() );
            }

            if ( SUCCEEDED( hr ) && nSavedPriority != nPriority )
            {
                fSuccess = SetThreadPriority( GetCurrentThread(), nPriority );
                if ( !fSuccess )
                {
                    hr = HRESULT_FROM_WIN32( SAFE_GETLASTERROR() );
                }
            }

            if ( pWorkItem->m_pDispatcher != nullptr )
            {
                pWorkItem->m_pDispatcher->Invoke();
            }

            //
            // Restore saved thread priority (if we actually were able to get it
            // and if the priority requested was different the thread's
            // priority)
            //
            if ( nSavedPriority != THREAD_PRIORITY_ERROR_RETURN &&
                 nSavedPriority != nPriority )
            {
                (void)SetThreadPriority( GetCurrentThread(), nSavedPriority );
            }

            SafeDelete( pWorkItem );

            return NOERROR;
        }

        static
        HRESULT
        QueueWorkItem(
            __in WorkDispatcherBase* pWorkItemDispatcher,
            __in ::WorkItemPri priority
            )
        {
            HRESULT hr = S_OK;

            _ASSERTE( pWorkItemDispatcher != nullptr );
            if ( pWorkItemDispatcher == nullptr )
            {
                return E_POINTER;
            }

            WorkItem* pWorkItem = new (std::nothrow) WorkItem( pWorkItemDispatcher, priority );
            if ( pWorkItem == nullptr  )
            {
                hr = E_OUTOFMEMORY;
            }

            if ( SUCCEEDED( hr ) )
            {
                BOOL fSuccess = QueueUserWorkItem(
                    ThreadProc,
                    pWorkItem,
                    WT_EXECUTELONGFUNCTION
                    );
                if ( ! fSuccess )
                {
                    delete pWorkItem;
                    hr = HRESULT_FROM_WIN32( SAFE_GETLASTERROR() );
                }
            }

            return hr;
        }
    } // namespace Details
} // namespace Threading

