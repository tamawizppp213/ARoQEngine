//////////////////////////////////////////////////////////////////////////////////
///             @file   UIRenderer.hpp
///             @brief  UIRenderer
///             @author Toide Yutaro
///             @date   2022_06_01
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef UI_RENDERER_HPP
#define UI_RENDERER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "Image.hpp"
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include <vector>
#include <string>
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class LowLevelGraphicsEngine;
namespace rhi::core
{
	class RHIDevice;
	class RHICommandList;
	class GPUBuffer;
	class GPUGraphicsPipelineState;
	class RHIResourceLayout;
	class GPUResourceCache;
	class GPUResourceView;
}
struct Texture;

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace ui
{
	/****************************************************************************
	*				  			Sprite
	*************************************************************************//**
	*  @class     Sprite
	*  @brief     2D Sprite
	*****************************************************************************/
	class UIRenderer : public NonCopyable
	{
		using LowLevelGraphicsEnginePtr = std::shared_ptr<LowLevelGraphicsEngine>;
		using VertexBufferPtr   = std::shared_ptr<rhi::core::GPUBuffer>;
		using IndexBufferPtr    = std::shared_ptr<rhi::core::GPUBuffer>;
		using PipelineStatePtr  = std::shared_ptr<rhi::core::GPUGraphicsPipelineState>;
		using ResourceLayoutPtr = std::shared_ptr<rhi::core::RHIResourceLayout>;
		using ResourceViewPtr   = std::shared_ptr<rhi::core::GPUResourceView>;
		using ImagePtr = std::shared_ptr<ui::Image>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void AddFrameObjects(const std::vector<ImagePtr>& images, const ResourceViewPtr& view);
		void Draw();
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		virtual ~UIRenderer();
		
		explicit UIRenderer(const LowLevelGraphicsEnginePtr& engine,
			const std::wstring& addName = L"",
			const std::uint32_t maxUICount = 1024);
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void PrepareMaxImageBuffer(const std::wstring& name);
		void PreparePipelineState(const std::wstring& name);
		void ClearVertexBuffer(const std::uint32_t frameIndex, const size_t vertexCount);
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		LowLevelGraphicsEnginePtr _engine = nullptr;
		// @brief : total rect vertex buffers (default : 1024 UI)
		std::vector<VertexBufferPtr> _vertexBuffers = {};
		// @brief : total rect index buffers (default : 1024 UI)
		std::vector<IndexBufferPtr>  _indexBuffers  = {};
		// @brief : Pipeline state
		PipelineStatePtr _pipeline = nullptr;
		// @brief bind resource layout and view
		ResourceLayoutPtr _resourceLayout = nullptr;
		std::vector<ResourceViewPtr> _resourceViews = {};
		/* @brief regist total image count per frame  */
		std::uint32_t  _totalImageCount        = 0;
		// @brief : call drawIndex command count per frame
		std::uint32_t  _needCallDrawIndexCount = 0;
		std::vector<std::uint32_t> _imageCountList;

	private:
		std::uint32_t _maxWritableUICount = 1024;
		static constexpr int MAX_WRITABLE_UI_COUNT = 1024;
	};
}
#endif