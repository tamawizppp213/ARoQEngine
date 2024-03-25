//////////////////////////////////////////////////////////////////////////////////
//              @file   Json.cpp
///             @brief  Json
///             @author Toide Yutaro
///             @date   2022_03_12
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/File/Include/Json.hpp"
#include "GameUtility/File/External/rapidjson/document.h"
#include "GameUtility/File/External/rapidjson/istreamwrapper.h"
#include "GameUtility/File/External/rapidjson/ostreamwrapper.h"
#include "GameUtility/File/External/rapidjson/prettywriter.h"
#include <iostream>
#include <fstream>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace json;
using namespace rapidjson;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region JsonDocument
JsonDocument::JsonDocument() : _document(rapidjson::Document())
{
	
}
JsonDocument::~JsonDocument()
{
	
}

JsonDocument::JsonDocument(const std::string& filePath)
{
	Load(filePath);
}

bool JsonDocument::Load(const std::string& filePath)
{
	std::ifstream  ifStream(filePath);
	IStreamWrapper iStream(ifStream);
	_document.ParseStream(iStream);
	return !_document.HasParseError();
}

void JsonDocument::Save(const std::string& filePath)
{
	std::ofstream ofstream(filePath);
	OStreamWrapper ostreamWrapper(ofstream);
	PrettyWriter<OStreamWrapper> writer(ostreamWrapper);
	_document.Accept(writer);
}
#pragma endregion JsonDocument