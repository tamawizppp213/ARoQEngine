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
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Base/Include/GUString.hpp"
#include "GameUtility/Container/Include/GUPair.hpp"
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
		using GPUResourceViewPtr  = gu::SharedPointer<rhi::core::GPUResourceView>;
		using VertexBufferPtr     = gu::SharedPointer<rhi::core::GPUBuffer>;
		using IndexBufferPtr      = gu::SharedPointer<rhi::core::GPUBuffer>;
		using ObjectConstantPtr   = gu::SharedPointer<rhi::core::GPUBuffer>;
		using GraphicsPipelinePtr = gu::SharedPointer<rhi::core::GPUGraphicsPipelineState>;
		using ResourceLayoutPtr   = gu::SharedPointer<rhi::core::RHIResourceLayout>;
		using LowLevelGraphicsEnginePtr = gu::SharedPointer<LowLevelGraphicsEngine>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Draw(const GPUResourceViewPtr& cameraResourceView);
		/****************************************************************************
		**                Public Property
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		SkyDome();

		~SkyDome();

		SkyDome(const LowLevelGraphicsEnginePtr& engine, const gu::tstring& cubeMapPath, const gu::tstring& addName = SP(""));
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void PrepareResourceView(const gu::SharedPointer<rhi::core::GPUTexture>& texture);
		
		void PreparePipelineState(const gu::tstring& addName);
		
		void PrepareVertexAndIndexBuffer(const gu::tstring& addName);
		
		void PrepareSkyObject(const gu::tstring& addName);
		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		ObjectConstantPtr            _skyObject = nullptr;
		gu::DynamicArray<VertexBufferPtr> _vertexBuffers = {};
		gu::DynamicArray<IndexBufferPtr>  _indexBuffers = {};
		gu::DynamicArray<gu::Pair<gu::uint32, GPUResourceViewPtr>> _resourceViews = {};
		ResourceLayoutPtr   _resourceLayout = nullptr;
		GraphicsPipelinePtr _pipeline = nullptr;
		LowLevelGraphicsEnginePtr _engine = nullptr;

		static constexpr float SKY_SCALE = 5000.0f;
	};
}
#endif