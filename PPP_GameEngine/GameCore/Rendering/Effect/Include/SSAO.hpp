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
#include <memory>
#include <string>
#include <vector>
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
			hlsl::float2 TextureSize    = gm::Float2(Screen::GetScreenWidth(), Screen::GetScreenHeight());
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

		using LowLevelGraphicsEnginePtr = std::shared_ptr<LowLevelGraphicsEngine>;
		using ResourceViewPtr           = std::shared_ptr<rhi::core::GPUResourceView>;
		using PipelineStatePtr          = std::shared_ptr<rhi::core::GPUGraphicsPipelineState>;
		using ResourceLayoutPtr         = std::shared_ptr<rhi::core::RHIResourceLayout>;
		using VertexBufferPtr           = std::shared_ptr<rhi::core::GPUBuffer>;
		using IndexBufferPtr            = std::shared_ptr<rhi::core::GPUBuffer>;
		using GaussianBlurPtr           = std::shared_ptr<gc::GaussianBlur>;
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
		SSAO(const LowLevelGraphicsEnginePtr& engine, const ResourceViewPtr& normalMap, const ResourceViewPtr& depthMap, const std::wstring& addName = L"");

		~SSAO();
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void PrepareSSAOSettings(const std::wstring& name);

		void PrepareBlurMode(const std::wstring& name);

		void PrepareRandomTexture(const std::wstring& name);

		void PreparePipelineState(const std::wstring& name);

		void PrepareVertexAndIndexBuffer(const std::wstring& name);
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

		std::vector<VertexBufferPtr> _vertexBuffers = {};

		std::vector<IndexBufferPtr>  _indexBuffers = {};

		LowLevelGraphicsEnginePtr _engine = nullptr;
	};
}
#endif