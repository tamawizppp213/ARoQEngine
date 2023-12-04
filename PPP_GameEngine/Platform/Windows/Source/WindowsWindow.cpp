//////////////////////////////////////////////////////////////////////////////////
//              @file   Application.cpp
///             @brief  Main Window
///             @author Toide Yutaro
///             @date   2022_04_13
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Windows/Include/WindowsWindow.hpp"
#if PLATFORM_OS_WINDOWS
#include "../../Windows/Include/WindowsPlatformApplication.hpp"
#include "../../Windows/Include/WindowsError.hpp"
#include "GameUtility/Base/Include/GUAssert.hpp"
#include "GameUtility/Memory/Include/GUMemory.hpp"
#include <stdexcept>
#include <dwmapi.h>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#pragma comment(lib, "Dwmapi.lib")

using namespace platform;
using namespace platform::windows;
using namespace gu;

//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
CoreWindow::CoreWindow() : core::CoreWindow()
{

}

CoreWindow::~CoreWindow()
{

}
#pragma endregion Constructor and Destructor
#pragma region Main Function
/****************************************************************************
*                     Create
*************************************************************************//**
*  @fn        void CoreWindow::Create(const SharedPointer<core::PlatformApplication>& application, const core::CoreWindowDesc& desc, const gu::SharedPointer<core::CoreWindow>& parentWindow)
*
*  @brief     �E�B���h�E���쐬���܂�
*
*  @param[in] const SharedPointer<core::PlatformApplication>& application
*  @param[in] const core::CoreWindowDesc& desc
*  @param[in] const gu::SharedPointer<core::CoreWindow>& parentWindow : �e�E�B���h�E
*
*  @return    bool
*****************************************************************************/
void CoreWindow::Create(const SharedPointer<core::PlatformApplication>& application, const core::CoreWindowDesc& desc, const gu::SharedPointer<core::CoreWindow>& parentWindow)
{
	/*-----------------------------------------------------------------
					   Prepare 
	--------------------------------------------------------------------*/
	Checkf(application, "application is nullptr");

	const auto windowsApplication = StaticPointerCast<windows::PlatformApplication>(application);

	/*-----------------------------------------------------------------
				desc�̓o�^
	--------------------------------------------------------------------*/
	_windowDesc = desc;

	/*-----------------------------------------------------------------
		               Window Settings
	--------------------------------------------------------------------*/
	int32 windowWidth   = static_cast<int32>(desc.DesiredScreenWidth);
	int32 windowHeight  = static_cast<int32>(desc.DesiredScreenHeight);
	int32 windowCenterX = static_cast<int32>(desc.DesiredScreenPositionX);
	int32 windowCenterY = static_cast<int32>(desc.DesiredScreenPositionY);
	
	// �T�C�Y���莞��DPI scale���l������
	_dpiScaleFactor = windowsApplication->GetDPIScaleFactorAtPixelPoint(static_cast<float>(windowCenterX), static_cast<float>(windowCenterY));

	// https://learn.microsoft.com/ja-jp/windows/win32/winmsg/extended-window-styles
	DWORD windowExtensionStyle = 0;
	DWORD windowDefaultStyle   = 0;
	if (desc.HasWindowOSWindowBorder)
	{
		// OS window border set up
		// �E�B���h�E���\������Ă���Ƃ��Ƀ^�X�N�o�[�ɍŏ�ʂ̃E�B���h�E�������I�ɔz�u����
		windowExtensionStyle = WS_EX_APPWINDOW;

		// �^�C�g���Ƙg�t���̃E�B���h�E + ���j���[�{�b�N�X + �^�C�g���o�[
		windowDefaultStyle   = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
		
		if (desc.IsVanillaWindow)
		{
			if (desc.SupportMaximize) { windowDefaultStyle |= WS_MAXIMIZEBOX; } // �ő剻�{�^�������E�B���h�E
			if (desc.SupportMinimize) { windowDefaultStyle |= WS_MINIMIZEBOX; } // �ŏ����{�^�������E�B���h�E
			if (desc.SupportResize  ) { windowDefaultStyle |= WS_THICKFRAME;  } // �T�C�Y�ύX���E�����E�B���h�E���쐬����
			else                      { windowDefaultStyle |= WS_BORDER;      } // ���E�������E�B���h�E���쐬����
		}
		else
		{
			// ��̃E�B���h�E + ���E�������E�B���h�E���쐬
			// UE5���Q�l�ɂ��Ă��邪, vanilla window�ł���Ȃ炱�����̕���I������̂ł�?
			windowDefaultStyle |= WS_POPUP | WS_BORDER;
		}

		/*-----------------------------------------------------------------
				Border�ɂ��E�B���h�E�T�C�Y�̍Ē���
		--------------------------------------------------------------------*/
		RECT windowRect = { 0, 0, 0, 0 };
		::AdjustWindowRectEx(&windowRect, windowDefaultStyle, false, windowExtensionStyle);

		windowCenterX += windowRect.left;
		windowCenterY += windowRect.top;
		windowWidth   += windowRect.right - windowRect.left;
		windowHeight  += windowRect.bottom - windowRect.top;
	}
	else
	{
		// �E�B���h�E�ɂ̓G�b�W���オ�����r�������� 
		windowExtensionStyle = WS_EX_WINDOWEDGE; 

		// �������E�B���h�E���o����悤�ɂ���
		if (_windowDesc.TransparencyType == core::WindowTransparency::PerWindow)
		{
			windowExtensionStyle |= WS_EX_LAYERED;
		}
		if (_windowDesc.TransparencyType == core::WindowTransparency::PerPixel)
		{
			windowExtensionStyle |= WS_EX_COMPOSITED;
		}

		// ��E�B���h�E + �e�E�B���h�E���ŕ`�悳��鎞�Ɏq�E�B���h�E����L����̈�����O
		windowDefaultStyle   = WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS; 
		
		// �^�X�N�o�[�Ή� 
		if (desc.AppearsInTaskbar)
		{
			windowExtensionStyle |= WS_EX_APPWINDOW;  // �E�B���h�E���\������Ă��鎞��, �^�X�N�o�[�ɍŏ�ʂ̃E�B���h�E�������I�ɔz�u����
		}
		else
		{ 
			windowExtensionStyle |= WS_EX_TOOLWINDOW; // �c�[���o�[�Ƃ��Ďg�p
		}

		// ��������������Ƃ���true�ɂȂ�܂�. 
		if (desc.IsTopmostWindow)
		{
			windowExtensionStyle |= WS_EX_TOPMOST;
		}

		// non Interactive�ȃE�B���h�E���쐬�������Ƃ�
		if (!desc.AcceptInput)
		{
			windowExtensionStyle |= WS_EX_TRANSPARENT;
		}
	}

	/*-----------------------------------------------------------------
	                	Create window
	--------------------------------------------------------------------*/
	_hwnd = CreateWindowEx
	(
		windowExtensionStyle, // Window style
		core::PlatformApplication::APPLICATION_NAME, // Name to register with Windows
		desc.Title,
		windowDefaultStyle,
		windowCenterX, // Center position_X
		windowCenterY, // Center position_Y
		windowWidth,   // window width
		windowHeight,  // window height
		parentWindow ? static_cast<HWND>(parentWindow->GetWindowHandle()) : nullptr, // parent window
		nullptr      , // menu handler 
		windowsApplication->GetWindowsInstanceHandle(), // instance handler 
		windowsApplication.Get()  // additional expansion area
	);

	if (_hwnd == NULL)
	{
		ErrorLogger::Log(NULL);
		return;
	}

	/*-----------------------------------------------------------------
				Enable to touch the ui button in the window  
	--------------------------------------------------------------------*/
	if (!RegisterTouchWindow(_hwnd, NULL))
	{
		ErrorLogger::Log(_hwnd);
	}

	/*-----------------------------------------------------------------
				�����I��touch Feedback��off�ɂ���
	--------------------------------------------------------------------*/
	if (!desc.UseTouchFeedback)
	{
		DisableTouchFeedback();
	}

	/*-----------------------------------------------------------------
				�œK���p��virtual window size���擾����
	--------------------------------------------------------------------*/
	const auto clientWindowWidth  = static_cast<int32>(desc.DesiredScreenWidth);
	const auto clientWindowHeight = static_cast<int32>(desc.DesiredScreenHeight);
	_virtualWindowWidth  = clientWindowWidth;
	_virtualWindowHeight = clientWindowHeight;

	Resize(desc.DesiredScreenPositionX, desc.DesiredScreenPositionY, clientWindowWidth, clientWindowHeight);

	/*-----------------------------------------------------------------
				���ߓx�̐ݒ�
	--------------------------------------------------------------------*/
	if (_windowDesc.TransparencyType == core::WindowTransparency::PerWindow)
	{
		SetOpacity(_windowDesc.Opacity);
	}

	/*-----------------------------------------------------------------
				��N���C�A���g�̈�̕`�斳����
	--------------------------------------------------------------------*/
	// os�E�B���h�E�̋��E����\�����Ȃ��ꍇ�́ADWM�����_�����O�Ɣ�N���C�A���g�̈�̕`��𖳌��ɂ��� 
	// ����ɂ��A�W���E�B���h�E�̃t���[�����`�悳��邱�Ƃ��Ȃ��Ȃ�
	if (!_windowDesc.HasWindowOSWindowBorder)
	{
		// ��N���C�A���g�̈�̕`��𖳌��ɂ���
		const auto renderingPolicy = DWMNCRP_DISABLED;
		Confirm(SUCCEEDED(DwmSetWindowAttribute(_hwnd, DWMWA_NCRENDERING_POLICY, &renderingPolicy, sizeof(renderingPolicy))));

		// ��N���C�A���g�̈�Ń����_�����O���ꂽ�R���e���c��DWM�ŕ`�悳�ꂽ�t���[���ɕ\�������悤�ɂ���B
		const auto enableNCPaint = false;
		Confirm(SUCCEEDED(DwmSetWindowAttribute(_hwnd, DWMWA_ALLOW_NCPAINT, &enableNCPaint, sizeof(enableNCPaint))));

		// ��قǏC��
		if (_windowDesc.TransparencyType == core::WindowTransparency::PerPixel)
		{
			MARGINS margins = { -1 };
			Confirm(SUCCEEDED(::DwmExtendFrameIntoClientArea(_hwnd, &margins)));
		}
	}

	/*-----------------------------------------------------------------
	    �񐳋K�E�B���h�E�܂���os�E�B���h�E�̋��E����\������E�B���h�E�̗̈���Ȃ���
	--------------------------------------------------------------------*/
	if (_windowDesc.IsVanillaWindow && !_windowDesc.HasWindowOSWindowBorder)
	{
		windowDefaultStyle |= WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
		if (desc.SupportMaximize) { windowDefaultStyle |= WS_MAXIMIZEBOX; } // �ő剻�{�^�������E�B���h�E
		if (desc.SupportMinimize) { windowDefaultStyle |= WS_MINIMIZEBOX; } // �ŏ����{�^�������E�B���h�E
		if (desc.SupportResize  ) { windowDefaultStyle |= WS_THICKFRAME;  } // �T�C�Y�ύX���E�����E�B���h�E���쐬����

		Confirm(SetWindowLong(_hwnd, GWL_STYLE, windowDefaultStyle));

		uint32 setWindowPositionFlags = SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED;
		if (_windowDesc.ActivationPolicy == core::ActivationPolicy::Never)
		{
			setWindowPositionFlags |= SWP_NOACTIVATE;
		}

		::SetWindowPos(_hwnd, nullptr, 0, 0, 0, 0, setWindowPositionFlags);

		//�Q�[���ȊO�̃E�B���h�E�ł́A�f�t�H���g�̃V�X�e�����j���[������郁�j���[���폜����
		//����ăA�v���P�[�V�����A�C�R�����_�u���N���b�N����win32�A�v������Ă��܂����Ƃ�h����
		::DeleteMenu(GetSystemMenu(_hwnd, false), SC_CLOSE, MF_BYCOMMAND);

		AdjustWindowRegion(clientWindowWidth, clientWindowHeight);
	}
	else if(_windowDesc.HasWindowOSWindowBorder && !_windowDesc.HasCloseButton)
	{
		// �����Ă��Ȃ����
		EnableMenuItem(GetSystemMenu(_hwnd, false), SC_CLOSE, MF_GRAYED);
	}

	// ���̂��Əo��
}

