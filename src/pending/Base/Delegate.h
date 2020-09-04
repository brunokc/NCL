
#pragma once

#include "FunctionTraits.h"

//
// References:
//
// - http://stackoverflow.com/questions/12403920/using-stdfunction-as-a-delegate-in-c11
// (see 3rd answer)
// - http://ideone.com/DebQgR
//

template<typename TFunction>
class Delegate
{
private:
    using ReturnType = typename WFx::Util::FunctionTraits<TFunction>::ReturnType;

public:
    Delegate()
    {
    }

    template<typename R, typename... Args>
    Delegate(
        _In_ R (*function)(Args...)
        ) :
        _delegate(function)
    {
    }

    template<class T, typename R, typename... Args>
    Delegate(
        _In_ T* object,
        _In_ R (T::*method)(Args...)
        )
    {
        _delegate = [object, method](_In_ Args&&... args) -> R
        {
            return (object->*method)(std::forward<Args>(args)...);
        };
    }

    template<class T, typename R, typename... Args>
    Delegate(
        _In_ const T* object,
        _In_ R (T::*method)(Args...) const
        )
    {
        _delegate = [object, method](_In_ Args&&... args) -> R
        {
            return (object->*method)(std::forward<Args>(args)...);
        };
    }

    template<typename... Args>
    ReturnType
    operator()(Args&&... args) const
    {
        return _delegate(std::forward<Args>(args)...);
    }

private:
    std::function<TFunction> _delegate;
};

template<class T>
struct FunctionType
{
    using Type = typename WFx::Util::FunctionTraits<decltype(&T::operator())>::FunctionType;
};

template<class T>
Delegate<typename FunctionType<T>::Type>
MakeDelegate(
    _In_ const T& lambda
    )
{
    return MakeDelegate(&lambda, &T::operator());
}

template<class T, typename R, typename... Args>
Delegate<R(Args...)>
MakeDelegate(
    _In_ T* object,
    _In_ R (T::*method)(Args...)
    )
{
    return Delegate<R(Args...)>(object, method);
}

template<class T, typename R, typename... Args>
Delegate<R(Args...)>
MakeDelegate(
    _In_ const T* object,
    _In_ R (T::*method)(Args...) const
    )
{
    return Delegate<R(Args...)>(object, method);
}
