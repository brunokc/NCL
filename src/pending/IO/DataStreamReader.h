
#pragma once

[uuid("731A2B22-F7CE-4F01-AD85-DB7BCE456FD3")]
interface IDataStreamReader : IUnknown
{
	IFACEMETHOD(ReadUInt8)(
		_Out_ uint8_t* data
		) = 0;

	IFACEMETHOD(ReadUInt16)(
		_Out_ uint16_t* data
		) = 0;

	IFACEMETHOD(ReadUInt32)(
		_Out_ uint32_t* data
		) = 0;

	IFACEMETHOD(ReadUInt64)(
		_Out_ uint64_t* data
		) = 0;

	IFACEMETHOD(ReadData)(
		_In_ uint32_t size,
		_Out_writes_bytes_(size) void* data
		) = 0;
};

HRESULT
CreateStreamReader(
	_In_ const std::wstring& fileName,
	_COM_Outptr_ IDataStreamReader** reader
	);
