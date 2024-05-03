//////////////////////////////////////////////////////////////////////////////////
///             @file   Bloom.hpp
///             @brief  Bloom effect 
///             @author Toide Yutaro
///             @date   2022_11_20
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef BLOOM_HPP
#define BLOOM_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "Blur.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
class LowLevelGraphicsEngine;
namespace rhi::core
{
	class GPUComputePipelineState;
	class RHIResourceLayout;
	class GPUResourceView;
}
namespace gc
{
	struct BloomDesc
	{

	};
	/****************************************************************************
	*				  			    Class
	*************************************************************************//**
	*  @class     Class
	*  @brief     temp
	*****************************************************************************/
	class Bloom : public gu::NonCopyable
	{
		using ResourceLayoutPtr = gu::SharedPointer<rhi::core::RHIResourceLayout>;
		using PipelineStatePtr = gu::SharedPointer<rhi::core::GPUComputePipelineState>;
		using GaussianBlurPtr = gu::SharedPointer<GaussianBlur>;
		using ResourceViewPtr = gu::SharedPointer<rhi::core::GPUResourceView>;
		using LowLevelGraphicsEnginePtr = gu::SharedPointer<LowLevelGraphicsEngine>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void OnResize(const std::uint32_t newWidth, const std::uint32_t newHeight);

		void Draw();

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		void UpdateBloomPower(const float power);

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Bloom() = default;

		Bloom(const LowLevelGraphicsEnginePtr& engine, const std::uint32_t width, const std::uint32_t height, 
			const float power = 10.0f, 
			const ResourceViewPtr& customLuminanceSRV = nullptr,
			const ResourceViewPtr& customLuminanceUAV = nullptr,
			const gu::tstring& addName = SP(""));

		~Bloom();

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void PrepareGaussianBlurs(const std::uint32_t width, const std::uint32_t height, const gu::tstring& name);

		void PreparePipelineState(const gu::tstring& name);

		void PrepareResourceView(const gu::tstring& name);

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		enum { ViewCount = 3 };

		PipelineStatePtr  _luminancePipeline = nullptr;

		PipelineStatePtr  _finalBloomPipeline = nullptr;

		GaussianBlurPtr   _gaussianBlur[4] = {};

		ResourceLayoutPtr _resourceLayout = nullptr;

		// 輝度テクスチャ
		ResourceViewPtr   _luminanceSRV = nullptr;
		ResourceViewPtr   _luminanceUAV = nullptr;

 
		// 縮小バッファ用
		ResourceViewPtr   _shaderResourceViews   [ViewCount] = {};	
		ResourceViewPtr   _unorderedResourceViews[ViewCount] = {};
		
		LowLevelGraphicsEnginePtr _engine = nullptr;

		float _explosion = 10.0f;
		static constexpr std::uint32_t THREAD = 16;
	};
}
#endif