//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12RayTracingBLASBuffer.hpp
///             @brief  BLAS Buffer
///             @author Toide Yutaro
///             @date   2022_11_23
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/RayTracing/Include/DirectX12RayTracingBLASBuffer.hpp"
#include "GraphicsCore/RHI/DirectX12/RayTracing/Include/DirectX12RayTracingGeometry.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Device.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12CommandList.hpp"
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUBuffer.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12EnumConverter.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
BLASBuffer::~BLASBuffer()
{

}
BLASBuffer::BLASBuffer(const std::shared_ptr<core::RHIDevice>& device,
	//const std::shared_ptr<core::GPUBuffer>& source,
	const std::vector<std::shared_ptr<core::RayTracingGeometry>>& geometryDesc,
	const core::BuildAccelerationStructureFlags flags)
	: core::BLASBuffer(device, geometryDesc, flags)
{
	const auto dxDevice = std::static_pointer_cast<directX12::RHIDevice>(_device);

	/*-------------------------------------------------------------------
	-         Push backs directX12 GeometryDesc
	---------------------------------------------------------------------*/
	std::vector<D3D12_RAYTRACING_GEOMETRY_DESC> dxGeometryDesc = {};
	for (const auto& desc : _geometryDescs)
	{
		dxGeometryDesc.emplace_back(std::static_pointer_cast<directX12::RayTracingGeometry>(desc)->GetDesc());
	}

	/*-------------------------------------------------------------------
	-         Set up acceleration structure inputs
	---------------------------------------------------------------------*/
	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS inputs = {};
	inputs.DescsLayout    = D3D12_ELEMENTS_LAYOUT_ARRAY;
	inputs.Flags          = EnumConverter::Convert(_flags);                            // build flag
	inputs.NumDescs       = static_cast<std::uint32_t>(dxGeometryDesc.size());         // geometry flag count
	inputs.pGeometryDescs = dxGeometryDesc.data(); 
	inputs.Type           = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL; // BLAS

	/*-------------------------------------------------------------------
	-         Get the size requirements for the scratch and AS buffers
	---------------------------------------------------------------------*/
	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO preBuildInfo = {};
	dxDevice->GetDevice()->GetRaytracingAccelerationStructurePrebuildInfo(&inputs, &preBuildInfo);

	/*-------------------------------------------------------------------
	-         Set up Scratch Buffer
	---------------------------------------------------------------------*/
	{
		core::GPUBufferMetaData metaData = core::GPUBufferMetaData::DefaultBuffer(preBuildInfo.ScratchDataSizeInBytes, 1);
		metaData.ResourceUsage = core::ResourceUsage::UnorderedAccess;
		metaData.State         = core::ResourceState::Common;
		_scratch = dxDevice->CreateBuffer(metaData);
	}

	/*-------------------------------------------------------------------
	-         Set up Destination Buffer
	---------------------------------------------------------------------*/
	{
		core::GPUBufferMetaData metaData = core::GPUBufferMetaData::DefaultBuffer(preBuildInfo.ResultDataMaxSizeInBytes, 1);
		metaData.ResourceUsage = core::ResourceUsage::UnorderedAccess;
		metaData.State         = core::ResourceState::RayTracingAccelerationStructure;
		_destination = dxDevice->CreateBuffer(metaData);
	}

	/*-------------------------------------------------------------------
	-         Create BLAS
	---------------------------------------------------------------------*/
	_rayTracingASDesc.Inputs = inputs;
	//_rayTracingASDesc.SourceAccelerationStructureData  = std::static_pointer_cast<directX12::GPUBuffer>(_source)->GetResource()->GetGPUVirtualAddress();
	_rayTracingASDesc.ScratchAccelerationStructureData = std::static_pointer_cast<directX12::GPUBuffer>(_scratch)->GetResource()->GetGPUVirtualAddress();
	_rayTracingASDesc.DestAccelerationStructureData    = std::static_pointer_cast<directX12::GPUBuffer>(_destination)->GetResource()->GetGPUVirtualAddress();
}
#pragma endregion Constructor and Destructor
#pragma region Build Function
void BLASBuffer::Build(const std::shared_ptr<core::RHICommandList>& commandList)
{
	/*-------------------------------------------------------------------
	-         Check Has Built 
	---------------------------------------------------------------------*/
	if (_hasBuilt) { return; }

	/*-------------------------------------------------------------------
	-         Prepare dxResource
	---------------------------------------------------------------------*/
	const auto dxDevice      = std::static_pointer_cast<directX12::RHIDevice>(_device);
	const auto dxCommandList = std::static_pointer_cast<directX12::RHICommandList>(commandList)->GetCommandList();
	const auto dxTLASBuffer  = std::static_pointer_cast<directX12::GPUBuffer>(_destination);

	/*-------------------------------------------------------------------
	-         Execute CommandList
	---------------------------------------------------------------------*/
	dxCommandList->BuildRaytracingAccelerationStructure(&_rayTracingASDesc, 0, nullptr);
	/*-------------------------------------------------------------------
	-         Resource barrier
	---------------------------------------------------------------------*/
	D3D12_RESOURCE_BARRIER uavBarrier = {};
	uavBarrier.Type          = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	uavBarrier.UAV.pResource = dxTLASBuffer->GetResourcePtr();
	dxCommandList->ResourceBarrier(1, &uavBarrier);

	_hasBuilt = true;
}
#pragma endregion Build Function