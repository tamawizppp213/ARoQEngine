//////////////////////////////////////////////////////////////////////////////////
///  @file   GUAssert.hpp
///  @brief  �G���[��\�����邽�߂Ɏg�p����N���X. �Q�l��Unreal engine���g�p @n
///          Check, Confirm, Ensure������܂�. @n
///          @b Check   : Debug�̂݋N�����܂�. ����false�ɂȂ����珈�������S�Ɏ~�߂܂�, ���s����Ƌ����I��. @n
///          @b Confirm : Debug���낤��Release���낤������false�ɂȂ�������s���~���܂�. @n
///          @b Ensure  : Debug�̂Ƃ��̓u���[�N�𔭐������Ď��s�𒆒f���邪, ���̂܂܌p�����ăv���O���������s�������邱�Ƃ��o���܂�.
///  
///  @note   @b Check   : Debug�̂݋N�����܂�. ����false�ɂȂ����珈�������S�Ɏ~�߂܂�, ���s����Ƌ����I��. @n
///          @b Confirm : Debug���낤��Release���낤������false�ɂȂ�������s���~���܂�. @n
///          @b Ensure  : Debug�̂Ƃ��̓u���[�N�𔭐������Ď��s�𒆒f���邪, ���̂܂܌p�����ăv���O���������s�������邱�Ƃ��o���܂�.         
///  @attention ����Windows�̂�Assert�Ŏ~�܂�܂�
///  @author Toide Yutaro
///  @date   2023_10_25
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
	****************************************************************************/
	/* @class     ErrorLogger
	*   @brief     Error��\������N���X
	*****************************************************************************/
	class ErrorLogger
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		
		/*!----------------------------------------------------------------------
		*  @brief �v���I�ȃo�O���������ۂɎg�p���܂�. 
		*         �u���[�N�|�C���g������Ɍ㑱�̏������s�킸, ���̂܂ܒ�~���܂�.
		*----------------------------------------------------------------------*/
		static bool CheckOrVerifyFailed(const char* expression, const char* fileName, const int lineNumber, const char* format);

		/*!----------------------------------------------------------------------
		*  @brief �v���I�łȂ��o�O�Ɏg�p���܂�.
		*         �u���[�N�|�C���g������Ɍ㑱�̏��������̂܂܎��s���܂�. .
		----------------------------------------------------------------------*/
		static bool EnsureFailed(const char* expression, const char* fileName, const int lineNumber, const char* format);
		
		/*! @brief : �f�o�b�O�p�Ƀu���[�N�|�C���g��ǉ����܂�*/
		static void PutDebugBreak();

		// @brief : �����I������ꍇ�Ɏg���܂�
		static void ExecuteCrash();
		/****************************************************************************
		**                Public Property
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
		**                Private Property
		*****************************************************************************/
	};
}
	/*---------------------------------------------------------------
			Check : Debug�̂݋N�����܂�. ����false�ɂȂ����珈�������S�Ɏ~�߂܂�.
	-----------------------------------------------------------------*/
