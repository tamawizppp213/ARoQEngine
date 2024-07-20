//////////////////////////////////////////////////////////////////////////////////
///             @file   RHICommonState.hpp
///             @brief  Common State 
///             @author Toide Yutaro
///             @date   2022_07_20
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::core;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
SamplerInfo SamplerInfo::GetDefaultSampler(DefaultSamplerType type)
{
	SamplerInfo samplerDesc = {};
	switch (type)
	{
	case DefaultSamplerType::NearestWrap:
	{
		samplerDesc.Filter       = FilterOption::Nearest;
		samplerDesc.AddressModeU = SamplerAddressMode::Wrap;
		samplerDesc.AddressModeV = SamplerAddressMode::Wrap;
		samplerDesc.AddressModeW = SamplerAddressMode::Wrap;
		break;
	}
	case DefaultSamplerType::NearestClamp:
	{
		samplerDesc.Filter       = FilterOption::Nearest;
		samplerDesc.AddressModeU = SamplerAddressMode::Clamp;
		samplerDesc.AddressModeV = SamplerAddressMode::Clamp;
		samplerDesc.AddressModeW = SamplerAddressMode::Clamp;
		break;
	}
	case DefaultSamplerType::LinearWrap:
	{
		samplerDesc.Filter       = FilterOption::BiLinear;
		samplerDesc.AddressModeU = SamplerAddressMode::Wrap;
		samplerDesc.AddressModeV = SamplerAddressMode::Wrap;
		samplerDesc.AddressModeW = SamplerAddressMode::Wrap;
		break;
	}
	case DefaultSamplerType::LinearClamp:
	{
		samplerDesc.Filter       = FilterOption::BiLinear;
		samplerDesc.AddressModeU = SamplerAddressMode::Clamp;
		samplerDesc.AddressModeV = SamplerAddressMode::Clamp;
		samplerDesc.AddressModeW = SamplerAddressMode::Clamp;
		break;
	}
	case DefaultSamplerType::LinearBorder:
	{
		samplerDesc.Filter = FilterOption::BiLinear;
		samplerDesc.AddressModeU = SamplerAddressMode::Border;
		samplerDesc.AddressModeV = SamplerAddressMode::Border;
		samplerDesc.AddressModeW = SamplerAddressMode::Border;
		break;
	}
	case DefaultSamplerType::AnisotropicNearestWrap:
	{
		samplerDesc.Filter       = FilterOption::AnisotropicNearest;
		samplerDesc.AddressModeU = SamplerAddressMode::Wrap;
		samplerDesc.AddressModeV = SamplerAddressMode::Wrap;
		samplerDesc.AddressModeW = SamplerAddressMode::Wrap;
		break;
	}
	case DefaultSamplerType::AnisotropicNearestClamp:
	{
		samplerDesc.Filter = FilterOption::AnisotropicNearest;
		samplerDesc.AddressModeU = SamplerAddressMode::Wrap;
		samplerDesc.AddressModeV = SamplerAddressMode::Wrap;
		samplerDesc.AddressModeW = SamplerAddressMode::Wrap;
		break;
	}
	case DefaultSamplerType::AnisotropicLinearWrap:
	{
		samplerDesc.Filter = FilterOption::AnisotropicLinear;
		samplerDesc.AddressModeU = SamplerAddressMode::Wrap;
		samplerDesc.AddressModeV = SamplerAddressMode::Wrap;
		samplerDesc.AddressModeW = SamplerAddressMode::Wrap;
		break;
	}
	case DefaultSamplerType::AnisotropicLinearClamp:
	{
		samplerDesc.Filter = FilterOption::AnisotropicNearest;
		samplerDesc.AddressModeU = SamplerAddressMode::Wrap;
		samplerDesc.AddressModeV = SamplerAddressMode::Wrap;
		samplerDesc.AddressModeW = SamplerAddressMode::Wrap;
		break;
	}
	}
	return samplerDesc;
}

#pragma region GPUBuffer
GPUBufferMetaData::GPUBufferMetaData(gu::uint32 stride, gu::uint32 count, core::BufferCreateFlags usage, ResourceState state, MemoryHeap heapType, void* initData )
	: Stride(stride), Count(count), Usage(usage), State(state), HeapType(heapType), ResourceType(core::ResourceType::Buffer), InitData(initData)
{
	if (gu::HasAnyFlags(Usage, BufferCreateFlags::ConstantBuffer)) 
	{
		Stride = (gu::uint32)CalcConstantBufferByteSize(stride); 
	}
}

