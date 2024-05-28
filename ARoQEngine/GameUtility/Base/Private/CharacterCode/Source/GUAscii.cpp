//////////////////////////////////////////////////////////////////////////////////
///  @file   GUAscii.cpp
///  @brief  Ascii�N���X�̎����t�@�C��
///  @author toide
///  @date   2024/05/24 23:32:54
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Private/CharacterCode/Include/GUAscii.hpp"
#include "GameUtility/Base/Include/GUAssert.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gu::details::string;

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor

#pragma endregion Constructor and Destructor

#pragma region Public Function
/*!**********************************************************************
*  @brief     UTF16����w�肵�������R�[�h�ɕϊ����܂�
*  @param[in] const uint16* input �ϊ����̕�����
*  @param[in] const uint64 inputElementSize �ϊ����̕�����̗v�f��
*  @param[in] uint8* output �ϊ���̕�����
*  @param[in] const uint64 outputByteSize �ϊ���̕�����̃o�C�g��
*  @param[in] CharacterDecodeResult* result �ϊ�����
*  @return    const tchar* �����R�[�h��
*************************************************************************/
bool Ascii::FromUTF16(const uint16* input, const uint64 inputElementSize, uint8* output, [[maybe_unused]] const uint64 outputByteSize, CharacterEncodeResult* result) const
{
	/*-------------------------------------------------------------------
	-              �����`�F�b�N
	---------------------------------------------------------------------*/
	Check(input);
	Check(output);
	Check(result);

	/*-------------------------------------------------------------------
	-              �����`�F�b�N
	---------------------------------------------------------------------*/
	for (uint64 i = 0; i < inputElementSize; ++i)
	{
		// ���ʂ�ASCII����
		if (input[i] <= 0x7F)
		{
			output[i] = static_cast<uint8>(input[i]);
		}
		else
		{
			return false;
		}

		result->ConsumedElementCount++;
		result->OutputByteSize += sizeof(uint16);
		result->OutputCharacterCount++;
	}

	return true;
}

/*!**********************************************************************
*  @brief     UTF32����w�肵�������R�[�h�ɕϊ����܂�
*  @param[in] const uint32* input �ϊ����̕�����
*  @param[in] const uint64 inputElementSize �ϊ����̕�����̃o�C�g��
*  @param[in] uint8* output �ϊ���̕�����
*  @param[in] const uint64 outputElementSize �ϊ���̕�����̗v�f��
*  @param[in] CharacterDecodeResult* result �ϊ�����
*  @return    const tchar* �����R�[�h��
*************************************************************************/
bool Ascii::FromUTF32(const uint32* input, const uint64 inputElementSize, uint8* output, [[maybe_unused]] const uint64 outputByteSize, CharacterEncodeResult* result) const
{
	/*-------------------------------------------------------------------
	-              �����`�F�b�N
	---------------------------------------------------------------------*/
	Check(input);
	Check(output);
	Check(result);

	/*-------------------------------------------------------------------
	-              �����`�F�b�N
	---------------------------------------------------------------------*/
	for (uint64 i = 0; i < inputElementSize; ++i)
	{
		// ���ʂ�ASCII����
		if (input[i] <= 0x7F)
		{
			output[i] = static_cast<uint8>(input[i]);
		}
		else
		{
			return false;
		}

		result->ConsumedElementCount++;
		result->OutputByteSize += sizeof(uint16);
		result->OutputCharacterCount++;
	}

	return true;
}

/*!**********************************************************************
*  @brief     �w�肵�������R�[�h����UTF16�ɕϊ����܂�
*  @param[in] const uint8* input �ϊ����̕�����
*  @param[in] const uint64 inputByteSize �ϊ����̕�����̃o�C�g��
*  @param[in] uint16* output �ϊ���̕�����
*  @param[in] const uint64 outputElementSize �ϊ���̕�����̗v�f��
*  @param[in] CharacterDecodeResult* result �ϊ�����
*  @return    const tchar* �����R�[�h��
*************************************************************************/
bool Ascii::ToUTF16(const uint8* input, const uint64 inputByteSize, uint16* output, [[maybe_unused]] const uint64 outputElementSize, CharacterDecodeResult* result) const
{
	/*-------------------------------------------------------------------
	-              �����`�F�b�N
	---------------------------------------------------------------------*/
	Check(input);
	Check(output);
	Check(result);

	/*-------------------------------------------------------------------
	-              �����`�F�b�N
	---------------------------------------------------------------------*/
	for (uint64 i = 0; i < inputByteSize; ++i)
	{
		// ���ʂ�ASCII����
		if(input[i] <= 0x7F)
		{
			output[i] = static_cast<uint16>(input[i]);
		}
		else
		{
			return false;
		}

		result->ConsumedByteSize++;
		result->OutputByteSize += sizeof(uint16);
		result->OutputCharacterCount++;
	}

	return true;
}

/*!**********************************************************************
*  @brief     �w�肵�������R�[�h����UTF32�ɕϊ����܂�
*  @param[in] const uint8* input �ϊ����̕�����
*  @param[in] const uint64 inputByteSize �ϊ����̕�����̃o�C�g��
*  @param[in] uint32* output �ϊ���̕�����
*  @param[in] const uint64 outputElementSize �ϊ���̕�����̗v�f��
*  @param[in] CharacterDecodeResult* result �ϊ�����
*  @return    const tchar* �����R�[�h��
*************************************************************************/
bool Ascii::ToUTF32(const uint8* input, const uint64 inputByteSize, uint32* output, [[maybe_unused]] const uint64 outputElementSize, CharacterDecodeResult* result) const
{
	/*-------------------------------------------------------------------
	-              �����`�F�b�N
	---------------------------------------------------------------------*/
	Check(input);
	Check(output);
	Check(result);

	/*-------------------------------------------------------------------
	-              �����`�F�b�N
	---------------------------------------------------------------------*/
	for (uint64 i = 0; i < inputByteSize; ++i)
	{
		// ���ʂ�ASCII����
		if (input[i] <= 0x7F)
		{
			output[i] = static_cast<uint32>(input[i]);
		}
		else
		{
			return false;
		}

		result->ConsumedByteSize++;
		result->OutputByteSize += sizeof(uint32);
		result->OutputCharacterCount++;
	}

	return true;
}
#pragma endregion Public Function