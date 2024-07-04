//////////////////////////////////////////////////////////////////////////////////
///             @file   GeneralWindow.hpp
///             @brief  �E�B���h�E���쐬����N���X
///             @author Toide Yutaro
///             @date   2023_08_31
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef CORE_PLATFORM_APPLICATION_HPP
#define CORE_PLATFORM_APPLICATION_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "CoreCommonState.hpp"
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::core
{
	class CoreWindowMessageHandler;
	class CoreWindow;
	class PlatformCommand;

	/****************************************************************************
	*				  			    PlatformApplication
	****************************************************************************/
	/* @brief     This class is the window list manager.
	*****************************************************************************/
	class PlatformApplication 
	{
	public:
		#pragma region Public Function
		/* @brief : This application creator is determined in according to platform macro*/
		static gu::SharedPointer<PlatformApplication> Create();

		/*!***********************************************************************
		*  @brief      �V�K�̃E�B���h�E�C���X�^���X���쐬���܂�. �����ł̓Z�b�g�A�b�v�͍s���܂���
		*  @param[in]  void
		*  @return     SharedPointer<core::CoreWindow> �E�B���h�E
		**************************************************************************/
		virtual gu::SharedPointer<CoreWindow> MakeWindow() = 0;

		virtual gu::SharedPointer<PlatformCommand> MakeCommand() = 0;
		
		/*!***********************************************************************
		*  @brief      �w��̃E�B���h�E�����ۂɍ쐬��, �Z�b�g�A�b�v����܂ōs���܂� (���������ɌĂяo���K�v)
		*  @param[in]  const gu::SharedPointer<CoreWindow>& window
		*  @param[in]  const CoreWindowDesc& window�̐ݒ荀��
		*  @param[in]  const gu::SharedPointer<CoreWindow>& �e�̃E�B���h�E
		*  @return     SharedPointer<core::CoreWindow> �E�B���h�E
		**************************************************************************/
		virtual void SetUpWindow(const gu::SharedPointer<CoreWindow>& window, const CoreWindowDesc& desc, const gu::SharedPointer<core::CoreWindow>& parentWindow = nullptr) = 0;

		/* @brief : This function pumps window message, when you are returned the true, you accept the message. */
		virtual bool PumpMessage() = 0;

		/* @brief : Is platform application is quited.*/
		virtual bool IsQuit() const = 0;

		#pragma endregion

		#pragma region Public Property
		static constexpr wchar_t* APPLICATION_NAME = L"PPP Game Window";

		virtual void* GetInstanceHandle() const noexcept = 0;

		#pragma region Monitor
		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : return the monitor DPI
		-----------------------------------------------------------------*/
		virtual gu::int32 GetMonitorDPI(const MonitorInfo& monitorInfo) const = 0;

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : return the high dpi mode
		-----------------------------------------------------------------*/
		virtual bool EnableHighDPIAwareness() const = 0;

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : Acquire the dpi zoom factor at pixel point (x,y)
		-----------------------------------------------------------------*/
		virtual float GetDPIScaleFactorAtPixelPoint(const float x, const float y) const = 0;

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : Return the work area without including the task bar
		-----------------------------------------------------------------*/
		virtual Rectangle GetWorkArea(const Rectangle& window) = 0;

		/*---------------------------------------------------------------
		�@�@�@�@�@@brief : Acquire the monitor list 
		-----------------------------------------------------------------*/
		virtual void GetMonitorsInfo(gu::DynamicArray<core::MonitorInfo>& monitorInfo) const = 0;

		/*!***********************************************************************
		*  @brief      Message�n���h�����擾����
		*  @param[in]  void
		*  @return     gu::SharedPointer<CoreWindowMessageHandler>
		**************************************************************************/
		__forceinline gu::SharedPointer<CoreWindowMessageHandler> GetMessageHandler() const
		{
			return _messageHandler;
		}

		/*!***********************************************************************
		*  @brief      Message�n���h����ݒ肷��
		*  @param[in]  gu::SharedPointer<CoreWindowMessageHandler>
		*  @return     void
		**************************************************************************/
		virtual void SetMessageHandler(const gu::SharedPointer<CoreWindowMessageHandler>& messageHandler)
		{
			_messageHandler = messageHandler;
		}

		#pragma endregion Monitor
		#pragma endregion

		#pragma region Public Operator

		#pragma endregion

		#pragma region Public Constructor and Destructor
		virtual ~PlatformApplication();

		#pragma endregion

	protected:
		#pragma region Protected Constructor and Destructor

		#pragma endregion

		#pragma region Protected Function

		#pragma endregion

		PlatformApplication();

		virtual bool SetHighDPIMode() = 0;

		#pragma region Protected Property
		/*! @brief Windows�̃��b�Z�[�W��~���Ă����n���h��*/
		gu::SharedPointer<CoreWindowMessageHandler> _messageHandler = nullptr;
		#pragma endregion
	};
}
#endif