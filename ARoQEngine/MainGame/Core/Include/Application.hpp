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
#include "GameUtility/Base/Include/GUClassUtility.hpp"
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
class Application final : public gu::NonCopyAndMove
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
private:
	/****************************************************************************
	**                Private Function
	*****************************************************************************/
	Application () = default;
	
	~Application() = default;

	/****************************************************************************
	**                Private Member Variables
	*****************************************************************************/
	GameInput& _gameInput             = GameInput::Instance();
	GameManager& _gameManager         = GameManager::Instance();
	rhi::core::GraphicsAPI _apiVersion = rhi::core::GraphicsAPI::DirectX12;
	bool _isApplicationPaused = false;
};
#endif