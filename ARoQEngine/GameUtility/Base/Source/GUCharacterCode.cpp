//////////////////////////////////////////////////////////////////////////////////
///  @file   GUCharacterCode.cpp
///  @brief  temp
///  @author toide
///  @date   2024/05/25 20:03:23
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUCharacterCode.hpp"
#include "GameUtility/Base/Private/CharacterCode/Include/GUAscii.hpp"
#include "GameUtility/Base/Private/CharacterCode/Include/GUShiftJIS.hpp"
#include "GameUtility/Base/Private/CharacterCode/Include/GUUTF8.hpp"
#include "GameUtility/Base/Private/CharacterCode/Include/GUUTF16.hpp"
#include "GameUtility/Base/Private/CharacterCode/Include/GUUTF32.hpp"
#include "GameUtility/Base/Include/GUTypeTraits.hpp"

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
	if constexpr(sizeof(wchar) == 2)
	{
		return MakeShared<UTF16>(false , false);
	}
	else if constexpr(sizeof(wchar) == 4)
	{
		return MakeShared<UTF32>(false , false);
	}
	else
	{
		return nullptr;
	}
}

/*!**********************************************************************
*  @brief     TStringで使用される文字コードを取得します
*  @param[in] void
*  @return    SharedPointer<CharacterCode> 文字コード
*************************************************************************/
SharedPointer<CharacterCode> CharacterCode::TStringCharacterCode()
{
	if constexpr (gu::type::IS_SAME<tchar, wchar>)
	{
		return WideStringCharacterCode();
	}
	else if constexpr (gu::type::IS_SAME<tchar, char16>)
	{
		return MakeShared<UTF16>(false , false);
	}
	else
	{
		return MakeShared<UTF8>(false);
	}
}

/*!**********************************************************************
*  @brief     指定した文字コードを取得します
*  @param[in] const CharacterCodeType 文字コードの種類
*  @return    SharedPointer<CharacterCode> 文字コード
*************************************************************************/
SharedPointer<CharacterCode> CharacterCode::GetCharacterCode(const CharacterCodeType codeType, const bool useBOM, const bool isBigEndian)
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
		case CharacterCodeType::UTF8:
		{
			return MakeShared<UTF8>(useBOM);
		}
		case CharacterCodeType::UTF16:
		{
			return MakeShared<UTF16>(useBOM, isBigEndian);
		}
		case CharacterCodeType::UTF32:
		{
			return MakeShared<UTF32>(useBOM, isBigEndian);
		}
		default:
		{
			return nullptr;
		}
	}
}
#pragma endregion Public Function