//////////////////////////////////////////////////////////////////////////////////
///             @file   SampleModel.hpp
///             @brief  3DModel drawing sample
///             @author Toide Yutaro
///             @date   2023_02_02
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SAMPLE_MODEL_HPP
#define SAMPLE_MODEL_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "MainGame/Core/Include/Scene.hpp"
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
namespace engine
{
	class SkyDome;
	class Camera;
	class Dof;
}
namespace engine
{
	class GameModel;
}

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace sample
{

	/****************************************************************************
	*				  			SampleSky
	****************************************************************************/
	/* @class     SampleSky
	*  @brief     Skybox sample
	*****************************************************************************/
	class SampleModel : public Scene
	{
		using SkyDomePtr = gu::SharedPointer<engine::SkyDome>;
		using CameraPtr = gu::SharedPointer<engine::Camera>;
		using ModelPtr = gu::SharedPointer<engine::GameModel>;
		using DofPtr   = gu::SharedPointer<engine::Dof>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Initialize(const PPPEnginePtr& engine, const GameTimerPtr& gameTimer) override;
		void Update() override;
		void Draw() override;
		void Terminate() override;
		/****************************************************************************
		**                Public Property
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		SampleModel();
		~SampleModel();
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void LoadMaterials() override;
		void OnKeyboardInput() override;
		void OnMouseInput() override;
		void OnGamePadInput() override;
		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		SkyDomePtr _skybox = nullptr;
		CameraPtr _camera = nullptr;
		ModelPtr  _model = nullptr;
		DofPtr _dof = nullptr;
	};
}
#endif