/////////////////////////////////////////////////////////////////////////////////
///             @file   UnicodeUtility.cpp
///             @brief  Convert Unicode size
///             @author Toide Yutaro
///             @date   2021_02_26
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/File/Include/UnicodeUtility.hpp"
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Functions
//////////////////////////////////////////////////////////////////////////////////
namespace unicode
{
	namespace
	{
		int GetU8ByteCount(char ch)
		{
			if (0    <= uint8_t(ch) && uint8_t(ch) < 0x80) { return 1; }
			if (0xC2 <= uint8_t(ch) && uint8_t(ch) < 0xE0) { return 2; }
			if (0xE0 <= uint8_t(ch) && uint8_t(ch) < 0xF0) { return 3; }
			if (0xF0 <= uint8_t(ch) && uint8_t(ch) < 0xF8) { return 4; }
			return 0;
		}

		bool IsU8LaterByte(char ch)
		{
			return 0x80 <= uint8_t(ch) && uint8_t(ch) < 0xC0;
		}

		bool IsU16HighSurrogate(char16_t ch)
		{ 
			return 0xD800 <= ch && ch < 0xDC00; 
		}

		bool IsU16LowSurrogate(char16_t ch)
		{ 
			return 0xDC00 <= ch && ch < 0xE000; 
		}
	}

	/****************************************************************************
	*                            ToWString
	*************************************************************************//**
	*  @fn        std::wstring ToWString(const std::string& utf8String)
	*  @brief     utf8 -> wstring
	*  @param[in] const std::string& utf8String
	*  @return 　　std::wstring
	*****************************************************************************/
	std::wstring ToWString(const std::string& utf8String)
	{
		std::wstring wString;
		if (!TryToWString(utf8String, wString))
		{
			throw std::invalid_argument("Failed to convert wString");
		}
		return wString;
	}

	/****************************************************************************
	*                            ToUtf8String
	*************************************************************************//**
	*  @fn        std::string ToUtf8String(const std::wstring& wString)
	*  @brief     wstring -> utf8
	*  @param[in] const std::wString& wString
	*  @return 　　std::string
	*****************************************************************************/
	std::string ToUtf8String(const std::wstring& wString)
	{
		std::string utf8String;
		if (!TryToUtf8String(wString, utf8String))
		{
			throw std::invalid_argument("Failed to convert UTF-8 string");
		}
		return utf8String;
	}

	/****************************************************************************
	*                            TryToWString
	*************************************************************************//**
	*  @fn         bool TryToWString(const std::string& utf8String, std::wstring& wString)
	*  @brief      utf8 -> wstring check
	*  @param[in]  const std::string& utf8String
	*  @param[out] std::wstring&      wString
	*  @return 　　 std::wstring
	*****************************************************************************/
	bool TryToWString(const std::string& utf8String, std::wstring& wString)
	{
		/*-------------------------------------------------------------------
		-              wchar_t <=> char_16_t
		---------------------------------------------------------------------*/
		size_t wcharSize = 0; wcharSize = sizeof(wchar_t);
		if (wcharSize == sizeof(char16_t))
		{
			std::u16string utf16String;
			if (!ConvertU8ToU16(utf8String, utf16String))
			{
				return false;
			}
			wString = reinterpret_cast<const wchar_t*>(utf16String.c_str());
		}
		/*-------------------------------------------------------------------
		-              wchar_t <=> char_32_t
		---------------------------------------------------------------------*/
		else if (wcharSize == sizeof(char32_t))
		{
			std::u32string utf32String;
			if (!ConvertU8ToU32(utf8String, utf32String))
			{
				return false;
			}
			wString = reinterpret_cast<const wchar_t*>(utf32String.c_str());
		}
		/*-------------------------------------------------------------------
		-              Error check
		---------------------------------------------------------------------*/
		else { return false; }
		return true;
	}

