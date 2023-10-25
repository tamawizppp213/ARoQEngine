//////////////////////////////////////////////////////////////////////////////////
//              Title:  GUAssert.hpp
//            Content:  �G���[��\�����邽�߂Ɏg�p����N���X. �Q�l��Unreal engine���g�p
///                     Check, Verify, Ensure������܂�. 
///           @author Toide Yutaro
///           @date   2023_10_25
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/GUAssert.hpp"
#include <Windows.h>
#include <iostream>
#include <string>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gu;

//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Main Function
bool ErrorLogger::CheckOrVerifyFailed(const char* expression, const char* fileName, const int lineNumber, const char* format)
{
	/*-------------------------------------------------------------------
	-           ���b�Z�[�W�o�� (�o�͕����ɕ\��)
	---------------------------------------------------------------------*/
	OutputErrorLog(expression, fileName, lineNumber, format);

	/*-------------------------------------------------------------------
	-     ����ǉ�: �N���b�V�����|�[�g��\������
	---------------------------------------------------------------------*/

	/*-------------------------------------------------------------------
	-     �����������I������悤�ɂ���.
	---------------------------------------------------------------------*/
	return true;
}

bool ErrorLogger::EnsureFailed(const char* expression, const char* fileName, const int lineNumber, const char* format)
{
	/*-------------------------------------------------------------------
	-           ���b�Z�[�W�o�� (�o�͕����ɕ\��)
	---------------------------------------------------------------------*/
	OutputErrorLog(expression, fileName, lineNumber, format);

	/*-------------------------------------------------------------------
	-     ����ǉ�: �N���b�V�����|�[�g��\������
	---------------------------------------------------------------------*/

	return true;
}

/****************************************************************************
*                       OutputErrorLog
*************************************************************************//**
*  @fn        void Debugger::OutputErrorLog(const char* expression, const char* fileName, const int lineNumber, bool isEnsure, const char* descriptionFormat, const va_list descriptionArguments)
*
*  @brief     �o�͕����ɃG���[�����o�͂��܂�
*
*  @param[in] const char* expression : Check�Ɏg�p�����^�U�l�]����
*  @param[in] const char* fileName : �t�@�C����
*  @param[in] const int lineNumber : �s�ԍ�
*
*  @return �@�@void
*****************************************************************************/
void ErrorLogger::OutputErrorLog(const char* expression, const char* fileName, const int lineNumber,const char* descriptionFormat)
{
	std::string errorLog = "";
	errorLog.append("Assertion Failed ");

	errorLog.append(" in ");
	errorLog.append(fileName);
	errorLog.append(", line ");
	errorLog.append(std::to_string(lineNumber));
	errorLog.append("\n");
	
	errorLog.append(expression);

	if (descriptionFormat)
	{
		errorLog.append(", ");
		errorLog.append(descriptionFormat);
		errorLog.append("\n");
	}

	OutputDebugStringA(errorLog.c_str());
}
#pragma endregion Main Function