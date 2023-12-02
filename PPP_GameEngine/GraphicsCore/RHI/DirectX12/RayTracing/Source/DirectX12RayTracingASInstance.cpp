//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12RayTracingBLASBuffer.hpp
///             @brief  BLAS Buffer
///             @author Toide Yutaro
///             @date   2022_11_23
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/RayTracing/Include/DirectX12RayTracingASInstance.hpp"
#include "GraphicsCore/RHI/DirectX12/RayTracing/Include/DirectX12RayTracingBLASBuffer.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Device.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12EnumConverter.hpp"
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUBuffer.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
ASInstance::~ASInstance()
{

}
ASInstance::ASInstance(
	const gu::SharedPointer<core::RHIDevice>& device,
	const gu::SharedPointer<core::BLASBuffer>& blasBuffer,
	const gm::Float3x4& blasTransform,
	const std::uint32_t instanceID,
	const std::uint32_t instanceContributionToHitGroupIndex,
	const std::uint32_t instanceMask,
	const core::RayTracingInstanceFlags flags)
	: core::ASInstance(device, blasBuffer, blasTransform, instanceID, instanceContributionToHitGroupIndex, instanceMask, flags)
{
	const auto dxBLASBuffer = gu::StaticPointerCast<directX12::BLASBuffer>(_blasBuffer);

	std::memcpy(_instanceDesc.Transform, &blasTransform, sizeof(_instanceDesc.Transform));
	_instanceDesc.InstanceID            = instanceID;
	_instanceDesc.InstanceMask          = instanceMask;
	_instanceDesc.Flags                 = EnumConverter::Convert(flags);
	_instanceDesc.AccelerationStructure = dxBLASBuffer->GetGPUAddress();
	_instanceDesc.InstanceContributionToHitGroupIndex = instanceContributionToHitGroupIndex;

}