	/****************************************************************************
	*                            TryToUtf8String
	*************************************************************************//**
	*  @fn         bool TryToUtf8String(const std::wstring& wString, std::string& utf8String)
	*  @brief      wstring -> utf8 check
	*  @param[in]  wString
	*  @param[out] utf8String
	*  @return 　 　bool
	*****************************************************************************/
	bool TryToUtf8String(const std::wstring& wString, std::string& utf8String)
	{
		/*-------------------------------------------------------------------
		-              wchar_t <=> char_16_t
		---------------------------------------------------------------------*/
		if constexpr(sizeof(wchar_t) == sizeof(char16_t))
		{
			const char16_t* utf16String = reinterpret_cast<const char16_t*>(wString.c_str());
			if (!ConvertU16ToU8(utf16String, utf8String))
			{
				return false;
			}
		}
		/*-------------------------------------------------------------------
		-              wchar_t <=> char_32_t
		---------------------------------------------------------------------*/
		else if constexpr(sizeof(wchar_t) == sizeof(char32_t))
		{
			const char32_t* utf32String = reinterpret_cast<const char32_t*>(wString.c_str());
			if (!ConvertU32ToU8(utf32String, utf8String))
			{
				return false;
			}
		}
		/*-------------------------------------------------------------------
		-              Error check
		---------------------------------------------------------------------*/
		else{ return false; }
		return true;
	}

	/****************************************************************************
	*                            ConvertCharU8ToU16
	*************************************************************************//**
	*  @fn         bool ConvertCharU8ToU16(const std::array<char, 4>& u8Char, std::array<char16_t, 2>& u16Char)
	*  @brief      convert char U8 to U16
	*  @param[in]  const std::array<char, 4>& u8Char
	*  @param[out] std::array<char16_t, 2>& u16Char
	*  @return 　　 bool
	*****************************************************************************/
	bool ConvertCharU8ToU16(const std::array<char, 4>& u8Char, std::array<char16_t, 2>& u16Char)
	{
		char32_t u32Char;
		if (!ConvertCharU8ToU32 (u8Char , u32Char)) { return false; }
		if (!ConvertCharU32ToU16(u32Char, u16Char)) { return false; }
		return true;
	}

	/****************************************************************************
	*                            ConvertCharU8ToU32
	*************************************************************************//**
	*  @fn         bool ConvertCharU8ToU32(const std::array<char, 4>& u8Char, char32_t& u32Char)
	*  @brief      convert char U8 to U32
	*  @param[in]  const std::array<char, 4>& u8Char
	*  @param[out] char32_t& u32Char
	*  @return 　　 bool
	*****************************************************************************/
	bool ConvertCharU8ToU32(const std::array<char, 4>& u8Char, char32_t& u32Char)
	{
		int byteCount = GetU8ByteCount(u8Char[0]);
		if (byteCount == 0) { return false; }

		switch (byteCount)
		{
			/*-------------------------------------------------------------------
			-              byteCount = 1
			---------------------------------------------------------------------*/
			case 1:
			{
				u32Char = char32_t((uint8_t(u8Char[0])));
				break;
			}
			/*-------------------------------------------------------------------
			-              byteCount = 2
			---------------------------------------------------------------------*/
			case 2:
			{
				if (!IsU8LaterByte(u8Char[1]))        { return false; }
				if ((uint8_t(u8Char[0]) & 0x1E) == 0) { return false; }
				u32Char  = char32_t(u8Char[0] & 0x1F) << 6;
				u32Char |= char32_t(u8Char[1] & 0x3F);
				break;
			}
			/*-------------------------------------------------------------------
			-              byteCount = 3
			---------------------------------------------------------------------*/
			case 3:
			{
				if (!IsU8LaterByte(u8Char[1]) || !IsU8LaterByte(u8Char[2]))
				{
					return false;
				}
				if ((uint8_t(u8Char[0]) & 0x0F) == 0 &&
					(uint8_t(u8Char[1]) & 0x20) == 0)
				{
					return false;
				}

				u32Char  = char32_t(u8Char[0] & 0x0F) << 12;
				u32Char |= char32_t(u8Char[1] & 0x3F) << 6;
				u32Char |= char32_t(u8Char[2] & 0x3F);
				break;
			}
			/*-------------------------------------------------------------------
			-              byteCount = 4
			---------------------------------------------------------------------*/
			case 4:
			{
				if (!IsU8LaterByte(u8Char[1]) || !IsU8LaterByte(u8Char[2]) ||
					!IsU8LaterByte(u8Char[3]))
				{
					return false;
				}
				if ((uint8_t(u8Char[0]) & 0x07) == 0 &&
					(uint8_t(u8Char[1]) & 0x30) == 0)
				{
					return false;
				}

				u32Char  = char32_t(u8Char[0] & 0x07) << 18;
				u32Char |= char32_t(u8Char[1] & 0x3F) << 12;
				u32Char |= char32_t(u8Char[2] & 0x3F) << 6;
				u32Char |= char32_t(u8Char[3] & 0x3F);
				break;
			}
			default:
			{
				return false;
			}
		}
		return true;
	}