/****************************************************************************
*                     Show
*************************************************************************//**
*  @fn        bool CoreWindow::Show()
*
*  @brief     �E�B���h�E��\�����܂�
*
*  @param[in] void
*
*  @return    bool
*****************************************************************************/
bool CoreWindow::Show()
{
	if (_isVisible) { return true; }
	
	_isVisible = true;
	
	/*---------------------------------------------------------------
					  �A�N�e�B�u�����s����
	-----------------------------------------------------------------*/
	bool shouldActiveWindow =  _windowDesc.ActivationPolicy == core::ActivationPolicy::Always;
	if (_isFirstTimeVisible && _windowDesc.ActivationPolicy == core::ActivationPolicy::FirstOnlyShow)
	{
		shouldActiveWindow = true;
	}

	/*---------------------------------------------------------------
					  �\���R�}���h�̑I��
	-----------------------------------------------------------------*/
	int showWindowCommand = shouldActiveWindow ? SW_SHOW : SW_SHOWNOACTIVATE;
	if (_isFirstTimeVisible)
	{
		_isFirstTimeVisible = false; // �ő剻��ŏ����̎��͏��߂ĕ\�������Ƃ͌����Ȃ�
		if (_initiallyMinimized)
		{
			showWindowCommand = shouldActiveWindow ? SW_MINIMIZE : SW_SHOWMINNOACTIVE;
		}
		else if (_initiallyMaximized)
		{
			showWindowCommand = shouldActiveWindow ? SW_SHOWMAXIMIZED : SW_MAXIMIZE; // �A�N�e�B�u�ł͂���
		}
	}

	//// SW_SHOW : �E�B���h�E���A�N�e�B�u����, ���݂̃T�C�Y�ƈʒu�ŕ\������
	::ShowWindow(_hwnd, showWindowCommand);
	return ErrorLogger::Succeed();
}

