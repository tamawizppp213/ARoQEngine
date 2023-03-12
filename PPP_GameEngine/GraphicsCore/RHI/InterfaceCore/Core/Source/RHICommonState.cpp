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
	case DefaultSamplerType::SamplerPointWrap:
	{
		samplerDesc.Filter       = FilterOption::MinPointMagPointMipPoint;
		samplerDesc.AddressModeU = SamplerAddressMode::Wrap;
		samplerDesc.AddressModeV = SamplerAddressMode::Wrap;
		samplerDesc.AddressModeW = SamplerAddressMode::Wrap;
		break;
	}
	case DefaultSamplerType::SamplerPointClamp:
	{
		samplerDesc.Filter       = FilterOption::MinPointMagPointMipPoint;
		samplerDesc.AddressModeU = SamplerAddressMode::Clamp;
		samplerDesc.AddressModeV = SamplerAddressMode::Clamp;
		samplerDesc.AddressModeW = SamplerAddressMode::Clamp;
		break;
	}
	case DefaultSamplerType::SamplerLinearWrap:
	{
		samplerDesc.Filter       = FilterOption::MinLinearMagLinearMipLinear;
		samplerDesc.AddressModeU = SamplerAddressMode::Wrap;
		samplerDesc.AddressModeV = SamplerAddressMode::Wrap;
		samplerDesc.AddressModeW = SamplerAddressMode::Wrap;
		break;
	}
	case DefaultSamplerType::SamplerLinearClamp:
	{
		samplerDesc.Filter       = FilterOption::MinLinearMagLinearMipLinear;
		samplerDesc.AddressModeU = SamplerAddressMode::Clamp;
		samplerDesc.AddressModeV = SamplerAddressMode::Clamp;
		samplerDesc.AddressModeW = SamplerAddressMode::Clamp;
		break;
	}
	case DefaultSamplerType::SamplerAnisotropicWrap:
	{
		samplerDesc.Filter       = FilterOption::Anisotropy;
		samplerDesc.AddressModeU = SamplerAddressMode::Wrap;
		samplerDesc.AddressModeV = SamplerAddressMode::Wrap;
		samplerDesc.AddressModeW = SamplerAddressMode::Wrap;
		break;
	}
	case DefaultSamplerType::SamplerAnisotropicClamp:
	{
		samplerDesc.Filter       = FilterOption::Anisotropy;
		samplerDesc.AddressModeU = SamplerAddressMode::Clamp;
		samplerDesc.AddressModeV = SamplerAddressMode::Clamp;
		samplerDesc.AddressModeW = SamplerAddressMode::Clamp;
		break;
	}
	}
	return samplerDesc;
}

#pragma region GPUBuffer
GPUBufferMetaData::GPUBufferMetaData(size_t stride, size_t count, core::ResourceUsage usage, ResourceState state, MemoryHeap heapType, core::BufferType bufferType, void* initData )
	: Stride(stride), Count(count), ResourceUsage(usage), State(state), HeapType(heapType), BufferType(bufferType),ResourceType(core::ResourceType::Buffer), InitData(initData)
{
	if (bufferType == BufferType::Constant) { Stride = CalcConstantBufferByteSize(stride); }
	ByteSize = Stride * Count;
}

GPUBufferMetaData GPUBufferMetaData::UploadBuffer(const InputFormat format, const size_t count, const MemoryHeap heap, void* initData)
{
	auto info = GPUBufferMetaData(InputFormatSizeOf::Get(format), count, core::ResourceUsage::ConstantBuffer, ResourceState::GeneralRead, heap, BufferType::Upload, initData);
	info.Format = format;
	return info;
}

