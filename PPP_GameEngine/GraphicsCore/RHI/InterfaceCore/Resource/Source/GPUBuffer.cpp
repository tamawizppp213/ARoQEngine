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

GPUBuffer::GPUBuffer(const std::shared_ptr<RHIDevice>& device, std::uint32_t stride, std::uint32_t count, BufferType bufferType)
{
	/*-------------------------------------------------------------------
	-           Constant Buffer Check (for 256 byte alignment )
	---------------------------------------------------------------------*/
	bool isConstantBuffer = bufferType == BufferType::Constant;
	/*-------------------------------------------------------------------
	-          Set Stride and Element Count
	---------------------------------------------------------------------*/
	_stride = isConstantBuffer ? CalcConstantBufferByteSize(stride) : stride;
	_count = count;
	/*_resourceLayout = ResourceLayout::GeneralRead;
	switch (bufferType)
	{
		case BufferType::Vertex:
		case BufferType::Index:
		{
			_memoryHeap     = MemoryHeap::Default;
			break;
		}
		case BufferType::Constant:
		case BufferType::Upload:
		{
			_memoryHeap     = MemoryHeap::Upload;
			break;
		}
	}*/
}