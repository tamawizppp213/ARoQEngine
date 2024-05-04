//////////////////////////////////////////////////////////////////////////////////
///  @file   GUHash.hpp
///  @brief  �l�X�ȃn�b�V���֐����܂Ƃ߂��N���X�ł�. 
///  @author toide
///  @date   2024/04/29 10:24:55
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_HASH_HPP
#define GU_HASH_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GUType.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu
{
	/****************************************************************************
	*				  			   GUHash
	*************************************************************************//**
	/*  @class     GUHash
	*   @brief     �l�X�ȃn�b�V���֐����܂Ƃ߂��N���X�ł�. 
	*****************************************************************************/
	class Hash
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     FNV_1 : �y�ʂł���Ȃ����r�I�������U������. @n
		              �Z�L�����e�B�p�r�ɗp���邱�Ƃ��ł���قǂ̑ϏՓː��͖��� (std::hash�͂���)
		*  @param[in] const void*   ������f�[�^
		*  @param[in] const uint64  �o�C�g��
		*  @param[in] const uint32  �����l (�V�[�h�l):�f�t�H���g��0�ł�
		*  @return    uint32        �n�b�V���l
		*************************************************************************/
		static uint32 FNV1a_32(const void* key, const uint64 byteLength, const uint32 seed = 0);

		/*!**********************************************************************
		*  @brief     FNV_1 : �y�ʂł���Ȃ����r�I�������U������. @n
					  �Z�L�����e�B�p�r�ɗp���邱�Ƃ��ł���قǂ̑ϏՓː��͖��� (std::hash�͂���)
		*  @param[in] const uint8*  ������f�[�^
		*  @param[in] const uint64  �o�C�g��
		*  @param[in] const uint64  �����l (�V�[�h�l):�f�t�H���g��0�ł�
		*  @return    gu::uint64 : �n�b�V���l
		*************************************************************************/
		static uint64 FNV1a_64(const void* key, const uint64 byteLength, const uint64 seed = 0);

		/*!**********************************************************************
		*  @brief     Murmur2 : FNV�Ɠ����ȏ�̐��\��, �y�ʂ��������U������. �A���C�������g�Ή��͂��Ă܂��� @n
		*  @param[in] const uint8*  ������f�[�^
		*  @param[in] const uint64  �o�C�g��
		*  @param[in] const uint32  �����l (�V�[�h�l):�f�t�H���g��0�ł�
		*  @return    gu::uint32 : �n�b�V���l
		*************************************************************************/
		static uint32 Murmur2_32(const void* key, const uint64 byteLength, const uint32 seed = 0);

		/*!**********************************************************************
		*  @brief     Murmur2 : FNV�Ɠ����ȏ�̐��\��, �y�ʂ��������U�����A���C�������g. �Ή��͂��Ă܂��� @n
		*  @param[in] const uint8*  ������f�[�^
		*  @param[in] const uint64  �o�C�g��
		*  @param[in] const uint64  �����l (�V�[�h�l):�f�t�H���g��0�ł�
		*  @return    gu::uint64 : �n�b�V���l
		*************************************************************************/
		static uint64 Murmur2_64(const void* key, const uint64 byteLength, const uint64 seed = 0);

		/*!**********************************************************************
		*  @brief     XX_32 : xxhash�A���S���Y����p������Í����n�b�V��. �����ɓ��삷�邽��, �{�G���W���̊�{�n�b�V�����A���S���Y���ɐݒ�
		*  @param[in] const uint8*  ������f�[�^
		*  @param[in] const uint64  �o�C�g��
		*  @param[in] const uint32  �����l (�V�[�h�l):�f�t�H���g��0�ł�
		*  @return    gu::uint32 : �n�b�V���l
		*************************************************************************/
		static uint32 XX_32(const void* key, const uint64 byteLength, const uint32 seed = 0);

		/*!**********************************************************************
		*  @brief     XX_64 : xxhash�A���S���Y����p������Í����n�b�V��. �����ɓ��삷�邽��, �{�G���W���̊�{�n�b�V�����A���S���Y���ɐݒ�
		*  @param[in] const uint8*  ������f�[�^
		*  @param[in] const uint64  �o�C�g��
		*  @param[in] const uint64  �����l (�V�[�h�l):�f�t�H���g��0�ł�
		*  @return    gu::uint64 : �n�b�V���l
		*************************************************************************/
		static uint64 XX_64(const void* key, const uint64 byteLength, const uint64 seed = 0);
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
		// FNV Const
		static constexpr uint32 FNV_OFFSET_BASIS_32 = 2166136261U;
		static constexpr uint64 FNV_OFFSET_BASIS_64 = 14695981039346656037U;
		static constexpr uint32 FNV_PRIME_32        = 16777619U;
		static constexpr uint64 FNV_PRIME_64        = 1099511628211LLU;

		// XX hash Const
		static constexpr uint32 XX_PRIME1_32 = 0x9E3779B1U;
		static constexpr uint32 XX_PRIME2_32 = 0x85EBCA77U;
		static constexpr uint32 XX_PRIME3_32 = 0xC2B2AE3DU;
		static constexpr uint32 XX_PRIME4_32 = 0x27D4EB2FU;
		static constexpr uint32 XX_PRIME5_32 = 0x165667B1U;

		static constexpr uint64 XX_PRIME1_64 = 11400714785074694791ULL;
		static constexpr uint64 XX_PRIME2_64 = 14029467366897019727ULL;
		static constexpr uint64 XX_PRIME3_64 = 1609587929392839161ULL;
		static constexpr uint64 XX_PRIME4_64 = 9650029242287828579ULL;
		static constexpr uint64 XX_PRIME5_64 = 2870177450012600261ULL;
		#pragma endregion 

	};

}
#endif