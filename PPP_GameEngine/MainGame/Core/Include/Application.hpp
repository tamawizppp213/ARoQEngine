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
#include <Windows.h>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
namespace platform::core
{
	class PlatformApplication;
	class CoreWindow;
}
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
private :
	using PlatformApplicationPtr = std::shared_ptr<platform::core::PlatformApplication>;
	using CoreWindowPtr          = std::shared_ptr<platform::core::CoreWindow>;
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	bool StartUp();
	void Run();
	void ShutDown();

	LRESULT WindowMessageProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
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
	bool CreateMainWindow();
	
	LRESULT ExecuteWindowsCommand(WPARAM wParam);
	/****************************************************************************
	**                Private Member Variables
	*****************************************************************************/
	PlatformApplicationPtr _platformApplication = nullptr;
	CoreWindowPtr          _coreWindow          = nullptr;
	HINSTANCE  _appInstance = nullptr;
	HWND       _mainWindow  = nullptr;
	WNDCLASSEX _windowClass = {};
	std::shared_ptr<GameTimer> _gameTimer = nullptr;
	GameInput& _gameInput             = GameInput::Instance();
	GameManager& _gameManager         = GameManager::Instance();
	rhi::core::APIVersion _apiVersion = rhi::core::APIVersion::DirectX12;
	bool _isApplicationPaused = false;
};
#endif