#if _DEBUG
	#ifndef Check
		/*!***********************************************************************
		*  @brief      Debug�̂݋N�����܂�. ����false�ɂȂ����珈�������S�Ɏ~�߂܂�, ���s����Ƌ����I��
		*  @param[in]  ������
		**************************************************************************/
		#define Check(expression) \
		{\
			if(!(expression) && gu::ErrorLogger::CheckOrVerifyFailed(#expression, __FILE__, __LINE__, nullptr)) \
			{                 \
				gu::ErrorLogger::PutDebugBreak();\
				gu::ErrorLogger::ExecuteCrash();\
			}\
		}
	#endif

	#ifndef Checkf
		/*!***********************************************************************
		*  @brief      Debug�̂݋N�����܂�. �G���[���̕��͂��o�͂�����false�ɂȂ����珈�������S�Ɏ~�߂܂�
		*  @param[in]  ������
		*  @param[in]  �G���[���ɕ\�����镶��
		**************************************************************************/
		#define Checkf(expression, format)\
		{\
			if(!(expression) && gu::ErrorLogger::CheckOrVerifyFailed(#expression, __FILE__, __LINE__, format)) \
			{                 \
				gu::ErrorLogger::PutDebugBreak(); \
				gu::ErrorLogger::ExecuteCrash();\
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
			Confirm : Debug���낤��Release���낤������false�ɂȂ�������s���~���܂�.
	-----------------------------------------------------------------*/
#ifndef Confirm
	/*!***********************************************************************
	*  @brief      Debug���낤��Release���낤������false�ɂȂ�������s���~���܂�.
	*  @param[in]  ������
	**************************************************************************/
	#define Confirm(expression)\
    {\
		if(!(expression) && gu::ErrorLogger::CheckOrVerifyFailed(#expression, __FILE__, __LINE__, nullptr)) \
		{                 \
			gu::ErrorLogger::PutDebugBreak(); \
			gu::ErrorLogger::ExecuteCrash();\
		}\
    }
#endif
#ifndef Confirmf
	/*!***********************************************************************
	*  @brief      Debug���낤��Release���낤������false�ɂȂ�������s���~���܂�.
	*  @param[in]  ������
	*  @param[in]  �G���[���ɕ\�����镶��
	**************************************************************************/
	#define Confirmf(expression, format)\
	{\
		if(!(expression) && gu::ErrorLogger::CheckOrVerifyFailed(#expression, __FILE__, __LINE__, format)) \
		{                 \
			gu::ErrorLogger::PutDebugBreak(); \
			gu::ErrorLogger::ExecuteCrash();\
		}\
    }
#endif

	/*---------------------------------------------------------------
		Ensure : Debug�̂Ƃ��̓u���[�N�𔭐������Ď��s�𒆒f����
	-----------------------------------------------------------------*/
#if _DEBUG
	#ifndef Ensure
		/*!***********************************************************************
		*  @brief      Debug�̂Ƃ��̓u���[�N�𔭐������Ď��s�𒆒f���邪, ���̂܂܌p�����ăv���O���������s�������邱�Ƃ��o���܂�.
		*  @param[in]  ������
		**************************************************************************/
		#define Ensure(expression) \
		{\
			if(!(expression) && gu::ErrorLogger::EnsureFailed(#expression, __FILE__, __LINE__, nullptr)) \
			{                 \
				gu::ErrorLogger::PutDebugBreak(); \
			}\
		}
	#endif

	#ifndef Ensuref
		/*!***********************************************************************
		*  @def        Ensuref(expression, format)
		*  @brief      Debug�̂Ƃ��̓u���[�N�𔭐������Ď��s�𒆒f���邪, ���̂܂܌p�����ăv���O���������s�������邱�Ƃ��o���܂�.
		*  @param[in]  ������
		*  @param[in]  �G���[���ɕ\�����镶��
		**************************************************************************/
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
		/*!***********************************************************************
		*  @def        Ensure(expression)
		*  @brief      Debug�̂Ƃ��̓u���[�N�𔭐������Ď��s�𒆒f���邪, ���̂܂܌p�����ăv���O���������s�������邱�Ƃ��o���܂�.
		*  @param[in]  ������
		**************************************************************************/
		#define Ensure(expression)
	#endif
	#ifndef Ensuref
		/*!***********************************************************************
		*  @def        Ensuref(expression, format)
		*  @brief      Debug�̂Ƃ��̓u���[�N�𔭐������Ď��s�𒆒f���邪, ���̂܂܌p�����ăv���O���������s�������邱�Ƃ��o���܂�.
		*  @param[in]  ������
		*  @param[in]  �G���[���ɕ\�����镶��
		**************************************************************************/
		#define Ensuref(expression, format)
	#endif
#endif

#endif