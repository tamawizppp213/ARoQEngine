//////////////////////////////////////////////////////////////////////////////////
///             @file   SampleSky.hpp
///             @brief  Skybox sample
///             @author Toide Yutaro
///             @date   2022_04_23
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SAMPLE_COLOR_CHANGE_HPP
#define SAMPLE_COLOR_CHANGE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "MainGame/Core/Include/Scene.hpp"
#include <memory>
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
namespace gc
{
	class SkyDome;
	class Camera;
	class ColorChange;
	class GaussianBlur;
	class Dof;
	class Mosaic;
	class Vignette;
	class WhiteBalance;
}

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace sample
{

	/****************************************************************************
	*				  			SampleSky
	*************************************************************************//**
	*  @class     SampleSky
	*  @brief     Skybox sample
	*****************************************************************************/
	class SampleColorChange : public Scene
	{
		using SkyDomePtr = std::shared_ptr<gc::SkyDome>;
		using CameraPtr  = std::shared_ptr<gc::Camera>;
		using ColorChangePtr  = std::shared_ptr<gc::ColorChange>;
		using GaussianBlurPtr = std::shared_ptr<gc::GaussianBlur>;
		using MosaicPtr = std::shared_ptr<gc::Mosaic>;
		using VignettePtr = std::shared_ptr<gc::Vignette>;
		using WhiteBalancePtr = std::shared_ptr<gc::WhiteBalance>;
		
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Initialize(const std::shared_ptr<PPPEngine>& engine, const GameTimerPtr& gameTimer) override;
		void Update() override;
		void Draw() override;
		void Terminate() override;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		SampleColorChange();
		~SampleColorChange();
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void LoadMaterials() override;
		void OnKeyboardInput() override;
		void OnMouseInput() override;
		void OnGamePadInput() override;
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		SkyDomePtr _skybox = nullptr;

		CameraPtr _camera = nullptr;

		std::vector<ColorChangePtr> _colorChanges = {};

		GaussianBlurPtr _gaussianBlur = nullptr;

		MosaicPtr _mosaic = nullptr;

		VignettePtr _vignette = nullptr;

		WhiteBalancePtr _whiteBalance = nullptr;

		bool _useBlur = false;
		bool _useMosaic = false;
		bool _useVignette = false;
		bool _useWhiteBalance = false;
		bool _useScreenSpaceReflection = false;

		std::uint32_t _colorIndex = 0;
	};
}
#endif