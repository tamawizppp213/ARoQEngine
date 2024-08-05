//////////////////////////////////////////////////////////////////////////////////
///  @file   GUAssert.hpp
///  @brief  エラーを表示するために使用するクラス. 参考はUnreal engineを使用 @n
///          Check, Confirm, Ensureがあります. @n
///          @b Check   : Debugのみ起動します. 式がfalseになったら処理を完全に止めます, 続行すると強制終了. @n
///          @b Confirm : DebugだろうがReleaseだろうが式がfalseになったら実行を停止します. @n
///          @b Ensure  : Debugのときはブレークを発生させて実行を中断するが, そのまま継続してプログラムを実行し続けることが出来ます.
///  
///  @note   @b Check   : Debugのみ起動します. 式がfalseになったら処理を完全に止めます, 続行すると強制終了. @n
///          @b Confirm : DebugだろうがReleaseだろうが式がfalseになったら実行を停止します. @n
///          @b Ensure  : Debugのときはブレークを発生させて実行を中断するが, そのまま継続してプログラムを実行し続けることが出来ます.         
///  @attention 現状WindowsのみAssertで止まります
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
//                         　　　　　Class
//////////////////////////////////////////////////////////////////////////////////

namespace gu
{
	/****************************************************************************
	*				  			ErrorLogger
	****************************************************************************/
	/* @class     ErrorLogger
	*   @brief     Errorを表示するクラス
	*****************************************************************************/
	class ErrorLogger
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		
		/*!----------------------------------------------------------------------
		*  @brief 致命的なバグが生じた際に使用します. 
		*         ブレークポイント発生後に後続の処理を行わず, そのまま停止します.
		*----------------------------------------------------------------------*/
		static bool CheckOrVerifyFailed(const char* expression, const char* fileName, const int lineNumber, const char* format);

		/*!----------------------------------------------------------------------
		*  @brief 致命的でないバグに使用します.
		*         ブレークポイント発生後に後続の処理をそのまま実行します. .
		----------------------------------------------------------------------*/
		static bool EnsureFailed(const char* expression, const char* fileName, const int lineNumber, const char* format);
		
		/*! @brief : デバッグ用にブレークポイントを追加します*/
		static void PutDebugBreak();

		// @brief : 即時終了する場合に使います
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
			Check : Debugのみ起動します. 式がfalseになったら処理を完全に止めます.
	-----------------------------------------------------------------*/
#if _DEBUG
	#ifndef Check
		/*!***********************************************************************
		*  @brief      Debugのみ起動します. 式がfalseになったら処理を完全に止めます, 続行すると強制終了
		*  @param[in]  条件式
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
		*  @brief      Debugのみ起動します. エラー時の文章も出力しつつ式がfalseになったら処理を完全に止めます
		*  @param[in]  条件式
		*  @param[in]  エラー時に表示する文章
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
			Confirm : DebugだろうがReleaseだろうが式がfalseになったら実行を停止します.
	-----------------------------------------------------------------*/
#ifndef Confirm
	/*!***********************************************************************
	*  @brief      DebugだろうがReleaseだろうが式がfalseになったら実行を停止します.
	*  @param[in]  条件式
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
	*  @brief      DebugだろうがReleaseだろうが式がfalseになったら実行を停止します.
	*  @param[in]  条件式
	*  @param[in]  エラー時に表示する文章
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
		Ensure : Debugのときはブレークを発生させて実行を中断する
	-----------------------------------------------------------------*/
#if _DEBUG
	#ifndef Ensure
		/*!***********************************************************************
		*  @brief      Debugのときはブレークを発生させて実行を中断するが, そのまま継続してプログラムを実行し続けることが出来ます.
		*  @param[in]  条件式
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
		*  @brief      Debugのときはブレークを発生させて実行を中断するが, そのまま継続してプログラムを実行し続けることが出来ます.
		*  @param[in]  条件式
		*  @param[in]  エラー時に表示する文章
		**************************************************************************/
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
		/*!***********************************************************************
		*  @def        Ensure(expression)
		*  @brief      Debugのときはブレークを発生させて実行を中断するが, そのまま継続してプログラムを実行し続けることが出来ます.
		*  @param[in]  条件式
		**************************************************************************/
		#define Ensure(expression)
	#endif
	#ifndef Ensuref
		/*!***********************************************************************
		*  @def        Ensuref(expression, format)
		*  @brief      Debugのときはブレークを発生させて実行を中断するが, そのまま継続してプログラムを実行し続けることが出来ます.
		*  @param[in]  条件式
		*  @param[in]  エラー時に表示する文章
		**************************************************************************/
		#define Ensuref(expression, format)
	#endif
#endif

#endif