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
namespace gc
{
	class SkyDome;
	class Camera;
	class URP;
}
namespace gc::core
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
	*************************************************************************//**
	*  @class     SampleURP
	*  @brief     Universal Rendering Pipeline sample
	*****************************************************************************/
	class SampleURP : public Scene
	{
		using RendererPtr = gu::SharedPointer<gc::URP>;
		using SkyDomePtr  = gu::SharedPointer<gc::SkyDome>;
		using CameraPtr   = gu::SharedPointer<gc::Camera>;
		using ModelPtr    = gu::SharedPointer<gc::core::GameModel>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Initialize(const PPPEnginePtr& engine, const GameTimerPtr& gameTimer) override;
		
		void Update() override;
		
		void Draw() override;
	
		void Terminate() override;
		
		/****************************************************************************
		**                Public Member Variables
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
		**                Protected Member Variables
		*****************************************************************************/
		SkyDomePtr _skybox = nullptr;
		CameraPtr  _camera = nullptr;
		ModelPtr   _model  = nullptr;
		ModelPtr   _floor  = nullptr;
		RendererPtr _renderer = nullptr;
	};
}
#endif