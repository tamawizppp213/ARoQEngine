//////////////////////////////////////////////////////////////////////////////////
///             @file   GLTFMeshPrimitiveUtils.hpp
///             @brief  Mesh Primitive Utils
///             @author Copyright (c) Microsoft Corporation. All rights reserved.Licensed under the MIT License.
///                     Partially edit by Toide
///             @date   2022_05_12  
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GLTF_MESH_PRIMITIVES_UTILS_HPP
#define GLTF_MESH_PRIMITIVES_UTILS_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFAsset.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gltf
{
    class GLTFDocument;
    class GLTFResourceReader;

    namespace MeshPrimitiveUtils
    {
        std::vector<uint16_t> GetIndices16(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFAccessor& accessor);
        std::vector<uint16_t> GetIndices16(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFMeshPrimitive& meshPrimitive);

        std::vector<uint32_t> GetIndices32(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFAccessor& accessor);
        std::vector<uint32_t> GetIndices32(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFMeshPrimitive& meshPrimitive);

        std::vector<uint16_t> GetTriangulatedIndices16(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFMeshPrimitive& meshPrimitive);
        std::vector<uint32_t> GetTriangulatedIndices32(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFMeshPrimitive& meshPrimitive);

        std::vector<uint16_t> GetSegmentedIndices16(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFMeshPrimitive& meshPrimitive);
        std::vector<uint32_t> GetSegmentedIndices32(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFMeshPrimitive& meshPrimitive);

        std::vector<float> GetPositions(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFAccessor& accessor);
        std::vector<float> GetPositions(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFMeshPrimitive& meshPrimitive);
        std::vector<float> GetPositions(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFMorphTarget& morphTarget);

        std::vector<float> GetNormals(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFAccessor& accessor);
        std::vector<float> GetNormals(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFMeshPrimitive& meshPrimitive);
        std::vector<float> GetNormals(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFMorphTarget& morphTarget);

        std::vector<float> GetTangents(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFAccessor& accessor);
        std::vector<float> GetTangents(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFMeshPrimitive& meshPrimitive);
        std::vector<float> GetTangents(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFMorphTarget& morphTarget);
        std::vector<float> GetMorphTangents(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFAccessor& accessor);

        std::vector<float> GetTexCoords(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFAccessor& accessor);
        std::vector<float> GetTexCoords_0(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFMeshPrimitive& meshPrimitive);
        std::vector<float> GetTexCoords_1(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFMeshPrimitive& meshPrimitive);

        std::vector<uint32_t> GetColors(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFAccessor& accessor);
        std::vector<uint32_t> GetColors_0(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFMeshPrimitive& meshPrimitive);

        std::vector<uint32_t> GetJointIndices32(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFAccessor& accessor);
        std::vector<uint32_t> GetJointIndices32_0(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFMeshPrimitive& meshPrimitive);

        std::vector<uint64_t> GetJointIndices64(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFAccessor& accessor);
        std::vector<uint64_t> GetJointIndices64_0(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFMeshPrimitive& meshPrimitive);

        std::vector<uint32_t> GetJointWeights32(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFAccessor& accessor);
        std::vector<uint32_t> GetJointWeights32_0(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFMeshPrimitive& meshPrimitive);

        std::vector<uint16_t> ReverseTriangulateIndices16(const uint16_t* indices, size_t indexCount, detail::asset::GLTFMeshMode mode);
        std::vector<uint32_t> ReverseTriangulateIndices32(const uint32_t* indices, size_t indexCount, detail::asset::GLTFMeshMode mode);

        std::vector<uint16_t> ReverseTriangulateIndices16(const std::vector<uint16_t>& indices, detail::asset::GLTFMeshMode mode);
        std::vector<uint32_t> ReverseTriangulateIndices32(const std::vector<uint32_t>& indices, detail::asset::GLTFMeshMode mode);

        std::vector<uint16_t> ReverseSegmentIndices16(const uint16_t* indices, size_t indexCount, detail::asset::GLTFMeshMode mode);
        std::vector<uint32_t> ReverseSegmentIndices32(const uint32_t* indices, size_t indexCount, detail::asset::GLTFMeshMode mode);

        std::vector<uint16_t> ReverseSegmentIndices16(const std::vector<uint16_t>& indices, detail::asset::GLTFMeshMode mode);
        std::vector<uint32_t> ReverseSegmentIndices32(const std::vector<uint32_t>& indices, detail::asset::GLTFMeshMode mode);
    }
}
#endif