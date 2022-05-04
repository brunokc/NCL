#pragma once

#include <memory>
#include <type_traits>
#include <wil/resource.h>

namespace WCL::Threading {

// CreateThreadFromLambda: allow for the creation of a thread while passing any lambda, including
// the ones with non-empty captures.
template<typename TLambda>
wil::unique_handle CreateThread(TLambda&& lambda)
{
    // When l-values are passed in, TLambda will be a reference (e.g.: SomeFunction::<lambda_XYZ>&)
    // We can't create pointers to references, so we'll remove the reference from the type first.
    using LambdaType = std::remove_reference<TLambda>::type;

    // Functions like CreateThread or QueueUserWorkItem take function pointers as parameters. Lambdas
    // can't be used with those functions unless they have empty captures, in which case they are 
    // automatically converted into function pointers. To make it work, we wrap the incoming lambda
    // in a capture-less lambda for our thread proc.
    auto threadProc = [](void* pParam) -> DWORD
    {
        std::unique_ptr<LambdaType> pLambda{ static_cast<LambdaType*>(pParam) };
        (*pLambda)();
        return 0;
    };

    // Create a copy of the lambda on the heap so we can pass it by reference to threadProc
    //std::unique_ptr<LambdaType> pLambda(new LambdaType(std::forward<TLambda>(lambda)));
    auto pLambda = std::make_unique<LambdaType>(std::forward<TLambda>(lambda));

    wil::unique_handle threadHandle(::CreateThread(
        nullptr,
        0,
        threadProc,
        pLambda.get(),
        0,
        nullptr));

    if (threadHandle)
    {
        // The thread now owns the lambda copy
        pLambda.release();
    }

    return threadHandle;
}

template<typename TLambda>
void RunOnThreadpool(TLambda&& lambda)
{
    // When l-values are passed in, TLambda will be a reference (e.g.: SomeFunction::<lambda_XYZ>&)
    // We can't create pointers to references, so we'll remove the reference from the type first.
    using LambdaType = std::remove_reference<TLambda>::type;

    // The thread proc lambda needs to have an empty capture so it can be converted into a function
    // pointer and thus be passed as a parameter to CreateThreadpoolWork
    auto threadpoolWork = [](PTP_CALLBACK_INSTANCE, PVOID context)
    {
        std::unique_ptr<LambdaType> pLambda{ static_cast<LambdaType*>(context) };
        (*pLambda)();
    };

    // Create a copy of the lambda on the heap so we can pass it by reference to threadProc
    //std::unique_ptr<LambdaType> pLambda(new LambdaType(std::forward<TLambda>(lambda)));
    auto pLambda = std::make_unique<LambdaType>(std::forward<TLambda>(lambda));

    BOOL success = ::TrySubmitThreadpoolCallback(threadpoolWork, pLambda.get(), nullptr);
    if (!success)
    {
        //throw 
    }

    // Threadpool thread owns the lambda object now
    pLambda.release();
}

} // namespace WCL::Threading
