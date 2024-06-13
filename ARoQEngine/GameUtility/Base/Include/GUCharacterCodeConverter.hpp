//////////////////////////////////////////////////////////////////////////////////
///  @file   GUCharacterCodeConverter.hpp
///  @brief  文字コードを変換します.
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
	/*   @brief   文字コードを変換します.
	*****************************************************************************/
	class CharacterCodeConverter
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     文字コードを変換し, バイト列を返します
		*  @param[in] void
		*  @return    bool 変換に成功した場合はtrue, 失敗した場合はfalse
		*************************************************************************/
		gu::DynamicArray<uint8>& Convert(const uint8* input, const uint64 inputByteSize);

		#pragma endregion 

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     変換元のテキストの文字コードを取得します
		*  @param[in] void
		*  @return    SharedPointer<CharacterCode> 変換元の文字コード
		*************************************************************************/
		__forceinline SharedPointer<CharacterCode> GetSourceCharacterCode() const
		{
			return _sourceCharacterCode;
		}

		/*!**********************************************************************
		*  @brief     変換先のテキストの文字コードを取得します
		*  @param[in] void
		*  @return    SharedPointer<CharacterCode> 変換先の文字コード
		*************************************************************************/
		__forceinline SharedPointer<CharacterCode> GetDestinationCharacterCode() const
		{
			return _destinationCharacterCode;
		}

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief デフォルトコンストラクタ*/
		CharacterCodeConverter() = default;

		/*! @brief デストラクタ*/
		~CharacterCodeConverter() = default;

		/*! @brief 文字コードを指定して初期化します*/
		CharacterCodeConverter(const CharacterCodeType before,  const CharacterCodeType after, 
			const bool beforeUseBOM      = false, const bool afterUseBOM      = false, 
			const bool beforeIsBigEndian = false, const bool afterIsBigEndian = false)
		{
			_sourceCharacterCode      = CharacterCode::GetCharacterCode(before, beforeUseBOM, beforeIsBigEndian);
			_destinationCharacterCode = CharacterCode::GetCharacterCode(after , afterUseBOM , afterIsBigEndian);
		}

		/*! @brief 文字コードのポインタを指定して初期化します. */
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
		/*! @brief 変換前の文字コード*/
		SharedPointer<CharacterCode> _sourceCharacterCode = nullptr;

		/*! @brief 変換後の文字コード*/
		SharedPointer<CharacterCode> _destinationCharacterCode = nullptr;

		/*! @brief 出力バッファ*/
		gu::DynamicArray<uint8> _outputBuffer = {};
		#pragma endregion 

	};
}

#endif