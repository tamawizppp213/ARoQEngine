//////////////////////////////////////////////////////////////////////////////////
///             @file   ColorChange.hpp
///             @brief  Color change post effect
///             @author Toide Yutaro
///             @date   2022_03_11
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef COLOR_CHANGE_HPP
#define COLOR_CHANGE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include <vector>
#include <string>
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class PipelineState;
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

	enum ColorChangeType
	{
		None,
		Monochrome,
		Sepia,
		GrayScale,
		Binary,
		Invert,
		CountOfType,
	};
	/****************************************************************************
	*				  			ColorChange
	*************************************************************************//**
	*  @class     Color Change
	*  @brief     color change post effect
	*****************************************************************************/
	class ColorChange : public NonCopyable
	{
		using VertexBufferPtr = std::shared_ptr<rhi::core::GPUBuffer>;
		using IndexBufferPtr  = std::shared_ptr<rhi::core::GPUBuffer>;
		using ResourceLayoutPtr = std::shared_ptr<rhi::core::RHIResourceLayout>;
		using ResourceViewPtr   = std::shared_ptr<rhi::core::GPUResourceView>;
		using PipelineStatePtr  = std::shared_ptr<rhi::core::GPUGraphicsPipelineState>;
		using LowLevelGraphicsEnginePtr = std::shared_ptr<LowLevelGraphicsEngine>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Resize frame buffer (Not implement)*/
		void OnResize(int newWidth, int newHeight);
		/*@brief : Render to back buffer*/
		void Draw();
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		ColorChange();
		~ColorChange();
		ColorChange(const ColorChangeType type, const LowLevelGraphicsEnginePtr& engine, const std::wstring& addName = L"");

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void PrepareVertexAndIndexBuffer(const std::wstring& addName);
		void PreparePipelineState(ColorChangeType type, const std::wstring& addName);
		void PrepareResourceView();
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		/* @brief : frame resources*/
		std::vector<VertexBufferPtr> _vertexBuffers = {};
		std::vector<IndexBufferPtr>  _indexBuffers = {};
		PipelineStatePtr  _pipeline       = nullptr;
		ResourceLayoutPtr _resourceLayout = nullptr;
		std::vector<ResourceViewPtr> _resourceViews = {};
		/* @brief : device and command list*/
		LowLevelGraphicsEnginePtr _engine = nullptr;
	};
}
#endif