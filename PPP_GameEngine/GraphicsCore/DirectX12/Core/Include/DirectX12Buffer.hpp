//////////////////////////////////////////////////////////////////////////////////
//              Title:  DirectX12Buffer.hpp
//            Content:  DirectX12 Buffer Class  
//             Author:  Toide Yutaro
//             Create:  2020_11_16
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_BUFFER_HPP
#define DIRECTX12_BUFFER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "DirectX12GPUResource.hpp"
#include "DirectX12Debug.hpp"
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
struct Texture;
class GraphicsDeviceDirectX12;
class CommandContext;
static inline UINT CalcConstantBufferByteSize(UINT byteSize)
{
	return (byteSize + 255) & ~255;
}

static inline UINT AlignmentValue(UINT size, UINT alignment)
{
	return (size + alignment - (size % alignment));
}
//////////////////////////////////////////////////////////////////////////////////
//                           Each Buffer Class
//////////////////////////////////////////////////////////////////////////////////
#pragma region UploadBuffer
/****************************************************************************
*							 UploadBuffer
*************************************************************************//**
*  @struct    Upload Buffer
*  @brief     Upload buffer elements need to be multiples of 256 bytes (In case updatable constant buffer)
*             The difference from Default Buffer is whether map and unmap are available or not.
*****************************************************************************/
class UploadBuffer : public GPUResource
{
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	inline void CopyStart() { ThrowIfFailed(_resource->Map(0, nullptr, reinterpret_cast<void**>(&_mappedData))); }
	inline void CopyData(int elementIndex, const void* data){ std::memcpy(&_mappedData[elementIndex * _elementByteSize], data, _elementByteSize); }
	inline void CopyTotalData(const void* data, int dataLength){ std::memcpy(&_mappedData[0], data, _elementByteSize * (size_t)dataLength); }
	inline void CopyEnd(){ _resource->Unmap(0, nullptr); }
	void Destroy() override
	{
		if (_resource)     { _resource = nullptr; }
		_elementByteSize = 0;
	}
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/
	inline UINT GetElementCount   () { return _elementCount; }
	inline UINT GetElementByteSize() { return _elementByteSize; }
	inline UINT GetTotalByteSize  () { return _elementByteSize * _elementCount; }
	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	UploadBuffer(IDevice* device, UINT elementByteSize, UINT elementCount, bool isConstantBuffer, const std::wstring& addName = L"");
	UploadBuffer(const UploadBuffer&)            = delete;
	UploadBuffer& operator=(const UploadBuffer&) = delete;
	UploadBuffer(UploadBuffer&&)                 = default;
	UploadBuffer& operator=(UploadBuffer&&)      = default;
	virtual ~UploadBuffer() { Destroy(); }
private:
	/****************************************************************************
	**                Private Function
	*****************************************************************************/

	/****************************************************************************
	**                Private Member Variables
	*****************************************************************************/
	BYTE* _mappedData      = nullptr;
	UINT  _elementByteSize = 0;
	UINT  _elementCount    = 0;
	bool _isConstantBuffer = false;
};
#pragma endregion  UploadBuffer
#pragma region MeshBuffer
/****************************************************************************
*							 MeshBuffer
*************************************************************************//**
*  @struct    MeshBuffer
*  @brief     Mesh Buffer is a structure that collectively handles CPU and GPU resources
              for vertex and index data.
*****************************************************************************/
struct MeshBuffer
{
	using VertexBufferPtr = std::unique_ptr<UploadBuffer>;
	using IndexBufferPtr  = std::unique_ptr<UploadBuffer>;
public:
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/
	VertexBufferPtr VertexBuffer = nullptr;
	IndexBufferPtr  IndexBuffer  = nullptr;
	DXGI_FORMAT  IndexFormat   = DXGI_FORMAT_R16_UINT;

	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	inline D3D12_VERTEX_BUFFER_VIEW VertexBufferView()
	{
		D3D12_VERTEX_BUFFER_VIEW vbv={};
		vbv.BufferLocation = VertexBuffer->GetGPUVirtualAddress();
		vbv.StrideInBytes  = VertexBuffer->GetElementByteSize();
		vbv.SizeInBytes    = VertexBuffer->GetTotalByteSize();

		return vbv;
	}

	inline D3D12_INDEX_BUFFER_VIEW IndexBufferView()
	{
		D3D12_INDEX_BUFFER_VIEW ibv={};
		ibv.BufferLocation = IndexBuffer->GetGPUVirtualAddress();
		ibv.Format         = IndexFormat;
		ibv.SizeInBytes    = IndexBuffer->GetTotalByteSize();

		return ibv;
	}


	inline void Dispose()
	{
		if (VertexBuffer != nullptr) { VertexBuffer.reset(); }
		if (IndexBuffer  != nullptr) { IndexBuffer.reset(); }
	}
	/****************************************************************************
	**                Constructor
	*****************************************************************************/
	MeshBuffer() = default;
	MeshBuffer(const MeshBuffer&)            = default;
	MeshBuffer& operator=(const MeshBuffer&) = default;
	MeshBuffer(MeshBuffer&&)                 = default;
	MeshBuffer& operator=(MeshBuffer&&)      = default;
	~MeshBuffer() 
	{
		Dispose();
	};
private:
};
#pragma endregion MeshBuffer
#pragma region GPUBuffer
/****************************************************************************
*				  			GPUBuffer
*************************************************************************//**
*  @class     GPUBuffer
*  @brief     GPUBuffer (use for colorBuffer and RWStruturedBuffer)
*****************************************************************************/
class GPUBuffer : public GPUResource
{
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUSRV() const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUUAV() const;
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUSRV() const;
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUUAV() const;

