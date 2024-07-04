//////////////////////////////////////////////////////////////////////////////////
//              @file   Application.cpp
///             @brief  Main Window
///             @author Toide Yutaro
///             @date   2022_04_13
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Windows/Include/WindowsPlatformApplication.hpp"
#if PLATFORM_OS_WINDOWS
#include "../../Windows/Include/WindowsMessage.hpp"
#include "../../Windows/Include/WindowsWindow.hpp"
#include "../../Windows/Include/WindowsPlatformCommand.hpp"
#include "../../Windows/Include/WindowsOS.hpp"
#include "../../Core/Include/CoreCommonState.hpp"
#include "../../Core/Include/CoreWindowMessageHandler.hpp"
#include "../../Windows/Include/WindowsError.hpp"
#include "GameUtility/Memory/Include/GUMemory.hpp"
#include "GameUtility/Base/Include/GUAssert.hpp"
#include "GameUtility/Base/Include/GUString.hpp"
#include "GameUtility/Base/Include/GUStringConverter.hpp"

#include <SetupAPI.h>
#include <cfgmgr32.h>

#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "cfgmgr32.lib")
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace platform;
using namespace platform::windows;
using namespace gu;

namespace
{
	using GetDPIForMonitorProcedure = HRESULT(WINAPI*)(HMONITOR monitor, int32 dpiType, uint32* dpiX, uint32* dpiY);
	GetDPIForMonitorProcedure GetDPIForMonitorFunction = nullptr;

	HMODULE SHCoreDLL = nullptr;
}

//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
static constexpr LPCWSTR g_ApplicationName = L"Game Window";

namespace
{
	gu::SharedPointer<windows::CoreWindow> FindWindowByHWND(const gu::DynamicArray<gu::SharedPointer<windows::CoreWindow>>& searchWindows, HWND handle)
	{
		for (int i = 0; i < searchWindows.Size(); ++i)
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
	const auto oleResult = OleInitialize(NULL);
	if(oleResult != S_OK)
	{
		printf("Failed to initialize OLE\n");
	}

	windows::WindowsOS::CoInitialize();

	_instanceHandle = GetModuleHandleA(NULL);
	Checkf(_instanceHandle, "_instanceHandle is nullptr.\n");

	/*---------------------------------------------------------------
					Create window class
	-----------------------------------------------------------------*/
	RegisterWindowClass();

	printf_s("Windows application is created\n");

	/*---------------------------------------------------------------
					Create message handler
	-----------------------------------------------------------------*/
	_messageHandler = gu::MakeShared<core::CoreWindowMessageHandler>();

	/*-----------------------------------------------------------------
					  ��DPI���ݒ�\���ǂ���
	--------------------------------------------------------------------*/
	_enableHighDPIMode = SetHighDPIMode();

	/*-----------------------------------------------------------------
					 �}�E�X���ڑ�����Ă��邩
	--------------------------------------------------------------------*/
	_isConnectedMouse = GetConnectedMiceCount() > 0;
}

PlatformApplication::~PlatformApplication()
{
	if (SHCoreDLL) { ::FreeLibrary(SHCoreDLL); }
	_windows.Clear();
	_windows.ShrinkToFit();
	_messageList.Clear();
	_messageList.ShrinkToFit();

	windows::PlatformCommand::CoUnInitialize();
	OleUninitialize();

	printf_s("Windows application is destroyed\n");
}

#pragma endregion Contructor and Destructor

#pragma region Main Function

/*!***********************************************************************
*  @brief      Windows�̃C�x���g�����s����R�[���o�b�N�֐�
*  @param[in]  HWND  �E�B���h�E�n���h��
*  @param[in]  UINT  ���b�Z�[�W
*  @param[in]  WPARAM
*  @param[in]  LPARAM
*  @return     LRESULT
**************************************************************************/
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

/*!***********************************************************************
*  @brief      Window�A�v���P�[�V�����ŐV�����E�B���h�E�N���X��o�^���܂�
*  @param[in]  void
*  @return     bool : �E�B���h�E�N���X�̓o�^�ɐ����������ǂ���
**************************************************************************/
bool PlatformApplication::RegisterWindowClass() const
{
	/*---------------------------------------------------------------
						Register Window Class
			http://kaitei.net/winapi/creating-windows/
	-----------------------------------------------------------------*/
	// Settings
	WNDCLASS windowClass      = {};
	windowClass.style         = CS_DBLCLKS;                      // enable to use double click
	windowClass.lpfnWndProc   = StaticWindowProcedure;           // window procedure. event driven 
	windowClass.cbClsExtra    = 0;                               // window class�\���̂̌�Ɋm�ۂ���̈�̃o�C�g��
	windowClass.cbWndExtra    = 0;                               // windowInstance�̌�Ɋm�ۂ���̈�̃o�C�g��
	windowClass.hInstance     = _instanceHandle;                 // lpfnWndProc�Ŏw�肵���֐����܂܂��C���X�^���X�ւ̃n���h�����w��
	windowClass.hIcon         = LoadIcon(NULL, IDI_APPLICATION); // �f�t�H���g�A�C�R��
	windowClass.hCursor       = NULL;                            // �ʓr�Ǘ�����
	windowClass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH); // �w�i�͓����ɂ���
	windowClass.lpszMenuName  = NULL;
	windowClass.lpszClassName = APPLICATION_NAME;

	// Register
	if (!::RegisterClassW(&windowClass))
	{
		MessageBoxW(NULL, L"Failed to register class.", NULL, NULL);
		return false;
	}

	return true;
}