GPUBufferMetaData GPUBufferMetaData::UploadBuffer(const size_t stride, const size_t count, const MemoryHeap heap, void* initData)
{
	return GPUBufferMetaData(stride, count, core::ResourceUsage::ConstantBuffer, ResourceState::GeneralRead, heap, BufferType::Upload, initData);
}
GPUBufferMetaData GPUBufferMetaData::DefaultBuffer(const size_t stride, const size_t count, const MemoryHeap heap, void* initData)
{
	return GPUBufferMetaData(stride, count, core::ResourceUsage::ConstantBuffer, ResourceState::Common, heap, BufferType::Default, initData);
}
GPUBufferMetaData GPUBufferMetaData::ConstantBuffer(const size_t stride, const size_t count, const MemoryHeap heap, const ResourceState state, void* initData)
{
	return GPUBufferMetaData(stride, count, core::ResourceUsage::ConstantBuffer, state, heap, BufferType::Constant, initData);
}
GPUBufferMetaData GPUBufferMetaData::VertexBuffer(const size_t stride, const size_t count, const MemoryHeap heap, const ResourceState state, void* initData)
{
	return GPUBufferMetaData(stride, count, core::ResourceUsage::VertexBuffer, state, heap, BufferType::Vertex, initData);
}
GPUBufferMetaData GPUBufferMetaData::IndexBuffer(const size_t stride, const size_t count, const MemoryHeap heap, const ResourceState state, void* initData)
{
	return GPUBufferMetaData(stride, count, core::ResourceUsage::IndexBuffer, state, heap, BufferType::Index, initData);
}
#pragma endregion GPUBuffer
#pragma region GPUTexture
GPUTextureMetaData GPUTextureMetaData::Texture1D(const size_t width, const core::PixelFormat format, const size_t mipLevels, const core::ResourceUsage usage)
{
	GPUTextureMetaData metaData = {};
	metaData.Width            = width;
	metaData.Height           = 1;
	metaData.DepthOrArraySize = 1;
	metaData.PixelFormat      = format;
	metaData.MipLevels        = mipLevels;
	metaData.ResourceUsage    = usage;
	metaData.State            = ResourceState::GeneralRead;
	metaData.Dimension        = core::ResourceDimension::Dimension1D;
	metaData.ResourceType     = core::ResourceType::Texture1D;
	metaData.HeapType         = core::MemoryHeap::Default;
	metaData.Sample           = MultiSample::Count1;
	metaData.CalculateByteSize();
	return metaData;
}
GPUTextureMetaData GPUTextureMetaData::Texture1DArray(const size_t width, const size_t length, const core::PixelFormat format, const size_t mipLevels, const core::ResourceUsage usage)
{
	GPUTextureMetaData metaData = {};
	metaData.Width            = width;
	metaData.Height           = 1;
	metaData.DepthOrArraySize = length;
	metaData.PixelFormat      = format;
	metaData.MipLevels        = mipLevels;
	metaData.ResourceUsage    = usage;
	metaData.State            = ResourceState::GeneralRead;
	metaData.Dimension        = core::ResourceDimension::Dimension1D;
	metaData.ResourceType     = core::ResourceType::Texture1DArray;
	metaData.HeapType         = core::MemoryHeap::Default;
	metaData.Sample           = MultiSample::Count1;
	metaData.CalculateByteSize();
	return metaData;
}
GPUTextureMetaData GPUTextureMetaData::Texture2D(const size_t width, const size_t height, const core::PixelFormat format, const size_t mipLevels, const core::ResourceUsage usage)
{
	GPUTextureMetaData metaData = {};
	metaData.Width            = width;
	metaData.Height           = height;
	metaData.DepthOrArraySize = 1;
	metaData.PixelFormat      = format;
	metaData.MipLevels        = mipLevels;
	metaData.ResourceUsage    = usage;
	metaData.State            = ResourceState::GeneralRead;
	metaData.Dimension        = core::ResourceDimension::Dimension2D;
	metaData.ResourceType     = core::ResourceType::Texture2D;
	metaData.HeapType         = core::MemoryHeap::Default;
	metaData.Sample           = MultiSample::Count1;
	metaData.CalculateByteSize();
	return metaData;
}
GPUTextureMetaData GPUTextureMetaData::Texture2DArray(const size_t width, const size_t height, const size_t length, const core::PixelFormat format, const size_t mipLevels, const core::ResourceUsage usage)
{
	GPUTextureMetaData metaData = {};
	metaData.Width            = width;
	metaData.Height           = height;
	metaData.DepthOrArraySize = length;
	metaData.PixelFormat      = format;
	metaData.MipLevels        = mipLevels;
	metaData.ResourceUsage    = usage;
	metaData.State            = ResourceState::GeneralRead;
	metaData.Dimension        = core::ResourceDimension::Dimension2D;
	metaData.ResourceType     = core::ResourceType::Texture2D;
	metaData.HeapType         = core::MemoryHeap::Default;
	metaData.Sample           = MultiSample::Count1;
	metaData.CalculateByteSize();
	return metaData;
}
GPUTextureMetaData GPUTextureMetaData::Texture3D(const size_t width, const size_t height, const size_t depth, const core::PixelFormat format, const size_t mipLevels, const core::ResourceUsage usage)
{
	GPUTextureMetaData metaData = {};
	metaData.Width            = width;
	metaData.Height           = height;
	metaData.DepthOrArraySize = depth;
	metaData.PixelFormat      = format;
	metaData.MipLevels        = mipLevels;
	metaData.ResourceUsage    = usage;
	metaData.State            = ResourceState::GeneralRead;
	metaData.Dimension        = core::ResourceDimension::Dimension3D;
	metaData.ResourceType     = core::ResourceType::Texture3D;
	metaData.HeapType         = core::MemoryHeap::Default;
	metaData.Sample           = MultiSample::Count1;
	metaData.CalculateByteSize();
	return metaData;
}
GPUTextureMetaData GPUTextureMetaData::Texture2DMultiSample(const size_t width, const size_t height, const core::PixelFormat format, const core::MultiSample sample, const size_t mipLevels, const core::ResourceUsage usage)
{
	GPUTextureMetaData metaData = {};
	metaData.Width            = width;
	metaData.Height           = height;
	metaData.DepthOrArraySize = 1;
	metaData.PixelFormat      = format;
	metaData.MipLevels        = mipLevels;
	metaData.ResourceUsage    = usage;
	metaData.State            = ResourceState::GeneralRead;
	metaData.Dimension        = core::ResourceDimension::Dimension2D;
	metaData.ResourceType     = core::ResourceType::Texture2DMultiSample;
	metaData.HeapType         = core::MemoryHeap::Default;
	metaData.Sample           = sample;
	metaData.CalculateByteSize();
	return metaData;
}
GPUTextureMetaData GPUTextureMetaData::Texture2DArrayMultiSample(const size_t width, const size_t height, const size_t length, const core::PixelFormat format, const core::MultiSample sample, const size_t mipLevels, const core::ResourceUsage usage)
{
	GPUTextureMetaData metaData = {};
	metaData.Width            = width;
	metaData.Height           = height;
	metaData.DepthOrArraySize = length;
	metaData.PixelFormat      = format;
	metaData.MipLevels        = mipLevels;
	metaData.ResourceUsage    = usage;
	metaData.State            = ResourceState::GeneralRead;
	metaData.Dimension        = core::ResourceDimension::Dimension2D;
	metaData.ResourceType     = core::ResourceType::Texture2DArrayMultiSample;
	metaData.HeapType         = core::MemoryHeap::Default;
	metaData.Sample           = sample;
	metaData.CalculateByteSize();
	return metaData;
}
GPUTextureMetaData GPUTextureMetaData::CubeMap(const size_t width, const size_t height, const core::PixelFormat format, const size_t mipLevels, const core::ResourceUsage usage)
{
	GPUTextureMetaData metaData = {};
	metaData.Width            = width;
	metaData.Height           = height;
	metaData.DepthOrArraySize = 6;
	metaData.PixelFormat      = format;
	metaData.MipLevels        = mipLevels;
	metaData.ResourceUsage    = usage;
	metaData.State            = ResourceState::GeneralRead;
	metaData.Dimension        = core::ResourceDimension::Dimension2D;
	metaData.ResourceType     = core::ResourceType::TextureCube;
	metaData.HeapType         = core::MemoryHeap::Default;
	metaData.Sample           = MultiSample::Count1;
	metaData.CalculateByteSize();
	return metaData;
}
GPUTextureMetaData GPUTextureMetaData::CubeMapArray(const size_t width, const size_t height, const size_t length, const core::PixelFormat format, const size_t mipLevels, const core::ResourceUsage usage)
{
	GPUTextureMetaData metaData = {};
	metaData.Width            = width;
	metaData.Height           = height;
	metaData.DepthOrArraySize = 6 * length;
	metaData.PixelFormat      = format;
	metaData.MipLevels        = mipLevels;
	metaData.ResourceUsage    = usage;
	metaData.State            = ResourceState::GeneralRead;
	metaData.Dimension        = core::ResourceDimension::Dimension2D;
	metaData.ResourceType     = core::ResourceType::TextureCubeArray;
	metaData.HeapType         = core::MemoryHeap::Default;
	metaData.Sample           = MultiSample::Count1;
	metaData.CalculateByteSize();
	return metaData;
}
GPUTextureMetaData GPUTextureMetaData::RenderTarget(const size_t width, const size_t height, const core::PixelFormat format, const core::ClearValue& clearValue)
{
	GPUTextureMetaData metaData = {};
	metaData.Width            = width;
	metaData.Height           = height;
	metaData.DepthOrArraySize = 1;
	metaData.PixelFormat      = format;
	metaData.MipLevels        = 1;
	metaData.ResourceUsage    = core::ResourceUsage::RenderTarget | ResourceUsage::UnorderedAccess; // for frame buffer
	metaData.State            = ResourceState::GeneralRead;
	metaData.Dimension        = core::ResourceDimension::Dimension2D;
	metaData.ResourceType     = core::ResourceType::Texture2D;
	metaData.HeapType         = core::MemoryHeap::Default;
	metaData.Sample           = MultiSample::Count1;
	metaData.ClearColor       = clearValue;
	metaData.CalculateByteSize();
	return metaData;

}
GPUTextureMetaData GPUTextureMetaData::RenderTargetMultiSample(const size_t width, const size_t height, const core::PixelFormat format, const core::MultiSample sample, const core::ClearValue& clearValue)
{
	GPUTextureMetaData metaData = {};
	metaData.Width            = width;
	metaData.Height           = height;
	metaData.DepthOrArraySize = 1;
	metaData.PixelFormat      = format;
	metaData.MipLevels        = 1;
	metaData.ResourceUsage    = core::ResourceUsage::RenderTarget | ResourceUsage::UnorderedAccess;
	metaData.State            = ResourceState::GeneralRead;
	metaData.Dimension        = core::ResourceDimension::Dimension2D;
	metaData.ResourceType     = core::ResourceType::Texture2DMultiSample;
	metaData.HeapType         = core::MemoryHeap::Default;
	metaData.Sample           = sample;
	metaData.ClearColor       = clearValue;
	metaData.CalculateByteSize();
	return metaData;
}
GPUTextureMetaData GPUTextureMetaData::DepthStencil(const size_t width, const size_t height, const core::PixelFormat format, const core::ClearValue& clearValue)
{
	GPUTextureMetaData metaData = {};
	metaData.Width            = width;
	metaData.Height           = height;
	metaData.DepthOrArraySize = 1;
	metaData.PixelFormat      = format;
	metaData.MipLevels        = 1;
	metaData.ResourceUsage    = core::ResourceUsage::DepthStencil;
	metaData.State            = ResourceState::DepthStencil;
	metaData.Dimension        = core::ResourceDimension::Dimension2D;
	metaData.ResourceType     = core::ResourceType::Texture2D;
	metaData.HeapType         = core::MemoryHeap::Default;
	metaData.Sample           = MultiSample::Count1;
	metaData.ClearColor       = clearValue;
	metaData.CalculateByteSize();
	return metaData;
}
GPUTextureMetaData GPUTextureMetaData::DepthStencilMultiSample(const size_t width, const size_t height, const core::PixelFormat format, const core::MultiSample sample, const core::ClearValue& clearValue)
{
	GPUTextureMetaData metaData = {};
	metaData.Width            = width;
	metaData.Height           = height;
	metaData.DepthOrArraySize = 1;
	metaData.PixelFormat      = format;
	metaData.MipLevels        = 1;
	metaData.ResourceUsage    = core::ResourceUsage::DepthStencil;
	metaData.State            = ResourceState::DepthStencil;
	metaData.Dimension        = core::ResourceDimension::Dimension2D;
	metaData.ResourceType     = core::ResourceType::Texture2DMultiSample;
	metaData.HeapType         = core::MemoryHeap::Default;
	metaData.Sample           = sample;
	metaData.ClearColor       = clearValue;
	metaData.CalculateByteSize();
	return metaData;
}
#pragma endregion GPUTexture
#pragma region BlendProperty
BlendProperty BlendProperty::NoColorWrite(const bool useAlphaToCoverage)
{
	return BlendProperty(BlendOperator::Add, BlendOperator::Add,
		BlendFactor::Inverse_Source_Alpha, BlendFactor::Inverse_Source_Alpha,
		BlendFactor::One, BlendFactor::Source_Alpha,
		ColorMask::None, useAlphaToCoverage, false);
}

