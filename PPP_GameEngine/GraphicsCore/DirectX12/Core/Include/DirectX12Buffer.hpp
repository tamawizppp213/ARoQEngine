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
#include "DirectX12Texture.hpp"
#include "DirectX12Debug.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
static inline UINT CalcConstantBufferByteSize(UINT byteSize)
{
	return (byteSize + 255) & ~255;
}

static inline UINT AlignmentValue(UINT size, UINT alignment)
{
	return (size + alignment - (size % alignment));
}
#pragma region DefaultBuffer
/****************************************************************************
*							 DefaultBuffer
*************************************************************************//**
*  @class     Default Buffer
*  @brief     Default Buffer (Constant buffer elements need to be multiples of 256 bytes)
*  @details   Constant buffer elements need to be multiples of 256 bytes.
              This is because the hardware can only view constant data
              at m*256 byte offsets and of n*256 byte lengths.
              typedef struct D3D12_CONSTANT_BUFFER_VIEW_DESC {
              UINT64 OffsetInBytes; // multiple of 256
              UINT   SizeInBytes;   // multiple of 256
              D3D12_CONSTANT_BUFFER_VIEW_DESC;
*****************************************************************************/
struct DefaultBuffer
{
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/

	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/
	inline       ResourceComPtr& Resource() { return _defaultBuffer; }
	inline const ResourceComPtr& Resource() const { return _defaultBuffer; }

	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	explicit DefaultBuffer(DeviceComPtr& device, CommandListComPtr& commandList, const void* initData, UINT64 totalByteSize, ResourceComPtr& uploadBuffer, const std::wstring& addName = L"");
	DefaultBuffer(const DefaultBuffer& defaultBuffer)            = delete;
	DefaultBuffer& operator=(const DefaultBuffer& defaultBuffer) = delete;
	DefaultBuffer(DefaultBuffer&&) = default;
	DefaultBuffer& operator=(DefaultBuffer&&) = default;
	~DefaultBuffer();
private:
	/****************************************************************************
	**                Private Function
	*****************************************************************************/

	/****************************************************************************
	**                Private Member Variables
	*****************************************************************************/
	ResourceComPtr _defaultBuffer = nullptr;
	UINT64 _totalByteSize = 0;
};
#pragma endregion DefaultBuffer
#pragma region UploadBuffer
/****************************************************************************
*							 UploadBuffer
*************************************************************************//**
*  @struct    Upload Buffer
*  @brief     Upload buffer elements need to be multiples of 256 bytes (In case updatable constant buffer)
*             The difference from Default Buffer is whether map and unmap are available or not.
*****************************************************************************/
template <typename T>
struct UploadBuffer
{
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	inline void CopyStart()  { ThrowIfFailed(_uploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&_mappedData))); }
	inline void CopyData     (int elementIndex, const T& data){ std::memcpy(&_mappedData[elementIndex * _elementByteSize], &data, sizeof(T)); }
	inline void CopyTotalData(const T* data, int dataLength)  { std::memcpy(&_mappedData[0], data, sizeof(T) * dataLength); }
	inline void CopyEnd  ()  { _uploadBuffer->Unmap(0, nullptr); }
	inline void Dispose()
	{
		if (_uploadBuffer) { _uploadBuffer = nullptr; }
		if (_mappedData  ) { delete[] _mappedData; }
		_elementByteSize = 0;
	}
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/
	inline const ResourceComPtr& Resource() const { return _uploadBuffer; }
	
	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	UploadBuffer(Device* device, UINT elementCount, bool isConstantBuffer, const std::wstring& addName = L"");
	UploadBuffer(const UploadBuffer&)            = delete;
	UploadBuffer& operator=(const UploadBuffer&) = delete;
	UploadBuffer(UploadBuffer&&)                 = default;
	UploadBuffer& operator=(UploadBuffer&&)      = default;
	~UploadBuffer();
