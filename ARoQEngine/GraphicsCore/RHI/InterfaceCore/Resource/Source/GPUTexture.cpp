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
gu::uint16 GPUTexture::GetDepth(const gu::uint8 mipLevel) const noexcept
{
	if (IsArray()) 
	{ 
		return 1; 
	}

	return _metaData.DepthOrArraySize > 1u << mipLevel ? 1u : _metaData.DepthOrArraySize >> mipLevel;
}

gu::uint64 GPUTexture::GetByteSize(const gu::uint8 mipLevel) const noexcept
{
	return GetWidth(mipLevel) * GetHeight(mipLevel) * GetDepth(mipLevel)
		* core::PixelFormatInfo::GetConst(GetPixelFormat()).BlockBytes
		* static_cast<gu::uint64>(GetMultiSample());
}