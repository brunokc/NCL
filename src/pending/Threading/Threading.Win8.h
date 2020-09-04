///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) Microsoft Corporation. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <winbase.h>
#include <utility>

#include "wrl/client.h"
#include "wrl/implements.h"
#include <windows.system.threading.h>

#include "inlines.h"
#include "SafeUtil.h"       // SAFE_GETLASTERROR()
#include "Callback.h"
#include "HStringWrap.h"

//
// Added by ShipAssertFileTagger.exe
// If you copy this file, delete this section
// For full mapping list, see %sdxroot%\wectools\bin\shipAssertMappings.txt
//
#undef FILE_TAG
#define FILE_TAG 0x5C7

using namespace ::ABI::Microsoft::Entertainment::Internal;


enum class WorkItemPri
{
    Low = ::ABI::Windows::System::Threading::WorkItemPriority_Low,
    Normal = ::ABI::Windows::System::Threading::WorkItemPriority_Normal,
    High = ::ABI::Windows::System::Threading::WorkItemPriority_High
};


namespace Threading
{
    namespace Details
    {
        //
        // Handlers for static function callbacks
        //

        inline
        Microsoft::WRL::ComPtr<::ABI::Windows::System::Threading::IWorkItemHandler>
        MakeFunctionHandler(
            __in HRESULT ( CALLBACK *pCallback )()
            )
        {
            return Microsoft::WRL::Make<FunctionWorkDispatcher>( pCallback );
        }

        template<typename P1>
        inline
        Microsoft::WRL::ComPtr<::ABI::Windows::System::Threading::IWorkItemHandler>
        MakeFunctionHandler(
            __in HRESULT ( CALLBACK *pCallback )( P1 ),
            __in const P1& param1
            )
        {
            return Microsoft::WRL::Make<FunctionWorkDispatcher1<P1>>(
                pCallback,
                param1
                );
        }

        template<typename P1, typename P2>
        inline
        Microsoft::WRL::ComPtr<::ABI::Windows::System::Threading::IWorkItemHandler>
        MakeFunctionHandler(
            __in HRESULT ( CALLBACK *pCallback )( P1, P2 ),
            __in const P1& param1,
            __in const P2& param2
            )
        {
            return Microsoft::WRL::Make<FunctionWorkDispatcher2<P1, P2>>(
                pCallback,
                param1,
                param2
                );
        }

        template<typename P1, typename P2, typename P3>
        inline
        Microsoft::WRL::ComPtr<::ABI::Windows::System::Threading::IWorkItemHandler>
        MakeFunctionHandler(
            __in HRESULT ( CALLBACK *pCallback )( P1, P2, P3 ),
            __in const P1& param1,
            __in const P2& param2,
            __in const P3& param3
            )
        {
            return Microsoft::WRL::Make<FunctionWorkDispatcher3<P1, P2, P3>>(
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
        Microsoft::WRL::ComPtr<::ABI::Windows::System::Threading::IWorkItemHandler>
        MakeHandler(
            __in T* pThis,
            __in HRESULT ( CALLBACK T::*pCallback )()
            )
        {
            return Microsoft::WRL::Make<MemberFunctionWorkDispatcher<T>>(
                pThis,
                pCallback
                );
        }

        template<class T, typename P1>
        inline
        Microsoft::WRL::ComPtr<::ABI::Windows::System::Threading::IWorkItemHandler>
        MakeHandler(
            __in T* pThis,
            __in HRESULT ( CALLBACK T::*pCallback )( P1 ),
            __in const P1& param1
            )
        {
            return Microsoft::WRL::Make<MemberFunctionWorkDispatcher1<T, P1>>(
                pThis,
                pCallback,
                param1
                );
        }

        template<class T, typename P1, typename P2>
        inline
        Microsoft::WRL::ComPtr<::ABI::Windows::System::Threading::IWorkItemHandler>
        MakeHandler(
            __in T* pThis,
            __in HRESULT ( CALLBACK T::*pCallback )( P1, P2 ),
            __in const P1& param1,
            __in const P2& param2
            )
        {
            return Microsoft::WRL::Make<MemberFunctionWorkDispatcher2<T, P1, P2>>(
                pThis,
                pCallback,
                param1,
                param2
                );
        }

        template<class T, typename P1, typename P2, typename P3>
        inline
        Microsoft::WRL::ComPtr<::ABI::Windows::System::Threading::IWorkItemHandler>
        MakeHandler(
            __in T* pThis,
            __in HRESULT ( CALLBACK T::*pCallback )( P1, P2, P3 ),
            __in const P1& param1,
            __in const P2& param2,
            __in const P3& param3
            )
        {
            return Microsoft::WRL::Make<MemberFunctionWorkDispatcher3<T, P1, P2, P3>>(
                pThis,
                pCallback,
                param1,
                param2,
                param3
                );
        }

        //
        // WinRT-friendly helper to queue up async work
        //

        static
        HRESULT
        QueueWorkItem(
            __in const Microsoft::WRL::ComPtr<::ABI::Windows::System::Threading::IWorkItemHandler>& workItemHandler,
            __in ::WorkItemPri priority
            )
        {
            HRESULT hr = S_OK;
            Microsoft::WRL::ComPtr<::ABI::Windows::System::Threading::IThreadPoolStatics> threadpoolStatics;
            StringReference strThreadPoolClassId( RuntimeClass_Windows_System_Threading_ThreadPool );

            SHIPASSERT( workItemHandler.Get() != nullptr );

            hr = ::ABI::Windows::Foundation::GetActivationFactory(
                strThreadPoolClassId.Get(),
                &threadpoolStatics
                );
            if ( SUCCEEDED( hr ) )
            {
                Microsoft::WRL::ComPtr<::ABI::Windows::Foundation::IAsyncAction> tpOperation;
                Microsoft::WRL::ComPtr<::ABI::Windows::Foundation::IAsyncInfo> tpAsync;

                hr = threadpoolStatics->RunWithPriorityAsync(
                    workItemHandler.Get(),
                    static_cast<::ABI::Windows::System::Threading::WorkItemPriority>( priority ),
                    &tpOperation
                    );

                if ( SUCCEEDED( hr ) )
                {
                    hr = tpOperation.As( &tpAsync );
                }
            }

            return hr;
        }
    } // namespace Details
} // namespace Threading

