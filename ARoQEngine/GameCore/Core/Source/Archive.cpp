//////////////////////////////////////////////////////////////////////////////////
///  @file   Archive.cpp
///  @brief  temp
///  @author toide
///  @date   2024/08/04 1:07:26
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Core/Include/Archive.hpp"
#include "GameUtility/File/Include/Json.hpp"
#include "GameUtility/File/Private/Json/Include/JsonObject.hpp"
#include "GameUtility/File/Private/Json/Include/JsonValueString.hpp"
#include "GameUtility/File/Private/Json/Include/JsonValueNumber.hpp"
#include "GameUtility/File/Private/Json/Include/JsonValueBoolean.hpp"
#include "GameUtility/File/Private/Json/Include/JsonValueArray.hpp"
#include "GameUtility/File/Private/Json/Include/JsonValueObject.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace engine;
using namespace gu;
using namespace gu::file;
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
Archive::Archive(const ArchiveMode mode, const OutputFileType fileType)
	: _mode(mode)
{
	switch (fileType)
	{
		case OutputFileType::Json:
		{
			_json = gu::MakeShared<gu::file::Json>();
			break;
		}
		default:
		{
			throw "Not Supported File Type";
		}
	}

	_object = gu::MakeShared<gu::file::json::JsonObject>();
}

#pragma endregion Constructor and Destructor

#pragma region Public Function
/*!**********************************************************************
*  @brief     Jsonファイルを対象のファイルパスから読み込みます.
*  @param[in] const gu::tstring& filePath : 読み込みたいJsonファイルのパス
*  @return    bool
*************************************************************************/
bool Archive::Load(const gu::tstring& filePath)
{
	Checkf(_json, "Json is nullptr");
	Checkf(_mode == ArchiveMode::Load, "Mode is not Load");

	_object = MakeShared<gu::file::json::JsonObject>();
	return _json->Deserialize(filePath, _object, gu::file::Json::ReadFlags::None, false);
}

/*!**********************************************************************
*  @brief     Jsonファイルを対象のファイルパスから読み込みます.
*  @param[in] const gu::tstring& filePath : 読み込みたいJsonファイルのパス
*  @return    bool
*************************************************************************/
bool Archive::Save(const gu::tstring& filePath)
{
	Checkf(_json, "Json is nullptr");
	Checkf(_mode == ArchiveMode::Save, "Mode is not Save");

	if(_object == nullptr)
	{
		return false;
	}

	return _json->Serialize(filePath, _object, false);

}
#pragma endregion Public Function

#pragma region Public Property

#pragma endregion Public Property

#pragma region Public Operator

/*! @brief Jsonオブジェクトに数値を登録します [int8]*/
Archive& Archive::operator << (const gu::Pair<gu::tstring, gu::int8*>& pair)
{
	if (IsSaveMode())
	{
		const auto value = MakeShared<json::JsonValueNumber>(static_cast<gu::double64>(*pair.Value));
		_object->AddMember(pair.Key, value);
	}
	else if(IsLoadMode())
	{
		*pair.Value = _object->GetMember(pair.Key)->GetInt8();
	}
	else
	{
		throw "Not Supported Mode";
	}
	return *this;
}

/*! @brief Jsonオブジェクトに数値を登録します [int16]*/
Archive& Archive::operator << (const gu::Pair<gu::tstring, gu::int16*>& pair)
{
	if (IsSaveMode())
	{
		const auto value = MakeShared<json::JsonValueNumber>(static_cast<gu::double64>(*pair.Value));
		_object->AddMember(pair.Key, value);
	}
	else if(IsLoadMode())
	{
		*pair.Value = _object->GetMember(pair.Key)->GetInt16();
	}
	else
	{
		throw "Not Supported Mode";
	}
	return *this;
}

/*! @brief Jsonオブジェクトに数値を登録します [int32]*/
Archive& Archive::operator << (const gu::Pair<gu::tstring, gu::int32*>& pair) 
{
	if (IsSaveMode())
	{
		const auto value = MakeShared<json::JsonValueNumber>(static_cast<gu::double64>(*pair.Value));
		_object->AddMember(pair.Key, value);
	}
	else if(IsLoadMode())
	{
		*pair.Value = _object->GetMember(pair.Key)->GetInt32();
	}
	else
	{
		throw "Not Supported Mode";
	}
	return *this;
}

/*! @brief Jsonオブジェクトに数値を登録します [int64]*/
Archive& Archive::operator << (const gu::Pair<gu::tstring, gu::int64*>& pair)
{
	if (IsSaveMode())
	{
		const auto value = MakeShared<json::JsonValueNumber>(static_cast<gu::double64>(*pair.Value));
		_object->AddMember(pair.Key, value);
	}
	else if(IsLoadMode())
	{
		*pair.Value = _object->GetMember(pair.Key)->GetInt64();
	}
	else
	{
		throw "Not Supported Mode";
	}
	return *this;
}

