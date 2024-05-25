//////////////////////////////////////////////////////////////////////////////////
///  @file   GUAscii.hpp
///  @brief  Ascii�R�[�h�������N���X�̃w�b�_�t�@�C��
///  @author toide
///  @date   2024/05/24 0:01:23
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_ASCII_HPP
#define GU_ASCII_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GUCharacterCode.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu::details::string
{
	/****************************************************************************
	*				  			   GUAsciiEncoder
	*************************************************************************//**
	/*  @class     GUAsciiEncoder
	*   @brief     temp
	*****************************************************************************/
	class Ascii : public CharacterCode
	{
	public:
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
		virtual bool FromUTF16(const uint16* input, const uint64 inputElementSize, uint8* output, const uint64 outputByteSize, CharacterEncodeResult* result) const override;

		/*!**********************************************************************
		*  @brief     UTF32����w�肵�������R�[�h�ɕϊ����܂�
		*  @param[in] const uint32* input �ϊ����̕�����
		*  @param[in] const uint64 inputElementSize �ϊ����̕�����̗v�f��
		*  @param[in] uint8* output �ϊ���̕�����
		*  @param[in] const uint64 outputByteSize �ϊ���̕�����̃o�C�g��
		*  @param[in] CharacterDecodeResult* result �ϊ�����
		*  @return    const tchar* �����R�[�h��
		*************************************************************************/
		virtual bool FromUTF32(const uint32* input, const uint64 inputByteSize, uint8* output, const uint64 outputElementSize, CharacterEncodeResult* result) const override;

		/*!**********************************************************************
		*  @brief     �w�肵�������R�[�h����UTF16�ɕϊ����܂�
		*  @param[in] const uint8* input �ϊ����̕�����
		*  @param[in] const uint64 inputByteSize �ϊ����̕�����̃o�C�g��
		*  @param[in] uint16* output �ϊ���̕�����
		*  @param[in] const uint64 outputElementSize �ϊ���̕�����̗v�f��
		*  @param[in] CharacterDecodeResult* result �ϊ�����
		*  @return    const tchar* �����R�[�h��
		*************************************************************************/
		virtual bool ToUTF16(const uint8* input, const uint64 inputByteSize, uint16* output, const uint64 outputElementSize, CharacterDecodeResult* result) const override;

		/*!**********************************************************************
		*  @brief     �w�肵�������R�[�h����UTF32�ɕϊ����܂�
		*  @param[in] const uint8* input �ϊ����̕�����
		*  @param[in] const uint64 inputByteSize �ϊ����̕�����̃o�C�g��
		*  @param[in] uint32* output �ϊ���̕�����
		*  @param[in] const uint64 outputElementSize �ϊ���̕�����̗v�f��
		*  @param[in] CharacterDecodeResult* result �ϊ�����
		*  @return    const tchar* �����R�[�h��
		*************************************************************************/
		virtual bool ToUTF32(const uint8* input, const uint64 inputByteSize, uint32* output, const uint64 outputElementSize, CharacterDecodeResult* result) const override;
		#pragma endregion 

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     �����R�[�h�����擾���܂�
		*  @param[in] void
		*  @return    const tchar* �����R�[�h��
		*************************************************************************/
		__forceinline virtual const tchar* GetName() const override { return SP("ASCII"); }

		/*!**********************************************************************
		*  @brief     �ϒ��ȃo�C�g���̕����R�[�h�����݂��邽��, �ŏ��̃o�C�g�����擾���܂�
		*  @param[in] void
		*  @return    const uint8 �ϒ������R�[�h�̍ŏ��o�C�g��
		*************************************************************************/
		__forceinline virtual uint8 GetMinByte() const override { return 1; }

		/*!**********************************************************************
		*  @brief     �ϒ��ȃo�C�g���̕����R�[�h�����݂��邽��, �ő�̃o�C�g�����擾���܂�
		*  @param[in] void
		*  @return    const uint8 �ϒ������R�[�h�̍ő�o�C�g��
		*************************************************************************/
		__forceinline virtual uint8 GetMaxByte() const override { return 1; }

		/*!**********************************************************************
		*  @brief     �����񒷂��擾���܂�
		*  @param[in] void
		*  @return    const uint64 ������
		*************************************************************************/
		__forceinline virtual uint64 GetCharacterLength(const void* buffer, const uint64 bufferSize) const override 
		{
			return buffer ? (uint64)bufferSize : 0;
		};
		
		/*!**********************************************************************
		*  @brief     �G���R�[�h�����ʂ��邽�߂�BOM(Byte order mark)���擾���܂�
		*  @param[in] void
		*  @return    const uint8* BOM
		*************************************************************************/
		virtual const uint8* GetBOM() const override { return nullptr; };
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

		#pragma endregion 

	};

}
#endif