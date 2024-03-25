//////////////////////////////////////////////////////////////////////////////////
//              @file   GLTFProperty.cpp
///             @brief  GLTF Property
///             @author Copyright (c) Microsoft Corporation. All rights reserved.
///                     Licensed under the MIT License. 
///             @date   2022_05_14
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFProperty.hpp"
#include <algorithm>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gltf;
using namespace gltf::detail;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
GLTFProperty::~GLTFProperty()
{

}
GLTFProperty::GLTFProperty(const GLTFProperty& other)
{
    for (const auto& extension : other._registeredExtensions)
    {
        _registeredExtensions.emplace(extension.first, extension.second->Clone());
    }
}
GLTFProperty& GLTFProperty::operator=(const GLTFProperty& other)
{
    if (this != &other)
    {
        GLTFProperty otherCopy(other);
        Extensions            = std::move(otherCopy.Extensions);
        _registeredExtensions = std::move(otherCopy._registeredExtensions);
        Extras                = std::move(otherCopy.Extras);
    }

    return *this;
}
/****************************************************************************
*							GetExtensions
*************************************************************************//**
*  @fn        std::vector<std::reference_wrapper<GLTFExtension> >GLTFProperty::GetExtensions() const
*  @brief     Get All Extension in this instance.
*  @param[in] void
*  @return Å@Å@std::vector<std::reference_wrapper<GLTFExtension> 
*****************************************************************************/
std::vector<std::reference_wrapper<GLTFExtension> >GLTFProperty::GetExtensions() const 
{
    std::vector<std::reference_wrapper<GLTFExtension>> exts;
    for (auto& registeredExt : _registeredExtensions)
    {
        exts.push_back(*registeredExt.second);
    }
    return exts;
}
bool GLTFProperty::Equals(const GLTFProperty& lhs, const GLTFProperty& rhs)
{
    auto fnRegisteredExtensionsEquals = [](const GLTFProperty& lhs, const GLTFProperty& rhs)
    {
        if (lhs._registeredExtensions.size() == rhs._registeredExtensions.size())
        {
            return std::all_of(
                lhs._registeredExtensions.begin(),
                lhs._registeredExtensions.end(),
                [&rhs](const std::pair<const std::type_index, std::unique_ptr<GLTFExtension>>& value)
                {
                    auto it = rhs._registeredExtensions.find(value.first);

                    if (it != rhs._registeredExtensions.end())
                    {
                        return it->second->IsEqual(*value.second);
                    }

                    return false;
                });
        }

        return false;
    };

    return lhs.Extensions == rhs.Extensions
        && lhs.Extras == rhs.Extras
        && fnRegisteredExtensionsEquals(lhs, rhs);
}