/****************************************************************************
*                     Hide
*************************************************************************//**
*  @fn        bool CoreWindow::Hide()
*
*  @brief     Visible�ł������ꍇ�ɃE�B���h�E���B���܂�
*
*  @param[in] void
*
*  @return    bool
*****************************************************************************/
bool CoreWindow::Hide()
{
	if (_isVisible)
	{
		_isVisible = false;
		::ShowWindow(_hwnd, SW_HIDE);
		return ErrorLogger::Succeed();
	}
	return true;
}

/****************************************************************************
*                     Minimize
*************************************************************************//**
*  @fn        bool CoreWindow::Minimize()
*
*  @brief     �E�B���h�E���ŏ������܂�.
*
*  @param[in] void
*
*  @return    bool
*****************************************************************************/
bool CoreWindow::Minimize()
{
	if (_isFirstTimeVisible)
	{
		_initiallyMaximized = false;
		_initiallyMinimized = true;
		return Show();
	}
	else
	{
		::ShowWindow(_hwnd, SW_MINIMIZE);
		return ErrorLogger::Succeed();
	}
}

/****************************************************************************
*                     Maximize
*************************************************************************//**
*  @fn        bool CoreWindow::Maximize()
*
*  @brief     �E�B���h�E���ő剻���܂�
*
*  @param[in] void
*
*  @return    bool
*****************************************************************************/
bool CoreWindow::Maximize()
{
	if (_isFirstTimeVisible)
	{
		_initiallyMaximized = true;
		_initiallyMinimized = false;
		return Show();
	}
	else
	{
		// �E�B���h�E���A�N�e�B�u�ɂ��ĕ\������
		::ShowWindow(_hwnd, SW_MAXIMIZE);
		return ErrorLogger::Succeed();
	}
}

/****************************************************************************
*                     ReStore
*************************************************************************//**
*  @fn        bool CoreWindow::ReStore()
*
*  @brief     �E�B���h�E���A�N�e�B�u�ɂ��ĕ\������B�ŏ����E�ő剻����Ă���ꍇ�͌��̃T�C�Y�ƈʒu�ɕ��������.
*
*  @param[in] void
*
*  @return    bool
*****************************************************************************/
bool CoreWindow::ReStore()
{
	// �E�B���h�E���A�N�e�B�u�ɂ��ĕ\������B�ŏ����E�ő剻����Ă���ꍇ�͌��̃T�C�Y�ƈʒu�ɕ��������.
	if (_isFirstTimeVisible)
	{
		_initiallyMaximized = false;
		_initiallyMinimized = false;
		return true;
	}
	else
	{
		::ShowWindow(_hwnd, SW_RESTORE);
		return ErrorLogger::Succeed();
	}
}

/****************************************************************************
*                     Destroy
*************************************************************************//**
*  @fn        bool CoreWindow::Destroy()
*
*  @brief     �E�B���h�E���A�N�e�B�u�ɂ��ĕ\������B�ŏ����E�ő剻����Ă���ꍇ�͌��̃T�C�Y�ƈʒu�ɕ��������.
*
*  @param[in] void
*
*  @return    bool
*****************************************************************************/
bool CoreWindow::Destroy()
{
	return ::DestroyWindow(_hwnd);
}

