
#pragma once

#include "Util.h"
#include "IOException.h"

#include "FileStream.h"
#include "File.h"
#include "IOHelper.h"

using namespace WCL::IO;

FileStream::FileStream(
	const std::wstring& path,
	FileMode fileMode,
	FileAccess fileAccess,
	FileShare fileShare,
	int bufferSize,
	FileOptions fileOptions
	)
{
	// bufferSize is unused for now
	bufferSize;

	bool append = false;
	if (fileMode == FileMode::Append)
	{
		fileMode = FileMode::OpenOrCreate;
		append = true;
	}

	_fileHandle = IOHelper::Open(
		path,
		static_cast<DWORD>(fileAccess),
		static_cast<DWORD>(fileShare),
		static_cast<DWORD>(fileMode),
		FILE_ATTRIBUTE_NORMAL | static_cast<DWORD>(fileOptions)
		);

	if (append)
	{
		Seek(0, SeekOrigin::End);
	}
}

bool FileStream::CanRead()
{
	return true;
}

bool FileStream::CanSeek()
{
	return true;
}

bool FileStream::CanWrite()
{
	return true;
}

int64_t FileStream::Length()
{
	LARGE_INTEGER fileSize{ };
	BOOL success = ::GetFileSizeEx(_fileHandle.get(), &fileSize);
	if (!success)
	{
		throw IOException(HResultFromLastError(), "error while getting file size");
	}

	return fileSize.QuadPart;
}

int64_t FileStream::Position()
{
	return Seek(0, SeekOrigin::Current);
}

void FileStream::Close()
{
	_fileHandle.reset();
}

void FileStream::Flush()
{
	BOOL success = ::FlushFileBuffers(_fileHandle.get());
	if (!success)
	{
		throw IOException(HResultFromLastError(), "error flushing file buffers");
	}
}

int32_t FileStream::Read(
	_Out_writes_bytes_(count) BYTE* buffer,
	int32_t count
	)
{
	DWORD numberOfBytesRead = 0;
	BOOL success = ::ReadFile(
		_fileHandle.get(),
		buffer,
		count,
		&numberOfBytesRead,
		nullptr
		);
	if (!success)
	{
		throw IOException(HResultFromLastError(), "error while reading from file");
	}

	return numberOfBytesRead;
}

int64_t FileStream::Seek(
	int64_t offset,
	SeekOrigin origin
	)
{
	LARGE_INTEGER distanceToMove{ };
	LARGE_INTEGER newFilePointer{ };

	distanceToMove.QuadPart = offset;
	BOOL success = ::SetFilePointerEx(
		_fileHandle.get(),
		distanceToMove,
		&newFilePointer,
		static_cast<DWORD>(origin)
		);
	if (!success)
	{
		throw IOException(HResultFromLastError(), "error while seeking file");
	}

	return newFilePointer.QuadPart;
}

void FileStream::SetLength(
	int64_t length
	)
{
	Seek(length, SeekOrigin::Begin);
	BOOL success = ::SetEndOfFile(_fileHandle.get());
	if (!success)
	{
		throw IOException(HResultFromLastError(), "error setting file length");
	}
}

int32_t FileStream::Write(
	_In_reads_bytes_(count) const BYTE* buffer,
	int32_t count
	)
{
	DWORD numberOfBytesWritten = 0;
	BOOL success = ::WriteFile(
		_fileHandle.get(),
		buffer,
		count,
		&numberOfBytesWritten,
		nullptr
		);
	if (!success)
	{
		throw IOException(HResultFromLastError(), "error while writing to file");
	}

	return numberOfBytesWritten;
}
