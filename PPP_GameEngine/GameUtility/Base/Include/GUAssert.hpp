//////////////////////////////////////////////////////////////////////////////////
///             @file   Assert.hpp
///             @brief  �G���[��\�����邽�߂Ɏg�p����N���X. �Q�l��Unreal engine���g�p
///                     Check, Verify, Ensure������܂�. 
///                     Check  : Debug�̂݋N�����܂�. ����false�ɂȂ����珈�������S�Ɏ~�߂܂�, ���s����Ƌ����I��.
///                     Verify : Debug���낤��Release���낤������false�ɂȂ�������s���~���܂�.
///                     Ensure : Debug�̂Ƃ��̓u���[�N�𔭐������Ď��s�𒆒f���邪, ���̂܂܌p�����ăv���O���������s�������邱�Ƃ��o���܂�.
///                     Windows�̂݌���~�܂�܂�.
///             @author Toide Yutaro
///             @date   2023_10_25
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_ASSERT_HPP
#define GU_ASSERT_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         �@�@�@�@�@Class
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
		
		// @brief : �f�o�b�O�p�Ƀu���[�N�|�C���g��ǉ����܂�
		static void PutDebugBreak();
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

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
			gu::ErrorLogger::PutDebugBreak();\
            exit(1); \
		}\
    }
#endif

#ifndef Checkf
	// Checkf�ł͒ʏ�̎��]���ɉ���, �G���[���͂�ǉ����Ă���܂�. 
	#define Checkf(expression, format)\
	{\
		if(!(expression) && gu::ErrorLogger::CheckOrVerifyFailed(#expression, __FILE__, __LINE__, format)) \
		{                 \
			gu::ErrorLogger::PutDebugBreak(); \
            exit(1); \
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
	#define Confirm(expression)\
    {\
		if(!(expression) && gu::ErrorLogger::CheckOrVerifyFailed(#expression, __FILE__, __LINE__, nullptr)) \
		{                 \
			gu::ErrorLogger::PutDebugBreak(); \
		}\
    }
#endif
#ifndef Verifyf
	// Vefifyf�ł͒ʏ�̎��]���ɉ���, �G���[���͂�ǉ����Ă���܂�. 
	#define Confirmf(expression, format)\
	{\
		if(!(expression) && gu::ErrorLogger::CheckOrVerifyFailed(#expression, __FILE__, __LINE__, format)) \
		{                 \
			gu::ErrorLogger::PutDebugBreak(); \
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
			gu::ErrorLogger::PutDebugBreak(); \
		}\
    }
#endif

#ifndef Ensuref
	// Ensuref�ł͒ʏ�̎��]���ɉ���, �G���[���͂�ǉ����Ă���܂�. 
	#define Ensuref(expression, format)\
	{\
		if(!(expression) && gu::ErrorLogger::EnsureVerifyFailed(#expression, __FILE__, __LINE__, format)) \
		{                 \
			gu::ErrorLogger::PutDebugBreak(); \
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