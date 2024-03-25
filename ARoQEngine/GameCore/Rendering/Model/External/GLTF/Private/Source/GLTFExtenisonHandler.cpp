//////////////////////////////////////////////////////////////////////////////////
//              @file   GLTFDeserialize.cpp
///             @brief  GLTF Deserialize
///             @author Partially edit by Toide
///             @date   2022_05_19
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFExtensionHandlers.hpp"
#include "GameCore/Rendering/Model/External/GLTF/Public/Include/GLTFDocument.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gltf;
using namespace gltf::detail; 

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
TypeKey detail::MakeTypeKey(const GLTFExtension& extension, const GLTFProperty& property)
{
	{ return { typeid(extension), typeid(property) }; }
}
NameKey detail::MakeNameKey(const std::string& name, const GLTFProperty& property) { return { name, typeid(property) }; }
ExtensionPair ExtensionSerializer::Serialize(const detail::GLTFExtension& extension, const detail::GLTFProperty& property, const GLTFDocument document) const
{
	auto it = _typeToName.find(detail::MakeTypeKey(extension, property));
	if (it == _typeToName.end())
	{
		it = _typeToName.find(detail::MakeTypeKey<GLTFPropertyAll>(extension));
	}

	if (it == _typeToName.end())
	{
		throw error::GLTFException("No handler registered to serialize the passed extension type");
	}
	return { it->second, Process(it->first, extension, document, *this) };
}

std::unique_ptr<detail::GLTFExtension> ExtensionDeserializer::DeSerialize(const ExtensionPair& extensionPair, const GLTFProperty& property) const
{
	auto it = _nameToType.find(detail::MakeNameKey(extensionPair.Name, property));

	if (it == _nameToType.end())
	{
		it = _nameToType.find(detail::MakeNameKey<GLTFPropertyAll>(extensionPair.Name));
	}

	if (it == _nameToType.end())
	{
		throw error::GLTFException("No handler registered to deserialize the specified extension name");
	}

	return Process({ it->second, it->first.second }, extensionPair.Value, *this);
}