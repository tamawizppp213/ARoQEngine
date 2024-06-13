//////////////////////////////////////////////////////////////////////////////////
///             @file   Mosaic.hpp
///             @brief  Mosaic post effect
///             @author Toide Yutaro
///             @date   2023_02_23
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef WHITE_BALANCE_HPP
#define WHITE_BALANCE_HPP

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
	/****************************************************************************
	*				  			ColorChange
	****************************************************************************/
	/* @class     Color Change
	*  @brief     color change post effect
	*****************************************************************************/
	class WhiteBalance : public IFullScreenEffector
	{
	protected:
		struct WhiteBalanceSettings
		{
			float Temperature = 0.0f;
			float Tint = 0.0f;
		};
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
		float GetTemperature() const { return _settings.Temperature; }

		float GetTint() const { return _settings.Tint; }

		// @note : 0Å`3ïtãﬂÇ™ñ⁄à¿Ç≈Ç∑.
		void SetTemperature(const float temperature) { _isSettingChanged = true; _settings.Temperature = temperature; }

		void SetTint(const float tint) { _isSettingChanged = true; _settings.Tint = tint; }

	
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		WhiteBalance();

		~WhiteBalance();

		WhiteBalance(const LowLevelGraphicsEnginePtr& engine, const float temperature, const float tint, const gu::tstring& addName = SP(""));

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void PrepareBuffer(const WhiteBalanceSettings& setting, const gu::tstring& addName);

		void PreparePipelineState(const gu::tstring& addName) override;

		void PrepareResourceView() override;

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		WhiteBalanceSettings _settings = {};

		bool _isSettingChanged = false;
	};
}
#endif