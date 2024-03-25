//////////////////////////////////////////////////////////////////////////////////
//              @file   GLTFSchemaValidation.cpp
///             @brief  GLTF Schema Validation
///             @author // Copyright (c) Microsoft Corporation. All rights reserved.
//						   Licensed under the MIT License. 
//                         Partially edit by Toide
///             @date   2022_05_22
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFSchemaValidation.hpp"
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFExceptions.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gltf;
using namespace gltf::detail;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
namespace
{
	/****************************************************************************
	*				  			TemplateClass
	*************************************************************************//**
	*  @class     TemplateClass
	*  @brief     temp
	*****************************************************************************/
	class RemoteSchemaDocumentProvider : public rapidjson::IRemoteSchemaDocumentProvider
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		const rapidjson::SchemaDocument* GetRemoteDocument(const std::string& uri)
		{
			auto iterator = _schemaDocuments.find(uri);
			if (iterator != _schemaDocuments.end()) { return &(iterator->second); }

			rapidjson::Document document;
			if (document.Parse(SchemaLocatorPtr->GetSchemaContent(uri)).HasParseError())
			{
				throw error::GLTFException("Schema document at " + uri + "is not valid JSON");
			}

			auto result = _schemaDocuments.emplace(uri, rapidjson::SchemaDocument(document, nullptr, 0, this));
			assert(result.second);
			auto resultSchemaDocument = &(result.first->second);
			assert(resultSchemaDocument);
			return resultSchemaDocument;
		}
		const rapidjson::SchemaDocument* GetRemoteDocument(const char* uri, rapidjson::SizeType length) override
		{
			return GetRemoteDocument({ uri, length });
		}
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		const std::unique_ptr<const ISchemaLocator> SchemaLocatorPtr;
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RemoteSchemaDocumentProvider(std::unique_ptr<const ISchemaLocator> schemaLocator) : SchemaLocatorPtr(std::move(schemaLocator))
		{
			assert(this->SchemaLocatorPtr);
		}
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		std::unordered_map<std::string, rapidjson::SchemaDocument> _schemaDocuments;
	};
}

void gltf::detail::ValidateDocumentAgainstSchema(const rapidjson::Document& jsonDocument, const std::string& schemaUri, std::unique_ptr<const ISchemaLocator> schemaLocator)
{
	if (!schemaLocator) { throw error::GLTFException("ISchemaLocator instance must not be null"); }

	RemoteSchemaDocumentProvider provider(std::move(schemaLocator));
	if (auto* schemaDocument = provider.GetRemoteDocument(schemaUri))
	{
		rapidjson::SchemaValidator schemaValidator(*schemaDocument);
		if (!jsonDocument.Accept(schemaValidator))
		{
			rapidjson::StringBuffer sb;

			const std::string schemaKeyword = schemaValidator.GetInvalidSchemaKeyword();
			schemaValidator.GetInvalidDocumentPointer().StringifyUriFragment(sb);
			const std::string schemaInvalid = sb.GetString();

			throw error::ValidationException("Schema violation at " + schemaInvalid + " due to " + schemaKeyword);
		}
	}
	else
	{
		throw error::GLTFException("Schema document at " + schemaUri + " could not be located");
	}

}