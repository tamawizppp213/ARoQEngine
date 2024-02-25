//////////////////////////////////////////////////////////////////////////////////
///             @file   Assert.hpp
///             @brief  エラーを表示するために使用するクラス. 参考はUnreal engineを使用
///                     Check, Verify, Ensureがあります. 
///                     Check  : Debugのみ起動します. 式がfalseになったら処理を完全に止めます, 続行すると強制終了.
///                     Verify : DebugだろうがReleaseだろうが式がfalseになったら実行を停止します.
///                     Ensure : Debugのときはブレークを発生させて実行を中断するが, そのまま継続してプログラムを実行し続けることが出来ます.
///                     Windowsのみ現状止まります.
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
//                         　　　　　Class
//////////////////////////////////////////////////////////////////////////////////

namespace gu
{
	/****************************************************************************
	*				  			ErrorLogger
	*************************************************************************//**
	*  @class     ErrorLogger
	*  @brief     Errorを表示するクラス
	*****************************************************************************/
	class ErrorLogger
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : 致命的なバグに使用します. ブレークポイント発生後処理を止めます.*/
		static bool CheckOrVerifyFailed(const char* expression, const char* fileName, const int lineNumber, const char* format);

		/* @brief : 致命的でないバグに使用します. ブレークポイント発生後, 処理を止めません.*/
		static bool EnsureFailed(const char* expression, const char* fileName, const int lineNumber, const char* format);
		
		// @brief : デバッグ用にブレークポイントを追加します
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
			Check : Debugのみ起動します. 式がfalseになったら処理を完全に止めます.
	-----------------------------------------------------------------*/
#if _DEBUG
#ifndef Check
	// Checkは式のみをLogに追加します.
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
	// Checkfでは通常の式評価に加え, エラー文章を追加してくれます. 
	#define Checkf(expression, format)\
	{\
		if(!(expression) && gu::ErrorLogger::CheckOrVerifyFailed(#expression, __FILE__, __LINE__, format)) \
		{                 \
			gu::ErrorLogger::PutDebugBreak(); \
            exit(1); \
		}\
    }
#endif
// Release時の対応
#else 
	// Release時は何もしない.
#ifndef Check
	#define Check(expression)
#endif
#ifndef Checkf
	#define Checkf(expression, format)
#endif
#endif

	/*---------------------------------------------------------------
			Verify : DebugだろうがReleaseだろうが式がfalseになったら実行を停止します.
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
	// Vefifyfでは通常の式評価に加え, エラー文章を追加してくれます. 
	#define Confirmf(expression, format)\
	{\
		if(!(expression) && gu::ErrorLogger::CheckOrVerifyFailed(#expression, __FILE__, __LINE__, format)) \
		{                 \
			gu::ErrorLogger::PutDebugBreak(); \
		}\
    }
#endif

	/*---------------------------------------------------------------
		Ensure : Debugのときはブレークを発生させて実行を中断する
	-----------------------------------------------------------------*/
#if _DEBUG
#ifndef Ensure
	// Ensureは式のみをLogに追加します.
	#define Ensure(expression) \
    {\
		if(!(expression) && gu::ErrorLogger::EnsureFailed(#expression, __FILE__, __LINE__, nullptr)) \
		{                 \
			gu::ErrorLogger::PutDebugBreak(); \
		}\
    }
#endif

#ifndef Ensuref
	// Ensurefでは通常の式評価に加え, エラー文章を追加してくれます. 
	#define Ensuref(expression, format)\
	{\
		if(!(expression) && gu::ErrorLogger::EnsureVerifyFailed(#expression, __FILE__, __LINE__, format)) \
		{                 \
			gu::ErrorLogger::PutDebugBreak(); \
		}\
    }
#endif
// Release時の対応
#else 
	// Release時は何もしない.
#ifndef Ensure
	#define Ensure(expression)
#endif
#ifndef Ensuref
	#define Ensuref(expression, format)
#endif
#endif

#endif