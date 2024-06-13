//////////////////////////////////////////////////////////////////////////////////
///             @file   DepthOfField.hpp
///             @brief  DepthOfField
///             @author Toide Yutaro
///             @date   2023_02_08
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DEPTH_OF_FIELD_HPP
#define DEPTH_OF_FIELD_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class LowLevelGraphicsEngine;

namespace rhi::core
{
	class GPUBuffer;
	class GPUTexture;
	class GPUResourceView;
	class RHIResourceLayout;
	class RHIRenderPass;
	class RHIFrameBuffer;
	class GPUComputePipelineState;
}

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc
{

	/****************************************************************************
	*				  			Dof
	****************************************************************************/
	/* @class     Dof
	*  @brief     Depth Of Field (linear depth)
	*  @note      ñ‚ëËì_: zPrepassÇ™çÏÇÁÇÍÇƒÇ»Ç¢.
	*****************************************************************************/
	class Dof : public gu::NonCopyable
	{
	protected:
		struct BlurParameter
		{
			float TextureSize[2];
			float Radius;
			float Padding = 0.0f;
		};

		struct ClipSize
		{
			float Near;
			float Far;
			float Padding[2] = {0.0f, 0.0f};
		};

		using GPUBufferPtr              = gu::SharedPointer<rhi::core::GPUBuffer>;
		using GPUTexturePtr             = gu::SharedPointer<rhi::core::GPUTexture>;
		using ResourceViewPtr           = gu::SharedPointer<rhi::core::GPUResourceView>;
		using FrameBufferPtr            = gu::SharedPointer<rhi::core::RHIFrameBuffer>;
		using LowLevelGraphicsEnginePtr = gu::SharedPointer<LowLevelGraphicsEngine>;
		using PipelineStatePtr          = gu::SharedPointer<rhi::core::GPUComputePipelineState>;
		using RenderPassPtr             = gu::SharedPointer<rhi::core::RHIRenderPass>;
		using ResourceLayoutPtr         = gu::SharedPointer<rhi::core::RHIResourceLayout>;

	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void OnResize(const float newWidth, const float newHeight);

		void Draw(const ResourceViewPtr& zPrepass);

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		void SetUpBlurParameter(float width, float height, float radius = 20.0f);

		void SetUpClipSize(float nearClip = 0.2f, float farClip = 0.8f);

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Dof(const LowLevelGraphicsEnginePtr& engine, 
			const float width, const float height, const float radius = 10.0f, 
			const float nearClip = 0.2f, const float farClip = 0.8f, const gu::tstring& addName = SP(""));

		virtual ~Dof();
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void PrepareRenderBuffer(const gu::uint32 width, const gu::uint32 height);

		void PrepareBlurParameterBuffer(const float width, const float height, const float radius, const gu::tstring& name);

		void PrepareClipSizeBuffer(const float nearClip, const float farClip, const gu::tstring& name);

		void PreparePipelineState(const gu::tstring& name);

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		enum {ViewCount = 6};
		
		// @brief Blur setting
		ResourceViewPtr _blurParameterView = nullptr;
		BlurParameter   _blurParameter = {};

		/* @brief : Clip size setting*/
		ResourceViewPtr _clipSizeView = nullptr;
		ClipSize        _clipSize = {};
		
		/*-------------------------------------------------------------------
		-             Render resource
		---------------------------------------------------------------------*/
		PipelineStatePtr _verticalPipeline    = nullptr;
		PipelineStatePtr _rhomboidPipeline    = nullptr;
		PipelineStatePtr _finalRenderPipeline = nullptr;

		ResourceLayoutPtr _resourceLayout = nullptr;


		/* @brief : Texture rendering setting*/
		gu::DynamicArray<ResourceViewPtr> _shaderResourceViews = {};
		gu::DynamicArray<ResourceViewPtr> _unorderedAccessViews = {};

		LowLevelGraphicsEnginePtr _engine = nullptr;

		static constexpr std::uint32_t THREAD = 16;
	};
}
#endif