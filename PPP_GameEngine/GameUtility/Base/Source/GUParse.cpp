//////////////////////////////////////////////////////////////////////////////////
///             @file   GUParse.cpp
///             @brief   �Ώۂ̕������������, �w�肵�����l��ǂݎ��܂�. 
///             @details �ǂݍ��ݏ�������܂��Ă��Ȃ�������(���ϐ��̎擾�Ȃ�)�ɑ΂���l�̓ǂݍ��݂Ɍ��ʂ𔭊����܂�. 
/// �@�@�@�@�@�@�@�@�@�@�@    ���l�f�[�^���A�����Ĕz�u����Ă�����, �ǂݍ��ݏ��������܂��Ă���悤�ȍ\���̏ꍇ�ɂ�, FileSystem�N���X�𗘗p���邱�Ƃ������߂��܂�.
///                      ���̗��R��, stream�|�C���^���g�̃����������炷���Ƃ͍s�킸, stream�̐擪���當���񌟍����s���邽��, �ǂݍ��ݑ��x�����Ȃ�ቺ���܂�.
///                      
///                      Value -> �ǂݍ��ݏ��������肳��Ă��Ȃ����̂�T���Ƃ��Ɏg�p���܂�.
///                               ������S�̂�T��, Match�ɍ���������̉ӏ��̌�ɑ������l��ǂݎ��܂�. (DesireToFind=100) 
/// �@�@�@�@�@�@�@�@�@�@�@�@ �@Line  -> 1�s�������܂Ƃ߂Ă��炷
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
*  @brief : ������S�̂�T��, Match�ɍ���������̉ӏ��̌�ɑ������l��ǂݎ��܂�.
*           �^�̔���͈����ɓ��ꂽ�^�Ŏ������ʂ��Ă��܂�����, ���O�Ɍ^�͌��߂Ă����Ă�������.
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
*  @brief     ������S�̂�T��, Parameter���������񒆂Ɋ܂܂�邩���������܂�
*
*  @param[in] const tstring& �T��������
*  @param[in] const tstring& �p�����[�^
* 
*  @return �@�@bool
*****************************************************************************/
bool Parse::Contains(const tstring& stream, const tstring& parameter)
{
	return stream.Contains(parameter, true);
}

#pragma endregion Main Function
