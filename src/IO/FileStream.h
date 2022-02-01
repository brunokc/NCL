
#pragma once

#include <memory>

#include <stdint.h>
#include <windows.h>
#include <wil/resource.h>

#include "Stream.h"
#include "File.h"

namespace WCL::IO {

class FileStream :
	public Stream
{
	friend class File;

	struct private_marker { };

public:
	FileStream(const private_marker&, wil::unique_hfile&& file) :
		_fileHandle(std::move(file))
	{
	}

	static std::shared_ptr<FileStream> Make(wil::unique_hfile&& file)
	{
		return std::make_shared<FileStream>(private_marker{}, std::move(file));
	}

public:
	FileStream(
		const std::wstring& path,
		FileMode fileMode,
		FileAccess fileAccess,
		FileShare fileShare,
		int bufferSize,
		FileOptions options
		);

	//
	// Stream implementation
	//
	virtual bool CanRead();
	virtual bool CanSeek();
	virtual bool CanWrite();

	virtual int64_t Length();
	virtual int64_t Position();

	virtual void Close();

	virtual void Flush();

	virtual int32_t Read(
		_Out_writes_bytes_(count) BYTE* buffer,
		int32_t count
	);

	virtual int64_t Seek(
		int64_t offset,
		SeekOrigin origin
	);

	virtual void SetLength(
		int64_t length
	);

	virtual int32_t Write(
		_In_reads_bytes_(count) const BYTE* buffer,
		int32_t count
	);

private:
	wil::unique_hfile _fileHandle;
};

} // namespace WCL::IO
