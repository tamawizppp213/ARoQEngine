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
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include <string>
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class LowLevelGraphicsEngine;
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
	*				  			   ShadowMap
	*************************************************************************//**
	*  @class     ShadowMap
	*  @brief     Rendering the shadow map
	*****************************************************************************/
	class ShadowMap : public NonCopyable
	{
	protected:
		using LowLevelGraphicsEnginePtr = std::shared_ptr<LowLevelGraphicsEngine>;
		using GameModelPtr              = std::shared_ptr<gc::core::GameModel>;
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

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		ShadowMap() = default;

		ShadowMap(const LowLevelGraphicsEnginePtr& engine, 
			const std::uint32_t width, 
			const std::uint32_t height,
			const std::wstring& addName = L"");
		
		~ShadowMap();

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void PrepareVertexAndIndexBuffer(const std::wstring& name);
		void PreparePipelineState(const std::wstring& name);

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
		std::vector<BufferPtr> _vertexBuffers = {};
		std::vector<BufferPtr> _indexBuffers = {};

		// registered game models.
		std::vector<GameModelPtr> _gameModels = {};
	};
}
#endif