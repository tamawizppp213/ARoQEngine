//////////////////////////////////////////////////////////////////////////////////
///             @file   GLTFExtensionHandler.hpp
///             @brief  Extension Handler Class:
///             @author // Copyright (c) Microsoft Corporation. All rights reserved.
///                        Licensed under the MIT License.
///                     Partially edit by Toide
///             @date   2022_05_22 
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GLTF_SCHEMA_VALIDATION_HPP
#define GLTF_SCHEMA_VALIDATION_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GLTFRapidJsonUtils.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Class
//////////////////////////////////////////////////////////////////////////////////
namespace gltf
{
	namespace detail
	{
		/****************************************************************************
		*				  			SchemaValidation
		****************************************************************************/
		/* @class     SchemaValidation
		*  @brief     Schema validation
		*****************************************************************************/
		class ISchemaLocator
		{
		public:
			/****************************************************************************
			**                Public Function
			*****************************************************************************/
			/****************************************************************************
			**                Public Property
			*****************************************************************************/

			/****************************************************************************
			**                Constructor and Destructor
			*****************************************************************************/
			virtual ~ISchemaLocator() = default;
			virtual const char* GetSchemaContent(const std::string& uri) const = 0;
		private:
			/****************************************************************************
			**                Private Function
			*****************************************************************************/

			/****************************************************************************
			**                Private Property
			*****************************************************************************/
		};

		void ValidateDocumentAgainstSchema(const rapidjson::Document& jsonDocument, const std::string& schemaUri, std::unique_ptr<const ISchemaLocator> schemaLocator);
	}
}
#endif