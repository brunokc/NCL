#pragma once

namespace WCL::Collections {

template<typename E>
struct IEnumerator
{
    virtual bool operator!=(const IEnumerator<E>& other) = 0;

    virtual IEnumerator<E>& operator++() = 0;

    virtual E& operator*() = 0;
};

template<typename E>
struct IEnumerable
{
    virtual IEnumerator<E>& begin() = 0;

    virtual IEnumerator<E>& end() = 0;
};

} // namespace WCL::Collections
