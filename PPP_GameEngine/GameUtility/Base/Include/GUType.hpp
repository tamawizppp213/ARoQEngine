//////////////////////////////////////////////////////////////////////////////////
///             @file   GUType.hpp
///             @brief  primitiveな型の定義です
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
	using float32  = float;
	using double64 = double;

	// 接頭辞の変更が必要となります
#if defined(_WIN32) || defined(_WIN64)
	using tchar = wchar;
	#define NEED_WIDE_CHAR (1)

	// SPとはString Prefixの略です. 
	#define SP(str) L##str 
#else 
	using tchar = char16;
	#define NEED_WIDE_CHAR (0)
	#define SP(str) u##str
#endif
	
	/*---------------------------------------------------------------
		各最大値/最小値を取得する
	-----------------------------------------------------------------*/
	#define MAX_FLOAT32 (3.402823466e+38F)
	#define MIN_FLOAT32 (1.175494351e-38F)
	#define MIN_INT8    (-127i8 - 1)
	#define MIN_INT16   (-32767i16 - 1)
	#define MIN_INT32   (-2147483647i32 - 1)
	#define MIN_INT64   (-9223372036854775807i64 - 1)
	#define MAX_INT8         (127i8)
	#define MAX_INT16        (32767i16)
	#define MAX_INT32        (2147483647i32)
	#define MAX_INT64        (9223372036854775807i64)
	#define MAX_UINT8        (0xffui8)
	#define MAX_UINT16       (0xffffui16)
	#define MAX_UINT32       (0xffffffffui32)
	#define MAX_UINT64       (0xffffffffffffffffui64)

}
#endif