private:
	/****************************************************************************
	**                Private Function
	*****************************************************************************/

	/****************************************************************************
	**                Private Member Variables
	*****************************************************************************/
	ResourceComPtr _uploadBuffer;
	BYTE* _mappedData      = nullptr;
	UINT  _elementByteSize = 0;
	bool _isConstantBuffer = false;
};
template <typename T>UploadBuffer<T>::UploadBuffer(Device* device, UINT elementCount, bool isConstantBuffer, const std::wstring& addName)
{
	_isConstantBuffer = constantBuffer;

	_elementByteSize = sizeof(T);
	/*-------------------------------------------------------------------
	-      Constant buffer elements need to be multiples of 256 bytes
	---------------------------------------------------------------------*/
	if (_isConstantBuffer) { _elementByteSize = CalcConstantBufferByteSize(sizeof(T)); }
	/*-------------------------------------------------------------------
	-      Create Upload buffer
	---------------------------------------------------------------------*/
	auto heapProperty = HEAP_PROPERTY(D3D12_HEAP_TYPE_UPLOAD);
	auto buffer = RESOURCE_DESC::Buffer((UINT64)_elementByteSize * elementCount);
	ThrowIfFailed(device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&buffer,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_uploadBuffer)));

	/*-------------------------------------------------------------------
	-           Set Debug Name
	---------------------------------------------------------------------*/
	std::wstring name = L""; if (addName != L"") { name += addName; name += L"::"; }
	name += L"UploadBuffer";
	_uploadBuffer->SetName(name.c_str());
}
template <typename T>UploadBuffer<T>::~UploadBuffer()
{
	Dispose();
}
#pragma endregion  UploadBuffer
#pragma region MeshBuffer
struct MeshBuffer
{
public:
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/
	BlobComPtr     VertexBufferCPU      = nullptr;
	BlobComPtr     IndexBufferCPU       = nullptr;
	ResourceComPtr VertexBufferGPU      = nullptr;
	ResourceComPtr IndexBufferGPU       = nullptr;
	ResourceComPtr VertexBufferUploader = nullptr;
	ResourceComPtr IndexBufferUploader  = nullptr;

	UINT IndexCount = 0;
	UINT StartIndexLocation   = 0;
	INT  BaseVertexLocation   = 0;
	UINT VertexByteStride     = 0;
	UINT VertexBufferByteSize = 0;
	UINT IndexBufferByteSize  = 0;
	DXGI_FORMAT IndexFormat   = DXGI_FORMAT_R16_UINT;

	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	inline D3D12_VERTEX_BUFFER_VIEW VertexBufferView() const
	{
		D3D12_VERTEX_BUFFER_VIEW vbv={};
		vbv.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();
		vbv.StrideInBytes  = VertexByteStride;
		vbv.SizeInBytes    = VertexBufferByteSize;

		return vbv;
	}

	inline D3D12_INDEX_BUFFER_VIEW IndexBufferView() const
	{
		D3D12_INDEX_BUFFER_VIEW ibv={};
		ibv.BufferLocation = IndexBufferGPU->GetGPUVirtualAddress();
		ibv.Format         = IndexFormat;
		ibv.SizeInBytes    = IndexBufferByteSize;

		return ibv;
	}

	inline void DisposeUploaders()
	{
		VertexBufferUploader = nullptr;
		IndexBufferUploader  = nullptr;
	}

	inline void Dispose()
	{
		IndexCount = 0;
		StartIndexLocation   = 0;
		BaseVertexLocation   = 0;
		VertexByteStride     = 0;
		VertexBufferByteSize = 0;
		IndexBufferByteSize  = 0;
		if (VertexBufferCPU) { VertexBufferCPU = nullptr; }
		if (IndexBufferCPU)  { IndexBufferCPU  = nullptr; }
		if (VertexBufferGPU) { VertexBufferGPU = nullptr; }
		if (IndexBufferGPU)  { IndexBufferGPU  = nullptr; }
		if (VertexBufferUploader) { VertexBufferUploader = nullptr; }
		if (IndexBufferUploader) { IndexBufferUploader   = nullptr; }
	}
	/****************************************************************************
	**                Constructor
	*****************************************************************************/
	MeshBuffer() = default;
	MeshBuffer(const MeshBuffer&)            = default;
	MeshBuffer& operator=(const MeshBuffer&) = default;
	MeshBuffer(MeshBuffer&&)                 = default;
	MeshBuffer& operator=(MeshBuffer&&)      = default;
	~MeshBuffer() {};
private:
};


