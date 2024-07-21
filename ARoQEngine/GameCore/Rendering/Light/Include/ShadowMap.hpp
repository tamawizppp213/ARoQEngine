//////////////////////////////////////////////////////////////////////////////////
///             @file   ShadowMap.hpp
///             @brief  Draw shadow map
///             @author Toide Yutaro
///             @date   2023_04_11
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SHADOW_MAP_HPP
#define SHADOW_MAP_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHITypeCore.hpp"
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Base/Include/GUString.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class LowLevelGraphicsEngine;
namespace engine
{
	class GaussianBlur;
	class GameModel;
}
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace engine
{
	/****************************************************************************
	*				  			   ShadowMap
	****************************************************************************/
	/* @class     ShadowMap
	*  @brief     Rendering the shadow map
	*****************************************************************************/
	class ShadowMap : public gu::NonCopyable
	{
	protected:
		using LowLevelGraphicsEnginePtr = gu::SharedPointer<LowLevelGraphicsEngine>;
		using GameModelPtr              = gu::SharedPointer<GameModel>;
		using GaussianBlurPtr           = gu::SharedPointer<GaussianBlur>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Draw(const GPUResourceViewPtr& scene);

		void Add(const GameModelPtr& gameModel);

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		RHIFrameBufferPtr GetFrameBuffer() const noexcept { return _frameBuffer; }

		GPUResourceViewPtr GetHalfDownSampledSRV() const noexcept;

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		ShadowMap() = default;

		ShadowMap(const LowLevelGraphicsEnginePtr& engine, 
			const std::uint32_t width, 
			const std::uint32_t height,
			const gu::tstring& addName = SP(""));
		
		~ShadowMap();

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void PrepareVertexAndIndexBuffer(const gu::tstring& name);
		void PrepareRenderResource(const std::uint32_t width, const std::uint32_t height, const gu::tstring& name);
		void PreparePipelineState(const gu::tstring& name);

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		LowLevelGraphicsEnginePtr _engine = nullptr;
		
		// GPU resource binding
		RHIFrameBufferPtr      _frameBuffer    = nullptr;
		RHIRenderPassPtr       _renderPass     = nullptr;
		GPUGraphicsPipelinePtr _pipeline       = nullptr;
		RHIResourceLayoutPtr   _resourceLayout = nullptr;

		// texture rectangle mesh
		gu::DynamicArray<GPUBufferPtr> _vertexBuffers = {};
		gu::DynamicArray<GPUBufferPtr> _indexBuffers = {};

		// registered game models.
		gu::DynamicArray<GameModelPtr> _gameModels = {};

		// gaussian blur for the VSM method.
		GaussianBlurPtr _gaussianBlur = nullptr;
	};
}
#endif