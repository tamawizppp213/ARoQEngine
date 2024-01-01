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
	*************************************************************************//**
	*  @class     PlatformApplication
	*  @brief     �E�B���h�E���Ǘ���, ���b�Z�[�W���[�v���Ǘ�����N���X
	*****************************************************************************/
	class PlatformApplication : public platform::core::PlatformApplication, public gu::EnableSharedFromThis<PlatformApplication>
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*---------------------------------------------------------------
		�@�@@brief : �V�K�̃E�B���h�E�C���X�^���X���쐬���܂�. �����ł̓Z�b�g�A�b�v�͍s���܂���
		-----------------------------------------------------------------*/
		gu::SharedPointer<core::CoreWindow> MakeWindow() override;

		/*---------------------------------------------------------------
		�@�@@brief : �V�K�ŃR�}���h���܂Ƃ߂��N���X�̃C���X�^���X���쐬���܂�.
		-----------------------------------------------------------------*/
		gu::SharedPointer<core::PlatformCommand> MakeCommand() override;

		/*---------------------------------------------------------------
		�@�@@brief : �w��̃E�B���h�E�����ۂɍ쐬����܂ōs���܂�. �Z�b�g�A�b�v���s���܂�
		-----------------------------------------------------------------*/
		void SetUpWindow(const gu::SharedPointer<core::CoreWindow>& window, const core::CoreWindowDesc& desc, const gu::SharedPointer<core::CoreWindow>& parentWindow) override;

		/*---------------------------------------------------------------
		�@�@@brief : ���b�Z�[�W���o���܂�. ���̊֐��͉��z�L�[���b�Z�[�W���󂯎���, 
		            ��������b�Z�[�W�`���ɕϊ���������true��Ԃ��܂�
		-----------------------------------------------------------------*/
		bool PumpMessage() override;

		/*---------------------------------------------------------------
		�@�@@brief : �A�v���P�[�V�������I�������������m���܂�.
		-----------------------------------------------------------------*/
		bool IsQuit() const override { return _isApplicationQuited; };
		
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		HINSTANCE GetWindowsInstanceHandle() const noexcept { return _instanceHandle; }

		void* GetInstanceHandle() const noexcept override { return _instanceHandle; }

#pragma region Monitor 
		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : ���j�^�[��display per inch���擾����.
		-----------------------------------------------------------------*/
		virtual gu::int32 GetMonitorDPI(const core::MonitorInfo& monitorInfo) const override;

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : ��DPI���[�h�ɂȂ��Ă��邩�ǂ���
		-----------------------------------------------------------------*/
		virtual bool EnableHighDPIAwareness() const override { return _enableHighDPIMode; }

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : ����s�N�Z���ʒu�ł�DPI�̊g�嗦���擾����
		-----------------------------------------------------------------*/
		virtual float GetDPIScaleFactorAtPixelPoint(const float x, const float y) const override;

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : �^�X�N�o�[�Ȃǂ𖳎�������Ɨ̈��Ԃ��܂�.
		-----------------------------------------------------------------*/
		virtual core::Rectangle GetWorkArea(const core::Rectangle& window) override;

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : ���j�^�[�̏����擾����
		-----------------------------------------------------------------*/
		virtual void GetMonitorsInfo(std::vector<core::MonitorInfo>& monitorInfo) const override;

#pragma endregion Monitor
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		PlatformApplication();

		~PlatformApplication();
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		/* @brief : Event driven windows function*/
		static LRESULT CALLBACK StaticWindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

		virtual LRESULT ApplicationWindowMessageProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
		
		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : 96dpi�ȏ��dpi��ݒ肷�邽�߂Ɏg�p���܂�.
		-----------------------------------------------------------------*/
		virtual bool SetHighDPIMode() override;
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		HINSTANCE _instanceHandle = NULL;

	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		static BOOL CALLBACK MonitorEnumProcedure(HMONITOR monitor, HDC monitorDC, LPRECT rect, LPARAM userData);
		
		bool RegisterWindowClass();

		LRESULT ProcessDeferredWindowsMessage(const DeferredMessage& message);
		
		void ProcessDeferredEvents();
		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		std::vector<gu::SharedPointer<windows::CoreWindow>> _windows = {};

		std::vector<DeferredMessage> _messageList = {};

		MSG _windowMessage = { NULL };

		// @brief : ���C�����[�v�̊O�Ŏ��s����Ƃ���true�ɂȂ�܂�
		bool _allowedToDeferredMessageProcessing = false;

		//@brief : �A�v���P�[�V�������I�������Ƃ���true�ɂȂ�
		bool _isApplicationQuited = false;

		bool _enableHighDPIMode = true;
	};
}
#endif
#endif