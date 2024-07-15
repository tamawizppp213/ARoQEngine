//////////////////////////////////////////////////////////////////////////////////
///             @file   SampleModel.hpp
///             @brief  3DModel drawing sample
///             @author Toide Yutaro
///             @date   2023_02_02
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SAMPLE_URP_HPP
#define SAMPLE_URP_HPP

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
	class URP;
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
	*				  			SampleURP
	****************************************************************************/
	/* @class     SampleURP
	*  @brief     Universal Rendering Pipeline sample
	*****************************************************************************/
	class SampleURP : public Scene
	{
		using RendererPtr = gu::SharedPointer<engine::URP>;
		using SkyDomePtr  = gu::SharedPointer<engine::SkyDome>;
		using CameraPtr   = gu::SharedPointer<engine::Camera>;
		using ModelPtr    = gu::SharedPointer<engine::GameModel>;
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
		SampleURP();

		~SampleURP();
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
		CameraPtr  _camera = nullptr;
		ModelPtr   _model  = nullptr;
		ModelPtr   _floor  = nullptr;
		RendererPtr _renderer = nullptr;
	};
}
#endif