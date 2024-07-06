//////////////////////////////////////////////////////////////////////////////////
///             @file   Mosaic.hpp
///             @brief  Mosaic post effect
///             @author Toide Yutaro
///             @date   2023_02_23
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VIGNETTE_HPP
#define VIGNETTE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "FullScreenEffector.hpp"
#include "GameUtility/Math/Include/GMVector.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc
{
	struct VignetteSettings
	{
		gm::Float4 Color = gm::Float4(0.0f, 0.0f, 0.0f, 1.0f); // ビネットの色
		gm::Float2 UVCenter = gm::Float2(0.5f, 0.5f);          //　中心座標 (ビネットは通常, 0.5, 0.5)の位置から広がる)
		float      Intensity = 0.0f;
		float      Smoothness = 0.0f;
	};

	/****************************************************************************
	*				  			ColorChange
	****************************************************************************/
	/* @class     Color Change
	*  @brief     color change post effect
	*****************************************************************************/
	class Vignette : public IFullScreenEffector
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Resize frame buffer (Not implement)*/
		void OnResize(int newWidth, int newHeight) override;

		/*@brief : Render to back buffer*/
		void Draw() override;

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		gm::Float4 GetColor() const { return _settings.Color; }

		float GetIntensity() const { return _settings.Intensity; }

		float GetSmoothness() const { return _settings.Smoothness; }

		gm::Float2 GetUVCenter() const { return _settings.UVCenter; }

		void SetColor(const gm::Float4& color) { _isSettingChanged = true; _settings.Color = color; }

		// @note : 0～3付近が目安です.
		void SetIntensity(const float intensity) { _isSettingChanged = true; _settings.Intensity = intensity; }

		void SetSmoothness(const float smoothness) { _isSettingChanged = true; _settings.Smoothness = smoothness; }

		void SetUVCenter(const gm::Float2& uvCenter) { _isSettingChanged = true; _settings.UVCenter = uvCenter; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Vignette();

		~Vignette();

		Vignette(const LowLevelGraphicsEnginePtr& engine, const VignetteSettings& settings, const gu::tstring& addName = SP(""));

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void PrepareBuffer(const gu::tstring& addName);

		void PreparePipelineState(const gu::tstring& addName) override;

		void PrepareResourceView() override;

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		VignetteSettings _settings = {};

		bool _isSettingChanged = false;
	};
}
#endif