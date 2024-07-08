//////////////////////////////////////////////////////////////////////////////////
///             @file   CoreWindowMessageHandler.hpp
///             @brief  �E�B���h�E���b�Z�[�W�̈�������o�^���܂�. 
///             @author Toide Yutaro
///             @date   2023_09_23
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef CORE_WINDOW_MESSAGE_HANDLER_HPP
#define CORE_WINDOW_MESSAGE_HANDLER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "CoreCommonState.hpp"
#include "GameUtility/Base/Include/GUType.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Base/Include/GUClassUtility.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::core
{
	class CoreWindow;
	/****************************************************************************
	*				  			    CoreWindowMessageHandler
	****************************************************************************/
	/* @class     CoreWindowMessageHandler
	*  @brief     �E�B���h�E�̃��b�Z�[�W���󂯎�������Ɏg�p����Handler�N���X
	*****************************************************************************/
	class CoreWindowMessageHandler : public gu::NonCopyable
	{
	protected:
		using CoreWindowPtr = gu::SharedPointer<core::CoreWindow>;
	public:
		#pragma region Static Function

		#pragma endregion

		#pragma region Public Function
		/*----------------------------------------------------------------------
		*  @brief : This function is called by window activation changed
		*----------------------------------------------------------------------*/
		virtual bool OnWindowActivationChanged([[maybe_unused]] const CoreWindowPtr& window, [[maybe_unused]] const core::ActivationType type)
		{
			return true;
		}

		/*!***********************************************************************
		*  @brief      Window���A�N�e�B�u��, ��A�N�e�B�u�����ꂽ���ɑ����郁�b�Z�[�W�̎���
		*  @param[in]  const bool isActive
		*  @return     bool
		**************************************************************************/
		virtual bool OnWindowApplicationActivationChanged([[maybe_unused]]const bool isActive)
		{
			return true;
		}

		/*!***********************************************************************
		*  @brief      Window���A�N�e�B�u��, ��A�N�e�B�u�����ꂽ���ɑ����郁�b�Z�[�W�̎���
		*  @param[in]  const CoreWindowPtr& window
		*  @return     bool
		**************************************************************************/
		virtual bool OnWindowAction([[maybe_unused]] const CoreWindowPtr& window, [[maybe_unused]]const core::WindowActionType action)
		{
			return true;
		}

		/*!***********************************************************************
		*  @brief      Window���ړ������Ƃ��ɑ����郁�b�Z�[�W�̎���
		*  @param[in]  const CoreWindowPtr& window
		*  @return     bool
		**************************************************************************/
		virtual bool OnWindowMoved([[maybe_unused]] const CoreWindowPtr& window, [[maybe_unused]]const gu::int32 x, [[maybe_unused]]const gu::int32 y)
		{
			return true;
		}

		/*!***********************************************************************
		*  @brief      Window���A�N�e�B�u��, ��A�N�e�B�u�����ꂽ���ɑ����郁�b�Z�[�W�̎���
		*  @param[in]  const CoreWindowPtr& window
		*  @return     bool
		**************************************************************************/
		virtual bool OnWindowResizing([[maybe_unused]] const CoreWindowPtr& window)
		{
			_isResizing = true;
			return true;
		}

		/*!***********************************************************************
		*  @brief      �ΏۃE�B���h�E�̈ꕔ��h��Ԃ��v�����s���ꍇ�ɑ����郁�b�Z�[�W�̎���
		*  @param[in]  const CoreWindowPtr& window
		*  @return     bool
		**************************************************************************/
		virtual bool OnOSPainted([[maybe_unused]]const CoreWindowPtr& window)
		{
			return true;
		}

		/*----------------------------------------------------------------------
		*  @brief : This function is called by window closed
		*----------------------------------------------------------------------*/
		virtual bool OnWindowClosed(const CoreWindowPtr& window)
		{
			return window ? true : false;
		}

		/*!***********************************************************************
		*  @brief      �V�X�e���S�̂̐ݒ��ύX�����Ƃ�, �܂��̓|���V�[�ݒ肪�ύX���ꂽ�Ƃ��ɑS�Ă̍ŏ�ʃE�B���h�E�ɑ��M����郁�b�Z�[�W
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		virtual bool OnSettingChanged()
		{
			return true;
		}

		/*----------------------------------------------------------------------
		*  @brief : This function is called by the window size changed
		*----------------------------------------------------------------------*/
		virtual bool OnSizeChanged(const CoreWindowPtr& window, const gu::uint32 width, const gu::uint32 height);

		#pragma endregion 

		#pragma region Public Property

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		CoreWindowMessageHandler();

		virtual ~CoreWindowMessageHandler();
		
		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor


		#pragma endregion

		#pragma region Protected Function
		#pragma endregion

		#pragma region Protected Property
		/*! @brief ���T�C�Y��*/
		bool _isResizing = false;
		#pragma endregion
		
	};
}
#endif
