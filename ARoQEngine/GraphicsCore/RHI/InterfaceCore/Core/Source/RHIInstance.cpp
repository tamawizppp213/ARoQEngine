//////////////////////////////////////////////////////////////////////////////////
/// @file   RHIInstance.cpp
/// @brief  GraphicsAPIを選択し, 以降でGraphicsAPIに対応するRenderResourceを呼び出すためのクラス@n
///         そのほか, 物理デバイスの選択やCPU, GPUデバッガの有無なども登録することが出来ます.@n 
/// @author Toide Yutaro
/// @date   2022_09_05
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIMacro.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIInstance.hpp"
// DirectX12
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Instance.hpp"
// Vulkan
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanInstance.hpp"
#include "stdio.h"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::core;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
/****************************************************************************
*                     CreateInstance
*************************************************************************//**
*  @fn        gu::SharedPointer<RHIInstance> rhi::core::RHIInstance::CreateInstance(const core::GraphicsAPI graphicsAPI, const RHIDebugCreateInfo& debugCreateInfo)
*
*  @brief     手動でGraphics APIを選択します. @n
              そのほかCPU, GPUデバッガを使用するかも選択できますが, リリースモードでは使用することが出来ません
*
*  @param[in] const core::GraphicsAPI グラフィクスAPI
*  @param[in] const RHIInstanceCreateInfo デバッガの指定用構造体
*
*  @return    void
*****************************************************************************/
gu::SharedPointer<RHIInstance> rhi::core::RHIInstance::CreateInstance(const core::GraphicsAPI graphicsAPI, const RHIDebugCreateInfo& debugCreateInfo)
{
	switch (graphicsAPI)
	{
		case GraphicsAPI::DirectX12: 
		{
			printf("DirectX12\n"); 
			return gu::MakeShared<rhi::directX12::RHIInstance>(debugCreateInfo);
		}
		case GraphicsAPI::Vulkan: 
		{
			printf("Vulkan\n"); 
			return gu::MakeShared<rhi::vulkan::RHIInstance>(debugCreateInfo); 
		}
		default:
		{
			Confirmf(false, "Unknown API");
		}
	}
}

/****************************************************************************
*                     CreateInstance
*************************************************************************//**
*  @fn        gu::SharedPointer<RHIInstance> rhi::core::RHIInstance::CreateInstance(const core::RHIDebugCreateInfo& debugCreateInfo)
*
*  @brief     プラットフォームに合わせて自動でGraphics APIを選択します. @n
              そのほかCPU, GPUデバッガを使用するかも選択できますが, リリースモードでは使用することが出来ません
*
*  @param[in] const RHIInstanceCreateInfo デバッガの指定用構造体
*
*  @return    void
*****************************************************************************/
gu::SharedPointer<RHIInstance> rhi::core::RHIInstance::CreateInstance(const core::RHIDebugCreateInfo& debugCreateInfo)
{
	return gu::MakeShared<DEFAULT_GRAPHICS_API::RHIInstance>(debugCreateInfo);
}