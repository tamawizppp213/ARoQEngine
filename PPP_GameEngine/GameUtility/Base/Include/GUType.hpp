//////////////////////////////////////////////////////////////////////////////////
///             @file   GUType.hpp
///             @brief  primitive�Ȍ^�̒�`�ł�
///             @author Toide Yutaro
///             @date   2023_11_08
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_TYPE
#define GU_TYPE

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                           Type def
//////////////////////////////////////////////////////////////////////////////////
namespace gu
{
	using int8   = signed char;
	using int16  = signed short int;
	using int32  = signed int;
	using int64  = signed long long;
	using uint8  = unsigned char;
	using uint16 = unsigned short int;
	using uint32 = unsigned int;
	using uint64 = unsigned long long;
	using char8    = char;
	using char16   = char16_t; // literal "u"
	using char32   = char32_t; // literal "U"
	using wchar    = wchar_t;  // literal "L"
	using u8char   = unsigned char; // literal "u8"
	
	// �ړ����̕ύX���K�v�ƂȂ�܂�
#if defined(_WIN32) || defined(_WIN64)
	using tchar = wchar;
	#define NEED_WIDE_CHAR (1)
	#define TCHAR_TEXT(str) L ##str
#else 
	using tchar = char16;
	#define NEED_WIDE_CHAR (0)
	#define TCHAR_TEXT(str) u ##str
#endif
}
#endif