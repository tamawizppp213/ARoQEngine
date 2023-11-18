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
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHITypeCore.hpp"
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include "GameUtility/Math/Include/GMVector.hpp"
#include <string>
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class LowLevelGraphicsEngine;

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
		using BlurParameterPtr        = gu::SharedPointer<rhi::core::GPUBuffer>;
		using TextureSizeBufferPtr    = gu::SharedPointer<rhi::core::GPUBuffer>;
		using LowLevelGraphicsEnginePtr = gu::SharedPointer<LowLevelGraphicsEngine>;

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

		struct PSResource
		{
			GraphicsPipelinePtr Pipeline    = nullptr;
			RenderPassPtr       RenderPass  = nullptr;
			FrameBufferPtr      FrameBuffer = nullptr;
			std::vector<BufferPtr> VB = {};
			std::vector<BufferPtr> IB = {};
			~PSResource() { VB.clear(); IB.clear(); VB.shrink_to_fit(); IB.shrink_to_fit(); }
		};

	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void OnResize(const std::uint32_t newWidth, const std::uint32_t newHeight);
		
		void Draw(const FrameBufferPtr& frameBuffer, const std::uint32_t renderTargetIndex = 0);

		void DrawCS(const ResourceViewPtr& sourceSRV, const ResourceViewPtr& destUAV);
		
		void DrawPS(const FrameBufferPtr& frameBuffer, const std::uint32_t renderTargetIndex = 0);
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
		
		GaussianBlur(const LowLevelGraphicsEnginePtr& engine, const std::uint32_t width, const std::uint32_t height, const bool useCS = true, const std::wstring& addName = L"");
	
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void PrepareBlurParameters(const std::wstring& name);
		
		void PrepareTextureSizeBuffer(const std::uint32_t width, const std::uint32_t height, const std::wstring& name);
		
		void PreparePipelineState(const std::wstring& name);
		
		void PrepareResourceView();

		void PrepareVertexAndIndexBuffer(const std::wstring& addName);
		
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
		ComputePipelinePtr _computePipeline = nullptr;

		/*-------------------------------------------------------------------
		-               Vertex and Pixel Shader Variables
		---------------------------------------------------------------------*/
		PSResource _xBlur;
		PSResource _yBlur;
		GraphicsPipelinePtr _graphicsPipeline = nullptr;

		std::vector<BufferPtr> _vertexBuffers = {};
		std::vector<BufferPtr> _indexBuffers  = {};

		ResourceViewPtr _shaderResourceViews[ViewCount];
		ResourceViewPtr _unorderedResourceViews[ViewCount];
		ResourceViewPtr _renderTargetResourceViews[ViewCount];

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