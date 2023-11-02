//////////////////////////////////////////////////////////////////////////////////
///             @file   Assert.hpp
///             @brief  �G���[��\�����邽�߂Ɏg�p����N���X. �Q�l��Unreal engine���g�p
///                     Check, Verify, Ensure������܂�. 
///             @author Toide Yutaro
///             @date   2023_10_25
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef ASSERT_HPP
#define ASSERT_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "Platform/Core/Include/CorePlatformCommand.hpp"
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////

namespace gu
{
	/****************************************************************************
	*				  			ErrorLogger
	*************************************************************************//**
	*  @class     ErrorLogger
	*  @brief     Error��\������N���X
	*****************************************************************************/
	class ErrorLogger
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : �v���I�ȃo�O�Ɏg�p���܂�. �u���[�N�|�C���g�����㏈�����~�߂܂�.*/
		static bool CheckOrVerifyFailed(const char* expression, const char* fileName, const int lineNumber, const char* format);

		/* @brief : �v���I�łȂ��o�O�Ɏg�p���܂�. �u���[�N�|�C���g������, �������~�߂܂���.*/
		static bool EnsureFailed(const char* expression, const char* fileName, const int lineNumber, const char* format);
		
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		static std::shared_ptr<platform::core::PlatformCommand> DebugCommand;

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		static void OutputErrorLog(const char* expression, const char* fileName, const int lineNumber, const char* descriptionFormat);

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
	};
}
	/*---------------------------------------------------------------
			Check : Debug�̂݋N�����܂�. ����false�ɂȂ����珈�������S�Ɏ~�߂܂�.
	-----------------------------------------------------------------*/
#if _DEBUG
#ifndef Check
	// Check�͎��݂̂�Log�ɒǉ����܂�.
	#define Check(expression) \
    {\
		if(!(expression) && gu::ErrorLogger::CheckOrVerifyFailed(#expression, __FILE__, __LINE__, nullptr)) \
		{                 \
			if(gu::ErrorLogger::DebugCommand){gu::ErrorLogger::DebugCommand->PutDebugBreak();} \
		}\
    }
#endif

#ifndef Checkf
	// Checkf�ł͒ʏ�̎��]���ɉ���, �G���[���͂�ǉ����Ă���܂�. 
	#define Checkf(expression, format)\
	{\
		if(!(expression) && gu::ErrorLogger::CheckOrVerifyFailed(#expression, __FILE__, __LINE__, format)) \
		{                 \
			DebugBreak(); \
		}\
    }
#endif
// Release���̑Ή�
#else 
	// Release���͉������Ȃ�.
#ifndef Check
	#define Check(expression)
#endif
#ifndef Checkf
	#define Checkf(expression, format)
#endif
#endif

	/*---------------------------------------------------------------
			Verify : Debug���낤��Release���낤������false�ɂȂ�������s���~���܂�.
	-----------------------------------------------------------------*/
#ifndef Verify 
	#define Verify(expression)\
    {\
		if(!(expression) && gu::ErrorLogger::CheckOrVerifyFailed(#expression, __FILE__, __LINE__, nullptr)) \
		{                 \
			if(gu::ErrorLogger::DebugCommand){gu::ErrorLogger::DebugCommand->PutDebugBreak();} \
		}\
    }
#endif
#ifndef Verifyf
	// Vefifyf�ł͒ʏ�̎��]���ɉ���, �G���[���͂�ǉ����Ă���܂�. 
	#define Verifyf(expression, format)\
	{\
		if(!(expression) && gu::ErrorLogger::CheckOrVerifyFailed(#expression, __FILE__, __LINE__, format)) \
		{                 \
			if(gu::ErrorLogger::DebugCommand){gu::ErrorLogger::DebugCommand->PutDebugBreak();} \
		}\
    }
#endif

	/*---------------------------------------------------------------
		Ensure : Debug�̂Ƃ��̓u���[�N�𔭐������Ď��s�𒆒f����
	-----------------------------------------------------------------*/
#if _DEBUG
#ifndef Ensure
	// Ensure�͎��݂̂�Log�ɒǉ����܂�.
	#define Ensure(expression) \
    {\
		if(!(expression) && gu::ErrorLogger::EnsureFailed(#expression, __FILE__, __LINE__, nullptr)) \
		{                 \
			DebugBreak(); \
		}\
    }
#endif

#ifndef Ensuref
	// Ensuref�ł͒ʏ�̎��]���ɉ���, �G���[���͂�ǉ����Ă���܂�. 
	#define Ensuref(expression, format)\
	{\
		if(!(expression) && gu::ErrorLogger::EnsureVerifyFailed(#expression, __FILE__, __LINE__, format)) \
		{                 \
			DebugBreak(); \
		}\
    }
#endif
// Release���̑Ή�
#else 
	// Release���͉������Ȃ�.
#ifndef Ensure
	#define Ensure(expression)
#endif
#ifndef Ensuref
	#define Ensuref(expression, format)
#endif
#endif

#endif