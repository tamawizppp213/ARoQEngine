//////////////////////////////////////////////////////////////////////////////////
//              @file   GLTFDocument.cpp
///             @brief  GLTF Document
///             @author Partially edit by Toide
///             @date   2022_05_19
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/GLTF/Public/Include/GLTFDocument.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gltf;
using namespace gltf::detail::asset;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
GLTFDocument::GLTFDocument()
{

}
GLTFDocument::GLTFDocument(GLTFAsset && asset) : Asset(std::move(asset)) {};

bool GLTFDocument::operator==(const GLTFDocument& rhs) const
{
	return this->Asset              == rhs.Asset
        && this->Accessors          == rhs.Accessors
        && this->Animations         == rhs.Animations
        && this->Buffers            == rhs.Buffers
        && this->BufferViews        == rhs.BufferViews
        && this->Cameras            == rhs.Cameras
        && this->Images             == rhs.Images
        && this->Materials          == rhs.Materials
        && this->Meshes             == rhs.Meshes
        && this->Nodes              == rhs.Nodes
        && this->Samplers           == rhs.Samplers
        && this->Scenes             == rhs.Scenes
        && this->Skins              == rhs.Skins
        && this->Textures           == rhs.Textures
        && this->ExtensionsUsed     == rhs.ExtensionsUsed
        && this->ExtensionsRequired == rhs.ExtensionsRequired
        && this->DefaultSceneID     == rhs.DefaultSceneID
        && GLTFProperty::Equals(*this, rhs);
}

bool GLTFDocument::IsExtenisonUsed(const std::string& extension) const
{
    return ExtensionsUsed.find(extension) != ExtensionsUsed.end();
}
bool GLTFDocument::IsExtensionRequired(const std::string& extension) const
{
    return ExtensionsRequired.find(extension) != ExtensionsRequired.end();
}
bool GLTFDocument::HasDefaultScene() const
{
    return !DefaultSceneID.empty();
}

const detail::asset::GLTFScene& GLTFDocument::GetDefaultScene() const 
{
    if (HasDefaultScene())            { return Scenes[DefaultSceneID]; }
    if (!Scenes.Elements().empty()) { return Scenes.Elements().front(); }
    throw gltf::detail::error::DocumentException("Default scene" + DefaultSceneID + "was not found.");
}

const detail::asset::GLTFScene& GLTFDocument::SetDefaultScene(detail::asset::GLTFScene&& scene, AppendIdPolicy policy)
{
    const auto& defaultScene = Scenes.Append(std::move(scene), policy);
    DefaultSceneID = defaultScene.ID;
    return defaultScene;
}