//////////////////////////////////////////////////////////////////////////////////
//            @fileÅ@Å@  DirectX12BaseStruct.hpp
//            @brief    DirectX12 Struct for DirexctX12_Base.hpp 
//                      (For Studying)(Reference: Microsoft d3dx12.h) 
//            @author:  Toide Yutaro
//            @date  :  2020_11_
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_BASE_STRUCT_HPP
#define DIRECTX12_BASE_STRUCT_HPP
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "DirectX12Core.hpp"
#include "DirectX12Include.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
struct NULL_STRUCT {};
extern const DECLSPEC_SELECTANY NULL_STRUCT D3D12_DEFAULT;
#pragma region Box
/****************************************************************************
*					DIRECTX12_BOX : public D3D12_BOX
*************************************************************************//**
*  @struct    DirectX12_BOX
*  @brief     DirectX12_BOX (This struct is derived from D3D12_BOX)
*****************************************************************************/
struct DIRECTX12_BOX : public D3D12_BOX
{
	DIRECTX12_BOX() {};
	explicit DIRECTX12_BOX(const DIRECTX12_BOX& box) : D3D12_BOX(box) {};
	explicit DIRECTX12_BOX(LONG Left, LONG Right)
	{
		left = Left;
		top = 0;
		front = 0;
		right = Right;
		bottom = 1;
		back = 1;
	}
	explicit DIRECTX12_BOX(LONG Left, LONG Top, LONG Right, LONG Bottom)
	{
		left = Left;
		top = Top;
		front = 0;
		right = Right;
		bottom = Bottom;
		back = 1;
	}
	explicit DIRECTX12_BOX(LONG Left, LONG Top, LONG Front, LONG Right, LONG Bottom, LONG Back)
	{
		left = Left;
		top = Top;
		front = Front;
		right = Right;
		bottom = Bottom;
		back = Back;
	}
	~DIRECTX12_BOX() {};
	operator const D3D12_BOX& () const { return *this; }
};

inline bool operator==(const D3D12_BOX& l, const D3D12_BOX& r)
{
	return l.left == r.left && l.top == r.top && l.front == r.front &&
		l.right == r.right && l.bottom == r.bottom && l.back == r.back;
}
inline bool operator!=(const D3D12_BOX& l, const D3D12_BOX& r)
{
	return !(l == r);
}
#pragma endregion Box
#pragma region DepthStencil
/****************************************************************************
*			DEPTH_STENCIL_DESC   public  D3D12_DEPTH_STENCIL_DESC
*************************************************************************//**
*  @struct    DEPTH_STENCIL_DESC
*  @brief     easier DEPTH_STENCIL_DESC (This struct is derived from D3D12_DEPTH_STENCIL_DESC)
*****************************************************************************/
struct DEPTH_STENCIL_DESC : public D3D12_DEPTH_STENCIL_DESC
{
	DEPTH_STENCIL_DESC() {};
	explicit DEPTH_STENCIL_DESC(const D3D12_DEPTH_STENCIL_DESC& desc) :D3D12_DEPTH_STENCIL_DESC(desc) {};

	explicit DEPTH_STENCIL_DESC(NULL_STRUCT)
	{
		DepthEnable = TRUE;
		DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		StencilEnable = FALSE;
		StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
		StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
		const D3D12_DEPTH_STENCILOP_DESC defaultStencilOp =
		{ D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_STENCIL_OP_KEEP, D3D12_COMPARISON_FUNC_ALWAYS };
		FrontFace = defaultStencilOp;
		BackFace = defaultStencilOp;
	}

	explicit DEPTH_STENCIL_DESC(
		BOOL depthEnable,
		D3D12_DEPTH_WRITE_MASK depthWriteMask,
		D3D12_COMPARISON_FUNC  depthFunc,
		BOOL  stencilEnable,
		UINT8 stencilReadMask,
		UINT8 stencilWriteMask,
		D3D12_STENCIL_OP frontStencilFailOp,
		D3D12_STENCIL_OP frontStencilDepthFailOp,
		D3D12_STENCIL_OP frontStencilPassOp,
		D3D12_COMPARISON_FUNC frontStencilFunc,
		D3D12_STENCIL_OP backStencilFailOp,
		D3D12_STENCIL_OP backStencilDepthFailOp,
		D3D12_STENCIL_OP backStencilPassOp,
		D3D12_COMPARISON_FUNC backStencilFunc)
	{
		DepthEnable = depthEnable;
		DepthWriteMask = depthWriteMask;
		DepthFunc = depthFunc;
		StencilEnable = stencilEnable;
		StencilReadMask = stencilReadMask;
		StencilWriteMask = stencilWriteMask;
		FrontFace.StencilFailOp = frontStencilFailOp;
		FrontFace.StencilDepthFailOp = frontStencilDepthFailOp;
		FrontFace.StencilPassOp = frontStencilPassOp;
		FrontFace.StencilFunc = frontStencilFunc;
		BackFace.StencilFailOp = backStencilFailOp;
		BackFace.StencilDepthFailOp = backStencilDepthFailOp;
		BackFace.StencilPassOp = backStencilPassOp;
		BackFace.StencilFunc = backStencilFunc;
	}
	~DEPTH_STENCIL_DESC() {};
	operator const D3D12_DEPTH_STENCIL_DESC& () const { return *this; }
};
#pragma endregion   DepthStencil
#pragma region BlendDesc
/****************************************************************************
*					BLEND_DESC : public D3D12_BLEND_DESC
*************************************************************************//**
*  @struct    BLEND_DESC
*  @brief     easier BLEND_DESC (This struct is derived from D3D12_BLEND_DESC )
*****************************************************************************/
struct BLEND_DESC : public D3D12_BLEND_DESC
{
	BLEND_DESC() {};
	explicit BLEND_DESC(const D3D12_BLEND_DESC& desc) :D3D12_BLEND_DESC(desc) {};

	explicit BLEND_DESC(NULL_STRUCT)
	{
		AlphaToCoverageEnable  = FALSE;
		IndependentBlendEnable = FALSE;
		const D3D12_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc =
		{
			FALSE,FALSE,
			D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
			D3D12_BLEND_ONE, D3D12_BLEND_ZERO, D3D12_BLEND_OP_ADD,
			D3D12_LOGIC_OP_NOOP,
			D3D12_COLOR_WRITE_ENABLE_ALL,
		};
		for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
		{
			RenderTarget[i] = defaultRenderTargetBlendDesc;
		}
	}
};
#pragma endregion      BlendDesc
#pragma region RasterizerDesc
/****************************************************************************
*				RASTERIZER_DESC : public D3D12_RASTERIZER_DESC
*************************************************************************//**
*  @struct    RASTERIZER_DESC
*  @brief     easier RASTERIZER_DESC (This struct is derived from D3D12_RASTERIZER_DESC )
*****************************************************************************/
struct RASTERIZER_DESC : public D3D12_RASTERIZER_DESC
{
	RASTERIZER_DESC() {};
	explicit RASTERIZER_DESC(const D3D12_RASTERIZER_DESC& desc) :D3D12_RASTERIZER_DESC(desc) {};

	explicit RASTERIZER_DESC(NULL_STRUCT)
	{
		FillMode = D3D12_FILL_MODE_SOLID;
		CullMode = D3D12_CULL_MODE_BACK;
		FrontCounterClockwise = FALSE;
		DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
		DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
		SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
		DepthClipEnable = TRUE;
		MultisampleEnable = FALSE;
		AntialiasedLineEnable = FALSE;
		ForcedSampleCount = 0;
		ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
	}

