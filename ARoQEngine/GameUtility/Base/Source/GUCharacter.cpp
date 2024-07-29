//////////////////////////////////////////////////////////////////////////////////
///  @file   Character.cpp
///  @brief  temp
///  @author toide
///  @date   2024/07/28 23:53:54
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUCharacter.hpp"

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
bool Character::IsNewLine(const char character)
{
	return character == '\n';
}

bool Character::IsNewLine(const wchar character)
{
	return character == L'\n';
}

bool Character::IsNewLine(const char8 character)
{
	return character == u8'\n';
}

bool Character::IsNewLine(const char16 character)
{
	return character == u'\n';
}

bool Character::IsNewLine(const char32 character)
{
	return character == U'\n';
}

bool Character::IsWhiteSpace(const char   character)
{
	return character == ' ' || character == '\t' || character == '\n' || character == '\r';
}
bool Character::IsWhiteSpace(const wchar  character)
{
	return character == L' ' || character == L'\t' || character == L'\n' || character == L'\r';
}
bool Character::IsWhiteSpace(const char8  character)
{
	return character == u8' ' || character == u8'\t' || character == u8'\n' || character == u8'\r';
}
bool Character::IsWhiteSpace(const char16 character)
{
	return character == u' ' || character == u'\t' || character == u'\n' || character == u'\r';
}
bool Character::IsWhiteSpace(const char32 character)
{
	return character == U' ' || character == U'\t' || character == U'\n' || character == U'\r';
}

bool Character::IsDigit(const char   character)
{
	return '0' <= character && character <= '9';
}
bool Character::IsDigit(const wchar  character)
{
	return L'0' <= character && character <= L'9';
}
bool Character::IsDigit(const char8  character)
{
	return u8'0' <= character && character <= u8'9';
}
bool Character::IsDigit(const char16 character)
{
	return u'0' <= character && character <= u'9';
}
bool Character::IsDigit(const char32 character)
{
	return U'0' <= character && character <= U'9';
}

/*!**********************************************************************
*  @brief     1‚©‚ç9‚Ü‚Å‚Ì”Žš‚©‚Ç‚¤‚©‚ð”»’è‚µ‚Ü‚·
*  @return    bool
*************************************************************************/
bool Character::IsNonZeroDigit(const char   character)
{
	return '1' <= character && character <= '9';
}
bool Character::IsNonZeroDigit(const wchar  character)
{
	return L'1' <= character && character <= L'9';
}
bool Character::IsNonZeroDigit(const char8  character)
{
	return u8'1' <= character && character <= u8'9';
}
bool Character::IsNonZeroDigit(const char16 character)
{
	return u'1' <= character && character <= u'9';
}
bool Character::IsNonZeroDigit(const char32 character)
{
	return U'1' <= character && character <= U'9';
}

bool Character::IsAlphabet(const char   character)
{
	return ('a' <= character && character <= 'z') || ('A' <= character && character <= 'Z');
}
bool Character::IsAlphabet(const wchar  character)
{
	return (L'a' <= character && character <= L'z') || (L'A' <= character && character <= L'Z');
}
bool Character::IsAlphabet(const char8  character)
{
	return (u8'a' <= character && character <= u8'z') || (u8'A' <= character && character <= u8'Z');
}
bool Character::IsAlphabet(const char16 character)
{
	return (u'a' <= character && character <= u'z') || (u'A' <= character && character <= u'Z');
}
bool Character::IsAlphabet(const char32 character)
{
	return (U'a' <= character && character <= U'z') || (U'A' <= character && character <= U'Z');
}
#pragma endregion Public Function