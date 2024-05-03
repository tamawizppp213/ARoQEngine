//////////////////////////////////////////////////////////////////////////////////
/// @file   GUCommandLine.hpp
/// @brief  コマンドライン引数を保持しておくクラスです. @n
///         main(WinMain)関数が始まったタイミングで呼ばれます. @n
///         コマンドラインはApplication実行中常に同じものを使用する(インスタンスごとに変わったりはしない)ことから, @n
///         実装は全てStaticで完結しています. @n
/// @attention また, Parseを使用する場合は文字長に注意してください. むやみに増やすと遅くなります. (文字の比較が入るため)
/// @note      最大文字数は16384です.
/// @author toide
/// @date   2024/03/01 1:50:25
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_COMMAND_LINE_HPP
#define GU_COMMAND_LINE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GUType.hpp"
#include "GUString.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu
{
	/****************************************************************************
	*				  			   CommandLine
	*************************************************************************//**
	/*  @struct   CommandLine
	*   @brief    コマンドライン引数を保持しておくクラスです. @n
	*   @note     main(WinMain)関数が始まったタイミングでBuildFromArgumentVectorを呼んでください. @n
	*****************************************************************************/
	struct CommandLine final
	{
	public:
		#pragma region Public Function
		/*!***********************************************************************
		*  @brief      main関数で使われるargv, argcを使ってコマンドラインを構築します.
		*  @param[in]  const int32   : コマンドライン引数の数 (一般的にargcと定義されるもの)
		*  @param[in]  const wchar** : コマンドラインの文字配列 (argumentCount分だけ用意. 一般的にargvと定義される)
		*  @return     tstring : コマンドラインを一行にまとめた文字列 
		**************************************************************************/
		static tstring BuildFromArgumentVector(const int32 argumentCount, const wchar** argumentVector);
		
		/*!***********************************************************************
		*  @brief      main関数で使われるargv, argcを使ってコマンドラインを構築します.
		*  @param[in]  const int32   : コマンドライン引数の数 (一般的にargcと定義されるもの)
		*  @param[in]  const wchar** : コマンドラインの文字配列 (argumentCount分だけ用意. 一般的にargvと定義される)
		*  @return     tstring : コマンドラインを一行にまとめた文字列 
		**************************************************************************/
		static tstring BuildFromArgumentVector(const int32 argumentCount, const char8** argumentVector);

		/*!***********************************************************************
		*  @brief      通常のtchar文字列を使ってコマンドラインを設定します
		*  @param[in]  const tchar* : tcharポインタによるコマンドライン文字列
		*  @return     void
		**************************************************************************/
		static void SetUp(const tchar* newCommandLine);

		/*!***********************************************************************
		*  @brief      コマンドラインを後ろに追加します
		*  @param[in]  const tchar* appendString : 後ろに追加したい文字列
		*  @return     void 
		**************************************************************************/
		static void Append(const tchar* appendString) 
		{
			Confirmf(_commandLine.Size() + tstring(appendString).Size() < MAX_COMMAND_LINE_LENGTH, "Over MAX_COMMAND_LINE_LENGTH Error");
			_commandLine.Append(appendString); 
		}

		/*!***********************************************************************
		*  @brief      コマンドラインを初期化状態に戻します.
		*  @param[in]  void
		*  @return     void
		**************************************************************************/
		static void Reset() { _commandLine.ClearAll(); _hasInitialized = false; }

		#pragma endregion Public Function

		#pragma region Public Property
		// @brief : 最大文字長
		static constexpr uint32 MAX_COMMAND_LINE_LENGTH = 16384;

		/*!***********************************************************************
		*  @brief      コマンドラインの文字列自体を返します. ただし, コマンドラインの直接的な改変は出来ません.
		*  @param[in]  void
		*  @return     const tstring& コマンドライン文字列
		**************************************************************************/
		static const tstring& Get() { return _commandLine; }

		/*!***********************************************************************
		*  @brief      コマンドラインの文字列の先頭ポインタを返します
		*  @param[in]  void
		*  @return     const tchar* コマンドライン文字列
		**************************************************************************/
		static const tchar* CString() { return _hasInitialized ? _commandLine.CString() : nullptr; }

		/*!***********************************************************************
		*  @brief      初期化されているかを調べます
		*  @param[in]  void
		*  @return     const bool ： trueで初期化済みを示す.
		**************************************************************************/
		static bool HasInitialized() { return _hasInitialized; }

		#pragma endregion Public Property

		#pragma region Public Constructor and Destructor

		#pragma endregion Public Constructor and Destructor

	private:
		#pragma region Private Function
	
		#pragma endregion Private Function

		#pragma region Private Property
		/*! @brief 初期化済みか*/
		static bool _hasInitialized;

		/*! @brief 一行のコマンドラインを保存しておく文字列*/
		static tstring _commandLine;
		#pragma endregion Private Property
	};
}

#endif