	explicit RASTERIZER_DESC(
		D3D12_FILL_MODE fillMode,
		D3D12_CULL_MODE cullMode,
		BOOL frontCounterClockwise,
		INT depthBias,
		FLOAT depthBiasClamp,
		FLOAT slopeScaledDepthBias,
		BOOL depthClipEnable,
		BOOL multisampleEnable,
		BOOL antialiasedLineEnable,
		UINT forcedSampleCount,
		D3D12_CONSERVATIVE_RASTERIZATION_MODE conservativeRaster)
	{
		FillMode = fillMode;
		CullMode = cullMode;
		FrontCounterClockwise = frontCounterClockwise;
		DepthBias = depthBias;
		DepthBiasClamp = depthBiasClamp;
		SlopeScaledDepthBias = slopeScaledDepthBias;
		DepthClipEnable = depthClipEnable;
		MultisampleEnable = multisampleEnable;
		AntialiasedLineEnable = antialiasedLineEnable;
		ForcedSampleCount = forcedSampleCount;
		ConservativeRaster = conservativeRaster;
	}
	~RASTERIZER_DESC() {};
	operator const D3D12_RASTERIZER_DESC& () const { return *this; }
};
#pragma endregion RasterizerDesc
/****************************************************************************
*		RESOURCE_ALLOCATION_INFO : public D3D12_RESOURCE_ALLOCATION_INFO
*************************************************************************//**
*  @struct    RESOURCE_ALLOCATION_INFO
*  @brief     easier RESOURCE_ALLOCATION_INFO (This struct is derived from D3D12_RESOURCE_ALLOCATION_INFO )
*****************************************************************************/
struct RESOURCE_ALLOCATION_INFO : public D3D12_RESOURCE_ALLOCATION_INFO
{
	RESOURCE_ALLOCATION_INFO() {};
	explicit RESOURCE_ALLOCATION_INFO(const D3D12_RESOURCE_ALLOCATION_INFO& info) :D3D12_RESOURCE_ALLOCATION_INFO(info) {};

	RESOURCE_ALLOCATION_INFO(
		UINT64 size,
		UINT64 alignment)
	{
		SizeInBytes = size;
		Alignment = alignment;
	}
	operator const D3D12_RESOURCE_ALLOCATION_INFO& () const { return *this; }
};

#pragma region HeapProperty
/****************************************************************************
*			HEAP_PROPERTY : public D3D12_HEAP_PROPERTIES
*************************************************************************//**
*  @struct    HEAP_PROPERTY
*  @brief     easier Heap property (This struct is derived from D3D12_HEAP_PROPERTIES)
*****************************************************************************/
struct HEAP_PROPERTY : public D3D12_HEAP_PROPERTIES
{
	HEAP_PROPERTY() {};

	// Heap property 
	explicit HEAP_PROPERTY(const D3D12_HEAP_PROPERTIES& heapDesc)
		: D3D12_HEAP_PROPERTIES(heapDesc)
	{
	};

	// Heap type: custom
	HEAP_PROPERTY(
		D3D12_CPU_PAGE_PROPERTY cpuPageProperty,
		D3D12_MEMORY_POOL memoryPoolPreference,
		UINT creationNodeMask = 1,
		UINT nodeMask = 1)
	{
		Type = D3D12_HEAP_TYPE_CUSTOM;
		CPUPageProperty = cpuPageProperty;
		MemoryPoolPreference = memoryPoolPreference;
		CreationNodeMask = creationNodeMask;
		VisibleNodeMask = nodeMask;
	}

	// Heap type: Non-custom
	explicit HEAP_PROPERTY(
		D3D12_HEAP_TYPE type,
		UINT creationNodeMask = 1,
		UINT nodeMask = 1)
	{
		Type = type;
		CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		CreationNodeMask     = creationNodeMask;
		VisibleNodeMask      = nodeMask;
	}

	// Return oneself pointer (unary operator )
	operator const D3D12_HEAP_PROPERTIES& () const { return *this; }

	bool IsCPUAccessible() const
	{
		return Type == D3D12_HEAP_TYPE_UPLOAD || Type == D3D12_HEAP_TYPE_READBACK || (Type == D3D12_HEAP_TYPE_CUSTOM &&
			(CPUPageProperty == D3D12_CPU_PAGE_PROPERTY_WRITE_COMBINE || CPUPageProperty == D3D12_CPU_PAGE_PROPERTY_WRITE_BACK));
	}
};

// Equal
inline bool operator==(const D3D12_HEAP_PROPERTIES& l, const D3D12_HEAP_PROPERTIES& r)
{
	return l.Type == r.Type && l.CPUPageProperty == r.CPUPageProperty &&
		l.MemoryPoolPreference == r.MemoryPoolPreference &&
		l.CreationNodeMask == r.CreationNodeMask &&
		l.VisibleNodeMask == r.VisibleNodeMask;
}

// Not Equal
inline bool operator!=(const D3D12_HEAP_PROPERTIES& l, const D3D12_HEAP_PROPERTIES& r)
{
	return !(l == r);
}
#pragma endregion HeapProperty
#pragma region HeapDesc
/****************************************************************************
*					HEAP_DESC : public D3D12_HEAP_DESC
*************************************************************************//**
*  @struct    HEAP_DESC
*  @brief     easier Heap descriptor (This struct is derived from D3D12_HEAP_DESC)
*****************************************************************************/
struct HEAP_DESC : public D3D12_HEAP_DESC
{
	HEAP_DESC() {};
	explicit HEAP_DESC(const D3D12_HEAP_DESC& heapDesc) :D3D12_HEAP_DESC(heapDesc) {};

	HEAP_DESC(
		UINT64 sizeInBytes,
		D3D12_HEAP_PROPERTIES heapProperties,
		UINT64 alignment = 0,
		D3D12_HEAP_FLAGS flags = D3D12_HEAP_FLAG_NONE)
	{
		SizeInBytes = sizeInBytes;
		Properties = heapProperties;
		Alignment = alignment;
		Flags = flags;
	}

	// The folowing constructors are used to make small changes tp the Heap Property 
	// Use HEAP_PROPERTY pointer
	HEAP_DESC(
		UINT64 sizeInBytes,
		D3D12_HEAP_TYPE type,
		UINT64 alignment = 0,
		D3D12_HEAP_FLAGS flags = D3D12_HEAP_FLAG_NONE)
	{
		SizeInBytes = sizeInBytes;
		Properties = HEAP_PROPERTY(type);
		Alignment = alignment;
		Flags = flags;
	}

	HEAP_DESC(
		UINT64 sizeInBytes,
		D3D12_CPU_PAGE_PROPERTY cpuPageProperty,
		D3D12_MEMORY_POOL memoryPoolPreference,
		UINT64 alignment = 0,
		D3D12_HEAP_FLAGS flags = D3D12_HEAP_FLAG_NONE)
	{
		SizeInBytes = sizeInBytes;
		Properties = HEAP_PROPERTY(cpuPageProperty, memoryPoolPreference);
		Alignment = alignment;
		Flags = flags;
	}

