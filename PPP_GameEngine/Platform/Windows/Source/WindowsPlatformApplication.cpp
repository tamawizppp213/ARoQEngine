//////////////////////////////////////////////////////////////////////////////////
//              @file   Application.cpp
///             @brief  Main Window
///             @author Toide Yutaro
///             @date   2022_04_13
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#ifdef _WIN32
#include "../../Windows/Include/WindowsPlatformApplication.hpp"
#include "../../Windows/Include/WindowsWindowsMessageHandler.hpp"
#include "../../Windows/Include/WindowsWindow.hpp"
#include "../../Core/Include/CoreCommonState.hpp"
#include "GameUtility/Base/Include/Screen.hpp"
#include <cassert>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace platform;
using namespace platform::windows;

//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
static constexpr LPCWSTR g_ApplicationName = L"Game Window";

namespace
{
	std::shared_ptr<windows::CoreWindow> FindWindowByHWND(const std::vector<std::shared_ptr<windows::CoreWindow>>& searchWindows, HWND handle)
	{
		for (int i = 0; i < searchWindows.size(); ++i)
		{
			if (searchWindows[i]->GetHWND() == handle)
			{
				return searchWindows[i];
			}
		}

		return nullptr;
	}
}

#pragma region Constructor and Destructor
PlatformApplication::PlatformApplication() : core::PlatformApplication()
{
	/*---------------------------------------------------------------
					Create instance handle
	-----------------------------------------------------------------*/
	_instanceHandle = GetModuleHandleA(NULL);
	assert(("_instanceHandle is nullptr.", _instanceHandle));

	/*---------------------------------------------------------------
					Create window class
	-----------------------------------------------------------------*/
	RegisterWindowClass();

	printf_s("Windows application is created\n");

	/*---------------------------------------------------------------
					Create message handler
	-----------------------------------------------------------------*/
	_messageHandler = std::make_shared<windows::CoreWindowMessageHandler>();
}

PlatformApplication::~PlatformApplication()
{
	_windows.clear();
	_windows.shrink_to_fit();
	_messageList.clear();
	_messageList.shrink_to_fit();

	::CoUninitialize();

	printf_s("Windows application is destroyed\n");
}

#pragma endregion Contructor and Destructor

#pragma region Main Function

LRESULT CALLBACK PlatformApplication::StaticWindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PlatformApplication* application = (windows::PlatformApplication*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	
	if (application)
	{
		return application->ApplicationWindowMessageProcedure(hwnd, message, wParam, lParam);
	}
	else
	{
		if (message == WM_CREATE)
		{ 
			const auto createStruct = (LPCREATESTRUCT)lParam;
			application = (PlatformApplication*)(createStruct)->lpCreateParams;
		}
		if (application)
		{
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)application);
			return application->ApplicationWindowMessageProcedure(hwnd, message, wParam, lParam);
		}
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

bool PlatformApplication::RegisterWindowClass()
{
	/*---------------------------------------------------------------
						Register Window Class
			http://kaitei.net/winapi/creating-windows/
	-----------------------------------------------------------------*/
	// Settings
	WNDCLASS windowClass      = {};
	windowClass.style         = CS_DBLCLKS;       // enable to use double click
	windowClass.lpfnWndProc   = StaticWindowProcedure;  // window procedure. event driven 
	windowClass.cbClsExtra    = 0;                // window class�\���̂̌�Ɋm�ۂ���̈�̃o�C�g��
	windowClass.cbWndExtra    = 0;                // windowInstance�̌�Ɋm�ۂ���̈�̃o�C�g��
	windowClass.hInstance     = _instanceHandle;  // lpfnWndProc�Ŏw�肵���֐����܂܂��C���X�^���X�ւ̃n���h�����w��
	windowClass.hIcon         = LoadIcon(NULL, IDI_APPLICATION); // �f�t�H���g�A�C�R��
	windowClass.hCursor       = NULL;             // �ʓr�Ǘ�����
	windowClass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH); // �w�i�͓����ɂ���
	windowClass.lpszMenuName  = NULL;
	windowClass.lpszClassName = APPLICATION_NAME;

	// Register
	if (!::RegisterClass(&windowClass))
	{
		MessageBox(NULL, L"Failed to register class.", NULL, NULL);
		return false;
	}

	return true;
}

std::shared_ptr<core::CoreWindow> PlatformApplication::MakeWindow()
{
	return std::make_shared<windows::CoreWindow>();
}

