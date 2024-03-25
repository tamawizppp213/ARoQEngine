//////////////////////////////////////////////////////////////////////////////////
//              @file   GLTFParser.cpp
///             @brief  GLTF File Parser
///             @author Toide Yutaro
///             @date   2022_05_05
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFAsset.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gltf;
using namespace gltf::detail::asset;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
bool GLTFAsset::operator==(const GLTFAsset& rhs) const
{
	return GLTFProperty::Equals(*this, rhs)
		&& this->CopyRight  == rhs.CopyRight
		&& this->Generator  == rhs.Generator
		&& this->Version    == rhs.Version
		&& this->MinVersion == rhs.MinVersion;
}
#pragma region GLTFAccessor
AccessorType  GLTFAccessor::ParseType(const std::string& type)
{
    const auto result = AccessorTypes.find(type);
    if (result != AccessorTypes.end())
    {
        return result->second;
    }
    throw error::GLTFException("Unknown type" + type);
}
std::string   GLTFAccessor::GetAccessorTypeName(AccessorType type)
{
    switch (type)
    {
        case AccessorType::Type_Scalar: return define::TYPE_NAME_SCALAR;
        case AccessorType::Type_Vec2  : return define::TYPE_NAME_VEC2;
        case AccessorType::Type_Vec3  : return define::TYPE_NAME_VEC3;
        case AccessorType::Type_Vec4  : return define::TYPE_NAME_VEC4;
        case AccessorType::Type_Mat2  : return define::TYPE_NAME_MAT2;
        case AccessorType::Type_Mat3  : return define::TYPE_NAME_MAT3;
        case AccessorType::Type_Mat4  : return define::TYPE_NAME_MAT4;
        default:
            throw error::GLTFException("Unknown type" + std::to_string(((int)type)));
    }
}
uint8_t       GLTFAccessor::GetTypeCount(AccessorType type)
{
    switch (type)
    {
        case AccessorType::Type_Scalar: return 1;
        case AccessorType::Type_Vec2  : return 2;
        case AccessorType::Type_Vec3  : return 3;
        case AccessorType::Type_Vec4  : return 4;
        case AccessorType::Type_Mat2  : return 4;
        case AccessorType::Type_Mat3  : return 9;
        case AccessorType::Type_Mat4  : return 16;
        default:
            throw error::GLTFException("Unknown type" + std::to_string(((int)type)));
    }
}
ComponentType GLTFAccessor::GetComponentType(uint32_t value)
{
    switch ((ComponentType)value)
    {
        case ComponentType::Component_Byte          : return ComponentType::Component_Byte;
        case ComponentType::Component_Unsigned_Byte : return ComponentType::Component_Unsigned_Byte;
        case ComponentType::Component_Short         : return ComponentType::Component_Short;
        case ComponentType::Component_Unsigned_Short: return ComponentType::Component_Unsigned_Short;
        case ComponentType::Component_Unsigned_Int  : return ComponentType::Component_Unsigned_Int;
        case ComponentType::Component_Float         : return ComponentType::Component_Float;
        default:
            return ComponentType::Component_Unknown;
    }
}
std::string   GLTFAccessor::GetComponentTypeName(ComponentType componentType)
{
    switch (componentType)
    {
        case ComponentType::Component_Byte          : return define::COMPONENT_TYPE_NAME_BYTE;
        case ComponentType::Component_Unsigned_Byte : return define::COMPONENT_TYPE_NAME_UNSIGNED_BYTE;
        case ComponentType::Component_Short         : return define::COMPONENT_TYPE_NAME_SHORT;
        case ComponentType::Component_Unsigned_Short: return define::COMPONENT_TYPE_NAME_UNSIGNED_SHORT;
        case ComponentType::Component_Unsigned_Int  : return define::COMPONENT_TYPE_NAME_UNSIGNED_INT;
        case ComponentType::Component_Float         : return define::COMPONENT_TYPE_NAME_FLOAT;
        default:
            throw error::GLTFException("Unknown componentType" + std::to_string((int)componentType));
    }
}
uint8_t       GLTFAccessor::GetComponentTypeSize(ComponentType componentType)
{
    switch (componentType)
    {
        case ComponentType::Component_Byte:
        case ComponentType::Component_Unsigned_Byte: 
            return 1;
        case ComponentType::Component_Short:
        case ComponentType::Component_Unsigned_Short:
            return 2;
        case ComponentType::Component_Unsigned_Int:
        case ComponentType::Component_Float:
            return 4;
        default:
            throw error::GLTFException("Unknown componentType" + std::to_string((int)componentType));
    }
}
bool GLTFAccessor::operator==(const GLTFAccessor& rhs) const
{
    return GLTFChildOfRootProperty::Equals(*this, rhs)
        && this->BufferViewID      == rhs.BufferViewID
        && this->ByteOffset        == rhs.ByteOffset
        && this->ComponentDataType == rhs.ComponentDataType
        && this->IsNormalized      == rhs.IsNormalized
        && this->Count             == rhs.Count
        && this->AccessorDataType  == rhs.AccessorDataType
        && this->MaxValue          == rhs.MaxValue
        && this->MinValue          == rhs.MinValue
        && this->Sparse            == rhs.Sparse;
}
#pragma endregion GLTFAccessor
#pragma region GLTFCamera
bool GLTFOrthographic::IsEqual(const GLTFProjection& rhs) const
{
    if (const auto other = dynamic_cast<const GLTFOrthographic*>(&rhs))
    {
        return GLTFProjection::IsEqual(rhs)
            && XMagnification == other->XMagnification
            && YMagnification == other->YMagnification
            && ZFar == other->ZFar;
    }

    return false;
}

