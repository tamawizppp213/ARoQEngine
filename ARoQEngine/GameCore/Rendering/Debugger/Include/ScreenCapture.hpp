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
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
#include <string>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class Keyboard;
class LowLevelGraphicsEngine;
namespace rhi::core
{
	class RHIFrameBuffer;
	class GPUTexture;
}
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace engine
{
	/****************************************************************************
	*				  			    ScreenCapture
	****************************************************************************/
	/* @class     ScreenCapture
	*  @brief     If you press the PrintScreen button, The texture is saved in the current time directory.
	              This class is used when executing in the debug mode 
	*****************************************************************************/
	class ScreenCapture : public gu::NonCopyable
	{
	protected:
		using KeyboardPtr    = gu::SharedPointer<Keyboard>;
		using TexturePtr     = gu::SharedPointer<rhi::core::GPUTexture>;
		using FrameBufferPtr = gu::SharedPointer<rhi::core::RHIFrameBuffer>;
		using LowLevelGraphicsEnginePtr = gu::SharedPointer<LowLevelGraphicsEngine>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		bool Capture(const TexturePtr& texture);

		bool Capture(const gu::DynamicArray<TexturePtr>& textures);

		bool Capture([[maybe_unused]]const FrameBufferPtr& frameBuffer);

		/****************************************************************************
		**                Public Property
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		ScreenCapture(const LowLevelGraphicsEnginePtr& engine, const KeyboardPtr& keyboard) : _engine(engine), _keyboard(keyboard){};

		virtual ~ScreenCapture() = default;

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		std::wstring CreateFileDirectoryAndPath(const size_t index = 0);

		std::wstring GetCurrentDateTime();
		
		bool IsTriggerPrintScreenKey();

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		LowLevelGraphicsEnginePtr _engine = nullptr;
		
		KeyboardPtr _keyboard = nullptr;
	};
}
#endif