	HEAP_DESC(
		const D3D12_RESOURCE_ALLOCATION_INFO& resAllocInfo,
		D3D12_HEAP_PROPERTIES properties,
		D3D12_HEAP_FLAGS flags = D3D12_HEAP_FLAG_NONE)
	{
		SizeInBytes = resAllocInfo.SizeInBytes;
		Properties = properties;
		Alignment = resAllocInfo.Alignment;
		Flags = flags;
	}
	HEAP_DESC(
		const D3D12_RESOURCE_ALLOCATION_INFO& resAllocInfo,
		D3D12_HEAP_TYPE type,
		D3D12_HEAP_FLAGS flags = D3D12_HEAP_FLAG_NONE)
	{
		SizeInBytes = resAllocInfo.SizeInBytes;
		Properties = HEAP_PROPERTY(type);
		Alignment = resAllocInfo.Alignment;
		Flags = flags;
	}

	HEAP_DESC(
		const D3D12_RESOURCE_ALLOCATION_INFO& resAllocInfo,
		D3D12_CPU_PAGE_PROPERTY cpuPageProperty,
		D3D12_MEMORY_POOL memoryPoolPreference,
		D3D12_HEAP_FLAGS flags = D3D12_HEAP_FLAG_NONE)
	{
		SizeInBytes = resAllocInfo.SizeInBytes;
		Properties = HEAP_PROPERTY(cpuPageProperty, memoryPoolPreference);
		Alignment = resAllocInfo.Alignment;
		Flags = flags;
	}
};

inline bool operator==(const D3D12_HEAP_DESC& l, const D3D12_HEAP_DESC& r)
{
	return l.SizeInBytes == r.SizeInBytes &&
		l.Properties == r.Properties &&
		l.Alignment == r.Alignment &&
		l.Flags == r.Flags;
}
inline bool operator!=(const D3D12_HEAP_DESC& l, const D3D12_HEAP_DESC& r)
{
	return !(l == r);
}
#pragma endregion HeapDesc
//////////////////////////////////////////////////////////////////////////////////
//               D3D12GetFormatPlaneCount
//////////////////////////////////////////////////////////////////////////////////
inline UINT8 D3D12GetFormatPlaneCount(_In_ IDevice* device, DXGI_FORMAT format)
{
	D3D12_FEATURE_DATA_FORMAT_INFO formatInfo = { format };
	if (FAILED(device->CheckFeatureSupport(D3D12_FEATURE_FORMAT_INFO, &formatInfo, sizeof(formatInfo))))
	{
		return 0;
	}
	return formatInfo.PlaneCount;
}

//////////////////////////////////////////////////////////////////////////////////
//                 D3D12CalcSubresource
//////////////////////////////////////////////////////////////////////////////////
inline UINT D3D12CalcSubresource(UINT mipSlice, UINT arraySlice, UINT planeSlice, UINT mipLevels, UINT arraySize)
{
	return mipSlice + arraySlice * mipLevels + planeSlice * mipLevels * arraySize;
}

/****************************************************************************
*					RESOURCE_DESC : D3D12_RESOURCE_DESC
*************************************************************************//**
*  @struct    RESOURCE_DESC
*  @brief     easier Resource descriptor (This struct is derived from DD3D12_RESOURCE_DESC)
*****************************************************************************/
struct RESOURCE_DESC : public D3D12_RESOURCE_DESC
{
	RESOURCE_DESC() {};
	explicit RESOURCE_DESC(const D3D12_RESOURCE_DESC& resourceDesc) :D3D12_RESOURCE_DESC(resourceDesc) {};

	RESOURCE_DESC(
		D3D12_RESOURCE_DIMENSION dimension,
		UINT64 alignment,
		UINT64 width,
		UINT   height,
		UINT16 depthOrArraySize,
		UINT16 mipLevels,
		DXGI_FORMAT format,
		UINT sampleCount,
		UINT sampleQuality,
		D3D12_TEXTURE_LAYOUT layout,
		D3D12_RESOURCE_FLAGS flags)
	{
		Dimension = dimension;
		Alignment = alignment;
		Width = width;
		Height = height;
		DepthOrArraySize = depthOrArraySize;
		MipLevels = mipLevels;
		Format = format;
		SampleDesc.Count = sampleCount;
		SampleDesc.Quality = sampleQuality;
		Layout = layout;
		Flags = flags;
	}

	static inline RESOURCE_DESC Buffer(
		const D3D12_RESOURCE_ALLOCATION_INFO& resAllocationInfo,
		D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE)
	{
		// width only
		return RESOURCE_DESC(D3D12_RESOURCE_DIMENSION_BUFFER, resAllocationInfo.Alignment, resAllocationInfo.SizeInBytes,
			1, 1, 1, DXGI_FORMAT_UNKNOWN, 1, 0, D3D12_TEXTURE_LAYOUT_ROW_MAJOR, flags);
	}

	static inline RESOURCE_DESC Buffer(
		UINT64 width,
		D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE,
		UINT64 alignment = 0)
	{
		return RESOURCE_DESC(D3D12_RESOURCE_DIMENSION_BUFFER, alignment, width, 1, 1, 1,
			DXGI_FORMAT_UNKNOWN, 1, 0, D3D12_TEXTURE_LAYOUT_ROW_MAJOR, flags);
	}

	// 1D Texture
	static inline RESOURCE_DESC Texture1D(
		DXGI_FORMAT format,
		UINT64 width,
		UINT16 arraySize = 1,
		UINT16 mipLevels = 0,
		D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE,
		D3D12_TEXTURE_LAYOUT layout = D3D12_TEXTURE_LAYOUT_UNKNOWN,
		UINT64 alignment = 0)
	{
		return RESOURCE_DESC(D3D12_RESOURCE_DIMENSION_TEXTURE1D, alignment, width, 1, arraySize,
			mipLevels, format, 1, 0, layout, flags);
	}

	// 2D Texture
	static inline RESOURCE_DESC Texture2D(
		DXGI_FORMAT format,
		UINT64 width,
		UINT height,
		UINT16 arraySize = 1,
		UINT16 mipLevels = 0,
		UINT sampleCount = 1,
		UINT sampleQuality = 0,
		D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE,
		D3D12_TEXTURE_LAYOUT layout = D3D12_TEXTURE_LAYOUT_UNKNOWN,
		UINT64 alignment = 0)
	{
		return RESOURCE_DESC(D3D12_RESOURCE_DIMENSION_TEXTURE2D, alignment, width, height, arraySize,
			mipLevels, format, sampleCount, sampleQuality, layout, flags);
	}

	// 3D Texture
	static inline RESOURCE_DESC Texture3D(
		DXGI_FORMAT format,
		UINT64 width,
		UINT height,
		UINT16 depth,
		UINT16 mipLevels = 0,
		D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE,
		D3D12_TEXTURE_LAYOUT layout = D3D12_TEXTURE_LAYOUT_UNKNOWN,
		UINT64 alignment = 0)
	{
		return RESOURCE_DESC(D3D12_RESOURCE_DIMENSION_TEXTURE3D, alignment, width, height, depth,
			mipLevels, format, 1, 0, layout, flags);
	}

	operator const D3D12_RESOURCE_DESC& () const { return *this; }

	inline UINT16 Depth() const
	{
		return (Dimension == D3D12_RESOURCE_DIMENSION_TEXTURE3D ? DepthOrArraySize : 1);
	}

	inline UINT16 ArraySize() const
	{
		return (Dimension != D3D12_RESOURCE_DIMENSION_TEXTURE3D ? DepthOrArraySize : 1);
	}

	inline UINT8 PlaneCount(_In_ IDevice* device) const
	{
		return D3D12GetFormatPlaneCount(device, Format);
	}

