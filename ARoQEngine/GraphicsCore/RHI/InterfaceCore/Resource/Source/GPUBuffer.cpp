//////////////////////////////////////////////////////////////////////////////////
//              @file   GPUBuffer.cpp
///             @brief  GPUBuffer
///             @author Toide Yutaro
///             @date   2022_07_09
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUBuffer.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::core;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
namespace
{
	static inline gu::uint32 CalcConstantBufferByteSize(const gu::uint32 byteSize) { return (byteSize + 255) & ~255; }
	static inline gu::uint32 AlignmentValue(const gu::uint32 size, const gu::uint32 alignment) { return (size + alignment - (size % alignment)); }
}

GPUBuffer::GPUBuffer(const gu::SharedPointer<RHIDevice>& device, const core::GPUBufferMetaData& metaData, [[maybe_unused]]const gu::tstring& name)
	: GPUResource(device), _metaData(metaData)
{
	/*-------------------------------------------------------------------
	-           Constant Buffer Check (for 256 byte alignment )
	---------------------------------------------------------------------*/
	bool isConstantBuffer = gu::HasAnyFlags(GetUsage(), ResourceUsage::ConstantBuffer);

	/*-------------------------------------------------------------------
	-          Set Stride and Element Count
	---------------------------------------------------------------------*/
	// If you select the constant buffer, 256 byte alignment is needed.
	_metaData.Stride   = isConstantBuffer ? static_cast<size_t>(CalcConstantBufferByteSize((gu::uint32)_metaData.Stride)) : _metaData.Stride;

}
