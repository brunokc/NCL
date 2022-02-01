
#pragma once

#include "Win32Exception.h"
#include "FunctionTraits.h"

namespace WCL {
namespace Util {

namespace Details {

    struct HMODULENullTraits
    {
        typedef HMODULE Type;

        inline static bool Close(Type h) throw()
        {
            return ::FreeLibrary(h) != FALSE;
        }

        inline static Type GetInvalidValue() throw()
        {
            return nullptr;
        }
    };

    using ModuleHandle = Microsoft::WRL::Wrappers::HandleT<HMODULENullTraits>;

} // namespace Details

template<typename TFunc>
class LibraryFunction
{
public:
    LibraryFunction(TFunc func) :
        _func(func)
    {
    }

    LibraryFunction(LibraryFunction&& libFunc) :
        _func(std::move(libFunc._func))
    {
    }

    template<typename... Args>
    typename FunctionTraits<TFunc>::ReturnType
    operator()(Args&&... args)
    {
        return _func(std::forward<Args>(args)...);
    }

private:
    TFunc _func;
};

class LibraryModule
{
public:
    LibraryModule(PCWSTR modulePath) :
        m_hrLoadLibrary(S_OK)
    {
        _module.Attach(::LoadLibraryEx(modulePath, nullptr, 0));
        if (!_module.IsValid())
        {
            // _hrLoadLibrary = HRESULT_FROM_WIN32(GetLastError());
            throw Win32Exception(HRESULT_FROM_WIN32(GetLastError()), "LoadLibraryEx failed");
        }
    }

    ~LibraryModule()
    {
    }

    operator bool()
    {
        return SUCCEEDED(_hrLoadLibrary);
    }

    HRESULT GetLoadResult()
    {
        return _hrLoadLibrary;
    }

    template<typename TFunc>
    LibraryFunction<TFunc> GetProc(PCSTR procName)
    {
        return static_cast<TFunc>(::GetProcAddress(_module.Get(), procName));
    }

private:
    Details::ModuleHandle _module;
    HRESULT _hrLoadLibrary;
};

/*
    Example Usage:

    LibraryModule mod(L"twinplat.dll");

    if (!mod)
    {
        return mod.GetLoadResult();
    }

    auto initProc = mod.GetProc<HRESULT()>("InitializeMoshplat");
    auto finalizeProc = mod.GetProc<HRESULT(IImmersiveShellController*)>("FinalizeInitMoshplat");
*/

}} // namespace WCL::Util