	inline UINT Subresource(_In_ IDevice* device) const
	{
		return MipLevels * ArraySize() * PlaneCount(device);
	}

	inline UINT CalcSubresources(UINT mipSlice, UINT arraySlice, UINT planeSlice)
	{
		return D3D12CalcSubresource(mipSlice, arraySlice, planeSlice, MipLevels, ArraySize());
	}
};

inline bool operator==(const D3D12_RESOURCE_DESC& l, const D3D12_RESOURCE_DESC& r)
{
	return l.Dimension == r.Dimension &&
		l.Alignment == r.Alignment &&
		l.Width == r.Width &&
		l.Height == r.Height &&
		l.DepthOrArraySize == r.DepthOrArraySize &&
		l.MipLevels == r.MipLevels &&
		l.Format == r.Format &&
		l.SampleDesc.Count == r.SampleDesc.Count &&
		l.SampleDesc.Quality == r.SampleDesc.Quality &&
		l.Layout == r.Layout &&
		l.Flags == r.Flags;
}

inline bool operator!=(const D3D12_RESOURCE_DESC& l, const D3D12_RESOURCE_DESC& r)
{
	return !(l == r);
}

inline void MemcpySubresource(
	_In_ const D3D12_MEMCPY_DEST* pDest,
	_In_ const D3D12_SUBRESOURCE_DATA* pSrc,
	SIZE_T RowSizeInBytes,
	UINT NumRows,
	UINT NumSlices)
{
	for (UINT z = 0; z < NumSlices; ++z)
	{
		BYTE* pDestSlice = reinterpret_cast<BYTE*>(pDest->pData) + pDest->SlicePitch * z;
		const BYTE* pSrcSlice = reinterpret_cast<const BYTE*>(pSrc->pData) + pSrc->SlicePitch * z;
		for (UINT y = 0; y < NumRows; ++y)
		{
			memcpy(pDestSlice + pDest->RowPitch * y,
				pSrcSlice + pSrc->RowPitch * y,
				RowSizeInBytes);
		}
	}
}

inline UINT64 GetRequiredIntermediateSize(
	_In_ ID3D12Resource* pDestinationResource,
	_In_range_(0, D3D12_REQ_SUBRESOURCES) UINT FirstSubresource,
	_In_range_(0, D3D12_REQ_SUBRESOURCES - FirstSubresource) UINT NumSubresources)
{
	D3D12_RESOURCE_DESC Desc = pDestinationResource->GetDesc();
	UINT64 RequiredSize = 0;

	ID3D12Device* pDevice = nullptr;
	pDestinationResource->GetDevice(__uuidof(*pDevice), reinterpret_cast<void**>(&pDevice));
	pDevice->GetCopyableFootprints(&Desc, FirstSubresource, NumSubresources, 0, nullptr, nullptr, nullptr, &RequiredSize);
	pDevice->Release();

	return RequiredSize;
}
//////////////////////////////////////////////////////////////////////////////////
//            TEXTURE_COPY_LOCATION : D3D12_TEXTURE_COPY_LOCATION
//////////////////////////////////////////////////////////////////////////////////
struct TEXTURE_COPY_LOCATION : public D3D12_TEXTURE_COPY_LOCATION
{
	TEXTURE_COPY_LOCATION() {};
	explicit TEXTURE_COPY_LOCATION(const D3D12_TEXTURE_COPY_LOCATION& location) : D3D12_TEXTURE_COPY_LOCATION(location) {};

	TEXTURE_COPY_LOCATION(Resource* resource) { pResource = resource; }
	TEXTURE_COPY_LOCATION(Resource* resource, D3D12_PLACED_SUBRESOURCE_FOOTPRINT const& footPrint)
	{
		pResource = resource;
		Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
		PlacedFootprint = footPrint;
	}
	TEXTURE_COPY_LOCATION(Resource* resource, UINT sub)
	{
		pResource = resource;
		Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
		SubresourceIndex = sub;
	}
};


//////////////////////////////////////////////////////////////////////////////////
//               Function for Texture  
//////////////////////////////////////////////////////////////////////////////////
/****************************************************************************
**                    UpdateSubResources
*****************************************************************************/
// All arrays must be populated (e.g. by calling GetCopyableFootprints)
inline UINT64 UpdateSubresources(
	_In_ ICommandList* commandList,
	_In_ Resource* destinationResource,
	_In_ Resource* intermediate,
	_In_range_(0, D3D12_REQ_SUBRESOURCES) UINT firstSubresource,
	_In_range_(0, D3D12_REQ_SUBRESOURCES - firstSubresource) UINT numSubresources,
	UINT64 requiredSize,
	_In_reads_(numSubresources) const D3D12_PLACED_SUBRESOURCE_FOOTPRINT* layouts,
	_In_reads_(numSubresources) const UINT* numRows,
	_In_reads_(numSubresources) const UINT64* rowSizesInBytes,
	_In_reads_(numSubresources) const D3D12_SUBRESOURCE_DATA* sourceData)
{
	D3D12_RESOURCE_DESC intermediateDesc = intermediate->GetDesc();
	D3D12_RESOURCE_DESC destinationDesc = destinationResource->GetDesc();
	if (intermediateDesc.Dimension != D3D12_RESOURCE_DIMENSION_BUFFER ||
		intermediateDesc.Width      < requiredSize + layouts[0].Offset ||
		requiredSize >(SIZE_T) - 1 ||
		(destinationDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER &&
		(firstSubresource != 0 || numSubresources != 1)))
	{
		return 0;
	}

	BYTE* data = nullptr;
	HRESULT hresult = intermediate->Map(0, NULL, reinterpret_cast<void**>(&data));
	if (FAILED(hresult))
	{
		return 0;
	}

	for (UINT i = 0; i < numSubresources; ++i)
	{
		if (rowSizesInBytes[i] > (SIZE_T)-1) return 0;
		D3D12_MEMCPY_DEST DestData = { data + layouts[i].Offset, layouts[i].Footprint.RowPitch, (UINT64)layouts[i].Footprint.RowPitch * numRows[i] };
		MemcpySubresource(&DestData, &sourceData[i], (SIZE_T)rowSizesInBytes[i], numRows[i], layouts[i].Footprint.Depth);
	}
	intermediate->Unmap(0, NULL);

	if (destinationDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER)
	{
		DIRECTX12_BOX sourceBox(UINT(layouts[0].Offset), UINT(layouts[0].Offset + layouts[0].Footprint.Width));
		commandList->CopyBufferRegion(
			destinationResource, 0, intermediate, layouts[0].Offset, layouts[0].Footprint.Width);
	}
	else
	{
		for (UINT i = 0; i < numSubresources; ++i)
		{
			TEXTURE_COPY_LOCATION Dst(destinationResource, i + firstSubresource);
			TEXTURE_COPY_LOCATION Src(intermediate, layouts[i]);
			commandList->CopyTextureRegion(&Dst, 0, 0, 0, &Src, nullptr);
		}
	}
	return requiredSize;
}

