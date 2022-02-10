
#pragma once

#include <memory>

#include <stdint.h>
#include <windows.h>
#include <wil/resource.h>

#include "Stream.h"
#include "File.h"

namespace WCL::IO {

class FileStream : public Stream
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

	virtual ~FileStream() = default;

	//
	// Stream implementation
	//
	virtual bool CanRead() override;
	virtual bool CanSeek() override;
	virtual bool CanWrite() override;

	virtual int64_t Length() override;
	virtual int64_t Position() override;

	virtual void Close() override;

	virtual void Flush() override;

	virtual int32_t Read(
		_Out_writes_bytes_(count) BYTE* buffer,
		int32_t count
		) override;

	virtual int32_t Read(
		std::vector<BYTE>& buffer
		) override;

	virtual int64_t Seek(
		int64_t offset,
		SeekOrigin origin
		) override;

	virtual void SetLength(
		int64_t length
		) override;

	virtual int32_t Write(
		_In_reads_bytes_(count) const BYTE* buffer,
		int32_t count
		) override;

	virtual int32_t Write(
		const std::vector<BYTE>& buffer
		) override;

private:
	wil::unique_hfile _fileHandle;
};

template<typename T>
int32_t ReadFromStream(std::shared_ptr<FileStream>& stream, T& object)
{
	return stream->Read(reinterpret_cast<BYTE*>(&object), sizeof(T));
}

template<typename T>
int32_t WriteToStream(std::shared_ptr<FileStream>& stream, const T& object)
{
	return stream->Write(reinterpret_cast<const BYTE*>(&object), sizeof(T));
}

} // namespace WCL::IO
