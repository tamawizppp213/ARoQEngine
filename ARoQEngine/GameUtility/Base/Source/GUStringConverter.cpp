//////////////////////////////////////////////////////////////////////////////////
///  @file   GUStringConverter.cpp
///  @brief  •¶Žš—ñ‚ð•ÏŠ·‚·‚é‹@”\‚ð’ñ‹Ÿ‚µ‚Ü‚·.
///  @author toide
///  @date   2024/05/19 18:17:56
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUStringConverter.hpp"
#include "GameUtility/Base/Private/CharacterCode/Include/GUUnicodeConverter.hpp"
#include "GameUtility/Base/Private/Base/Include/GUTypeTraitsStruct.hpp"
#include "GameUtility/Base/Include/GUCharacterCode.hpp"
#include <stdlib.h>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gu;
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor

#pragma endregion Constructor and Destructor

#pragma region Public Function
/*!**********************************************************************
*  @brief     UTF8‚©‚çUTF16‚É•¶Žš—ñ‚ð•ÏŠ·‚µ‚Ü‚·.
*  @param[in] const string& utf8
*  @return    u16string
*************************************************************************/
u16string StringConverter::ConvertUTF8ToUTF16(const u8string& utf8)
{
	u16string result(utf8.Size() * 2, true);

	gu::details::string::UnicodeConverter::Options options = {};
	options.ReplacementChar = '?';

	const auto convertedResult = gu::details::string::UnicodeConverter::ConvertUTF8ToUTF16
	(
		reinterpret_cast<const uint8*>(utf8.CString()),
		utf8.Size(),
		reinterpret_cast<uint16*>(result.CString()),
		result.Size(),
		options
	);

	if (convertedResult != gu::details::string::UnicodeConvertResult::Success)
	{
		throw "Convert failed";
	}

	return u16string(result.CString(), options.ConvertedTargetLength);
}

/*!**********************************************************************
*  @brief     UTF8‚©‚çUTF32‚É•¶Žš—ñ‚ð•ÏŠ·‚µ‚Ü‚·.
*  @param[in] const string& utf8
*  @return    u16string
*************************************************************************/
u32string StringConverter::ConvertUTF8ToUTF32(const u8string& utf8)
{
	u32string result(utf8.Size() * 2, true);

	gu::details::string::UnicodeConverter::Options options = {};
	options.ReplacementChar = '?';

	const auto convertedResult = gu::details::string::UnicodeConverter::ConvertUTF8ToUTF32
	(
		reinterpret_cast<const uint8*>(utf8.CString()),
		utf8.Size(),
		reinterpret_cast<uint32*>(result.CString()),
		result.Size(),
		options
	);

	if (convertedResult != gu::details::string::UnicodeConvertResult::Success)
	{
		throw "Convert failed";
	}

	return u32string(result.CString(), options.ConvertedTargetLength);

}

/*!**********************************************************************
*  @brief     UTF16‚©‚çUTF32‚É•¶Žš—ñ‚ð•ÏŠ·‚µ‚Ü‚·.
*  @param[in] const u16string& utf16
*  @return    u16string
*************************************************************************/
u8string StringConverter::ConvertUTF16ToUTF8(const u16string& utf16)
{
	u8string result(utf16.Size() * 4, true);

	gu::details::string::UnicodeConverter::Options options = {};
	options.ReplacementChar = '?';

	const auto convertedResult = gu::details::string::UnicodeConverter::ConvertUTF16ToUTF8
	(
		reinterpret_cast<const uint16*>(utf16.CString()),
		utf16.Size(),
		reinterpret_cast<uint8*>(result.CString()),
		result.Size(),
		options
	);

	if (convertedResult != gu::details::string::UnicodeConvertResult::Success)
	{
		throw "Convert failed";
	}

	return u8string(result.CString(), options.ConvertedTargetLength);
}

/*!**********************************************************************
*  @brief     UTF16‚©‚çUTF32‚É•¶Žš—ñ‚ð•ÏŠ·‚µ‚Ü‚·.
*  @param[in] const u16string& utf16
*  @return    u16string
*************************************************************************/
u32string StringConverter::ConvertUTF16ToUTF32(const u16string& utf16)
{
	u32string result(utf16.Size() * 2, true);

	gu::details::string::UnicodeConverter::Options options = {};
	options.ReplacementChar = '?';

	const auto convertedResult = gu::details::string::UnicodeConverter::ConvertUTF16ToUTF32
	(
		reinterpret_cast<const uint16*>(utf16.CString()),
		utf16.Size(),
		reinterpret_cast<uint32*>(result.CString()),
		result.Size(),
		options
	);

	if (convertedResult != gu::details::string::UnicodeConvertResult::Success)
	{
		throw "Convert failed";
	}

	return u32string(result.CString(), options.ConvertedTargetLength);
}

/*!**********************************************************************
*  @brief     UTF32‚©‚çUTF8‚É•¶Žš—ñ‚ð•ÏŠ·‚µ‚Ü‚·.
*  @param[in] const u32string& utf32
*  @return    string
*************************************************************************/
string StringConverter::ConvertUTF32ToUTF8(const u32string& utf32)
{
	string result(utf32.Size() * 4, true);

	gu::details::string::UnicodeConverter::Options options = {};
	options.ReplacementChar = '?';

	const auto convertedResult = gu::details::string::UnicodeConverter::ConvertUTF32ToUTF8
	(
		reinterpret_cast<const uint32*>(utf32.CString()),
		utf32.Size(),
		reinterpret_cast<uint8*>(result.CString()),
		result.Size(),
		options
	);

	if (convertedResult != gu::details::string::UnicodeConvertResult::Success)
	{
		throw "Convert failed";
	}

	return string(result.CString(), options.ConvertedTargetLength);
}