/****************************************************************************
*                     Resize
*************************************************************************//**
*  @fn        void CoreWindow::Resize(const gu::int32 x, const gu::int32 y, const gu::int32 width, const gu::int32 height)
*
*  @brief     �E�B���h�E�̃T�C�Y��ύX���܂�
*
*  @param[in] const int32 window center position x
*  @param[in] const int32 window center position y
*  @param[in] const int32 window width
*  @param[in] const int32 window height
*
*  @return    bool
*****************************************************************************/
void CoreWindow::Resize(const gu::int32 x, const gu::int32 y, const gu::int32 width, const gu::int32 height)
{
	/*---------------------------------------------------------------
					  WindowInfo�̎擾
	-----------------------------------------------------------------*/
	// ����������ĂȂ��̂�, �������[���ŏ�����
	WINDOWINFO windowInfo = {};
	Memory::Zero(&windowInfo, sizeof(windowInfo));

	// window info�̎擾
	windowInfo.cbSize = sizeof(windowInfo);
	::GetWindowInfo(_hwnd, &windowInfo);

	// aspect ratio�̎擾
	_aspectRatio = (float)width / (float)height;

	/*---------------------------------------------------------------
					  ���O����
	-----------------------------------------------------------------*/
	int32 newX      = x;
	int32 newY      = y;
	int32 newWidth  = width;
	int32 newHeight = height;

	/*---------------------------------------------------------------
					 ���E���������ꍇ�̃E�B���h�E�T�C�Y����
	-----------------------------------------------------------------*/
	if (_windowDesc.HasWindowOSWindowBorder)
	{
		RECT borderRectangle = { 0,0,0,0 };
		::AdjustWindowRectEx(&borderRectangle, windowInfo.dwStyle, false, windowInfo.dwExStyle);

		newX += borderRectangle.left;
		newY += borderRectangle.top;
		newWidth  += borderRectangle.right - borderRectangle.left;
		newHeight += borderRectangle.bottom - borderRectangle.top;
	}

	/*---------------------------------------------------------------
					 �E�B���h�E�T�C�Y�̎擾
	-----------------------------------------------------------------*/
	// �E�B���h�E�T�C�Y���p�ɂɕύX�����ꍇ�́A�E�B���h�E��傫�����邾���ŁA�����ď��������Ȃ��B
	const auto virtualSizeChanged = newWidth != _virtualWindowWidth || newHeight != _virtualWindowHeight;
	_virtualWindowWidth  = newWidth;
	_virtualWindowHeight = newHeight;

	/*---------------------------------------------------------------
				   �p�ɂɃT�C�Y�ύX��������ꍇ�̍œK��
	-----------------------------------------------------------------*/
	// SizeWillChangeOften���ݒ肳��Ă���ꍇ�A���ƍ����̍ő�E�B���h�E�T�C�Y��ݒ肵�܂��B
    // �v�����ꂽ���ۂ̃E�B���h�E �T�C�Y���������Ȃ��Ă��A�m�ۂ������܂��B ����́A�E�B���h�E�Ɗ֘A���� GPU �o�b�t�@�̃T�C�Y����ɕύX���邱�Ƃ�����邽�߂ł��B
    // �v���b�g�t�H�[���ɂ���Ă͔��ɒx���Ȃ�܂��B
	if (_windowDesc.SizeWillChangeOften)
	{
		const auto oldWindowRectangle = windowInfo.rcWindow;
		const auto oldWidth           = oldWindowRectangle.right - oldWindowRectangle.left;
		const auto oldHeight          = oldWindowRectangle.bottom - oldWindowRectangle.top;

		const auto minRetainedWidth   = _windowDesc.ExpectedMaxWidth  < 0.0f ? _windowDesc.ExpectedMaxWidth : oldWidth;
		const auto minRetainedHeight = _windowDesc.ExpectedMaxHeight < 0.0f ? _windowDesc.ExpectedMaxHeight : oldHeight;

		newWidth  = max(newWidth , min(oldWidth, minRetainedWidth));
		newHeight = max(newHeight, min(oldHeight, minRetainedHeight));
	}

	if (IsMaximized()) { ReStore(); }

	/*---------------------------------------------------------------
				    �E�B���h�E�T�C�Y�̕ύX
	-----------------------------------------------------------------*/
	// �t���X�N���[�����[�h�ł�SWP_NOSENDCHANGING���g�p���AWindows���E�B���h�E�T�C�Y�����݂̉𑜓x�ɐ�������̂�h���܂��B
	::SetWindowPos(_hwnd, nullptr, newX, newY, newWidth, newHeight,
		SWP_NOZORDER | SWP_NOACTIVATE | ((_windowDesc.WindowMode == core::WindowMode::FullScreen) ? SWP_NOSENDCHANGING : 0));

	const auto adjustedSizeChange = _windowDesc.SizeWillChangeOften || virtualSizeChanged;
	const auto adjustedCorners    = _windowDesc.EngineWindowType != core::EngineWindowType::Menu && _windowDesc.CornerRadius > 0;

	// �܂�������
	if (!_windowDesc.HasWindowOSWindowBorder && (adjustedSizeChange || adjustedCorners))
	{
		AdjustWindowRegion(_virtualWindowWidth, _virtualWindowHeight);
	}
}
/****************************************************************************
*                     DisableTouchFeedback
*************************************************************************//**
*  @fn        void CoreWindow::DisableTouchFeedback()
*
*  @brief     �^�b�`���͂𖳌������܂�. 
*
*  @param[in] void
*
*  @return    void
*****************************************************************************/
void CoreWindow::DisableTouchFeedback()
{
#if WINVER < 0x0602
	const auto userDLL = GetModuleHandle(L"user32.dll");

	/*---------------------------------------------------------------
					User32DLL���ǂݍ��܂�Ă���
	-----------------------------------------------------------------*/
	if (!userDLL) { return; }

	/*---------------------------------------------------------------
					���������邽�߂̃t�B�[�h�o�b�N�̎��
	-----------------------------------------------------------------*/
	enum FeedBackType
	{
		FEEDBACK_TOUCH_CONTACTVISUALIZATION = 1,
		FEEDBACK_PEN_BARRELVISUALIZATION = 2,
		FEEDBACK_PEN_TAP = 3,
		FEEDBACK_PEN_DOUBLETAP = 4,
		FEEDBACK_PEN_PRESSANDHOLD = 5,
		FEEDBACK_PEN_RIGHTTAP = 6,
		FEEDBACK_TOUCH_TAP = 7,
		FEEDBACK_TOUCH_DOUBLETAP = 8,
		FEEDBACK_TOUCH_PRESSANDHOLD = 9,
		FEEDBACK_TOUCH_RIGHTTAP = 10,
		FEEDBACK_GESTURE_PRESSANDTAP = 11,
		FEEDBACK_MAX = 0xFFFFFFFF
	};

	/*---------------------------------------------------------------
					�֐��|�C���^�̐ݒ�
	-----------------------------------------------------------------*/
	using SetWindowFeedbackSettingProcedure = BOOL(WINAPI*)
		(_In_ HWND hwnd,
			_In_ FeedBackType feedback,
			_In_ DWORD flags,
			_In_ UINT32 size,
			_In_reads_bytes_opt_(size) CONST VOID* configuration);

	auto SetWindowFeedbackSetting = (SetWindowFeedbackSettingProcedure)::GetProcAddress(userDLL, "SetWindowFeedbackSetting");

	/*---------------------------------------------------------------
					����������t�B�[�h�o�b�N�̎���
	-----------------------------------------------------------------*/
	if (SetWindowFeedbackSetting)
	{
		BOOL enabled = 0;
		SetWindowFeedbackSetting(_hwnd, FEEDBACK_TOUCH_CONTACTVISUALIZATION, 0, sizeof(enabled), &enabled);
		SetWindowFeedbackSetting(_hwnd, FEEDBACK_TOUCH_TAP, 0         , sizeof(enabled), &enabled);
		SetWindowFeedbackSetting(_hwnd, FEEDBACK_TOUCH_PRESSANDHOLD, 0, sizeof(enabled), &enabled);
	}
#else
	BOOL enabled = 0;
	SetWindowFeedbackSetting(_hwnd, FEEDBACK_TOUCH_CONTACTVISUALIZATION, 0, sizeof(enabled), &enabled);
	SetWindowFeedbackSetting(_hwnd, FEEDBACK_TOUCH_TAP, 0         , sizeof(enabled), &enabled);
	SetWindowFeedbackSetting(_hwnd, FEEDBACK_TOUCH_PRESSANDHOLD, 0, sizeof(enabled), &enabled);
#endif
}