//GPUBufferMetaData GPUBufferMetaData::UploadBuffer(const PixelFormat format, const gu::uint64 count, const MemoryHeap heap, void* initData)
//{
//	auto info = GPUBufferMetaData(InputFormatSizeOf::Get(format), count, core::BufferCreateFlags::ConstantBuffer, ResourceState::GeneralRead, heap, BufferType::Upload, initData);
//	//info.Format = format;
//	return info;
//}

GPUBufferMetaData GPUBufferMetaData::UploadBuffer(const gu::uint32 stride, const gu::uint32 count, const MemoryHeap heap, void* initData)
{
	using enum core::BufferCreateFlags;
	return GPUBufferMetaData(stride, count, core::BufferCreateFlags::ConstantBuffer | Dynamic, ResourceState::GeneralRead, heap, initData);
}
GPUBufferMetaData GPUBufferMetaData::DefaultBuffer(const gu::uint32 stride, const gu::uint32 count, const MemoryHeap heap, void* initData)
{
	using enum core::BufferCreateFlags;
	return GPUBufferMetaData(stride, count, core::BufferCreateFlags::ConstantBuffer | Static, ResourceState::Common, heap, initData);
}
GPUBufferMetaData GPUBufferMetaData::ConstantBuffer(const gu::uint32 stride, const gu::uint32 count, const MemoryHeap heap, const ResourceState state, void* initData)
{
	using enum core::BufferCreateFlags;
	return GPUBufferMetaData(stride, count, core::BufferCreateFlags::ConstantBuffer | Dynamic, state, heap, initData);
}
GPUBufferMetaData GPUBufferMetaData::VertexBuffer(const gu::uint32 stride, const gu::uint32 count, const MemoryHeap heap, const ResourceState state, void* initData)
{
	using enum core::BufferCreateFlags;
	return GPUBufferMetaData(stride, count, VertexBuffer | Dynamic, state, heap, initData);
}
GPUBufferMetaData GPUBufferMetaData::IndexBuffer(const gu::uint32 stride, const gu::uint32 count, const MemoryHeap heap, const ResourceState state, void* initData)
{
	using enum core::BufferCreateFlags;
	return GPUBufferMetaData(stride, count, core::BufferCreateFlags::IndexBuffer | Static, state, heap, initData);
}
#pragma endregion GPUBuffer
#pragma region GPUTexture
GPUTextureMetaData GPUTextureMetaData::Texture1D(const gu::uint32 width, const core::PixelFormat format, const gu::uint8 mipLevels, const core::TextureCreateFlags usage)
{
	GPUTextureMetaData metaData =
	{
		.ClearColor       = ClearValue(),
		.Usage            = usage | TextureCreateFlags::ShaderResource,
		.Width            = width,
		.Height           = 1,
		.DepthOrArraySize = 1,
		.MipMapLevels     = mipLevels,
		.State            = ResourceState::GeneralRead,
		.PixelFormat      = format,
		.Sample           = MultiSample::Count1,
		.Dimension        = core::ResourceDimension::Texture1D,
		.ResourceType     = core::ResourceType::Texture1D,
		.HeapType         = core::MemoryHeap::Default,
	};

	metaData.CalculateByteSize();
	return metaData;
}
GPUTextureMetaData GPUTextureMetaData::Texture1DArray(const gu::uint32 width, const gu::uint16 length, const core::PixelFormat format, const gu::uint8 mipLevels, const core::TextureCreateFlags usage)
{
	GPUTextureMetaData metaData =
	{
		.ClearColor       = ClearValue(),
		.Usage            = usage | TextureCreateFlags::ShaderResource,
		.Width            = width,
		.Height           = 1,
		.DepthOrArraySize = length,
		.MipMapLevels     = mipLevels,
		.State            = ResourceState::GeneralRead,
		.PixelFormat      = format,
		.Sample           = MultiSample::Count1,
		.Dimension        = core::ResourceDimension::Texture1D,
		.ResourceType     = core::ResourceType::Texture1DArray,
		.HeapType         = core::MemoryHeap::Default,
	};

	metaData.CalculateByteSize();
	return metaData;
}
GPUTextureMetaData GPUTextureMetaData::Texture2D(const gu::uint32 width, const gu::uint32 height, const core::PixelFormat format, const gu::uint8 mipLevels, const core::TextureCreateFlags usage)
{
	GPUTextureMetaData metaData =
	{
		.ClearColor       = ClearValue(),
		.Usage            = usage | TextureCreateFlags::ShaderResource,
		.Width            = width,
		.Height           = height,
		.DepthOrArraySize = 1,
		.MipMapLevels     = mipLevels,
		.State            = ResourceState::GeneralRead,
		.PixelFormat      = format,
		.Sample           = MultiSample::Count1,
		.Dimension        = core::ResourceDimension::Texture2D,
		.ResourceType     = core::ResourceType::Texture2D,
		.HeapType         = core::MemoryHeap::Default,
	};

	metaData.CalculateByteSize();
	return metaData;
}
GPUTextureMetaData GPUTextureMetaData::Texture2DArray(const gu::uint32 width, const gu::uint32 height, const gu::uint16 length, const core::PixelFormat format, const gu::uint8 mipLevels, const core::TextureCreateFlags usage)
{
	GPUTextureMetaData metaData =
	{
		.ClearColor       = ClearValue(),
		.Usage            = usage | TextureCreateFlags::ShaderResource,
		.Width            = width,
		.Height           = height,
		.DepthOrArraySize = length,
		.MipMapLevels     = mipLevels,
		.State            = ResourceState::GeneralRead,
		.PixelFormat      = format,
		.Sample           = MultiSample::Count1,
		.Dimension        = core::ResourceDimension::Texture2D,
		.ResourceType     = core::ResourceType::Texture2DArray,
		.HeapType         = core::MemoryHeap::Default,
	};

	metaData.CalculateByteSize();
	return metaData;
}
GPUTextureMetaData GPUTextureMetaData::Texture3D(const gu::uint32 width, const gu::uint32 height, const gu::uint16 depth, const core::PixelFormat format, const gu::uint8 mipLevels, const core::TextureCreateFlags usage)
{
	GPUTextureMetaData metaData =
	{
		.ClearColor       = ClearValue(),
		.Usage            = usage | TextureCreateFlags::ShaderResource,
		.Width            = width,
		.Height           = height,
		.DepthOrArraySize = depth,
		.MipMapLevels     = mipLevels,
		.State            = ResourceState::GeneralRead,
		.PixelFormat      = format,
		.Sample           = MultiSample::Count1,
		.Dimension        = core::ResourceDimension::Texture3D,
		.ResourceType     = core::ResourceType::Texture3D,
		.HeapType         = core::MemoryHeap::Default,
	};

	metaData.CalculateByteSize();
	return metaData;
}

