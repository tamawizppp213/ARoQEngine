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
#include "UIImage.hpp"
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include <vector>
#include <string>
#include "GameUtility/Base/Include/GUMemory.hpp"
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
namespace gc::ui
{
	/****************************************************************************
	*				  			UIRenderer
	*************************************************************************//**
	*  @class     UIRenderer
	*  @brief     2D Sprite
	*****************************************************************************/
	class UIRenderer : public NonCopyable
	{
	protected:
		using LowLevelGraphicsEnginePtr = gu::SharedPointer<LowLevelGraphicsEngine>;
		using VertexBufferPtr   = gu::SharedPointer<rhi::core::GPUBuffer>;
		using IndexBufferPtr    = gu::SharedPointer<rhi::core::GPUBuffer>;
		using PipelineStatePtr  = gu::SharedPointer<rhi::core::GPUGraphicsPipelineState>;
		using ResourceLayoutPtr = gu::SharedPointer<rhi::core::RHIResourceLayout>;
		using ResourceViewPtr   = gu::SharedPointer<rhi::core::GPUResourceView>;
		using ImagePtr = gu::SharedPointer<ui::Image>;

	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Clear();

		/* @brief : Add frame ui objects (image, text etc...)*/
		//void AddFrameObjects(const std::vector<ImagePtr>& images, const ResourceViewPtr& view);
		void AddFrameObjects(const std::vector<ui::Image>& images, const ResourceViewPtr& view);

		/* @brief : Render all registered frame ui objects*/
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
		/* @brief: Prepare max writable ui count size buffer*/
		void PrepareMaxImageBuffer(const std::wstring& name);

		/* @brief: Prepare graphics pipeline state objects. */
		void PreparePipelineState(const std::wstring& name);

		/* @brief: Clear gpu vertex buffer */
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
		std::uint32_t  _totalImageCount = 0;
		std::uint32_t  _prevTotalImageCount = 0;

		// @brief : call drawIndex command count per frame
		std::uint32_t  _needCallDrawIndexCount = 0;
		std::vector<std::uint32_t> _imageCountList = {};

	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		/* @brief : AddFrameObject function is used. count up draw image and push back resource view.s*/
		void CountUpDrawImageAndView(const std::uint64_t arrayLength, const ResourceViewPtr& view);
		
		/****************************************************************************
		**             Private Member Variables
		*****************************************************************************/
		std::uint32_t _maxWritableUICount = 1024;
	};
}
#endif