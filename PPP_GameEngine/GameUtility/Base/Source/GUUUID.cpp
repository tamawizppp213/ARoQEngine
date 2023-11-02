//////////////////////////////////////////////////////////////////////////////////
///             @file   GUUUID.cpp
///             @brief  temp
///             @author toide
///             @date   2023/10/29 16:42:22
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/GUUUID.hpp"
#include "../Include/GUAssert.hpp"
#include <vector>
#include <Windows.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gu;
using namespace gm;

//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////
namespace
{
	template <typename ... Args>
	std::string Format(const std::string& fmt, Args ... args)
	{
		size_t len = std::snprintf(nullptr, 0, fmt.c_str(), args ...);
		std::vector<char> buf(len + 1);
		std::snprintf(&buf[0], len + 1, fmt.c_str(), args ...);
		return std::string(&buf[0], &buf[0] + len);
	}
}

#pragma region Constructor and Destructor 

#pragma endregion Constructor and Destructor

#pragma region Main Function
/****************************************************************************
*                       Create
*************************************************************************//**
*  @fn        gu::UUID gu::UUID::Create()
*
*  @brief     GUID�𐶐����܂�
*
*  @param[in] void
*
*  @return �@�@void
*****************************************************************************/
gu::UUID gu::UUID::Create()
{
	gu::UUID result(0, 0, 0, 0);

#if defined(_WIN32) || defined(_WIN64)
	Verifyf(CoCreateGuid((GUID*)&result) == S_OK, "Failed to create guid.");
#else
	OutputDebugStringA("����GUID�����ɂ͖��Ή���OS�ł�.\n");
#endif

	return result;
}

/****************************************************************************
*                       AppendString
*************************************************************************//**
*  @fn        void gu::UUID::AppendString(std::string& string, const UUIDFormat format)
*
*  @brief     �������ǉ����܂�
*
*  @param[in] std::string ������
* �@@param[in] UUIDFormat ������t�H�[�}�b�g
*
*  @return �@�@void
*****************************************************************************/
void gu::UUID::AppendString(std::string& string, const UUIDFormat format)
{
	switch (format)
	{
		case UUIDFormat::Default:
		{
			string += Format("%08X-%08X-%08X-%08X", A, B, C, D);
			return;
		}
		default:
		{
			OutputDebugStringA("���݂͖��Ή��̌`���ł�\n");
			return;
		}	
	}
}

/****************************************************************************
*                       ToString
*************************************************************************//**
*  @fn        std::string gu::UUID::ToString(const UUIDFormat format)
*
*  @brief     �w�肵���t�H�[�}�b�g�ŕ������ǉ����܂�
*
* �@@param[in] UUIDFormat ������t�H�[�}�b�g
*
*  @return �@�@void
*****************************************************************************/
std::string gu::UUID::ToString(const UUIDFormat format)
{
	std::string result = "";
	AppendString(result, format);
	return result;
}
#pragma endregion Main Function

#pragma region Operator
uint32& gu::UUID::operator[](const uint32 index)
{
	Check(index >= 0);
	Check(index <  4);

	if      (index == 0) { return A; }
	else if (index == 1) { return B; }
	else if (index == 2) { return C; }
	else                 { return D; }
}

const gm::uint32& gu::UUID::operator[](const gm::uint32 index) const
{
	Check(index >= 0);
	Check(index <  4);

	if      (index == 0) { return A; }
	else if (index == 1) { return B; }
	else if (index == 2) { return C; }
	else                 { return D; }
}
#pragma endregion Operator