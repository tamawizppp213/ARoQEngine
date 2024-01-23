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
	static inline std::uint32_t CalcConstantBufferByteSize(std::uint32_t byteSize) { return (byteSize + 255) & ~255; }
	static inline std::uint32_t AlignmentValue(std::uint32_t size, std::uint32_t alignment) { return (size + alignment - (size % alignment)); }
}

GPUBuffer::GPUBuffer(const gu::SharedPointer<RHIDevice>& device, const core::GPUBufferMetaData& metaData, [[maybe_unused]]const std::wstring& name)
	: GPUResource(device), _metaData(metaData)
{
	/*-------------------------------------------------------------------
	-           Constant Buffer Check (for 256 byte alignment )
	---------------------------------------------------------------------*/
	bool isConstantBuffer = _metaData.BufferType == BufferType::Constant;
	/*-------------------------------------------------------------------
	-          Set Stride and Element Count
	---------------------------------------------------------------------*/
	// If you select the constant buffer, 256 byte alignment is needed.
	_metaData.Stride   = isConstantBuffer ? static_cast<size_t>(CalcConstantBufferByteSize((std::uint32_t)_metaData.Stride)) : _metaData.Stride;
	_metaData.ByteSize = _metaData.Stride * _metaData.Count;
	_isTexture = false;
}

void GPUBuffer::Update(const void* data, const size_t dataLength)
{
	CopyStart();
	CopyTotalData(data, dataLength, 0);
	CopyEnd();
}