/****************************************************************************
**                    UpdateSubResources
*****************************************************************************/
// Heap - allocating UpdateSubresources implementation
inline UINT64 UpdateSubresources(
	_In_ ICommandList* commandList,
	_In_ Resource* destinationResource,
	_In_ Resource* intermediate,
	UINT64 intermediateOffset,
	_In_range_(0, D3D12_REQ_SUBRESOURCES) UINT firstSubresource,
	_In_range_(0, D3D12_REQ_SUBRESOURCES - firstSubresource) UINT numSubresources,
	_In_reads_(numSubresources) D3D12_SUBRESOURCE_DATA* sourceData)
{
	UINT64 requiredSize = 0;
	UINT64 memoryToAllocation = static_cast<UINT64>(sizeof(D3D12_PLACED_SUBRESOURCE_FOOTPRINT) + sizeof(UINT) + sizeof(UINT64)) * numSubresources;
	if (memoryToAllocation > SIZE_MAX) { return 0; }

	void* memory = HeapAlloc(GetProcessHeap(), 0, static_cast<SIZE_T>(memoryToAllocation));
	if (memory == NULL) { return 0; }

	D3D12_PLACED_SUBRESOURCE_FOOTPRINT* layouts = reinterpret_cast<D3D12_PLACED_SUBRESOURCE_FOOTPRINT*>(memory);
	UINT64* rowSizesInBytes = reinterpret_cast<UINT64*>(layouts + numSubresources);
	UINT* numRows = reinterpret_cast<UINT*>(rowSizesInBytes + numSubresources);

	D3D12_RESOURCE_DESC resourceDesc = destinationResource->GetDesc();
	IDevice* device = nullptr;
	destinationResource->GetDevice(__uuidof(*device), reinterpret_cast<void**>(&device));
	device->GetCopyableFootprints(&resourceDesc, firstSubresource, numSubresources, intermediateOffset, layouts, numRows, rowSizesInBytes, &requiredSize);
	device->Release();

	UINT64 result = UpdateSubresources(commandList, destinationResource, intermediate, firstSubresource, numSubresources, requiredSize, layouts, numRows, rowSizesInBytes, sourceData);
	HeapFree(GetProcessHeap(), 0, memory);
	return result;
}

/****************************************************************************
**                    UpdateSubResources
*****************************************************************************/
template <UINT maxSubresources>
inline UINT64 UpdateSubresources(
	_In_ ICommandList* commandList,
	_In_ Resource* destinationResource,
	_In_ Resource* intermediate,
	UINT64 intermediateOffset,
	_In_range_(0, maxSubresources) UINT firstSubresource,
	_In_range_(1, maxSubresources - firstSubresource) UINT numSubresources,
	_In_reads_(numSubresources) D3D12_SUBRESOURCE_DATA* sourceData)
{
	UINT64 requiredSize = 0;
	UINT   rows[maxSubresources] = {};
	UINT64 rowSizesInBytes[maxSubresources] = {};
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT layouts[maxSubresources] = {};

	D3D12_RESOURCE_DESC resourceDesc = destinationResource->GetDesc();
	Device* device = nullptr;
	destinationResource->GetDevice(__uuidof(*device), reinterpret_cast<void**>(&device));
	device->GetCopyableFootprints(&resourceDesc, firstSubresource, numSubresources, intermediateOffset, layouts, rows, rowSizesInBytes, &requiredSize);
	device->Release();

	return UpdateSubresources(commandList, destinationResource, intermediate, firstSubresource, numSubresources, requiredSize, layouts, rows, rowSizesInBytes, sourceData);
}
/****************************************************************************
*					BARRIER : public D3D12_RESOURCE_BARRIER
*************************************************************************//**
*  @struct    BARRIER
*  @brief     easier Barrier (This struct is derived from D3D12_RESOURCE_BARRIER)
*             Transition, Aliasing, Unordered Access View
*****************************************************************************/
struct BARRIER : public D3D12_RESOURCE_BARRIER
{
	BARRIER() {};
	explicit BARRIER(const D3D12_RESOURCE_BARRIER& barrier) :D3D12_RESOURCE_BARRIER(barrier) {};

	/****************************************************************************
	**                    Transition
	*****************************************************************************/
	static inline BARRIER Transition(
		_In_ ID3D12Resource* pResource,
		D3D12_RESOURCE_STATES before,
		D3D12_RESOURCE_STATES after,
		UINT subResource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
		D3D12_RESOURCE_BARRIER_FLAGS flags = D3D12_RESOURCE_BARRIER_FLAG_NONE)
	{
		BARRIER result;
		ZeroMemory(&result, sizeof(result));
		D3D12_RESOURCE_BARRIER& barrier = result;

		result.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		result.Flags = flags;

		barrier.Transition.pResource = pResource;
		barrier.Transition.StateBefore = before;
		barrier.Transition.StateAfter = after;
		barrier.Transition.Subresource = subResource;
		return result;
	}

	/****************************************************************************
	**                    Aliasing
	*****************************************************************************/
	static inline BARRIER Aliasing(
		_In_ ID3D12Resource* before,
		_In_ ID3D12Resource* after)
	{
		BARRIER result;
		ZeroMemory(&result, sizeof(result));

		D3D12_RESOURCE_BARRIER& barrier = result;
		result.Type = D3D12_RESOURCE_BARRIER_TYPE_ALIASING;

		barrier.Aliasing.pResourceBefore = before;
		barrier.Aliasing.pResourceAfter = after;
		return result;
	}

	/****************************************************************************
	**                    Unordered Access View (UAV)
	*****************************************************************************/
	static inline BARRIER UAV(
		_In_ ID3D12Resource* pResource)
	{
		BARRIER result;
		ZeroMemory(&result, sizeof(result));

		D3D12_RESOURCE_BARRIER& barrier = result;
		result.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;

		barrier.UAV.pResource = pResource;
		return result;
	}

	operator const D3D12_RESOURCE_BARRIER& () const { return *this; }
};

/****************************************************************************
*				DESCRIPTOR_RANGE : public D3D12_DESCRIPTOR_RANGE
*************************************************************************//**
*  @struct    DESCRIPTOR_RANGE
*  @brief     Easier descriptor range (This struct is derived from D3D12_DESCRIPTOR_RANGE)
*****************************************************************************/
struct DESCRIPTOR_RANGE : public D3D12_DESCRIPTOR_RANGE
{
	DESCRIPTOR_RANGE() {};
	explicit DESCRIPTOR_RANGE(const D3D12_DESCRIPTOR_RANGE& range) : D3D12_DESCRIPTOR_RANGE(range) {};

	DESCRIPTOR_RANGE(
		D3D12_DESCRIPTOR_RANGE_TYPE rangeType,
		UINT numDescriptors,
		UINT baseShaderRegister,
		UINT registerSpace = 0,
		UINT offsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND)
	{
		Init(rangeType, numDescriptors, baseShaderRegister, registerSpace, offsetInDescriptorsFromTableStart);
	}

	inline void Init(
		D3D12_DESCRIPTOR_RANGE_TYPE rangeType,
		UINT numDescriptors,
		UINT baseShaderRegister,
		UINT registerSpace = 0,
		UINT offsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND)
	{
		Init(*this, rangeType, numDescriptors, baseShaderRegister, registerSpace, offsetInDescriptorsFromTableStart);
	}

	static inline void Init(
		_Out_ D3D12_DESCRIPTOR_RANGE& range,
		D3D12_DESCRIPTOR_RANGE_TYPE rangeType,
		UINT numDescriptors,
		UINT baseSharderRegister,
		UINT registerSpace = 0,
		UINT offsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND)
	{
		range.RangeType = rangeType;
		range.NumDescriptors = numDescriptors;
		range.BaseShaderRegister = baseSharderRegister;
		range.RegisterSpace = registerSpace;
		range.OffsetInDescriptorsFromTableStart = offsetInDescriptorsFromTableStart;
	}
};