/****************************************************************************
*                     AdjustWindowRegion
*************************************************************************//**
*  @fn        void CoreWindow::AdjustWindowRegion(const int32 width, const int32 height)
*
*  @brief     �E�B���h�E�̈�̒���
*
*  @param[in] const int32 width
*  @param[in] const int32 height
*
*  @return    void
*****************************************************************************/
void CoreWindow::AdjustWindowRegion(const int32 width, const int32 height)
{
	_regionWidth  = width;
	_regionHeight = height;

	HRGN regionHandle = CreateWindowRegionObject(true);
	Confirm(SetWindowRgn(_hwnd, regionHandle, false))
}

/****************************************************************************
*                     CreateWindowRegionObject
*************************************************************************//**
*  @fn        HRGN CoreWindow::CreateWindowRegionObject(const bool useBorderWhenMaximized) const
*
*  @brief     Region�I�u�W�F�N�g�̍쐬
*
*  @param[in] const bool useBorderWhenMaximized�͍ő剻���Ă���Ƃ��ɂ����E�������g����.
*
*  @return    HRGN 
*****************************************************************************/
HRGN CoreWindow::CreateWindowRegionObject(const bool useBorderWhenMaximized) const
{
	HRGN region = nullptr;

	/*---------------------------------------------------------------
				window region���ݒ肳��Ă���ꍇ�̑Ή�
	-----------------------------------------------------------------*/
	if (_regionWidth != -1 && _regionHeight != -1)
	{
		const bool isBorderlessGameWindow = _windowDesc.EngineWindowType == core::EngineWindowType::Game 
			                             && !_windowDesc.HasWindowOSWindowBorder;
		/*---------------------------------------------------------------
					�E�B���h�E���ő剻����Ă��鎞
		-----------------------------------------------------------------*/
		if (IsMaximized())
		{
			if (isBorderlessGameWindow)
			{
				// Windows�́A�E�B���h�E�쐬����cxWindowBorders�T�C�Y���L���b�V������. 
				// �{�[�_�[���폜���ꂽ��A�T�C�Y���ύX���ꂽ�肵�Ă��AWindows�͗̈��]��������A
				// �E�B���h�E�̃T�C�Y�𒲐������肷��Ƃ��ɁA���̒l���g�������܂��B
				// �ő剻����ƁA�E�B���h�E�̈ʒu�͉�ʂ̌��_���� (-cxWindowBorders,-cxWindowBorders) �����I�t�Z�b�g����܂��B
				// �ő剻���ꂽ��ʗ̈���̗̈悾����\���������̂ŁA����ƉE���� cxWindowBorders �����I�t�Z�b�g���܂��B
				WINDOWINFO windowInfo;
				Memory::Zero(&windowInfo, sizeof(windowInfo));
				windowInfo.cbSize = sizeof(windowInfo);
				::GetWindowInfo(_hwnd, &windowInfo);

				const auto windowBorderSize = useBorderWhenMaximized ? windowInfo.cxWindowBorders : 0;
				region = CreateRectRgn(windowBorderSize, windowBorderSize, _regionWidth + windowBorderSize, _regionHeight + windowBorderSize);
			}
			else
			{
				const auto windowBorderSize = useBorderWhenMaximized ? GetWindowBorderSize() : 0;
				region = CreateRectRgn(windowBorderSize, windowBorderSize, _regionWidth - windowBorderSize, _regionHeight - windowBorderSize);
			}
		}
		/*---------------------------------------------------------------
					�E�B���h�E���펞
		-----------------------------------------------------------------*/
		else
		{
			const bool useCornerRadius = _windowDesc.WindowMode == core::WindowMode::Windowed && !isBorderlessGameWindow &&
				                   _windowDesc.TransparencyType != core::WindowTransparency::PerPixel
				                   && _windowDesc.CornerRadius > 0;

			if (useCornerRadius)
			{
				region = CreateRoundRectRgn(0, 0, _regionWidth + 1, _regionHeight + 1, _windowDesc.CornerRadius, _windowDesc.CornerRadius);
			}
			else
			{
				region = CreateRectRgn(0, 0, _regionWidth, _regionHeight);
			}
		}
	}
	/*---------------------------------------------------------------
				window region�����ݒ�̏ꍇ�̑Ή�
	-----------------------------------------------------------------*/
	else
	{
		RECT windowRectangle = {};
		GetWindowRect(_hwnd, &windowRectangle);
		region = CreateRectRgn(0, 0, windowRectangle.right - windowRectangle.left, windowRectangle.bottom - windowRectangle.top);
	}

	return region;
}

/****************************************************************************
*                     GetFullScreenInfo
*************************************************************************//**
*  @fn        bool CoreWindow::GetFullScreenInfo(gu::int32& x, gu::int32& y, gu::int32& width, gu::int32& height) const
*
*  @brief     ���ݎw�肵���E�B���h�E�ɍł��������Ă��郂�j�^�[�̃T�C�Y���擾���܂�. 
*
*  @return    bool 
*****************************************************************************/
bool CoreWindow::GetFullScreenInfo(gu::int32& x, gu::int32& y, gu::int32& width, gu::int32& height) const
{
	const bool isFullScreen = _windowDesc.WindowMode == core::WindowMode::FullScreen;

	/*---------------------------------------------------------------
				Monitor�̏����擾����
	-----------------------------------------------------------------*/
	// �w�肵���E�B���h�E�̊T������l�p�`�Ƃ̌����̈悪�ł��傫���f�B�X�v���C���j�^�[�ւ̃n���h�����擾����
	// isFullScreen�ł͌������Ȃ��ꍇ�̖߂�l��ݒ肵�Ă���.
	HMONITOR monitor = MonitorFromWindow(_hwnd, isFullScreen ? MONITOR_DEFAULTTOPRIMARY : MONITOR_DEFAULTTONEAREST);

	// Monitor info���擾����
	MONITORINFO monitorInfo = {};

	Memory::Zero(&monitorInfo, sizeof(monitorInfo));
	monitorInfo.cbSize = sizeof(MONITORINFO);
	
	GetMonitorInfo(monitor, &monitorInfo);

	/*---------------------------------------------------------------
				�e�����ɐݒ�𔽉f����
	-----------------------------------------------------------------*/
	x      = monitorInfo.rcMonitor.left;
	y      = monitorInfo.rcMonitor.top;
	width  = monitorInfo.rcMonitor.right - x;
	height = monitorInfo.rcMonitor.bottom - y;

	return true;
}

