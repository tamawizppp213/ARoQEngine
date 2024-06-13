//////////////////////////////////////////////////////////////////////////////////
///  @file   GUMemory.hpp
///  @brief  string.hのラッパークラスです. 不要なincludeを減らしているため, 軽量です.@n
///          本クラスでは, メモリの操作を行います. メモリの確保, 解放, コピー, 移動, 比較, ゼロクリアなどの操作が可能です.@n
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
	/*   @brief     本クラスでは, メモリの操作を行います. メモリの確保, 解放, コピー, 移動, 比較, ゼロクリアなどの操作が可能です.
	*****************************************************************************/
	class Memory
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     1byteの値で指定したバイト長だけ初期化します.
		*  @param[out] void* 初期化する先頭アドレス
		*  @param[in] const uint8 1byteの値
		*  @param[in] const uint64 初期化するバイト長
		*  @return    void*
		*************************************************************************/
		static void* Set(void* destination, const uint8 oneByteValue, const uint64 byteLength);

		/*!**********************************************************************
		*  @brief     指定したバイト長だけゼロに初期化します.
		*  @param[out]void* 0クリアする先頭アドレス
		*  @param[in] uint64 ゼロクリアするバイト長
		*  @return    void*
		*************************************************************************/
		static void* Zero(void* destination, const uint64 byteLength);

		/*!**********************************************************************
		*  @brief     指定した分だけメモリを移動します. memmoveと同じため, オーバーラップしても問題ありません.
		*  @param[out]void* 移動先のアドレス
		*  @param[in] const void* 移動元のアドレス
		*  @param[in] const uint64 移動するバイト長
		*  @return    void*
		*************************************************************************/
		static void* Move(void* destination, const void* source, const uint64 byteLength);

		/*!**********************************************************************
		*  @brief    メモリを指定したバイト数分だけコピー. memcpyと同じため, オーバーラップしていると問題が発生します.
		*  @param[out]void* 移動先のアドレス
		*  @param[in] const void* 移動元のアドレス
		*  @param[in] const uint64 移動するバイト長
		*  @return    void*
		*************************************************************************/
		static void* Copy(void* destination, const void* source, const uint64 byteLength);

		/*!**********************************************************************
		*  @brief     文字を比較する (+) left > right, (0) left == right (-) left < right
		*  @param[in] const void* 比較する左のアドレス
		*  @param[in] const void* 比較する右のアドレス
		*  @param[in] const uint64 比較するバイト長
		*  @return    int32 バッファー間で最初に異なるバイトの差を返します. 値に意味はあまりないため, 重要なのはその符号になります.
		*************************************************************************/
		static int32 Compare(const void* left, const void* right, const uint64 byteLength);

		/*!**********************************************************************
		*  @brief     メモリをコンストラクタを呼び出すことなくメモリ領域だけを確保します (Malloc)
		*  @param[in] const uint64 確保する文字列長
		*  @return    void* 確保したメモリの先頭アドレス
		*************************************************************************/
		static void* Allocate       (const uint64 byteLength);

		/*!**********************************************************************
		*  @brief     メモリをコンストラクタを呼び出すことなくメモリ領域だけを確保します (Malloc) アライメントを指定できます.
		*  @param[in] const uint64 確保する文字列長
		*  @param[in] const uint64 アライメント
		*  @return    void* 確保したメモリの先頭アドレス
		*************************************************************************/
		static void* AllocateAligned(const uint64 byteLength, const uint64 alignment);

		/*!**********************************************************************
		*  @brief     メモリ領域を再割り当てします pointer == nullptr -> mallocと同じ @n
		*             byteLength < 既存のメモリ : アドレスのサイズ情報を書き換えて既存のポインタを返す @n
		*             byteLength = 既存のメモリ : 何もしないで返す @n
		*             byteLength > 既存のメモリ : 拡張可能であればサイズ変更, 不可能だけでメモリは足りてれば, 新しいアドレスにMoveして確保, メモリ不足はnullを返す@n
		*             https://qiita.com/sawasaka/items/2df474fc00ec2105acab
		*  @param[in] void* 再割り当てするメモリの先頭アドレス
		*  @param[in] const uint64 確保するバイト長
		*  @return    void* 再確保したメモリの先頭アドレス
		*************************************************************************/
		static void* Reallocate(void* pointer, const uint64 byteLength);

		/*!**********************************************************************
		*  @brief     メモリをデストラクタを呼び出すことなくメモリ領域だけを破棄します (Free)
		*  @param[in] void* 破棄するメモリの先頭アドレス
		*  @return    void
		*************************************************************************/
		static void Free(void* pointer);

		/*!**********************************************************************
		*  @brief     メモリをデストラクタを呼び出すことなくメモリ領域だけを破棄します. アライメントを指定できます.
		*  @param[in] void* 破棄するメモリの先頭アドレス
		*  @return    void
		*************************************************************************/
		static void FreeAligned(void* pointer);

		#pragma region Force Class Functiokn
		/*!**********************************************************************
		*  @brief     デフォルトコンストラクタを強制的に呼び出します @n
		*             Memory::Allocateを呼び出す前提の処理
		*  @param[in] void* コンストラクタを呼び出したいアドレス. templateで型を指定してください
		*  @param[in] const uint64 コンストラクタを呼び出したい配列のサイズ
		*  @return    void
		*************************************************************************/
		template<typename ElementType>
		static void ForceExecuteDefaultConstructors(void* address, const uint64 count);

		/*!**********************************************************************
		*  @brief     デストラクタを強制的に呼び出します
		*  @param[in] void* デストラクタを呼び出したいアドレス.
		*  @param[in] const uint64 デストラクタを呼び出したい配列のサイズ
		*  @return    void
		*************************************************************************/
		template<typename ElementType>
		static void ForceExecuteDestructors(ElementType* address, const uint64 count);

		/*!**********************************************************************
		*  @brief     強制的にコピーコンストラクタを呼び出す
		*  @param[in] ElementType* コピー先
		*  @param[in] ElementType* コピー元
		*  @param[in] const uint64 コピーコンストラクタを呼び出したい配列のサイズ
		*  @return    void
		*************************************************************************/
		template<class ElementType>
		static void ForceExecuteCopyConstructors(ElementType* destination, const ElementType* source, const uint64 count);
		
		/*!**********************************************************************
		*  @brief     強制的にコピー代入演算子を呼び出す
		*  @param[in] ElementType* コピー先
		*  @param[in] ElementType* コピー元
		*  @param[in] const uint64 コピーコンストラクタを呼び出したい配列のサイズ
		*  @return    void
		*************************************************************************/
		template<class ElementType>
		static void ForceExecuteCopyAssignOperators(ElementType* destination, const ElementType* source, const uint64 count);

		/*!**********************************************************************
		*  @brief     強制的にムーブコンストラクタを呼び出す
		*  @param[in] ElementType* ムーブ先
		*  @param[in] ElementType* ムーブ元
		*  @param[in] const uint64 ムーブコンストラクタを呼び出したい配列のサイズ
		*  @return    void
		*************************************************************************/
		template<class ElementType>
		static void ForceExecuteMoveConstructors(ElementType* destination, const ElementType* source, const uint64 count);

		/*!**********************************************************************
		*  @brief     強制的にムーブ代入演算子を呼び出す
		*  @param[in] ElementType* ムーブ先
		*  @param[in] ElementType* ムーブ元
		*  @param[in] const uint64 ムーブコンストラクタを呼び出したい配列のサイズ
		*  @return    void
		*************************************************************************/
		template<class ElementType>
		static void ForceExecuteMoveAssignOperators(ElementType* destination, const ElementType* source, const uint64 count);

		#pragma endregion Force Class Function
		#pragma endregion
		
		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     指定したバイトがゼロに設定されているか
		*  @param[in] const void* 比較するアドレス
		*  @param[in] const uint64 比較するバイト長
		*  @return    bool ゼロに設定されている場合はtrue, それ以外はfalse
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
				new (element) ElementType; // デフォルトコンストラクタを呼び出す
				++element;
			}
		}
	}

	template<class ElementType>
	void Memory::ForceExecuteDestructors(ElementType* element, const uint64 count)
	{
		// 早期リターンを行うとコンパイラが制御が渡らないコードですと警告が出る場合があるため, 早期リターンを行わない
		if constexpr (!type::HAS_TRIVIAL_DESTRUCTOR<ElementType>)
		{
			// ここでは, ElementTypeというメンバがあった場合にコンパイルをしてくれないという記述がUE5にあったため.
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
		if constexpr (type::HAS_TRIVIAL_COPY_CONSTRUCTOR<ElementType>) // ムーブの判定がないため
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
		if constexpr (type::HAS_TRIVIAL_ASSIGN_OPERATOR<ElementType>) // ムーブの判定がないため
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