/****************************************************************************
*				ROOT_DESCRIPTOR_TABLE : public D3D12_ROOT_DESCRIPTOR_TABLE
*************************************************************************//**
*  @struct    ROOT_DESCRIPTOR_TABLE
*  @brief     Easier RootSignature desicriptor table(This struct is derived from D3D12_ROOT_DESCRIPTOR_TABLE)
*****************************************************************************/
struct ROOT_DESCRIPTOR_TABLE : public D3D12_ROOT_DESCRIPTOR_TABLE
{
	ROOT_DESCRIPTOR_TABLE() {};
	explicit ROOT_DESCRIPTOR_TABLE(const D3D12_ROOT_DESCRIPTOR_TABLE& table) : D3D12_ROOT_DESCRIPTOR_TABLE(table) {};

	ROOT_DESCRIPTOR_TABLE(
		UINT numDescriptorRanges,
		_In_reads_opt_(numDescriptorRanges) const D3D12_DESCRIPTOR_RANGE* descriptorRanges)
	{
		// nullptr check
		if (descriptorRanges != nullptr)
		{
			Init(numDescriptorRanges, descriptorRanges);
		}
	}

	inline void Init(
		UINT numDescriptorRanges,
		_In_reads_(numDescriptorRanges) const D3D12_DESCRIPTOR_RANGE* descriptorRanges)
	{
		Init(*this, numDescriptorRanges, descriptorRanges);
	}

	static inline void Init(
		_Out_ D3D12_ROOT_DESCRIPTOR_TABLE& rootDescriptorTable,
		UINT numDescriptorRanges,
		_In_reads_opt_(numDescriptorRanges) const D3D12_DESCRIPTOR_RANGE* descriptorRanges)
	{
		rootDescriptorTable.NumDescriptorRanges = numDescriptorRanges;
		rootDescriptorTable.pDescriptorRanges = descriptorRanges;
	}
};

/****************************************************************************
*				ROOT_CONSTANTS : public D3D12_ROOT_CONSTANTS
*************************************************************************//**
*  @struct    ROOT_CONSTANTS
*  @brief     Easier RootSignature Constant(This struct is derived from D3D12_ROOT_CONSTANTS)
*****************************************************************************/
struct ROOT_CONSTANTS : public D3D12_ROOT_CONSTANTS
{
	ROOT_CONSTANTS() {};
	explicit ROOT_CONSTANTS(const D3D12_ROOT_CONSTANTS& constants) : D3D12_ROOT_CONSTANTS(constants) {};

	ROOT_CONSTANTS(
		UINT num32BitValues,
		UINT shaderRegister,
		UINT registerSpace = 0)
	{
		Init(num32BitValues, shaderRegister, registerSpace);
	}

	inline void Init(
		UINT num32BitValues,
		UINT shaderRegister,
		UINT registerSpace = 0)
	{
		Init(*this, num32BitValues, shaderRegister, registerSpace);
	}

	static inline void Init(
		_Out_ D3D12_ROOT_CONSTANTS& rootConstants,
		UINT num32BitValues,
		UINT shaderRegister,
		UINT registerSpace = 0)
	{
		rootConstants.Num32BitValues = num32BitValues;
		rootConstants.ShaderRegister = shaderRegister;
		rootConstants.RegisterSpace = registerSpace;
	}
};

/****************************************************************************
*				ROOT_DESCRIPTOR : public D3D12_ROOT_DESCRIPTOR
*************************************************************************//**
*  @struct    ROOT_DESCRIPTOR
*  @brief     Easier RootSignature Discriptor(This struct is derived from D3D12_ROOT_DESCRIPTOR)
*****************************************************************************/
struct ROOT_DESCRIPTOR : public D3D12_ROOT_DESCRIPTOR
{
	ROOT_DESCRIPTOR() {};
	explicit ROOT_DESCRIPTOR(const ROOT_DESCRIPTOR& desc) : D3D12_ROOT_DESCRIPTOR(desc) {};

	ROOT_DESCRIPTOR(
		UINT shaderRegister,
		UINT registerSpace = 0)
	{
		Init(shaderRegister, registerSpace);
	}

	inline void Init(
		UINT shaderRegister,
		UINT registerSpace = 0)
	{
		Init(*this, shaderRegister, registerSpace);
	}

	static inline void Init(_Out_ D3D12_ROOT_DESCRIPTOR& table, UINT shaderRegister, UINT registerSpace = 0)
	{
		table.ShaderRegister = shaderRegister;
		table.RegisterSpace = registerSpace;
	}
};

/****************************************************************************
*				ROOT_PARAMETER : public D3D12_ROOT_PARAMETER
*************************************************************************//**
*  @struct    ROOT_PARAMETER
*  @brief     Easier RootSignature Discriptor(This struct is derived from D3D12_ROOT_PARAMETER)
*****************************************************************************/
struct ROOT_PARAMETER : public D3D12_ROOT_PARAMETER
{
	ROOT_PARAMETER() {};
	explicit ROOT_PARAMETER(const D3D12_ROOT_PARAMETER& rootParam) : D3D12_ROOT_PARAMETER(rootParam) {};

	static inline void InitAsDescriptorTable(
		_Out_ D3D12_ROOT_PARAMETER& rootParameter,
		UINT numDescriptorRanges,
		_In_reads_(numDescriptorRanges) const D3D12_DESCRIPTOR_RANGE* descriptorRanges,
		D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		rootParameter.ParameterType    = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		rootParameter.ShaderVisibility = visibility;
		ROOT_DESCRIPTOR_TABLE::Init(rootParameter.DescriptorTable, numDescriptorRanges, descriptorRanges);
	}

	static inline void InitAsConstants(
		_Out_ D3D12_ROOT_PARAMETER& rootParameter,
		UINT num32BitValues,
		UINT shaderRegister,
		UINT registerSpace = 0,
		D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		rootParameter.ParameterType    = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rootParameter.ShaderVisibility = visibility;
		ROOT_CONSTANTS::Init(rootParameter.Constants, num32BitValues, shaderRegister, registerSpace);
	}

	static inline void InitAsConstantBufferView(
		_Out_ D3D12_ROOT_PARAMETER& rootParameter,
		UINT shaderRegister,
		UINT registerSpace = 0,
		D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		rootParameter.ParameterType    = D3D12_ROOT_PARAMETER_TYPE_CBV;
		rootParameter.ShaderVisibility = visibility;
		ROOT_DESCRIPTOR::Init(rootParameter.Descriptor, shaderRegister, registerSpace);
	}

	static inline void InitAsShaderResourceView(
		_Out_ D3D12_ROOT_PARAMETER& rootParameter,
		UINT shaderRegister,
		UINT registerSpace = 0,
		D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		rootParameter.ParameterType    = D3D12_ROOT_PARAMETER_TYPE_SRV;
		rootParameter.ShaderVisibility = visibility;
		ROOT_DESCRIPTOR::Init(rootParameter.Descriptor, shaderRegister, registerSpace);
	}

	static inline void InitAsUnorderedAccessView(
		_Out_ D3D12_ROOT_PARAMETER& rootParameter,
		UINT shaderRegister,
		UINT registerSpace = 0,
		D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		rootParameter.ParameterType    = D3D12_ROOT_PARAMETER_TYPE_UAV;
		rootParameter.ShaderVisibility = visibility;
		ROOT_DESCRIPTOR::Init(rootParameter.Descriptor, shaderRegister, registerSpace);
	}

