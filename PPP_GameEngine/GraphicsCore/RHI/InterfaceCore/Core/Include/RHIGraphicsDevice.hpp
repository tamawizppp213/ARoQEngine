//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIGraphicsDevice.hpp
///             @brief  Graphics Device
///             @author Toide Yutaro
///             @date   2022_06_09
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_GRAPHICS_DEVICE_HPP
#define RHI_GRAPHICS_DEVICE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include <Windows.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	/****************************************************************************
	*				  			RHIGraphicsDevice
	*************************************************************************//**
	*  @class     RHIGraphicsDevice
	*  @brief     Graphics Device
	*****************************************************************************/
	class RHIGraphicsDevice
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Start up graphics device. This function is called when start.*/
		virtual void StartUp(HWND hwnd, HINSTANCE hInstance) = 0;
		/* @brief : Begin draw frame. This function is called when draw frame start.*/
		virtual void BeginDrawFrame() = 0;
		/* @brief : End draw frame. This function is called when draw frame end.*/
		virtual void EndDrawFrame  () = 0;
		/* @brief : Shut down graphics device. This class is called when end.*/
		virtual void ShutDown() = 0;
		/* @brief : OnResize*/
		virtual void OnResize() = 0;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		static const UINT32 FRAME_BUFFER_COUNT = 3;
		static const UINT32 VSYNC              = 0; // 0: don't wait, 1:wait(60fps)
		
		int _currentFrame = 0;
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIGraphicsDevice() = default;
		virtual ~RHIGraphicsDevice() = default;
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		HWND      _hwnd      = nullptr;
		HINSTANCE _hInstance = nullptr;
		/** @brief use HDR Display format*/
		bool      _useHDR        = false;
		bool      _useRaytracing = true;
	};
}
#endif