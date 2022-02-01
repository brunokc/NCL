#pragma once

#include "Win32Exception.h"
#include "Util.h"

namespace WCL::IO {

class IOException :
	public Win32Exception
{
public:
	IOException(const char* message) :
		Win32Exception(HResultFromLastError(), message)
	{
	}

	IOException(HRESULT hr, const char* message) :
		Win32Exception(hr, message)
	{
	}
};

} // namespace WCL::IO