	/****************************************************************************
	*                            ConvertCharU16ToU8
	*************************************************************************//**
	*  @fn         bool ConvertCharU16ToU8(const std::array<char16_t, 2>& u16Char, std::array<char, 4>& u8Char)
	*  @brief      convert char U16 to U8
	*  @param[in]  const std::array<char16_t, 2>& u16Char
	*  @param[out] std::array<char, 4>& u8Char
	*  @return 　　 bool
	*****************************************************************************/
	bool ConvertCharU16ToU8(const std::array<char16_t, 2>& u16Char, std::array<char, 4>& u8Char)
	{
		char32_t u32Char;
		if (!ConvertCharU16ToU32(u16Char, u32Char)){ return false; }
		if (!ConvertCharU32ToU8(u32Char, u8Char))  { return false; }
		return true;
	}

	/****************************************************************************
	*                            ConvertCharU16ToU32
	*************************************************************************//**
	*  @fn         bool ConvertCharU16ToU32(const std::array<char16_t, 2>& u16Char, char32_t& u32Char)
	*  @brief      convert char U16 to U32
	*  @param[in]  const std::array<char16_t, 2>& u16Char
	*  @param[out] char32_t& u32Char
	*  @return 　　 bool
	*****************************************************************************/
	bool ConvertCharU16ToU32(const std::array<char16_t, 2>& u16Char, char32_t& u32Char)
	{
		if (IsU16HighSurrogate(u16Char[0]))
		{
			if (IsU16LowSurrogate(u16Char[1]))
			{
				u32Char = 0x10000 + (char32_t(u16Char[0]) - 0xD800) * 0x400 + (char32_t(u16Char[1]) - 0xDC00);
			}
			else if (u16Char[1] == 0){ u32Char = u16Char[0]; }
			else{ return false; }
		}
		else if (IsU16LowSurrogate(u16Char[0]))
		{
			if (u16Char[1] == 0) { u32Char = u16Char[0]; }
			else { return false; }
		}
		else { u32Char = u16Char[0]; }

		return true;
	}

	/****************************************************************************
	*                            ConvertCharU32ToU8
	*************************************************************************//**
	*  @fn         bool ConvertCharU32ToU8(const char32_t u32Char, std::array<char, 4>& u8Char)
	*  @brief      convert char U32 to U8
	*  @param[in]  const char32_t u32Char
	*  @param[out] std::array<char, 4>& u8Char
	*  @return 　　 bool
	*****************************************************************************/
	bool ConvertCharU32ToU8(const char32_t u32Char, std::array<char, 4>& u8Char)
	{
		if (u32Char > 0x10FFFF)
		{
			return false;
		}

		if (u32Char < 128)
		{
			u8Char[0] = char(u32Char);
			u8Char[1] = 0;
			u8Char[2] = 0;
			u8Char[3] = 0;
		}
		else if (u32Char < 2048)
		{
			u8Char[0] = 0xC0 |  char(u32Char >> 6);
			u8Char[1] = 0x80 | (char(u32Char) & 0x3F);
			u8Char[2] = 0;
			u8Char[3] = 0;
		}
		else if (u32Char < 65536)
		{
			u8Char[0] = 0xE0 | char(u32Char >> 12);
			u8Char[1] = 0x80 | (char(u32Char >> 6) & 0x3F);
			u8Char[2] = 0x80 | (char(u32Char) & 0x3F);
			u8Char[3] = 0;
		}
		else
		{
			u8Char[0] = 0xF0 | char(u32Char  >> 18);
			u8Char[1] = 0x80 | (char(u32Char >> 12) & 0x3F);
			u8Char[2] = 0x80 | (char(u32Char >> 6) & 0x3F);
			u8Char[3] = 0x80 | (char(u32Char) & 0x3F);
		}

		return true;
	}

