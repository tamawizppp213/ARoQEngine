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
#include "GameUtility/Base/Include/GUMemory.hpp"

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
		using LowLevelGraphicsEnginePtr = gu::SharedPointer<LowLevelGraphicsEngine>;
		using ResourceViewPtr           = gu::SharedPointer<rhi::core::GPUResourceView>;
		using ResourceLayoutPtr         = gu::SharedPointer<rhi::core::RHIResourceLayout>;
		using PipelineStatePtr          = gu::SharedPointer<rhi::core::GPUGraphicsPipelineState>;
		using GameModelPtr              = gu::SharedPointer<gc::core::GameModel>;
	
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