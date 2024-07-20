//////////////////////////////////////////////////////////////////////////////////
///  @file   WindowsHardware.cpp
///  @brief  �����̎Q�l����: https://learn.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-2010/hskdteyh(v=vs.100)?redirectedfrom=MSDN
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
*  @brief     �����R�A�����擾���܂�
*  @param[in] void
*  @return    gu::uint32 �����R�A��
*************************************************************************/
gu::uint32 CoreHardware::GetPhysicalCoreCount()
{
	/*----------------------------------------------------------------------
	*         �����R�A�̌��̃L���b�V�����m�F��, �l�����݂���΂��̂܂܎g�p����
	*----------------------------------------------------------------------*/
	static gu::uint32 physicalCoreCount = 0;

	if (physicalCoreCount != 0)
	{
		return physicalCoreCount;
	}

	return 0;
}

/*!**********************************************************************
*  @brief     �_���R�A�����擾���܂�
*  @param[in] void
*  @return    gu::uint32 �����R�A��
*************************************************************************/
gu::uint32 CoreHardware::GetLogicalCoreCount()
{
	return 0;
}

/*!**********************************************************************
*  @brief     CPU�̃x���_�[�����擾���܂�
*  @param[in] void
*  @return    tstring �x���_�[��
*************************************************************************/
gu::tstring CoreHardware::GetCPUVendorName()
{
	if (!CPU_VENDER_NAME.IsEmpty()) 
	{
		return CPU_VENDER_NAME; 
	}

	/*----------------------------------------------------------------------
	*         CPUID���߂��g�p���ăx���_�[�����擾����
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
*  @brief     CPU�̃��f�������擾���܂�
*  @param[in] void
*  @return    tstring ���f����
*************************************************************************/
gu::tstring CoreHardware::GetCPUModelName()
{
	if (!CPU_MODEL_NAME.IsEmpty())
	{
		return CPU_MODEL_NAME;
	}

	/*----------------------------------------------------------------------
	*         CPUID���߂��g�p���ă��f�������擾����
	*	 CPUID���߂̑������ɂ�0x80000002����0x80000004�܂ł̒l��ݒ肷��
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
*  @brief     CPU�̃N���b�N���g�����擾���܂�
*  @param[in] void
*  @return    gu::uint32 �N���b�N���g��
*************************************************************************/
gu::uint32 CoreHardware::GetCPUClockFrequency()
{
	return 0;
}

/*!**********************************************************************
*  @brief     CPU�̃L���b�V�����C���T�C�Y���擾���܂�. �L���b�V���Ɋi�[����P�ʃf�[�^�̑傫��.
*  @param[in] void
*  @return    gu::uint32 �L���b�V�����C���T�C�Y
*************************************************************************/
gu::uint32 CoreHardware::GetCPUCacheLineSize()
{
	int32 arguments[4] = { 0 };
	__cpuid(arguments, 0x80000006);

	return arguments[2] & 0xFF;
}

/*!**********************************************************************
*  @brief     GPU�̃x���_�[�����擾���܂�
*  @param[in] void
*  @return    tstring �x���_�[��
*************************************************************************/
gu::tstring CoreHardware::GetGPUVendorName()
{
	return tstring();
}

/*!**********************************************************************
*  @brief     GPU�̃��f�������擾���܂�
*  @param[in] void
*  @return    tstring ���f����
*************************************************************************/
gu::tstring CoreHardware::GetGPUModelName()
{
	return tstring();
}
#pragma endregion Public Function

#pragma region Other Function

#pragma endregion Other Function