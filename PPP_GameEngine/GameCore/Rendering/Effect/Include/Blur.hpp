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
		using BlurParameterPtr     = std::shared_ptr<rhi::core::GPUBuffer>;
		using TextureSizeBufferPtr = std::shared_ptr<rhi::core::GPUBuffer>;
		using ResourceLayoutPtr    = std::shared_ptr<rhi::core::RHIResourceLayout>;
		using ResourceViewPtr      = std::shared_ptr<rhi::core::GPUResourceView>;
		using PipelineStatePtr     = std::shared_ptr<rhi::core::GPUComputePipelineState>;
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
		void Draw();
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		void SetUpWeightTable(float sigma);
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GaussianBlur();
		~GaussianBlur();
		GaussianBlur(const LowLevelGraphicsEnginePtr& engine, const std::uint32_t width, const std::uint32_t height, const std::wstring& addName = L"");
		GaussianBlur(const GaussianBlur&) = delete;
		GaussianBlur& operator=(const GaussianBlur&) = delete;
		GaussianBlur(GaussianBlur&&) = default;
		GaussianBlur& operator=(GaussianBlur&&) = default;
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

		TextureSizeParameter _textureSize;
		ResourceLayoutPtr    _resourceLayout    = nullptr;
		PipelineStatePtr     _xBlurPipeline     = nullptr;
		PipelineStatePtr     _yBlurPipeline     = nullptr;
		PipelineStatePtr     _finalBlurPipeline = nullptr;
		ResourceViewPtr      _shaderResourceViews[ViewCount];
		ResourceViewPtr      _unorderedResourceViews[ViewCount];
		ResourceViewPtr      _blurParameterView = nullptr;
		ResourceViewPtr      _textureSizeView = nullptr;
		std::wstring _addName = L"";
		LowLevelGraphicsEnginePtr _engine = nullptr;

		static constexpr std::uint32_t WEIGHT_TABLE_SIZE = 8;
		static constexpr float        DEFAULT_BLUR_SIGMA = 8.0f;
		static constexpr std::uint32_t THREAD = 16;

	};
}
#endif