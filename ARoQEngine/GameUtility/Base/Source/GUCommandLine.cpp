//////////////////////////////////////////////////////////////////////////////////
///             @file   GUCommandLine.cpp
///             @brief  temp
///             @author toide
///             @date   2024/03/02 1:38:12
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/GUCommandLine.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gu;

bool    CommandLine::_hasInitialized = false;
tstring CommandLine::_commandLine    = SP("");

//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////

#pragma region Constructor and Destructor 

#pragma endregion Constructor and Destructor

#pragma region Main Function
/****************************************************************************
*                       BuildFromArgumentVector
****************************************************************************/
/* @brief     main関数で使われるargv, argcを使ってコマンドラインを構築します.

*  @param[in] const int32   argumentCount  : コマンドライン引数の数
*  @param[in] const wchar** argumentVector : コマンドラインの文字配列 (argumentCount分だけ) 
*
*  @return 　　tstring
*****************************************************************************/
tstring CommandLine::BuildFromArgumentVector(const int32 argumentCount, const wchar** argumentVector)
{
	/*----------------------------------------------------------------------
	*  最初の実行ファイル名に対する引数を無視しつつ, 各引数をstringに渡す
	*----------------------------------------------------------------------*/
	tstring result = SP("");
	for (int32 i = 1; i < argumentCount; ++i)
	{
		wstring argument(argumentVector[i]);

		// 空白文字が含まれとダブルクオーテーションが含まれていなければargumentに足す
		if (argument.Contains(L" ", true) && !argument.Contains(L"\"", true))
		{
			const auto equalIndex = argument.Find(L"=");

			// equalは最初に現れることはないので(引数名が分からないため)、1以降のindexで調査
			if (equalIndex > 0 && argument.Find(L" ") > equalIndex)
			{
				argument = argument.Left(equalIndex + 1) + L"\"" + argument.SubString(equalIndex + 1) + L"\"";
			}
			else
			{
				argument = wstring(L"\"") + argument + L"\"";
			}
		}

		// コマンドを後ろに追加する
		result.Reserve(result.Size() + argument.Size() + 1);
		for (int32 j = 0; j < argument.Size(); ++j)
		{
			result += static_cast<tchar>(argument[j]);
		}

		// 各引数の間には必ず空白を追加する
		if (i + 1 < argumentCount)
		{
			result += L" ";
		}
	}

	return result;
}

/****************************************************************************
*                       BuildFromArgumentVector
****************************************************************************/
/* @brief     main関数で使われるargv, argcを使ってコマンドラインを構築します.

*  @param[in] const int32   argumentCount  : コマンドライン引数の数
*  @param[in] const char** argumentVector : コマンドラインの文字配列 (argumentCount分だけ)
*
*  @return 　　tstring
*****************************************************************************/
tstring CommandLine::BuildFromArgumentVector(const int32 argumentCount, const char** argumentVector)
{
	/*----------------------------------------------------------------------
	*  最初の実行ファイル名に対する引数を無視しつつ, 各引数をstringに渡す
	*----------------------------------------------------------------------*/
	tstring result = SP("");
	for (int32 i = 1; i < argumentCount; ++i)
	{
		string argument(argumentVector[i]);

		// 空白文字が含まれとダブルクオーテーションが含まれていなければargumentに足す
		if (argument.Contains(" ", true) && !argument.Contains("\"", true))
		{
			const auto equalIndex = argument.Find("=");

			// equalは最初に現れることはないので(引数名が分からないため)、1以降のindexで調査
			if (equalIndex > 0 && argument.Find(" ") > equalIndex)
			{
				argument = argument.Left(equalIndex + 1) + "\"" + argument.SubString(equalIndex + 1) + "\"";
			}
			else
			{
				argument = string("\"") + argument + "\"";
			}
		}

		// コマンドを後ろに追加する
		result.Reserve(result.Size() + argument.Size() + 1);
		for (int32 j = 0; j < argument.Size(); ++j)
		{
			result += static_cast<tchar>(argument[j]);
		}

		// 各引数の間には必ず空白を追加する
		if (i + 1 < argumentCount)
		{
			result += SP(" ");
		}
	}

	return result;
}

void CommandLine::SetUp(const tchar* newCommandLine)
{
	if (_hasInitialized)
	{
		Reset(); 
	}

	_commandLine    = newCommandLine;
	_hasInitialized = true;
}
#pragma endregion Main Function