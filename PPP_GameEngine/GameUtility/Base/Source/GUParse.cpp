//////////////////////////////////////////////////////////////////////////////////
///             @file   GUParse.cpp
///             @brief   対象の文字列を検索し, 指定した数値を読み取ります. 
///             @details 読み込み順序が定まっていない文字列(環境変数の取得など)に対する値の読み込みに効果を発揮します. 
/// 　　　　　　　　　　　    数値データが連続して配置されていたり, 読み込み順序が決まっているような構造の場合には, FileSystemクラスを利用することをお勧めします.
///                      この理由は, streamポインタ自身のメモリをずらすことは行わず, streamの先頭から文字列検索が行われるため, 読み込み速度がかなり低下します.
///                      
///                      Value -> 読み込み順序が決定されていないものを探すときに使用します.
///                               文字列全体を探し, Matchに合う文字列の箇所の後に続く数値を読み取ります. (DesireToFind=100) 
/// 　　　　　　　　　　　　 　Line  -> 1行分だけまとめてずらす
///             @author toide
///             @date   2024/02/28 2:38:43
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/GUParse.hpp"
#include "../../Base/Include/GUString.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gu;
//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////
namespace
{
	gu::uint64 CheckValue(const tstring& stream, const tstring& match)
	{
		const auto findIndex = stream.Find(match, 0, true);
		
		if (findIndex == tstring::NPOS)
		{
			return tstring::NPOS;
		}
		
		const auto readIndex = findIndex + match.Size();
		Check(readIndex < stream.Size());

		return readIndex;
	}
}
#pragma region Constructor and Destructor 

#pragma endregion Constructor and Destructor

#pragma region Main Function
/*----------------------------------------------------------------------
*  @brief : 文字列全体を探し, Matchに合う文字列の箇所の後に続く数値を読み取ります.
*           型の判定は引数に入れた型で自動判別してしまうため, 事前に型は決めておいてください.
/*----------------------------------------------------------------------*/
bool Parse::Value(const tstring& stream, const tstring& match, int8& value)
{
	const auto checkValue = CheckValue(stream, match);
	if (checkValue == tstring::NPOS) 
	{
		return false; 
	}

	value = stream.SubString(checkValue, 4).ToInt8();
	return true;
}
bool Parse::Value(const tstring& stream, const tstring& match, int16& value)
{
	const auto checkValue = CheckValue(stream, match);
	if (checkValue == tstring::NPOS)
	{
		return false;
	}

	value = stream.SubString(checkValue, 6).ToInt16();
	return true;
}
bool Parse::Value(const tstring& stream, const tstring& match, int32& value)
{
	const auto checkValue = CheckValue(stream, match);
	if (checkValue == tstring::NPOS)
	{
		return false;
	}

	value = stream.SubString(checkValue, 11).ToInt32();
	return true;
}
bool Parse::Value(const tstring& stream, const tstring& match, int64& value)
{
	const auto checkValue = CheckValue(stream, match);
	if (checkValue == tstring::NPOS)
	{
		return false;
	}

	value = stream.SubString(checkValue, 20).ToInt64();
	return true;
}
bool Parse::Value(const tstring& stream, const tstring& match, uint8& value)
{
	const auto checkValue = CheckValue(stream, match);
	if (checkValue == tstring::NPOS)
	{
		return false;
	}

	value = stream.SubString(checkValue, 4).ToUInt8();
	return true;
}
bool Parse::Value(const tstring& stream, const tstring& match, uint16& value)
{
	const auto checkValue = CheckValue(stream, match);
	if (checkValue == tstring::NPOS)
	{
		return false;
	}

	value = stream.SubString(checkValue, 5).ToUInt16();
	return true;
}
bool Parse::Value(const tstring& stream, const tstring& match, uint32& value)
{
	const auto checkValue = CheckValue(stream, match);
	if (checkValue == tstring::NPOS)
	{
		return false;
	}

	value = stream.SubString(checkValue, 10).ToUInt32();
	return true;
}

bool Parse::Value(const tstring& stream, const tstring& match, uint64& value)
{
	const auto checkValue = CheckValue(stream, match);
	if (checkValue == tstring::NPOS)
	{
		return false;
	}

	value = stream.SubString(checkValue, 20).ToUInt64();
	return true;
}

bool Parse::Bool(const tstring& stream, const tstring& match, bool& value)
{
	const auto checkValue = CheckValue(stream, match);
	if (checkValue == tstring::NPOS)
	{
		value = false;
		return false;
	}

	auto string = stream.SubString(checkValue, 5);
	if (string.Contains(SP("True"), false) || 
		string.Contains(SP("Yes") , false) || 
		string.Contains(SP("On")  , false) ||
		string.Contains(SP("1")   , false))
	{
		value = true;
	}
	else if (
		string.Contains(SP("False"), false) ||
		string.Contains(SP("No"), false) ||
		string.Contains(SP("Off"), false) ||
		string.Contains(SP("0"), false))
	{
		value = false;
	}
	return true;
}

/****************************************************************************
*                       Contains
*************************************************************************//**
*  @fn        bool Contains(const tstring& stream, const tstring& parameter);
*
*  @brief     文字列全体を探し, Parameter名が文字列中に含まれるかを検索します
*
*  @param[in] const tstring& 探す文字列
*  @param[in] const tstring& パラメータ
* 
*  @return 　　bool
*****************************************************************************/
bool Parse::Contains(const tstring& stream, const tstring& parameter)
{
	return stream.Contains(parameter, true);
}

#pragma endregion Main Function