	/****************************************************************************
	*                            ConvertCharU32ToU16
	*************************************************************************//**
	*  @fn         bool ConvertCharU32ToU16(const char32_t u32Char, std::array<char16_t, 2>& u16Char)
	*  @brief      convert char U32 to U16
	*  @param[in]  const char32_t u32Char
	*  @param[out] std::array<char16_t, 2>& u16Char
	*  @return 　　 bool
	*****************************************************************************/
	bool ConvertCharU32ToU16(const char32_t u32Char, std::array<char16_t, 2>& u16Char)
	{
		if (u32Char > 0x10FFFF)
		{
			return false;
		}

		if (u32Char < 0x10000)
		{
			u16Char[0] = char16_t(u32Char);
			u16Char[1] = 0;
		}
		else
		{
			u16Char[0] = char16_t((u32Char - 0x10000) / 0x400 + 0xD800);
			u16Char[1] = char16_t((u32Char - 0x10000) % 0x400 + 0xDC00);
		}

		return true;
	}

	/****************************************************************************
	*                            ConvertCharU8ToU16
	*************************************************************************//**
	*  @fn         bool ConvertU8ToU16(const std::string& u8String, std::u16string& u16String)
	*  @brief      convert char U8 to U16
	*  @param[in]  const std::string& u8String
	*  @param[out] std::u16string& u16String
	*  @return 　　 bool
	*****************************************************************************/
	bool ConvertU8ToU16(const std::string& u8String, std::u16string& u16String)
	{
		for (auto u8It = u8String.begin(); u8It != u8String.end(); ++u8It)
		{
			auto numBytes = GetU8ByteCount((*u8It));
			if (numBytes == 0) { return false; }

			std::array<char, 4> u8Char = {};
			u8Char[0] = (*u8It);
			for (int i = 1; i < numBytes; i++)
			{
				++u8It;
				if (u8It == u8String.end()) { return false; }
				u8Char[i] = (*u8It);
			}

			std::array<char16_t, 2> u16Char;
			if (!ConvertCharU8ToU16(u8Char, u16Char)) { return false; }

			u16String.push_back(u16Char[0]);
			if (u16Char[1] != 0)
			{
				u16String.push_back(u16Char[1]);
			}
		}
		return true;
	}

	/****************************************************************************
	*                            ConvertCharU8ToU32
	*************************************************************************//**
	*  @fn         bool ConvertU8ToU32(const std::string& u8String, std::u32string& u32String)
	*  @brief      convert char U8 to U32
	*  @param[in]  const std::string& u8String
	*  @param[out] std::u32string& u32String
	*  @return 　　 bool
	*****************************************************************************/
	bool ConvertU8ToU32(const std::string& u8String, std::u32string& u32String)
	{
		for (auto u8It = u8String.begin(); u8It != u8String.end(); ++u8It)
		{
			auto numBytes = GetU8ByteCount((*u8It));
			if (numBytes == 0) { return false; }

			std::array<char, 4> u8Char = {};
			u8Char[0] = (*u8It);
			for (int i = 1; i < numBytes; i++)
			{
				++u8It;
				if (u8It == u8String.end()){ return false; }
				u8Char[i] = (*u8It);
			}

			char32_t u32Char;
			if (!ConvertCharU8ToU32(u8Char, u32Char)) { return false; }

			u32String.push_back(u32Char);
		}
		return true;
	}

