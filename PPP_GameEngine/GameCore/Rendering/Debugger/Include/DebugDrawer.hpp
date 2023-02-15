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
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include "GameUtility/Math/Include/GMVector.hpp"
#include <memory>
#include <vector>
#include <string>
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
	*************************************************************************//**
	*  @class     DebugDrawer
	*  @brief     Wire frame renderer
	*****************************************************************************/
	class DebugDrawer : public NonCopyable
	{
	protected:
		using LowLevelGraphicsEnginePtr = std::shared_ptr<LowLevelGraphicsEngine>;
		using PipelineStatePtr   = std::shared_ptr<rhi::core::GPUGraphicsPipelineState>;
		using ResourceLayoutPtr  = std::shared_ptr<rhi::core::RHIResourceLayout>;
		using GPUResourceViewPtr = std::shared_ptr<rhi::core::GPUResourceView>;
		using TexturePtr         = std::shared_ptr<rhi::core::GPUTexture>;
		using RenderPassPtr      = std::shared_ptr<rhi::core::RHIRenderPass>;
		using GameModelPtr       = std::shared_ptr<gc::core::GameModel>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Draw(const GPUResourceViewPtr& scene);

		void Add(const GameModelPtr& gameModel);

		void Clear();

		void Clear(const GameModelPtr& gameModel);

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		DebugDrawer(const LowLevelGraphicsEnginePtr& engine, const std::wstring& addName = L"");

		~DebugDrawer();

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		
		void PreparePipelineState(const std::wstring& name);

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		LowLevelGraphicsEnginePtr _engine = nullptr;

		std::vector<GameModelPtr> _gameModels = {};

		/*-------------------------------------------------------------------
		-          Render Resource
		---------------------------------------------------------------------*/
		PipelineStatePtr _pipeline = nullptr;
		
		ResourceLayoutPtr _resourceLayout = nullptr;
	};
}
#endif