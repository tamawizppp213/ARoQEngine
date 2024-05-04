//////////////////////////////////////////////////////////////////////////////////
///  @file   HashMapElement.hpp
///  @brief  temp
///  @author toide
///  @date   2024/05/04 0:20:02
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_HASH_MAP_ELEMENT_HPP
#define GU_HASH_MAP_ELEMENT_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Container/Include/GUPair.hpp"
#include "GameUtility/Base/Include/GUType.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu::details::hashmap
{
	/****************************************************************************
	*				  			   HashMapElement
	*************************************************************************//**
	/*  @class     HashMapElement
	*   @brief     ���r���t�b�h�@���g�p���邽�߂̘A�z�z��̗v�f
	*****************************************************************************/
	template<typename Key, typename Value>
	struct HashMapElement
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     ��̃f�t�H���g�e�[�u����n�� (�l�̕ύX�͂��ꂮ������Ȃ��悤�ɂ��Ă�������)
		*  @param[in] void
		*  @return    HashMapElement*
		*************************************************************************/
		static HashMapElement* EmptyDefaultTable()
		{
			static HashMapElement result[MIN_SEARCH_COUNT] = { {}, {}, {}, {SPECIAL_END_VALUE} };
			return result;
		}
		#pragma endregion 

		#pragma region Public Property
		/*! @brief �ŏ��̒T����*/
		static constexpr gu::int8 MIN_SEARCH_COUNT = 4;

		/*! @brief �����l*/
		static constexpr gu::int8 INVALID_VALUE = -1;

		/*! @brief ���ʂȏI�[�l*/
		static constexpr gu::int8 SPECIAL_END_VALUE = 0;

		/*! @brief �e�G���g�������z�I�Ȉʒu����ǂꂾ������Ă��邩*/
		gu::int8 ProbeDistance = INVALID_VALUE;

		/*! @brief HashMap�̎��f�[�^*/
		gu::Pair<Key, Value> Data = {};

		/*!**********************************************************************
		*  @brief     �l�����ɏ���������Ă��邩
		*  @param[in] void
		*  @return    bool : true�ŏ������ς�
		*************************************************************************/
		__forceinline bool HasValue() const { return ProbeDistance >= 0; }

		/*!**********************************************************************
		*  @brief     �l���܂��ݒ肳��Ă��Ȃ���
		*  @param[in] void
		*  @return    bool : true�Ŗ��ݒ�
		*************************************************************************/
		__forceinline bool IsEmpty() const { return ProbeDistance < 0; }

		/*!**********************************************************************
		*  @brief     �]�񂾏ꏊ�ɂ��邩�ǂ���
		*  @param[in] void
		*  @return    bool : true�ł��̏ꏊ�ɂ���
		*************************************************************************/
		__forceinline bool IsDesiredPosition() const { return ProbeDistance == 0; }

		/*!**********************************************************************
		*  @brief     �l��j������
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		__forceinline void DestroyValue()
		{
			Memory::ForceExecuteDestructors(&Data.Value, 1);
			ProbeDistance = INVALID_VALUE;
		}

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