//////////////////////////////////////////////////////////////////////////////////
///             @file   ScreenCapture.hpp
///             @brief  Texture saving. 
///                     If you press the PrintScreen button, The texture is saved in the current time directory.
///                     This class is used when executing in the debug mode 
///             @author Toide Yutaro
///             @date   2023_02_07
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SCREEN_CAPTURE_HPP
#define SCREEN_CAPTURE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include <memory>
#include <vector>
#include <string>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class Keyboard;
namespace rhi::core
{
	class RHIFrameBuffer;
	class GPUTexture;
}
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gc::rendering
{
	/****************************************************************************
	*				  			    ScreenCapture
	*************************************************************************//**
	*  @class     ScreenCapture
	*  @brief     If you press the PrintScreen button, The texture is saved in the current time directory.
	              This class is used when executing in the debug mode 
	*****************************************************************************/
	class ScreenCapture : public NonCopyable
	{
	protected:
		using KeyboardPtr    = std::shared_ptr<Keyboard>;
		using TexturePtr     = std::shared_ptr<rhi::core::GPUTexture>;
		using FrameBufferPtr = std::shared_ptr<rhi::core::RHIFrameBuffer>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		bool Capture(const TexturePtr& texture);

		bool Capture(const std::vector<TexturePtr>& textures);

		bool Capture(const FrameBufferPtr& frameBuffer);

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		ScreenCapture(const KeyboardPtr& keyboard) : _keyboard(keyboard){};

		virtual ~ScreenCapture() = default;

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		std::wstring GetCurrentDataTime();

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		KeyboardPtr _keyboard = nullptr;
	};
}
#endif