//////////////////////////////////////////////////////////////////////////////////
///             @file   GLTFDocument.hpp
///             @brief  Document
///             @author Copyright (c) Microsoft Corporation. All rights reserved.Licensed under the MIT License.
///                     Partially edit by Toide
///             @date   2022_05_12  
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GLTF_DOCUMENT_HPP
#define GLTF_DOCUMENT_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFAsset.hpp"
#include <unordered_set>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gltf
{
	/****************************************************************************
	*				  			GLTFDocument
	*************************************************************************//**
	*  @class     GLTFDocument
	*  @brief     Document
	*****************************************************************************/
	class GLTFDocument : public gltf::detail::GLTFProperty
	{
	public:
		
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		bool IsExtenisonUsed    (const std::string& extenison) const;
		bool IsExtensionRequired(const std::string& extenison) const;
		bool HasDefaultScene() const;
		
		const detail::asset::GLTFScene& GetDefaultScene() const;
		const detail::asset::GLTFScene& SetDefaultScene(detail::asset::GLTFScene&& scene, AppendIdPolicy policy);
		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		detail::asset::GLTFAsset Asset;
		IndexedContainer<const detail::asset::GLTFAccessor>   Accessors;
		IndexedContainer<const detail::asset::GLTFAnimation>  Animations;
		IndexedContainer<const detail::asset::GLTFBuffer>     Buffers;
		IndexedContainer<const detail::asset::GLTFBufferView> BufferViews;
		IndexedContainer<const detail::asset::GLTFCamera>     Cameras;
		IndexedContainer<const detail::asset::GLTFImage>      Images;
		IndexedContainer<const detail::asset::GLTFMaterial>   Materials;
		IndexedContainer<const detail::asset::GLTFMesh>       Meshes;
		IndexedContainer<const detail::asset::GLTFNode>       Nodes;
		IndexedContainer<const detail::asset::GLTFSampler>    Samplers;
		IndexedContainer<const detail::asset::GLTFScene>      Scenes;
		IndexedContainer<const detail::asset::GLTFSkin>       Skins;
		IndexedContainer<const detail::asset::GLTFTexture>    Textures;

		std::unordered_set<std::string> ExtensionsUsed;
		std::unordered_set<std::string> ExtensionsRequired;
		std::string DefaultSceneID;
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GLTFDocument();
		GLTFDocument(detail::asset::GLTFAsset&& asset);
		bool operator==(const GLTFDocument& rhs) const;
		bool operator!=(const GLTFDocument& rhs) const { return !operator==(rhs); }
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Private Property
		*****************************************************************************/
	};

}
#endif