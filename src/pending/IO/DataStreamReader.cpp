
#include "pch.h"
#include "DataStreamReader.h"

using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;

class DataStreamReader :
	public RuntimeClass<
		RuntimeClassFlags<ClassicCom>,
		IDataStreamReader,
		FtmBase
	>
{
public:
	HRESULT RuntimeClassInitialize(
		_In_ const std::wstring& path
		);

	//
	// IDataStreamReader
	//

	IFACEMETHOD(ReadUInt8)(
		_Out_ uint8_t* data
		);

	IFACEMETHOD(ReadUInt16)(
		_Out_ uint16_t* data
		);

	IFACEMETHOD(ReadUInt32)(
		_Out_ uint32_t* data
		);

	IFACEMETHOD(ReadUInt64)(
		_Out_ uint64_t* data
		);

	IFACEMETHOD(ReadData)(
		_In_ uint32_t size,
		_Out_writes_bytes_(size) void* data
		);

private:
	FileHandle _file;
};

HRESULT DataStreamReader::RuntimeClassInitialize(
	_In_ const std::wstring& path
	)
{
	FileHandle handle(::CreateFile(
		path.c_str(), 
		GENERIC_READ, 
		FILE_SHARE_READ, 
		nullptr, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		nullptr
		));
	if (!handle.IsValid())
	{
		return HRESULT_FROM_WIN32(::GetLastError());
	}

	_file = std::move(handle);
	return S_OK;
}

IFACEMETHODIMP DataStreamReader::ReadUInt8(
	_Out_ uint8_t* data
	)
{
	*data = 0;
	DWORD bytesRead = 0;
	if (!ReadFile(_file.Get(), data, sizeof(*data), &bytesRead, nullptr))
	{
		return HRESULT_FROM_WIN32(::GetLastError());
	}

	return S_OK;
}

IFACEMETHODIMP DataStreamReader::ReadUInt16(
	_Out_ uint16_t* data
	)
{
	*data = 0;
	DWORD bytesRead = 0;
	if (!ReadFile(_file.Get(), data, sizeof(*data), &bytesRead, nullptr))
	{
		return HRESULT_FROM_WIN32(::GetLastError());
	}

	return S_OK;
}

IFACEMETHODIMP DataStreamReader::ReadUInt32(
	_Out_ uint32_t* data
	)
{
	*data = 0;
	DWORD bytesRead = 0;
	if (!ReadFile(_file.Get(), data, sizeof(*data), &bytesRead, nullptr))
	{
		return HRESULT_FROM_WIN32(::GetLastError());
	}

	return S_OK;
}

IFACEMETHODIMP DataStreamReader::ReadUInt64(
	_Out_ uint64_t* data
	)
{
	*data = 0;
	DWORD bytesRead = 0;
	if (!ReadFile(_file.Get(), data, sizeof(*data), &bytesRead, nullptr))
	{
		return HRESULT_FROM_WIN32(::GetLastError());
	}

	return S_OK;
}

IFACEMETHODIMP DataStreamReader::ReadData(
	_In_ uint32_t size,
	_Out_writes_bytes_(size) void* data
	)
{
	DWORD bytesRead = 0;
	if (!ReadFile(_file.Get(), data, size, &bytesRead, nullptr))
	{
		return HRESULT_FROM_WIN32(::GetLastError());
	}

	return S_OK;
}

HRESULT
CreateStreamReader(
	_In_ const std::wstring& fileName,
	_COM_Outptr_ IDataStreamReader** reader
	)
{
	ComPtr<DataStreamReader> streamReader;
	HRESULT hr = MakeAndInitialize<DataStreamReader>(&streamReader, fileName);
	if (SUCCEEDED(hr))
	{
		hr = streamReader.CopyTo(reader);
	}
	return hr;
}
