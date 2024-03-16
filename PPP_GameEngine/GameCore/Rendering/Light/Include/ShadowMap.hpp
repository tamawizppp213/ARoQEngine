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
namespace gc
{
	class GaussianBlur;

	namespace core
	{
		class GameModel;
	}
}
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc::rendering
{
	/****************************************************************************
	*				  			   ShadowMap
	*************************************************************************//**
	*  @class     ShadowMap
	*  @brief     Rendering the shadow map
	*****************************************************************************/
	class ShadowMap : public gu::NonCopyable
	{
	protected:
		using LowLevelGraphicsEnginePtr = gu::SharedPointer<LowLevelGraphicsEngine>;
		using GameModelPtr              = gu::SharedPointer<gc::core::GameModel>;
		using GaussianBlurPtr           = gu::SharedPointer<gc::GaussianBlur>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Draw(const ResourceViewPtr& scene);

		void Add(const GameModelPtr& gameModel);

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		FrameBufferPtr GetFrameBuffer() const noexcept { return _frameBuffer; }

		ResourceViewPtr GetHalfDownSampledSRV() const noexcept;

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
		**                Protected Member Variables
		*****************************************************************************/
		LowLevelGraphicsEnginePtr _engine = nullptr;
		
		// GPU resource binding
		FrameBufferPtr      _frameBuffer    = nullptr;
		RenderPassPtr       _renderPass     = nullptr;
		GraphicsPipelinePtr _pipeline       = nullptr;
		ResourceLayoutPtr   _resourceLayout = nullptr;

		// texture rectangle mesh
		gu::DynamicArray<BufferPtr> _vertexBuffers = {};
		gu::DynamicArray<BufferPtr> _indexBuffers = {};

		// registered game models.
		gu::DynamicArray<GameModelPtr> _gameModels = {};

		// gaussian blur for the VSM method.
		GaussianBlurPtr _gaussianBlur = nullptr;
	};
}
#endif