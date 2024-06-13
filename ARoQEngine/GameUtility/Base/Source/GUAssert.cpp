//////////////////////////////////////////////////////////////////////////////////
//              Title:  GUAssert.hpp
//            Content:  エラーを表示するために使用するクラス. 参考はUnreal engineを使用
///                     Check, Verify, Ensureがあります. 
///           @author Toide Yutaro
///           @date   2023_10_25
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/GUAssert.hpp"
#include <iostream>
#include <string>

// platform毎の処理
#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#endif

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
	-           メッセージ出力 (出力部分に表示)
	---------------------------------------------------------------------*/
	OutputErrorLog(expression, fileName, lineNumber, format);

	/*-------------------------------------------------------------------
	-     今後追加: クラッシュレポートを表示する
	---------------------------------------------------------------------*/

	/*-------------------------------------------------------------------
	-     処理を強制終了するようにする.
	---------------------------------------------------------------------*/
	return true;
}

bool ErrorLogger::EnsureFailed(const char* expression, const char* fileName, const int lineNumber, const char* format)
{
	/*-------------------------------------------------------------------
	-           メッセージ出力 (出力部分に表示)
	---------------------------------------------------------------------*/
	OutputErrorLog(expression, fileName, lineNumber, format);

	/*-------------------------------------------------------------------
	-     今後追加: クラッシュレポートを表示する
	---------------------------------------------------------------------*/

	return true;
}

/****************************************************************************
*                       OutputErrorLog
****************************************************************************/
/* @fn        void Debugger::OutputErrorLog(const char* expression, const char* fileName, const int lineNumber, bool isEnsure, const char* descriptionFormat, const va_list descriptionArguments)
*
*  @brief     出力部分にエラー文を出力します
*
*  @param[in] const char* expression : Checkに使用した真偽値評価文
*  @param[in] const char* fileName : ファイル名
*  @param[in] const int lineNumber : 行番号
*
*  @return 　　void
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
	else
	{
		errorLog.append("\n");
	}

#if defined(_WIN32) || defined(_WIN64)
	OutputDebugStringA(errorLog.c_str());
#else
	std::cout << errorLog << std::endl;
#endif
}

void ErrorLogger::PutDebugBreak()
{
#if defined(_WIN32) || defined(_WIN64)
	DebugBreak();
#else

#endif
}

void ErrorLogger::ExecuteCrash()
{
	exit(1);
}
#pragma endregion Main Function