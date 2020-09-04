///////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) Microsoft Corporation. All rights reserved.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <winbase.h>
#include <utility>

#include "Callback.h"

#ifdef MODERN_SDK
#include "Threading.Win8.h"
#else
#include "Threading.Downlevel.h"

//
// Added by ShipAssertFileTagger.exe
// If you copy this file, delete this section
// For full mapping list, see %sdxroot%\wectools\bin\shipAssertMappings.txt
//
#undef FILE_TAG
#define FILE_TAG 0x5C6
#endif

namespace Threading
{
    //
    // New threading functions that will either take a function pointer with its
    // associated parameters or take an object instance and a member function.
    // They live under the Threading namespace, so calls will be qualified with
    // the Threading:: prefix (strongly suggested -- don't 'import' the
    // Threading namespace), which helps differentiate it from similar Win32
    // API.
    //

    //
    // Static function flavors of QueueWorkItem
    //

    template<typename... Args>
    inline
    HRESULT
    QueueWorkItem(
        _In_ ::WorkItemPri priority,
        _In_ HRESULT (CALLBACK *pCallback)(Args&&...),
        _In_ Args&&... args
       )
    {
        return Details::QueueWorkItem(
            Details::MakeFunctionHandler(pCallback, std::forward<Args>(args)...),
            priority
            );
    }


    // inline
    // HRESULT
    // QueueWorkItem(
    //     _In_ HRESULT (CALLBACK *pCallback)(),
    //     _In_ ::WorkItemPri priority = ::WorkItemPri::Normal
    //    )
    // {
    //     return Details::QueueWorkItem(
    //         Details::MakeFunctionHandler(pCallback),
    //         priority
    //         );
    // }


    // template<typename P1>
    // inline
    // HRESULT
    // QueueWorkItem(
    //     _In_ HRESULT (CALLBACK *pCallback)(P1),
    //     _In_ const P1& param1,
    //     _In_ ::WorkItemPri priority = ::WorkItemPri::Normal
    //     )
    // {
    //     return Details::QueueWorkItem(
    //         Details::MakeFunctionHandler(
    //             pCallback,
    //             param1
    //             ),
    //         priority
    //         );
    // }


    // template<typename P1, typename P2>
    // inline
    // HRESULT
    // QueueWorkItem(
    //     _In_ HRESULT (CALLBACK *pCallback)(P1, P2),
    //     _In_ const P1& param1,
    //     _In_ const P2& param2,
    //     _In_ ::WorkItemPri priority = ::WorkItemPri::Normal
    //     )
    // {
    //     return Details::QueueWorkItem(
    //         Details::MakeFunctionHandler(
    //             pCallback,
    //             param1,
    //             param2
    //             ),
    //         priority
    //         );
    // }


    // template<typename P1, typename P2, typename P3>
    // inline
    // HRESULT
    // QueueWorkItem(
    //     _In_ HRESULT (CALLBACK *pCallback)(P1, P2, P3),
    //     _In_ const P1& param1,
    //     _In_ const P2& param2,
    //     _In_ const P3& param3,
    //     _In_ ::WorkItemPri priority = ::WorkItemPri::Normal
    //     )
    // {
    //     return Details::QueueWorkItem(
    //         Details::MakeFunctionHandler(
    //             pCallback,
    //             param1,
    //             param2,
    //             param3
    //             ),
    //         priority
    //         );
    // }

    //
    // Member function flavors of QueueWorkItem
    //

    template<class T, typename... Args>
    inline
    HRESULT
    QueueWorkItem(
        _In_ ::WorkItemPri priority,
        _In_ T* pThis,
        _In_ HRESULT (CALLBACK T::*pCallback)(Args&&...),
        _In_ Args&&... args
        )
    {
        return Details::QueueWorkItem(
            Details::MakeHandler(pThis, pCallback, std::forward<Args>(args)...),
            priority
            );
    }


    // template<class T>
    // inline
    // HRESULT
    // QueueWorkItem(
    //     _In_ T* pThis,
    //     _In_ HRESULT (CALLBACK T::*pCallback)(),
    //     _In_ ::WorkItemPri priority = ::WorkItemPri::Normal
    //     )
    // {
    //     return Details::QueueWorkItem(
    //         Details::MakeHandler(
    //             pThis,
    //             pCallback
    //             ),
    //         priority
    //         );
    // }


    // template<class T, typename P1>
    // inline
    // HRESULT
    // QueueWorkItem(
    //     _In_ T* pThis,
    //     _In_ HRESULT (CALLBACK T::*pCallback)(P1),
    //     _In_ const P1& param1,
    //     _In_ ::WorkItemPri priority = ::WorkItemPri::Normal
    //     )
    // {
    //     return Details::QueueWorkItem(
    //         Details::MakeHandler(
    //             pThis,
    //             pCallback,
    //             param1
    //             ),
    //         priority
    //         );
    // }


    // template<class T, typename P1, typename P2>
    // inline
    // HRESULT
    // QueueWorkItem(
    //     _In_ T* pThis,
    //     _In_ HRESULT (CALLBACK T::*pCallback)(P1, P2),
    //     _In_ const P1& param1,
    //     _In_ const P2& param2,
    //     _In_ ::WorkItemPri priority = ::WorkItemPri::Normal
    //     )
    // {
    //     return Details::QueueWorkItem(
    //         Details::MakeHandler(
    //             pThis,
    //             pCallback,
    //             param1,
    //             param2
    //             ),
    //         priority
    //         );
    // }


    // template<class T, typename P1, typename P2, typename P3>
    // inline
    // HRESULT
    // QueueWorkItem(
    //     _In_ T* pThis,
    //     _In_ HRESULT (CALLBACK T::*pCallback)(P1, P2, P3),
    //     _In_ const P1& param1,
    //     _In_ const P2& param2,
    //     _In_ const P3& param3,
    //     _In_ ::WorkItemPri priority = ::WorkItemPri::Normal
    //     )
    // {
    //     return Details::QueueWorkItem(
    //         Details::MakeHandler(
    //             pThis,
    //             pCallback,
    //             param1,
    //             param2,
    //             param3
    //             ),
    //         priority
    //         );
    // }
} // namespace Threading

