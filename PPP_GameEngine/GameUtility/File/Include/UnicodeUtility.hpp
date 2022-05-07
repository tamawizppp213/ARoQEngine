//////////////////////////////////////////////////////////////////////////////////
///             @file   UnicodeUtility.hpp
///             @brief  Convert Unicode size
///             @author Toide Yutaro
///             @date   2021_02_26
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef UNICODE_UTILITY_HPP
#define UNICODE_UTILITY_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include <string>
#include <array>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Functions
//////////////////////////////////////////////////////////////////////////////////
namespace unicode
{
	std::wstring ToWString(const std::string& utf8String);
	std::string  ToUtf8String(const std::wstring& wString);

	bool TryToWString(const std::string& utf8String, std::wstring& wString);
	bool TryToUtf8String(const std::wstring& wString, std::string& utf8String);

	bool ConvertCharU8ToU16(const std::array<char, 4>& u8Char, std::array<char16_t, 2>& u16Char);
	bool ConvertCharU8ToU32(const std::array<char, 4>& u8Char, char32_t& u32Char);

	bool ConvertCharU16ToU8(const std::array<char16_t, 2>& u16Char, std::array<char, 4>& u8Char);
	bool ConvertCharU16ToU32(const std::array<char16_t, 2>& u16Char, char32_t& u32Char);

	bool ConvertCharU32ToU8(const char32_t u32Char, std::array<char, 4>& u8Char);
	bool ConvertCharU32ToU16(const char32_t u32Char, std::array<char16_t, 2>& u16Char);

	bool ConvertU8ToU16(const std::string& u8String, std::u16string& u16String);
	bool ConvertU8ToU32(const std::string& u8String, std::u32string& u32String);

	bool ConvertU16ToU8(const std::u16string& u16String, std::string& u8String);
	bool ConvertU16ToU32(const std::u16string& u16String, std::u32string& u32String);

	bool ConvertU32ToU8(const std::u32string& u32String, std::string& u8String);
	bool ConvertU32ToU16(const std::u32string& u32String, std::u16string& u16String);



}
#endif