BlendProperty BlendProperty::OverWrite(const bool useAlphaToCoverage)
{
	return BlendProperty(BlendOperator::Add, BlendOperator::Add,
		BlendFactor::Inverse_Source_Alpha, BlendFactor::Inverse_Source_Alpha,
		BlendFactor::One, BlendFactor::Source_Alpha,
		ColorMask::All, useAlphaToCoverage, false);
}

BlendProperty BlendProperty::AlphaBlend(const bool useAlphaToCoverage)
{
	return BlendProperty(BlendOperator::Add, BlendOperator::Add,
		BlendFactor::Inverse_Source_Alpha, BlendFactor::Inverse_Source_Alpha,
		BlendFactor::One, BlendFactor::Source_Alpha,
		ColorMask::All, useAlphaToCoverage, true);
}
#pragma endregion      BlendProperty
#pragma region RasterizerProperty
RasterizerProperty RasterizerProperty::Solid(const bool useMultiSample, const FrontFace frontFace, const CullingMode cullingMode)
{
	return RasterizerProperty(frontFace, cullingMode, FillMode::Solid, true, useMultiSample);
}

RasterizerProperty RasterizerProperty::WireFrame(const bool useMultiSample, const FrontFace frontFace, const CullingMode cullingMode)
{
	return RasterizerProperty(frontFace, cullingMode, FillMode::Solid, true, useMultiSample);
}
#pragma endregion RasterizerProperty