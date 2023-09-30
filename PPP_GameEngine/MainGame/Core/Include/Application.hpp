//////////////////////////////////////////////////////////////////////////////////
///             @file   TemplateText.hpp
///             @brief  TemplateText
///             @author Toide Yutaro
///             @date   2022_03_11
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef APPLICATION_HPP
#define APPLICATION_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GameTimer.hpp"
#include "GameCore/Input/Include/GameInput.hpp"
#include "GameManager.hpp"


//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////////////
//                              Class 
//////////////////////////////////////////////////////////////////////////////////

/****************************************************************************
*				  			Application
*************************************************************************//**
*  @class     Application
*  @brief     Create Main Window Class
*****************************************************************************/
class Application final
{
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	bool StartUp();
	void Run();
	void ShutDown();

	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/
	
	/******************************************** ********************************
	**                Constructor and Destructor
	*****************************************************************************/
	static Application& Instance()
	{
		static Application application;
		return application;
	}
	Application(const Application&)            = delete;
	Application& operator=(const Application&) = delete;
	Application(Application&&)                 = delete;
	Application& operator=(Application&&)      = delete;
private:
	/****************************************************************************
	**                Private Function
	*****************************************************************************/
	Application () = default;
	
	~Application() = default;

	/****************************************************************************
	**                Private Member Variables
	*****************************************************************************/
	std::shared_ptr<GameTimer> _gameTimer = nullptr;
	GameInput& _gameInput             = GameInput::Instance();
	GameManager& _gameManager         = GameManager::Instance();
	rhi::core::APIVersion _apiVersion = rhi::core::APIVersion::DirectX12;
	bool _isApplicationPaused = false;
};
#endif