	inline void InitAsDescriptorTable(
		UINT numDescriptorRanges,
		_In_reads_(numDescriptorRanges) const D3D12_DESCRIPTOR_RANGE* descriptorRanges,
		D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		InitAsDescriptorTable(*this, numDescriptorRanges, descriptorRanges, visibility);
	}

	inline void InitAsConstants(
		UINT num32BitValues,
		UINT shaderRegister,
		UINT registerSpace = 0,
		D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		InitAsConstants(*this, num32BitValues, shaderRegister, registerSpace, visibility);
	}

	inline void InitAsConstantBufferView(
		UINT shaderRegister,
		UINT registerSpace = 0,
		D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		InitAsConstantBufferView(*this, shaderRegister, registerSpace, visibility);
	}

	inline void InitAsShaderResourceView(
		UINT shaderRegister,
		UINT registerSpace = 0,
		D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		InitAsShaderResourceView(*this, shaderRegister, registerSpace, visibility);
	}

	inline void InitAsUnorderedAccessView(
		UINT shaderRegister,
		UINT registerSpace = 0,
		D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL)
	{
		InitAsUnorderedAccessView(*this, shaderRegister, registerSpace, visibility);
	}
};

/****************************************************************************
*			STATIC_SAMPLER_DESC : public D3D12_STATIC_SAMPLER_DESC
*************************************************************************//**
*  @struct    STATIC_SAMPLER_DESC
*  @brief     Easier Sampler (This struct is derived from D3D12_STATIC_SAMPLER_DESC)
*****************************************************************************/
struct STATIC_SAMPLER_DESC : public D3D12_STATIC_SAMPLER_DESC
{
	STATIC_SAMPLER_DESC() {};
	explicit STATIC_SAMPLER_DESC(const D3D12_STATIC_SAMPLER_DESC& desc) :D3D12_STATIC_SAMPLER_DESC(desc) {};

	STATIC_SAMPLER_DESC(
		UINT shaderRegister,
		D3D12_FILTER filter = D3D12_FILTER_ANISOTROPIC,
		D3D12_TEXTURE_ADDRESS_MODE addressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE addressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE addressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		FLOAT mipLODBias = 0,
		UINT maxAnisotropy = 16,
		D3D12_COMPARISON_FUNC comparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL,
		D3D12_STATIC_BORDER_COLOR borderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE,
		FLOAT minLOD = 0.f,
		FLOAT maxLOD = D3D12_FLOAT32_MAX,
		D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL,
		UINT registerSpace = 0)
	{
		Init(
			shaderRegister,
			filter,
			addressU,
			addressV,
			addressW,
			mipLODBias,
			maxAnisotropy,
			comparisonFunc,
			borderColor,
			minLOD,
			maxLOD,
			shaderVisibility,
			registerSpace);
	}

	static inline void Init(
		_Out_ D3D12_STATIC_SAMPLER_DESC& samplerDesc,
		UINT shaderRegister,
		D3D12_FILTER filter = D3D12_FILTER_ANISOTROPIC,
		D3D12_TEXTURE_ADDRESS_MODE addressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE addressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE addressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		FLOAT mipLODBias = 0,
		UINT maxAnisotropy = 16,
		D3D12_COMPARISON_FUNC comparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL,
		D3D12_STATIC_BORDER_COLOR borderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE,
		FLOAT minLOD = 0.f,
		FLOAT maxLOD = D3D12_FLOAT32_MAX,
		D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL,
		UINT registerSpace = 0)
	{
		samplerDesc.ShaderRegister = shaderRegister;
		samplerDesc.Filter = filter;
		samplerDesc.AddressU = addressU;
		samplerDesc.AddressV = addressV;
		samplerDesc.AddressW = addressW;
		samplerDesc.MipLODBias = mipLODBias;
		samplerDesc.MaxAnisotropy = maxAnisotropy;
		samplerDesc.ComparisonFunc = comparisonFunc;
		samplerDesc.BorderColor = borderColor;
		samplerDesc.MinLOD = minLOD;
		samplerDesc.MaxLOD = maxLOD;
		samplerDesc.ShaderVisibility = shaderVisibility;
		samplerDesc.RegisterSpace = registerSpace;
	}

	inline void Init(
		UINT shaderRegister,
		D3D12_FILTER filter = D3D12_FILTER_ANISOTROPIC,
		D3D12_TEXTURE_ADDRESS_MODE addressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE addressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		D3D12_TEXTURE_ADDRESS_MODE addressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP,
		FLOAT mipLODBias = 0,
		UINT maxAnisotropy = 16,
		D3D12_COMPARISON_FUNC comparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL,
		D3D12_STATIC_BORDER_COLOR borderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE,
		FLOAT minLOD = 0.f,
		FLOAT maxLOD = D3D12_FLOAT32_MAX,
		D3D12_SHADER_VISIBILITY shaderVisibility = D3D12_SHADER_VISIBILITY_ALL,
		UINT registerSpace = 0)
	{
		Init(
			*this,
			shaderRegister,
			filter,
			addressU,
			addressV,
			addressW,
			mipLODBias,
			maxAnisotropy,
			comparisonFunc,
			borderColor,
			minLOD,
			maxLOD,
			shaderVisibility,
			registerSpace);
	}

};

/****************************************************************************
*			ROOT_SIGNATURE_DESC : public D3D12_ROOT_SIGNATURE_DESC
*************************************************************************//**
*  @struct    ROOT_SIGNATURE_DESC
*  @brief     Easier RootSignature Descriptor (This struct is derived from D3D12_ROOT_SIGNATURE_DESC)
*****************************************************************************/
struct ROOT_SIGNATURE_DESC : public D3D12_ROOT_SIGNATURE_DESC
{
	ROOT_SIGNATURE_DESC() {};
	explicit ROOT_SIGNATURE_DESC(const D3D12_ROOT_SIGNATURE_DESC& desc) :D3D12_ROOT_SIGNATURE_DESC(desc) {};

	ROOT_SIGNATURE_DESC(
		UINT numParameters,
		_In_reads_opt_(numParameters) const D3D12_ROOT_PARAMETER* parameters,
		UINT numStaticSamplers = 0,
		_In_reads_opt_(numStaticSamplers) const D3D12_STATIC_SAMPLER_DESC* staticSamplers = NULL,
		D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_NONE)
	{
		Init(numParameters, parameters, numStaticSamplers, staticSamplers, flags);
	}

	ROOT_SIGNATURE_DESC(NULL_STRUCT)
	{
		Init(0, NULL, 0, NULL, D3D12_ROOT_SIGNATURE_FLAG_NONE);
	}

	inline void Init(
		UINT numParameters,
		_In_reads_opt_(numParameters) const D3D12_ROOT_PARAMETER* parameters,
		UINT numStaticSamplers = 0,
		_In_reads_opt_(numStaticSamplers) const D3D12_STATIC_SAMPLER_DESC* staticSamplers = NULL,
		D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_NONE)
	{
		Init(*this, numParameters, parameters, numStaticSamplers, staticSamplers, flags);
	}

	static inline void Init(
		_Out_ D3D12_ROOT_SIGNATURE_DESC& desc,
		UINT numParameters,
		_In_reads_opt_(numParameters) const D3D12_ROOT_PARAMETER* parameters,
		UINT numStaticSamplers = 0,
		_In_reads_opt_(numStaticSamplers) const D3D12_STATIC_SAMPLER_DESC* staticSamplers = NULL,
		D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_NONE)
	{
		desc.NumParameters     = numParameters;
		desc.pParameters       = parameters;
		desc.NumStaticSamplers = numStaticSamplers;
		desc.pStaticSamplers   = staticSamplers;
		desc.Flags = flags;
	}

