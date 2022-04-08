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
	void CopyData(int elementIndex, const void* data);
	void CopyTotalData(const void* data, int dataLength);
	void Destroy() override
	{
		if (_resource)     { _resource = nullptr; }
		if (_mappedData  ) { delete[] _mappedData; }
		_elementByteSize = 0;
	}
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/

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

#endif