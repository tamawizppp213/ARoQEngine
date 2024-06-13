//////////////////////////////////////////////////////////////////////////////////
///  @file   GUMemory.hpp
///  @brief  string.h�̃��b�p�[�N���X�ł�. �s�v��include�����炵�Ă��邽��, �y�ʂł�.@n
///          �{�N���X�ł�, �������̑�����s���܂�. �������̊m��, ���, �R�s�[, �ړ�, ��r, �[���N���A�Ȃǂ̑��삪�\�ł�.@n
///  @author toide
///  @date   2023/12/02 23:07:20
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_MEMORY_HPP
#define GU_MEMORY_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUTypeTraits.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gu
{
	/****************************************************************************
	*				  			   GUMemory
	****************************************************************************/
	/*   @brief     �{�N���X�ł�, �������̑�����s���܂�. �������̊m��, ���, �R�s�[, �ړ�, ��r, �[���N���A�Ȃǂ̑��삪�\�ł�.
	*****************************************************************************/
	class Memory
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     1byte�̒l�Ŏw�肵���o�C�g���������������܂�.
		*  @param[out] void* ����������擪�A�h���X
		*  @param[in] const uint8 1byte�̒l
		*  @param[in] const uint64 ����������o�C�g��
		*  @return    void*
		*************************************************************************/
		static void* Set(void* destination, const uint8 oneByteValue, const uint64 byteLength);

		/*!**********************************************************************
		*  @brief     �w�肵���o�C�g�������[���ɏ��������܂�.
		*  @param[out]void* 0�N���A����擪�A�h���X
		*  @param[in] uint64 �[���N���A����o�C�g��
		*  @return    void*
		*************************************************************************/
		static void* Zero(void* destination, const uint64 byteLength);

		/*!**********************************************************************
		*  @brief     �w�肵�����������������ړ����܂�. memmove�Ɠ�������, �I�[�o�[���b�v���Ă���肠��܂���.
		*  @param[out]void* �ړ���̃A�h���X
		*  @param[in] const void* �ړ����̃A�h���X
		*  @param[in] const uint64 �ړ�����o�C�g��
		*  @return    void*
		*************************************************************************/
		static void* Move(void* destination, const void* source, const uint64 byteLength);

		/*!**********************************************************************
		*  @brief    ���������w�肵���o�C�g���������R�s�[. memcpy�Ɠ�������, �I�[�o�[���b�v���Ă���Ɩ�肪�������܂�.
		*  @param[out]void* �ړ���̃A�h���X
		*  @param[in] const void* �ړ����̃A�h���X
		*  @param[in] const uint64 �ړ�����o�C�g��
		*  @return    void*
		*************************************************************************/
		static void* Copy(void* destination, const void* source, const uint64 byteLength);

		/*!**********************************************************************
		*  @brief     �������r���� (+) left > right, (0) left == right (-) left < right
		*  @param[in] const void* ��r���鍶�̃A�h���X
		*  @param[in] const void* ��r����E�̃A�h���X
		*  @param[in] const uint64 ��r����o�C�g��
		*  @return    int32 �o�b�t�@�[�Ԃōŏ��ɈقȂ�o�C�g�̍���Ԃ��܂�. �l�ɈӖ��͂��܂�Ȃ�����, �d�v�Ȃ̂͂��̕����ɂȂ�܂�.
		*************************************************************************/
		static int32 Compare(const void* left, const void* right, const uint64 byteLength);

		/*!**********************************************************************
		*  @brief     ���������R���X�g���N�^���Ăяo�����ƂȂ��������̈悾�����m�ۂ��܂� (Malloc)
		*  @param[in] const uint64 �m�ۂ��镶����
		*  @return    void* �m�ۂ����������̐擪�A�h���X
		*************************************************************************/
		static void* Allocate       (const uint64 byteLength);

		/*!**********************************************************************
		*  @brief     ���������R���X�g���N�^���Ăяo�����ƂȂ��������̈悾�����m�ۂ��܂� (Malloc) �A���C�����g���w��ł��܂�.
		*  @param[in] const uint64 �m�ۂ��镶����
		*  @param[in] const uint64 �A���C�����g
		*  @return    void* �m�ۂ����������̐擪�A�h���X
		*************************************************************************/
		static void* AllocateAligned(const uint64 byteLength, const uint64 alignment);

		/*!**********************************************************************
		*  @brief     �������̈���Ċ��蓖�Ă��܂� pointer == nullptr -> malloc�Ɠ��� @n
		*             byteLength < �����̃����� : �A�h���X�̃T�C�Y�������������Ċ����̃|�C���^��Ԃ� @n
		*             byteLength = �����̃����� : �������Ȃ��ŕԂ� @n
		*             byteLength > �����̃����� : �g���\�ł���΃T�C�Y�ύX, �s�\�����Ń������͑���Ă��, �V�����A�h���X��Move���Ċm��, �������s����null��Ԃ�@n
		*             https://qiita.com/sawasaka/items/2df474fc00ec2105acab
		*  @param[in] void* �Ċ��蓖�Ă��郁�����̐擪�A�h���X
		*  @param[in] const uint64 �m�ۂ���o�C�g��
		*  @return    void* �Ċm�ۂ����������̐擪�A�h���X
		*************************************************************************/
		static void* Reallocate(void* pointer, const uint64 byteLength);

		/*!**********************************************************************
		*  @brief     ���������f�X�g���N�^���Ăяo�����ƂȂ��������̈悾����j�����܂� (Free)
		*  @param[in] void* �j�����郁�����̐擪�A�h���X
		*  @return    void
		*************************************************************************/
		static void Free(void* pointer);

		/*!**********************************************************************
		*  @brief     ���������f�X�g���N�^���Ăяo�����ƂȂ��������̈悾����j�����܂�. �A���C�����g���w��ł��܂�.
		*  @param[in] void* �j�����郁�����̐擪�A�h���X
		*  @return    void
		*************************************************************************/
		static void FreeAligned(void* pointer);

		#pragma region Force Class Functiokn
		/*!**********************************************************************
		*  @brief     �f�t�H���g�R���X�g���N�^�������I�ɌĂяo���܂� @n
		*             Memory::Allocate���Ăяo���O��̏���
		*  @param[in] void* �R���X�g���N�^���Ăяo�������A�h���X. template�Ō^���w�肵�Ă�������
		*  @param[in] const uint64 �R���X�g���N�^���Ăяo�������z��̃T�C�Y
		*  @return    void
		*************************************************************************/
		template<typename ElementType>
		static void ForceExecuteDefaultConstructors(void* address, const uint64 count);

		/*!**********************************************************************
		*  @brief     �f�X�g���N�^�������I�ɌĂяo���܂�
		*  @param[in] void* �f�X�g���N�^���Ăяo�������A�h���X.
		*  @param[in] const uint64 �f�X�g���N�^���Ăяo�������z��̃T�C�Y
		*  @return    void
		*************************************************************************/
		template<typename ElementType>
		static void ForceExecuteDestructors(ElementType* address, const uint64 count);

		/*!**********************************************************************
		*  @brief     �����I�ɃR�s�[�R���X�g���N�^���Ăяo��
		*  @param[in] ElementType* �R�s�[��
		*  @param[in] ElementType* �R�s�[��
		*  @param[in] const uint64 �R�s�[�R���X�g���N�^���Ăяo�������z��̃T�C�Y
		*  @return    void
		*************************************************************************/
		template<class ElementType>
		static void ForceExecuteCopyConstructors(ElementType* destination, const ElementType* source, const uint64 count);
		
		/*!**********************************************************************
		*  @brief     �����I�ɃR�s�[������Z�q���Ăяo��
		*  @param[in] ElementType* �R�s�[��
		*  @param[in] ElementType* �R�s�[��
		*  @param[in] const uint64 �R�s�[�R���X�g���N�^���Ăяo�������z��̃T�C�Y
		*  @return    void
		*************************************************************************/
		template<class ElementType>
		static void ForceExecuteCopyAssignOperators(ElementType* destination, const ElementType* source, const uint64 count);

		/*!**********************************************************************
		*  @brief     �����I�Ƀ��[�u�R���X�g���N�^���Ăяo��
		*  @param[in] ElementType* ���[�u��
		*  @param[in] ElementType* ���[�u��
		*  @param[in] const uint64 ���[�u�R���X�g���N�^���Ăяo�������z��̃T�C�Y
		*  @return    void
		*************************************************************************/
		template<class ElementType>
		static void ForceExecuteMoveConstructors(ElementType* destination, const ElementType* source, const uint64 count);

		/*!**********************************************************************
		*  @brief     �����I�Ƀ��[�u������Z�q���Ăяo��
		*  @param[in] ElementType* ���[�u��
		*  @param[in] ElementType* ���[�u��
		*  @param[in] const uint64 ���[�u�R���X�g���N�^���Ăяo�������z��̃T�C�Y
		*  @return    void
		*************************************************************************/
		template<class ElementType>
		static void ForceExecuteMoveAssignOperators(ElementType* destination, const ElementType* source, const uint64 count);

		#pragma endregion Force Class Function
		#pragma endregion
		
		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     �w�肵���o�C�g���[���ɐݒ肳��Ă��邩
		*  @param[in] const void* ��r����A�h���X
		*  @param[in] const uint64 ��r����o�C�g��
		*  @return    bool �[���ɐݒ肳��Ă���ꍇ��true, ����ȊO��false
		*************************************************************************/
		static bool IsZero(const void* pointer, const uint64 byteLength);
		#pragma endregion

		#pragma region Public Constructor and Destructor
		#pragma endregion

	protected:
		#pragma region Protected Function
		#pragma endregion

		#pragma region Protected Property
		#pragma endregion
	};

#pragma region Implement
	template<class ElementType>
	void Memory::ForceExecuteDefaultConstructors(void* address, const uint64 count)
	{
		if constexpr (type::HAS_TRIVIAL_CONSTRUCTOR<ElementType>)
		{
			Memory::Set(address, 0, sizeof(ElementType) * count)
		}
		else 
		{
			ElementType* element = (ElementType*)address;
			for (uint64 i = 0; i < count; ++i)
			{
				new (element) ElementType; // �f�t�H���g�R���X�g���N�^���Ăяo��
				++element;
			}
		}
	}

	template<class ElementType>
	void Memory::ForceExecuteDestructors(ElementType* element, const uint64 count)
	{
		// �������^�[�����s���ƃR���p�C�������䂪�n��Ȃ��R�[�h�ł��ƌx�����o��ꍇ�����邽��, �������^�[�����s��Ȃ�
		if constexpr (!type::HAS_TRIVIAL_DESTRUCTOR<ElementType>)
		{
			// �����ł�, ElementType�Ƃ��������o���������ꍇ�ɃR���p�C�������Ă���Ȃ��Ƃ����L�q��UE5�ɂ���������.
			using DestructItemsElementType = ElementType;

			for (uint64 i = 0; i < count; ++i)
			{
				element->DestructItemsElementType::~DestructItemsElementType();
				++element;
			}
		}
	}

	template<class ElementType>
	void Memory::ForceExecuteCopyAssignOperators(ElementType* destination, const ElementType* source, const uint64 count)
	{
		if constexpr (type::HAS_TRIVIAL_ASSIGN_OPERATOR<ElementType>)
		{
			Memory::Copy(destination, source, sizeof(ElementType) * count)
		}
		else
		{
			ElementType* pointer = destination;
			for (uint64 i = 0; i < count; ++i)
			{
				*pointer = *source;
				++pointer;
				++source;
			}
		}
	}

	template<class ElementType>
	void Memory::ForceExecuteCopyConstructors(ElementType* destination, const ElementType* source, const uint64 count)
	{
		if constexpr (type::HAS_TRIVIAL_COPY_CONSTRUCTOR<ElementType>)
		{
			Memory::Copy(destination, source, sizeof(ElementType) * count);
		}
		else
		{
			ElementType* pointer = destination;
			for (uint64 i = 0; i < count; ++i)
			{
				new (pointer) ElementType(*source);
				++pointer;
				++source;
			}
		}
	}

	template<class ElementType>
	static void Memory::ForceExecuteMoveConstructors(ElementType* destination, const ElementType* source, const uint64 count)
	{
		if constexpr (type::HAS_TRIVIAL_COPY_CONSTRUCTOR<ElementType>) // ���[�u�̔��肪�Ȃ�����
		{
			Memory::Move(destination, move, sizeof(ElementType) * count)
		}
		else
		{
			ElementType* pointer = destination;
			for (uint64 i = 0; i < count; ++i)
			{
				new (pointer) ElementType((ElementType&&)*source);
				++pointer;
				++source;
			}
		}
	}

	template<class ElementType>
	static void Memory::ForceExecuteMoveAssignOperators(ElementType* destination, const ElementType* source, const uint64 count)
	{
		if constexpr (type::HAS_TRIVIAL_ASSIGN_OPERATOR<ElementType>) // ���[�u�̔��肪�Ȃ�����
		{
			Memory::Move(destination, move, sizeof(ElementType) * count)
		}
		else
		{
			ElementType* pointer = destination;
			for (uint64 i = 0; i < count; ++i)
			{
				*pointer = (ElementType&&)*source;
				++pointer;
				++source;
			}
		}
	}
#pragma endregion Implement
}

#endif