	inline void Create(IDevice* device, RootSignatureComPtr* rootSignature)
	{
		BlobComPtr rootSigBlob = nullptr;
		BlobComPtr errorBlob = nullptr;

		HRESULT hresult = D3D12SerializeRootSignature(this, D3D_ROOT_SIGNATURE_VERSION_1, &rootSigBlob, &errorBlob);

		if (errorBlob != nullptr)
		{
			::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		}
		if (FAILED(hresult))
		{
			::OutputDebugStringA("Result False");
		}

		if (FAILED(device->CreateRootSignature(
			0,
			rootSigBlob->GetBufferPointer(),
			rootSigBlob->GetBufferSize(),
			IID_PPV_ARGS(rootSignature->GetAddressOf()))))
		{
			::OutputDebugStringA("Didn't create rootsignature");
		};

	}
};



//////////////////////////////////////////////////////////////////////////////////
//            CPU_DESCRIPTOR_HANDLER : D3D12_CPU_DESCRIPTOR_HANDLE
//////////////////////////////////////////////////////////////////////////////////
struct CPU_DESC_HANDLER : public D3D12_CPU_DESCRIPTOR_HANDLE
{
	CPU_DESC_HANDLER() { ptr = NULL; };
	explicit CPU_DESC_HANDLER(const D3D12_CPU_DESCRIPTOR_HANDLE& handler) :D3D12_CPU_DESCRIPTOR_HANDLE(handler) {};

	CPU_DESC_HANDLER(_In_ const D3D12_CPU_DESCRIPTOR_HANDLE& other, INT offsetScaledByIncrementSize)
	{
		InitOffsetted(other, offsetScaledByIncrementSize);
	}

	CPU_DESC_HANDLER(_In_ const D3D12_CPU_DESCRIPTOR_HANDLE& other, INT offsetInDescriptors, UINT descriptorIncrementSize)
	{
		InitOffsetted(other, offsetInDescriptors, descriptorIncrementSize);
	}

	CPU_DESC_HANDLER& Offset(INT offsetInDescriptors, UINT descriptorIncrementSize)
	{
		ptr += (SIZE_T)offsetInDescriptors * descriptorIncrementSize;
		return *this;
	}

	CPU_DESC_HANDLER& Offset(INT offsetScaledByIncrementSize)
	{
		ptr += offsetScaledByIncrementSize;
		return *this;
	}

	bool operator==(_In_ const D3D12_CPU_DESCRIPTOR_HANDLE& other)
	{
		return (ptr == other.ptr);
	}

	bool operator!=(_In_ const D3D12_CPU_DESCRIPTOR_HANDLE& other)
	{
		return (ptr != other.ptr);
	}

	CPU_DESC_HANDLER& operator=(const D3D12_CPU_DESCRIPTOR_HANDLE& other)
	{
		ptr = other.ptr;
		return *this;
	}

	inline void InitOffsetted(_In_ const D3D12_CPU_DESCRIPTOR_HANDLE& base, INT offsetScaledByIncrementSize)
	{
		InitOffsetted(*this, base, offsetScaledByIncrementSize);
	}

	inline void InitOffsetted(_In_ const D3D12_CPU_DESCRIPTOR_HANDLE& base, INT offsetInDescriptors, UINT descriptorIncrementSize)
	{
		InitOffsetted(*this, base, offsetInDescriptors, descriptorIncrementSize);
	}

	static inline void InitOffsetted(_Out_ D3D12_CPU_DESCRIPTOR_HANDLE& handle, _In_ const D3D12_CPU_DESCRIPTOR_HANDLE& base, INT offsetScaledByIncrementSize)
	{
		handle.ptr = base.ptr + offsetScaledByIncrementSize;
	}

	static inline void InitOffsetted(_Out_ D3D12_CPU_DESCRIPTOR_HANDLE& handle, _In_ const D3D12_CPU_DESCRIPTOR_HANDLE& base, INT offsetInDescriptors, UINT descriptorIncrementSize)
	{
		handle.ptr = base.ptr + (SIZE_T)offsetInDescriptors * descriptorIncrementSize;
	}
};

//////////////////////////////////////////////////////////////////////////////////
//            GPU_DESCRIPTOR_HANDLER : D3D12_GPU_DESCRIPTOR_HANDLE
//////////////////////////////////////////////////////////////////////////////////
struct GPU_DESC_HANDLER : public D3D12_GPU_DESCRIPTOR_HANDLE
{
	GPU_DESC_HANDLER() { ptr = NULL; };
	explicit GPU_DESC_HANDLER(const D3D12_GPU_DESCRIPTOR_HANDLE& handler) :D3D12_GPU_DESCRIPTOR_HANDLE(handler) {};

	GPU_DESC_HANDLER(_In_ const D3D12_GPU_DESCRIPTOR_HANDLE& other, INT offsetScaledByIncrementSize)
	{
		InitOffsetted(other, offsetScaledByIncrementSize);
	}

	GPU_DESC_HANDLER(_In_ const D3D12_GPU_DESCRIPTOR_HANDLE& other, INT offsetInDescriptors, UINT descriptorIncrementSize)
	{
		InitOffsetted(other, offsetInDescriptors, descriptorIncrementSize);
	}

	GPU_DESC_HANDLER& Offset(INT offsetInDescriptors, UINT descriptorIncrementSize)
	{
		ptr += (SIZE_T)offsetInDescriptors * descriptorIncrementSize;
		return *this;
	}

	GPU_DESC_HANDLER& Offset(INT offsetScaledByIncrementSize)
	{
		ptr += offsetScaledByIncrementSize;
		return *this;
	}

	bool operator==(_In_ const D3D12_GPU_DESCRIPTOR_HANDLE& other)
	{
		return (ptr == other.ptr);
	}

	bool operator!=(_In_ const D3D12_GPU_DESCRIPTOR_HANDLE& other)
	{
		return (ptr != other.ptr);
	}

	GPU_DESC_HANDLER& operator=(const D3D12_GPU_DESCRIPTOR_HANDLE& other)
	{
		ptr = other.ptr;
		return *this;
	}

	inline void InitOffsetted(_In_ const D3D12_GPU_DESCRIPTOR_HANDLE& base, INT offsetScaledByIncrementSize)
	{
		InitOffsetted(*this, base, offsetScaledByIncrementSize);
	}

	inline void InitOffsetted(_In_ const D3D12_GPU_DESCRIPTOR_HANDLE& base, INT offsetInDescriptors, UINT descriptorIncrementSize)
	{
		InitOffsetted(*this, base, offsetInDescriptors, descriptorIncrementSize);
	}

	static inline void InitOffsetted(_Out_ D3D12_GPU_DESCRIPTOR_HANDLE& handle, _In_ const D3D12_GPU_DESCRIPTOR_HANDLE& base, INT offsetScaledByIncrementSize)
	{
		handle.ptr = base.ptr + offsetScaledByIncrementSize;
	}

	static inline void InitOffsetted(_Out_ D3D12_GPU_DESCRIPTOR_HANDLE& handle, _In_ const D3D12_GPU_DESCRIPTOR_HANDLE& base, INT offsetInDescriptors, UINT descriptorIncrementSize)
	{
		handle.ptr = base.ptr + (SIZE_T)offsetInDescriptors * descriptorIncrementSize;
	}
};

#endif