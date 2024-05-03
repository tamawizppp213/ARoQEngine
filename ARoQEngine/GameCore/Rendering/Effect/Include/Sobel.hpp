//////////////////////////////////////////////////////////////////////////////////
///             @file   Sobel.hpp
///             @brief  Detect outline 
///             @author Toide Yutaro
///             @date   2023_03_17
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SOBEL_HPP
#define SOBEL_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Math/Include/GMVector.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Base/Include/GUString.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class LowLevelGraphicsEngine;
namespace rhi::core
{
	class GPUBuffer;
	class GPUResourceView;
	class RHIResourceLayout;
	class GPUComputePipelineState;
}

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc
{
	/****************************************************************************
	*				  			    Sobel
	*************************************************************************//**
	*  @class     Sobel
	*  @brief     Sobel filter. Use this class to detect the outlines. 
	*****************************************************************************/
	class Sobel
	{
		using LowLevelGraphicsEnginePtr = gu::SharedPointer<LowLevelGraphicsEngine>;
		using ResourceLayoutPtr    = gu::SharedPointer<rhi::core::RHIResourceLayout>;
		using ResourceViewPtr      = gu::SharedPointer<rhi::core::GPUResourceView>;
		using BufferPtr            = gu::SharedPointer<rhi::core::GPUBuffer>;
		using PipelineStatePtr     = gu::SharedPointer<rhi::core::GPUComputePipelineState>;
	protected:
		struct OutlineInfo
		{
			gm::Float4 Color = gm::Float4(1, 1, 1, 1);
		};
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void OnResize(const std::uint32_t newWidth, const std::uint32_t newHeight);

		/* @brief : draw outline.*/
		void Draw(const ResourceViewPtr& sourceSRV); 

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		ResourceViewPtr GetRenderedSRV() const noexcept { return _srv; }

		ResourceViewPtr GetRenderedUAV() const noexcept { return _uav; }
		
		void SetColor(const gm::Float4& color);
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Sobel(const LowLevelGraphicsEnginePtr& engine, const std::uint32_t width, const std::uint32_t height, const gm::Float4& color = gm::Float4(1,1,1,1), const gu::tstring& addName = SP(""));

		~Sobel();

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void PrepareOutlineInfo(const gu::tstring& name);

		void PrepareResourceView();

		void PreparePipelineState(const gu::tstring& name);

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		OutlineInfo _outlineInfo = {};

		// screen size
		std::uint32_t _width  = 0;
		std::uint32_t _height = 0;
		
		// Compute pipeline resource
		ResourceLayoutPtr _resourceLayout = nullptr;
		PipelineStatePtr  _pipeline       = nullptr;

		// ResourceView
		ResourceViewPtr _outlineCBV = nullptr;
		ResourceViewPtr _srv        = nullptr;
		ResourceViewPtr _uav        = nullptr;

		LowLevelGraphicsEnginePtr _engine = nullptr;

		static constexpr std::uint32_t THREAD = 16;
	};
}

#endif