	/****************************************************************************
	*                            ConvertCharU16ToU8
	*************************************************************************//**
	*  @fn         bool ConvertU16ToU8(const std::u16string& u16String, std::string& u8String)
	*  @brief      convert char U16 to U8
	*  @param[in]  const std::u16string& u16String
	*  @param[out] std::string& u8String
	*  @return 　　 bool
	*****************************************************************************/
	bool ConvertU16ToU8(const std::u16string& u16String, std::string& u8String)
	{
		for (auto u16It = u16String.begin(); u16It != u16String.end(); ++u16It)
		{
			std::array<char16_t, 2> u16Char = {};
			if (IsU16HighSurrogate((*u16It)))
			{
				u16Char[0] = (*u16It);
				++u16It;
				if (u16It == u16String.end()) { return false; }
				u16Char[1] = (*u16It);
			}
			else
			{
				u16Char[0] = (*u16It);
				u16Char[1] = 0;
			}

			std::array<char, 4> u8Char;
			if (!ConvertCharU16ToU8(u16Char, u8Char)){ return false; }
			if (u8Char[0] != 0) { u8String.push_back(u8Char[0]); }
			if (u8Char[1] != 0) { u8String.push_back(u8Char[1]); }
			if (u8Char[2] != 0) { u8String.push_back(u8Char[2]); }
			if (u8Char[3] != 0) { u8String.push_back(u8Char[3]); }
		}
		return true;
	}

	/****************************************************************************
	*                            ConvertCharU16ToU32
	*************************************************************************//**
	*  @fn         bool ConvertU16ToU32(const std::u16string& u16String, std::u32string& u32String)
	*  @brief      convert char U16 to U32
	*  @param[in]  const std::u16string& u16String
	*  @param[out] std::u32string& u32String
	*  @return 　　 bool
	*****************************************************************************/
	bool ConvertU16ToU32(const std::u16string& u16String, std::u32string& u32String)
	{
		for (auto u16It = u16String.begin(); u16It != u16String.end(); ++u16It)
		{
			std::array<char16_t, 2> u16Char = {};
			if (IsU16HighSurrogate((*u16It)))
			{
				u16Char[0] = (*u16It);
				++u16It;
				if (u16It == u16String.end()) { return false; }
				u16Char[1] = (*u16It);
			}
			else
			{
				u16Char[0] = (*u16It);
				u16Char[1] = 0;
			}

			char32_t u32Char;
			if (!ConvertCharU16ToU32(u16Char, u32Char)) { return false; }
			u32String.push_back(u32Char);
		}
		return true;
	}

	/****************************************************************************
	*                            ConvertCharU32ToU8
	*************************************************************************//**
	*  @fn         bool ConvertU32ToU8(const std::u32string& u32String, std::string& u8String)
	*  @brief      convert char U32 to U8
	*  @param[in]  const std::u32string& u32String,
	*  @param[out] std::string& u8String
	*  @return 　　 bool
	*****************************************************************************/
	bool ConvertU32ToU8(const std::u32string& u32String, std::string& u8String)
	{
		for (auto u32It = u32String.begin(); u32It != u32String.end(); ++u32It)
		{
			std::array<char, 4> u8Char;
			if (!ConvertCharU32ToU8((*u32It), u8Char)) { return false; }

			if (u8Char[0] != 0) { u8String.push_back(u8Char[0]); }
			if (u8Char[1] != 0) { u8String.push_back(u8Char[1]); }
			if (u8Char[2] != 0) { u8String.push_back(u8Char[2]); }
			if (u8Char[3] != 0) { u8String.push_back(u8Char[3]); }
		}
		return true;
	}

	/****************************************************************************
	*                            ConvertCharU32ToU16
	*************************************************************************//**
	*  @fn         bool ConvertU32ToU16(const std::u32string& u32String, std::u16string& u16String)
	*  @brief      convert char U32 to U8
	*  @param[in]  const std::u32string& u32String,
	*  @param[out] std::u16string& u16String
	*  @return 　　 bool
	*****************************************************************************/
	bool ConvertU32ToU16(const std::u32string& u32String, std::u16string& u16String)
	{
		for (auto u32It = u32String.begin(); u32It != u32String.end(); ++u32It)
		{
			std::array<char16_t, 2> u16Char;
			if (!ConvertCharU32ToU16((*u32It), u16Char)) { return false; }

			if (u16Char[0] != 0)
			{
				u16String.push_back(u16Char[0]);
			}
			if (u16Char[1] != 0)
			{
				u16String.push_back(u16Char[1]);
			}
		}
		return true;
	}
}