//////////////////////////////////////////////////////////////////////////////////
///             @file   ModelConverter.hpp
///             @brief  ModelConverter
///             @author Toide Yutaro
///             @date   2022_06_06
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef MODEL_CONVERTER_HPP
#define MODEL_CONVERTER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include <string>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class Model;
//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
class ModelConverter
{
	bool ToModel(const std::wstring& filePath, Model* model);
	bool FromModel(const std::wstring& filePath, const Model* model);
};


#endif