GPUTextureMetaData GPUTextureMetaData::Texture2DMultiSample(const gu::uint32 width, const gu::uint32 height, const core::PixelFormat format, const core::MultiSample sample, const gu::uint8 mipLevels, const core::TextureCreateFlags usage)
{
	GPUTextureMetaData metaData =
	{
		.ClearColor       = ClearValue(),
		.Usage            = usage | TextureCreateFlags::ShaderResource,
		.Width            = width,
		.Height           = height,
		.DepthOrArraySize = 1,
		.MipMapLevels     = mipLevels,
		.State            = ResourceState::GeneralRead,
		.PixelFormat      = format,
		.Sample           = sample,
		.Dimension        = core::ResourceDimension::Texture2D,
		.ResourceType     = core::ResourceType::Texture2DMultiSample,
		.HeapType         = core::MemoryHeap::Default,
	};

	metaData.CalculateByteSize();
	return metaData;
}

GPUTextureMetaData GPUTextureMetaData::Texture2DArrayMultiSample(const gu::uint32 width, const gu::uint32 height, const gu::uint16 length, const core::PixelFormat format, const core::MultiSample sample, const gu::uint8 mipLevels, const core::TextureCreateFlags usage)
{
	GPUTextureMetaData metaData =
	{
		.ClearColor       = ClearValue(),
		.Usage            = usage | TextureCreateFlags::ShaderResource,
		.Width            = width,
		.Height           = height,
		.DepthOrArraySize = length,
		.MipMapLevels     = mipLevels,
		.State            = ResourceState::GeneralRead,
		.PixelFormat      = format,
		.Sample           = sample,
		.Dimension        = core::ResourceDimension::Texture2D,
		.ResourceType     = core::ResourceType::Texture2DArrayMultiSample,
		.HeapType         = core::MemoryHeap::Default,
	};

	metaData.CalculateByteSize();
	return metaData;
}

