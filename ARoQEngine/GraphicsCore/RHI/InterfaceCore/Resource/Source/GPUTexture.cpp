//////////////////////////////////////////////////////////////////////////////////
//              @file   GPUBuffer.cpp
///             @brief  GPUBuffer
///             @author Toide Yutaro
///             @date   2022_07_09
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUTexture.hpp"
#include <algorithm>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::core;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
size_t GPUTexture::GetWidth(const size_t mipLevel) const noexcept
{
	auto result = _metaData.Width;
	for (size_t index = 0; index < mipLevel; ++index)
	{
		result = result / 2 > static_cast<size_t>(1) ? result/2 : static_cast<size_t>(1);
	}
	return result;
}

size_t GPUTexture::GetHeight(const size_t mipLevel) const noexcept
{
	auto result = _metaData.Height;
	for (size_t index = 0; index < mipLevel; ++index)
	{
		result = result / 2 > static_cast<size_t>(1) ? result / 2 : static_cast<size_t>(1);
	}
	return result;
}

size_t GPUTexture::GetDepth(const size_t mipLevel) const noexcept
{
	auto result = _metaData.DepthOrArraySize;
	if (IsArray()) 
	{ 
		return 1; 
	}

	for (size_t index = 0; index < mipLevel; ++index)
	{
		result = result / 2 > static_cast<size_t>(1) ? result / 2 : static_cast<size_t>(1);
	}
	return result;
}

size_t GPUTexture::GetByteSize(const size_t mipLevel) const noexcept
{
	return GetWidth(mipLevel) * GetHeight(mipLevel) * GetDepth(mipLevel) * core::PixelFormatSizeOf::Get(GetPixelFormat()) * static_cast<gu::uint64>(GetMultiSample());
}