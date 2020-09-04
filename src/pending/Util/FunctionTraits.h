
#pragma once

namespace WFx {
namespace Util {

template<class T>
struct FunctionTraits;

// Matches functions
template<typename R, typename... Args>
struct FunctionTraits<R(Args...)>
{
    static const size_t NArgs = sizeof...(Args);
    using ReturnType = R;
    using Arguments = std::tuple<Args...>;

    // We need FunctionType to be R(Args...) but the compiler won't accept it.
    // It accepts R(*)(Args...) thought, so ...
    using FunctionType = typename std::remove_pointer<R(*)(Args...)>::type;

    template<size_t N>
    struct Arg
    {
        static_assert(N < NArgs, "Invalid argument index for function trait");
        using Type = typename std::tuple_element<N, Arguments>::type;
    };

    using LastArgType = typename std::enable_if<(NArgs > 0), typename Arg<NArgs - 1>::Type>::type;
};

// Matches pointer to functions
template<typename R, typename... Args>
struct FunctionTraits<R(*)(Args...)> :
    public FunctionTraits<R(Args...)>
{
};

// Matches non-const (mutable) member functions
template<class C, typename R, typename... Args>
struct FunctionTraits<R(C::*)(Args...)> :
    public FunctionTraits<R(Args...)>
{
};

// Matches const member functions
template<class C, typename R, typename... Args>
struct FunctionTraits<R(C::*)(Args...) const> :
    public FunctionTraits<R(Args...)>
{
};

// Matches lambdas or anything that implements operator()
template<class T>
struct FunctionTraits :
    public FunctionTraits<decltype(&T::operator())>
{
};

}} //namespace WFx::Util
