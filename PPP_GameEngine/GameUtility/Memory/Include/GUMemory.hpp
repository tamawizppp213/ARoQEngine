//////////////////////////////////////////////////////////////////////////////////
///             @file   GUMemory.hpp
///             @brief  string.h�̃��b�p�[�N���X�ł�. �s�v��include�����炵�Ă��邽��, �y�ʂł�.
///             @author toide
///             @date   2023/12/02 23:07:20
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_MEMORY_HPP
#define GU_MEMORY_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Base/Include/GUTypeTraits.hpp"

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
	*************************************************************************//**
	*  @class     GUMemory
	*  @brief     memory function wrapper
	*****************************************************************************/
	class Memory
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*---------------------------------------------------------------
			@brief :  �w�肵���������������̏��������s���܂�.
		-----------------------------------------------------------------*/
		static void* Set(void* destination, const uint8 oneByteValue, const uint64 byteLength);

		/*---------------------------------------------------------------
			@brief :  �w�肵���o�C�g�������[���ɏ��������܂�.
		-----------------------------------------------------------------*/
		static void* Zero(void* destination, const uint64 byteLength);

		/*---------------------------------------------------------------
			@brief :  �w�肵�����������������ړ����܂�.
		-----------------------------------------------------------------*/
		static void* Move(void* destination, const void* source, const uint64 byteLength);

		/*---------------------------------------------------------------
			@brief :  ���������w�肵���o�C�g���������R�s�[
		-----------------------------------------------------------------*/
		static void* Copy(void* destination, const void* source, const uint64 byteLength);

		/*---------------------------------------------------------------
			@brief :  �������r���� (+) left > right, (0) left == right (-) left < right
		-----------------------------------------------------------------*/
		static int32 Compare(const void* left, const void* right, const uint64 byteLength);

		/*---------------------------------------------------------------
			@brief :  ���������R���X�g���N�^���Ăяo�����ƂȂ��������̈悾�����m�ۂ��܂� (Malloc)
		-----------------------------------------------------------------*/
		static void* Allocate       (const uint64 byteLength);
		static void* AllocateAligned(const uint64 byteLength, const uint64 alignment);

		/*---------------------------------------------------------------
			@brief :  ���������f�X�g���N�^���Ăяo�����ƂȂ��������̈悾����j�����܂� (Free)
		-----------------------------------------------------------------*/
		static void Free(void* pointer);
		static void FreeAligned(void* pointer);

#pragma region Force Class Functiokn
		/*---------------------------------------------------------------
			@brief :  �f�t�H���g�R���X�g���N�^�������I�ɌĂяo���܂�
			          Memory::Allocate���Ăяo���O��̏���
		-----------------------------------------------------------------*/
		template<typename ElementType>
		static void ForceExecuteDefaultConstructors(void* address, const uint64 count);

		/*---------------------------------------------------------------
			@brief :  �f�X�g���N�^�������I�ɌĂяo���܂�
		-----------------------------------------------------------------*/
		template<typename ElementType>
		static void ForceExecuteDestructors(ElementType* address, const uint64 count);

		/*---------------------------------------------------------------
			@brief :  �����I�ɃR�s�[�R���X�g���N�^���Ăяo��
		-----------------------------------------------------------------*/
		template<class ElementType>
		static void ForceExecuteCopyConstructors(ElementType* destination, const ElementType* source, const uint64 count);
		
		/*---------------------------------------------------------------
			@brief :  �����I�ɃR�s�[������Z�q���Ăяo��
		-----------------------------------------------------------------*/
		template<class ElementType>
		static void ForceExecuteCopyAssignOperators(ElementType* destination, const ElementType* source, const uint64 count);

		/*---------------------------------------------------------------
			@brief :  �����I�Ƀ��[�u�R���X�g���N�^���Ăяo��
		-----------------------------------------------------------------*/
		template<class ElementType>
		static void ForceExecuteMoveConstructors(ElementType* destination, const ElementType* source, const uint64 count);

		/*---------------------------------------------------------------
			@brief :  �����I�Ƀ��[�u������Z�q���Ăяo��
		-----------------------------------------------------------------*/
		template<class ElementType>
		static void ForceExecuteMoveAssignOperators(ElementType* destination, const ElementType* source, const uint64 count);

#pragma endregion Force Class Function
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/*---------------------------------------------------------------
			@brief :  �w�肵���o�C�g���[���ɐݒ肳��Ă��邩
		-----------------------------------------------------------------*/
		static bool IsZero(const void* pointer, const size_t byteLength);

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
	};

