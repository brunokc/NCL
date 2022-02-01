#pragma once

#include <windows.h>
#include "Win32Exception.h"

using namespace WCL;

inline HRESULT
HResultFromLastError()
{
	return HRESULT_FROM_WIN32(::GetLastError());
}

template<class TException>
inline void
ThrowIfFailed(HRESULT hr, const char* message)
{
	if (FAILED(hr))
	{
		throw TException(hr, message);
	}
}

template<class TException>
inline void
ThrowIfLastError(const char* message)
{
	HRESULT hr = HResultFromLastError();
	ThrowIfFailed<TException>(hr, message);
}

template<typename THandle>
THandle
DuplicateFileHandle(
	const THandle& handle
	)
{
	THandle dupHandle;

	BOOL success = ::DuplicateHandle(
		GetCurrentProcess(),
		handle.Get(),
		GetCurrentProcess(),
		dupHandle.GetAddressOf(),
		0,
		FALSE,
		DUPLICATE_SAME_ACCESS
		);
	if (!success)
	{
		throw Win32Exception(HResultFromLastError(), "Error duplicating handle");
	}

	return dupHandle;
}
