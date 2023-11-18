//////////////////////////////////////////////////////////////////////////////////
///             @file   FullScreenEffector.hpp
///             @brief  This class is mainly used for the sake of post effect.
///                     This class provides full screen polygon and drawing interface.
///             @author Toide Yutaro
///             @date   2023_02_20
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef FULL_SCREEN_EFFECTOR_HPP
#define FULL_SCREEN_EFFECTOR_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include <vector>
#include <string>
#include "GameUtility/Base/Include/GUSharedPointer.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class LowLevelGraphicsEngine;
namespace rhi::core
{
	class RHIResourceLayout;
	class GPUResourceView;
	class GPUGraphicsPipelineState;
	class GPUBuffer;
	class GPUTexture;
}
//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc
{
	/****************************************************************************
	*				  			IFullScreenEffector
	*************************************************************************//**
	*  @class     IFullScreenEffector
	*  @brief     Provide the full screen polygon and drawing interface.
	*****************************************************************************/
	class IFullScreenEffector : public NonCopyable
	{
	protected:
		using VertexBufferPtr           = gu::SharedPointer<rhi::core::GPUBuffer>;
		using IndexBufferPtr            = gu::SharedPointer<rhi::core::GPUBuffer>;
		using ResourceLayoutPtr         = gu::SharedPointer<rhi::core::RHIResourceLayout>;
		using ResourceViewPtr           = gu::SharedPointer<rhi::core::GPUResourceView>;
		using PipelineStatePtr          = gu::SharedPointer<rhi::core::GPUGraphicsPipelineState>;
		using LowLevelGraphicsEnginePtr = gu::SharedPointer<LowLevelGraphicsEngine>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Resize frame buffer (Not implement)*/
		virtual void OnResize(int newWidth, int newHeight) = 0;

		/*@brief : Render to back buffer*/
		virtual void Draw() = 0;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/


	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		IFullScreenEffector();

		virtual ~IFullScreenEffector();

		IFullScreenEffector(const LowLevelGraphicsEnginePtr& engine);
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		virtual void PreparePipelineState(const std::wstring& addName) = 0;

		virtual void PrepareResourceView() = 0;
		
		void PrepareVertexAndIndexBuffer(const std::wstring& addName);
		
		std::wstring DefineDebugName(const std::wstring& addName = L"")
		{
			std::wstring name = L""; if (addName != L"") { name += addName; name += L"::"; }
			name += L"FSEffector::"; // 後に各リソース名称が追加されます.
			return name;
		}

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		/* @brief : frame resources*/
		std::vector<VertexBufferPtr> _vertexBuffers = {};
		
		std::vector<IndexBufferPtr>  _indexBuffers = {};
		
		PipelineStatePtr  _pipeline       = nullptr;
		
		ResourceLayoutPtr _resourceLayout = nullptr;
		
		std::vector<ResourceViewPtr> _resourceViews = {};

		std::int32_t _width  = 0;
		std::int32_t _height = 0;

		/* @brief : device and command list*/
		LowLevelGraphicsEnginePtr _engine = nullptr;
	};
}
#endif