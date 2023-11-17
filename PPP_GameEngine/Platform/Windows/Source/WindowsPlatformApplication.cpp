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
#include "../../Windows/Include/WindowsWindowsMessageHandler.hpp"
#include "../../Windows/Include/WindowsWindow.hpp"
#include "../../Windows/Include/WindowsPlatformApplication.hpp"
#include "../../Windows/Include/WindowsPlatformCommand.hpp"
#include "../../Core/Include/CoreCommonState.hpp"
#include <cassert>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace platform;
using namespace platform::windows;
using namespace gu;

//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
static constexpr LPCWSTR g_ApplicationName = L"Game Window";

namespace
{
	gu::SharedPointer<windows::CoreWindow> FindWindowByHWND(const std::vector<gu::SharedPointer<windows::CoreWindow>>& searchWindows, HWND handle)
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
	_messageHandler = gu::MakeShared<windows::CoreWindowMessageHandler>();
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
		application->ApplicationWindowMessageProcedure(hwnd, message, wParam, lParam);
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
			application->ApplicationWindowMessageProcedure(hwnd, message, wParam, lParam);
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

/****************************************************************************
*                     MakeWindow
*************************************************************************//**
*  @fn        std::shared_ptr<core::CoreWindow> PlatformApplication::MakeWindow()
*
*  @brief     �V�K�̃E�B���h�E�C���X�^���X���쐬���܂�. �����ł̓Z�b�g�A�b�v�͍s���܂���
*
*  @param[in] void
*
*  @return    std::shared_ptr<core::CoreWindow>
*****************************************************************************/
SharedPointer<core::CoreWindow> PlatformApplication::MakeWindow()
{
	return StaticPointerCast<core::CoreWindow>(MakeShared<windows::CoreWindow>());
}

/****************************************************************************
*                     MakeCommand
*************************************************************************//**
*  @fn        std::shared_ptr<core::PlatformCommand> PlatformApplication::MakeCommand()
*
*  @brief     �V�K�ŃR�}���h���܂Ƃ߂��N���X�̃C���X�^���X���쐬���܂�.
*
*  @param[in] void
*
*  @return    std::shared_ptr<core::PlatformCommand>
*****************************************************************************/
SharedPointer<core::PlatformCommand> PlatformApplication::MakeCommand()
{
	return StaticPointerCast<core::PlatformCommand>(MakeShared<windows::PlatformCommand>());
}

/****************************************************************************
*                     SetUpWindow
*************************************************************************//**
*  @fn        void PlatformApplication::SetUpWindow(const std::shared_ptr<core::CoreWindow>& window, const core::CoreWindowDesc& desc)
*
*  @brief     �w��̃E�B���h�E�����ۂɍ쐬����܂ōs���܂�. �Z�b�g�A�b�v���s���܂�
*
*  @param[in] const std::shared_ptr<core::CoreWindow>& window, ����̃E�B���h�E
*  @param[in] const core:CoreWidowDesc& desc, �ݒ�
*
*  @return    void
*****************************************************************************/
void PlatformApplication::SetUpWindow(const SharedPointer<core::CoreWindow>& window, const core::CoreWindowDesc& desc)
{
	assert(("window is nullptr", window));

	_windows.push_back(StaticPointerCast<windows::CoreWindow>(window));
	
	const auto windowsCoreWindow = StaticPointerCast<windows::CoreWindow>(window);
	windowsCoreWindow->Create(StaticPointerCast<core::PlatformApplication>(SharedFromThis()), desc);
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
		TranslateMessage(&_windowMessage);
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

/****************************************************************************
*                     ApplicationWindowMessageProcedure
*************************************************************************//**
*  @fn        LRESULT PlatformApplication::ApplicationWindowMessageProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
*
*  @brief     �����o�֐��ŌĂяo����悤�ɂ��� (StaticWindowProcedure�̑���)
*
*  @param[in] HWND windowHandle
*  @param[in] UINT messageCode
*  @param[in] WPARAM wParam
*  @param[in] LPARAM lParam
*
*  @return    LRESULT
*****************************************************************************/
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

	if (!window) { return 0; }

	switch (messageCode)
	{
		/*-----------------------------------------------------------------
		  Activated or deactivated�̎��ɑ�����
		--------------------------------------------------------------------*/
		case WM_ACTIVATE:
		{
			break;
		}
		/*-----------------------------------------------------------------
		  �A�N�e�B�u�ȃE�B���h�E�Ƃ͈قȂ�A�v���P�[�V�����ɑ�����E�B���h�E���A�N�e�B�u������鎞�ɑ�����
		--------------------------------------------------------------------*/
		case WM_ACTIVATEAPP:
		{
			break;
		}
		/*-----------------------------------------------------------------
		  �E�B���h�E�T�C�Y���ς�����Ƃ��ɑ�����
		--------------------------------------------------------------------*/
		case WM_SIZE:
		{
			/*-----------------------------------------------------------------
			   fullscreen window rendering size should be determined by the application. 
			   Do not adjust based on WM_SIZE message.
			--------------------------------------------------------------------*/
			if (window->GetWindowMode() == core::WindowMode::FullScreen)
			{
				return 0;
			}

			_messageHandler->OnSizeChanged(LOWORD(message.LParam), HIWORD(message.LParam));

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
				default:
				{
					break;
				}
			}
			return 0;
		}
		/*-----------------------------------------------------------------
		  �E�B���h�E�T�C�Y��ύX���Ă���E�B���h�E�ɑ��M
		--------------------------------------------------------------------*/
		case WM_SIZING:
		{
			return 0;
		}
		/*-----------------------------------------------------------------
		  DPI���ω��������ɑ��M
		--------------------------------------------------------------------*/
		case WM_DPICHANGED:
		{
			return 0;
		}
		/*-----------------------------------------------------------------
			WM_CLOSE is sent when the window is closed
		--------------------------------------------------------------------*/
		case WM_CLOSE:
		{
			_messageHandler->OnWindowClosed(gu::StaticPointerCast<core::CoreWindow>(window));
			return 0;
		}
		/*-----------------------------------------------------------------
			WM_DESTROY is sent when the window is deleted
		--------------------------------------------------------------------*/
		case WM_DESTROY:
		{
			std::erase(_windows, window);
			if (_windows.empty())
			{
				PostQuitMessage(0);
			}
			return 0;
		}
	}

	return DefWindowProc(message.WindowHandle, message.MessageCode, message.WParam, message.LParam); // default window procedure
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