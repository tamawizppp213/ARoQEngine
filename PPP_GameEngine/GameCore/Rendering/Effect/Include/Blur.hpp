//////////////////////////////////////////////////////////////////////////////////
///             @file   Blur.hpp
///             @brief  Blur effect
///             @author Toide Yutaro
///             @date   2022_05_01
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef BLUR_HPP
#define BLUR_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include "GameUtility/Math/Include/GMVector.hpp"
#include <memory>
#include <string>
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
	class GPUGraphicsPipelineState;
	class RHIFrameBuffer;
	class RHIRenderPass;
}
//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////

/****************************************************************************
*				  			GaussianBlur
*************************************************************************//**
*  @class     GaussianBlur
*  @brief     Gaussian + HalfDownSampling + Bilinear Filter
*****************************************************************************/
namespace gc
{

	class GaussianBlur : public NonCopyable
	{
		using BlurParameterPtr        = std::shared_ptr<rhi::core::GPUBuffer>;
		using TextureSizeBufferPtr    = std::shared_ptr<rhi::core::GPUBuffer>;
		using ResourceLayoutPtr       = std::shared_ptr<rhi::core::RHIResourceLayout>;
		using ResourceViewPtr         = std::shared_ptr<rhi::core::GPUResourceView>;
		using ComputePipelineStatePtr = std::shared_ptr<rhi::core::GPUComputePipelineState>;
		using GraphicsPipelineStatePtr = std::shared_ptr<rhi::core::GPUGraphicsPipelineState>;
		using FrameBufferPtr          = std::shared_ptr<rhi::core::RHIFrameBuffer>;
		using LowLevelGraphicsEnginePtr = std::shared_ptr<LowLevelGraphicsEngine>;

	protected:
		struct BlurParameter
		{
			gm::Float4 Weights[2];
		};
		struct TextureSizeParameter
		{
			std::uint32_t OriginalTexture[2];
			std::uint32_t XBlurTexture[2];
			std::uint32_t YBlurTexture[2];
		};
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void OnResize(const std::uint32_t newWidth, const std::uint32_t newHeight);
		
		void Draw(const FrameBufferPtr& frameBuffer, const std::uint32_t renderTargetIndex = 0);

		void DrawCS(const ResourceViewPtr& sourceSRV, const ResourceViewPtr& destUAV);
		
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		ResourceViewPtr GetHalfDownSampledSRV() const noexcept { return _shaderResourceViews[1]; }

		ResourceViewPtr GetHalfDownSampledUAV() const noexcept { return _unorderedResourceViews[1]; }

		void SetUpWeightTable(float sigma);
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GaussianBlur();
		
		~GaussianBlur();
		
		GaussianBlur(const LowLevelGraphicsEnginePtr& engine, const std::uint32_t width, const std::uint32_t height, const std::wstring& addName = L"");
	
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void PrepareBlurParameters(const std::wstring& name);
		
		void PrepareTextureSizeBuffer(const std::uint32_t width, const std::uint32_t height, const std::wstring& name);
		
		void PreparePipelineState(const std::wstring& name);
		
		void PrepareResourceView();
		
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		enum { ViewCount = 3 };

		/*-------------------------------------------------------------------
		-               Compute and Pixel Shader Common Variables
		---------------------------------------------------------------------*/
		// graphics engine.
		LowLevelGraphicsEnginePtr _engine = nullptr;

		// original + xBlur + yBlur texture size
		TextureSizeParameter _textureSize;
		ResourceViewPtr      _textureSizeView = nullptr;

		// gaussian blur weight parameters [in total 8 count]
		ResourceViewPtr      _blurParameterView = nullptr;

		// pipeline state resources
		ResourceLayoutPtr _resourceLayout = nullptr;

		bool _useCS = true;

		std::wstring _addName = L"";

		/*-------------------------------------------------------------------
		-               Compute Shader Variables
		---------------------------------------------------------------------*/
		ComputePipelineStatePtr _computePipeline = nullptr;

		/*-------------------------------------------------------------------
		-               Vertex and Pixel Shader Variables
		---------------------------------------------------------------------*/
		GraphicsPipelineStatePtr _xBlurPipeline = nullptr;
		GraphicsPipelineStatePtr _yBlurPipeline = nullptr;


		ResourceViewPtr _shaderResourceViews[ViewCount];
		ResourceViewPtr _unorderedResourceViews[ViewCount];


		/*-------------------------------------------------------------------
		-               Const value
		---------------------------------------------------------------------*/
		static constexpr std::uint32_t WEIGHT_TABLE_SIZE = 8;
		static constexpr float         DEFAULT_BLUR_SIGMA = 8.0f;
		static constexpr std::uint32_t THREAD = 16;

	private:
	};
}
#endif