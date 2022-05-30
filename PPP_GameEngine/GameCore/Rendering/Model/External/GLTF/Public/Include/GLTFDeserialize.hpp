//////////////////////////////////////////////////////////////////////////////////
///             @file   GLTFDocument.hpp
///             @brief  Document
///             @author Copyright (c) Microsoft Corporation. All rights reserved.Licensed under the MIT License.
///                     Partially edit by Toide
///             @date   2022_05_12  
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GLTF_DESERIALIZE_HPP
#define GLTF_DESERIALIZE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GLTFDocument.hpp"
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFSchema.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gltf
{
	enum class DeserializeFlags
	{
		None = 0x0,
		IgnoreByteOrderMark
	};

	DeserializeFlags  operator| (DeserializeFlags lhs, DeserializeFlags rhs);
	DeserializeFlags& operator|=(DeserializeFlags& lhs, DeserializeFlags rhs);
	DeserializeFlags  operator& (DeserializeFlags lhs, DeserializeFlags rhs);
	DeserializeFlags& operator&=(DeserializeFlags& lhs, DeserializeFlags rhs);

	class ExtensionDeserializer;
	GLTFDocument Deserialize(const std::string& json, DeserializeFlags flag = DeserializeFlags::None, detail::define::SchemaFlags schemaFlags = detail::define::SchemaFlags::None);
	GLTFDocument Deserialize(const std::string& json, const ExtensionDeserializer& extensionDeserializer, DeserializeFlags flag = DeserializeFlags::None, detail::define::SchemaFlags schemaFlags = detail::define::SchemaFlags::None);
	GLTFDocument Deserialize(std::istream& jsonStream, DeserializeFlags flags = DeserializeFlags::None, detail::define::SchemaFlags schemaFlags = detail::define::SchemaFlags::None);
	GLTFDocument Deserialize(std::istream& jsonStream, const ExtensionDeserializer& extensionDeserializer , DeserializeFlags flags = DeserializeFlags::None, detail::define::SchemaFlags schemaFlags = detail::define::SchemaFlags::None);

}
#endif