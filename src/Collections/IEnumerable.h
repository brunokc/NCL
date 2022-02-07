#pragma once

namespace WCL::Collections {

template<class E>
struct IEnumerator
{
    virtual bool operator!=(const IEnumerator<E>& other) = 0;

    virtual IEnumerator<E>& operator++() = 0;

    virtual E& operator*() = 0;
};

template<class E>
struct IEnumerable
{
    virtual IEnumerator<E>& begin() = 0;

    virtual IEnumerator<E>& end() = 0;
};

} // namespace WCL::Collections