/****************************************************************************
*                     BringToFront
*************************************************************************//**
*  @fn        void BringToFront(const bool forceFront)
*
*  @brief     ���ɐݒ肳�ꂽ�E�B���h�E���őO�ʂɕ\������
* 
*  @param[in] bool force front 
�@�@�@�@�@�@�@�@�@ ���̃E�B���h�E����t�H�[�J�X��D�����ƂɂȂ��Ă��A�E�B���h�E�������I��Z���̐擪�ɂ���
		     ����̓Q�[���E�E�B���h�E�̂悤�ɁA�����I�ɍőO�ʂɈړ������Ȃ��ƃ}�E�X�E�L���v�`����}�E�X�E���b�N���N���邪�A
			 �E�B���h�E���\������Ȃ��悤�ȃE�B���h�E�ɂ̂ݗL���ł���B
*
*  @return    void
*****************************************************************************/
void CoreWindow::BringToFront(const bool forceFront)
{
	if (_windowDesc.IsVanillaWindow)
	{
		// �A�C�R����ԂɂȂ��Ă���Ε\�����s��, �����łȂ���΃A�N�e�B�u�����s��
		if (::IsIconic(_hwnd))
		{
			::ShowWindow(_hwnd, SW_RESTORE);
		}
		else
		{
			::SetActiveWindow(_hwnd);
		}
	}
	else
	{
		//http://www.freak.ne.jp/HEPOPO/lib/prog_win32/win32009.html
		// �E�B���h�E��Z�I�[�_�[�̏㕔�ɔz�u����
		// TOP�̕��͍őO�ʃE�B���h�E�̈���ł͂��邪, ��{�I�Ɉ�ԑO�ł�. TOPMOST�͊��S�ɍőO�ʃE�B���h�E�ƂȂ�
		HWND   hwndInsertAfter = _windowDesc.IsTopmostWindow ? HWND_TOPMOST : HWND_TOP;
		
		// ���݂̈ʒu + �T�C�Y + zOrder��ێ�����. (zOrder��insertAfter�ȊO)
		uint32 flags = SWP_NOMOVE | SWP_NOSIZE | SWP_NOOWNERZORDER;
		if (!forceFront) { flags |= SWP_NOACTIVATE; }


		::SetWindowPos(_hwnd, hwndInsertAfter, 0, 0, 0, 0, flags);
	}
}

/****************************************************************************
*                     Move
*************************************************************************//**
*  @fn        void CoreWindow::Move(const int32 x, const int32 y)
*
*  @brief     �w�肵��x, y���W�ɃE�B���h�E���ړ������܂�
*
*  @param[in] int32 x
*  @param[in] int32 y
*
*  @return    void
*****************************************************************************/
void CoreWindow::Move(const int32 x, const int32 y)
{
	int32 correctedX = x;
	int32 correctedY = y;

	/*---------------------------------------------------------------
		�@�@���E���������ꍇ�̕␳�����@
	-----------------------------------------------------------------*/
	if (_windowDesc.HasWindowOSWindowBorder)
	{
		const auto windowDefaultStyle   = ::GetWindowLong(_hwnd, GWL_STYLE);
		const auto windowExtensionStyle = ::GetWindowLong(_hwnd, GWL_EXSTYLE);

		// �{�[�_�[�T�C�Y�̎擾
		RECT borderRectangle = { 0,0,0,0 };
		::AdjustWindowRectEx(&borderRectangle, windowDefaultStyle, false, windowExtensionStyle);

		// ���ۂ̃{�[�_�[�T�C�Y�̕␳
		correctedX += borderRectangle.left;
		correctedY += borderRectangle.top;
	}

	/*---------------------------------------------------------------
		�@�@�E�B���h�E�̔z�u
	-----------------------------------------------------------------*/
	::SetWindowPos(_hwnd, nullptr, correctedX, correctedY, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER);
}

