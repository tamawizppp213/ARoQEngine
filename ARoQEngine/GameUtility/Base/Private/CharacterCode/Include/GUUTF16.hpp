//////////////////////////////////////////////////////////////////////////////////
///  @file   GUUTF16.hpp
///  @brief  UTF16�����R�[�h�������N���X�̃w�b�_�t�@�C��
///  @author toide
///  @date   2024/05/26 2:05:18
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_UTF16_HPP
#define GU_UTF16_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUCharacterCode.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gu::details::string
{

	/****************************************************************************
	*				  			   UTF16
	*************************************************************************//**
	/*   @brief  UTF8�����R�[�h�������N���X
	*****************************************************************************/
	class UTF16 : public CharacterCode
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
		__forceinline virtual const tchar* GetName() const override 
		{
			if (_isBOM)
			{
				return _isBigEndian ? SP("UTF-16BE-BOM") : SP("UTF-16LE-BOM");
			}
			else
			{
				return _isBigEndian ? SP("UTF-16BE") : SP("UTF-16LE");
			}
		}

		/*!**********************************************************************
		*  @brief     �ϒ��ȃo�C�g���̕����R�[�h�����݂��邽��, �ŏ��̃o�C�g�����擾���܂�
		*  @param[in] void
		*  @return    const uint8 �ϒ������R�[�h�̍ŏ��o�C�g��
		*************************************************************************/
		__forceinline virtual uint8 GetMinByte() const override { return 2; }

		/*!**********************************************************************
		*  @brief     �ϒ��ȃo�C�g���̕����R�[�h�����݂��邽��, �ő�̃o�C�g�����擾���܂�
		*  @param[in] void
		*  @return    const uint8 �ϒ������R�[�h�̍ő�o�C�g��
		*************************************************************************/
		__forceinline virtual uint8 GetMaxByte() const override { return 4; }

		/*!**********************************************************************
		*  @brief     �����񒷂��擾���܂�
		*  @param[in] const void* buffer ������
		*  @param[in] const uint64 bufferSize ������̃o�C�g��
		*  @return    const uint64 ������
		*************************************************************************/
		virtual uint64 GetCharacterLength(const void* buffer, const uint64 bufferSize) const override;

		/*!**********************************************************************
		*  @brief     �G���R�[�h�����ʂ��邽�߂�BOM(Byte order mark)���擾���܂�
		*  @param[in] void
		*  @return    const uint8* BOM
		*************************************************************************/
		virtual const uint8* GetBOM() const override { return BOM ; }

		/*!**********************************************************************
		*  @brief     �w�肳�ꂽ�o�b�t�@�̐擪���}���`�o�C�g�R�[�h��T���Q�[�g�y�A�̐�s�����ł���ꍇ�A�ǉ��œǂނׂ������񒷂���Ԃ��܂��B
		*  @param[in] const void* buffer ������
		*  @param[in] const uint64 bufferSize ������̃o�C�g��
		*  @return    const tchar* �����R�[�h��
		*************************************************************************/
		virtual uint64 GetLeadExtraLength(const void* buffer, const uint64 bufferSize) const override;

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		UTF16() = default;

		UTF16(const bool isBOM, const bool isBigEndian) : _isBOM(isBOM), _isBigEndian(isBigEndian) {}

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
		/*! @brief Byte Order Mark (�����R�[�h����)���g�p���邩*/
		bool _isBOM = false;

		/*! @brief �r�b�N�G���f�B�A�����ǂ��� (�f�t�H���g��false)*/
		bool _isBigEndian = false;

		static constexpr uint8 BOM[] = { 0xFE, 0xFF };
		#pragma endregion 

	};

}
#endif