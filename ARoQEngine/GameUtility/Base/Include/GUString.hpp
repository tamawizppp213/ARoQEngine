//////////////////////////////////////////////////////////////////////////////////
/// @file   GUStringBase.hpp
/// @brief  動的な文字列クラスです. std::stringと同等に扱えるクラス
///         stringを使用する場合は, SP("text")というように接頭辞にSPをつけること. 
///         SP : String Prefix (wide文字列ならL"", uStringならu""のように自動で振り分ける)
/// @author toide
/// @date   2023/11/08 0:50:57
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_STRING_HPP
#define GU_STRING_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Private/Base/Include/GUStringBase.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu
{
	/*! @brief char8型の文字列*/
	using string    = gu::details::string::StringBase<char8, 1>;

	/*! @brief wchar型の文字列*/
	using wstring   = gu::details::string::StringBase<wchar, 2>;

	/*! @brief char8型の文字列*/
	using u8string  = gu::details::string::StringBase<char8, 1>;

	/*! @brief char16型の文字列*/
	using u16string = gu::details::string::StringBase<char16, 2>;

	/*! @brief char32型の文字列*/
	using u32string = gu::details::string::StringBase<char32, 4>;

#if NEED_WIDE_CHAR
	/*! @brief Platformによって種類が変わる文字列*/
	using tstring = gu::wstring;
#else 
	using tstring = gu::u16string;
#endif
}

#endif