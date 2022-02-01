#pragma once

namespace WCL::Collections {

template<class C, class E>
struct IEnumerator
{
    virtual bool operator!=(const C& other) = 0;

    virtual IEnumerator<C, E>& operator++() = 0;

    virtual E& operator*() = 0;
};

template<class C, class E>
struct IEnumerable
{
    virtual IEnumerator<C, E>& begin() = 0;

    virtual IEnumerator<C, E>& end() = 0;
};

} // namespace WCL::Collections
