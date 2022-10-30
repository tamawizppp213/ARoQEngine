//////////////////////////////////////////////////////////////////////////////////
///             @file   SampleSky.hpp
///             @brief  Skybox sample
///             @author Toide Yutaro
///             @date   2022_04_23
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SAMPLE_SKY_HPP
#define SAMPLE_SKY_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "MainGame/Core/Include/Scene.hpp"
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class SkyDome;
class Camera;
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
	class SampleSky : public Scene
	{
		using SkyDomePtr = std::unique_ptr<SkyDome>;
		using CameraPtr = std::unique_ptr<Camera>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Initialize(const std::shared_ptr<LowLevelGraphicsEngine>& engine, GameTimer* gameTimer) override;
		void Update   () override;
		void Draw     () override;
		void Terminate() override;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		SampleSky();
		~SampleSky();
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void PrepareCamera();
		void LoadMaterials() override;
		void OnKeyboardInput() override;
		void OnMouseInput  () override;
		void OnGamePadInput() override;
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		SkyDomePtr _skybox = nullptr;
		CameraPtr _camera = nullptr;
	};
}
#endif