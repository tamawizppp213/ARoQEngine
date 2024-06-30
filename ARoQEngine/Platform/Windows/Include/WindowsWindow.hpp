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
	****************************************************************************/
	/* @brief     �`��p�̃E�B���h�E���쐬����N���X
	*****************************************************************************/
	class CoreWindow : public platform::core::CoreWindow, public IDropTarget
	{
	public:
		#pragma region Public Function
		/*!***********************************************************************
		*  @brief      Application�ɓo�^����, �w�肵��Descriptor�ŃE�B���h�E���쐬���܂�.
		*  @param[in]  const gu::SharedPointer<core::PlatformApplication>& application
		*  @param[in]  const core::CoreWindowDesc& desc
		*  @param[in]  const gu::SharedPointer<core::CoreWindow>& parentWindow
		*  @return     bool
		**************************************************************************/
		void Create(const gu::SharedPointer<core::PlatformApplication>& application, const core::CoreWindowDesc& desc, const gu::SharedPointer<core::CoreWindow>& parentWindow = nullptr);

		/*!***********************************************************************
		*  @brief      Native Window��\�����܂�
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool Show() override;

		/*!***********************************************************************
		*  @brief      Native Window���\���ɂ��܂�
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool Hide() override;

		/*!***********************************************************************
		*  @brief      �E�B���h�E���ŏ������܂�
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool Minimize() override;

		/*!***********************************************************************
		*  @brief      �E�B���h�E���ő剻���܂�
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool Maximize() override;

		/*!***********************************************************************
		*  @brief      �E�B���h�E���A�N�e�B�u�ɂ��ĕ\������. �ŏ����E�ő剻����Ă���ꍇ�͌��̃T�C�Y�ƈʒu�ɕ��������.
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool ReStore() override;

		/*!***********************************************************************
		*  @brief      �E�B���h�E��j�����܂�
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool Destroy() override;

		/*!***********************************************************************
		*  @brief      �E�B���h�E�T�C�Y��ύX���܂�
		*  @param[in]  const gu::int32 x
		*  @param[in]  const gu::int32 y
		*  @param[in]  const gu::int32 width
		*  @param[in]  const gu::int32 height
		*  @return     void
		**************************************************************************/
		virtual void Resize(const gu::int32 x, const gu::int32 y, const gu::int32 width, const gu::int32 height) override;
		
		/*!***********************************************************************
		*  @brief      ���ɐݒ肳�ꂽ�E�B���h�E���őO�ʂɕ\������
		*  @param[in]  const bool �����I�ɍőO�ʂɕ\�����邩
		*  @return     void
		**************************************************************************/
		virtual void BringToFront(const bool forceFront = false) override;

		/*!***********************************************************************
		*  @brief      �w�肵��x, y���W�ɃE�B���h�E���ړ������܂�
		*  @param[in]  const gu::int32 x
		*  @param[in]  const gu::int32 y
		*  @return     void
		**************************************************************************/
		virtual void Move(const gu::int32 x, const gu::int32 y) override;

		/*!***********************************************************************
		*  @brief      �E�B���h�E��L��/����������. ����������ꍇ���͂��󂯕t���Ȃ�.
		*  @param[in]  const bool �L����
		*  @return     void
		**************************************************************************/
		virtual void Enable(const bool enable) override { ::EnableWindow(_hwnd, enable); }

		/*!***********************************************************************
		*  @brief      ���ߓx��ύX����
		*  @param[in]  const core::WindowTransparency ���ߓx
		*  @return     void
		**************************************************************************/
		virtual void OnTransparencySupportChanged(const core::WindowTransparency transparency) override;
		
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
		/*!***********************************************************************
		*  @brief      �C���^�t�F�[�X (Droptarget)�̃|�C���^��Ԃ�
		*  @param[in]  REFIID riid
		*  @param[out] ppvObject
		*  @return     HRESULT
		**************************************************************************/
		virtual HRESULT STDMETHODCALLTYPE QueryInterface(
			/* [in] */ REFIID riid,
			/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) override;

		/*!***********************************************************************
		*  @brief      OLE�̎Q�ƃJ�E���^����グ��
		*  @param[in]  void
		*  @return     ULONG �Q�ƃJ�E���^
		**************************************************************************/
		virtual ULONG STDMETHODCALLTYPE AddRef() override;

		/*!***********************************************************************
		*  @brief      OLE�̎Q�ƃJ�E���^���������
		*  @param[in]  void
		*  @return     ULONG �Q�ƃJ�E���^
		**************************************************************************/
		virtual ULONG STDMETHODCALLTYPE Release() override;
		#pragma endregion IUnknown Interface
		#pragma endregion	

		#pragma region Public Property
		/*!***********************************************************************
		*  @brief      �t���X�N���[�����T�|�[�g���Ă��邩�𒲂ׂ܂�.
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool IsFullscreenSupported() const override;

		/*!***********************************************************************
		*  @brief      ���ݍ�ƒ��̃E�B���h�E�ł��邩�𒲂ׂ܂�.
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool IsForegroundWindow() const override;

		/*!***********************************************************************
		*  @brief      �ő剻����Ă��邩�𒲂ׂ܂�
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		__forceinline virtual bool IsMaximized() const override { return !!::IsZoomed(_hwnd); }

		/*!***********************************************************************
		*  @brief      �ŏ�������Ă��邩�𒲂ׂ܂� (�A�C�R����ԂɂȂ��Ă���̂�)
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		__forceinline virtual bool IsMinimized() const override { return !!::IsIconic(_hwnd); }

		/*!***********************************************************************
		*  @brief      �E�B���h�E���L��������Ă��邩
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		__forceinline virtual bool IsEnabled() override { return !!::IsWindowEnabled(_hwnd); }

		/*!***********************************************************************
		*  @brief      Window handler���擾���܂�
		*  @param[in]  void
		*  @return     HWND
		**************************************************************************/
		[[nodiscard]] __forceinline HWND GetHWND() const { return _hwnd; }

		/*!***********************************************************************
		*  @brief      �蓮��DPI��ύX�\�ɂ��邩
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool IsManualManageDPIChanges() const override { return _useManualDPIChange; }

		/*!***********************************************************************
		*  @brief      �蓮��DPI��ύX����
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual void SetManualManageDPIChanges(const bool manualManageDPIChange) override { _useManualDPIChange = manualManageDPIChange; }

		/*!***********************************************************************
		*  @brief      �E�B���h�E�̃n���h�����擾���܂�
		*  @param[in]  void
		*  @return     void*
		**************************************************************************/
		[[nodiscard]] __forceinline virtual void* GetWindowHandle() const noexcept override { return _hwnd; }

		/*!***********************************************************************
		*  @brief      Window�̃A�X�y�N�g���n���܂�.
		*  @param[in]  void
		*  @return     float
		**************************************************************************/
		__forceinline virtual float GetAspectRatio() const override { return _aspectRatio; }

		/*!***********************************************************************
		*  @brief      Window�̍���ʒu���擾���܂�.
		*  @param[out] gu::int32& x
		*  @param[out] gu::int32& y
		*  @return     void
		**************************************************************************/
		virtual void GetWindowPostion(gu::int32& x, gu::int32& y) const override;

		/*!***********************************************************************
		*  @brief      Window�̃{�[�_�[�T�C�Y���擾���܂�.
		*  @param[in]  void
		*  @return     gu::int32
		**************************************************************************/
		virtual gu::int32 GetWindowBorderSize() const override;

		/*!***********************************************************************
		*  @brief      Window�̃^�C�g���o�[�̃T�C�Y���擾���܂�.
		*  @param[in]  void
		*  @return     gu::int32
		**************************************************************************/
		virtual gu::int32 GetWindowTitleBarSize() const override;

		/*!***********************************************************************
		*  @brief      �E�B���h�E���������ꂽ�Ƃ��̃T�C�Y�ƈʒu��Ԃ�
		*  @param[out] gu::int32& x
		*  @param[out] gu::int32& y
		*  @param[out] gu::int32& width
		*  @param[out] gu::int32& height
		*  @return     bool
		**************************************************************************/
		bool GetRestoredDimensions(gu::int32& x, gu::int32& y, gu::int32& width, gu::int32& height);

		/*!***********************************************************************
		*  @brief      ���W�̉��ɃE�B���h�E�����݂���ꍇ��true��Ԃ�
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool ExistPointInWindow(const gu::int32 x, const gu::int32 y) const override;
		
		/*!***********************************************************************
		*  @brief       Window handler��ݒ肵�܂�
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		__forceinline void SetHWND(const HWND hwnd) { _hwnd = hwnd; }

		/*!***********************************************************************
		*  @brief      �t���X�N���[����E�B���h�E���[�h�Ƃ�����Window�̏�Ԃ�ݒ肵�܂�
		*  @param[in]  const WindowMode �E�B���h�E���[�h
		*  @return     void
		**************************************************************************/
		virtual void SetWindowMode(const core::WindowMode windowMode) override;

		/*!***********************************************************************
		*  @brief      �L���v�V�����Ƀe�L�X�g��ݒ肷��
		*  @param[in]  const gu::tchar* const text
		*  @return     void
		**************************************************************************/
		__forceinline virtual void SetText(const wchar_t* const text) override { SetWindowText(_hwnd, text); }

		/*!***********************************************************************
		*  @brief      ���ߓx��ݒ肷��
		*  @param[in]  const float ���ߓx
		*  @return     void
		**************************************************************************/
		virtual void SetOpacity(const float opacity) const;

		/*!***********************************************************************
		*  @brief      KeyboardFocus�ɂ��̃E�B���h�E��ݒ肷��
		*  @param[in]  void
		*  @return     void
		**************************************************************************/
		virtual void SetKeyboardFocus() override;

		/*!***********************************************************************
		*  @brief      FullScreen�����擾����
		*  @param[out] gu::int32& x
		*  @param[out] gu::int32& y
		*  @param[out] gu::int32& width
		*  @param[out] gu::int32& height
		*  @return     bool
		**************************************************************************/
		virtual bool GetFullScreenInfo(gu::int32& x, gu::int32& y, gu::int32& width, gu::int32& height) const override;

		/*!***********************************************************************
		*  @brief      �e�E�B���h�E���ŏ������ꂽ�Ƃ��ɌĂяo�����֐�
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		bool OnParentWindowMinimized();

		/*!***********************************************************************
		*  @brief      �e�E�B���h�E��Restore���ꂽ�Ƃ��ɌĂяo�����֐�
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		bool OnParentWindowRestored();

		#pragma endregion

		#pragma region Public Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		CoreWindow();

		/*! @brief �f�X�g���N�^*/
		~CoreWindow();

		#pragma endregion

	protected:
		#pragma region Protected Constructor and Destructor

		#pragma endregion

		#pragma region Protected Function

		/*!***********************************************************************
		*  @brief      �^�b�`�t�B�[�h�o�b�N�𖳌�������.
		*  @param[in]  void
		*  @return     void
		**************************************************************************/
		void DisableTouchFeedback();
		
		/*!***********************************************************************
		*  @brief      �E�B���h�E�̈�̒���
		*  @param[in]  const gu::int32 ��
		*  @param[in]  const gu::int32 ����
		*  @return     void
		**************************************************************************/
		void AdjustWindowRegion(const gu::int32 width, const gu::int32 height);

		#pragma endregion

		#pragma region Protected Property

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		/* @brief Window handle*/
		HWND _hwnd = NULL;

		/*! @brief Window��Client�̈�̃A�X�y�N�g��*/
		float _aspectRatio = 0.0f;

		/*! @brief �E�B���h�E���܂��ŏ���Show() ���Ăяo���Ă��Ȃ���*/
		bool _isFirstTimeVisible = true;

		/*! @brief ����̍ŏ���*/
		bool _initiallyMinimized = false;

		/*! @brief ����̍ő�*/
		bool _initiallyMaximized = false;

		/*! @brief 96DPI�ȏ��DPI�ݒ��L�������邩*/
		bool _enableHighDPIMode = false;

		// @brief : �f�B�X�v���C�̊g�嗦�ł�. 
		float _dpiScaleFactor = 1.0f;

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

		// @brief : �Q�ƃJ�E���g
		gu::int32 _oleReferenceCount = 0;

		// @brief : DPI���蓮�ŕύX���邩
		bool _useManualDPIChange = false;

		#pragma endregion
	private:
		#pragma region Private Constructor and Destructor

		#pragma endregion

		#pragma region Private Function
		/*!***********************************************************************
		*  @brief      Region�I�u�W�F�N�g�̍쐬
		*  @param[in]  const bool useBorderWhenMaximized�͍ő剻���Ă���Ƃ��ɂ����E�������g����.
		*  @return     void
		**************************************************************************/
		HRGN CreateWindowRegionObject(const bool useBorderWhenMaximized) const;
		#pragma endregion

		#pragma region Private Property

		#pragma endregion
	};
}
#endif _WIN32
#endif 