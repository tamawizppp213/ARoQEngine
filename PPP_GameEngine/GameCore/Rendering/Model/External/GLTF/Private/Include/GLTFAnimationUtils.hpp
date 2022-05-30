//////////////////////////////////////////////////////////////////////////////////
///             @file   GLTFAnimationUtils.hpp
///             @brief  Animation Utils
///             @author Copyright (c) Microsoft Corporation. All rights reserved.Licensed under the MIT License.
///                     Partially edit by Toide
///             @date   2022_05_23  
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GLTF_ANIMATION_UTILS_HPP
#define GLTF_ANIMATION_UTILS_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include <vector>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gltf
{
    class  GLTFDocument;
    class  GLTFResourceReader;
 
    namespace detail
    {
        namespace asset
        {
            struct GLTFAccessor;
            struct GLTFAnimationSampler;
            struct GLTFSkin;
        }
    }

	namespace AnimationUtils
	{
        std::vector<float> GetKeyframeTimes(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFAccessor& accessor);
        std::vector<float> GetKeyframeTimes(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFAnimationSampler& sampler);

        std::vector<float> GetInverseBindMatrices(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFAccessor& skin);
        std::vector<float> GetInverseBindMatrices(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFSkin& skin);

        std::vector<float> GetTranslations(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFAccessor& accessor);
        std::vector<float> GetTranslations(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFAnimationSampler& accessor);

        std::vector<float> GetRotations(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFAccessor& accessor);
        std::vector<float> GetRotations(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFAnimationSampler& accessor);

        std::vector<float> GetScales(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFAccessor& accessor);
        std::vector<float> GetScales(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFAnimationSampler& accessor);

        std::vector<float> GetMorphWeights(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFAccessor& accessor);
        std::vector<float> GetMorphWeights(const GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFAnimationSampler& accessor);
	}
}
#endif