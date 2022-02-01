////////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) Microsoft Corporation. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////

#pragma once

namespace WCL {

//
// This class represents a value which can be null.
//

template<typename T>
class Nullable
{
public:
    typedef T ValueType;

    Nullable() :
        _isNull(true)
    {
    }

    Nullable(Nullable&& other) :
        _value(std::move(other._value)),
        _isNull(std::move(other._isNull))
    {
        other._isNull = true;
        other._value = T();
    }

    Nullable(std::nullptr_t) :
        _value(T()),
        _isNull(true)
    {
    }

    // Nullable(const T& value, bool isNull = false) :
    //     _value(value),
    //     _isNull(isNull)
    // {
    // }

    Nullable(const T* value)
    {
        *this = value;
    }

    template<typename U>
    explicit Nullable(const U& value) :
        _value(T(value)),
        _isNull(false)
    {
    }

    template<typename U>
    explicit Nullable(const U* value) :
        _isNull(true)
    {
        if (value != nullptr)
        {
            *this = T(*value);
        }
    }

    const T& GetValueOrDefault(const T& defaultValue) const
    {
        return (IsNull() ? defaultValue : _value);
    }

    T& GetValue()
    {
        _isNull = false;
        return _value;
    }

    bool IsNull() const
    {
        return _isNull;
    }

    operator bool() const
    {
        return !IsNull();
    }

    operator const T*() const
    {
        return (IsNull() ? nullptr : &_value);
    }

    const T& operator*() const
    {
        AssertNotNull();
        return _value;
    }

    const T* operator->() const
    {
        AssertNotNull();
        return &_value;
    }

    T* operator->()
    {
        AssertNotNull();
        return &_value;
    }

    Nullable& operator=(const T& value)
    {
        _value = value;
        _isNull = false;
        return *this;
    }

    Nullable& operator=(const T* value)
    {
        if (value != nullptr)
        {
            _value = *value;
            _isNull = false;
        }
        else
        {
            _isNull = true;
        }

        return *this;
    }

    Nullable& operator=(Nullable&& other)
    {
        if (this != &other)
        {
            _isNull = std::move(other._isNull);
            other._isNull = true;

            _value = std::move(other._value);
            other._value = T();
        }
        return *this;
    }

    Nullable& operator=(std::nullptr_t)
    {
        _isNull = true;
        return *this;
    }

    bool operator==(std::nullptr_t) const
    {
        return _isNull;
    }

    bool operator==(const Nullable& nullable) const
    {
        return _isNull == nullable._isNull
            && _value == nullable._value;
    }
    // OR ...
    /*
    bool operator==(const Nullable& nullable) const
    {
        if (_isNull == nullable._isNull)
        {
            return (_isNull || _value == nullable._value);
        }
        else
        {
            return false;
        }
    }
    */

private:
    void AssertNotNull()
    {
        if (_isNull)
        {
            throw std::runtime_error("Value is null");
        }
    }

private:
    T _value;
    bool _isNull;
};

template<typename T>
Nullable<T> MakeNullable(const T& value)
{
    return Nullable<T>(value);
}

template<typename T>
Nullable<T> MakeNullable(const T* value)
{
    return Nullable<T>(value);
}

template<typename T>
Nullable<T> MakeNullable()
{
    return Nullable<T>();
}

} // namespace WCL
