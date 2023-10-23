//////////////////////////////////////////////////////////////////////////////////
///             @file   ScreenSpaceReflection.hpp
///             @brief  ポストエフェクト的に反射を表現する
///             @author Toide Yutaro
///             @date   2023_10_17
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SCREEN_SPACE_REFLECTION_HPP
#define SCREEN_SPACE_REFLECTION_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "FullScreenEffector.hpp"
#include "GameUtility/Math/Include/GMVector.hpp"
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc
{
	struct SSRSettings
	{
		int   MaxRayMarchIteraction    = 16; // レイマーチングを行う最大の試行回数
		int   MaxBinarySearchIteration = 16; // 二分探索を行う試行回数
		float MaxRayDistance = 100;           // レイの最大距離
		float RayThicknessInViewSpace = 10;  // レイの厚み
		float MaxFadeDistance         = 50;  // 最大の距離フェードを行う長さ
	};

	/****************************************************************************
	*				  			ScreenSpaceReflection
	*************************************************************************//**
	*  @class     ScreenSpaceReflection
	*  @brief     ポストエフェクト的に反射を表現する
	*****************************************************************************/
	class ScreenSpaceReflection : public NonCopyable
	{
	protected:
		using LowLevelGraphicsEnginePtr = std::shared_ptr<LowLevelGraphicsEngine>;
		using ResourceViewPtr           = std::shared_ptr<rhi::core::GPUResourceView>;
		using PipelineStatePtr          = std::shared_ptr<rhi::core::GPUGraphicsPipelineState>;
		using VertexBufferPtr           = std::shared_ptr<rhi::core::GPUBuffer>;
		using IndexBufferPtr            = std::shared_ptr<rhi::core::GPUBuffer>;
		using ResourceLayoutPtr         = std::shared_ptr<rhi::core::RHIResourceLayout>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Resize frame buffer (Not implement)*/
		void OnResize(int newWidth, int newHeight);

		/*@brief : Render to back buffer*/
		void Draw(const ResourceViewPtr& scene);

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		int GetMaxRayMarchIteration() const { return _settings.MaxRayMarchIteraction; }
		
		int GetMaxBinarySearchIteration() const { return _settings.MaxBinarySearchIteration; }
		
		float GetMaxRayDistance() const { return _settings.MaxRayDistance; }
		
		float GetMaxFadeDistance() const { return _settings.MaxFadeDistance; }
		
		float GetRayThicknessInViewSpace() const { return _settings.RayThicknessInViewSpace; }

		void SetMaxRayMarchIteration(const int iteration) { _isSettingChanged = true; _settings.MaxRayMarchIteraction; }

		void SetMaxBinarySearchIteration(const int iteration) { _isSettingChanged = true; _settings.MaxBinarySearchIteration; }

		void SetMaxRayDistance(const float distance) { _isSettingChanged = true; _settings.MaxRayDistance; }

		void SetMaxFadeDistance(const float distance) { _isSettingChanged = true; _settings.MaxFadeDistance; }

		void SetRayThicknessInViewSpace(const float thickness) { _isSettingChanged = true; _settings.RayThicknessInViewSpace; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		ScreenSpaceReflection();

		~ScreenSpaceReflection();

		ScreenSpaceReflection(const LowLevelGraphicsEnginePtr& engine, const ResourceViewPtr& normalMap, const ResourceViewPtr& depthMap, const SSRSettings& settings, const std::wstring& addName = L"");

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void PrepareVertexAndIndexBuffer(const std::wstring& name);
		
		void PrepareBuffer(const SSRSettings& setting, const std::wstring& addName);

		void PreparePipelineState(const std::wstring& addName);

		void PrepareResourceView();

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		SSRSettings _settings = {};

		ResourceViewPtr _settingsView = nullptr;

		bool _isSettingChanged = false;

		/* @brief : Normal texture*/
		ResourceViewPtr _normalMap = nullptr;

		/* @brief : Linear depth texture*/
		ResourceViewPtr _depthMap = nullptr;

		ResourceLayoutPtr _resourceLayout = nullptr;
		PipelineStatePtr _pipeline = nullptr;

		std::vector<VertexBufferPtr> _vertexBuffers = {};

		std::vector<IndexBufferPtr>  _indexBuffers = {};

		LowLevelGraphicsEnginePtr _engine = nullptr;

	};
}
#endif