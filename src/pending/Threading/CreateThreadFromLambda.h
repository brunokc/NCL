//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//

#pragma once

#include <wil/resources.h>

namespace WFx {
namespace Threading {

// CreateThreadFromLambda: allow for the creation of a thread while passing any lambda, including
// the ones with non-empty captures.
template<typename TLambda>
wil::unique_handle
CreateThread(
    _In_ TLambda const &lambda
    )
{
    // The thread proc lambda needs to have an empty capture so it can be converted into a function
    // pointer and thus be passed as a parameter to functions like CreateThread or QueueUserWorkItem
    auto threadProc = [](_In_ void* pParam) -> DWORD
    {
        std::unique_ptr<TLambda> pLambda(static_cast<TLambda*>(pParam));
        (*pLambda)();
        return 0;
    };

    // Create a copy of the lambda on the heap so we can pass it by reference to threadProc
    std::unique_ptr<TLambda> pLambda(new TLambda(lambda));

    wil::unique_handle threadHandle(::CreateThread(
        nullptr,
        0,
        threadProc,
        pLambda.get(),
        0,
        nullptr));

    // The thread now owns the lambda copy
    pLambda.release();

    return threadHandle;
}

}} // namespace WFx::Threading
