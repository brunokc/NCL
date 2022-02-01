
#pragma once

#include <functional>
#include "FunctionTraits.h"

template<typename T>
using Delegate = std::function<T>;

template<class T>
Delegate<typename WCL::Util::FunctionTraits<T>::FunctionType>
MakeDelegate(
    const T& lambda
    )
{
    return lambda;
}

template<class T, typename R, typename... Args>
Delegate<R(Args...)>
MakeDelegate(
    T* object,
    R (T::*method)(Args...)
    )
{
    return [object, method](Args&&... args)
    {
        return (object->*method)(std::forward<Args>(args)...);
    };
}

template<class T, typename R, typename... Args>
Delegate<R(Args...)>
MakeDelegate(
    const T* object,
    R (T::*method)(Args...) const
    )
{
    return [object, method](Args&&... args)
    {
        return (object->*method)(std::forward<Args>(args)...);
    };
}
