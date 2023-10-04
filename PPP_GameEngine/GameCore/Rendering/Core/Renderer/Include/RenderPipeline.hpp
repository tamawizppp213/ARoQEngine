//////////////////////////////////////////////////////////////////////////////////
///             @file   RenderingEngine.hpp
///             @brief  RenderingEngine
///             @author Toide Yutaro
///             @date   2022_06_04
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RENDERING_PIPELINE_HPP
#define RENDERING_PIPELINE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include <memory>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class LowLevelGraphicsEngine;
namespace rhi::core
{
	class GPUResourceView;
	class RHIResourceLayout;
	class GPUGraphicsPipelineState;
}
namespace gc::core
{
	class GameModel;
}
//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc
{
	/****************************************************************************
	*				  			RenderingEngine
	*************************************************************************//**
	*  @class     RenderingEngine
	*  @brief     Rendering Engine
	*****************************************************************************/
	class IRenderPipeline : public NonCopyable
	{
	protected:
		using LowLevelGraphicsEnginePtr = std::shared_ptr<LowLevelGraphicsEngine>;
		using ResourceViewPtr           = std::shared_ptr<rhi::core::GPUResourceView>;
		using ResourceLayoutPtr         = std::shared_ptr<rhi::core::RHIResourceLayout>;
		using PipelineStatePtr          = std::shared_ptr<rhi::core::GPUGraphicsPipelineState>;
		using GameModelPtr              = std::shared_ptr<gc::core::GameModel>;
	
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		virtual bool Draw() = 0;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		virtual ResourceViewPtr GetSceneView() const noexcept = 0;
		virtual void SetSceneView(const ResourceViewPtr& scene) = 0;

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		IRenderPipeline(const LowLevelGraphicsEnginePtr& engine) : _engine(engine){};

		virtual ~IRenderPipeline();
	protected:
		/****************************************************************************
		**                Proteced Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		// äeÉÇÉfÉãÇë„ì¸èoóàÇÈÇÊÇ§Ç…Ç∑ÇÈ. 
		LowLevelGraphicsEnginePtr _engine = nullptr;
	};
}
#endif