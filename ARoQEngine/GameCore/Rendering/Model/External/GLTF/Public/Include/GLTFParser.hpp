//////////////////////////////////////////////////////////////////////////////////
///             @file   Json.hpp
///             @brief  Json reader and writer:
///                     How To : Load -> std::map like Object->call["name"] or Array->call[index]-> Save etc
///             @author Toide Yutaro (dependency : rapidjson)
///             @date   2022_05_12  
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GLTF_PARSER_HPP
#define GLTF_PARSER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GLTFDocument.hpp"
#include "GLTFSerialize.hpp"
#include <vector>
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gltf
{
	using namespace gm;

	class GLTFResourceReader;
	class GLTFResourceWriter;
	/****************************************************************************
	*				  			JsonDocument
	*************************************************************************//**
	*  @class     JsonDocument
	*  @brief     Document
	*****************************************************************************/
	class GLTFFile
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Load(const std::string& filePath);
		void Save(const std::string& filePath, SerializeFlags flags = SerializeFlags::None); // まだ使用しないでください
		/****************************************************************************
		**                Public Property
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GLTFDocument Document;
		std::shared_ptr<GLTFResourceReader> ResourceReader;
		std::shared_ptr<GLTFResourceWriter> ResourceWriter;
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/

	};


}
#endif