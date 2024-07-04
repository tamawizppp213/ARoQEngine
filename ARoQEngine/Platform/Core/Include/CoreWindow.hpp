//////////////////////////////////////////////////////////////////////////////////
///             @file   GeneralWindow.hpp
///             @brief  �E�B���h�E���쐬����N���X
///             @author Toide Yutaro
///             @date   2023_08_31
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GENERAL_WINDOW_HPP
#define GENERAL_WINDOW_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Core/Include/CoreCommonState.hpp"
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::core
{
	class PlatformApplication;

	/****************************************************************************
	*				  			    CoreWindow
	****************************************************************************/
	/* @brief    �I�u�W�F�N�g��\�����邽�߂̃E�B���h�E���쐬���܂�
	*****************************************************************************/
	class CoreWindow : public gu::NonCopyable
	{
	public:
		#pragma region Static 
		struct EmbeddedWindowHandle
		{
			/*! @brief ���ߍ��݃��[�h���g�p���邩*/
			bool UseEmbeddedMode = false;

			/*! @brief �E�B���h�E�n���h��*/
			void* WindowHandle = nullptr;

			/*! @brief �㏑���\��*/
			bool IsOverride = true;

			/*! @brief ��*/
			gu::int32 Width = 0;

			/*! @brief ����*/
			gu::int32 Height = 0;
		};

		static EmbeddedWindowHandle EmbeddedWindow;

		#pragma endregion
		#pragma region Public Function
		/*!***********************************************************************
		*  @brief      Native Window��\�����܂�
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool Show() = 0;

		/*!***********************************************************************
		*  @brief      Native Window���\���ɂ��܂�
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool Hide() = 0;

		/*!***********************************************************************
		*  @brief      �E�B���h�E���ŏ������܂�
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool Minimize() = 0;

		/*!***********************************************************************
		*  @brief      �E�B���h�E���ő剻���܂�
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool Maximize() = 0;

		/*!***********************************************************************
		*  @brief      �E�B���h�E���A�N�e�B�u�ɂ��ĕ\������. �ŏ����E�ő剻����Ă���ꍇ�͌��̃T�C�Y�ƈʒu�ɕ��������.
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool ReStore() = 0;

		/*!***********************************************************************
		*  @brief      �E�B���h�E��j�����܂�
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool Destroy() = 0;

		/*!***********************************************************************
		*  @brief      �E�B���h�E�T�C�Y��ύX���܂�
		*  @param[in]  const gu::int32 x
		*  @param[in]  const gu::int32 y
		*  @param[in]  const gu::int32 width
		*  @param[in]  const gu::int32 height
		*  @return     void
		**************************************************************************/
		virtual void Resize(const gu::int32 x, const gu::int32 y, const gu::int32 width, const gu::int32 height) = 0;
		
		/*!***********************************************************************
		*  @brief      ���ɐݒ肳�ꂽ�E�B���h�E���őO�ʂɕ\������
		*  @param[in]  const bool �����I�ɍőO�ʂɕ\�����邩
		*  @return     void
		**************************************************************************/
		virtual void BringToFront(const bool forceFront = false) = 0;

		/*!***********************************************************************
		*  @brief      �w�肵��x, y���W�ɃE�B���h�E���ړ������܂�
		*  @param[in]  const gu::int32 x
		*  @param[in]  const gu::int32 y
		*  @return     void
		**************************************************************************/
		virtual void Move(const gu::int32 x, const gu::int32 y) = 0;

		/*!***********************************************************************
		*  @brief      �E�B���h�E��L��/����������. ����������ꍇ���͂��󂯕t���Ȃ�.
		*  @param[in]  const bool �L����
		*  @return     void
		**************************************************************************/
		virtual void Enable(const bool enable) = 0;

		/*!***********************************************************************
		*  @brief      ���ߓx��ύX����
		*  @param[in]  const core::WindowTransparency ���ߓx
		*  @return     void
		**************************************************************************/
		virtual void OnTransparencySupportChanged(const core::WindowTransparency transparency) = 0;

		#pragma endregion 

		#pragma region Public Property
		/*!***********************************************************************
		*  @brief      �t���X�N���[�����T�|�[�g���Ă��邩�𒲂ׂ܂�. 
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool IsFullscreenSupported() const = 0;

		/*!***********************************************************************
		*  @brief      ���ݍ�ƒ��̃E�B���h�E�ł��邩�𒲂ׂ܂�.
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool IsForegroundWindow() const = 0;

		/*!***********************************************************************
		*  @brief      �ő剻����Ă��邩�𒲂ׂ܂�
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool IsMaximized() const = 0;

		/*!***********************************************************************
		*  @brief      �ŏ�������Ă��邩�𒲂ׂ܂�
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool IsMinimized() const = 0;

		/*!***********************************************************************
		*  @brief      �E�B���h�E���L��������Ă��邩
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool IsEnabled() = 0;

		/*!***********************************************************************
		*  @brief      �蓮��DPI��ύX�\�ɂ��邩
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool IsManualManageDPIChanges() const = 0;

		/*!***********************************************************************
		*  @brief      �蓮��DPI��ύX����
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual void SetManualManageDPIChanges(const bool manualManageDPIChange) = 0;

		/*!***********************************************************************
		*  @brief      �E�B���h�E�̃n���h�����擾���܂�
		*  @param[in]  void
		*  @return     void*
		**************************************************************************/
		[[nodiscard]] virtual void* GetWindowHandle() const noexcept = 0;

		/*!***********************************************************************
		*  @brief      Window�̃A�X�y�N�g���n���܂�.
		*  @param[in]  void
		*  @return     float
		**************************************************************************/
		virtual float GetAspectRatio() const = 0;

		/*!***********************************************************************
		*  @brief      Window�̃��[�h���擾���܂�.
		*  @param[in]  void
		*  @return     WindowMode
		**************************************************************************/
		WindowMode GetWindowMode() const { return _windowDesc.WindowMode; };

		/*!***********************************************************************
		*  @brief      Window�̍���ʒu���擾���܂�.
		*  @param[out] gu::int32& x
		*  @param[out] gu::int32& y
		*  @return     void
		**************************************************************************/
		virtual void GetWindowPostion(gu::int32& x, gu::int32& y) const = 0;

		/*!***********************************************************************
		*  @brief      Window�̃{�[�_�[�T�C�Y���擾���܂�.
		*  @param[in]  void
		*  @return     gu::int32
		**************************************************************************/
		virtual gu::int32 GetWindowBorderSize() const = 0;

		/*!***********************************************************************
		*  @brief      Window�̃^�C�g���o�[�̃T�C�Y���擾���܂�.
		*  @param[in]  void
		*  @return     gu::int32
		**************************************************************************/
		virtual gu::int32 GetWindowTitleBarSize() const = 0;

		/*!***********************************************************************
		*  @brief      �E�B���h�E���������ꂽ�Ƃ��̃T�C�Y�ƈʒu��Ԃ�
		*  @param[out] gu::int32& x
		*  @param[out] gu::int32& y
		*  @param[out] gu::int32& width
		*  @param[out] gu::int32& height
		*  @return     bool
		**************************************************************************/
		virtual bool GetRestoredDimensions(gu::int32& x, gu::int32& y, gu::int32& width, gu::int32& height) = 0;

		/*!***********************************************************************
		*  @brief      ���W�̉��ɃE�B���h�E�����݂���ꍇ��true��Ԃ�
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool ExistPointInWindow(const gu::int32 x, const gu::int32 y) const = 0;

		/*!***********************************************************************
		*  @brief      �E�B���h�E���L��������Ă��邩
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		bool IsVisible() const { return _isVisible; }
	
		/*!***********************************************************************
		*  @brief      �E�B���h�E���[�h��ݒ肷��
		*  @param[in]  const WindowMode �E�B���h�E���[�h
		*  @return     void
		**************************************************************************/
		virtual void SetWindowMode(const WindowMode windowMode) = 0;

		/*!***********************************************************************
		*  @brief      �L���v�V�����Ƀe�L�X�g��ݒ肷��
		*  @param[in]  const gu::tchar* const text
		*  @return     void
		**************************************************************************/
		virtual void SetText(const gu::tchar* const text) = 0;

		/*!***********************************************************************
		*  @brief      ���ߓx��ݒ肷��
		*  @param[in]  const float ���ߓx
		*  @return     void
		**************************************************************************/
		virtual void SetOpacity(const float opacity) const = 0;

		/*!***********************************************************************
		*  @brief      KeyboardFocus�ɂ��̃E�B���h�E��ݒ肷��
		*  @param[in]  void
		*  @return     void
		**************************************************************************/
		virtual void SetKeyboardFocus() = 0;

		/*!***********************************************************************
		*  @brief      FullScreen�����擾����
		*  @param[out] gu::int32& x
		*  @param[out] gu::int32& y
		*  @param[out] gu::int32& width
		*  @param[out] gu::int32& height
		*  @return     bool
		**************************************************************************/
		virtual bool GetFullScreenInfo(gu::int32& x, gu::int32& y, gu::int32& width, gu::int32& height) const = 0;

		/*!***********************************************************************
		*  @brief      Window�쐬�̂��߂̏����ݒ��Ԃ��܂�. 
		*  @param[in]  void
		*  @return     const CoreWindowDesc& 
		**************************************************************************/
		__forceinline const CoreWindowDesc& GetDesc() const { return _windowDesc; }

		#pragma endregion

		#pragma region Public Constructor and Destructor
		
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		CoreWindow();

		/*! @brief �f�X�g���N�^*/
		~CoreWindow();

		/*! @brief WindowMode�ŏ�����*/
		CoreWindow(const WindowMode windowMode);

		/*! @brief CoreWindowDesc�ŏ�����*/
		CoreWindow(const CoreWindowDesc& desc);
		#pragma endregion
	protected:

		#pragma region Protected Constructor and Destructor
		#pragma endregion
		
		#pragma region Protected Function
		#pragma endregion

		#pragma region Protected Property
		/*! @brief Window�̐ݒ�*/
		CoreWindowDesc _windowDesc = {};

		/*! @brief Platform Application*/
		gu::SharedPointer<PlatformApplication> _application = nullptr;

		/*! @brief �E�B���h�E���\������Ă��邩*/
		bool _isVisible = false;

		#pragma endregion
	};
}
#endif