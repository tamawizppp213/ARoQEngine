//////////////////////////////////////////////////////////////////////////////////
///             @file   LowLevelGraphicsEngine.hpp
///             @brief  Low level API grapchis engine
///             @author Toide Yutaro
///             @date   2022_03_11
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef LOW_LEVEL_GRAPHICS_ENGINE_HPP
#define LOW_LEVEL_GRAPHICS_ENGINE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
struct HWND__;
struct HINSTANCE__;
//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class RHIDevice;
	/****************************************************************************
	*				  			LowLevelGraphicsEngine
	*************************************************************************//**
	*  @class     LowLevelGraphicsEngine
	*  @brief     LowLevelGraphicsEngine
	*****************************************************************************/
	class LowLevelGraphicsEngine final : public NonCopyable
	{
		using HWND = HWND__*;
		using HINSTANCE = HINSTANCE__*;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void StartUp(APIVersion apiVersion, HWND hwnd, HINSTANCE hInstance);
		void OnResize();
		void ShutDown();
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		LowLevelGraphicsEngine() = default;
		~LowLevelGraphicsEngine() = default;
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		void SelectDevice(APIVersion apiVersion);
		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		/* @brief : Graphics API version. (DirectX12 or Vulkan)*/
		APIVersion _apiVersion = APIVersion::Unknown;
		/* @brief : Logical Device*/
		std::shared_ptr<RHIDevice> _device = nullptr;

		/* @brief : Windows API*/
		HWND      _hwnd      = nullptr;
		HINSTANCE _hInstance = nullptr;
	};
}
#endif