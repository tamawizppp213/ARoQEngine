//////////////////////////////////////////////////////////////////////////////////
/// @file   GUStringBase.hpp
/// @brief  “®“I‚È•¶š—ñƒNƒ‰ƒX‚Å‚·. std::string‚Æ“¯“™‚Éˆµ‚¦‚éƒNƒ‰ƒX
///         string‚ğg—p‚·‚éê‡‚Í, SP("text")‚Æ‚¢‚¤‚æ‚¤‚ÉÚ“ª«‚ÉSP‚ğ‚Â‚¯‚é‚±‚Æ. 
///         SP : String Prefix (wide•¶š—ñ‚È‚çL"", uString‚È‚çu""‚Ì‚æ‚¤‚É©“®‚ÅU‚è•ª‚¯‚é)
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
	/*! @brief char8Œ^‚Ì•¶š—ñ*/
	using string    = gu::details::string::StringBase<char8, 1>;

	/*! @brief wcharŒ^‚Ì•¶š—ñ*/
	using wstring   = gu::details::string::StringBase<wchar, 2>;

	/*! @brief char8Œ^‚Ì•¶š—ñ*/
	using u8string  = gu::details::string::StringBase<char8, 1>;

	/*! @brief char16Œ^‚Ì•¶š—ñ*/
	using u16string = gu::details::string::StringBase<char16, 2>;

	/*! @brief char32Œ^‚Ì•¶š—ñ*/
	using u32string = gu::details::string::StringBase<char32, 4>;

#if NEED_WIDE_CHAR
	/*! @brief Platform‚É‚æ‚Á‚Äí—Ş‚ª•Ï‚í‚é•¶š—ñ*/
	using tstring = gu::wstring;
#else 
	using tstring = gu::u16string;
#endif
}

#endif