#pragma endregion MeshBuffer
#pragma region ColorBuffer
/****************************************************************************
*				  			Color Buffer
*************************************************************************//**
*  @struct    Color Buffer
*  @brief     colur buffer consumes RTV, SRV, UAV resource.
*****************************************************************************/
struct ColorBuffer
{
	enum class ResourceID 
	{
		RTV,
		SRV, 
		UAV,
		CountOfResourceType
	};
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	bool Create(int width, int height, DXGI_FORMAT colorFormat = DXGI_FORMAT_R8G8B8A8_UNORM, float clearColor[4] = nullptr, const std::wstring& addName = L"");
	bool OnResize(int newWidth, int newHeight);
	bool CopyToColorBuffer(ResourceComPtr& resource, D3D12_RESOURCE_STATES sourceState = D3D12_RESOURCE_STATE_COMMON);
	bool TransitionResourceState(D3D12_RESOURCE_STATES after);
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUSRV() const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPURTV() const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUUAV() const;
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUSRV() const;
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPURTV() const;
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUUAV() const;
	Texture& GetColorBuffer() { return _colorBuffer; }
	D3D12_RESOURCE_STATES GetUsageState() const { return _usageState; }
	DXGI_FORMAT   GetFormat()      const { return _format; }
	const float*  GetClearColor  () const { return _clearColor; }
	gm::Float2    GetTextureSize () const { return _colorBuffer.ImageSize; }
	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	ColorBuffer() = default;
	~ColorBuffer();
	ColorBuffer(const ColorBuffer&)            = delete;
	ColorBuffer& operator=(const ColorBuffer&) = delete;
	ColorBuffer(ColorBuffer&&)                 = default;
	ColorBuffer& operator=(ColorBuffer&&)      = default;
private:
	/****************************************************************************
	**                Private Function
	*****************************************************************************/
	bool CreateTexture(int width, int height, DXGI_FORMAT format, float clearColor[4]);
	bool CreateDescriptor(DXGI_FORMAT format);
	bool IssueViewID();

	/****************************************************************************
	**                Private Member Variables
	*****************************************************************************/
	Texture     _colorBuffer;
	UINT        _resourceID[(int)ResourceID::CountOfResourceType];
	DXGI_FORMAT _format;
	float       _clearColor[4];
	bool        _isInitialzed = false;
	D3D12_RESOURCE_STATES _usageState;
};
#pragma endregion ColorBuffer
#pragma region RWStructuredBuffer
/****************************************************************************
*				  			RWStructuredBuffer
*************************************************************************//**
*  @class     RWStructured Buffer
*  @brief     RW Structured Buffer
*****************************************************************************/
class RWStructuredBuffer
{
	enum class ResourceID
	{
		SRV,
		UAV,
		CountOfResourceType
	};

public:
	/**************************************************************************
	**                Public Function
	*****************************************************************************/
	bool Create(int elementByteSize, int elementCount, const std::wstring& addName = L"");
	bool OnResize(int elementCount);
	inline void CopyStart()                                 { ThrowIfFailed(_buffer->Map(0, nullptr, reinterpret_cast<void**>(&_mappedData)));}
	inline void CopyData(int elementIndex, const void* data){ std::memcpy(&_mappedData[elementIndex * _elementByteSize], data, _elementByteSize);}
	inline void CopyTotalData(const void* data)             { std::memcpy(&_mappedData[0], data, (__int64)_elementByteSize * _elementCount); }
	inline void CopyEnd(){ _buffer->Unmap(0, nullptr); }
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUSRV() const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUUAV() const;
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUSRV() const;
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUUAV() const;
	Resource* GetResource() { return _buffer.Get(); }
	bool IsCreated() const { return _isInitialized; }
	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	RWStructuredBuffer() = default;
	~RWStructuredBuffer() {};
	RWStructuredBuffer(const RWStructuredBuffer&)            = delete; // prohibit copy
	RWStructuredBuffer& operator=(const RWStructuredBuffer&) = delete; // prohibit copy
	RWStructuredBuffer(RWStructuredBuffer&&)                 = default;
	RWStructuredBuffer& operator=(RWStructuredBuffer&&)      = default;
private:
	/****************************************************************************
	**                Private Function
	*****************************************************************************/
	bool IssueViewID();
	bool PrepareDescriptor();
	bool PrepareResource();
	

	/****************************************************************************
	**                Private Member Variables
	*****************************************************************************/
	ResourceComPtr _buffer = nullptr;
	BYTE* _mappedData      = nullptr;
	UINT  _elementByteSize = 0;
	UINT  _elementCount    = 0;
	bool _isInitialized    = false;
	UINT _resourceID[(int)ResourceID::CountOfResourceType];
};

#pragma endregion RWStructuredBuffer
#endif