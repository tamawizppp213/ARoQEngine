//////////////////////////////////////////////////////////////////////////////////
///             @file   SSAO.hpp
///             @brief  Screen space ambient occlusion
///             @author Toide Yutaro
///             @date   2023_04_02
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SSAO_HPP
#define SSAO_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include "GameUtility/Base/Include/HLSLUtility.hpp"
#include "GameUtility/Base/Include/Screen.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Base/Include/GUString.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class LowLevelGraphicsEngine;
namespace gc
{
	class GaussianBlur;
};
namespace rhi::core
{
	class GPUBuffer;
	class GPUResourceView;
	class RHIResourceLayout;
	class GPUGraphicsPipelineState;

}
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gc
{
	/****************************************************************************
	*				  			    Class
	*************************************************************************//**
	*  @class     Class
	*  @brief     temp
	*****************************************************************************/
	class SSAO : public NonCopyable
	{
	protected:
		/* @brief : SSAO setting : Define radius, attenuated shapness, fade view space value from start to end.*/
		struct SSAOSetting
		{
			hlsl::float2 TextureSize    = gm::Float2((float)Screen::GetScreenWidth(), (float)Screen::GetScreenHeight());
			hlsl::float1 Radius         = 1.5f;
			hlsl::float1 Sharpness      = 4.0f;
			hlsl::float1 FadeStart      = 0.2f;  // view space
			hlsl::float1 FadeEnd        = 500.0f; // view space
			hlsl::float1 SurfaceEpsilon = 0.05f;
			int          Padding     = 0;
			hlsl::float4 Offsets[14] = {}; // SSAO初期化時に生成するため, 設定不要です. 8 cube corners + 6 centers of cube faces
		};

		/* @brief : Blur mode: This struct is used for the denoize */
		struct BlurMode
		{
			hlsl::float4  Weights[2]   = {};    // SSAO初期化時に生成するため, 設定不要です. 
			hlsl::float1  BlurRadius   = 5.0f;
			bool          IsHorizontal = false;
		};

		using LowLevelGraphicsEnginePtr = gu::SharedPointer<LowLevelGraphicsEngine>;
		using ResourceViewPtr           = gu::SharedPointer<rhi::core::GPUResourceView>;
		using PipelineStatePtr          = gu::SharedPointer<rhi::core::GPUGraphicsPipelineState>;
		using ResourceLayoutPtr         = gu::SharedPointer<rhi::core::RHIResourceLayout>;
		using VertexBufferPtr           = gu::SharedPointer<rhi::core::GPUBuffer>;
		using IndexBufferPtr            = gu::SharedPointer<rhi::core::GPUBuffer>;
		using GaussianBlurPtr           = gu::SharedPointer<gc::GaussianBlur>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void OnResize(const std::uint32_t newWidth, const std::uint32_t newHeight);

		void Draw(const ResourceViewPtr& scene);

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/* @brief : Return rendered texture view pointer*/
		ResourceViewPtr GetRenderedTextureView() const noexcept { return _ambientMap; }

		void SetOcclusionRadius(const float radius);

		void SetSharpness(const float sharpness);

		void SetFadeStart(const float startDepthInViewSpace);

		void SetFadeEnd(const float endDepthnViewSpace);

		void SetSurfaceEpsilon(const float epsilonDepth);

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		SSAO(const LowLevelGraphicsEnginePtr& engine, const ResourceViewPtr& normalMap, const ResourceViewPtr& depthMap, const gu::tstring& addName = SP(""));

		~SSAO();
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void PrepareSSAOSettings(const gu::tstring& name);

		void PrepareBlurMode(const gu::tstring& name);

		void PrepareRandomTexture(const gu::tstring& name);

		void PreparePipelineState(const gu::tstring& name);

		void PrepareVertexAndIndexBuffer(const gu::tstring& name);
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		/* ssao setting : resource view and struct.*/
		SSAOSetting     _setting = {};
		ResourceViewPtr _settingView = nullptr;

		ResourceLayoutPtr _ssaoResourceLayout = nullptr; // commonly used between ssao and blur. 
		PipelineStatePtr  _ssaoPipeline       = nullptr;

		/* blur mode setting : resource view*/
		ResourceViewPtr  _blurHorizontalModeView = nullptr;
		ResourceViewPtr  _blurVerticalModeView   = nullptr;
		PipelineStatePtr _blurPipeline = nullptr;
		GaussianBlurPtr _gaussianBlur = nullptr;

		/* @brief : This value is used when the ssao settings set function is called.*/
		bool _isUpdated = false;

		/* @brief : Normal texture*/
		ResourceViewPtr _normalMap = nullptr;
		
		/* @brief : Depth texture. */
		ResourceViewPtr _depthMap  = nullptr;

		/* @brief : Final rendered texture. This class is drawed ambient occlusion map*/
		ResourceViewPtr _ambientMap = nullptr;

		/* @brief : CPU calculate random value map. */
		ResourceViewPtr _randomMap = nullptr;

		gu::DynamicArray<VertexBufferPtr> _vertexBuffers = {};

		gu::DynamicArray<IndexBufferPtr>  _indexBuffers = {};

		LowLevelGraphicsEnginePtr _engine = nullptr;
	};
}
#endif