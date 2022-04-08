//////////////////////////////////////////////////////////////////////////////////
//              Title:  DirectX12Buffer.cpp
//            Content:  DirectX12 Buffer Class  
//             Author:  Toide Yutaro
//             Create:  2020_11_
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/DirectX12/Core/Include/DirectX12Buffer.hpp"
#include "GraphicsCore/DirectX12/Core/Include/DirectX12BaseStruct.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#pragma region Upload Buffer
UploadBuffer::UploadBuffer(IDevice* device, UINT elementByteSize, UINT elementCount, bool isConstantBuffer, const std::wstring& addName)
{
	_elementByteSize = _isConstantBuffer ? CalcConstantBufferByteSize(elementByteSize) : elementByteSize;
	/*-------------------------------------------------------------------
	-           Create 
	---------------------------------------------------------------------*/
	auto heapProp = HEAP_PROPERTY(D3D12_HEAP_TYPE_UPLOAD);
	auto buffer   = RESOURCE_DESC::Buffer((UINT64)_elementByteSize * elementCount);
	device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&buffer,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_resource));

	/*-------------------------------------------------------------------
	-           Set Name
	---------------------------------------------------------------------*/
	std::wstring name = L""; if (addName != L"") { name += addName; name += L"::"; }
	name += L"UploadBuffer";
	_resource->SetName(name.c_str());

}
void UploadBuffer::CopyData(int elementIndex, const void* data)
{
	ThrowIfFailed(_resource->Map(0, nullptr, reinterpret_cast<void**>(&_mappedData)));
	std::memcpy(&_mappedData[elementIndex * _elementByteSize], &data, _elementByteSize);
	_resource->Unmap(0, nullptr);
}
void UploadBuffer::CopyTotalData(const void* data, int dataLength)
{
	ThrowIfFailed(_resource->Map(0, nullptr, reinterpret_cast<void**>(&_mappedData)));
	std::memcpy(&_mappedData[0], data, _elementByteSize * (size_t)dataLength);
	_resource->Unmap(0, nullptr);
}
#pragma endregion Upload Buffer
