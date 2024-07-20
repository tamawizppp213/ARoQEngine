//////////////////////////////////////////////////////////////////////////////////
///  @file   WindowsHardware.cpp
///  @brief  実装の参考資料: https://learn.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2010/hskdteyh(v=vs.100)?redirectedfrom=MSDN
///  @author toide
///  @date   2024/06/23 20:22:10
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "Platform/Windows/Include/WindowsHardware.hpp"
#include "GameUtility/Memory/Include/GUMemory.hpp"
#include "GameUtility/Base/Include/GUStringConverter.hpp"
#include <Windows.h>
#include <intrin.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace platform::windows;
using namespace gu;

tstring CoreHardware::CPU_VENDER_NAME = SP("");
tstring CoreHardware::CPU_MODEL_NAME  = SP("");
tstring CoreHardware::GPU_VENDER_NAME = SP("");
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor

#pragma endregion Constructor and Destructor

#pragma region Public Function
/*!**********************************************************************
*  @brief     物理コア数を取得します
*  @param[in] void
*  @return    gu::uint32 物理コア数
*************************************************************************/
gu::uint32 CoreHardware::GetPhysicalCoreCount()
{
	/*----------------------------------------------------------------------
	*         物理コアの個数のキャッシュを確認し, 値が存在すればそのまま使用する
	*----------------------------------------------------------------------*/
	static gu::uint32 physicalCoreCount = 0;

	if (physicalCoreCount != 0)
	{
		return physicalCoreCount;
	}

	return 0;
}

/*!**********************************************************************
*  @brief     論理コア数を取得します
*  @param[in] void
*  @return    gu::uint32 物理コア数
*************************************************************************/
gu::uint32 CoreHardware::GetLogicalCoreCount()
{
	return 0;
}

/*!**********************************************************************
*  @brief     CPUのベンダー名を取得します
*  @param[in] void
*  @return    tstring ベンダー名
*************************************************************************/
gu::tstring CoreHardware::GetCPUVendorName()
{
	if (!CPU_VENDER_NAME.IsEmpty()) 
	{
		return CPU_VENDER_NAME; 
	}

	/*----------------------------------------------------------------------
	*         CPUID命令を使用してベンダー名を取得する
	*----------------------------------------------------------------------*/
	int32 arguments[4] = { 0 };
	__cpuid(arguments, 0);

	union 
	{
		int32 Data[3];
		char  Buffer[12 + 1];
	} CPUVenderNameInfo = {};

	CPUVenderNameInfo.Data[0] = arguments[1];
	CPUVenderNameInfo.Data[1] = arguments[3];
	CPUVenderNameInfo.Data[2] = arguments[2];
	CPUVenderNameInfo.Buffer[12] = '\0';

	CPU_VENDER_NAME = StringConverter::ConvertStringToTString(string(CPUVenderNameInfo.Buffer, 13));

	return CPU_VENDER_NAME;
}

/*!**********************************************************************
*  @brief     CPUのモデル名を取得します
*  @param[in] void
*  @return    tstring モデル名
*************************************************************************/
gu::tstring CoreHardware::GetCPUModelName()
{
	if (!CPU_MODEL_NAME.IsEmpty())
	{
		return CPU_MODEL_NAME;
	}

	/*----------------------------------------------------------------------
	*         CPUID命令を使用してモデル名を取得する
	*	 CPUID命令の第一引数には0x80000002から0x80000004までの値を設定する
	*----------------------------------------------------------------------*/
	char  brandString[0x40] = { 0  };
	int32 arguments[4]      = { -1 };
	__cpuid(arguments, 0x80000000);

	if (arguments[0] < 0x80000004)
	{
		return tstring();
	}

	const int32 modelInfo[3] = { (int32)0x80000002, (int32)0x80000003, (int32)0x80000004 };

	for (int32 i = 0; i < 3; ++i)
	{
		__cpuid(arguments, modelInfo[i]);

		Memory::Copy(brandString + sizeof(arguments) * i, arguments, sizeof(arguments));
	}

	CPU_MODEL_NAME = StringConverter::ConvertStringToTString(string(brandString));
	return CPU_MODEL_NAME;
}

/*!**********************************************************************
*  @brief     CPUのクロック周波数を取得します
*  @param[in] void
*  @return    gu::uint32 クロック周波数
*************************************************************************/
gu::uint32 CoreHardware::GetCPUClockFrequency()
{
	return 0;
}

/*!**********************************************************************
*  @brief     CPUのキャッシュラインサイズを取得します. キャッシュに格納する単位データの大きさ.
*  @param[in] void
*  @return    gu::uint32 キャッシュラインサイズ
*************************************************************************/
gu::uint32 CoreHardware::GetCPUCacheLineSize()
{
	int32 arguments[4] = { 0 };
	__cpuid(arguments, 0x80000006);

	return arguments[2] & 0xFF;
}

/*!**********************************************************************
*  @brief     GPUのベンダー名を取得します
*  @param[in] void
*  @return    tstring ベンダー名
*************************************************************************/
gu::tstring CoreHardware::GetGPUVendorName()
{
	return tstring();
}

/*!**********************************************************************
*  @brief     GPUのモデル名を取得します
*  @param[in] void
*  @return    tstring モデル名
*************************************************************************/
gu::tstring CoreHardware::GetGPUModelName()
{
	return tstring();
}
#pragma endregion Public Function

#pragma region Other Function

#pragma endregion Other Function