
#pragma once

namespace WCL {
namespace Base {

namespace Details {

    // HasArrowOperator returns true for pointers and for classes that overload the
    // arrow operator (operator->())
    template<typename T>
    struct HasArrowOperator
    {
    private:
        static uint8_t test(&T::operator->())

    public:
        static const bool value = std::is_pointer<T>::value;
    };

    // template<bool _Test, class _Ty = void>
    // using enable_if_t = typename std::enable_if<_Test, _Ty>::type;
} // namespace Details

template<typename T>
class Property
{
public:
    operator T() const
    {
        return Get();
    }

    Property& operator=(const T& value)
    {
        Set(value);
        return *this;
    }

    // Allow assignment of values of other types, as long as they can be converted to T
    template<typename U>
    // Property& operator=(const U& value, typename std::enable_if<std::is_convertible<U, T>::value>::type* = nullptr)
    Property& operator=(const U& value, std::enable_if_t<std::is_convertible<U, T>::value>* = nullptr)
    {
        _value = value;
        return *this;
    }

    // typename std::enable_if<std::is_pointer<T>::value, T>::type operator->()
    std::enable_if_t<Details::HasArrowOperator<T>::value, T> operator->() const
    {
        return _value;
    }

protected:
    T Get()
    {
        return _value;
    }

    void Set(const T& value)
    {
        _value = value;
    }

private:
    T _value;
};

}} // namespace WCL::Base
