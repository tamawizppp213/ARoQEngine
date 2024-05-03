//////////////////////////////////////////////////////////////////////////////////
/// @file   GUCommandLine.hpp
/// @brief  �R�}���h���C��������ێ����Ă����N���X�ł�. @n
///         main(WinMain)�֐����n�܂����^�C�~���O�ŌĂ΂�܂�. @n
///         �R�}���h���C����Application���s����ɓ������̂��g�p����(�C���X�^���X���Ƃɕς������͂��Ȃ�)���Ƃ���, @n
///         �����͑S��Static�Ŋ������Ă��܂�. @n
/// @attention �܂�, Parse���g�p����ꍇ�͕������ɒ��ӂ��Ă�������. �ނ�݂ɑ��₷�ƒx���Ȃ�܂�. (�����̔�r�����邽��)
/// @note      �ő啶������16384�ł�.
/// @author toide
/// @date   2024/03/01 1:50:25
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_COMMAND_LINE_HPP
#define GU_COMMAND_LINE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GUType.hpp"
#include "GUString.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu
{
	/****************************************************************************
	*				  			   CommandLine
	*************************************************************************//**
	/*  @struct   CommandLine
	*   @brief    �R�}���h���C��������ێ����Ă����N���X�ł�. @n
	*   @note     main(WinMain)�֐����n�܂����^�C�~���O��BuildFromArgumentVector���Ă�ł�������. @n
	*****************************************************************************/
	struct CommandLine final
	{
	public:
		#pragma region Public Function
		/*!***********************************************************************
		*  @brief      main�֐��Ŏg����argv, argc���g���ăR�}���h���C�����\�z���܂�.
		*  @param[in]  const int32   : �R�}���h���C�������̐� (��ʓI��argc�ƒ�`��������)
		*  @param[in]  const wchar** : �R�}���h���C���̕����z�� (argumentCount�������p��. ��ʓI��argv�ƒ�`�����)
		*  @return     tstring : �R�}���h���C������s�ɂ܂Ƃ߂������� 
		**************************************************************************/
		static tstring BuildFromArgumentVector(const int32 argumentCount, const wchar** argumentVector);
		
		/*!***********************************************************************
		*  @brief      main�֐��Ŏg����argv, argc���g���ăR�}���h���C�����\�z���܂�.
		*  @param[in]  const int32   : �R�}���h���C�������̐� (��ʓI��argc�ƒ�`��������)
		*  @param[in]  const wchar** : �R�}���h���C���̕����z�� (argumentCount�������p��. ��ʓI��argv�ƒ�`�����)
		*  @return     tstring : �R�}���h���C������s�ɂ܂Ƃ߂������� 
		**************************************************************************/
		static tstring BuildFromArgumentVector(const int32 argumentCount, const char8** argumentVector);

		/*!***********************************************************************
		*  @brief      �ʏ��tchar��������g���ăR�}���h���C����ݒ肵�܂�
		*  @param[in]  const tchar* : tchar�|�C���^�ɂ��R�}���h���C��������
		*  @return     void
		**************************************************************************/
		static void SetUp(const tchar* newCommandLine);

		/*!***********************************************************************
		*  @brief      �R�}���h���C�������ɒǉ����܂�
		*  @param[in]  const tchar* appendString : ���ɒǉ�������������
		*  @return     void 
		**************************************************************************/
		static void Append(const tchar* appendString) 
		{
			Confirmf(_commandLine.Size() + tstring(appendString).Size() < MAX_COMMAND_LINE_LENGTH, "Over MAX_COMMAND_LINE_LENGTH Error");
			_commandLine.Append(appendString); 
		}

		/*!***********************************************************************
		*  @brief      �R�}���h���C������������Ԃɖ߂��܂�.
		*  @param[in]  void
		*  @return     void
		**************************************************************************/
		static void Reset() { _commandLine.ClearAll(); _hasInitialized = false; }

		#pragma endregion Public Function

		#pragma region Public Property
		// @brief : �ő啶����
		static constexpr uint32 MAX_COMMAND_LINE_LENGTH = 16384;

		/*!***********************************************************************
		*  @brief      �R�}���h���C���̕����񎩑̂�Ԃ��܂�. ������, �R�}���h���C���̒��ړI�ȉ��ς͏o���܂���.
		*  @param[in]  void
		*  @return     const tstring& �R�}���h���C��������
		**************************************************************************/
		static const tstring& Get() { return _commandLine; }

		/*!***********************************************************************
		*  @brief      �R�}���h���C���̕�����̐擪�|�C���^��Ԃ��܂�
		*  @param[in]  void
		*  @return     const tchar* �R�}���h���C��������
		**************************************************************************/
		static const tchar* CString() { return _hasInitialized ? _commandLine.CString() : nullptr; }

		/*!***********************************************************************
		*  @brief      ����������Ă��邩�𒲂ׂ܂�
		*  @param[in]  void
		*  @return     const bool �F true�ŏ������ς݂�����.
		**************************************************************************/
		static bool HasInitialized() { return _hasInitialized; }

		#pragma endregion Public Property

		#pragma region Public Constructor and Destructor

		#pragma endregion Public Constructor and Destructor

	private:
		#pragma region Private Function
	
		#pragma endregion Private Function

		#pragma region Private Property
		/*! @brief �������ς݂�*/
		static bool _hasInitialized;

		/*! @brief ��s�̃R�}���h���C����ۑ����Ă���������*/
		static tstring _commandLine;
		#pragma endregion Private Property
	};
}

#endif