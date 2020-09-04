#pragma once

#include "Exception.h"

inline HRESULT
HResultFromLastError()
{
	return HRESULT_FROM_WIN32(::GetLastError());
}

template<typename THandle>
THandle
DuplicateFileHandle(
	_In_ const THandle& handle
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
