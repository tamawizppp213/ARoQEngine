//////////////////////////////////////////////////////////////////////////////////
///             @file   GeneralWindow.hpp
///             @brief  �E�B���h�E���쐬����N���X
///             @author Toide Yutaro
///             @date   2023_08_31
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef WINDOWS_PLATFORM_APPLICATION_HPP
#define WINDOWS_PLATFORM_APPLICATION_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Core/Include/CorePlatformMacros.hpp"

#if PLATFORM_OS_WINDOWS
#include "../../Core/Include/CorePlatformApplication.hpp"
#include "../Private/Include/WindowsDeferredMessage.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::windows
{
	class CoreWindow;
	/****************************************************************************
	*				  			    PlatformApplication
	****************************************************************************/
	/* @brief     �E�B���h�E���Ǘ���, ���b�Z�[�W���[�v���Ǘ�����N���X
	*****************************************************************************/
	class PlatformApplication : public platform::core::PlatformApplication, public gu::EnableSharedFromThis<PlatformApplication>
	{
	public:
		#pragma region Public Function

		/*!***********************************************************************
		*  @brief      �V�K�̃E�B���h�E�C���X�^���X���쐬���܂�. �����ł̓Z�b�g�A�b�v�͍s���܂���
		*  @param[in]  void
		*  @return     SharedPointer<core::CoreWindow> �E�B���h�E
		**************************************************************************/
		gu::SharedPointer<core::CoreWindow> MakeWindow() override;

		/*---------------------------------------------------------------
		�@�@@brief : �V�K�ŃR�}���h���܂Ƃ߂��N���X�̃C���X�^���X���쐬���܂�.
		-----------------------------------------------------------------*/
		gu::SharedPointer<core::PlatformCommand> MakeCommand() override;

		/*!***********************************************************************
		*  @brief      �w��̃E�B���h�E�����ۂɍ쐬��, �Z�b�g�A�b�v����܂ōs���܂� (���������ɌĂяo���K�v)
		*  @param[in]  const gu::SharedPointer<CoreWindow>& window
		*  @param[in]  const CoreWindowDesc& window�̐ݒ荀��
		*  @param[in]  const gu::SharedPointer<CoreWindow>& �e�̃E�B���h�E
		*  @return     SharedPointer<core::CoreWindow> �E�B���h�E
		**************************************************************************/
		void SetUpWindow(const gu::SharedPointer<core::CoreWindow>& window, const core::CoreWindowDesc& desc, const gu::SharedPointer<core::CoreWindow>& parentWindow) override;

		/*---------------------------------------------------------------
		�@�@@brief : ���b�Z�[�W���o���܂�. ���̊֐��͉��z�L�[���b�Z�[�W���󂯎���, 
		            ��������b�Z�[�W�`���ɕϊ���������true��Ԃ��܂�
		-----------------------------------------------------------------*/
		bool PumpMessage() override;

		/*!***********************************************************************
		*  @brief      �A�v���P�[�V�������I�������������m���܂�.
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		bool IsQuit() const override { return _isApplicationQuited; };

		#pragma endregion
		
		#pragma region Public Property

		HINSTANCE GetWindowsInstanceHandle() const noexcept { return _instanceHandle; }

		void* GetInstanceHandle() const noexcept override { return _instanceHandle; }

		#pragma region Monitor 
		/*!***********************************************************************
		*  @brief      ���j�^�[��dot per inch���擾����.  https://zenn.dev/tenka/articles/windows_display_monitor_dpi
		*  @param[in]  const core::MonitorInfo& monitor
		*  @return     gu::int32 DPI
		**************************************************************************/
		virtual gu::int32 GetMonitorDPI(const core::MonitorInfo& monitorInfo) const override;

		/*!***********************************************************************
		*  @brief      ��DPI���[�h�ɂȂ��Ă��邩�ǂ���
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool EnableHighDPIAwareness() const override { return _enableHighDPIMode; }

		/*!***********************************************************************
		*  @brief      ����s�N�Z���ʒu�ł�DPI�̊g�嗦���擾����
		*  @param[in]  const float x
		*  @param[in]  const float y
		*  @return     float �g�嗦
		**************************************************************************/
		virtual float GetDPIScaleFactorAtPixelPoint(const float x, const float y) const override;

		/*!***********************************************************************
		*  @brief      �^�X�N�o�[�Ȃǂ𖳎�������Ɨ̈��Ԃ��܂�.
		*  @param[in]  const core::Rectangle& �E�B���h�E
		*  @return     core::Rectangle ��Ɨ̈�
		**************************************************************************/
		virtual core::Rectangle GetWorkArea(const core::Rectangle& window) override;

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : ���j�^�[�̏����擾����
		-----------------------------------------------------------------*/
		virtual void GetMonitorsInfo(gu::DynamicArray<core::MonitorInfo>& monitorInfo) const override;

		#pragma endregion Monitor

		#pragma endregion
		
		#pragma region Public Constructor and Destructor
		PlatformApplication();

		~PlatformApplication();

		#pragma endregion

	protected:
		#pragma region Protected Function
		/*!***********************************************************************
		*  @brief      Windows�̃C�x���g�����s����R�[���o�b�N�֐�
		*  @param[in]  HWND  �E�B���h�E�n���h��
		*  @param[in]  UINT  ���b�Z�[�W
		*  @param[in]  WPARAM 
		*  @param[in]  LPARAM
		*  @return     LRESULT
		**************************************************************************/
		static LRESULT CALLBACK StaticWindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

		/*!***********************************************************************
		*  @brief      Windows�̃C�x���g�����s�������
		*  @param[in]  HWND  �E�B���h�E�n���h��
		*  @param[in]  UINT  ���b�Z�[�W
		*  @param[in]  WPARAM
		*  @param[in]  LPARAM
		*  @return     LRESULT
		**************************************************************************/
		virtual LRESULT ApplicationWindowMessageProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
		
		/*!***********************************************************************
		*  @brief      96dpi�ȏ��dpi��ݒ肷�邽�߂Ɏg�p���܂�.
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool SetHighDPIMode() override;

		#pragma endregion

		#pragma region Protected Property
		HINSTANCE _instanceHandle = NULL;

		#pragma endregion

	private:
		#pragma region Private Constructor and Destructor

		#pragma endregion

		#pragma region Private Function

		/*!***********************************************************************
		*  @brief 	   ���j�^�[�̏����擾���邽�߂̃R�[���o�b�N�֐�
		*  @param[in]  HMONITOR monitor : ���j�^�[�̃n���h��
		*  @param[in]  HDC monitorDC : ���j�^�[�̃f�o�C�X�R���e�L�X�g
		*  @param[in]  LPRECT rect : ���j�^�[�̋�`
		*  @param[in]  LPARAM userData : ���[�U�[�f�[�^
		*  @return     bool : �E�B���h�E�N���X�̓o�^�ɐ����������ǂ���
		**************************************************************************/
		static BOOL CALLBACK MonitorEnumProcedure(HMONITOR monitor, [[maybe_unused]]HDC monitorDC, [[maybe_unused]]LPRECT rect, LPARAM userData);
		
		/*!***********************************************************************
		*  @brief      Window�A�v���P�[�V�����ŐV�����E�B���h�E�N���X��o�^���܂�
		*  @param[in]  void
		*  @return     bool : �E�B���h�E�N���X�̓o�^�ɐ����������ǂ���
		**************************************************************************/
		bool RegisterWindowClass() const;

		/*!***********************************************************************
		*  @brief      �E�B���h�E�̊e���b�Z�[�W�������������(�����������s�������������L�q���܂�)
		*  @param[in]  HWND �E�B���h�E�n���h��
		*  @param[in]  UINT ���b�Z�[�W
		*  @param[in]  WPARAM
		*  @param[in]  LPARAM
		*  @return     LRESULT
		**************************************************************************/
		LRESULT ProcessImmediateWindowsMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

		/*!***********************************************************************
		*  @brief      �x�����������s���邽�߂̃E�B���h�E���b�Z�[�W��o�^���܂�
		*  @param[in]  HWND �E�B���h�E�n���h��
		*  @param[in]  UINT ���b�Z�[�W
		*  @param[in]  WPARAM
		*  @param[in]  LPARAM
		*  @return     LRESULT
		**************************************************************************/
		LRESULT PushDeferredWindowsMessage(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

		/*!***********************************************************************
		*  @brief      �E�B���h�E�̊e���b�Z�[�W������������� (�x�����Ă܂Ƃ߂ď������Ă���薳���悤�ȏ������L�q���܂�.)
		*  @param[in]  const DeferredMessage Windows��p�̃��b�Z�[�W
		*  @return     LRESULT
		**************************************************************************/
		LRESULT ProcessDeferredWindowsMessage(const DeferredMessage& message);
		
		void ProcessDeferredEvents();

		/*!***********************************************************************
		*  @brief      �ڑ�����Ă���}�E�X�̌����擾���܂�. 
		*  @param[in]  void
		*  @return     gu:int32 
		**************************************************************************/
		gu::int32 GetConnectedMiceCount();

		#pragma endregion
		
		#pragma region Private Property
		gu::DynamicArray<gu::SharedPointer<windows::CoreWindow>> _windows = {};

		gu::DynamicArray<DeferredMessage> _messageList = {};

		MSG _windowMessage = { NULL };

		/*! @brief �ύX���̃E�B���h�E�̕�*/
		gu::uint32 _resizingWidth = 0;
		gu::uint32 _resizingHeight = 0;

		// @brief : ���C�����[�v�̊O�Ŏ��s����Ƃ���true�ɂȂ�܂�
		bool _allowedToDeferredMessageProcessing = false;

		//@brief : �A�v���P�[�V�������I�������Ƃ���true�ɂȂ�
		bool _isApplicationQuited = false;

		/*! @brief �}�E�X���g�p���邱�ƂŃE�B���h�E�������I�ɃA�N�e�B�u��Ԃɂ��邩*/
		bool _isForceActivateByMouse = false;

		/*! @brief �T�C�Y�ύX�������͈ړ���*/
		bool _inModalSizeLoop = false;

		/*! @brief ���T�C�Y��*/
		bool _isResizing = false;

		bool _enableHighDPIMode = true;

		/*! @brief �ڑ�����Ă���}�E�X�����݂��邩*/
		bool _isConnectedMouse = false;
		#pragma endregion
	};
}
#endif
#endif