#pragma region Implement
	/****************************************************************************
	*                    ForceExecuteDefaultConstructors
	*************************************************************************//**
	*  @fn       �@template<class ElementType> void Memory::ForceExecuteDefaultConstructors(void* address, const uint64 count)
	*
	*  @brief     �f�t�H���g�R���X�g���N�^�������I�ɌĂяo���܂�.  
	*             Memory::Allocate���Ăяo���O��̏����ƂȂ��Ă��܂�.
	*
	*  @param[in] void* �Ăяo���擪�|�C���^
	*  @param[in] �z��̃T�C�Y (!�o�C�g�T�C�Y�ł͂���܂���)
	*
	*  @return �@�@void
	*****************************************************************************/
	template<class ElementType>
	void Memory::ForceExecuteDefaultConstructors(void* address, const uint64 count)
	{
		if constexpr (HAS_TRIVIAL_CONSTRUCTOR<ElementType>)
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

	/****************************************************************************
	*                    ForceExecuteDestructors
	*************************************************************************//**
	*  @fn       �@void Memory::ForceExecuteDestructor(ElementType* element)
	*
	*  @brief     �f�X�g���N�^�������I�ɌĂяo���܂�
	*             Memory::Allocate���Ăяo���O��̏����ƂȂ��Ă��܂�.
	*
	*  @param[in] void* �Ăяo���擪�|�C���^
	*
	*  @return �@�@void
	*****************************************************************************/
	template<class ElementType>
	void Memory::ForceExecuteDestructors(ElementType* element, const uint64 count)
	{
		if constexpr (HAS_TRIVIAL_DESTRUCTOR<ElementType>) { return; }

		for (uint64 i = 0; i < count; ++i)
		{
			element->ElementType::~ElementType();
			++element;
		}
	}

	/****************************************************************************
	*                    ForceExecuteCopyConstructors
	*************************************************************************//**
	*  @fn       �@static void ForceExecuteCopyAssignOperators(ElementType* destination, const ElementType* source, const uint64 count);
	*
	*  @brief     �����I�ɃR�s�[������Z�q���Ăяo��
	*
	*  @param[in] ElementType* �Ăяo���擪�|�C���^
	*  @param[in] ElementType* �R�s�[��
	*  @param[in] uint64 �z��̃T�C�Y
	*
	*  @return �@�@void
	*****************************************************************************/
	template<class ElementType>
	void Memory::ForceExecuteCopyAssignOperators(ElementType* destination, const ElementType* source, const uint64 count)
	{
		if constexpr (HAS_TRIVIAL_ASSIGN_OPERATOR<ElementType>)
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

	/****************************************************************************
	*                    ForceExecuteCopyAssignOperators
	*************************************************************************//**
	*  @fn       �@static void Memory::ForceExecuteCopyConstructors(ElementType* destination, const ElementType* source, const uint64 count)
	*
	*  @brief     �����I�ɃR�s�[�R���X�g���N�^���Ăяo��
	*
	*  @param[in] ElementType* �Ăяo���擪�|�C���^
	*  @param[in] ElementType* �R�s�[��
	*  @param[in] uint64 �z��̃T�C�Y
	*
	*  @return �@�@void
	*****************************************************************************/
	template<class ElementType>
	void Memory::ForceExecuteCopyConstructors(ElementType* destination, const ElementType* source, const uint64 count)
	{
		if constexpr (HAS_TRIVIAL_COPY_CONSTRUCTOR<ElementType>)
		{
			Memory::Copy(destination, source, sizeof(ElementType) * count)
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


	/*---------------------------------------------------------------
		@brief :  �����I�Ƀ��[�u
	-----------------------------------------------------------------*/
	template<class ElementType>
	static void Memory::ForceExecuteMoveConstructors(ElementType* destination, const ElementType* source, const uint64 count)
	{
		if constexpr (HAS_TRIVIAL_COPY_CONSTRUCTOR<ElementType>) // ���[�u�̔��肪�Ȃ�����
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

	/*---------------------------------------------------------------
		@brief :  �����I�Ƀ��[�u
	-----------------------------------------------------------------*/
	template<class ElementType>
	static void Memory::ForceExecuteMoveAssignOperators(ElementType* destination, const ElementType* source, const uint64 count)
	{
		if constexpr (HAS_TRIVIAL_ASSIGN_OPERATOR<ElementType>) // ���[�u�̔��肪�Ȃ�����
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