/*!***********************************************************************
*  @brief      �V�K�̃E�B���h�E�C���X�^���X���쐬���܂�. �����ł̓Z�b�g�A�b�v�͍s���܂���
*  @param[in]  void
*  @return     SharedPointer<core::CoreWindow> �E�B���h�E
**************************************************************************/
SharedPointer<core::CoreWindow> PlatformApplication::MakeWindow()
{
	return StaticPointerCast<core::CoreWindow>(MakeShared<windows::CoreWindow>());
}

/****************************************************************************
*                     MakeCommand
****************************************************************************/
/* @fn        gu::SharedPointer<core::PlatformCommand> PlatformApplication::MakeCommand()
*
*  @brief     �V�K�ŃR�}���h���܂Ƃ߂��N���X�̃C���X�^���X���쐬���܂�.
*
*  @param[in] void
*
*  @return    gu::SharedPointer<core::PlatformCommand>
*****************************************************************************/
SharedPointer<core::PlatformCommand> PlatformApplication::MakeCommand()
{
	return StaticPointerCast<core::PlatformCommand>(MakeShared<windows::PlatformCommand>());
}

/*!***********************************************************************
*  @brief      �w��̃E�B���h�E�����ۂɍ쐬��, �Z�b�g�A�b�v����܂ōs���܂� (���������ɌĂяo���K�v)
*  @param[in]  const gu::SharedPointer<CoreWindow>& window
*  @param[in]  const CoreWindowDesc& window�̐ݒ荀��
*  @param[in]  const gu::SharedPointer<CoreWindow>& �e�̃E�B���h�E
*  @return     SharedPointer<core::CoreWindow> �E�B���h�E
**************************************************************************/
void PlatformApplication::SetUpWindow(const SharedPointer<core::CoreWindow>& window, const core::CoreWindowDesc& desc, const gu::SharedPointer<core::CoreWindow>& parentWindow)
{
	Checkf(window, "window is nullptr");

	_windows.Push(StaticPointerCast<windows::CoreWindow>(window));
	
	const auto windowsCoreWindow = StaticPointerCast<windows::CoreWindow>(window);
	windowsCoreWindow->Create(StaticPointerCast<core::PlatformApplication>(SharedFromThis()), desc, parentWindow);
}

/****************************************************************************
*                     PumpMessage
****************************************************************************/
/* @fn        bool PlatformApplication::PumpMessage()
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
****************************************************************************/
/* @fn        LRESULT PlatformApplication::ApplicationWindowMessageProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
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
	return ProcessImmediateWindowsMessage(hwnd, message, wParam, lParam);
}

/*!***********************************************************************
*  @brief      �E�B���h�E�̊e���b�Z�[�W�������������(�����������s�������������L�q���܂�)
*  @param[in]  HWND �E�B���h�E�n���h��
*  @param[in]  UINT ���b�Z�[�W
*  @param[in]  WPARAM
*  @param[in]  LPARAM
*  @return     LRESULT
**************************************************************************/
LRESULT PlatformApplication::PushDeferredWindowsMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (_allowedToDeferredMessageProcessing)
	{
		_messageList.Push(DeferredMessage(hwnd, message, wParam, lParam));
		return 0;
	}
	else
	{
		return ProcessDeferredWindowsMessage(DeferredMessage(hwnd, message, wParam, lParam));
	}
}

