//////////////////////////////////////////////////////////////////////////////////
//              @file   GLTFValidation.cpp
///             @brief  GLTF Validation
///             @author Copyright (c) Microsoft Corporation. All rights reserved.
///                     Licensed under the MIT License. 
///             @date   2022_05_23
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFValidation.hpp"
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFBufferBuilder.hpp"
#include <algorithm>
#include <sstream>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gltf;
using namespace gltf::detail;
using namespace gltf::detail::asset;
using namespace gltf::detail::validation;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
namespace
{
    template<typename It>
    std::string Join(It it, It itEnd, const char* const delimiter)
    {
        std::stringstream sstream;

        if (it != itEnd)
        {
            sstream << *it++;

            while (it != itEnd)
            {
                sstream << delimiter << *it++;
            }
        }

        return sstream.str();
    }
    /****************************************************************************
    *                       GetAccessorTypesAsString
    *************************************************************************//**
    *  @fn        std::string GetAccessorTypesAsString(const std::set<AccessorType>& accessorTypes)
    *  @brief     Get Accessor Types as string 
    *  @param[in] const std::set<AccessorType>&
    *  @return 　　std::string
    *****************************************************************************/
    std::string GetAccessorTypesAsString(const std::set<AccessorType>& accessorTypes)
    {
        std::vector<std::string> accessorTypeNames(accessorTypes.size());

        std::transform(
            accessorTypes.begin(),
            accessorTypes.end(),
            accessorTypeNames.begin(),
            GLTFAccessor::GetAccessorTypeName);

        return Join(accessorTypeNames.begin(), accessorTypeNames.end(), ", ");
    }
    /****************************************************************************
    *                       GetComponentTypesAsString
    *************************************************************************//**
    *  @fn        std::string GetAccessorTypesAsString(const std::set<ComponentType>& componentTypes)
    *  @brief     Get Accessor Types as string
    *  @param[in] const std::set<ComponentType>&
    *  @return 　　std::string
    *****************************************************************************/
    std::string GetComponentTypesAsString(const std::set<ComponentType>& componentTypes)
    {
        std::vector<std::string> componentTypeNames(componentTypes.size());

        std::transform(
            componentTypes.begin(),
            componentTypes.end(),
            componentTypeNames.begin(),
            GLTFAccessor::GetComponentTypeName);

        return Join(componentTypeNames.begin(), componentTypeNames.end(), ", ");
    }
    /****************************************************************************
    *                       ValidateAccessorsImpl
    *************************************************************************//**
    *  @fn        void ValidateAccessorsImpl(const size_t count, const size_t byteOffset, const ComponentType& componentType,
        const AccessorType& accessorType, const std::string& id, const GLTFBufferView& bufferView, const GLTFBuffer& buffer)
    *  @brief     Validate accessor 
    *  @param[in] size_t count
    *  @param[in] size_t byteOffset
    *  @param[in] const ComponentType& componentType
    *  @param[in] const AccessorType& accessorType
    *  @param[in] const std::string id
    *  @param[in] const GLTFBufferView& bufferView
    *  @param[in] const GLTFBuffer& buffer
    *  @return 　　void
    *****************************************************************************/
    void ValidateAccessorsImpl(const size_t count, const size_t byteOffset, const ComponentType& componentType,
        const AccessorType& accessorType, const std::string& id, const GLTFBufferView& bufferView, const GLTFBuffer& buffer)
    {
        /*-------------------------------------------------------------------
        -                        Validate byte offset
        ---------------------------------------------------------------------*/
        if (byteOffset > bufferView.ByteLength)
        {
            std::string byteLength = std::to_string(bufferView.ByteLength);
            throw error::ValidationException("Accessor" + id + " byteoffset (" + std::to_string(byteOffset) + ") is larger than bufferview byte length (" + byteLength + ")");
        }

        /*-------------------------------------------------------------------
        -      Check the multiplication in accessor.GetByteLength() for overflow
        ---------------------------------------------------------------------*/
        size_t testAccessorByteLength;
        if (!SafeMultiplication(static_cast<size_t>(count),
            (static_cast<size_t>(GLTFAccessor::GetComponentTypeSize(componentType) * GLTFAccessor::GetTypeCount(accessorType))),
            testAccessorByteLength))
        {
            throw error::ValidationException("Accessor" + id + " byte length too large");
        }

        /*-------------------------------------------------------------------
        -            Check ByteLength between safe value and actual value
        ---------------------------------------------------------------------*/
        size_t byteLength = static_cast<size_t>(count * GLTFAccessor::GetComponentTypeSize(componentType) * GLTFAccessor::GetTypeCount(accessorType));

        if (testAccessorByteLength != byteLength)
        {
            throw error::ValidationException("Accessor" + id + " byte length safe value does not match actual value");
        }

        if (byteLength > bufferView.ByteLength)
        {
            std::string accessorByteLengthStr = std::to_string(byteLength);
            std::string bvByteLength = std::to_string(bufferView.ByteLength);
            throw error::ValidationException("Accessor" + id + " byte length (" + accessorByteLengthStr + ") greater than buffer view (" + bvByteLength + ")");
        }

        short accessorComponentTypeSize = GLTFAccessor::GetComponentTypeSize(componentType);
        if ((byteOffset + bufferView.ByteOffset) % accessorComponentTypeSize != 0)
        {
            throw error::ValidationException("Accessor" + id + ": the accessor offset must be a multiple of the size of the accessor component type.");
        }

        ValidateBufferView(bufferView, buffer);
    }