/*!**********************************************************************
*  @brief     UTF32‚©‚çUTF16‚É•¶Žš—ñ‚ð•ÏŠ·‚µ‚Ü‚·.
*  @param[in] const u32string& utf32
*  @return    u16string
*************************************************************************/
u16string StringConverter::ConvertUTF32ToUTF16(const u32string& utf32)
{
	u16string result(utf32.Size() * 2, true);

	gu::details::string::UnicodeConverter::Options options = {};
	options.ReplacementChar = '?';

	const auto convertedResult = gu::details::string::UnicodeConverter::ConvertUTF32ToUTF16
	(
		reinterpret_cast<const uint32*>(utf32.CString()),
		utf32.Size(),
		reinterpret_cast<uint16*>(result.CString()),
		result.Size(),
		options
	);

	if (convertedResult != gu::details::string::UnicodeConvertResult::Success)
	{
		throw "Convert failed";
	}

	return u16string(result.CString(), options.ConvertedTargetLength);

}

/*!**********************************************************************
*  @brief     String‚©‚ç‚ÉWString‚É•¶Žš—ñ‚ð•ÏŠ·‚µ‚Ü‚·.
*  @param[in] const string& utf8
*  @return    wstring
*************************************************************************/
wstring StringConverter::ConvertStringToWString(const string& utf8String)
{
	const auto codeType      = gu::details::string::CharacterCodeType::ShiftJIS;
	const auto characterCode = gu::details::string::CharacterCode::GetCharacterCode(codeType);

	/*-------------------------------------------------------------------
	-              wchar_t <=> char_16_t
	---------------------------------------------------------------------*/
	if constexpr (sizeof(wchar) == sizeof(char16))
	{
		gu::u16string utf16String(utf8String.Size() * 2, true);
		gu::details::string::CharacterDecodeResult result = {};

		characterCode->ToUTF16(
			reinterpret_cast<const uint8*>(utf8String.CString()), utf8String.Size(),
			const_cast<uint16*>(reinterpret_cast<const uint16*>(utf16String.CString())), utf16String.Size(), &result);

		gu::wstring test;
		test.Assign(reinterpret_cast<const wchar_t*>(utf16String.CString()), result.OutputCharacterCount);
		return gu::wstring(reinterpret_cast<const wchar_t*>(utf16String.CString(), result.OutputCharacterCount));
	}
	/*-------------------------------------------------------------------
	-              wchar_t <=> char_32_t
	---------------------------------------------------------------------*/
	else if constexpr (sizeof(wchar) == sizeof(char32))
	{
		gu::u8string  utf8String(reinterpret_cast<const char8*>(utf8String.CString()));
		gu::u32string utf32String = ConvertUTF8ToUTF32(utf8String.CString());

		return gu::wstring(reinterpret_cast<const wchar_t*>(utf32String.CString()));
	}
	/*-------------------------------------------------------------------
	-              Error check
	---------------------------------------------------------------------*/
	else
	{
		throw "Error: wchar_t size is not supported.";
	}
}

/*!**********************************************************************
*  @brief     WString‚©‚ç‚ÉString‚É•¶Žš—ñ‚ð•ÏŠ·‚µ‚Ü‚·.
*  @param[in] const wstring& utf8
*  @return    wstring
*************************************************************************/
string StringConverter::ConvertWStringToString(const wstring& wideString)
{
	/*-------------------------------------------------------------------
	-              wchar_t <=> char_16_t
	---------------------------------------------------------------------*/ 
	if constexpr (sizeof(wchar) == sizeof(char16))
	{
		const u16string utf16String = u16string(wideString.Size(), true);

		return string(reinterpret_cast<const char*>(ConvertUTF16ToUTF8(utf16String).CString()));
	}
	/*-------------------------------------------------------------------
	-              wchar_t <=> char_32_t
	---------------------------------------------------------------------*/
	else if constexpr (sizeof(wchar) == sizeof(char32))
	{
		const u32string utf32String = u32string(reinterpret_cast<const char32*>(wideString.CString()));

		return ConvertUTF32ToUTF8(utf32String);
	}
	/*-------------------------------------------------------------------
	-              Error check
	---------------------------------------------------------------------*/
	else
	{
		throw "Error: wchar_t size is not supported.";
	}
}

/*!**********************************************************************
*  @brief     String‚©‚ç‚ÉTString‚É•¶Žš—ñ‚ð•ÏŠ·‚µ‚Ü‚·.
*  @param[in] const string& utf8
*  @return    tstring
*************************************************************************/
tstring StringConverter::ConvertStringToTString(const string& utf8)
{
	/*-------------------------------------------------------------------
	-              wchar_t <=> char_16_t
	---------------------------------------------------------------------*/
	if constexpr (gu::type::IS_SAME<tchar, wchar>)
	{
		return ConvertStringToWString(utf8);
	}
	else if constexpr (gu::type::IS_SAME<tchar, char16>)
	{
		gu::wstring wString = ConvertStringToWString(utf8);

		return gu::tstring(reinterpret_cast<const tchar*>(wString.CString()));
	}
	else
	{
		return gu::tstring(reinterpret_cast<const tchar*>(utf8.CString()));
	}

}

#pragma endregion Public Function

#pragma region Private Function

#pragma endregion Private Function