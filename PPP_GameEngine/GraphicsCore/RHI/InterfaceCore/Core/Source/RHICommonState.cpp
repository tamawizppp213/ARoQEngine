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

#pragma region GPUTexture
GPUTextureMetaData GPUTextureMetaData::Texture1D(const size_t width, const core::PixelFormat format, const size_t mipLevels, const core::ResourceUsage usage)
{
	GPUTextureMetaData metaData = {};
	metaData.Width         = width;
	metaData.Height        = 1;
	metaData.Depth         = 1;
	metaData.PixelFormat   = format;
	metaData.MipLevels     = mipLevels;
	metaData.ResourceUsage = usage;
	metaData.Layout        = ResourceLayout::GeneralRead;
	metaData.Dimension     = core::ResourceDimension::Dimension1D;
	metaData.ResourceType  = core::ResourceType::Texture1D;
	metaData.HeapType      = core::MemoryHeap::Default;
	metaData.DescType      = DescriptorType::Texture;
	metaData.Sample        = MultiSample::Count1;
	metaData.CalculateByteSize();
	return metaData;
}
GPUTextureMetaData GPUTextureMetaData::Texture1DArray(const size_t width, const size_t length, const core::PixelFormat format, const size_t mipLevels, const core::ResourceUsage usage)
{
	GPUTextureMetaData metaData = {};
	metaData.Width         = width;
	metaData.Height        = 1;
	metaData.Depth         = length;
	metaData.PixelFormat   = format;
	metaData.MipLevels     = mipLevels;
	metaData.ResourceUsage = usage;
	metaData.Layout        = ResourceLayout::GeneralRead;
	metaData.Dimension     = core::ResourceDimension::Dimension1D;
	metaData.ResourceType  = core::ResourceType::Texture1DArray;
	metaData.HeapType      = core::MemoryHeap::Default;
	metaData.DescType      = DescriptorType::Texture;
	metaData.Sample        = MultiSample::Count1;
	metaData.CalculateByteSize();
	return metaData;
}
GPUTextureMetaData GPUTextureMetaData::Texture2D(const size_t width, const size_t height, const core::PixelFormat format, const size_t mipLevels, const core::ResourceUsage usage)
{
	GPUTextureMetaData metaData = {};
	metaData.Width         = width;
	metaData.Height        = height;
	metaData.Depth         = 1;
	metaData.PixelFormat   = format;
	metaData.MipLevels     = mipLevels;
	metaData.ResourceUsage = usage;
	metaData.Layout        = ResourceLayout::GeneralRead;
	metaData.Dimension     = core::ResourceDimension::Dimension2D;
	metaData.ResourceType  = core::ResourceType::Texture2D;
	metaData.HeapType      = core::MemoryHeap::Default;
	metaData.DescType      = DescriptorType::Texture;
	metaData.Sample        = MultiSample::Count1;
	metaData.CalculateByteSize();
	return metaData;
}
GPUTextureMetaData GPUTextureMetaData::Texture2DArray(const size_t width, const size_t height, const size_t length, const core::PixelFormat format, const size_t mipLevels, const core::ResourceUsage usage)
{
	GPUTextureMetaData metaData = {};
	metaData.Width         = width;
	metaData.Height        = height;
	metaData.Depth         = length;
	metaData.PixelFormat   = format;
	metaData.MipLevels     = mipLevels;
	metaData.ResourceUsage = usage;
	metaData.Layout        = ResourceLayout::GeneralRead;
	metaData.Dimension     = core::ResourceDimension::Dimension2D;
	metaData.ResourceType  = core::ResourceType::Texture2D;
	metaData.HeapType      = core::MemoryHeap::Default;
	metaData.DescType      = DescriptorType::Texture;
	metaData.Sample        = MultiSample::Count1;
	metaData.CalculateByteSize();
	return metaData;
}
GPUTextureMetaData GPUTextureMetaData::Texture3D(const size_t width, const size_t height, const size_t depth, const core::PixelFormat format, const size_t mipLevels, const core::ResourceUsage usage)
{
	GPUTextureMetaData metaData = {};
	metaData.Width         = width;
	metaData.Height        = height;
	metaData.Depth         = depth;
	metaData.PixelFormat   = format;
	metaData.MipLevels     = mipLevels;
	metaData.ResourceUsage = usage;
	metaData.Layout        = ResourceLayout::GeneralRead;
	metaData.Dimension     = core::ResourceDimension::Dimension3D;
	metaData.ResourceType  = core::ResourceType::Texture3D;
	metaData.HeapType      = core::MemoryHeap::Default;
	metaData.DescType      = DescriptorType::Texture;
	metaData.Sample        = MultiSample::Count1;
	metaData.CalculateByteSize();
	return metaData;
}
GPUTextureMetaData GPUTextureMetaData::Texture2DMultiSample(const size_t width, const size_t height, const core::PixelFormat format, const core::MultiSample sample, const size_t mipLevels, const core::ResourceUsage usage)
{
	GPUTextureMetaData metaData = {};
	metaData.Width         = width;
	metaData.Height        = height;
	metaData.Depth         = 1;
	metaData.PixelFormat   = format;
	metaData.MipLevels     = mipLevels;
	metaData.ResourceUsage = usage;
	metaData.Layout        = ResourceLayout::GeneralRead;
	metaData.Dimension     = core::ResourceDimension::Dimension2D;
	metaData.ResourceType  = core::ResourceType::Texture2DMultiSample;
	metaData.HeapType      = core::MemoryHeap::Default;
	metaData.DescType      = DescriptorType::Texture;
	metaData.Sample        = sample;
	metaData.CalculateByteSize();
	return metaData;
}
GPUTextureMetaData GPUTextureMetaData::Texture2DArrayMultiSample(const size_t width, const size_t height, const size_t length, const core::PixelFormat format, const core::MultiSample sample, const size_t mipLevels, const core::ResourceUsage usage)
{
	GPUTextureMetaData metaData = {};
	metaData.Width         = width;
	metaData.Height        = height;
	metaData.Depth         = length;
	metaData.PixelFormat   = format;
	metaData.MipLevels     = mipLevels;
	metaData.ResourceUsage = usage;
	metaData.Layout        = ResourceLayout::GeneralRead;
	metaData.Dimension     = core::ResourceDimension::Dimension2D;
	metaData.ResourceType  = core::ResourceType::Texture2DArrayMultiSample;
	metaData.HeapType      = core::MemoryHeap::Default;
	metaData.DescType      = DescriptorType::Texture;
	metaData.Sample        = sample;
	metaData.CalculateByteSize();
	return metaData;
}
GPUTextureMetaData GPUTextureMetaData::CubeMap(const size_t width, const size_t height, const core::PixelFormat format, const size_t mipLevels, const core::ResourceUsage usage)
{
	GPUTextureMetaData metaData = {};
	metaData.Width         = width;
	metaData.Height        = height;
	metaData.Depth         = 6;
	metaData.PixelFormat   = format;
	metaData.MipLevels     = mipLevels;
	metaData.ResourceUsage = usage;
	metaData.Layout        = ResourceLayout::GeneralRead;
	metaData.Dimension     = core::ResourceDimension::Dimension2D;
	metaData.ResourceType  = core::ResourceType::TextureCube;
	metaData.HeapType      = core::MemoryHeap::Default;
	metaData.DescType      = DescriptorType::Texture;
	metaData.Sample        = MultiSample::Count1;
	metaData.CalculateByteSize();
	return metaData;
}
GPUTextureMetaData GPUTextureMetaData::CubeMapArray(const size_t width, const size_t height, const size_t length, const core::PixelFormat format, const size_t mipLevels, const core::ResourceUsage usage)
{
	GPUTextureMetaData metaData = {};
	metaData.Width         = width;
	metaData.Height        = height;
	metaData.Depth         = 6 * length;
	metaData.PixelFormat   = format;
	metaData.MipLevels     = mipLevels;
	metaData.ResourceUsage = usage;
	metaData.Layout        = ResourceLayout::GeneralRead;
	metaData.Dimension     = core::ResourceDimension::Dimension2D;
	metaData.ResourceType  = core::ResourceType::TextureCubeArray;
	metaData.HeapType      = core::MemoryHeap::Default;
	metaData.DescType      = DescriptorType::Texture;
	metaData.Sample        = MultiSample::Count1;
	metaData.CalculateByteSize();
	return metaData;
}
GPUTextureMetaData GPUTextureMetaData::RenderTarget(const size_t width, const size_t height, const core::PixelFormat format, const core::ClearValue& clearValue)
{
	GPUTextureMetaData metaData = {};
	metaData.Width         = width;
	metaData.Height        = height;
	metaData.Depth         = 1;
	metaData.PixelFormat   = format;
	metaData.MipLevels     = 1;
	metaData.ResourceUsage = core::ResourceUsage::RenderTarget;
	metaData.Layout        = ResourceLayout::GeneralRead;
	metaData.Dimension     = core::ResourceDimension::Dimension2D;
	metaData.ResourceType  = core::ResourceType::Texture2D;
	metaData.HeapType      = core::MemoryHeap::Default;
	metaData.DescType      = DescriptorType::Texture;
	metaData.Sample        = MultiSample::Count1;
	metaData.ClearColor    = clearValue;
	metaData.CalculateByteSize();
	return metaData;

}
GPUTextureMetaData GPUTextureMetaData::RenderTargetMultiSample(const size_t width, const size_t height, const core::PixelFormat format, const core::MultiSample sample, const core::ClearValue& clearValue)
{
	GPUTextureMetaData metaData = {};
	metaData.Width         = width;
	metaData.Height        = height;
	metaData.Depth         = 1;
	metaData.PixelFormat   = format;
	metaData.MipLevels     = 1;
	metaData.ResourceUsage = core::ResourceUsage::RenderTarget;
	metaData.Layout        = ResourceLayout::GeneralRead;
	metaData.Dimension     = core::ResourceDimension::Dimension2D;
	metaData.ResourceType  = core::ResourceType::Texture2DMultiSample;
	metaData.HeapType      = core::MemoryHeap::Default;
	metaData.DescType      = DescriptorType::Texture;
	metaData.Sample        = sample;
	metaData.ClearColor    = clearValue;
	metaData.CalculateByteSize();
	return metaData;
}
GPUTextureMetaData GPUTextureMetaData::DepthStencil(const size_t width, const size_t height, const core::PixelFormat format, const core::ClearValue& clearValue)
{
	GPUTextureMetaData metaData = {};
	metaData.Width         = width;
	metaData.Height        = height;
	metaData.Depth         = 1;
	metaData.PixelFormat   = format;
	metaData.MipLevels     = 1;
	metaData.ResourceUsage = core::ResourceUsage::DepthStencil;
	metaData.Layout        = ResourceLayout::GeneralRead;
	metaData.Dimension     = core::ResourceDimension::Dimension2D;
	metaData.ResourceType  = core::ResourceType::Texture2D;
	metaData.HeapType      = core::MemoryHeap::Default;
	metaData.DescType      = DescriptorType::Texture;
	metaData.Sample        = MultiSample::Count1;
	metaData.ClearColor    = clearValue;
	metaData.CalculateByteSize();
	return metaData;
}
GPUTextureMetaData GPUTextureMetaData::DepthStencilMultiSample(const size_t width, const size_t height, const core::PixelFormat format, const core::MultiSample sample, const core::ClearValue& clearValue)
{
	GPUTextureMetaData metaData = {};
	metaData.Width         = width;
	metaData.Height        = height;
	metaData.Depth         = 1;
	metaData.PixelFormat   = format;
	metaData.MipLevels     = 1;
	metaData.ResourceUsage = core::ResourceUsage::DepthStencil;
	metaData.Layout        = ResourceLayout::GeneralRead;
	metaData.Dimension     = core::ResourceDimension::Dimension2D;
	metaData.ResourceType  = core::ResourceType::Texture2DMultiSample;
	metaData.HeapType      = core::MemoryHeap::Default;
	metaData.DescType      = DescriptorType::Texture;
	metaData.Sample        = sample;
	metaData.ClearColor    = clearValue;
	metaData.CalculateByteSize();
	return metaData;
}
#pragma endregion GPUTexture