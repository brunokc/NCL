
#pragma once

#include <vector>

#include <windows.h>

namespace WCL::IO {

enum class SeekOrigin 
{
	Begin = 0,
	Current = 1,
	End = 2,
};

class Stream {
public:
	static const Stream* Null;

	virtual ~Stream() { }

	virtual bool CanRead() = 0;
	virtual bool CanSeek() = 0;
	virtual bool CanWrite() = 0;

	virtual int64_t Length() = 0;
	virtual int64_t Position() = 0;

	virtual void Close() = 0;

	virtual void Flush() = 0;
	
	virtual int32_t Read(
		BYTE* buffer,
		int32_t count
		) = 0;

	virtual int32_t Read(
		std::vector<BYTE>& buffer
		) = 0;

	virtual int64_t Seek(
		int64_t offset,
		SeekOrigin origin
		) = 0;

	virtual void SetLength(
		int64_t length
		) = 0;
	
	virtual int32_t Write(
		const BYTE* buffer,
		int32_t count
		) = 0;

	virtual int32_t Write(
		const std::vector<BYTE>& buffer
		) = 0;
};

} // namespace WCL::IO