	UINT64 GetBufferSize     () const { return (UINT64)_elementByteSize * _elementCount; }
	UINT   GetElementByteSize() const { return _elementByteSize; }
	UINT   GetElementCount   () const { return _elementCount; }
	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	GPUBuffer() = default;
	GPUBuffer(const GPUBuffer&)            = delete; // prohibit copy
	GPUBuffer& operator=(const GPUBuffer&) = delete; // prohibit copy
	GPUBuffer(GPUBuffer&&)                 = default;
	GPUBuffer& operator=(GPUBuffer&&)      = default;
	virtual ~GPUBuffer() { Destroy(); };
protected:
	/****************************************************************************
	**                Protected Function
	*****************************************************************************/
	virtual void Create(GraphicsDeviceDirectX12& graphicsDevice, UINT elementByteSize, UINT elementCount, const std::wstring& addName = L"", const void* data = nullptr);
	virtual void PrepareDescriptor() = 0;
	virtual void PrepareResource() = 0;
	/****************************************************************************
	**                Protected Member Variables
	*****************************************************************************/
	GraphicsDeviceDirectX12* _graphicsDevice = nullptr;
	UINT _elementByteSize = 0;
	UINT _elementCount    = 0;
	UINT _srvID           = 0;
	UINT _uavID           = 0;
	BYTE* _mappedData     = nullptr;
	bool _hasCreated      = false;
};
#pragma endregion GPUBuffer
#pragma region ColorBuffer
/****************************************************************************
*				  			ColorBuffer
*************************************************************************//**
*  @class     ColorBuffer
*  @brief     Color buffer
*****************************************************************************/
class ColorBuffer
{
	using TexturePtr = std::shared_ptr<Texture>;
	enum class ResourceID
	{
		RTV, SRV, UAV, CountOfResourceType
	};
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	void Create(GraphicsDeviceDirectX12& graphicsDevice, UINT width, UINT height, UINT arraySize = 0, const std::wstring& addName = L"", DXGI_FORMAT colorFormat = DXGI_FORMAT_R8G8B8A8_UNORM, float clearColor[4] = nullptr, UINT mipmaps = 1, UINT  fragmentCount = 1);
	void OnResize(UINT newWidth, UINT newHeight, UINT arraySize);
	void CopyFrom(CommandContext* commandContext, GPUResource* source);
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPURTV() const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUSRV() const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUUAV() const;
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPURTV() const;
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUSRV() const;
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUUAV() const;
	TexturePtr GetColorBuffer() { return _texture; }
	
	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	ColorBuffer() = default;
	virtual ~ColorBuffer() { _texture.reset(); };
	ColorBuffer(const ColorBuffer&)            = delete; // prohibit copy
	ColorBuffer& operator=(const ColorBuffer&) = delete; // prohibit copy
	ColorBuffer(ColorBuffer&&)                 = default;
	ColorBuffer& operator=(ColorBuffer&&)      = default;
protected:
	/****************************************************************************
	**                Protected Function
	*****************************************************************************/
	bool CreateTexture(UINT width, UINT height,UINT arraySize, DXGI_FORMAT format, float clearColor[4]);
	void IssueViewID();
	void PrepareDescriptor(DXGI_FORMAT format);

	/****************************************************************************
	**                Protected Member Variables
	*****************************************************************************/
	GraphicsDeviceDirectX12* _graphicsDevice;
	UINT   _resourceID[(int)ResourceID::CountOfResourceType];
	float  _clearColor[4] = { 0.0f };
	bool   _hasCreated = false;
	/*-------------------------------------------------------------------
	-                     Texture infomation
	---------------------------------------------------------------------*/
	UINT        _mipMapCount   = 0;
	UINT        _fragmentCount = 0;
	TexturePtr  _texture       = nullptr;

};
#pragma endregion ColorBuffer
#pragma region RWStructuredBuffer
/****************************************************************************
*				  			RWStructuredBuffer
*************************************************************************//**
*  @class     RWStructuredBuffer
*  @brief     Read and Write structured buffer
*****************************************************************************/
class RWStructuredBuffer : public GPUBuffer
{
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	void Create(GraphicsDeviceDirectX12& graphicsDevice, UINT elementByteSize, UINT elementCount, const std::wstring& addName = L"");
	void OnResize(UINT elementCount);
	void CopyData(UINT elementIndex, const void* data);
	void CopyTotalData(UINT elementCount, const void* data);
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/

	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	RWStructuredBuffer() = default;
	virtual ~RWStructuredBuffer() { Destroy(); };
	RWStructuredBuffer(const RWStructuredBuffer&)            = delete; // prohibit copy
	RWStructuredBuffer& operator=(const RWStructuredBuffer&) = delete; // prohibit copy
	RWStructuredBuffer(RWStructuredBuffer&&)                 = default;
	RWStructuredBuffer& operator=(RWStructuredBuffer&&)      = default;
protected:
	/****************************************************************************
	**                Protected Function
	*****************************************************************************/
	void PrepareDescriptor() override;
	void PrepareResource() override;
	/****************************************************************************
	**                Protected Member Variables
	*****************************************************************************/
	
};
#pragma endregion RWStructuredBuffer
#endif