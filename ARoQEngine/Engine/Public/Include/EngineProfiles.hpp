//////////////////////////////////////////////////////////////////////////////////
///             @file   PPPEngine.hpp
///             @brief  Engine
///             @author Toide Yutaro
///             @date   2023_09_24
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef PPP_ENGINE_PROFILES_HPP
#define PPP_ENGINE_PROFILES_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "Platform/Core/Include/CoreWindow.hpp"
#include "Platform/Core/Include/CorePlatformCommand.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace engine
{
	struct GraphicsSettings
	{
		/*! @brief 本エンジンで使用するグラフィクスAPI*/
		rhi::core::GraphicsAPI APIversion = rhi::core::GraphicsAPI::DirectX12;

		/*! @brief フレームバッファの数 (デフォルトは3)*/
		gu::uint8 FrameBufferCount = 3;

		/*! @brief 垂直同期の使用 (0は使用しない, 1以上からfpsが半減していく)*/
		gu::uint8 VSync = 0;
	};

	struct StartUpParameters
	{
		platform::core::CoreWindowDesc WindowSettings = {};
		GraphicsSettings GraphicsSettings = {};
	};
}
#endif