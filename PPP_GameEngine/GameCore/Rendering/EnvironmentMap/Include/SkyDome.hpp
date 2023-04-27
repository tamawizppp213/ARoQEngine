//////////////////////////////////////////////////////////////////////////////////
///             @file   TemplateText.hpp
///             @brief  TemplateText
///             @author Toide Yutaro
///             @date   2022_03_11
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SKYBOX_HPP
#define SKYBOX_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <string>
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class LowLevelGraphicsEngine;
namespace rhi::core
{
	class RHIResourceLayout;
	class GPUGraphicsPipelineState;
	class GPUBuffer;
	class GPUTexture;
	class GPUResourceView;
}

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc
{

	/****************************************************************************
	*				  			SkyDome
	*************************************************************************//**
	*  @class     SkyDome
	*  @brief     SkyDome
	*****************************************************************************/
	class SkyDome
	{
		using GPUResourceViewPtr  = std::shared_ptr<rhi::core::GPUResourceView>;
		using VertexBufferPtr     = std::shared_ptr<rhi::core::GPUBuffer>;
		using IndexBufferPtr      = std::shared_ptr<rhi::core::GPUBuffer>;
		using ObjectConstantPtr   = std::shared_ptr<rhi::core::GPUBuffer>;
		using GraphicsPipelinePtr = std::shared_ptr<rhi::core::GPUGraphicsPipelineState>;
		using ResourceLayoutPtr   = std::shared_ptr<rhi::core::RHIResourceLayout>;
		using LowLevelGraphicsEnginePtr = std::shared_ptr<LowLevelGraphicsEngine>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Draw(const GPUResourceViewPtr& cameraResourceView);
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		SkyDome();

		~SkyDome();

		SkyDome(const LowLevelGraphicsEnginePtr& engine, const std::wstring& cubeMapPath, const std::wstring& addName = L"");
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void PrepareResourceView(const std::shared_ptr<rhi::core::GPUTexture>& texture);
		
		void PreparePipelineState(const std::wstring& addName);
		
		void PrepareVertexAndIndexBuffer(const std::wstring& addName);
		
		void PrepareSkyObject(const std::wstring& addName);
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		ObjectConstantPtr            _skyObject = nullptr;
		std::vector<VertexBufferPtr> _vertexBuffers = {};
		std::vector<IndexBufferPtr>  _indexBuffers = {};
		std::vector<std::pair<std::uint32_t, GPUResourceViewPtr>> _resourceViews = {};
		ResourceLayoutPtr   _resourceLayout = nullptr;
		GraphicsPipelinePtr _pipeline = nullptr;
		LowLevelGraphicsEnginePtr _engine = nullptr;

		static constexpr float SKY_SCALE = 5000.0f;
	};
}
#endif