//////////////////////////////////////////////////////////////////////////////////
///  @file   GUStringConverter.cpp
///  @brief  �������ϊ�����@�\��񋟂��܂�.
///  @author toide
///  @date   2024/05/19 18:17:56
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUStringConverter.hpp"
#include "GameUtility/Base/Include/GUCharacterCodeConverter.hpp"
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
*  @brief     UTF8����UTF16�ɕ������ϊ����܂�.
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
*  @brief     UTF8����UTF32�ɕ������ϊ����܂�.
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
*  @brief     UTF16����UTF32�ɕ������ϊ����܂�.
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
*  @brief     UTF16����UTF32�ɕ������ϊ����܂�.
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
*  @brief     UTF32����UTF8�ɕ������ϊ����܂�.
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
*  @brief     UTF32����UTF16�ɕ������ϊ����܂�.
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
*  @brief     String�����WString�ɕ������ϊ����܂�.
*  @param[in] const string& utf8
*  @return    wstring
*************************************************************************/
wstring StringConverter::ConvertStringToWString(const string& utf8String)
{
	/*-------------------------------------------------------------------
	-              �ϊ�
	---------------------------------------------------------------------*/
	const auto source                 = CharacterCode::MultiByteCharacterCode();
	const auto destination            = CharacterCode::WideStringCharacterCode();
	const auto characterCodeConverter = MakeShared<CharacterCodeConverter>(source, destination);

	Check(source);
	Check(destination);

	const auto& utf16ByteArray = characterCodeConverter->Convert(reinterpret_cast<const uint8*>(utf8String.CString()), utf8String.Size());

	/*-------------------------------------------------------------------
	-              ������ɑ��
	---------------------------------------------------------------------*/
	wstring result = L"";
	result.Assign(reinterpret_cast<const wchar*>(utf16ByteArray.Data()), utf16ByteArray.Size() / sizeof(wchar));
	
	return result;
}

/*!**********************************************************************
*  @brief     utf8�����񂩂��WString�ɕ������ϊ����܂�.
*  @param[in] const string& utf8
*  @return    wstring
*************************************************************************/
tstring StringConverter::ConvertUTF8ToTString(const u8string& utf8)
{
	/*-------------------------------------------------------------------
	-              �ϊ�
	---------------------------------------------------------------------*/
	const auto source                 = CharacterCode::GetCharacterCode(CharacterCodeType::UTF8);
	const auto destination            = CharacterCode::TStringCharacterCode();
	const auto characterCodeConverter = MakeShared<CharacterCodeConverter>(source, destination);

	Check(source);
	Check(destination);

	const auto& byteArray = characterCodeConverter->Convert(reinterpret_cast<const uint8*>(utf8.CString()), utf8.Size());

	/*-------------------------------------------------------------------
	-              ������ɑ��
	---------------------------------------------------------------------*/
	tstring result = SP("");
	result.Assign(reinterpret_cast<const tchar*>(byteArray.Data()), byteArray.Size() / sizeof(tchar));

	return result;

}

/*!**********************************************************************
*  @brief     WString�����String�ɕ������ϊ����܂�.
*  @param[in] const wstring& utf8
*  @return    wstring
*************************************************************************/
string StringConverter::ConvertWStringToString(const wstring& wideString)
{
	/*-------------------------------------------------------------------
	-              �ϊ�
	---------------------------------------------------------------------*/
	const auto source                 = CharacterCode::WideStringCharacterCode();
	const auto destination            = CharacterCode::MultiByteCharacterCode();
	const auto characterCodeConverter = MakeShared<CharacterCodeConverter>(source, destination);

	Check(source);
	Check(destination);

	const auto& byteArray = characterCodeConverter->Convert(reinterpret_cast<const uint8*>(wideString.CString()), wideString.Size() * sizeof(wchar));

	/*-------------------------------------------------------------------
	-              ������ɑ��
	---------------------------------------------------------------------*/
	string result = "";
	result.Assign(reinterpret_cast<const char*>(byteArray.Data()), byteArray.Size() / sizeof(wchar));

	return result;
}

/*!**********************************************************************
*  @brief     String�����TString�ɕ������ϊ����܂�.
*  @param[in] const string& utf8
*  @return    tstring
*************************************************************************/
tstring StringConverter::ConvertStringToTString(const string& utf8String)
{
	/*-------------------------------------------------------------------
	-              �ϊ�
	---------------------------------------------------------------------*/
	const auto source                 = CharacterCode::MultiByteCharacterCode();
	const auto destination            = CharacterCode::TStringCharacterCode();
	const auto characterCodeConverter = MakeShared<CharacterCodeConverter>(source, destination);

	Check(source);
	Check(destination);

	const auto& byteArray = characterCodeConverter->Convert(reinterpret_cast<const uint8*>(utf8String.CString()), utf8String.Size());

	/*-------------------------------------------------------------------
	-              ������ɑ��
	---------------------------------------------------------------------*/
	tstring result = SP("");
	result.Assign(reinterpret_cast<const tchar*>(byteArray.Data()), byteArray.Size() / sizeof(tchar));

	return result;
}

/*!**********************************************************************
*  @brief     TString�����String�ɕ������ϊ����܂�.
*  @param[in] const tstring& tString
*  @return    tstring
*************************************************************************/
string StringConverter::ConvertTStringToString(const tstring& tString)
{
	/*-------------------------------------------------------------------
	-              �ϊ�
	---------------------------------------------------------------------*/
	const auto source                 = CharacterCode::TStringCharacterCode();
	const auto destination            = CharacterCode::MultiByteCharacterCode();
	const auto characterCodeConverter = MakeShared<CharacterCodeConverter>(source, destination);

	Check(source);
	Check(destination);

	const auto& byteArray = characterCodeConverter->Convert(reinterpret_cast<const uint8*>(tString.CString()), tString.Size() * sizeof(tchar));

	/*-------------------------------------------------------------------
	-              ������ɑ��
	---------------------------------------------------------------------*/
	string result = "";
	result.Assign(reinterpret_cast<const char*>(byteArray.Data()), byteArray.Size() / sizeof(char));

	return result;

}

/*!**********************************************************************
*  @brief     TString�����UTF8�ɕ������ϊ����܂�.
*  @param[in] const tstring& tString
*  @return    tstring
*************************************************************************/
u8string StringConverter::ConvertTStringToUTF8(const tstring& tString)
{
	/*-------------------------------------------------------------------
	-              �ϊ�
	---------------------------------------------------------------------*/
	const auto source                 = CharacterCode::TStringCharacterCode();
	const auto destination            = CharacterCode::GetCharacterCode(gu::CharacterCodeType::UTF8);
	const auto characterCodeConverter = MakeShared<CharacterCodeConverter>(source, destination);

	Check(source);
	Check(destination);

	const auto& byteArray = characterCodeConverter->Convert(reinterpret_cast<const uint8*>(tString.CString()), tString.Size() * sizeof(tchar));

	/*-------------------------------------------------------------------
	-              ������ɑ��
	---------------------------------------------------------------------*/
	u8string result = u8"";
	result.Assign(reinterpret_cast<const char8*>(byteArray.Data()), byteArray.Size() / sizeof(char8));

	return result;

}

#pragma endregion Public Function

#pragma region Private Function

#pragma endregion Private Function