GPUTextureMetaData GPUTextureMetaData::CubeMap(const gu::uint32 width, const gu::uint32 height, const core::PixelFormat format, const gu::uint8 mipLevels, const core::TextureCreateFlags usage)
{
	GPUTextureMetaData metaData =
	{
		.ClearColor       = ClearValue(),
		.Usage            = usage | TextureCreateFlags::ShaderResource,
		.Width            = width,
		.Height           = height,
		.DepthOrArraySize = 6,
		.MipMapLevels     = mipLevels,
		.State            = ResourceState::GeneralRead,
		.PixelFormat      = format,
		.Sample           = MultiSample::Count1,
		.Dimension        = core::ResourceDimension::Texture2D,
		.ResourceType     = core::ResourceType::TextureCube,
		.HeapType         = core::MemoryHeap::Default,
	};

	metaData.CalculateByteSize();
	return metaData;
}
GPUTextureMetaData GPUTextureMetaData::CubeMapArray(const gu::uint32 width, const gu::uint32 height, const gu::uint16 length, const core::PixelFormat format, const gu::uint8 mipLevels, const core::TextureCreateFlags usage)
{
	GPUTextureMetaData metaData =
	{
		.ClearColor       = ClearValue(),
		.Usage            = usage | TextureCreateFlags::ShaderResource,
		.Width            = width,
		.Height           = height,
		.DepthOrArraySize = (gu::uint16)(6 * length),
		.MipMapLevels     = mipLevels,
		.State            = ResourceState::GeneralRead,
		.PixelFormat      = format,
		.Sample           = MultiSample::Count1,
		.Dimension        = core::ResourceDimension::Texture2D,
		.ResourceType     = core::ResourceType::TextureCubeArray,
		.HeapType         = core::MemoryHeap::Default,
	};

	metaData.CalculateByteSize();
	return metaData;
}
GPUTextureMetaData GPUTextureMetaData::RenderTarget(const gu::uint32 width, const gu::uint32 height, const core::PixelFormat format, const core::ClearValue& clearValue)
{
	GPUTextureMetaData metaData =
	{
		.ClearColor       = clearValue,
		.Usage            = core::TextureCreateFlags::RenderTargetable | TextureCreateFlags::UnorderedAccess | TextureCreateFlags::ShaderResource,
		.Width            = width,
		.Height           = height,
		.DepthOrArraySize = 1,
		.MipMapLevels     = 1,
		.State            = ResourceState::GeneralRead,
		.PixelFormat      = format,
		.Sample           = MultiSample::Count1,
		.Dimension        = core::ResourceDimension::Texture2D,
		.ResourceType     = core::ResourceType::Texture2D,
		.HeapType         = core::MemoryHeap::Default,
	};

	metaData.CalculateByteSize();
	return metaData;

}
GPUTextureMetaData GPUTextureMetaData::RenderTargetMultiSample(const gu::uint32 width, const gu::uint32 height, const core::PixelFormat format, const core::MultiSample sample, const core::ClearValue& clearValue)
{
	GPUTextureMetaData metaData = 
	{
		.ClearColor       = clearValue,
		.Usage            = core::TextureCreateFlags::RenderTargetable | TextureCreateFlags::UnorderedAccess | TextureCreateFlags::ShaderResource,
		.Width            = width,
		.Height           = height,
		.DepthOrArraySize = 1,
		.MipMapLevels     = 1,
		.State            = ResourceState::GeneralRead,
		.PixelFormat      = format,
		.Sample           = sample,
		.Dimension        = core::ResourceDimension::Texture2D,
		.ResourceType     = core::ResourceType::Texture2DMultiSample,
		.HeapType         = core::MemoryHeap::Default
	};

	metaData.CalculateByteSize();
	return metaData;
}

GPUTextureMetaData GPUTextureMetaData::DepthStencil(const gu::uint32 width, const gu::uint32 height, const core::PixelFormat format, const core::ClearValue& clearValue)
{
	GPUTextureMetaData metaData = 
	{
		.ClearColor       = clearValue,
		.Usage            = core::TextureCreateFlags::DepthStencilTargetable | TextureCreateFlags::ShaderResource,
		.Width            = width,
		.Height           = height,
		.DepthOrArraySize = 1,
		.MipMapLevels     = 1,
		.State            = ResourceState::DepthStencil,
		.PixelFormat      = format,
		.Sample           = MultiSample::Count1,
		.Dimension        = core::ResourceDimension::Texture2D,
		.ResourceType     = core::ResourceType::Texture2D,
		.HeapType         = core::MemoryHeap::Default
	};

	metaData.CalculateByteSize();
	return metaData;
}

