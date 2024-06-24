//////////////////////////////////////////////////////////////////////////////////
///  @file   WindowsHardware.hpp
///  @brief  CPUやGPUなどのハードウェア情報を取得するクラスです.
///  @author toide
///  @date   2024/06/23 18:40:50
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef WINDOWS_HARDWARE_HPP
#define WINDOWS_HARDWARE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUType.hpp"
#include "GameUtility/Base/Include/GUString.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::windows
{
	/****************************************************************************
	*				  			   WindowsHardware
	*************************************************************************//**
	/*   @brief  CPUやGPUなどのハードウェア情報を取得するクラスです.
	*****************************************************************************/
	class CoreHardware
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     物理コア数を取得します
		*  @param[in] void
		*  @return    gu::uint32 物理コア数
		*************************************************************************/
		static gu::uint32 GetPhysicalCoreCount();

		/*!**********************************************************************
		*  @brief     論理コア数を取得します
		*  @param[in] void
		*  @return    gu::uint32 物理コア数
		*************************************************************************/
		static gu::uint32 GetLogicalCoreCount();

		/*!**********************************************************************
		*  @brief     CPUのベンダー名を取得します
		*  @param[in] void
		*  @return    tstring ベンダー名
		*************************************************************************/
		static gu::tstring GetCPUVendorName();

		/*!**********************************************************************
		*  @brief     CPUのモデル名を取得します
		*  @param[in] void
		*  @return    tstring モデル名
		*************************************************************************/
		static gu::tstring GetCPUModelName();

		/*!**********************************************************************
		*  @brief     CPUのクロック周波数を取得します
		*  @param[in] void
		*  @return    gu::uint32 クロック周波数
		*************************************************************************/
		static gu::uint32 GetCPUClockFrequency();

		/*!**********************************************************************
		*  @brief     CPUのキャッシュラインサイズを取得します. キャッシュに格納する単位データの大きさ.[Byte]
		*  @param[in] void
		*  @return    gu::uint32 キャッシュラインサイズ
		*************************************************************************/
		static gu::uint32 GetCPUCacheLineSize();

		/*!**********************************************************************
		*  @brief     GPUのベンダー名を取得します
		*  @param[in] void
		*  @return    tstring ベンダー名
		*************************************************************************/
		static gu::tstring GetGPUVendorName();

		/*!**********************************************************************
		*  @brief     GPUのモデル名を取得します
		*  @param[in] void
		*  @return    tstring モデル名
		*************************************************************************/
		static gu::tstring GetGPUModelName();

		#pragma endregion 

		#pragma region Public Property

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor

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
		
		/*! @brief CPUのベンダー名*/
		static gu::tstring CPU_VENDER_NAME;

		/*! @brief CPUのブランド名*/
		static gu::tstring CPU_MODEL_NAME;

		/*! @brief GPUのベンダー名*/
		static gu::tstring GPU_VENDER_NAME;

		#pragma endregion 

	};
}

#endif