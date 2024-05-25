//////////////////////////////////////////////////////////////////////////////////
///  @file   GUCharacterCode.cpp
///  @brief  temp
///  @author toide
///  @date   2024/05/25 20:03:23
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Private/CharacterCode/Include/GUCharacterCode.hpp"
#include "GameUtility/Base/Private/CharacterCode/Include/GUAscii.hpp"
#include "GameUtility/Base/Private/CharacterCode/Include/GUShiftJIS.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gu;
using namespace gu::details::string;
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor

#pragma endregion Constructor and Destructor

#pragma region Public Function
/*!**********************************************************************
*  @brief     環境依存のワイド文字列で使用される文字コードを取得します
*  @param[in] void
*  @return    SharedPointer<CharacterCode> 文字コード
*************************************************************************/
SharedPointer<CharacterCode> CharacterCode::WideStringCharacterCode()
{
	return nullptr;
}

/*!**********************************************************************
*  @brief     TStringで使用される文字コードを取得します
*  @param[in] void
*  @return    SharedPointer<CharacterCode> 文字コード
*************************************************************************/
SharedPointer<CharacterCode> CharacterCode::TStringCharacterCode()
{
	return nullptr;
}

/*!**********************************************************************
*  @brief     指定した文字コードを取得します
*  @param[in] const CharacterCodeType 文字コードの種類
*  @return    SharedPointer<CharacterCode> 文字コード
*************************************************************************/
SharedPointer<CharacterCode> CharacterCode::GetCharacterCode(const CharacterCodeType codeType)
{
	switch (codeType)
	{
		case CharacterCodeType::ASCII:
		{
			return MakeShared<Ascii>();
		}
		case CharacterCodeType::ShiftJIS:
		{
			return MakeShared<ShiftJIS>();
		}
	}
}
#pragma endregion Public Function