/*!***********************************************************************
*  @brief      �E�B���h�E�̊e���b�Z�[�W�������������(�����������s�������������L�q���܂�)
*  @param[in]  const DeferredMessage Windows��p�̃��b�Z�[�W
*  @return     LRESULT
**************************************************************************/
LRESULT PlatformApplication::ProcessImmediateWindowsMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// �E�B���h�E�����݂��Ȃ��ꍇ�͉������Ȃ�
	if (_windows.IsEmpty()) 
	{
		return DefWindowProc(hwnd, message, wParam, lParam);
	} 

	// �E�B���h�E�̎擾
	const auto window = FindWindowByHWND(_windows, hwnd);
	
	if (!window) { return DefWindowProc(hwnd, message, wParam, lParam); }

	/*-----------------------------------------------------------------
		�e���b�Z�[�W�̏���
	--------------------------------------------------------------------*/
	switch (message)
	{
		/*-----------------------------------------------------------------
			���[�U�[���z�b�g�L�[���g�p���邩�V�������͌����I�������Ƃ��ɑ��M
			(wParam: ���̓��P�[��, lParam: ���ʎq)
			https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-inputlangchangerequest
		--------------------------------------------------------------------*/
		case WM_INPUTLANGCHANGEREQUEST:
		/*-----------------------------------------------------------------
			���͌��ꂪ�ύX���ꂽ�Ƃ��ɑ��M
			(wParam: ���͌����BYTE�t�H���g����, lParam: ���ʎq)
			https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-inputlangchange
		--------------------------------------------------------------------*/
		case WM_INPUTLANGCHANGE:
		{
			PushDeferredWindowsMessage(hwnd, message, wParam, lParam);
			break;
		}
		/*-----------------------------------------------------------------
			�E�B���h�E����\���܂��͕\������悤�Ƃ��Ă��鎞�ɃE�B���h�E�ɑ��M
			(wParam: �E�B���h�E���\������Ă��邩�ǂ����̃t���O, lParam: �\������Ă���E�B���h�E�̏��)
			https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-showwindow
		--------------------------------------------------------------------*/
		case WM_SHOWWINDOW:
		{
			PushDeferredWindowsMessage(hwnd, message, wParam, lParam);
			break;
		}
		/*-----------------------------------------------------------------
			�E�B���h�E�ɑ΂��A���̃T�C�Y���ύX���ꂽ��ɑ��M����܂�
			(wParam: �v�����ꂽ�T�C�Y�ύX�̎��, lParam: ���ʂ̓N���C�A���g�̈�̕�, ��ʂ͍���)
			https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-size
		--------------------------------------------------------------------*/
		case WM_SIZE:
		{
			PushDeferredWindowsMessage(hwnd, message, wParam, lParam);

			const bool wasMaximized = wParam == SIZE_MAXIMIZED;
			const bool wasRestored  = wParam == SIZE_RESTORED;

			if (wasMaximized || wasRestored)
			{
				_messageHandler->OnWindowAction(window, wasMaximized ? core::WindowActionType::Maximize : core::WindowActionType::Restore);
			}
			
			return 0;
		}
		/*-----------------------------------------------------------------
			�E�B���h�E�ɑ΂��A���̃T�C�Y���ύX����Ă���Œ��ɑ��M����܂�
			(wParam: �T�C�Y���ݒ肳��Ă���E�B���h�E�̒[, lParam:  RECT �\���̂ւ̃|�C���^)
			https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-sizing
		--------------------------------------------------------------------*/
		case WM_SIZING:
		{
			PushDeferredWindowsMessage(hwnd, message, wParam, lParam);
			break;
		}
		/*-----------------------------------------------------------------
			�ړ��܂��̓T�C�Y�ύX���[�_�� ���[�v�ɓ�������A�E�B���h�E�� 1 �񑗐M
			(wParam: �g�p���Ȃ�, lParam: �g�p���Ȃ�)
			https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-entersizemove
		--------------------------------------------------------------------*/
		case WM_ENTERSIZEMOVE:
		{
			_inModalSizeLoop = true;
			PushDeferredWindowsMessage(hwnd, message, wParam, lParam);
			break;
		}
		/*-----------------------------------------------------------------
			�ړ��܂��̓T�C�Y�ύX���[�_�� ���[�v���I��������A�E�B���h�E�� 1 �񑗐M����܂�
			(wParam: �g�p���Ȃ�, lParam: �g�p���Ȃ�)
			https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-exitsizemove
		--------------------------------------------------------------------*/
		case WM_EXITSIZEMOVE:
		{
			_inModalSizeLoop = false;
			PushDeferredWindowsMessage(hwnd, message, wParam, lParam);
			break;
		}
		/*-----------------------------------------------------------------
			�E�B���h�E���ړ����ꂽ��ɑ��M����܂��B
			(wParam: �g�p���Ȃ� lParam : �E�B���h�E�̃N���C�A���g�̈�̍������ x �� y ���W ���ʃ��[�h�ɂ� x ���W���܂܂�A��ʃ��[�h�ɂ� y ���W���܂܂�܂�)
			https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-sizing
		--------------------------------------------------------------------*/
		case WM_MOVE:
		{
			const int32 x = LOWORD(lParam);
			const int32 y = HIWORD(lParam);

			_messageHandler->OnWindowMoved(window, x, y);
			return 0;
		}
		/*-----------------------------------------------------------------
		 ����̉�ʍ��W�ɑΉ�����E�B���h�E�̕��������肷�邽�߂ɁA�E�B���h�E�ɑ��M
		 (wParam : �g�p���Ȃ�, lParam : �J�[�\����x, y���W)
		 https://learn.microsoft.com/ja-jp/windows/win32/inputdev/wm-nchittest
		--------------------------------------------------------------------*/
		case WM_NCHITTEST:
		{
			break;
		}
		/*-----------------------------------------------------------------
		 �J�[�\������A�N�e�B�u�E�B���h�E�ɂ���Ƃ��Ƀ}�E�X�{�^���������Ƒ��M
		 (wParam : �A�N�e�B�u�������E�B���h�E�n���h��, lParam : ����16�r�b�g�Ƀq�b�g�e�X�g�l, ���16�r�b�g�Ƀ}�E�X���b�Z�[�W)
		 https://learn.microsoft.com/ja-jp/windows/win32/inputdev/wm-mouseactivate
		--------------------------------------------------------------------*/
		case WM_MOUSEACTIVATE:
		{
			PushDeferredWindowsMessage(hwnd, message, wParam, lParam);
			break;
		}
		/*-----------------------------------------------------------------
				Window���A�N�e�B�u��, ��A�N�e�B�u�����ꂽ���ɑ�����	
		--------------------------------------------------------------------*/
		case WM_ACTIVATE:
		{
			PushDeferredWindowsMessage(hwnd, message, wParam, lParam);
			break;
		}
		/*-----------------------------------------------------------------
		�A�N�e�B�u�ȃE�B���h�E�Ƃ͈قȂ�A�v���P�[�V�����ɑ�����E�B���h�E���A�N�e�B�u������鎞�ɑ�����
		--------------------------------------------------------------------*/
		case WM_ACTIVATEAPP:
		{
			_isResizing = false;
			PushDeferredWindowsMessage(hwnd, message, wParam, lParam);
			break;
		}
		/*-----------------------------------------------------------------
			WM_CLOSE�̓E�B���h�E������ꂽ�Ƃ��ɑ��M����܂�
		--------------------------------------------------------------------*/
		case WM_CLOSE:
		{
			PushDeferredWindowsMessage(hwnd, message, wParam, lParam);
			break;
		}
		/*-----------------------------------------------------------------
		  �V�X�e���S�̂̐ݒ��ύX�����Ƃ�, �|���V�[�ݒ肪�ύX���ꂽ�Ƃ��ɑS�Ă̍ŏ�ʃE�B���h�E�ɑ��M
		--------------------------------------------------------------------*/
		case WM_SETTINGCHANGE:
		{
			PushDeferredWindowsMessage(hwnd, message, wParam, lParam);
			break;
		}
		/*-----------------------------------------------------------------
		  �ΏۃE�B���h�E�̈ꕔ��h��Ԃ��v�����s���ꍇ�ɑ��M
		--------------------------------------------------------------------*/
		case WM_PAINT:
		{
			if (_isResizing)
			{
				_messageHandler->OnOSPainted(window);
			}
			break;
		}
		/*-----------------------------------------------------------------
		 �E�B���h�E�̔w�i����������K�v������Ƃ� (���Ƃ��΁A�E�B���h�E�̃T�C�Y��ύX����ꍇ) �ɑ��M����܂�
		--------------------------------------------------------------------*/
		case WM_ERASEBKGND:
		{
			// https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-erasebkgnd
			return 1; // ���g�Ŕw�i�̏������s������, 0�ȊO�̒l��Ԃ��K�v������. 
		}
		/*-----------------------------------------------------------------
		 �E�B���h�E�̔w�i����������K�v������Ƃ� (���Ƃ��΁A�E�B���h�E�̃T�C�Y��ύX����ꍇ) �ɑ��M����܂�
		--------------------------------------------------------------------*/
		case WM_NCACTIVATE:
		{
			//  OS�E�B���h�E�̃{�[�_�[���g�p���Ȃ�����A��N���C�A���g�̈悪�A�N�e�B�u���܂��͔�A�N�e�B�u�����Ƀ{�[�_�[��`�悵�Ȃ��悤�ɂ��܂�.
			//  �W���I�ȃA�N�e�B�u�����s����悤��true��Ԃ�
			if (!window->GetDesc().HasWindowOSWindowBorder)
			{
				return true;
			}
			break;
		}
		/*-----------------------------------------------------------------
		 �t���[����h��Ԃ��K�v������Ƃ��ɃE�B���h�E�ɑ��M�����
		--------------------------------------------------------------------*/
		case WM_NCPAINT:
		{
			//  OS�̃E�B���h�E���E���g�p���Ȃ�����A�N���C�A���g�ȊO�̗̈��`�悷��Ăяo�����C���^�[�Z�v�g����
			if (!window->GetDesc().HasWindowOSWindowBorder)
			{
				return 0;
			}
			break;
		}
		/*-----------------------------------------------------------------
		 �E�B���h�E���j���[����R�}���h��I�������Ƃ�, ���[�U�[���ő剻�E�ŏ����E�����E����{�^�����N���b�N�����Ƃ��ɑ��M
		--------------------------------------------------------------------*/
		case WM_SYSCOMMAND:
		{
			switch(wParam & 0xFFF0)
			{
				case SC_RESTORE:
				{
					if (IsIconic(hwnd))
					{
						::ShowWindow(hwnd, SW_RESTORE);
						return 0;
					}
					if (!_messageHandler->OnWindowAction(window, core::WindowActionType::Restore))
					{
						return 1;
					}
					break;
				}
				case SC_MINIMIZE:
				{
					_messageHandler->OnWindowAction(window, core::WindowActionType::Minimize);
					break;
				}
				case SC_MAXIMIZE:
				{
					_messageHandler->OnWindowAction(window, core::WindowActionType::Maximize);
					break;
				}
				case SC_CLOSE:
				{
					PushDeferredWindowsMessage(hwnd, message, wParam, lParam);
					break;
				}
			}
			break;
		}
		/*-----------------------------------------------------------------
		 window�̃T�C�Y�ύX�ł���ŏ��̑傫���ƍő�̑傫����ύX����
		 (wParam : �g�p���Ȃ�, lParam : MINMAXINFO�ւ̃|�C���^)
		 https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-getminmaxinfo
		--------------------------------------------------------------------*/
		case WM_GETMINMAXINFO:
		{
			return 0;
		}
		/*-----------------------------------------------------------------
		 ��N���C�A���g�̈�ɃJ�[�\��������Ƃ��ɃN���b�N���ꂽ�Ƃ��ɑ��M
		 (wParam : �q�b�g�e�X�g�l(WM_NCHITTEST), lParam : �J�[�\����x, y���W���܂�POINTS�\���̂ł͂��邪, �g�p����ȂƋL�� 
		 https://learn.microsoft.com/ja-jp/windows/win32/inputdev/wm-nclbuttondown)
		--------------------------------------------------------------------*/
		case WM_NCLBUTTONDOWN:
		case WM_NCRBUTTONDOWN:
		case WM_NCMBUTTONDOWN:
		{
			switch (wParam)
			{
				case HTMINBUTTON:
				case HTMAXBUTTON:
				case HTCLOSE:
				case HTCAPTION:
				{
					if (!_messageHandler->OnWindowAction(window, core::WindowActionType::ClickedNonClientArea))
					{
						return 1;
					}
					break;
				}
				default:
				{
					break;
				}
			}
			break;
		}
		/*-----------------------------------------------------------------
			�f�B�X�v���C�̉𑜓x���ύX���ꂽ�ꍇ�ɑ��M����� (�ŏ�ʃE�B���h�E)
			(wParam:�C���[�W�[�x, lParam: ��ʂ̐���(����), �����𑜓x(���))
			https://learn.microsoft.com/ja-jp/windows/win32/gdi/wm-displaychange
		--------------------------------------------------------------------*/
		case WM_DISPLAYCHANGE:
		{
			break;
		}
		/*-----------------------------------------------------------------
			�L��DPI���ύX���ꂽ�ꍇ�ɑ��M
			(wParam: dpi��X (HI), Y�� (LO), lParam: RECT�\���̂̃|�C���^)
			https://learn.microsoft.com/ja-jp/windows/win32/hidpi/wm-dpichanged
		--------------------------------------------------------------------*/
		case WM_DPICHANGED:
		{
			PushDeferredWindowsMessage(hwnd, message, wParam, lParam);
			break;
		}
		/*-----------------------------------------------------------------
			�R���g���[���Ɋ֘A�t�����Ă���L�[�{�[�h���̓��b�Z�[�W����������
			https://learn.microsoft.com/ja-jp/windows/win32/dlgbox/wm-getdlgcode
		--------------------------------------------------------------------*/
		case WM_GETDLGCODE:
		{
			return DLGC_WANTALLKEYS;
		}
		/*-----------------------------------------------------------------
			CreateWindow���Ăяo���ăE�B���h�E�̍쐬��v������Ƃ��ɑ��M
			(wParam : �g�p���Ȃ�, lParam : �C�x���g�ŗL�̃f�[�^���܂ލ\���̂ւ̃|�C���^)
			https://learn.microsoft.com/ja-jp/windows/win32/devio/wm-devicechange
		--------------------------------------------------------------------*/
		case WM_CREATE:
		{
			return 0;
		}
		/*-----------------------------------------------------------------
			�f�o�C�X�܂��̓R���s���[�^�[�̃n�[�h�E�F�A�\���̕ύX���A�v���P�[�V�����ɒʒm���܂�
			(wParam : �����C�x���g, lParam : CreateStruct�\���̂ւ̃|�C���^)
			https://learn.microsoft.com/ja-jp/windows/win32/winmsg/wm-create
		--------------------------------------------------------------------*/
		case WM_DEVICECHANGE:
		{
			break;
		}
		/*-----------------------------------------------------------------
			Window���j�����ꂽ���ɑ�����
		--------------------------------------------------------------------*/
		case WM_DESTROY:
		{
			_windows.Remove(window);
			if (_windows.IsEmpty())
			{
				PostQuitMessage(0);
			}
			return 0;
		}
	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}

