//////////////////////////////////////////////////////////////////////////////////
///             @file   GUStringBase.hpp
///             @brief  ���I�ȕ�����N���X�ł�. std::string�Ɠ����Ɉ�����N���X
///                     tstring���g�p����ꍇ��, SP("text")�Ƃ����悤�ɐړ�����SP�����邱��. 
///                     SP : String Prefix (wide������Ȃ�L"", uString�Ȃ�u""�̂悤�Ɏ����ŐU�蕪����)
///             @author toide
///             @date   2023/11/08 0:50:57
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
	using string    = gu::details::string::StringBase<char8, 1>;
	using wstring   = gu::details::string::StringBase<wchar, 2>;
	using u8string  = gu::details::string::StringBase<char8, 1>;
	using u16string = gu::details::string::StringBase<char16, 2>;
	using u32string = gu::details::string::StringBase<char32, 4>;

#if NEED_WIDE_CHAR
	using tstring = gu::wstring;
#else 
	using tstring = gu::u16string;
#endif
}

#endif