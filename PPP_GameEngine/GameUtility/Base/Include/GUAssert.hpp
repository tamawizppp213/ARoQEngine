//////////////////////////////////////////////////////////////////////////////////
///             @file   Assert.hpp
///             @brief  エラーを表示するために使用するクラス. 参考はUnreal engineを使用
///                     Check, Verify, Ensureがあります. 
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
			Check : Debugのみ起動します. 式がfalseになったら処理を完全に止めます.
	-----------------------------------------------------------------*/
#if _DEBUG
#ifndef Check
	// Checkは式のみをLogに追加します.
	#define Check(expression) \
    {\
		if(!(expression) && gu::ErrorLogger::CheckOrVerifyFailed(#expression, __FILE__, __LINE__, nullptr)) \
		{                 \
			if(gu::ErrorLogger::DebugCommand){gu::ErrorLogger::DebugCommand->PutDebugBreak();} \
		}\
    }
#endif

#ifndef Checkf
	// Checkfでは通常の式評価に加え, エラー文章を追加してくれます. 
	#define Checkf(expression, format)\
	{\
		if(!(expression) && gu::ErrorLogger::CheckOrVerifyFailed(#expression, __FILE__, __LINE__, format)) \
		{                 \
			DebugBreak(); \
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
	#define Verify(expression)\
    {\
		if(!(expression) && gu::ErrorLogger::CheckOrVerifyFailed(#expression, __FILE__, __LINE__, nullptr)) \
		{                 \
			if(gu::ErrorLogger::DebugCommand){gu::ErrorLogger::DebugCommand->PutDebugBreak();} \
		}\
    }
#endif
#ifndef Verifyf
	// Vefifyfでは通常の式評価に加え, エラー文章を追加してくれます. 
	#define Verifyf(expression, format)\
	{\
		if(!(expression) && gu::ErrorLogger::CheckOrVerifyFailed(#expression, __FILE__, __LINE__, format)) \
		{                 \
			if(gu::ErrorLogger::DebugCommand){gu::ErrorLogger::DebugCommand->PutDebugBreak();} \
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
			DebugBreak(); \
		}\
    }
#endif

#ifndef Ensuref
	// Ensurefでは通常の式評価に加え, エラー文章を追加してくれます. 
	#define Ensuref(expression, format)\
	{\
		if(!(expression) && gu::ErrorLogger::EnsureVerifyFailed(#expression, __FILE__, __LINE__, format)) \
		{                 \
			DebugBreak(); \
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