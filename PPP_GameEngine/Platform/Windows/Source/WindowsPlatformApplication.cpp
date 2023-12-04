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
#include "../../Windows/Include/WindowsWindowsMessageHandler.hpp"
#include "../../Windows/Include/WindowsWindow.hpp"
#include "../../Windows/Include/WindowsPlatformCommand.hpp"
#include "../../Core/Include/CoreCommonState.hpp"
#include "../../Windows/Include/WindowsError.hpp"
#include <cassert>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace platform;
using namespace platform::windows;
using namespace gu;

namespace
{
	using GetDPIForMonitorProcedure = HRESULT(WINAPI*)(HMONITOR monitor, int32 dpiType, uint32* dpiX, uint32* dpiY);
	GetDPIForMonitorProcedure GetDPIForMonitor = nullptr;

	HMODULE SHCoreDLL = nullptr;
}

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

	/*-----------------------------------------------------------------
					  ��DPI���ݒ�\���ǂ���
	--------------------------------------------------------------------*/
	_enableHighDPIMode = SetHighDPIMode();
}

PlatformApplication::~PlatformApplication()
{
	if (SHCoreDLL) { ::FreeLibrary(SHCoreDLL); }
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
*  @fn        gu::SharedPointer<core::CoreWindow> PlatformApplication::MakeWindow()
*
*  @brief     �V�K�̃E�B���h�E�C���X�^���X���쐬���܂�. �����ł̓Z�b�g�A�b�v�͍s���܂���
*
*  @param[in] void
*
*  @return    gu::SharedPointer<core::CoreWindow>
*****************************************************************************/
SharedPointer<core::CoreWindow> PlatformApplication::MakeWindow()
{
	return StaticPointerCast<core::CoreWindow>(MakeShared<windows::CoreWindow>());
}

/****************************************************************************
*                     MakeCommand
*************************************************************************//**
*  @fn        gu::SharedPointer<core::PlatformCommand> PlatformApplication::MakeCommand()
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

/****************************************************************************
*                     SetUpWindow
*************************************************************************//**
*  @fn        void PlatformApplication::SetUpWindow(const gu::SharedPointer<core::CoreWindow>& window, const core::CoreWindowDesc& desc)
*
*  @brief     �w��̃E�B���h�E�����ۂɍ쐬����܂ōs���܂�. �Z�b�g�A�b�v���s���܂�
*
*  @param[in] const gu::SharedPointer<core::CoreWindow>& window, ����̃E�B���h�E
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

#pragma region Monitor
/****************************************************************************
*                     GetMonitorDPI
*************************************************************************//**
*  @fn        gu::int32 PlatformApplication::GetMonitorDPI(const core::MonitorInfo& monitorInfo) const
*
*  @brief     ���j�^�[��DPI���擾���܂�
*             https://zenn.dev/tenka/articles/windows_display_monitor_dpi
*
*  @param[in] const core::MonitorInfo
*
*  @return    gu::int32 DPI
*****************************************************************************/
gu::int32 PlatformApplication::GetMonitorDPI(const core::MonitorInfo& monitorInfo) const
{	
	if (!EnableHighDPIAwareness()) { return USER_DEFAULT_SCREEN_DPI; }

	int32 displayDPI = USER_DEFAULT_SCREEN_DPI;

	if (GetDPIForMonitor)
	{
		const RECT monitorDimenstion =
		{
			.left   = monitorInfo.ActualRectangle.Left,
			.top    = monitorInfo.ActualRectangle.Top,
			.right  = monitorInfo.ActualRectangle.Right,
			.bottom = monitorInfo.ActualRectangle.Bottom
		};
		
		uint32 dpiX = 0, dpiY = 0;
		HMONITOR monitor = MonitorFromRect(&monitorDimenstion, MONITOR_DEFAULTTONEAREST);
		if(monitor && SUCCEEDED(GetDPIForMonitor(monitor, 0, &dpiX, &dpiY)))
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
*************************************************************************//**
*  @fn        void CoreApplication::SetHighDPIMode()
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
		GetDPIForMonitor            = (GetDPIForMonitorProcedure)      ::GetProcAddress(SHCoreDLL, "GetDpiForMonitor");

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
*************************************************************************//**
*  @fn        float CoreApplication::GetDPIScaleFactorAtPixelPoint(const float x, const float y) const
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

	if (GetDPIForMonitor)
	{
		POINT    position = { static_cast<LONG>(x), static_cast<LONG>(y) };
		HMONITOR monitor  = MonitorFromPoint(position, MONITOR_DEFAULTTONEAREST);

		uint32 dpiX = 0, dpiY = 0;
		if (monitor && SUCCEEDED(GetDPIForMonitor(monitor, 0, &dpiX, &dpiY)))
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
#pragma endregion Monitor
#endif