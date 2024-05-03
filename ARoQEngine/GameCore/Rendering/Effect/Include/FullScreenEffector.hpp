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
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
#include "GameUtility/Base/Include/GUString.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
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
	class IFullScreenEffector : public gu::NonCopyable
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
		**                Public Property
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
		virtual void PreparePipelineState(const gu::tstring& addName) = 0;

		virtual void PrepareResourceView() = 0;
		
		void PrepareVertexAndIndexBuffer(const gu::tstring& addName);
		
		gu::tstring DefineDebugName(const gu::tstring& addName = SP(""))
		{
			gu::tstring name = SP(""); if (addName != SP("")) { name += addName; name += SP("::"); }
			name += SP("FSEffector::"); // 後に各リソース名称が追加されます.
			return name;
		}

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		/* @brief : frame resources*/
		gu::DynamicArray<VertexBufferPtr> _vertexBuffers = {};
		
		gu::DynamicArray<IndexBufferPtr>  _indexBuffers = {};
		
		PipelineStatePtr  _pipeline       = nullptr;
		
		ResourceLayoutPtr _resourceLayout = nullptr;
		
		gu::DynamicArray<ResourceViewPtr> _resourceViews = {};

		std::int32_t _width  = 0;
		std::int32_t _height = 0;

		/* @brief : device and command list*/
		LowLevelGraphicsEnginePtr _engine = nullptr;
	};
}
#endif