    void ValidateVertexCount(const GLTFMeshMode mode, const size_t count, const std::string& type)
    {
        switch (mode)
        {
        case GLTFMeshMode::Points:
            break;

        case GLTFMeshMode::Lines:
            if (count < 2)
            {
                throw error::ValidationException(type + " count must be at least 2.");
            }

            if (count % 2 != 0)
            {
                throw error::ValidationException(type + " count for MESH_LINES must be a multiple of 2.");
            }
            break;

        case GLTFMeshMode::Line_Loop:
        case GLTFMeshMode::Line_Strip:
            if (count < 2)
            {
                throw error::ValidationException(type + " count must be at least 2.");
            }
            break;

        case GLTFMeshMode::Triangles:
            if (count < 3)
            {
                throw error::ValidationException(type + " count must be at least 3.");
            }

            if (count % 3 != 0)
            {
                throw error::ValidationException(type + " count for MESH_TRIANGLES must be a multiple of 3.");
            }
            break;

        case GLTFMeshMode::Triangle_Fan:
        case GLTFMeshMode::Triangle_Strip:
            if (count < 3)
            {
                throw error::ValidationException(type + " count must be at least 3.");
            }
            break;

        default:
            throw error::ValidationException(type + " invalid mesh mode for validation " + std::to_string(mode));
            break;
        }
    }
}
/****************************************************************************
*                       Validate
*************************************************************************//**
*  @fn        void gltf::detail::validation::Validate(const GLTFDocument& document)
*  @brief     Validate accessors and meshes
*  @param[in] const GLTFDocument& document
*  @return 　　void
*****************************************************************************/
void gltf::detail::validation::Validate(const GLTFDocument& document)
{
    ValidateAccessors(document);
    ValidateMeshes(document);
}
/****************************************************************************
*                       ValidateAccessors
*************************************************************************//**
*  @fn        void gltf::detail::validation::ValidateAccessors(const GLTFDocument& document)
*  @brief     Validate accessors
*  @param[in] const GLTFDocument& document
*  @return 　　void
*****************************************************************************/
void gltf::detail::validation::ValidateAccessors(const GLTFDocument& document)
{
    for (const auto& accessor : document.Accessors.Elements())
    {
        ValidateAccessor(document, accessor);
    }
}
/****************************************************************************
*                       ValidateMeshes
*************************************************************************//**
*  @fn        void gltf::detail::validation::ValidateMeshes(const GLTFDocument& document)
*  @brief     Validate meshes
*  @param[in] const GLTFDocument& document
*  @return 　　void
*****************************************************************************/
void gltf::detail::validation::ValidateMeshes(const GLTFDocument& document)
{
    for (const auto& mesh : document.Meshes.Elements())
    {
        for (const auto& primitive : mesh.Primitives)
        {
            ValidateMeshPrimitive(document, primitive);
        }
    }
}
/****************************************************************************
*                       ValidateMeshPrimitive
*************************************************************************//**
*  @fn        void gltf::detail::validation::ValidateMeshPrimitive(const GLTFDocument& document, const asset::GLTFMeshPrimitive& primitive)
*  @brief     Validate mesh primitive
*  @param[in] const GLTFDocument& document
*  @param[in] const GLTFMeshPrimitive& primitive
*  @return 　　void
*****************************************************************************/
void gltf::detail::validation::ValidateMeshPrimitive(const GLTFDocument& document, const asset::GLTFMeshPrimitive& primitive)
{
    if (!primitive.HasAttribute(define::ACCESSOR_POSITION))
    {
        throw error::ValidationException("MeshPrimitive mush have POSITION attribute");
    }

    size_t vertexCount = document.Accessors.Get(primitive.GetAttributeAccessorID(define::ACCESSOR_POSITION)).Count;

    const auto& indicesAccessorId = primitive.IndicesAccessorID;
    if (!indicesAccessorId.empty())
    {
        const auto& indicesAccessor = document.Accessors.Get(indicesAccessorId);
        ValidateAccessorTypes(indicesAccessor, "indices", { AccessorType::Type_Scalar }, { ComponentType::Component_Unsigned_Byte, ComponentType::Component_Unsigned_Short, ComponentType::Component_Unsigned_Int });
        ValidateVertexCount(primitive.MeshMode, indicesAccessor.Count, "Index");
    }
    else
    {
        ValidateVertexCount(primitive.MeshMode, vertexCount, "Position");
    }

    ValidateMeshPrimitiveAttributeAccessors(document, primitive.Attributes, vertexCount);
}
/****************************************************************************
*                       ValidateMeshPrimitiveAttributeAccessors
*************************************************************************//**
*  @fn        void gltf::detail::validation::ValidateMeshPrimitiveAttributeAccessors(const GLTFDocument& document, const std::unordered_map<std::string, std::string>& attributes, const size_t vertexCount)
*  @brief     Validate mesh primitive attribute
*  @param[in] const GLTFDocument& document
*  @param[in]  const std::unordered_map<std::string, std::string>& attributes, 
*  @param[in] const size_t vertexCount
*  @return 　　void
*****************************************************************************/
void gltf::detail::validation::ValidateMeshPrimitiveAttributeAccessors(const GLTFDocument& document, const std::unordered_map<std::string, std::string>& attributes, const size_t vertexCount)
{
    static const std::unordered_map <std::string, std::pair<std::set<AccessorType>, std::set<ComponentType>>> attributeDefinitions =
    {
        { define::ACCESSOR_POSITION,   { { AccessorType::Type_Vec3 },            { ComponentType::Component_Float } } },
        { define::ACCESSOR_NORMAL,     { { AccessorType::Type_Vec3 },            { ComponentType::Component_Float } } },
        { define::ACCESSOR_TANGENT,    { { AccessorType::Type_Vec4 },            { ComponentType::Component_Float } } },
        { define::ACCESSOR_TEXCOORD_0, { { AccessorType::Type_Vec2 },            { ComponentType::Component_Float, ComponentType::Component_Unsigned_Byte, ComponentType::Component_Unsigned_Short } } },
        { define::ACCESSOR_TEXCOORD_1, { { AccessorType::Type_Vec2 },            { ComponentType::Component_Float, ComponentType::Component_Unsigned_Byte, ComponentType::Component_Unsigned_Short  } } },
        { define::ACCESSOR_COLOR_0,    { { AccessorType::Type_Vec3, AccessorType::Type_Vec4 }, { ComponentType::Component_Float, ComponentType::Component_Unsigned_Byte, ComponentType::Component_Unsigned_Short  } } },
        { define::ACCESSOR_JOINTS_0,   { { AccessorType::Type_Vec4 },            {                  ComponentType::Component_Unsigned_Byte, ComponentType::Component_Unsigned_Short  } } },
        { define::ACCESSOR_WEIGHTS_0,  { { AccessorType::Type_Vec4 },            { ComponentType::Component_Float, ComponentType::Component_Unsigned_Byte, ComponentType::Component_Unsigned_Short  } } }
    };

    // TODO: Validate by prefix TEXCOORD_/COLOR_/JOINTS_/WEIGHTS_ 
    for (const auto& attribute : attributes)
    {
        const auto& attributeName = attribute.first;
        const auto& attributeAccessorId = attribute.second;

        const auto it = attributeDefinitions.find(attributeName);
        if (it != attributeDefinitions.end())
        {
            const auto& accessor = document.Accessors.Get(attributeAccessorId);
            ValidateAccessorTypes(accessor, attributeName, it->second.first, it->second.second);

            if (accessor.Count != vertexCount)
            {
                throw error::ValidationException(
                    "MeshPrimitive attribute '" + std::string(attributeName) + "' had an incorrect count ("
                    + std::to_string(accessor.Count) + " vs. " + std::to_string(vertexCount));
            }
        }
    }
}
/****************************************************************************
*                         ValidateAccessorTypes
*************************************************************************//**
*  @fn        void gltf::detail::validation::ValidateAccessorTypes(const asset::GLTFAccessor& accessor, const std::string& accessorName,
	          const std::set<asset::AccessorType>& accessorTypes, const std::set<asset::ComponentType>& componentTypes)
*  @brief     Validate accessor types
*  @param[in] const asset::GLTFAccessor& accessor,
*  @param[in] const std::string& accessorName,
*  @param[in] const std::set<asset::AccessorType>& accessorTypes, 
*  @param[in] const std::set<asset::ComponentType>& componentTypes
*  @return 　　void
*****************************************************************************/
void gltf::detail::validation::ValidateAccessorTypes(const asset::GLTFAccessor& accessor, const std::string& accessorName,
	const std::set<asset::AccessorType>& accessorTypes, const std::set<asset::ComponentType>& componentTypes)
{
    /*-------------------------------------------------------------------
    -                      Validate Accessor data type
    ---------------------------------------------------------------------*/
    if (accessorTypes.find(accessor.AccessorDataType) == accessorTypes.end())
    {
        throw error::ValidationException(
            "Accessor " + accessor.ID + " " + accessorName + " type must be: [" + GetAccessorTypesAsString(accessorTypes) + "]"
        );
    }
    /*-------------------------------------------------------------------
    -                      Validate Component data type
    ---------------------------------------------------------------------*/
    if (componentTypes.find(accessor.ComponentDataType) == componentTypes.end())
    {
        throw error::ValidationException(
            "Accessor " + accessor.ID + " " + accessorName + " componentType must be: [" + GetComponentTypesAsString(componentTypes) + "]"
        );
    }
}
/****************************************************************************
*                         ValidateAccessor
*************************************************************************//**
*  @fn        void gltf::detail::validation::ValidateAccessor(const GLTFDocument& document, const asset::GLTFAccessor& accessor)
*  @brief     Validate accessor
*  @param[in] const GLTFDocument& document, 
*  @param[in] const asset::GLTFAccessor& accessor
*  @return 　　void
*****************************************************************************/
void gltf::detail::validation::ValidateAccessor(const GLTFDocument& document, const asset::GLTFAccessor& accessor)
{
    /*-------------------------------------------------------------------
    -                      Validate buffer view
    ---------------------------------------------------------------------*/
    if (!accessor.BufferViewID.empty())
    {
        const GLTFBufferView& bufferView = document.BufferViews.Get(accessor.BufferViewID);
        const GLTFBuffer&     buffer     = document.Buffers.Get(bufferView.BufferID);
        ValidateAccessorsImpl(accessor.Count, accessor.ByteOffset, accessor.ComponentDataType, accessor.AccessorDataType, accessor.ID, bufferView, buffer);
    }
    /*-------------------------------------------------------------------
    -                      Validate sparse count
    ---------------------------------------------------------------------*/
    if (accessor.Sparse.Count > 0U)
    {
        const GLTFBufferView& indicesBufferView = document.BufferViews.Get(accessor.Sparse.IndicesBufferViewID);
        const GLTFBuffer& indicesBuffer = document.Buffers.Get(indicesBufferView.BufferID);
        std::string indices_id = accessor.ID + "_sparseIndices";
        ValidateAccessorsImpl(accessor.Sparse.Count, accessor.Sparse.IndicesByteOffset, accessor.Sparse.IndicesComponentType,
            AccessorType::Type_Scalar, indices_id, indicesBufferView, indicesBuffer);

        const GLTFBufferView& valuesBufferView = document.BufferViews.Get(accessor.Sparse.ValuesBufferViewID);
        const GLTFBuffer&     valuesBuffer     = document.Buffers    .Get(valuesBufferView.BufferID);
        std::string valuesID = accessor.ID + "_sparseValues";
        ValidateAccessorsImpl(accessor.Sparse.Count, accessor.Sparse.ValuesByteOffset, accessor.ComponentDataType,
            accessor.AccessorDataType, valuesID, valuesBufferView, valuesBuffer);
    }
}
/****************************************************************************
*                         ValidateBufferView
*************************************************************************//**
*  @fn        void gltf::detail::validation::ValidateBufferView(const asset::GLTFBufferView& bufferView, const asset::GLTFBuffer& buffer)
*  @brief     Validate buffer view
*  @param[in] const asset::GLTFBufferView& bufferView, 
*  @param[in] const asset::GLTFBuffer& buffer
*  @return 　　void
*****************************************************************************/
void gltf::detail::validation::ValidateBufferView(const asset::GLTFBufferView& bufferView, const asset::GLTFBuffer& buffer)
{
    /*-------------------------------------------------------------------
    -                      Buffer Size Check
    ---------------------------------------------------------------------*/
    size_t totalBufferViewLength;
    if (!SafeAddition(bufferView.ByteOffset, bufferView.ByteLength, totalBufferViewLength))
    {
        throw error::ValidationException("Buffer view size too large");
    }
    /*-------------------------------------------------------------------
   -                      Report error
   ---------------------------------------------------------------------*/
    if (totalBufferViewLength > buffer.ByteLength)
    {
        std::string totalBufferViewLengthStr = std::to_string(totalBufferViewLength);
        std::string byteLength = std::to_string(buffer.ByteLength);
        throw error::ValidationException("BufferView " + bufferView.BufferID + " offset + length (" + totalBufferViewLengthStr + ") greater than buffer length (" + byteLength + ")");
    }
}

bool gltf::detail::validation::SafeAddition(size_t a, size_t b, size_t& result)
{
    if (b <= std::numeric_limits<size_t>::max() - a)
    {
        // No overflow
        result = a + b;
        return true;
    }

    return false;
}
bool gltf::detail::validation::SafeMultiplication(size_t a, size_t b, size_t& result)
{
    if (b == 0)
    {
        result = 0;
        return true;
    }

    if (a <= std::numeric_limits<size_t>::max() / b)
    {
        // No overflow
        result = a * b;
        return true;
    }

    return false;
}