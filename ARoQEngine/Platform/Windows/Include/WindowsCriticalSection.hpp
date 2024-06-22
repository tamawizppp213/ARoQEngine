//////////////////////////////////////////////////////////////////////////////////
///  @file   WindowsCriticalSection.hpp
///  @brief  OSにおいて, データの競合を防ぐためのクリティカルセクションを提供します.@n
///          一つのプロセス内で使用でき, プロセス間で共有することは出来ない.
///  @author toide
///  @date   2024/06/21 23:51:23
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef WINDOWS_CRITICAL_SECTION_HPP
#define WINDOWS_CRITICAL_SECTION_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUClassUtility.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::windows
{
	namespace details::criticalSection
	{
		/* @brief Windowsをヘッダに追加したくないだけ*/
		struct Info
		{
			/* @brief 基本メンバ変数自体は使用しない. */
			void* Opaque1[1] = {};

			/*! @brief 基本メンバ変数自体は使用しない*/
			long Opaque2[2] = {};

			/*! @brief 基本メンバ変数自体は使用しない*/
			void* Opaque3[3] = {};
		};
	}

	/****************************************************************************
	*				  			   WindowsCriticalSection
	*************************************************************************//**
	/*  @class     WindowsCriticalSection
	*   @brief     OSにおいて, データの競合を防ぐためのクリティカルセクションを提供します.@n
	*              一つのプロセス内で使用でき, プロセス間で共有することは出来ない.
	*****************************************************************************/
	class CoreCriticalSection : public gu::NonCopyable
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     スレッドの排他制御を行うために, 呼び出しスレッドをロックします. @n
		*             他のスレッドがロックしている場合は, ロックが解除されるまで待機します.
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void Lock();

		/*!**********************************************************************
		*  @brief     スレッドの排他制御を行うために, 呼び出しスレッドをロックします. @n
		*             他のスレッドがロックしている場合は, 即座にfalseを返します.
		*  @param[in] void
		*  @return    bool ロックが成功した場合はtrue
		* ************************************************************************/
		bool TryLock();

		/*!**********************************************************************
		*  @brief     Lock関数でロックしたスレッドを解放し, 内部の変数を読み書き可能にします
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void Unlock();

		#pragma endregion 

		#pragma region Public Property

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief デフォルトコンストラクタ*/
		CoreCriticalSection();

		/*! @brief デストラクタ*/
		~CoreCriticalSection();

		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor

		#pragma endregion 

		#pragma region Protected Function

		#pragma endregion 

		#pragma region Protected Property

		#pragma endregion

	private:
		#pragma region Private Constructor and Destructor

		#pragma endregion

		#pragma region Private Function

		#pragma endregion 

		#pragma region Private Property
		details::criticalSection::Info _criticalSectionInfo = {};

		#pragma endregion 

	};
}

#endif