/****************************************************************************
*                     SetWindowMode
*************************************************************************//**
*  @fn        void CoreWindow::SetWindowMode(const core::WindowMode windowMode)
*
*  @brief     �E�B���h�E���[�h�̐ݒ�
*
*  @param[in] const core::WindowMode windowMode
*
*  @return    void
*****************************************************************************/
void CoreWindow::SetWindowMode(const core::WindowMode windowMode)
{
	if (_windowDesc.WindowMode == windowMode) { return; }

	/*---------------------------------------------------------------
				�E�B���h�E�̐ݒ�
	-----------------------------------------------------------------*/
	const auto previousWindowMode = _windowDesc.WindowMode;
	_windowDesc.WindowMode        = windowMode;

	auto windowDefaultStyle  = GetWindowLong(_hwnd, GWL_STYLE);
	auto windowedModeStyle   = WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION;
	auto fullScreenModeStyle = WS_POPUP;
	const auto isTrueFullScreen = windowMode == core::WindowMode::FullScreen;

	if (_windowDesc.IsVanillaWindow)
	{
		if (_windowDesc.SupportMaximize) { windowDefaultStyle |= WS_MAXIMIZEBOX; }
		if (_windowDesc.SupportMinimize) { windowDefaultStyle |= WS_MINIMIZEBOX; }
		if (_windowDesc.SupportResize  ) { windowDefaultStyle |= WS_THICKFRAME;  } // �T�C�Y�ύX���E�����E�B���h�E���쐬����
		else                             { windowDefaultStyle |= WS_BORDER;      } // ���E�������E�B���h�E���쐬����
	}
	else
	{
		windowDefaultStyle |= WS_POPUP | WS_BORDER;
	}

	/*---------------------------------------------------------------
				�t���X�N���[��
	-----------------------------------------------------------------*/
	if (windowMode == core::WindowMode::BorderLess || windowMode == core::WindowMode::FullScreen)
	{
		/*---------------------------------------------------------------
				�t���X�N���[���ɂȂ�O�ɃE�B���h�E�̔z�u���L�����Ă���
		-----------------------------------------------------------------*/
		if (previousWindowMode == core::WindowMode::Windowed)
		{
			_previousFullScreenWindowPlacement.length = sizeof(WINDOWPLACEMENT);
			::GetWindowPlacement(_hwnd, &_previousFullScreenWindowPlacement);
		}

		/*---------------------------------------------------------------
				�E�B���h�E�̃X�^�C����ύX����
		-----------------------------------------------------------------*/
		windowDefaultStyle &= ~windowedModeStyle;
		windowDefaultStyle |= fullScreenModeStyle;

		SetWindowLong(_hwnd, GWL_STYLE, windowDefaultStyle);
		::SetWindowPos(_hwnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

		if (windowMode == core::WindowMode::BorderLess)
		{
			// �{�[�_�[���X�̏ꍇ��, �E�B���h�E�𕜌��ł���悤�ɂ��Ă���
			::ShowWindow(_hwnd, SW_RESTORE);
		}

		/*---------------------------------------------------------------
				���j�^�[���̎擾
		-----------------------------------------------------------------*/
		// ���݂̃E�B���h�E�̈ʒu���擾����
		RECT clientRectangle;
		GetClientRect(_hwnd, &clientRectangle);

		// ���j�^�[���̎擾
		HMONITOR    monitor = MonitorFromWindow(_hwnd, isTrueFullScreen? MONITOR_DEFAULTTOPRIMARY : MONITOR_DEFAULTTONEAREST);
		
		MONITORINFO monitorInfo;
		monitorInfo.cbSize = sizeof(MONITORINFO);
		GetMonitorInfo(monitor, &monitorInfo);

		/*---------------------------------------------------------------
				���j�^�[�T�C�Y�̌���
		-----------------------------------------------------------------*/
		const auto monitorWidth       = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
		const auto monitorHeight      = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;
		const auto targetClientWidth  = isTrueFullScreen ? min(monitorWidth, clientRectangle.right - clientRectangle.left) : monitorWidth;
		const auto targetClientHeight = isTrueFullScreen ? min(monitorHeight, clientRectangle.bottom - clientRectangle.top) : monitorHeight;

		/*---------------------------------------------------------------
				�T�C�Y��ύX����
		-----------------------------------------------------------------*/
		Resize(monitorInfo.rcMonitor.left, monitorInfo.rcMonitor.top, targetClientWidth, targetClientHeight);
	}
	/*---------------------------------------------------------------
				Windowed
	-----------------------------------------------------------------*/
	else
	{
		windowDefaultStyle &= ~fullScreenModeStyle;
		windowDefaultStyle |= windowedModeStyle;

		SetWindowLong(_hwnd, GWL_STYLE, windowDefaultStyle);
		::SetWindowPos(_hwnd, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);

		if (_previousFullScreenWindowPlacement.length)
		{
			::SetWindowPlacement(_hwnd, &_previousFullScreenWindowPlacement);
		}

		HICON icon = (HICON)::GetClassLong(_hwnd, GCLP_HICON);
		if (icon != nullptr)
		{
			::SendMessageW(_hwnd, WM_SETICON, ICON_SMALL, (LPARAM)icon);
		}
	}
}

/****************************************************************************
*                     GetWindowBorderSize
*************************************************************************//**
*  @fn        int32 CoreWindow::GetWindowBorderSize() const
*
*  @brief     �E�B���h�E�̋��E�����̃T�C�Y��Ԃ�
*
*  @param[in] void
*
*  @return    int32 border size
*****************************************************************************/
int32 CoreWindow::GetWindowBorderSize() const
{
	/*---------------------------------------------------------------
		�@�@�@�@�{�[�_�[���X�E�B���h�E�ł����0��Ԃ�
	-----------------------------------------------------------------*/
	if ( _windowDesc.EngineWindowType == core::EngineWindowType::Game && 
		!_windowDesc.HasWindowOSWindowBorder)
	{
		return 0;
	}

	/*---------------------------------------------------------------
		�@�@�@�@�{�[�_�[���X�E�B���h�E�ȊO�͋��E��Ԃ�
	-----------------------------------------------------------------*/
	WINDOWINFO windowInfo = {};
	Memory::Zero(&windowInfo, sizeof(windowInfo));
	windowInfo.cbSize = sizeof(windowInfo);
	::GetWindowInfo(_hwnd, &windowInfo);
	
	return windowInfo.cxWindowBorders;
}

/****************************************************************************
*                     GetWindowTitleBarSize
*************************************************************************//**
*  @fn        int32 CoreWindow::GetWindowTitleBarSize() const
*
*  @brief     �E�B���h�E�̃^�C�g���o�[�̃T�C�Y��Ԃ�
*
*  @param[in] void
*
*  @return    int32 size
*****************************************************************************/
int32 CoreWindow::GetWindowTitleBarSize() const
{
	return ::GetSystemMetrics(SM_CYCAPTION);
}
/****************************************************************************
*                     GetRestoredDimensions
*************************************************************************//**
*  @fn        bool CoreWindow::GetRestoredDimensions(gu::int32& x, gu::int32& y, gu::int32& width, gu::int32& height)
*
*  @brief     �T�C�Y�ƃE�B���h�E�̈ʒu���擾���܂�
*
*  @return   bool
*****************************************************************************/
bool CoreWindow::GetRestoredDimensions(gu::int32& x, gu::int32& y, gu::int32& width, gu::int32& height)
{
	/*---------------------------------------------------------------
		�@�@�@�@�@Window�̔z�u���擾����
	-----------------------------------------------------------------*/
	WINDOWPLACEMENT windowPlacement;
	Memory::Zero(&windowPlacement, sizeof(windowPlacement));
	windowPlacement.length = sizeof(WINDOWPLACEMENT);

	if (::GetWindowPlacement(_hwnd, &windowPlacement) == 0) { return false; }

	/*---------------------------------------------------------------
		�@�@���̃E�B���h�E�͍ŏ�������Ă���\��������܂��B  ���ɖ߂����Ƃ��̈ʒu���擾���܂��B
	-----------------------------------------------------------------*/
	const auto restoredWindow = windowPlacement.rcNormalPosition;

	x      = restoredWindow.left;
	y      = restoredWindow.top;
	width  = restoredWindow.right  - restoredWindow.left;
	height = restoredWindow.bottom - restoredWindow.top;

	// WS_EX_TOOLWINDOW�E�B���h�E�X�^�C���������Ȃ��E�B���h�E�̏ꍇ 
	// WindowPlacement.rcNormalPosition�̓��[�N�X�y�[�X���W�Ȃ̂ŁA�X�N���[�����W�ɕϊ�����K�v������B
	const auto windowExtensionStyle = ::GetWindowLong(_hwnd, GWL_EXSTYLE);
	if ((windowExtensionStyle & WS_EX_TOOLWINDOW) == 0)
	{
		const bool isTrueFullscreen = (_windowDesc.WindowMode == core::WindowMode::FullScreen);
		const auto monitor = MonitorFromWindow(_hwnd, isTrueFullscreen ? MONITOR_DEFAULTTOPRIMARY : MONITOR_DEFAULTTONEAREST);
		
		MONITORINFO monitorInfo;
		monitorInfo.cbSize = sizeof(MONITORINFO);
		GetMonitorInfo(monitor, &monitorInfo);

		x += monitorInfo.rcWork.left - monitorInfo.rcMonitor.left;
		y += monitorInfo.rcWork.top  - monitorInfo.rcMonitor.top;
	}

	return true;
}

/****************************************************************************
*                     ExistPointInWindow
*************************************************************************//**
*  @fn        bool CoreWindow::ExistPointInWindow(const gu::int32 x, const gu::int32 y) const
*
*  @brief     �w�肵���ʒu���E�B���h�E���ɑ��݂��邩�𔻒肵�܂�.
*
*  @param[in] const gu::int32 x
*  @param[in] const gu::int32 y
*
*  @return    bool
*****************************************************************************/
bool CoreWindow::ExistPointInWindow(const gu::int32 x, const gu::int32 y) const
{
	const auto region = CreateWindowRegionObject(false);
	const auto result = !!PtInRegion(region, x, y);
	DeleteObject(region);
	
	return result;
}

/****************************************************************************
*                     SetOpacity
*************************************************************************//**
*  @fn        void CoreWindow::SetOpacity(const float opacity) const
*
*  @brief     �s�����x��0�`1�͈̔͂Őݒ肵�܂�
*
*  @param[in] const float opacity (�s�����x0�`1)
* 
*  @return    void
*****************************************************************************/
void CoreWindow::SetOpacity(const float opacity) const
{
	const auto adjustOpacity = max(0, min(opacity, 255));
	SetLayeredWindowAttributes(_hwnd, 0, adjustOpacity, LWA_ALPHA);
}

/****************************************************************************
*                     SetKeyboardFocus
*************************************************************************//**
*  @fn        void CoreWindow::SetKeyboardFocus()
*
*  @brief      KeyboardFocus�ɂ��̃E�B���h�E��ݒ肷��
*
*  @param[in] void
*
*  @return    void
*****************************************************************************/
void CoreWindow::SetKeyboardFocus()
{
	if (GetFocus() != _hwnd) { ::SetFocus(_hwnd); }
}

/****************************************************************************
*                     SetKeyboardFocus
*************************************************************************//**
*  @fn        void CoreWindow::SetKeyboardFocus()
*
*  @brief     �e�E�B���h�E���ŏ������ꂽ�Ƃ��ɌĂяo�����֐�
*
*  @param[in] void
*
*  @return    bool
*****************************************************************************/
bool CoreWindow::OnParentWindowMinimized()
{
	return !!::GetWindowPlacement(_hwnd, &_previousParentMinimizedWindowPlacement);
}

/****************************************************************************
*                     SetKeyboardFocus
*************************************************************************//**
*  @fn        void CoreWindow::SetKeyboardFocus()
*
*  @brief     �e�E�B���h�E���ŏ������ꂽ�Ƃ��ɌĂяo�����֐�
*
*  @param[in] void
*
*  @return    bool
*****************************************************************************/
bool CoreWindow::OnParentWindowRestored()
{
	return !!::SetWindowPlacement(_hwnd, &_previousParentMinimizedWindowPlacement);
}
#pragma endregion Main Function
#pragma region Supported Check Function
bool CoreWindow::IsForegroundWindow() const
{
	return ::GetForegroundWindow() == _hwnd;
}

bool CoreWindow::IsFullscreenSupported() const 
{
	// fullscreen not supported when using remote desktop
	return !::GetSystemMetrics(SM_REMOTESESSION);
}

/****************************************************************************
*                     OnTransparenySupportChanged
*************************************************************************//**
*  @fn        void CoreWindow::OnTransparenySupportChanged(const core::WindowTransparency transparency)
*
*  @brief     ���ߓx�̐ݒ�
*
*  @param[in] const core::WindowTransparency ���ߓx�̐ݒ�
*
*  @return    void
*****************************************************************************/
void CoreWindow::OnTransparenySupportChanged(const core::WindowTransparency transparency)
{
	if (transparency != core::WindowTransparency::PerPixel) { return; }

	/*---------------------------------------------------------------
		�@�@�@�@�@�s�N�Z�����ɓ��ߓx��ݒ�ł����ꍇ�̏���
	-----------------------------------------------------------------*/
	const auto windowExtensionStyle = GetWindowLong(_hwnd, GWL_EXSTYLE);

	if (transparency == core::WindowTransparency::PerPixel)
	{
		SetWindowLong(_hwnd, GWL_EXSTYLE, windowExtensionStyle | WS_EX_COMPOSITED);

		MARGINS margins = { -1 };
		Confirm(SUCCEEDED(::DwmExtendFrameIntoClientArea(_hwnd, &margins)));
	}
	else
	{
		SetWindowLong(_hwnd, GWL_EXSTYLE, windowExtensionStyle & ~WS_EX_COMPOSITED);
	}

	//�����L���b�V�����X�V���邽�߁A�E�B���h�E�̃X�^�C���������X�V����Ƃ���SWP_FRAMECHANGED���Ăяo���K�v������iMSDN�ɂ��j
	SetWindowPos(_hwnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOREDRAW | SWP_NOSIZE | SWP_NOSENDCHANGING | SWP_NOZORDER);
}
#pragma endregion Supported Check Function

#endif