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
#include "GameUtility/Base/Include/GUParse.hpp"
#include "GameUtility/Base/Include/GUCommandLine.hpp"
#include "stdio.h"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::core;
using namespace gu;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Public Function
/*!**********************************************************************
*  @brief     手動でGraphics APIを選択します. @n
*             そのほかCPU, GPUデバッガを使用するかも選択できますが, リリースモードでは使用することが出来ません
*  @param[in] const core::GraphicsAPI グラフィクスAPI
*  @param[in] const RHIInstanceCreateInfo&  CPU, GPUデバッガの有無を指定する構造体
*  @return gu::SharedPointer<RHIInstance> RHIInstanceのポインタ
*************************************************************************/
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
			return nullptr;
		}
	}
}

/*!**********************************************************************
*  @brief     プラットフォームに合わせて自動でGraphics APIを選択します. @n
*             そのほかCPU, GPUデバッガを使用するかも選択できますが, リリースモードでは使用することが出来ません
*  @param[in] const RHIDebugCreateInfo& CPU, GPUデバッガの有無を指定する構造体
*  @return gu::SharedPointer<RHIInstance> RHIInstanceのポインタ
*************************************************************************/
gu::SharedPointer<RHIInstance> rhi::core::RHIInstance::CreateInstance(const core::RHIDebugCreateInfo& debugCreateInfo)
{
	return gu::MakeShared<DEFAULT_GRAPHICS_API::RHIInstance>(debugCreateInfo);
}

/*!**********************************************************************
*  @brief     物理デバイスの好みがある場合は, コマンドラインの結果に基づいてDisplayAdapterVenderTypeを返す @n
*             コマンドラインの文字列は, prefer_(DisplayAdapterVenderTypeに指定されるベンダー名)で指定してください
*  @details   コマンド一覧 @n
*             prefer_AMD, prefer_Intel, prefer_Nvidia, prefer_Microsoft
*************************************************************************/
DisplayAdapterVendorType rhi::core::RHIInstance::GetPreferredAdapterVendor() const
{
	if(Parse::Contains(CommandLine::Get(), SP("prefer_AMD")))
	{
		return DisplayAdapterVendorType::Amd;
	}

	if (Parse::Contains(CommandLine::Get(), SP("prefer_Intel")))
	{
		return DisplayAdapterVendorType::Intel;
	}

	if (Parse::Contains(CommandLine::Get(), SP("prefer_Nvidia")))
	{
		return DisplayAdapterVendorType::Nvidia;
	}

	if (Parse::Contains(CommandLine::Get(), SP("prefer_Microsoft")))
	{
		return DisplayAdapterVendorType::Microsoft;
	}

	return DisplayAdapterVendorType::Unknown;
}
#pragma endregion Public Function