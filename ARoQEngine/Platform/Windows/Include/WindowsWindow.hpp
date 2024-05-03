//////////////////////////////////////////////////////////////////////////////////
///             @file   GeneralWindow.hpp
///             @brief  �E�B���h�E���쐬����N���X
///             @author Toide Yutaro
///             @date   2023_08_31
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef WINDOWS_WINDOW_HPP
#define WINDOWS_WINDOW_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Core/Include/CorePlatformMacros.hpp"

#if PLATFORM_OS_WINDOWS
#include "../../Core/Include/CoreWindow.hpp"
#include <Windows.h>
#include <Ole2.h>
#include <oleidl.h>
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::windows
{
	/****************************************************************************
	*				  			    CoreWindow
	*************************************************************************//**
	*  @class     CoreWindow
	*  @brief     �`��p�̃E�B���h�E���쐬����N���X
	*****************************************************************************/
	class CoreWindow : public platform::core::CoreWindow, public IDropTarget
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : Application�ɓo�^����, �w�肵��Descriptor�ŃE�B���h�E���쐬���܂�. 
		-----------------------------------------------------------------*/
		void Create(const gu::SharedPointer<core::PlatformApplication>& application, const core::CoreWindowDesc& desc, const gu::SharedPointer<core::CoreWindow>& parentWindow = nullptr);

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : Visible�łȂ������ꍇ�ɃE�B���h�E��\�����܂�
		-----------------------------------------------------------------*/
		virtual bool Show() override;

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : Visible�ł������ꍇ�ɃE�B���h�E���B���܂�
		-----------------------------------------------------------------*/
		virtual bool Hide() override;

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : �E�B���h�E���ŏ������܂�
		-----------------------------------------------------------------*/
		virtual bool Minimize() override;

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : �E�B���h�E���ő剻���܂�.
		-----------------------------------------------------------------*/
		virtual bool Maximize() override;

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : �E�B���h�E���A�N�e�B�u�ɂ��ĕ\������
			            �ŏ����E�ő剻����Ă���ꍇ�͌��̃T�C�Y�ƈʒu�ɕ��������.
		-----------------------------------------------------------------*/
		virtual bool ReStore() override;

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief :  �E�B���h�E��j�����܂�
		-----------------------------------------------------------------*/
		virtual bool Destroy() override;

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief :  �E�B���h�E�T�C�Y��ύX���܂�
		-----------------------------------------------------------------*/
		virtual void Resize(const gu::int32 x, const gu::int32 y, const gu::int32 width, const gu::int32 height) override;
		
		/*---------------------------------------------------------------
		�@�@�@�@�@@brief :  ���ɐݒ肳�ꂽ�E�B���h�E���őO�ʂɕ\������
		-----------------------------------------------------------------*/
		virtual void BringToFront(const bool forceFront = false) override;

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : �w�肵��x, y���W�ɃE�B���h�E���ړ������܂�
		-----------------------------------------------------------------*/
		virtual void Move(const gu::int32 x, const gu::int32 y) override;

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : �E�B���h�E��L��/����������. ����������ꍇ���͂��󂯕t���Ȃ�.
		-----------------------------------------------------------------*/
		virtual void Enable(const bool enable) override { ::EnableWindow(_hwnd, enable); }

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : ���ߓx��ύX����	
		-----------------------------------------------------------------*/
		void OnTransparenySupportChanged(const core::WindowTransparency transparency);
		
#pragma region IDropTarget
		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : �I�u�W�F�N�g���R���g���[���̋��E���Ƀh���b�O�����Ɣ�������C�x���g
		-----------------------------------------------------------------*/
		virtual HRESULT STDMETHODCALLTYPE DragEnter(
			/* [unique][in] */ __RPC__in_opt IDataObject* dataObject,
			/* [in] */ DWORD keyState,
			/* [in] */ POINTL cursorPosition,
			/* [out][in] */ __RPC__inout DWORD* cursorEffect) override;

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : �I�u�W�F�N�g���R���g���[���̋��E���z���ăh���b�O�����Ɣ�������C�x���g
		-----------------------------------------------------------------*/
		virtual HRESULT STDMETHODCALLTYPE DragOver(
			/* [in] */ DWORD keyState,
			/* [in] */ POINTL cursorPosition,
			/* [out][in] */ __RPC__inout DWORD* cursorEffect) override;

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : �I�u�W�F�N�g���R���g���[���̋��E�O�Ƀh���b�O���ꂽ�Ƃ��ɔ�������C�x���g
		-----------------------------------------------------------------*/
		virtual HRESULT STDMETHODCALLTYPE DragLeave() override;

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : �I�u�W�F�N�g���h���b�v����Ƃ��ɔ�������C�x���g
		-----------------------------------------------------------------*/
		virtual HRESULT STDMETHODCALLTYPE Drop(
			/* [unique][in] */ __RPC__in_opt IDataObject* dataObject,
			/* [in] */ DWORD keyState,
			/* [in] */ POINTL cursorPosition,
			/* [out][in] */ __RPC__inout DWORD* cursorEffect) override;

#pragma endregion IDropTarget

#pragma region IUnknown Interface
		virtual HRESULT STDMETHODCALLTYPE QueryInterface(
			/* [in] */ REFIID riid,
			/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

		virtual ULONG STDMETHODCALLTYPE AddRef() override;

		virtual ULONG STDMETHODCALLTYPE Release() override;
#pragma endregion IUnknown Interface
		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : Windows��p����Ȃ��ꍇ��Window Handler�̎󂯓n���ł�.
		-----------------------------------------------------------------*/
		[[nodiscard]] __forceinline virtual void* GetWindowHandle() const noexcept override { return _hwnd; }

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : Window�̃A�X�y�N�g���n���܂�.
		-----------------------------------------------------------------*/
		__forceinline virtual float GetAspectRatio() const override { return _aspectRatio;  }

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : Window handler���擾���܂�
		-----------------------------------------------------------------*/
		[[nodiscard]] __forceinline HWND GetHWND() const { return _hwnd; }

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : return the border size
		-----------------------------------------------------------------*/
		virtual gu::int32 GetWindowBorderSize() const override;

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : return the title bar size
		-----------------------------------------------------------------*/
		virtual gu::int32 GetWindowTitleBarSize() const override;

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : window�̌��݂̍���̈ʒu��Ԃ��܂�.
		-----------------------------------------------------------------*/
		virtual void GetWindowPostion(gu::int32& x, gu::int32& y) const override;
		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : return the title bar size
		-----------------------------------------------------------------*/
		bool GetRestoredDimensions(gu::int32& x, gu::int32& y, gu::int32& width, gu::int32& height);

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : �w�肵���ʒu���E�B���h�E���ɑ��݂��邩�𔻒肵�܂�.
		-----------------------------------------------------------------*/
		virtual bool ExistPointInWindow(const gu::int32 x, const gu::int32 y) const override;
		
		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : Window handler��ݒ肵�܂�
		-----------------------------------------------------------------*/
		__forceinline void SetHWND(const HWND hwnd) { _hwnd = hwnd; }

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : �L���v�V�����Ƀe�L�X�g��ǉ����܂�.
		-----------------------------------------------------------------*/
		__forceinline virtual void SetText(const wchar_t* const text) override { SetWindowText(_hwnd, text); }

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : ���ݍ�ƒ��̃E�B���h�E�ł��邩�𒲂ׂ܂�.
		-----------------------------------------------------------------*/
		virtual bool IsForegroundWindow() const override;

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : �t���X�N���[�����T�|�[�g���Ă��邩�𒲂ׂ܂�. 
		-----------------------------------------------------------------*/
		virtual bool IsFullscreenSupported() const override;

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : �ő剻����Ă��邩�𒲂ׂ܂�
		-----------------------------------------------------------------*/
		__forceinline virtual bool IsMaximized() const override { return !!::IsZoomed(_hwnd); }

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : �ŏ�������Ă��邩�𒲂ׂ܂� (�A�C�R����ԂɂȂ��Ă��邩)
		-----------------------------------------------------------------*/
		__forceinline virtual bool IsMinimized() const override { return !!::IsIconic(_hwnd); }
		
		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : �E�B���h�E���L��������Ă��邩
		-----------------------------------------------------------------*/
		virtual bool IsEnabled() override { return !!::IsWindowEnabled(_hwnd); }

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : �蓮��DPI��ύX�\�ɂ��邩
		-----------------------------------------------------------------*/
		virtual bool IsManualManageDPIChanges() const override { return _useManualDPIChange; }

		virtual void SetManualManageDPIChanges(const bool manualManageDPIChange) override { _useManualDPIChange = manualManageDPIChange; }
		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : Return the rectangle of the winow is associated with
		-----------------------------------------------------------------*/
		virtual bool GetFullScreenInfo(gu::int32& x, gu::int32& y, gu::int32& width, gu::int32& height) const override;

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : window mode��ݒ肷��
		-----------------------------------------------------------------*/
		virtual void SetWindowMode(const core::WindowMode windowMode) override;

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : ���ߓx��ݒ肷��
		-----------------------------------------------------------------*/
		virtual void SetOpacity(const float opacity) const;

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : KeyboardFocus�ɂ��̃E�B���h�E��ݒ肷��
		-----------------------------------------------------------------*/
		virtual void SetKeyboardFocus() override;

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : �e�E�B���h�E���ŏ������ꂽ�Ƃ��ɌĂяo�����֐�
		-----------------------------------------------------------------*/
		bool OnParentWindowMinimized();

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : �e�E�B���h�E��Restore���ꂽ�Ƃ��ɌĂяo�����֐�
		-----------------------------------------------------------------*/
		bool OnParentWindowRestored();

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		CoreWindow();

		~CoreWindow();

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : �^�b�`�t�B�[�h�o�b�N�𖳌�������.
		-----------------------------------------------------------------*/
		void DisableTouchFeedback();

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : �E�B���h�E�̈�̒���
		-----------------------------------------------------------------*/
		void AdjustWindowRegion(const gu::int32 width, const gu::int32 height);

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		/* @brief : Window handle*/
		HWND _hwnd = NULL;

		float _aspectRatio = 0.0f;

		bool _isFirstTimeVisible = true;
		bool _initiallyMinimized = false;
		bool _initiallyMaximized = false;

		// @brief : �f�B�X�v���C�̊g�嗦�ł�. 
		float _dpiScaleFactor = 1.0f;

		bool _enableHighDPIMode = false;

		// @brief : �f�B�X�v���C�̉��z�I�ȕ��ƍ�����ݒ肵�܂�. 
		// ���ۂɃ��T�C�Y����GPU�o�b�t�@�̃p�t�H�[�}���X�ɉe����^����̂ł͂Ȃ�, 
		// Windows���ŏ�����������悤�Ƀg���~���O���ĕ`�悳���邽�߂Ɏg�p���܂�.
		gu::int32 _virtualWindowWidth  = 0;
		gu::int32 _virtualWindowHeight = 0;

		// @brief : �C�ӂ̃s�N�Z�������E�B���h�E���ɂ��邩�ǂ�����₢���킹�邽�߂̃E�B���h�E�̈�T�C�Y���i�[����
		gu::int32 _regionWidth  = -1;
		gu::int32 _regionHeight = -1;

		// @brief : �t���X�N���[����ԂɂȂ�O�̃E�B���h�E�̔z�u�����肷��
		WINDOWPLACEMENT _previousFullScreenWindowPlacement = WINDOWPLACEMENT();

		// @brief : �e�E�B���h�E���ŏ��������O�̔z�u���
		WINDOWPLACEMENT _previousParentMinimizedWindowPlacement = WINDOWPLACEMENT();

		// @brief : DPI���蓮�ŕύX���邩
		bool _useManualDPIChange = false;

		// @brief : �Q�ƃJ�E���g
		gu::int32 _oleReferenceCount = 0;
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : Region�I�u�W�F�N�g�̍쐬]
			 const bool useBorderWhenMaximized�͍ő剻���Ă���Ƃ��ɂ����E�������g����.
		-----------------------------------------------------------------*/
		HRGN CreateWindowRegionObject(const bool useBorderWhenMaximized) const;
	};
}
#endif _WIN32
#endif 