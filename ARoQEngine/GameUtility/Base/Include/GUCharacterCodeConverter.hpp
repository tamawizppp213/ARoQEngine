//////////////////////////////////////////////////////////////////////////////////
///  @file   GUCharacterCodeConverter.hpp
///  @brief  �����R�[�h��ϊ����܂�.
///  @author toide
///  @date   2024/05/26 23:29:00
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_CHARACTER_CODE_CONVERTER_HPP
#define GU_CHARACTER_CODE_CONVERTER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GUCharacterCode.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gu
{
	/****************************************************************************
	*				  			  CharacterCodeConverter
	****************************************************************************/
	/*   @brief   �����R�[�h��ϊ����܂�.
	*****************************************************************************/
	class CharacterCodeConverter
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     �����R�[�h��ϊ���, �o�C�g���Ԃ��܂�
		*  @param[in] void
		*  @return    bool �ϊ��ɐ��������ꍇ��true, ���s�����ꍇ��false
		*************************************************************************/
		gu::DynamicArray<uint8>& Convert(const uint8* input, const uint64 inputByteSize);

		#pragma endregion 

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     �ϊ����̃e�L�X�g�̕����R�[�h���擾���܂�
		*  @param[in] void
		*  @return    SharedPointer<CharacterCode> �ϊ����̕����R�[�h
		*************************************************************************/
		__forceinline SharedPointer<CharacterCode> GetSourceCharacterCode() const
		{
			return _sourceCharacterCode;
		}

		/*!**********************************************************************
		*  @brief     �ϊ���̃e�L�X�g�̕����R�[�h���擾���܂�
		*  @param[in] void
		*  @return    SharedPointer<CharacterCode> �ϊ���̕����R�[�h
		*************************************************************************/
		__forceinline SharedPointer<CharacterCode> GetDestinationCharacterCode() const
		{
			return _destinationCharacterCode;
		}

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		CharacterCodeConverter() = default;

		/*! @brief �f�X�g���N�^*/
		~CharacterCodeConverter() = default;

		/*! @brief �����R�[�h���w�肵�ď��������܂�*/
		CharacterCodeConverter(const CharacterCodeType before,  const CharacterCodeType after, 
			const bool beforeUseBOM      = false, const bool afterUseBOM      = false, 
			const bool beforeIsBigEndian = false, const bool afterIsBigEndian = false)
		{
			_sourceCharacterCode      = CharacterCode::GetCharacterCode(before, beforeUseBOM, beforeIsBigEndian);
			_destinationCharacterCode = CharacterCode::GetCharacterCode(after , afterUseBOM , afterIsBigEndian);
		}

		/*! @brief �����R�[�h�̃|�C���^���w�肵�ď��������܂�. */
		CharacterCodeConverter(const SharedPointer<CharacterCode>& before, const SharedPointer<CharacterCode>& after)
			: _sourceCharacterCode(before), _destinationCharacterCode(after) {};

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
		/*! @brief �ϊ��O�̕����R�[�h*/
		SharedPointer<CharacterCode> _sourceCharacterCode = nullptr;

		/*! @brief �ϊ���̕����R�[�h*/
		SharedPointer<CharacterCode> _destinationCharacterCode = nullptr;

		/*! @brief �o�̓o�b�t�@*/
		gu::DynamicArray<uint8> _outputBuffer = {};
		#pragma endregion 

	};
}

#endif