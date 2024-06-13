//////////////////////////////////////////////////////////////////////////////////
///             @file   DebugDrawer.hpp
///             @brief  Wire frame drawing.
///             @author Toide Yutaro
///             @date   2023_02_07
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DEBUG_DRAWER_HPP
#define DEBUG_DRAWER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Math/Include/GMVector.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Base/Include/GUString.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class LowLevelGraphicsEngine;

namespace rhi::core
{
	class RHIResourceLayout;
	class RHIFrameBuffer;
	class RHIRenderPass;
	class GPUResourceView;
	class GPUGraphicsPipelineState;
	class GPUTexture;
	class GPUBuffer;
}

namespace gc::core
{
	class GameModel;
}
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gc::rendering
{
	/****************************************************************************
	*				  			    DebugDrawer
	****************************************************************************/
	/* @class     DebugDrawer
	*  @brief     Wire frame renderer
	*****************************************************************************/
	class DebugDrawer : public gu::NonCopyable
	{
	protected:
		using LowLevelGraphicsEnginePtr = gu::SharedPointer<LowLevelGraphicsEngine>;
		using PipelineStatePtr   = gu::SharedPointer<rhi::core::GPUGraphicsPipelineState>;
		using ResourceLayoutPtr  = gu::SharedPointer<rhi::core::RHIResourceLayout>;
		using GPUResourceViewPtr = gu::SharedPointer<rhi::core::GPUResourceView>;
		using TexturePtr         = gu::SharedPointer<rhi::core::GPUTexture>;
		using RenderPassPtr      = gu::SharedPointer<rhi::core::RHIRenderPass>;
		using GameModelPtr       = gu::SharedPointer<gc::core::GameModel>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Draw(const GPUResourceViewPtr& scene);

		void Add(const GameModelPtr& gameModel);

		void Clear();

		void Clear(const GameModelPtr& gameModel);

		/****************************************************************************
		**                Public Property
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		DebugDrawer(const LowLevelGraphicsEnginePtr& engine, const gu::tstring& addName = SP(""));

		~DebugDrawer();

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		
		void PreparePipelineState(const gu::tstring& name);

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		LowLevelGraphicsEnginePtr _engine = nullptr;

		gu::DynamicArray<GameModelPtr> _gameModels = {};

		/*-------------------------------------------------------------------
		-          Render Resource
		---------------------------------------------------------------------*/
		PipelineStatePtr _pipeline = nullptr;
		
		ResourceLayoutPtr _resourceLayout = nullptr;
	};
}
#endif