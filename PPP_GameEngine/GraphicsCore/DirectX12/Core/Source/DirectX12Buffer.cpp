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
#include "GraphicsCore/Engine/Include/GraphicsCoreEngine.hpp"
#include "GraphicsCore/DirectX12/Core/Include/DirectX12Texture.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#pragma region Upload Buffer
UploadBuffer::UploadBuffer(IDevice* device, UINT elementByteSize, UINT elementCount, bool isConstantBuffer, const std::wstring& addName)
{
	_isConstantBuffer = isConstantBuffer;
	_elementByteSize = _isConstantBuffer ? CalcConstantBufferByteSize(elementByteSize) : elementByteSize;
	_elementCount    = elementCount;
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

#pragma endregion      Upload Buffer
#pragma region GPUBuffer

void GPUBuffer::Create(GraphicsDeviceDirectX12& graphicsDevice, UINT elementByteSize, UINT elementCount, const std::wstring& addName, const void* data)
{
	/*-------------------------------------------------------------------
	-                  Check Initialize
	---------------------------------------------------------------------*/
	if (_hasCreated) { return; }
	/*-------------------------------------------------------------------
	-                  Set up buffer infomation
	---------------------------------------------------------------------*/
	_graphicsDevice  = &graphicsDevice;
	_elementByteSize = elementByteSize;
	_elementCount    = elementCount;
	/*-------------------------------------------------------------------
	-                  Issue view id
	---------------------------------------------------------------------*/
	_srvID = _graphicsDevice->IssueViewID(HeapFlag::SRV);
	_uavID = _graphicsDevice->IssueViewID(HeapFlag::UAV);
	/*-------------------------------------------------------------------
	-                  Prepare Resource
	---------------------------------------------------------------------*/
	PrepareResource();
	/*-------------------------------------------------------------------
	-                  Prepare Descriptor
	---------------------------------------------------------------------*/
	PrepareDescriptor();
	/*-------------------------------------------------------------------
	-                  Set Name
	---------------------------------------------------------------------*/
	std::wstring name; if (addName != L"") { name += addName; name += L"::"; }
	name += L"RWStructured::";
	_resource->SetName(name.c_str());

	_hasCreated = true;
	
}

D3D12_CPU_DESCRIPTOR_HANDLE GPUBuffer::GetCPUSRV() const
{
	return _graphicsDevice->GetCPUResourceView(HeapFlag::SRV, _srvID);
}
D3D12_CPU_DESCRIPTOR_HANDLE GPUBuffer::GetCPUUAV() const
{
	return _graphicsDevice->GetCPUResourceView(HeapFlag::UAV, _uavID);
}
D3D12_GPU_DESCRIPTOR_HANDLE GPUBuffer::GetGPUSRV() const
{
	return _graphicsDevice->GetGPUResourceView(HeapFlag::SRV, _srvID);
}
D3D12_GPU_DESCRIPTOR_HANDLE GPUBuffer::GetGPUUAV() const
{
	return _graphicsDevice->GetGPUResourceView(HeapFlag::UAV, _uavID);
}
#pragma endregion   GPUBuffer
#pragma region ColorBuffer
#pragma region Public Function
/****************************************************************************
*				  			Create
*************************************************************************//**
*  @fn     bool ColorBuffer::Create(int width, int height, DXGI_FORMAT colorFormat, float clearColor[4], const std::wstring& addName)
*  @brief     Create Color Buffer
*  @param[in] GraphicsDeviceDirectX12& graphicsDevice
*  @param[in] UINT width
*  @param[in] UINT height
*  @param[in] UINT arraySize
*  @param[in] std::wstring addName
*  @param[in] DXGI_FORMAT colorFormat
*  @param[in] float clearColor[4]
*  @@aram[in] UINT mipmaps,
*  @param[in] UINT  fragmentCount
*  @param[in] addName
*****************************************************************************/
void ColorBuffer::Create(GraphicsDeviceDirectX12& graphicsDevice, UINT width, UINT height,UINT arraySize, const std::wstring& addName, DXGI_FORMAT colorFormat, float clearColor[4], UINT mipmaps, UINT  fragmentCount)
{
	/*-------------------------------------------------------------------
	-                  Check Initialize
	---------------------------------------------------------------------*/
	if (_hasCreated) { return; }
	/*-------------------------------------------------------------------
	-                  Set up buffer infomation
	---------------------------------------------------------------------*/
	_graphicsDevice  = &graphicsDevice;
	_texture         = std::make_shared<Texture>();
	_mipMapCount     = mipmaps;
	/*-------------------------------------------------------------------
	-                  IssueViewID
	---------------------------------------------------------------------*/
	IssueViewID();
	/*-------------------------------------------------------------------
	-                  Create Texture 
	---------------------------------------------------------------------*/
	CreateTexture(width, height, arraySize, colorFormat, clearColor);

	/*-------------------------------------------------------------------
	-           Set Resource Name
	---------------------------------------------------------------------*/
	std::wstring name = L""; if (addName != L"") { name += addName; name += L"::"; }
	name += L"ColorBuffer";
	_texture->Resource->SetName(name.c_str());
}
/****************************************************************************
*				  			OnResize
*************************************************************************//**
*  @fn    bool ColorBuffer::OnResize(UINT newWidth, UINT newHeight, UINT arraySize)
*  @brief     Resize buffer
*  @param[in] UINT width
*  @param[in] UINT height
*  @param[in] UINT arraySize
*****************************************************************************/
void ColorBuffer::OnResize(UINT newWidth, UINT newHeight, UINT arraySize)
{
	if (_texture->PixelSize.x != newWidth || _texture->PixelSize.y != newHeight)
	{
		CreateTexture(newWidth, newHeight, arraySize, _texture->Format, _clearColor);
		PrepareDescriptor(_texture->Format);
	}
}
/****************************************************************************
*                       CopyFrom
*************************************************************************//**
*  @fn        void ColorBuffer::CopyFrom(CommandContext* commandContext, GPUResource* source)
*  @brief     Copy from other resource to this buffer
*  @param[in] CommandContext* commandcContext
*  @param[in] GPUResource* source
*  @return 　　void
*****************************************************************************/
void ColorBuffer::CopyFrom(CommandContext* commandContext, GPUResource* source)
{
	commandContext->CopyBuffer(&_texture.get()->Resource, source);
}
#pragma region ResourceView
D3D12_CPU_DESCRIPTOR_HANDLE ColorBuffer::GetCPURTV() const
{
	return _graphicsDevice->GetCPUResourceView(HeapFlag::SRV, _resourceID[(int)ResourceID::RTV]);
}
D3D12_GPU_DESCRIPTOR_HANDLE ColorBuffer::GetGPURTV() const
{
	return _graphicsDevice->GetGPUResourceView(HeapFlag::RTV, _resourceID[(int)ResourceID::RTV]);
}
D3D12_CPU_DESCRIPTOR_HANDLE ColorBuffer::GetCPUSRV() const
{
	return _graphicsDevice->GetCPUResourceView(HeapFlag::SRV, _resourceID[(int)ResourceID::SRV]);
}
D3D12_CPU_DESCRIPTOR_HANDLE ColorBuffer::GetCPUUAV() const
{
	return _graphicsDevice->GetCPUResourceView(HeapFlag::UAV, _resourceID[(int)ResourceID::UAV]);
}
D3D12_GPU_DESCRIPTOR_HANDLE ColorBuffer::GetGPUSRV() const
{
	return _graphicsDevice->GetGPUResourceView(HeapFlag::SRV, _resourceID[(int)ResourceID::SRV]);
}
D3D12_GPU_DESCRIPTOR_HANDLE ColorBuffer::GetGPUUAV() const
{
	return _graphicsDevice->GetGPUResourceView(HeapFlag::UAV, _resourceID[(int)ResourceID::UAV]);
}
#pragma endregion ResourceView
#pragma endregion Public Function
#pragma region Protected Function
/****************************************************************************
*				  			CreateTexture
*************************************************************************//**
*  @fn         bool ColorBuffer::CreateTexture(UINT width, UINT height,UINT arraySize, DXGI_FORMAT format, float clearColor[4])
*  @brief     Create texture infomation
*  @param[in] UINT width
*  @param[in] UINT height
*  @param[in] DXGI_FORMAT format
*  @param[in] float clearColor[4]
*  @return    bool
*****************************************************************************/
bool ColorBuffer::CreateTexture(UINT width, UINT height,UINT arraySize, DXGI_FORMAT format, float clearColor[4])
{
	D3D12_HEAP_PROPERTIES heapProperty = HEAP_PROPERTY(D3D12_HEAP_TYPE_DEFAULT);
	D3D12_RESOURCE_DESC resourceDesc   = RESOURCE_DESC::Texture2D(format, width, height, 1, 1, 1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS | D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
	/*-------------------------------------------------------------------
	-                  Set clear color
	---------------------------------------------------------------------*/
	D3D12_CLEAR_VALUE clearValue = {};
	clearValue.Format = format;
	if (clearColor != nullptr)
	{
		for (int i = 0; i < 4; ++i) 
		{
			clearValue.Color[i] = clearColor[i];
			_clearColor[i] = clearColor[i];
		}
	}
	else
	{
		clearValue.Color[0] = 0.0f;
		clearValue.Color[1] = 0.0f;
		clearValue.Color[2] = 0.0f;
		clearValue.Color[3] = 1.0f;
	}

	/*-------------------------------------------------------------------
	-                  Commit resource to GPU
	---------------------------------------------------------------------*/
	_graphicsDevice->GetDevice()->CreateCommittedResource(
			&heapProperty, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_COMMON, &clearValue,
			IID_PPV_ARGS(_texture.get()->Resource.GetAddressOf()));
	/*-------------------------------------------------------------------
	-                  Set texture infomation
	---------------------------------------------------------------------*/
	_texture.get()->Format      = format;
	_texture.get()->PixelSize.x = static_cast<float>(width);
	_texture.get()->PixelSize.y = static_cast<float>(height);
	_texture.get()->PixelSize.z = static_cast<float>(arraySize);
	return true;
}
/****************************************************************************
*                       IssueViewID
*************************************************************************//**
*  @fn        void ColorBuffer::IssueViewID()
*  @brief     Issue resource view id (call once)
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void ColorBuffer::IssueViewID()
{
	_resourceID[(int)ResourceID::RTV] = _graphicsDevice->IssueViewID(HeapFlag::RTV);
	_resourceID[(int)ResourceID::SRV] = _graphicsDevice->IssueViewID(HeapFlag::SRV);
	_resourceID[(int)ResourceID::UAV] = _graphicsDevice->IssueViewID(HeapFlag::UAV);
}
/****************************************************************************
*                       PrepareDescriptor
*************************************************************************//**
*  @fn        void ColorBuffer::PrepareDescriptor(DXGI_FORMAT format)
*  @brief     Prepare descriptor
*  @param[in] DXGI_FORMAT format
*  @return 　　void
*****************************************************************************/
void ColorBuffer::PrepareDescriptor(DXGI_FORMAT format)
{
	/*-------------------------------------------------------------------
	-      Create descriptor for rtv
	---------------------------------------------------------------------*/
	D3D12_RENDER_TARGET_VIEW_DESC colorBufferDesc = {};
	colorBufferDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	colorBufferDesc.Format        = format;
	_graphicsDevice->GetDevice()->CreateRenderTargetView(
		_texture->Resource.GetResource(), &colorBufferDesc,
		_graphicsDevice->GetCPUResourceView(HeapFlag::RTV, _resourceID[(int)ResourceID::RTV]));

	/*-------------------------------------------------------------------
	-      Create descriptor for srv
	---------------------------------------------------------------------*/
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format                        = format;
	srvDesc.Shader4ComponentMapping       = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension                 = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels           = _mipMapCount;
	srvDesc.Texture2D.PlaneSlice          = 0;
	srvDesc.Texture2D.MostDetailedMip     = 0;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.0f;

	_graphicsDevice->GetDevice()->CreateShaderResourceView(
		_texture->Resource.GetResource(), &srvDesc,
		_graphicsDevice->GetCPUResourceView(HeapFlag::SRV, _resourceID[(int)ResourceID::SRV]));

	/*-------------------------------------------------------------------
	-      Create descriptor for uav
	---------------------------------------------------------------------*/
	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.ViewDimension        = D3D12_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Texture2D.MipSlice   = 0;
	uavDesc.Texture2D.PlaneSlice = 0;
	uavDesc.Format               = format;

	_graphicsDevice->GetDevice()->CreateUnorderedAccessView(
		_texture->Resource.GetResource(), nullptr,
		&uavDesc, 
		_graphicsDevice->GetCPUResourceView(HeapFlag::UAV, _resourceID[(int)ResourceID::UAV]));
}

#pragma endregion         Protected Function
#pragma endregion ColorBuffer
#pragma region RWStructuredBuffer
void RWStructuredBuffer::Create(GraphicsDeviceDirectX12& graphicsDevice, UINT elementByteSize, UINT elementCount, const std::wstring& addName)
{
	GPUBuffer::Create(graphicsDevice, elementByteSize, elementCount, addName, nullptr);
}
void RWStructuredBuffer::OnResize(UINT elementCount)
{
	if (elementCount != _elementCount)
	{
		_elementCount = elementCount;
		PrepareResource();
		PrepareDescriptor();
	}
}
void RWStructuredBuffer::CopyData(UINT elementIndex, const void* data)
{
	ThrowIfFailed(_resource->Map(0, nullptr, reinterpret_cast<void**>(&_mappedData)));
	std::memcpy(&_mappedData[elementIndex * _elementByteSize], &data, _elementByteSize);
	_resource->Unmap(0, nullptr);
}
void RWStructuredBuffer::CopyTotalData(UINT elementCount, const void* data)
{
	ThrowIfFailed(_resource->Map(0, nullptr, reinterpret_cast<void**>(&_mappedData)));
	std::memcpy(&_mappedData[0], data, _elementByteSize * (size_t)elementCount);
	_resource->Unmap(0, nullptr);
}
void RWStructuredBuffer::PrepareResource()
{
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	heapProp.CreationNodeMask     = 1;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	heapProp.Type                 = D3D12_HEAP_TYPE_CUSTOM;
	heapProp.VisibleNodeMask      = 1;

	auto buffer = RESOURCE_DESC::Buffer((UINT64)_elementByteSize * _elementCount);
	buffer.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

	ThrowIfFailed(_graphicsDevice->GetDevice()->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&buffer,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		nullptr,
		IID_PPV_ARGS(&_resource)));

}
void RWStructuredBuffer::PrepareDescriptor()
{

	/*-------------------------------------------------------------------
	-      Create descriptor for srv
	---------------------------------------------------------------------*/
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.Shader4ComponentMapping    = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension              = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement        = 0;
	srvDesc.Buffer.NumElements         = _elementCount;
	srvDesc.Buffer.StructureByteStride = _elementByteSize;
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;

	_graphicsDevice->GetDevice()->CreateShaderResourceView(
		_resource.Get(), &srvDesc,
		_graphicsDevice->GetCPUResourceView(HeapFlag::SRV, _srvID));

	/*-------------------------------------------------------------------
	-      Create descriptor for uav
	---------------------------------------------------------------------*/
	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	ZeroMemory(&uavDesc, sizeof(uavDesc));
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Buffer.NumElements = _elementCount;
	uavDesc.Buffer.StructureByteStride = _elementByteSize;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;

	_graphicsDevice->GetDevice()->CreateUnorderedAccessView(
		_resource.Get(), nullptr,
		&uavDesc,
		_graphicsDevice->GetCPUResourceView(HeapFlag::UAV, _uavID));
}
#pragma endregion RWStructuredBuffer