bool GLTFPerspective::IsEqual(const GLTFProjection& rhs) const
{
    if (const auto other = dynamic_cast<const GLTFPerspective*>(&rhs))
    {
        return GLTFProjection::IsEqual(rhs)
            && AspectRatio == other->AspectRatio
            && YFov == other->YFov
            && ZFar == other->ZFar;
    }

    return false;
}

GLTFCamera::GLTFCamera(std::unique_ptr<GLTFProjection> projection) : GLTFChildOfRootProperty(), Projection(std::move(projection))
{
    if (!(this->Projection))
    {
        throw error::GLTFException("Cannot create camera with null projection");
    }
}
GLTFCamera::GLTFCamera(std::string id, std::string name, std::unique_ptr<GLTFProjection> projection) :GLTFChildOfRootProperty(), Projection(std::move(projection))
{
    if (!(this->Projection))
    {
        throw error::GLTFException("Cannot create camera with null projection");
    }
}

const GLTFPerspective& GLTFCamera::GetPerspective() const
{
    if (const auto ret = dynamic_cast<GLTFPerspective*>(Projection.get()))
    {
        return *ret;
    }
    throw error::GLTFException("Failed to cast projection to orthographic");
}

const GLTFOrthographic& GLTFCamera::GetOrthographic() const
{
    if (const auto ret = dynamic_cast<GLTFOrthographic*>(Projection.get()))
    {
        return *ret;
    }
    throw error::GLTFException("Failed to cast projection to orthographic");
}

bool GLTFCamera::operator==(const GLTFCamera& rhs) const
{
    if (!GLTFChildOfRootProperty::Equals(*this, rhs))
    {
        return false;
    }
    if (!Projection || !rhs.Projection)
    {
        return !Projection && !rhs.Projection;
    }

    return *Projection == *(rhs.Projection);
}
#pragma endregion  GLTFCamera
#pragma region GLTFImage
bool GLTFImage::operator==(const GLTFImage& rhs) const 
{
    return GLTFChildOfRootProperty::Equals(*this, rhs)
        && this->URI          == rhs.URI
        && this->MimeType     == rhs.MimeType
        && this->BufferViewId == rhs.BufferViewId;

}
#pragma endregion   GLTFImage
#pragma region GLTFSampler
bool GLTFSampler::operator==(const GLTFSampler& rhs) const 
{
    return GLTFChildOfRootProperty::Equals(*this, rhs)
        && this->MagFilter == rhs.MagFilter
        && this->MinFilter == rhs.MinFilter
        && this->TextureAddressingU == rhs.TextureAddressingU
        && this->TextureAddressingV == rhs.TextureAddressingV;
}
MinFilterMode GLTFSampler::GetMinFilterMode       (size_t readValue)
{
    switch ((MinFilterMode)readValue)
    {
        case MinFilterMode::MinFilter_Nearest: return MinFilterMode::MinFilter_Nearest;
        case MinFilterMode::MinFilter_Linear : return MinFilterMode::MinFilter_Linear;
        case MinFilterMode::MinFilter_Nearest_Mipmap_Nearest: return MinFilterMode::MinFilter_Nearest_Mipmap_Nearest;
        case MinFilterMode::MinFilter_Nearest_Mipmap_Linear : return MinFilterMode::MinFilter_Nearest_Mipmap_Linear;
        case MinFilterMode::MinFilter_Linear_Mipmap_Nearset : return MinFilterMode::MinFilter_Linear_Mipmap_Nearset;
        case MinFilterMode::MinFilter_Linear_Mipmap_Linear  : return MinFilterMode::MinFilter_Linear_Mipmap_Linear;
        default:
            throw gltf::detail::error::InvalidGLTFException("Invalid sampler min filter value: " + std::to_string(readValue));
    }
}
MagFilterMode GLTFSampler::GetMagFilterMode(size_t readValue)
{
    switch ((MagFilterMode)readValue)
    {
        case MagFilterMode::MagFilter_Nearest: return MagFilterMode::MagFilter_Nearest;
        case MagFilterMode::MagFilter_Linear:  return MagFilterMode::MagFilter_Linear;
        default:
            throw detail::error::InvalidGLTFException("Invalid sampler mag filter value: " + std::to_string(readValue));
    }
}
WrapMode      GLTFSampler::GetAddressingMode(size_t readValue)
{
    switch ((WrapMode)readValue)
    {
        case WrapMode::Clamp: return WrapMode::Clamp;
        case WrapMode::Repeat: return WrapMode::Repeat;
        case WrapMode::Mirrored_Repeat: return WrapMode::Mirrored_Repeat;
        default:
            throw detail::error::InvalidGLTFException("Invalid sampler wrap value: " + std::to_string(readValue));
    }
}
#pragma endregion GLTFSampler
#pragma region GLTFMesh
#pragma endregion GLTFMesh
const std::string& GLTFMeshPrimitive::GetAttributeAccessorID(const std::string& name) const
{
    auto iterator = Attributes.find(name);
    if (iterator != Attributes.end()) { return iterator->second; }
    throw error::GLTFException("Mesh primitive has no attribute named" + name);
}
bool GLTFMeshPrimitive::TryGetAttributeAccessorID(const std::string& name, std::string& accessorID) const
{
    auto iterator = Attributes.find(name);
    if (iterator != Attributes.end())
    {
        accessorID = iterator->second;
        return true;
    }
    return false;
}
#pragma region GLTFSkin
bool GLTFSkin::operator==(const GLTFSkin& rhs)const 
{
    return GLTFChildOfRootProperty::Equals(*this, rhs)
        && this->InverseBindMatricesAccessorID == rhs.InverseBindMatricesAccessorID
        && this->SkeletonID == rhs.SkeletonID
        && this->JointIDs   == rhs.JointIDs;
}
#pragma endregion    GLTFSkin