/*!***********************************************************************
*  @brief      �E�B���h�E�̊e���b�Z�[�W�������������
*  @param[in]  const DeferredMessage Windows��p�̃��b�Z�[�W
*  @return     LRESULT
**************************************************************************/
LRESULT PlatformApplication::ProcessDeferredWindowsMessage(const DeferredMessage& message)
{
	const auto window      = FindWindowByHWND(_windows, message.WindowHandle);
	const auto messageCode = message.MessageCode;

	if (!window)          { return FALSE; }
	if (!_messageHandler) { return FALSE; }

	switch (messageCode)
	{
		/*-----------------------------------------------------------------
		  Window���A�N�e�B�u��, ��A�N�e�B�u�����ꂽ���ɑ�����
		--------------------------------------------------------------------*/
		case WM_ACTIVATE:
		{
			core::ActivationType activationType = core::ActivationType::InActivate;

			if (LOWORD(message.WParam) & WA_ACTIVE)
			{
				activationType = _isForceActivateByMouse ? core::ActivationType::ClickActivate : core::ActivationType::Activate;
			}
			else if (LOWORD(message.WParam) & WA_CLICKACTIVE)
			{
				activationType = core::ActivationType::ClickActivate;
			}

			_isForceActivateByMouse = false;

			return _messageHandler->OnWindowActivationChanged(window, core::ActivationType::Activate) ? 0 : 1;
		}

		/*-----------------------------------------------------------------
		  �A�N�e�B�u�ȃE�B���h�E�Ƃ͈قȂ�A�v���P�[�V�����ɑ�����E�B���h�E���A�N�e�B�u������鎞�ɑ�����
		--------------------------------------------------------------------*/
		case WM_ACTIVATEAPP:
		{
			return _messageHandler->OnWindowApplicationActivationChanged(!!message.WParam) ? 0 : 1;
		}
		/*-----------------------------------------------------------------
		  �V�X�e���S�̂̐ݒ��ύX�����Ƃ�, �|���V�[�ݒ肪�ύX���ꂽ�Ƃ��ɑS�Ă̍ŏ�ʃE�B���h�E�ɑ��M
		--------------------------------------------------------------------*/
		case WM_SETTINGCHANGE:
		{
			return _messageHandler->OnSettingChanged() ? 0 : 1;
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

			_messageHandler->OnSizeChanged(window, LOWORD(message.LParam), HIWORD(message.LParam));

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
			_messageHandler->OnWindowResizing(window);
			break;
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
			return _messageHandler->OnWindowClosed(gu::StaticPointerCast<core::CoreWindow>(window)) ? 0 : 1;
		}
		/*-----------------------------------------------------------------
			WM_DESTROY is sent when the window is deleted
		--------------------------------------------------------------------*/
		case WM_DESTROY:
		{
			_windows.Remove(window);
			if (_windows.IsEmpty())
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
****************************************************************************/
/* @fn        void PlatformApplication::ProcessDeferredEvents()
*
*  @brief     �܂Ƃ߂Ĉꊇ�ł��߂��C�x���g�𔭍s����
*
*  @param[in] void
*
*  @return    void
*****************************************************************************/
void PlatformApplication::ProcessDeferredEvents()
{
	for (int i = 0; i < _messageList.Size(); ++i)
	{
		ProcessDeferredWindowsMessage(_messageList[i]);
	}

	_messageList.Clear();
	_messageList.ShrinkToFit();
}

#pragma endregion Main Function

#pragma region Monitor
/*!***********************************************************************
*  @brief      ���j�^�[��dot per inch���擾����.  https://zenn.dev/tenka/articles/windows_display_monitor_dpi
*  @param[in]  const core::MonitorInfo& monitor
*  @return     gu::int32 DPI
**************************************************************************/
gu::int32 PlatformApplication::GetMonitorDPI(const core::MonitorInfo& monitorInfo) const
{	
	if (!EnableHighDPIAwareness()) { return USER_DEFAULT_SCREEN_DPI; }

	int32 displayDPI = USER_DEFAULT_SCREEN_DPI;

	if (::GetDPIForMonitorFunction)
	{
		const RECT monitorDimenstion =
		{
			.left   = monitorInfo.DisplayRectangle.Left,
			.top    = monitorInfo.DisplayRectangle.Top,
			.right  = monitorInfo.DisplayRectangle.Right,
			.bottom = monitorInfo.DisplayRectangle.Bottom
		};
		
		uint32 dpiX = 0, dpiY = 0;
		HMONITOR monitor = MonitorFromRect(&monitorDimenstion, MONITOR_DEFAULTTONEAREST);
		if(monitor && SUCCEEDED(GetDPIForMonitorFunction(monitor, 0, &dpiX, &dpiY)))
		{
			displayDPI = dpiX;
		}
	}
	else
	{
		auto displayContext = GetDC(nullptr);
		displayDPI = GetDeviceCaps(displayContext, LOGPIXELSX);
		ReleaseDC(nullptr, displayContext);
	}

	return displayDPI;
}

/****************************************************************************
*                     SetHighDPIMode
****************************************************************************/
/* @fn        void CoreApplication::SetHighDPIMode()
*
*  @brief     ��DPI�̐ݒ�ɏ��������܂�.
*             https://zenn.dev/tenka/articles/windows_display_monitor_dpi
*
*  @param[in] void
*
*  @return    bool
*****************************************************************************/
bool PlatformApplication::SetHighDPIMode()
{
	/*---------------------------------------------------------------
					  DLL�̓ǂݍ���
	-----------------------------------------------------------------*/
	const auto userDLL   = LoadLibrary(L"user32.dll");
	SHCoreDLL = LoadLibrary(L"shcore.dll");

	/*---------------------------------------------------------------
					  DPI���[�h�̎�ސݒ�
	-----------------------------------------------------------------*/
	enum Process_DPI_Awareness
	{
		Process_DPI_UnAware           = 0,
		Process_System_DPI_Aware      = 1,
		Process_Per_Monitor_DPI_Aware = 2
	};

	/*---------------------------------------------------------------
			      DPI���[�h�̎擾 : Win 8.1 and later
	-----------------------------------------------------------------*/
	if (SHCoreDLL)
	{
		// �֐��|�C���^�̓o�^, �Ԃ�l(�|�C���^)(����)�Ŏ擾�ł���
		using GetProcessDPIAwarenessProcedure = HRESULT(WINAPI*)(HANDLE process, Process_DPI_Awareness* awareness);
		using SetProcessDPIAwarenessProcedure = HRESULT(WINAPI*)(Process_DPI_Awareness value);

		auto SetProcessDPIAwareness = (SetProcessDPIAwarenessProcedure)::GetProcAddress(SHCoreDLL, "SetProcessDpiAwareness");
		auto GetProcessDPIAwareness = (GetProcessDPIAwarenessProcedure)::GetProcAddress(SHCoreDLL, "GetProcessDpiAwareness");
		GetDPIForMonitorFunction    = (GetDPIForMonitorProcedure)      ::GetProcAddress(SHCoreDLL, "GetDpiForMonitor");

		if (!GetProcessDPIAwareness) { ::FreeLibrary(SHCoreDLL); return false; }
		if (!SetProcessDPIAwareness) { ::FreeLibrary(SHCoreDLL); return false; }

		// awareness�̐ݒ� (DPI���[�h�̐ݒ�: MS���������Ă���perMonitor�ɕύX����)
		Process_DPI_Awareness currentAwareness = Process_DPI_UnAware;
		GetProcessDPIAwareness(nullptr, &currentAwareness);

		if (currentAwareness != Process_Per_Monitor_DPI_Aware)
		{
			OutputDebugString(L"Setting process to per monitor DPI aware\n");
			
			SetProcessDPIAwareness(Process_Per_Monitor_DPI_Aware);

			return ErrorLogger::Succeed();
		}
		else
		{
			return true;
		}
	}
	/*---------------------------------------------------------------
				 DPI���[�h�̎擾 :For vista, win7, and win8
	-----------------------------------------------------------------*/
	else if (userDLL)
	{
		// �֐��|�C���^�̓o�^, �Ԃ�l(�|�C���^)(����)�Ŏ擾�o����
		using SetProcessDPIAwareProcedure = BOOL(WINAPI*)(void);
		auto SetProcessDPIAware = (SetProcessDPIAwareProcedure)::GetProcAddress(userDLL, "SetProcessDPIAware");

		// �֐��̎��s
		if (SetProcessDPIAware) 
		{
			return SetProcessDPIAware();
		}
		else
		{
			::OutputDebugString(L"Failed to set dpi mode\n");
			return false;
		}
	}
	else
	{
		return false;
	}
}

/****************************************************************************
*                     GetDPIScaleFactorAtPixelPoint
****************************************************************************/
/* @fn        float CoreApplication::GetDPIScaleFactorAtPixelPoint(const float x, const float y) const
*
*  @brief     ����s�N�Z���ʒu�ł�DPI�̊g�嗦�������܂�.
*
*  @param[in] const float x
*  @param[in] const float y
*
*  @return    float
*****************************************************************************/
float PlatformApplication::GetDPIScaleFactorAtPixelPoint(const float x, const float y) const
{
	float scale = 1.0f;

	if (GetDPIForMonitorFunction)
	{
		POINT    position = { static_cast<LONG>(x), static_cast<LONG>(y) };
		HMONITOR monitor  = MonitorFromPoint(position, MONITOR_DEFAULTTONEAREST);

		uint32 dpiX = 0, dpiY = 0;
		if (monitor && SUCCEEDED(GetDPIForMonitorFunction(monitor, 0, &dpiX, &dpiY)))
		{
			scale = (float)dpiX / USER_DEFAULT_SCREEN_DPI;
		}
	}
	else
	{
		auto displayContext = GetDC(nullptr);
		int32 displayDPI    = GetDeviceCaps(displayContext, LOGPIXELSX);
		scale = (float)displayDPI / USER_DEFAULT_SCREEN_DPI;
		ReleaseDC(nullptr, displayContext);
	}

	return scale;
}

/****************************************************************************
*                     GetMonitorsInfo
****************************************************************************/
/* @fn        void PlatformApplication::GetMonitorsInfo(gu::DynamicArray<core::MonitorInfo>& monitorInfo) const
*
*  @brief     ���j�^�[�̏����擾����
*
*  @param[out] gu::DynamicArray<core::MOnitorInfo> ���j�^�[�̏�񃊃X�g
* 
*  @return    void
*****************************************************************************/
void PlatformApplication::GetMonitorsInfo(gu::DynamicArray<core::MonitorInfo>& monitorInfos) const
{
	/*---------------------------------------------------------------
				 ���O����
	-----------------------------------------------------------------*/
	// ���j�^�[�̐���������p
	DISPLAY_DEVICE device;
	Memory::Zero(&device, sizeof(device));
	device.cb = sizeof(device);

	// ���j�^�[���擾�p
	DISPLAY_DEVICE monitor;
	Memory::Zero(&monitor, sizeof(monitor));
	monitor.cb = sizeof(monitor);

	/*---------------------------------------------------------------
				 �f�B�X�v���C�A�_�v�^�[�擾
	-----------------------------------------------------------------*/
	for (DWORD displayIndex = 0; EnumDisplayDevices(nullptr, displayIndex, &device, 0); 
		++displayIndex, Memory::Zero(&device, sizeof(device)), device.cb = sizeof(device)) // �I����
	{
		// �f�X�N�g�b�v�ɐڑ�����Ă�����̈ȊO�͖���
		if ((device.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP) <= 0) { continue; }

		/*---------------------------------------------------------------
					 ���j�^�[�̎擾
		-----------------------------------------------------------------*/
		for (DWORD monitorIndex = 0; EnumDisplayDevices(device.DeviceName, monitorIndex, &monitor, 0);
			++monitorIndex, Memory::Zero(&monitor, sizeof(monitor)), monitor.cb = sizeof(monitor))
		{
			// �A�N�e�B�u�ȃf�o�C�X�ȊO�͖��� + ��\���̋^���f�o�C�X������
			if (!(monitor.StateFlags & DISPLAY_DEVICE_ACTIVE         )) { continue; }
			if (  monitor.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER) { continue; }

			/*---------------------------------------------------------------
						 core::MonitorInfo�ɏ����i�[
			-----------------------------------------------------------------*/
			core::MonitorInfo monitorInfo = {};

			EnumDisplayMonitors(nullptr, nullptr, MonitorEnumProcedure, (LPARAM)&monitorInfo);
			
			// ���j�^�[�̃f�o�C�X��
			monitorInfo.Name      = device.DeviceName;

			// monitor��ID
			monitorInfo.ID        = monitor.DeviceID;
			
			// Primary monitor���ǂ���
			monitorInfo.IsPrimary = (device.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) > 0;

			// ���j�^�[�̕����I�ȃT�C�Y���擾����
			const auto hdc = CreateDC(device.DeviceName, device.DeviceName, nullptr, nullptr);
			const auto physicalWidth  = GetDeviceCaps(hdc, HORZSIZE) / 10.0f; //[cm]�P��
			const auto physicalHeight = GetDeviceCaps(hdc, VERTSIZE) / 10.0f;
			const auto pixelWidth     = GetDeviceCaps(hdc, HORZRES); // �s�N�Z������
			const auto pixelHeight    = GetDeviceCaps(hdc, VERTRES); // �s�N�Z���c��   
			const auto inch           = sqrtf(physicalHeight * physicalHeight + physicalWidth * physicalWidth) / 2.54f;
			
			// ���j�^�[�̕����I��DPI���擾����
			const auto dpi = sqrtf((float)pixelWidth * pixelWidth + (float)pixelHeight * pixelHeight) / inch;

			// ���j�^�[��DPI���擾����
			if (dpi < 96 || dpi > 300)
			{
				monitorInfo.DPI = GetMonitorDPI(monitorInfo);
			}
			else
			{
				const auto centerX = 0.5f * float(monitorInfo.WorkArea.Right + monitorInfo.WorkArea.Left);
				const auto centerY = 0.5f * float(monitorInfo.WorkArea.Top + monitorInfo.WorkArea.Bottom);
				const auto dpiScale = GetDPIScaleFactorAtPixelPoint(centerX, centerY);
				monitorInfo.DPI = (int32)((float)monitorInfo.DPI * dpiScale);
			}

			// ���j�^�[�̕��ƍ������擾����
			monitorInfo.NativeWidth = pixelWidth;
			monitorInfo.NativeHeight = pixelHeight;

			monitorInfos.Push(monitorInfo);

			DeleteDC(hdc);
		}
		
	}
}

/*!***********************************************************************
*  @brief      Window�A�v���P�[�V�����ŐV�����E�B���h�E�N���X��o�^���܂�
*  @param[in]  void
*  @return     bool : �E�B���h�E�N���X�̓o�^�ɐ����������ǂ���
**************************************************************************/
BOOL CALLBACK PlatformApplication::MonitorEnumProcedure(HMONITOR monitor, [[maybe_unused]]HDC monitorDC, [[maybe_unused]]LPRECT rect, LPARAM userData)
{
	MONITORINFOEX monitorInfoExtension = {};
	monitorInfoExtension.cbSize = sizeof(monitorInfoExtension);
	GetMonitorInfo(monitor, &monitorInfoExtension);

	auto monitorInfo = (core::MonitorInfo*)userData;
	if (monitorInfo->Name == monitorInfoExtension.szDevice)
	{
		monitorInfo->DisplayRectangle.Bottom = monitorInfoExtension.rcMonitor.bottom;
		monitorInfo->DisplayRectangle.Left   = monitorInfoExtension.rcMonitor.left;
		monitorInfo->DisplayRectangle.Right  = monitorInfoExtension.rcMonitor.right;
		monitorInfo->DisplayRectangle.Top    = monitorInfoExtension.rcMonitor.top;

		monitorInfo->WorkArea.Bottom = monitorInfoExtension.rcWork.bottom;
		monitorInfo->WorkArea.Left   = monitorInfoExtension.rcWork.left;
		monitorInfo->WorkArea.Right  = monitorInfoExtension.rcWork.right;
		monitorInfo->WorkArea.Top    = monitorInfoExtension.rcWork.top;
		return FALSE;
	}

	return TRUE;
}

/*!***********************************************************************
*  @brief      �^�X�N�o�[�Ȃǂ𖳎�������Ɨ̈��Ԃ��܂�.
*  @param[in]  const core::Rectangle& �E�B���h�E
*  @return     core::Rectangle ��Ɨ̈�
**************************************************************************/
core::Rectangle PlatformApplication::GetWorkArea(const core::Rectangle& window)
{
	const RECT windowsDimension =
	{
		.left   = window.Left,
		.top    = window.Top,
		.right  = window.Right,
		.bottom = window.Bottom
	};

	/*---------------------------------------------------------------
		   �E�B���h�E�T�C�Y�ɑ΂������̈悪�ł��傫���f�B�X�v���C���j�^�[���擾
	-----------------------------------------------------------------*/
	HMONITOR bestMonitor = MonitorFromRect(&windowsDimension, MONITOR_DEFAULTTONEAREST);

	// monitor Info���擾
	MONITORINFO monitorInfo;
	Memory::Zero(&monitorInfo, sizeof(monitorInfo));
	monitorInfo.cbSize = sizeof(monitorInfo);
	GetMonitorInfo(bestMonitor, &monitorInfo);

	/*---------------------------------------------------------------
		   ��Ɨ̈���擾����	
	-----------------------------------------------------------------*/
	core::Rectangle workArea =
	{
		.Left   = monitorInfo.rcWork.left,
		.Top    = monitorInfo.rcWork.top,
		.Right  = monitorInfo.rcWork.right,
		.Bottom = monitorInfo.rcWork.bottom
	};

	return workArea;
}

/*!***********************************************************************
*  @brief      �ڑ�����Ă���}�E�X�̌����擾���܂�.
*  @param[in]  void
*  @return     gu:int32
**************************************************************************/
gu::int32 PlatformApplication::GetConnectedMiceCount()
{
	DynamicArray<RAWINPUTDEVICELIST> rawInputDevices = {};
	
	gu::uint32 deviceCount = 0;

	/*---------------------------------------------------------------
				�ڑ�����Ă���Input�f�o�C�X���擾
	-----------------------------------------------------------------*/
	GetRawInputDeviceList(nullptr, &deviceCount, sizeof(RAWINPUTDEVICELIST));

	if (deviceCount == 0)
	{
		_isConnectedMouse = false;
		return 0;
	}

	rawInputDevices.Resize(deviceCount);
	GetRawInputDeviceList(rawInputDevices.Data(), &deviceCount, sizeof(RAWINPUTDEVICELIST));

	/*---------------------------------------------------------------
				�}�E�X�̌��̒��o
	-----------------------------------------------------------------*/
	gu::int32 mouseCount = 0;

	for (const auto& device : rawInputDevices)
	{
		// �}�E�X�f�o�C�X�݂̂𒊏o
		if (device.dwType != RIM_TYPEMOUSE) { continue; }

		// ���������擾
		uint32 nameLength = 0;
		if (GetRawInputDeviceInfoA(device.hDevice, RIDI_DEVICENAME, nullptr, &nameLength) == static_cast<uint32>(-1))
		{
			continue;
		}

		// ���������擾
		gu::string deviceName(nameLength, true);
		if (GetRawInputDeviceInfoA(device.hDevice, RIDI_DEVICENAME, deviceName.CString(), &nameLength) == static_cast<uint32>(-1))
		{
			continue;
		}

		if (deviceName.Contains("RDP_MOU", true))
		{
			continue;
		}

		mouseCount++;
	}

	// �����[�g�f�X�N�g�b�v�Z�b�V���������݂���ꍇ�ɂ�, �}�E�X���ڑ�����Ă���Ɖ��肷��
	if (GetSystemMetrics(SM_REMOTESESSION))
	{
		mouseCount++;
	}

	_isConnectedMouse = mouseCount > 0;

	return mouseCount;
}

#endif