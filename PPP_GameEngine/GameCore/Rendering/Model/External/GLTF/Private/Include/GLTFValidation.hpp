//////////////////////////////////////////////////////////////////////////////////
///             @file   GLTFValidation.hpp
///             @brief  Validate each data
///             @author Copyright (c) Microsoft Corporation. All rights reserved.Licensed under the MIT License.
///                     Partially edit by Toide
///             @date   2022_05_23  
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GLTF_VALIDATION_HPP
#define GLTF_VALIDATION_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/GLTF/Public/Include/GLTFDocument.hpp"
#include <set>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gltf
{
	namespace detail
	{

		namespace validation
		{
			void Validate(const GLTFDocument& document);
			void ValidateAccessors(const GLTFDocument& document);
			void ValidateMeshes(const GLTFDocument& document);
			void ValidateMeshPrimitive(const GLTFDocument& document, const asset::GLTFMeshPrimitive& primitive);
			void ValidateMeshPrimitiveAttributeAccessors(const GLTFDocument& document, const std::unordered_map<std::string, std::string>& attributes, const size_t vertexCount);
			void ValidateAccessorTypes(const asset::GLTFAccessor& accessor, const std::string& accessorName,
				const std::set<asset::AccessorType>& accessorTypes, const std::set<asset::ComponentType>& componentTypes);
			void ValidateAccessor(const GLTFDocument& document, const asset::GLTFAccessor& accessor);
			void ValidateBufferView(const asset::GLTFBufferView& bufferView, const asset::GLTFBuffer& buffer);

			bool SafeAddition(size_t a, size_t b, size_t& result);
			bool SafeMultiplication(size_t a, size_t b, size_t& result);
		}
	}
}
#endif