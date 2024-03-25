//////////////////////////////////////////////////////////////////////////////////
///             @file   GLTFDocument.hpp
///             @brief  Document
///             @author Copyright (c) Microsoft Corporation. All rights reserved.Licensed under the MIT License.
///                     Partially edit by Toide
///             @date   2022_05_12  
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GLTF_SERIALIZE_HPP
#define GLTF_SERIALIZE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GLTFDocument.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gltf
{
	enum class SerializeFlags
	{
		None = 0x0,
		Pretty = 0x1
	};

	SerializeFlags  operator| (SerializeFlags lhs, SerializeFlags rhs);
	SerializeFlags& operator|=(SerializeFlags& lhs, SerializeFlags rhs);
	SerializeFlags  operator& (SerializeFlags lhs, SerializeFlags rhs);
	SerializeFlags& operator&=(SerializeFlags& lhs, SerializeFlags rhs);

	class ExtensionSerializer;
	std::string Serialize(const GLTFDocument& gltfDocument, SerializeFlags flags = SerializeFlags::Pretty);
	std::string Serialize(const GLTFDocument& gltfDocument, const ExtensionSerializer& extensionHandler, SerializeFlags flags = SerializeFlags::None);


}
#endif