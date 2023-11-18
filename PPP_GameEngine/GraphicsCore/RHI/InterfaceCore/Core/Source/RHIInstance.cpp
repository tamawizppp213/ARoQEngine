//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIInstance.hpp
///             @brief  Select device api (このエンジンを使用時最初に呼び出す.) 
///             @author Toide Yutaro
///             @date   2022_09_05
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIInstance.hpp"
// DirectX12
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Instance.hpp"
// Vulkan
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanInstance.hpp"

// common
#include <stdexcept>
#include <iostream>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::core;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
gu::SharedPointer<RHIInstance> rhi::core::RHIInstance::CreateInstance(const rhi::core::APIVersion version, bool enableCPUDebugger, bool enableGPUDebugger, bool useGPUDebugBreak)
{
	switch (version)
	{
		case APIVersion::DirectX12: { std::cout << "DirectX12" << std::endl; return gu::MakeShared<rhi::directX12::RHIInstance>(enableCPUDebugger,enableGPUDebugger, useGPUDebugBreak);}
		case APIVersion::Vulkan:    { std::cout << "Vulkan"    << std::endl; return gu::MakeShared<rhi::vulkan::RHIInstance>(enableCPUDebugger, enableGPUDebugger, useGPUDebugBreak); }
		default: { throw std::runtime_error("Unknown API."); }
	}
}

