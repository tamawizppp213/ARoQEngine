//////////////////////////////////////////////////////////////////////////////////
//              @file   GLTFAnimationUtils.cpp
///             @brief  GLTF AnimationUtils
///             @author Copyright (c) Microsoft Corporation. All rights reserved.
///                     Licensed under the MIT License. 
///             @date   2022_05_30
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFAnimationUtils.hpp"
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFAsset.hpp"
#include "GameCore/Rendering/Model/External/GLTF/Public/Include/GLTFResourceReader.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gltf;
using namespace gltf::detail;
using namespace gltf::detail::asset;
using namespace gltf::detail::define;
using namespace gltf::detail::error;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
std::vector<float> AnimationUtils::GetKeyframeTimes(const GLTFDocument& doc, const GLTFResourceReader& reader, const GLTFAccessor& accessor)
{
    if (accessor.AccessorDataType != AccessorType::Type_Scalar)
    {
        throw GLTFException("Invalid type for animation input accessor " + accessor.ID);
    }

    if (accessor.ComponentDataType != ComponentType::Component_Float)
    {
        throw GLTFException("Invalid componentType for animation input accessor " + accessor.ID);
    }

    return reader.ReadBinaryData<float>(doc, accessor);
}

std::vector<float> AnimationUtils::GetKeyframeTimes(const GLTFDocument& doc, const GLTFResourceReader& reader, const GLTFAnimationSampler& sampler)
{
    auto& accessor = doc.Accessors[sampler.InputAccessorID];
    return GetKeyframeTimes(doc, reader, accessor);
}

std::vector<float> AnimationUtils::GetInverseBindMatrices(const GLTFDocument& doc, const GLTFResourceReader& reader, const GLTFAccessor& accessor)
{
    if (accessor.AccessorDataType != AccessorType::Type_Mat4)
    {
        throw GLTFException("Invalid type for inverse bind matrices accessor " + accessor.ID);
    }

    if (accessor.ComponentDataType != ComponentType::Component_Float)
    {
        throw GLTFException("Invalid componentType for inverse bind matrices accessor " + accessor.ID);
    }

    return reader.ReadBinaryData<float>(doc, accessor);
}

std::vector<float> AnimationUtils::GetInverseBindMatrices(const GLTFDocument& doc, const GLTFResourceReader& reader, const GLTFSkin& skin)
{
    auto& accessor = doc.Accessors[skin.InverseBindMatricesAccessorID];
    return GetInverseBindMatrices(doc, reader, accessor);
}

std::vector<float> AnimationUtils::GetTranslations(const GLTFDocument& doc, const GLTFResourceReader& reader, const GLTFAccessor& accessor)
{
    if (accessor.AccessorDataType != AccessorType::Type_Vec3)
    {
        throw GLTFException("Invalid type for translations accessor " + accessor.ID);
    }

    if (accessor.ComponentDataType != ComponentType::Component_Float)
    {
        throw GLTFException("Invalid componentType for translations accessor " + accessor.ID);
    }

    return reader.ReadBinaryData<float>(doc, accessor);
}

std::vector<float> AnimationUtils::GetTranslations(const GLTFDocument& doc, const GLTFResourceReader& reader, const GLTFAnimationSampler& sampler)
{
    auto& accessor = doc.Accessors[sampler.OutputAccessorID];
    return GetTranslations(doc, reader, accessor);
}

std::vector<float> AnimationUtils::GetRotations(const GLTFDocument& doc, const GLTFResourceReader& reader, const GLTFAccessor& accessor)
{
    if (accessor.AccessorDataType != AccessorType::Type_Vec4)
    {
        throw GLTFException("Invalid type for rotations accessor " + accessor.ID);
    }

    return reader.ReadFloatData(doc, accessor);
}

std::vector<float> AnimationUtils::GetRotations(const GLTFDocument& doc, const GLTFResourceReader& reader, const GLTFAnimationSampler& sampler)
{
    auto& accessor = doc.Accessors[sampler.OutputAccessorID];
    return GetRotations(doc, reader, accessor);
}

std::vector<float> AnimationUtils::GetScales(const GLTFDocument& doc, const GLTFResourceReader& reader, const GLTFAccessor& accessor)
{
    if (accessor.AccessorDataType != AccessorType::Type_Vec3)
    {
        throw GLTFException("Invalid type for scales accessor " + accessor.ID);
    }

    if (accessor.ComponentDataType != ComponentType::Component_Float)
    {
        throw GLTFException("Invalid componentType for scales accessor " + accessor.ID);
    }

    return reader.ReadBinaryData<float>(doc, accessor);
}

std::vector<float> AnimationUtils::GetScales(const GLTFDocument& doc, const GLTFResourceReader& reader, const GLTFAnimationSampler& sampler)
{
    auto& accessor = doc.Accessors[sampler.OutputAccessorID];
    return GetScales(doc, reader, accessor);
}

std::vector<float> AnimationUtils::GetMorphWeights(const GLTFDocument& doc, const GLTFResourceReader& reader, const GLTFAccessor& accessor)
{
    if (accessor.AccessorDataType != AccessorType::Type_Scalar)
    {
        throw GLTFException("Invalid type for weights accessor " + accessor.ID);
    }

    return reader.ReadFloatData(doc, accessor);
}

std::vector<float> AnimationUtils::GetMorphWeights(const GLTFDocument& doc, const GLTFResourceReader& reader, const GLTFAnimationSampler& sampler)
{
    auto& accessor = doc.Accessors[sampler.OutputAccessorID];
    return GetMorphWeights(doc, reader, accessor);
}