void PlatformApplication::SetUpWindow(const std::shared_ptr<core::CoreWindow>& window, const core::CoreWindowDesc& desc)
{
	assert(("window is nullptr", window != nullptr));

	_windows.push_back(std::static_pointer_cast<windows::CoreWindow>(window));
	
	const auto windowsCoreWindow = std::static_pointer_cast<windows::CoreWindow>(window);
	windowsCoreWindow->Create(shared_from_this(), desc);
}

/****************************************************************************
*                     PumpMessage
*************************************************************************//**
*  @fn        bool PlatformApplication::PumpMessage()
*
*  @brief     ���b�Z�[�W���o���܂�. ���̊֐��͉��z�L�[���b�Z�[�W���󂯎���, ��������b�Z�[�W�`���ɕϊ���������true��Ԃ��܂�
*
*  @param[in] void
*
*  @return    bool
*****************************************************************************/
bool PlatformApplication::PumpMessage()
{
	if (PeekMessage(&_windowMessage, NULL, 0, 0, PM_REMOVE))
	{
		/*-----------------------------------------------------------------
				���b�Z�[�W�̕ϊ��Ɗ��蓖��
		--------------------------------------------------------------------*/
		if (!TranslateMessage(&_windowMessage)) { return false; }
		DispatchMessage(&_windowMessage);

		/*-----------------------------------------------------------------
		        �A�v���P�[�V�����̏I�������m����
		--------------------------------------------------------------------*/
		if(!_isApplicationQuited && _windowMessage.message == WM_QUIT)
		{
			_isApplicationQuited = true;
		}
		return true;
	}
	else
	{
		return false;
	}
}
LRESULT PlatformApplication::ApplicationWindowMessageProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (_allowedToDeferredMessageProcessing)
	{
		_messageList.push_back(DeferredMessage(hwnd, message, wParam, lParam));
	}
	else
	{
		return ProcessDeferredWindowsMessage(DeferredMessage(hwnd, message, wParam, lParam));
	}

	return DefWindowProc(hwnd, message, wParam, lParam); // default window procedure
}

LRESULT PlatformApplication::ProcessDeferredWindowsMessage(const DeferredMessage& message)
{
	const auto window      = FindWindowByHWND(_windows, message.WindowHandle);
	const auto messageCode = message.MessageCode;

	switch (messageCode)
	{
		/*-----------------------------------------------------------------
		  Activated or deactivated�̎��ɑ�����
		--------------------------------------------------------------------*/
		case WM_ACTIVATE:
		{
			return 0; // Non action
		}
		/*-----------------------------------------------------------------
		  �A�N�e�B�u�ȃE�B���h�E�Ƃ͈قȂ�A�v���P�[�V�����ɑ�����E�B���h�E���A�N�e�B�u������鎞�ɑ�����
		--------------------------------------------------------------------*/
		case WM_ACTIVATEAPP:
		{
			return 0; // Non action
		}
		/*-----------------------------------------------------------------
		  �E�B���h�E�T�C�Y���ς�����Ƃ��ɑ�����
		--------------------------------------------------------------------*/
		case WM_SIZE:
		{
			switch (message.WParam)
			{
				case SIZE_RESTORED:
				{

					break;
				}
				case SIZE_MINIMIZED:
				{
					break;
				}
				case SIZE_MAXSHOW:
				{
					break;
				}
				case SIZE_MAXIMIZED:
				{
					break;
				}
				case SIZE_MAXHIDE:
				{
					break;
				}
			}
			return 0;
		}
		/*-----------------------------------------------------------------
			WM_CLOSE is sent when the window is closed
		--------------------------------------------------------------------*/
		case WM_CLOSE:
		{
			if(window){ _messageHandler->OnWindowClosed(window); }
			return 0;
		}
		/*-----------------------------------------------------------------
			WM_DESTROY is sent when the window is deleted
		--------------------------------------------------------------------*/
		case WM_DESTROY:
		{
			return 0;
		}
		case WM_QUIT:
		{
			PostQuitMessage(0);
			return 0;
		}
	}
}

/****************************************************************************
*                     ProcessDeferredEvents
*************************************************************************//**
*  @fn        void PlatformApplication::ProcessDeferredEvents()
*
*  @brief     �܂Ƃ߂Ĉꊇ�ł��߂��C�x���g�𔭍s����
*
*  @param[in] void
*
*  @return    void
*****************************************************************************/
void PlatformApplication::ProcessDeferredEvents()
{
	for (int i = 0; i < _messageList.size(); ++i)
	{
		ProcessDeferredWindowsMessage(_messageList[i]);
	}

	_messageList.clear();
	_messageList.shrink_to_fit();
}

#pragma endregion Main Function
#endif