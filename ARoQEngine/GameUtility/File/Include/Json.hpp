//////////////////////////////////////////////////////////////////////////////////
///             @file   Json.hpp
///             @brief  Json reader and writer:
///                     How To : Load -> std::map like Object->call["name"] or Array->call[index]-> Save etc
///             @author Toide Yutaro (dependency : rapidjson)
///             @date   2022_05_12  
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef JSON_HPP
#define JSON_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/File/External/rapidjson/document.h"
#include <string>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace json
{

	/****************************************************************************
	*				  			JsonDocument
	*************************************************************************//**
	*  @class     JsonDocument
	*  @brief     Document
	*****************************************************************************/
	class JsonDocument
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		bool   Load(const std::string& filePath);
		void   Save(const std::string& filePath);
		inline rapidjson::Document& Ref() { return _document; }

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		JsonDocument();
		explicit JsonDocument(const std::string& filePath);
		~JsonDocument();

		      rapidjson::Value& operator[](unsigned int index)       { return _document[index]; }
		      rapidjson::Value& operator[](const char* name)         { return _document[name]; }
		const rapidjson::Value& operator[](unsigned int index) const { return _document[index]; }
		const rapidjson::Value& operator[](const char* name)   const { return _document[name]; }
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		rapidjson::Document _document;
	};


}
#endif