GPUTextureMetaData GPUTextureMetaData::DepthStencilMultiSample(const gu::uint32 width, const gu::uint32 height, const core::PixelFormat format, const core::MultiSample sample, const core::ClearValue& clearValue)
{
	GPUTextureMetaData metaData = 
	{
		.ClearColor       = clearValue,
		.Usage            = core::TextureCreateFlags::DepthStencilTargetable | TextureCreateFlags::ShaderResource,
		.Width            = width,
		.Height           = height,
		.DepthOrArraySize = 1,
		.MipMapLevels     = 1,
		.State            = ResourceState::DepthStencil,
		.PixelFormat      = format,
		.Sample           = sample,
		.Dimension        = core::ResourceDimension::Texture2D,
		.ResourceType     = core::ResourceType::Texture2DMultiSample,
		.HeapType         = core::MemoryHeap::Default
	};

	metaData.CalculateByteSize();
	return metaData;
}
#pragma endregion GPUTexture
#pragma region BlendProperty
/****************************************************************************
*                     NoColorWrite
****************************************************************************/
/* @brief     そのままレンダーターゲットの出力を表示します
*
*  @param[in] bool useAlphaToCoverage
*
*  @return    BlendProperty
*****************************************************************************/
BlendProperty BlendProperty::NoColorWrite(const bool enableBlend)
{
	return BlendProperty
	(
		BlendOperator::Add,  // color operator
		BlendOperator::Add,  // alpha operator
		BlendFactor::One,    // Render Target alpha(a) element blend mode
		BlendFactor::One,    // Render Target color(rgb) element blend mode    
		BlendFactor::Zero,   // pixel shader alpha 
		BlendFactor::Zero,   // pixel shader color
		ColorMask::None,
		enableBlend
	);
}

/****************************************************************************
*                     OverWrite
****************************************************************************/
/* @brief     そのままピクセルシェーダーの出力を上書きします
*
*  @param[in] bool useAlphaToCoverage
*
*  @return    BlendProperty
*****************************************************************************/
BlendProperty BlendProperty::OverWrite(const bool enableBlend)
{
	return BlendProperty(
		BlendOperator::Add, // color operator
		BlendOperator::Add, // alpha operator
		BlendFactor::Zero,  // Render Target alpha(a) element blend mode
		BlendFactor::Zero,  // Render Target color(rgb) element blend mode    
		BlendFactor::One,   // pixel shader alpha 
		BlendFactor::One,   // pixel shader color
		ColorMask::All,
		enableBlend);
}

/****************************************************************************
*                     AlphaBlend
****************************************************************************/
/*  @brief    アルファブレンドを実行します destination * (1 - source.Alpha) + source * 1
*
*  @param[in] bool useAlphaToCoverage
*
*  @return    BlendProperty
*****************************************************************************/
BlendProperty BlendProperty::AlphaBlend(const bool enableBlend)
{
	return BlendProperty
	(
		BlendOperator::Add,                 // color operator
		BlendOperator::Add,                 // alpha operator
		BlendFactor::Zero ,                 // Render Target alpha(a) element blend mode
		BlendFactor::Inverse_Source_Alpha,  // Render Target color(rgb) element blend mode    
		BlendFactor::One,                   // source alpha 
		BlendFactor::Source_Alpha,          // source
		ColorMask::All,                     // color mask
		enableBlend
	);
}
#pragma endregion      BlendProperty
#pragma region RasterizerProperty
RasterizerProperty RasterizerProperty::Solid(const bool useMultiSample, const FrontFace frontFace, const CullingMode cullingMode, const float depthBias, const float slopeScaleDepthBias, const float clampMaxDepthBias)
{
	return RasterizerProperty(frontFace, cullingMode, FillMode::Solid, true, useMultiSample, depthBias, slopeScaleDepthBias, clampMaxDepthBias);
}

RasterizerProperty RasterizerProperty::WireFrame(const bool useMultiSample, const FrontFace frontFace, const CullingMode cullingMode, const float depthBias, const float slopeScaleDepthBias, const float clampMaxDepthBias)
{
	return RasterizerProperty(frontFace, cullingMode, FillMode::Solid, true, useMultiSample, depthBias, slopeScaleDepthBias, clampMaxDepthBias);
}
#pragma endregion RasterizerProperty