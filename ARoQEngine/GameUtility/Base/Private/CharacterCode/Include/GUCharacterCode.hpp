//////////////////////////////////////////////////////////////////////////////////
///  @file   GUCharacterCode.hpp
///  @brief  ������̃R�[�h�������N���X�̃w�b�_�t�@�C��
///  @author toide
///  @date   2024/05/24 0:03:40
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_CHARACTER_CODE_HPP
#define GU_CHARACTER_CODE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUType.hpp"
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gu::details::string
{
	/****************************************************************************
	*				  			 CharacterCodeType
	*************************************************************************//**
	/*   @brief     �����镶���R�[�h�̎�ނ��L�q���܂�
	*****************************************************************************/
	enum class CharacterCodeType
	{
		Unknown,
		UTF8, 
		UTF16,
		UTF32,
		ShiftJIS,
		ASCII,
		ANSI,
	};

	struct CharacterEncodeResult
	{
		/*! @brief ����v�f�� (���͗v�f���ɑ΂��đ���Ȃ���Εϊ��G���[������)*/
		uint64 ConsumedElementCount = 0;

		/*! @brief �ϊ����ʂ̃o�C�g��*/
		uint64 OutputByteSize = 0;

		/*! @brief �ϊ����ʂ̕�����*/
		uint64 OutputCharacterCount = 0;
	};


	struct CharacterDecodeResult
	{
		/*! @brief ����o�C�g�� (���̓o�C�g���ɑ΂��đ���Ȃ���Εϊ��G���[������)*/
		uint64 ConsumedByteSize = 0;

		/*! @brief �ϊ����ʂ̃o�C�g��*/
		uint64 OutputByteSize = 0;

		/*! @brief �ϊ����ʂ̕�����*/
		uint64 OutputCharacterCount = 0;
	};


	/****************************************************************************
	*				  			   CharacterCode
	*************************************************************************//**
	/*   @brief     �����R�[�h�̃G���R�[�h��ŃR�[�h���������N���X
	*****************************************************************************/
	class CharacterCode : public NonCopyable
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     ���ˑ��̃��C�h������Ŏg�p����镶���R�[�h���擾���܂�
		*  @param[in] void
		*  @return    SharedPointer<CharacterCode> �����R�[�h
		*************************************************************************/
		static SharedPointer<CharacterCode> WideStringCharacterCode();

		/*!**********************************************************************
		*  @brief     TString�Ŏg�p����镶���R�[�h���擾���܂�
		*  @param[in] void
		*  @return    SharedPointer<CharacterCode> �����R�[�h
		*************************************************************************/
		static SharedPointer<CharacterCode> TStringCharacterCode();

		/*!**********************************************************************
		*  @brief     �w�肵�������R�[�h���擾���܂�
		*  @param[in] const CharacterCodeType �����R�[�h�̎��
		*  @return    SharedPointer<CharacterCode> �����R�[�h
		*************************************************************************/
		static SharedPointer<CharacterCode> GetCharacterCode(const CharacterCodeType codeType, const bool useBOM = false, const bool isBigEndian = true);

		/*!**********************************************************************
		*  @brief     UTF16����w�肵�������R�[�h�ɕϊ����܂�
		*  @param[in] const uint16* input �ϊ����̕�����
		*  @param[in] const uint64 inputByteSize �ϊ����̕�����̃o�C�g��
		*  @param[in] uint8* output �ϊ���̕�����
		*  @param[in] const uint64 outputElementSize �ϊ���̕�����̗v�f��
		*  @param[in] CharacterDecodeResult* result �ϊ�����
		*  @return    const tchar* �����R�[�h��
		*************************************************************************/
		virtual bool FromUTF16(const uint16* input, const uint64 inputElementSize, uint8* output, const uint64 outputByteSize, CharacterEncodeResult* result) const = 0;

		/*!**********************************************************************
		*  @brief     UTF32����w�肵�������R�[�h�ɕϊ����܂�
		*  @param[in] const uint32* input �ϊ����̕�����
		*  @param[in] const uint64 inputByteSize �ϊ����̕�����̃o�C�g��
		*  @param[in] uint8* output �ϊ���̕�����
		*  @param[in] const uint64 outputElementSize �ϊ���̕�����̗v�f��
		*  @param[in] CharacterDecodeResult* result �ϊ�����
		*  @return    const tchar* �����R�[�h��
		*************************************************************************/
		virtual bool FromUTF32(const uint32* input, const uint64 inputByteSize, uint8* output, const uint64 outputElementSize, CharacterEncodeResult* result) const = 0;

		/*!**********************************************************************
		*  @brief     �w�肵�������R�[�h����UTF16�ɕϊ����܂�
		*  @param[in] const uint8* input �ϊ����̕�����
		*  @param[in] const uint64 inputByteSize �ϊ����̕�����̃o�C�g��
		*  @param[in] uint16* output �ϊ���̕�����
		*  @param[in] const uint64 outputElementSize �ϊ���̕�����̗v�f��
		*  @param[in] CharacterDecodeResult* result �ϊ�����
		*  @return    const tchar* �����R�[�h��
		*************************************************************************/
		virtual bool ToUTF16(const uint8* input, const uint64 inputByteSize, uint16* output, const uint64 outputElementSize, CharacterDecodeResult* result) const = 0;

		/*!**********************************************************************
		*  @brief     �w�肵�������R�[�h����UTF32�ɕϊ����܂�
		*  @param[in] const uint8* input �ϊ����̕�����
		*  @param[in] const uint64 inputByteSize �ϊ����̕�����̃o�C�g��
		*  @param[in] uint32* output �ϊ���̕�����
		*  @param[in] const uint64 outputElementSize �ϊ���̕�����̗v�f��
		*  @param[in] CharacterDecodeResult* result �ϊ�����
		*  @return    const tchar* �����R�[�h��
		*************************************************************************/
		virtual bool ToUTF32(const uint8* input, const uint64 inputByteSize, uint32* output, const uint64 outputElementSize, CharacterDecodeResult* result) const = 0;

		#pragma endregion 

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     �����R�[�h�����擾���܂�
		*  @param[in] void 
		*  @return    const tchar* �����R�[�h��
		*************************************************************************/
		virtual const tchar* GetName() const = 0;

		/*!**********************************************************************
		*  @brief     �ϒ��ȃo�C�g���̕����R�[�h�����݂��邽��, �ŏ��̃o�C�g�����擾���܂�
		*  @param[in] void
		*  @return    const uint8 �ϒ������R�[�h�̍ŏ��o�C�g��
		*************************************************************************/
		virtual uint8 GetMinByte() const = 0;

		/*!**********************************************************************
		*  @brief     �ϒ��ȃo�C�g���̕����R�[�h�����݂��邽��, �ő�̃o�C�g�����擾���܂�
		*  @param[in] void
		*  @return    const uint8 �ϒ������R�[�h�̍ő�o�C�g��
		*************************************************************************/
		virtual uint8 GetMaxByte() const = 0;

		/*!**********************************************************************
		*  @brief     �����񒷂��擾���܂�
		*  @param[in] const void* buffer ������
		*  @param[in] const uint64 bufferSize ������̃o�C�g��
		*  @return    const uint64 ������
		*************************************************************************/
		virtual uint64 GetCharacterLength(const void* buffer, const uint64 bufferSize) const = 0;

		/*!**********************************************************************
		*  @brief     �G���R�[�h�����ʂ��邽�߂�BOM(Byte order mark)���擾���܂�
		*  @param[in] void
		*  @return    const uint8* BOM
		*************************************************************************/
		virtual const uint8* GetBOM() const = 0;

		/*!**********************************************************************
		*  @brief     �w�肳�ꂽ�o�b�t�@�̐擪���}���`�o�C�g�R�[�h��T���Q�[�g�y�A�̐�s�����ł���ꍇ�A�ǉ��œǂނׂ������񒷂���Ԃ��܂��B
		*  @param[in] const void* buffer ������
		*  @param[in] const uint64 bufferSize ������̃o�C�g��
		*  @return    const tchar* �����R�[�h��
		*************************************************************************/
		virtual uint64 GetReadExtraLength(const void* buffer, const uint64 bufferSize) const = 0;

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor

		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor

		#pragma endregion 

		#pragma region Protected Property

		#pragma endregion

		#pragma region Protected Function

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