/*! @brief Jsonオブジェクトに数値を登録します [uint8]*/
Archive& Archive::operator << (const gu::Pair<gu::tstring, gu::uint8*>& pair)
{
	if (IsSaveMode())
	{
		const auto value = MakeShared<json::JsonValueNumber>(static_cast<gu::double64>(*pair.Value));
		_object->AddMember(pair.Key, value);
	}
	else if(IsLoadMode())
	{
		*pair.Value = _object->GetMember(pair.Key)->GetUInt8();
	}
	else
	{
		throw "Not Supported Mode";
	}
	return *this;
}

/*! @brief Jsonオブジェクトに数値を登録します [uint16]*/
Archive& Archive::operator << (const gu::Pair<gu::tstring, gu::uint16*>& pair)
{
	if (IsSaveMode())
	{
		const auto value = MakeShared<json::JsonValueNumber>(static_cast<gu::double64>(*pair.Value));
		_object->AddMember(pair.Key, value);
	}
	else if(IsLoadMode())
	{
		*pair.Value = _object->GetMember(pair.Key)->GetUInt16();
	}
	else
	{
		throw "Not Supported Mode";
	}
	return *this;
}

/*! @brief Jsonオブジェクトに数値を登録します [uint32]*/
Archive& Archive::operator << (const gu::Pair<gu::tstring, gu::uint32*>& pair)
{
	if (IsSaveMode())
	{
		const auto value = MakeShared<json::JsonValueNumber>(static_cast<gu::double64>(*pair.Value));
		_object->AddMember(pair.Key, value);
	}
	else if(IsLoadMode())
	{
		*pair.Value = _object->GetMember(pair.Key)->GetUInt32();
	}
	else
	{
		throw "Not Supported Mode";
	}
	return *this;
}

/*! @brief Jsonオブジェクトに数値を登録します [uint64]*/
Archive& Archive::operator << (const gu::Pair<gu::tstring, gu::uint64*>& pair)
{
	if (IsSaveMode())
	{
		const auto value = MakeShared<json::JsonValueNumber>(static_cast<gu::double64>(*pair.Value));
		_object->AddMember(pair.Key, value);
	}
	else if(IsLoadMode())
	{
		*pair.Value = _object->GetMember(pair.Key)->GetUInt64();
	}
	else
	{
		throw "Not Supported Mode";
	}
	return *this;
}

/*! @brief Jsonオブジェクトに数値を登録します [float]*/
Archive& Archive::operator << (const gu::Pair<gu::tstring, gu::float32*>& pair)
{
	if (IsSaveMode())
	{
		const auto value = MakeShared<json::JsonValueNumber>(static_cast<gu::double64>(*pair.Value));
		_object->AddMember(pair.Key, value);
	}
	else if(IsLoadMode())
	{
		*pair.Value = _object->GetMember(pair.Key)->GetFloat();
	}
	else
	{
		throw "Not Supported Mode";
	}
	return *this;
}

/*! @brief Jsonオブジェクトに数値を登録します [double]*/
Archive& Archive::operator << (const gu::Pair<gu::tstring, gu::double64*>& pair)
{
	if (IsSaveMode())
	{
		const auto value = MakeShared<json::JsonValueNumber>(*pair.Value);
		_object->AddMember(pair.Key, value);
	}
	else if(IsLoadMode())
	{
		*pair.Value = _object->GetMember(pair.Key)->GetDouble();
	}
	else
	{
		throw "Not Supported Mode";
	}
	return *this;
}

/*! @brief Jsonオブジェクトに数値を登録します [bool]*/
Archive& Archive::operator << (const gu::Pair<gu::tstring, bool*>& pair)
{
	if (IsSaveMode())
	{
		const auto value = MakeShared<json::JsonValueBoolean>(*pair.Value);
		_object->AddMember(pair.Key, value);
	}
	else if(IsLoadMode())
	{
		*pair.Value = _object->GetMember(pair.Key)->GetBool();
	}
	else
	{
		throw "Not Supported Mode";
	}
	return *this;
}

/*! @brief Jsonオブジェクトに文字列を登録します*/
Archive& Archive::operator << (const gu::Pair<gu::tstring, gu::tstring*>& pair)
{
	if (IsSaveMode())
	{
		const auto value = MakeShared<json::JsonValueString>(*pair.Value);
		_object->AddMember(pair.Key, value);
	}
	else if(IsLoadMode())
	{
		*pair.Value = _object->GetMember(pair.Key)->GetString();
	}
	else
	{
		throw "Not Supported Mode";
	}
	return *this;
}
