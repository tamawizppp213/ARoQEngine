//////////////////////////////////////////////////////////////////////////////////
/// @file   GUOptional.hpp @n
/// @brief  Optionalは, 値の有効無効が確認できるクラスです. @n
///         HasValueで値が有効値かを確認する @n
///         Valueで実際の値を取得する. @n
/// @author toide
/// @date   2024/02/26 0:54:45
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_OPTIONAL_HPP
#define GU_OPTIONAL_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUTypeCast.hpp"
#include "GameUtility/Base/Include/GUAssert.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu
{
	/****************************************************************************
	*				  			   GUOptional
	****************************************************************************/
	/* @class     GUOptional
	*  @brief     Optionalは, 値の有効無効が確認できるクラスです. @n
    *             HasValueで値が有効値かを確認する @n
    *             Valueで実際の値を取得する.@n 
	*       	  https://qiita.com/ashtkn/items/d6de4a9f7524eadb4222
	*****************************************************************************/
	template<class ElementType>
	class Optional
	{
	public:
		#pragma region Public Function
		/*!***********************************************************************
		*  @brief      値が有効値(何かしらの値が代入されている状態)かを確認します. @n
		*              trueであれば有効値が設定されているものとします. 
		*  @param[in]  void
		*  @return     bool
		**************************************************************************/
		constexpr bool HasValue() const noexcept { return _hasValue; }

		/*!***********************************************************************
		*  @brief      値を無効値の場合と同じ状態に初期化を行います. 
		*  @param[in]  void
		*  @return     void
		**************************************************************************/
		__forceinline void Reset() { _hasValue = false; _value = ElementType(); }

		/*-------------------------------------------------------------------
		-           値の取得
		---------------------------------------------------------------------*/
		/*! @brief : 保持する値の左辺値参照を取得する*/
		constexpr ElementType& Value()&
		{
			if (!_hasValue) { Check(false); }
			return _value;
		}

		/*! @brief : 保持する値の右辺値参照を取得する*/
		constexpr ElementType&& Value()&&
		{
			if (!_hasValue) { Check(false); }
			return gu::Forward<ElementType>(_value)
		}

		/*! @brief : 保持する値の左辺値参照を取得する*/
		constexpr const ElementType& Value() const&
		{
			if (!_hasValue) { Check(false); }
			return _value;
		}

		/*! @brief : 保持する値の右辺値参照を取得する*/
		constexpr const ElementType&& Value() const&&
		{
			if (!_hasValue) { Check(false); }
			return gu::Forward<ElementType>(_value)
		}

		#pragma endregion Public Function
		#pragma region Public Operator Function
		/*! @brief : 有効値かどうかを返す */
		constexpr explicit operator bool() const noexcept { return _hasValue; }

		/*! @brief : メンバアクセス演算子 */
		constexpr       ElementType* operator->()       noexcept { return &_value; }
		constexpr const ElementType* operator->() const noexcept { return &_value; }

		/*! @brief : 間接参照演算子 (左辺値参照)*/
		constexpr ElementType& operator*() & noexcept { return _value; }

		/*! @brief : 間接参照演算子 (右辺値参照) */
		constexpr ElementType&& operator*() && noexcept { return gu::Forward<Optional<ElementType>>(_value); }

		/*! @brief : 間接参照演算子 (const 左辺値参照) */
		constexpr const ElementType& operator* () const& noexcept { return _value; }

		/*! @brief : 間接参照演算子 8const 右辺値参照) */
		constexpr const ElementType&& operator*() const&& noexcept { return gu::Forward<Optional<ElementType>>(_value); }

		#pragma endregion Public Operator Function

		#pragma region Public Constructor and Destructor
		/*! @brief: 値を持っていないオブジェクトを構築する */
		constexpr Optional() noexcept : _value(), _hasValue(false) {};

		/*! @brief : 値を持っているオブジェクトを構築する */
		constexpr Optional(const ElementType& value) : _value(value), _hasValue(true) {};

		/*! @brief : コピーコンストラクタ */
		constexpr Optional(const Optional& other) : _value(other._value), _hasValue(other._hasValue) {};

		/*! @brief : ムーブコンストラクタ */
		constexpr Optional(Optional<ElementType>&& other) : _value(gu::Forward<Optional<ElementType>>(other._value)), _hasValue(other._hasValue)
		{
			other._hasValue = false;
		}

		~Optional() = default;

		/*! @brief : コピー代入演算子 */
		Optional& operator=(const Optional& other)
		{
			if (this != &other) { _value = other._value; _hasValue = other._hasValue; }
			return *this;
		}

		/*! ムーブ代入演算子 */
		Optional& operator=(Optional<ElementType>&& other) noexcept
		{
			if (this != &other)
			{
				_value = other.Value();
				_hasValue = other._hasValue;
				other._hasValue = false;
			}

			return *this;
		}

		#pragma endregion Public Constructor and Destructor

	protected:
		#pragma region Protected Function

		#pragma endregion Protected Function
	
		#pragma region Protected Property

		/*! @brief : 実際の値 */
		ElementType _value = ElementType();

		/*! @brief : 値が代入されているか */
		bool _hasValue = false;

		#pragma endregion Private Property
	};

}
#endif