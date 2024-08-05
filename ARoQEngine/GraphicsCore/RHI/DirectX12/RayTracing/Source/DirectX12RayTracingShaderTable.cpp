//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12RayTracingGeometry.hpp
///             @brief  RayTracing geometry descriptor
///             @author Toide Yutaro
///             @date   2022_11_22
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/RayTracing/Include/DirectX12RayTracingShaderTable.hpp"
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUBuffer.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12EnumConverter.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Device.hpp"
#include <stdexcept>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#define INVALID_VALUE 0
using namespace rhi;
using namespace rhi::directX12;

//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////

#pragma region Constructor and Destructor 
RayTracingShaderTable::RayTracingShaderTable(const gu::SharedPointer<rhi::core::RHIDevice>& device) 
	: rhi::core::RayTracingShaderTable(device)
{
	//const auto recordSize            = D3D12_SHADER_IDENTIFIER_SIZE_IN_BYTES;         // 32 byte
	//const auto shaderRecordAlignment = D3D12_RAYTRACING_SHADER_RECORD_BYTE_ALIGNMENT; // 32 byte


	const auto totalByteSize = 0;

	/*-------------------------------------------------------------------
	-         Prepare buffer meta data
	---------------------------------------------------------------------*/
	rhi::core::GPUBufferMetaData metaData = {};
	metaData.UploadBuffer(totalByteSize, 1, rhi::core::MemoryHeap::Upload, nullptr);

	/*-------------------------------------------------------------------
	-         Build shader table
	---------------------------------------------------------------------*/
	_buffer = _device->CreateBuffer(metaData);

	/*-------------------------------------------------------------------
	-         Bind buffer table to the Dispatch rays descriptor
	---------------------------------------------------------------------*/
	const auto dxBuffer = gu::StaticPointerCast<directX12::GPUBuffer>(_buffer);

	auto eachRecordStartAddress = dxBuffer->GetResourcePtr()->GetGPUVirtualAddress();

	// ray generation shader record
	_dispatchRaysDesc.RayGenerationShaderRecord = D3D12_GPU_VIRTUAL_ADDRESS_RANGE
	{
		.StartAddress = eachRecordStartAddress,
		.SizeInBytes  = INVALID_VALUE	
	};
	eachRecordStartAddress += INVALID_VALUE; 

	// miss shader record
	_dispatchRaysDesc.MissShaderTable = D3D12_GPU_VIRTUAL_ADDRESS_RANGE_AND_STRIDE
	{
		.StartAddress  = eachRecordStartAddress,
		.SizeInBytes   = INVALID_VALUE,
		.StrideInBytes = INVALID_VALUE
	};
	eachRecordStartAddress += INVALID_VALUE;

	// hit group table shader record
	_dispatchRaysDesc.HitGroupTable = D3D12_GPU_VIRTUAL_ADDRESS_RANGE_AND_STRIDE
	{
		.StartAddress  = eachRecordStartAddress,
		.SizeInBytes   = INVALID_VALUE,
		.StrideInBytes = INVALID_VALUE
	};
	eachRecordStartAddress += INVALID_VALUE;

	_dispatchRaysDesc.CallableShaderTable = D3D12_GPU_VIRTUAL_ADDRESS_RANGE_AND_STRIDE
	{
		.StartAddress  = eachRecordStartAddress,
		.SizeInBytes   = INVALID_VALUE,
		.StrideInBytes = INVALID_VALUE
	};
	eachRecordStartAddress += INVALID_VALUE;


	/*-------------------------------------------------------------------
	-         Write shader records
	---------------------------------------------------------------------*/
	//_buffer->CopyStart();

	//_buffer->CopyEnd();
}
#pragma endregion Constructor and Destructor 