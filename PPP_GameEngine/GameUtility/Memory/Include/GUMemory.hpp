//////////////////////////////////////////////////////////////////////////////////
///             @file   GUMemory.hpp
///             @brief  string.hのラッパークラスです. 不要なincludeを減らしているため, 軽量です.
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
			@brief :  指定した分だけメモリの初期化を行います.
		-----------------------------------------------------------------*/
		static void* Set(void* destination, const uint8 oneByteValue, const uint64 byteLength);

		/*---------------------------------------------------------------
			@brief :  指定したバイト長だけゼロに初期化します.
		-----------------------------------------------------------------*/
		static void* Zero(void* destination, const uint64 byteLength);

		/*---------------------------------------------------------------
			@brief :  指定した分だけメモリを移動します.
		-----------------------------------------------------------------*/
		static void* Move(void* destination, const void* source, const uint64 byteLength);

		/*---------------------------------------------------------------
			@brief :  メモリを指定したバイト数分だけコピー
		-----------------------------------------------------------------*/
		static void* Copy(void* destination, const void* source, const uint64 byteLength);

		/*---------------------------------------------------------------
			@brief :  文字を比較する (+) left > right, (0) left == right (-) left < right
		-----------------------------------------------------------------*/
		static int32 Compare(const void* left, const void* right, const uint64 byteLength);

		/*---------------------------------------------------------------
			@brief :  メモリをコンストラクタを呼び出すことなくメモリ領域だけを確保します (Malloc)
		-----------------------------------------------------------------*/
		static void* Allocate       (const uint64 byteLength);
		static void* AllocateAligned(const uint64 byteLength, const uint64 alignment);

		/*---------------------------------------------------------------
			@brief :  メモリをデストラクタを呼び出すことなくメモリ領域だけを破棄します (Free)
		-----------------------------------------------------------------*/
		static void Free(void* pointer);
		static void FreeAligned(void* pointer);

#pragma region Force Class Functiokn
		/*---------------------------------------------------------------
			@brief :  デフォルトコンストラクタを強制的に呼び出します
			          Memory::Allocateを呼び出す前提の処理
		-----------------------------------------------------------------*/
		template<typename ElementType>
		static void ForceExecuteDefaultConstructors(void* address, const uint64 count);

		/*---------------------------------------------------------------
			@brief :  デストラクタを強制的に呼び出します
		-----------------------------------------------------------------*/
		template<typename ElementType>
		static void ForceExecuteDestructors(ElementType* address, const uint64 count);

		/*---------------------------------------------------------------
			@brief :  強制的にコピーコンストラクタを呼び出す
		-----------------------------------------------------------------*/
		template<class ElementType>
		static void ForceExecuteCopyConstructors(ElementType* destination, const ElementType* source, const uint64 count);
		
		/*---------------------------------------------------------------
			@brief :  強制的にコピー代入演算子を呼び出す
		-----------------------------------------------------------------*/
		template<class ElementType>
		static void ForceExecuteCopyAssignOperators(ElementType* destination, const ElementType* source, const uint64 count);

		/*---------------------------------------------------------------
			@brief :  強制的にムーブコンストラクタを呼び出す
		-----------------------------------------------------------------*/
		template<class ElementType>
		static void ForceExecuteMoveConstructors(ElementType* destination, const ElementType* source, const uint64 count);

		/*---------------------------------------------------------------
			@brief :  強制的にムーブ代入演算子を呼び出す
		-----------------------------------------------------------------*/
		template<class ElementType>
		static void ForceExecuteMoveAssignOperators(ElementType* destination, const ElementType* source, const uint64 count);

#pragma endregion Force Class Function
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/*---------------------------------------------------------------
			@brief :  指定したバイトがゼロに設定されているか
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
	*  @fn       　template<class ElementType> void Memory::ForceExecuteDefaultConstructors(void* address, const uint64 count)
	*
	*  @brief     デフォルトコンストラクタを強制的に呼び出します.  
	*             Memory::Allocateを呼び出す前提の処理となっています.
	*
	*  @param[in] void* 呼び出す先頭ポインタ
	*  @param[in] 配列のサイズ (!バイトサイズではありません)
	*
	*  @return 　　void
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
				new (element) ElementType; // デフォルトコンストラクタを呼び出す
				++element;
			}
		}
	}

	/****************************************************************************
	*                    ForceExecuteDestructors
	*************************************************************************//**
	*  @fn       　void Memory::ForceExecuteDestructor(ElementType* element)
	*
	*  @brief     デストラクタを強制的に呼び出します
	*             Memory::Allocateを呼び出す前提の処理となっています.
	*
	*  @param[in] void* 呼び出す先頭ポインタ
	*
	*  @return 　　void
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
	*  @fn       　static void ForceExecuteCopyAssignOperators(ElementType* destination, const ElementType* source, const uint64 count);
	*
	*  @brief     強制的にコピー代入演算子を呼び出す
	*
	*  @param[in] ElementType* 呼び出す先頭ポインタ
	*  @param[in] ElementType* コピー元
	*  @param[in] uint64 配列のサイズ
	*
	*  @return 　　void
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
	*  @fn       　static void Memory::ForceExecuteCopyConstructors(ElementType* destination, const ElementType* source, const uint64 count)
	*
	*  @brief     強制的にコピーコンストラクタを呼び出す
	*
	*  @param[in] ElementType* 呼び出す先頭ポインタ
	*  @param[in] ElementType* コピー元
	*  @param[in] uint64 配列のサイズ
	*
	*  @return 　　void
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
		@brief :  強制的にムーブ
	-----------------------------------------------------------------*/
	template<class ElementType>
	static void Memory::ForceExecuteMoveConstructors(ElementType* destination, const ElementType* source, const uint64 count)
	{
		if constexpr (HAS_TRIVIAL_COPY_CONSTRUCTOR<ElementType>) // ムーブの判定がないため
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
		@brief :  強制的にムーブ
	-----------------------------------------------------------------*/
	template<class ElementType>
	static void Memory::ForceExecuteMoveAssignOperators(ElementType* destination, const ElementType* source, const uint64 count)
	{
		if constexpr (HAS_TRIVIAL_ASSIGN_OPERATOR<ElementType>) // ムーブの判定がないため
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