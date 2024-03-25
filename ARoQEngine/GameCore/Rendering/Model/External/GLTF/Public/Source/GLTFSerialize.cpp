//////////////////////////////////////////////////////////////////////////////////
//              @file   GLTFSerialize.cpp
///             @brief  GLTF Serialize
///             @author Partially edit by Toide
///             @date   2022_05_21
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/GLTF/Public/Include/GLTFSerialize.hpp"
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFProperty.hpp"
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFExtensionHandlers.hpp"
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFRapidJsonUtils.hpp"
#include "GameUtility/File/Include/Json.hpp"
#include <array>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gltf;
using namespace gltf::detail;
using namespace gltf::detail::asset;
using namespace gltf::detail::define;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
namespace
{
	std::string AccessorTypeToString(AccessorType type)
	{
		switch (type)
		{
			case AccessorType::Type_Scalar: return TYPE_NAME_SCALAR;
			case AccessorType::Type_Vec2  : return TYPE_NAME_VEC2;
			case AccessorType::Type_Vec3  : return TYPE_NAME_VEC3;
			case AccessorType::Type_Vec4  : return TYPE_NAME_VEC4;
			case AccessorType::Type_Mat2  : return TYPE_NAME_MAT2;
			case AccessorType::Type_Mat3  : return TYPE_NAME_MAT3;
			case AccessorType::Type_Mat4  : return TYPE_NAME_MAT4;
			default: return "";
		}
	}

	std::string AlphaModeToString(GLTFAlphaMode mode)
	{
		switch (mode)
		{
			case GLTFAlphaMode::Opaque: return ALPHAMODE_NAME_OPAQUE;
			case GLTFAlphaMode::Blend : return ALPHAMODE_NAME_BLEND;
			case GLTFAlphaMode::Mask  : return ALPHAMODE_NAME_MASK;
			default: return "";
		}
	}

	std::string TargetPathToString(TargetPath target)
	{
		switch (target)
		{
			case TargetPath::Translation: return TARGETPATH_NAME_TRANSLATION;
			case TargetPath::Scale      : return TARGETPATH_NAME_SCALE;
			case TargetPath::Rotation   : return TARGETPATH_NAME_ROTATION;
			case TargetPath::Weight     : return TARGETPATH_NAME_WEIGHTS;
			default: return "";
		}
	}
	std::string InterpolationTypeToString(InterpolationType type)
	{
		switch (type)
		{
			case InterpolationType::Interpolation_Linear     : return INTERPOLATIONTYPE_NAME_LINEAR;
			case InterpolationType::Interpolation_Step       : return INTERPOLATIONTYPE_NAME_STEP;
			case InterpolationType::Interpolation_CubicSpline: return INTERPOLATIONTYPE_NAME_CUBICSPLINE;
			default: return "";
		}
	}

	template<typename T>
	void SerializeIndexedContainer(const char* name, const IndexedContainer<const T>& indexedContainer, const GLTFDocument& gltfDocument, rapidjson::Document& jsonDocument, const ExtensionSerializer& extensionSerializer,
		rapidjson::Value(*fn)(const T&, const GLTFDocument&, rapidjson::Document&, const ExtensionSerializer&))
	{
		if (indexedContainer.Size() > 0)
		{
			rapidjson::Document::AllocatorType& a = jsonDocument.GetAllocator();
			rapidjson::Value containerItems(rapidjson::kArrayType);
			for (const auto& containerElement : indexedContainer.Elements())
			{
				rapidjson::Value value = fn(containerElement, gltfDocument, jsonDocument, extensionSerializer);
				containerItems.PushBack(value, a);
			}

			jsonDocument.AddMember(rapidjson::StringRef(name), containerItems, a);
		}
	}
	/****************************************************************************
	*                       SerializePropertyExtensions
	*************************************************************************//**
	*  @fn        void SerializePropertyExtensions(const GLTFDocument& document, const GLTFProperty& property, rapidjson::Value& propertyValue, rapidjson::Document::AllocatorType& allocatorType, const ExtensionSerializer& extensionSerializer)
	*  @brief     Serialize extension
	*  @param[in] GLTFDocument& document
	*  @param[in] GLTFProperty& property
	*  @param[in,out] rapidjson::Value& propertyValue, 
	*  @param[in,out] rapidjson::Document::AllocatorType& allocatorType,
	*  @param[in] ExtensionSerializer& extensionSerializer
	*  @return 　　void
	*****************************************************************************/
	void SerializePropertyExtensions(const GLTFDocument& document, const GLTFProperty& property, rapidjson::Value& propertyValue, rapidjson::Document::AllocatorType& allocatorType, const ExtensionSerializer& extensionSerializer)
	{
		auto registeredExtensions = property.GetExtensions();

		if (!property.Extensions.empty() || !registeredExtensions.empty())
		{
			rapidjson::Value& extensions = RapidJsonUtils::FindOrAddMember(propertyValue, "extensions", allocatorType);

			/*-------------------------------------------------------------------
			-                      Add registered extensions
			---------------------------------------------------------------------*/
			for (const auto& extension : registeredExtensions)
			{
				const auto extensionPair = extensionSerializer.Serialize(extension, property, document);

				if (property.HasUnregisteredExtension(extensionPair.Name))
				{
					throw error::GLTFException("Registered extension '" + extensionPair.Name + "' is also present as an unregistered extension.");
				}

				if (document.ExtensionsUsed.find(extensionPair.Name) == document.ExtensionsUsed.end())
				{
					throw error::GLTFException("Registered extension '" + extensionPair.Name + "' is not present in extensionsUsed");
				}

				const auto d = RapidJsonUtils::CreateDocumentFromString(extensionPair.Value);//TODO: validate the returned document against the extension schema!
				rapidjson::Value v(rapidjson::kObjectType);
				v.CopyFrom(d, allocatorType);
				extensions.AddMember(RapidJsonUtils::ToStringValue(extensionPair.Name, allocatorType), v, allocatorType);
			}

			/*-------------------------------------------------------------------
			-                      Add unregistered extensions
			---------------------------------------------------------------------*/
			for (const auto& extension : property.Extensions)
			{
				if (document.ExtensionsUsed.find(extension.first) == document.ExtensionsUsed.end())
				{
					throw error::GLTFException("Unregistered extension '" + extension.first + "' is not present in extensionsUsed");
				}

				const auto d = RapidJsonUtils::CreateDocumentFromString(extension.second);//TODO: validate the returned document against the extension schema!
				rapidjson::Value v(rapidjson::kObjectType);
				v.CopyFrom(d, allocatorType);
				extensions.AddMember(RapidJsonUtils::ToStringValue(extension.first, allocatorType), v, allocatorType);
			}
		}
	}
	/****************************************************************************
	*                       SerializeExtras
	*************************************************************************//**
	*  @fn        void SerializePropertyExtras(const GLTFProperty& property, rapidjson::Value& propertyValue, rapidjson::Document::AllocatorType& allocatorType)
	*  @brief     Serialize extra
	*  @param[in] GLTFProperty& property
	*  @param[in,out] rapidjson::Value& propertyValue,
	*  @param[in,out] rapidjson::Document::AllocatorType& allocatorType,
	*  @return 　　void
	*****************************************************************************/
	void SerializePropertyExtras(const GLTFProperty& property, rapidjson::Value& propertyValue, rapidjson::Document::AllocatorType& allocatorType)
	{
		if (!property.Extras.empty())
		{
			auto d = RapidJsonUtils::CreateDocumentFromString(property.Extras);
			rapidjson::Value v(rapidjson::kObjectType);
			v.CopyFrom(d, allocatorType);
			propertyValue.AddMember("extras", v, allocatorType);
		}
	}
	/****************************************************************************
	*                       SerializeProperty
	*************************************************************************//**
	*  @fn        void SerializeProperty(const GLTFDocument& document, const GLTFProperty& property, rapidjson::Value& propertyValue, rapidjson::Document::AllocatorType& allocatorType, const ExtensionSerializer& extensionSerializer)
	*  @brief     Serialize extra property
	*  @param[in] GLTFDocument& document
	*  @param[in] GLTFProperty& property
	*  @param[in,out] rapidjson::Value& propertyValue,
	*  @param[in,out] rapidjson::Document::AllocatorType& allocatorType,
	*  @param[in] ExtensionSerializer& extensionSerializer
	*  @return 　　void
	*****************************************************************************/
	void SerializeProperty(const GLTFDocument& document, const GLTFProperty& property, rapidjson::Value& propertyValue, rapidjson::Document::AllocatorType& allocatorType, const ExtensionSerializer& extensionSerializer)
	{
		SerializePropertyExtensions(document, property, propertyValue, allocatorType, extensionSerializer);
		SerializePropertyExtras(property, propertyValue, allocatorType);
	}
	/****************************************************************************
	*                       SerializeTextureInfo
	*************************************************************************//**
	*  @fn        void SerializeTextureInfo(const GLTFDocument& document, const GLTFTextureInfo& textureInfo, rapidjson::Value& textureValue, rapidjson::Document::AllocatorType& allocatorType, const IndexedContainer<const GLTFTexture>& textures, const ExtensionSerializer& extensionSerializer)
	*  @brief     Serialize Texture infomation
	*  @param[in] GLTFDocument& document
	*  @param[in] GLTFTextureInfo& textureInfomation
	*  @param[in,out] rapidjson::Value& propertyValue,
	*  @param[in,out] rapidjson::Document::AllocatorType& allocatorType,
	*  @param[in] IndexedContainer<const GLTFTexture>& textures
	*  @param[in] ExtensionSerializer& extensionSerializer
	*  @return 　　void
	*****************************************************************************/
	void SerializeTextureInfo(const GLTFDocument& document, const GLTFTextureInfo& textureInfo, rapidjson::Value& textureValue, rapidjson::Document::AllocatorType& allocatorType, const IndexedContainer<const GLTFTexture>& textures, const ExtensionSerializer& extensionSerializer)
	{
		RapidJsonUtils::AddOptionalMemberIndex("index", textureValue, textureInfo.TextureID, textures, allocatorType);
		if (textureInfo.TexCoord != 0)
		{
			textureValue.AddMember("texCoord", ToKnownSizeType(textureInfo.TexCoord), allocatorType);
		}
		SerializeProperty(document, textureInfo, textureValue, allocatorType, extensionSerializer);
	}
	/****************************************************************************
	*                       SerializeAsset
	*************************************************************************//**
	*  @fn        void SerializeAsset(const GLTFDocument& gltfDocument, rapidjson::Document& jsonDocument, const ExtensionSerializer& extensionSerializer)
	*  @brief     Serialize Asset
	*  @param[in] GLTFDocument& document
	*  @param[in,out] rapidjson::Document
	*  @param[in] ExtensionSerializer& extensionSerializer
	*  @return 　　void
	*****************************************************************************/
	void SerializeAsset(const GLTFDocument& gltfDocument, rapidjson::Document& jsonDocument, const ExtensionSerializer& extensionSerializer)
	{
		rapidjson::Document::AllocatorType& a = jsonDocument.GetAllocator();
		rapidjson::Value assetValue(rapidjson::kObjectType);

		RapidJsonUtils::AddOptionalMember("copyright" , assetValue, gltfDocument.Asset.CopyRight, a);
		RapidJsonUtils::AddOptionalMember("generator" , assetValue, gltfDocument.Asset.Generator, a);
		assetValue      .AddMember       ("version"   , RapidJsonUtils::ToStringValue(gltfDocument.Asset.Version, a), a);
		RapidJsonUtils::AddOptionalMember("minVersion", assetValue, gltfDocument.Asset.MinVersion, a);

		SerializeProperty(gltfDocument, gltfDocument.Asset, assetValue, a, extensionSerializer);

		jsonDocument.AddMember("asset", assetValue, a);
	}
	/****************************************************************************
	*                       SerializeDefaultScene
	*************************************************************************//**
	*  @fn        void SerializeDefaultScene(const GLTFDocument& gltfDocument, rapidjson::Document& jsonDocument, const ExtensionSerializer& extensionSerializer)
	*  @brief     Serialize DefaultScene
	*  @param[in] GLTFDocument& document
	*  @param[in,out] rapidjson::Document
	*  @return 　　void
	*****************************************************************************/
	void SerializeDefaultScene(const GLTFDocument& gltfDocument, rapidjson::Document& jsonDocument)
	{
		rapidjson::Document::AllocatorType& a = jsonDocument.GetAllocator();

		if (gltfDocument.HasDefaultScene())
		{
			jsonDocument.AddMember("scene", ToKnownSizeType(gltfDocument.Scenes.GetIndex(gltfDocument.DefaultSceneID)), a);
		}
	}
	/****************************************************************************
	*                       SerializeAccessor
	*************************************************************************//**
	*  @fn        rapidjson::Value SerializeAccessor(const GLTFAccessor& accessor, const GLTFDocument& gltfDocument, rapidjson::Document& jsonDocument, const ExtensionSerializer& extensionSerializer)
	*  @brief     Serialize Accessor
	*  @param[in] GLTFAccessor
	*  @param[in] GLTFDocument& document
	*  @param[in,out] rapidjson::Document
	*  @param[in] ExtensionSerializer& extensionSerializer
	*  @return 　　rapidjson::Value
	*****************************************************************************/
	rapidjson::Value SerializeAccessor(const GLTFAccessor& accessor, const GLTFDocument& gltfDocument, rapidjson::Document& jsonDocument, const ExtensionSerializer& extensionSerializer)
	{
		rapidjson::Document::AllocatorType& a = jsonDocument.GetAllocator();

		rapidjson::Value accessorValue;
		accessorValue.SetObject();

		RapidJsonUtils::AddOptionalMember("name", accessorValue, accessor.Name, a);

		/*-------------------------------------------------------------------
		-                      Sparse
		---------------------------------------------------------------------*/
		if (accessor.Sparse.Count > 0U)
		{
			if (!accessor.BufferViewID.empty())
			{
				accessorValue.AddMember("bufferView", ToKnownSizeType(gltfDocument.BufferViews.GetIndex(accessor.BufferViewID)), a);
			}

			rapidjson::Value sparseRoot(rapidjson::kObjectType);
			rapidjson::Value indices(rapidjson::kObjectType);
			rapidjson::Value values(rapidjson::kObjectType);

			indices.AddMember("bufferView", ToKnownSizeType(gltfDocument.BufferViews.GetIndex(accessor.Sparse.IndicesBufferViewID)), a);
			if (accessor.Sparse.IndicesByteOffset != 0U)
			{
				indices.AddMember("byteOffset", ToKnownSizeType(accessor.Sparse.IndicesByteOffset), a);
			}
			indices.AddMember("componentType", accessor.Sparse.IndicesComponentType, a);

			values.AddMember("bufferView", ToKnownSizeType(gltfDocument.BufferViews.GetIndex(accessor.Sparse.ValuesBufferViewID)), a);
			if (accessor.Sparse.ValuesByteOffset != 0U)
			{
				values.AddMember("byteOffset", ToKnownSizeType(accessor.Sparse.ValuesByteOffset), a);
			}

			sparseRoot.AddMember("count", ToKnownSizeType(accessor.Sparse.Count), a);
			sparseRoot.AddMember("indices", indices, a);
			sparseRoot.AddMember("values", values, a);

			accessorValue.AddMember("sparse", sparseRoot, a);
		}
		else
		{
			RapidJsonUtils::AddOptionalMemberIndex("bufferView", accessorValue, accessor.BufferViewID, gltfDocument.BufferViews, a);
		}
		/*-------------------------------------------------------------------
		-                      ByteOffset
		---------------------------------------------------------------------*/
		if (accessor.ByteOffset != 0U)
		{
			accessorValue.AddMember("byteOffset", ToKnownSizeType(accessor.ByteOffset), a);
		}

		/*-------------------------------------------------------------------
		-                      Normalized
		---------------------------------------------------------------------*/
		if (accessor.IsNormalized)
		{
			accessorValue.AddMember("normalized", accessor.IsNormalized, a);
		}

		accessorValue.AddMember("componentType", accessor.ComponentDataType, a);
		accessorValue.AddMember("count", ToKnownSizeType(accessor.Count), a);
		accessorValue.AddMember("type", RapidJsonUtils::ToStringValue(AccessorTypeToString(accessor.AccessorDataType), a), a);

		rapidjson::Value max(rapidjson::kArrayType);
		rapidjson::Value min(rapidjson::kArrayType);
		/*-------------------------------------------------------------------
		-                    Max and Min
		---------------------------------------------------------------------*/
		if (!accessor.MaxValue.empty())
		{
			accessorValue.AddMember("max", RapidJsonUtils::ToJsonArray(accessor.MaxValue, a), a);
		}

		if (!accessor.MinValue.empty())
		{
			accessorValue.AddMember("min", RapidJsonUtils::ToJsonArray(accessor.MinValue, a), a);
		}

		SerializeProperty(gltfDocument, accessor, accessorValue, a, extensionSerializer);

		return accessorValue;
	}
	/****************************************************************************
	*                       SerializeAnimation
	*************************************************************************//**
	*  @fn        rapidjson::Value SerializeAnimation(const GLTFAnimation& accessor, const GLTFDocument& gltfDocument, rapidjson::Document& jsonDocument, const ExtensionSerializer& extensionSerializer)
	*  @brief     Serialize Animation
	*  @param[in] GLTFAnimation
	*  @param[in] GLTFDocument& document
	*  @param[in,out] rapidjson::Document
	*  @param[in] ExtensionSerializer& extensionSerializer
	*  @return 　　rapidjson::Value
	*****************************************************************************/
	rapidjson::Value SerializeAnimation(const GLTFAnimation& animation, const GLTFDocument& gltfDocument, rapidjson::Document& jsonDocument, const ExtensionSerializer& extensionSerializer)
	{
		rapidjson::Document::AllocatorType& a = jsonDocument.GetAllocator();

		rapidjson::Value animationValue(rapidjson::kObjectType);
		rapidjson::Value channelValues(rapidjson::kArrayType);
		rapidjson::Value samplerValues(rapidjson::kArrayType);
		/*-------------------------------------------------------------------
		-                        Animation Channel
		---------------------------------------------------------------------*/
		for (const auto& channel : animation.Channels.Elements())
		{
			rapidjson::Value channelValue(rapidjson::kObjectType);
			rapidjson::Value targetValue(rapidjson::kObjectType);

			RapidJsonUtils::AddOptionalMemberIndex("node", targetValue, channel.Target.NodeID, gltfDocument.Nodes, a);
			targetValue.AddMember("path", RapidJsonUtils::ToStringValue(TargetPathToString(channel.Target.Path), a), a);

			channelValue.AddMember("sampler", ToKnownSizeType(animation.Samplers.GetIndex(channel.SampleID)), a);
			channelValue.AddMember("target", targetValue, a);

			SerializeProperty(gltfDocument, channel, channelValue, a, extensionSerializer);

			channelValues.PushBack(channelValue, a);
		}

		/*-------------------------------------------------------------------
		-                        Animation Sampler
		---------------------------------------------------------------------*/
		for (const auto& sampler : animation.Samplers.Elements())
		{
			rapidjson::Value samplerValue(rapidjson::kObjectType);
			samplerValue.AddMember("input", ToKnownSizeType(gltfDocument.Accessors.GetIndex(sampler.InputAccessorID)), a);
			RapidJsonUtils::AddOptionalMember("interpolation", samplerValue, InterpolationTypeToString(sampler.Interpolation), a);
			samplerValue.AddMember("output", ToKnownSizeType(gltfDocument.Accessors.GetIndex(sampler.OutputAccessorID)), a);

			SerializeProperty(gltfDocument, sampler, samplerValue, a, extensionSerializer);

			samplerValues.PushBack(samplerValue, a);
		}

		animationValue.AddMember("channels", channelValues, a);
		animationValue.AddMember("samplers", samplerValues, a);
		RapidJsonUtils::AddOptionalMember("name", animationValue, animation.Name, a);

		SerializeProperty(gltfDocument, animation, animationValue, a, extensionSerializer);

		return animationValue;
	}
	/****************************************************************************
	*                       SerializeBufferView
	*************************************************************************//**
	*  @fn        rapidjson::Value SerializeBufferView(const GLTFBufferView& bufferView, const GLTFDocument& gltfDocument, rapidjson::Document& jsonDocument, const ExtensionSerializer& extensionSerializer)
	*  @brief     Serialize Buffer View
	*  @param[in] GLTFBufferView
	*  @param[in] GLTFDocument& document
	*  @param[in,out] rapidjson::Document
	*  @param[in] ExtensionSerializer& extensionSerializer
	*  @return 　　rapidjson::Value
	*****************************************************************************/
	rapidjson::Value SerializeBufferView(const GLTFBufferView& bufferView, const GLTFDocument& gltfDocument, rapidjson::Document& jsonDocument, const ExtensionSerializer& extensionSerializer)
	{
		rapidjson::Document::AllocatorType& a = jsonDocument.GetAllocator();

		rapidjson::Value bufferViewValue;
		bufferViewValue.SetObject();

		RapidJsonUtils::AddOptionalMember("name", bufferViewValue, bufferView.Name, a);
		bufferViewValue.AddMember("buffer", ToKnownSizeType(gltfDocument.Buffers.GetIndex(bufferView.BufferID)), a);
		bufferViewValue.AddMember("byteOffset", ToKnownSizeType(bufferView.ByteOffset), a);
		bufferViewValue.AddMember("byteLength", ToKnownSizeType(bufferView.ByteLength), a);

		if (bufferView.ByteStride)
		{
			bufferViewValue.AddMember("byteStride", ToKnownSizeType(bufferView.ByteStride.Get()), a);
		}

		if (bufferView.Target)
		{
			bufferViewValue.AddMember("target", bufferView.Target.Get(), a);
		}

		SerializeProperty(gltfDocument, bufferView, bufferViewValue, a, extensionSerializer);

		return bufferViewValue;
	}
	/****************************************************************************
	*                       SerializeBuffer
	*************************************************************************//**
	*  @fn        rapidjson::Value SerializeBuffer(const GLTFBuffer& buffer, const GLTFDocument& gltfDocument, rapidjson::Document& jsonDocument, const ExtensionSerializer& extensionSerializer)
	*  @brief     Serialize Buffer View
	*  @param[in] GLTFBuffer
	*  @param[in] GLTFDocument& document
	*  @param[in,out] rapidjson::Document
	*  @param[in] ExtensionSerializer& extensionSerializer
	*  @return 　　rapidjson::Value
	*****************************************************************************/
	rapidjson::Value SerializeBuffer(const GLTFBuffer& buffer, const GLTFDocument& gltfDocument, rapidjson::Document& jsonDocument, const ExtensionSerializer& extensionSerializer)
	{
		rapidjson::Document::AllocatorType& a = jsonDocument.GetAllocator();
		rapidjson::Value bufferValue(rapidjson::kObjectType);

		bufferValue.AddMember("byteLength", ToKnownSizeType(buffer.ByteLength), a);
		RapidJsonUtils::AddOptionalMember("uri", bufferValue, buffer.URI, a);

		SerializeProperty(gltfDocument, buffer, bufferValue, a, extensionSerializer);

		return bufferValue;
	}
	/****************************************************************************
	*                       SerializeImage
	*************************************************************************//**
	*  @fn        rapidjson::Value SerializeImage(const GLTFImage& image, const GLTFDocument& gltfDocument, rapidjson::Document& jsonDocument, const ExtensionSerializer& extensionSerializer)
	*  @brief     Serialize Image
	*  @param[in] GLTFImage
	*  @param[in] GLTFDocument& document
	*  @param[in,out] rapidjson::Document
	*  @param[in] ExtensionSerializer& extensionSerializer
	*  @return 　　rapidjson::Value
	*****************************************************************************/
	rapidjson::Value SerializeImage(const GLTFImage& image, const GLTFDocument& gltfDocument, rapidjson::Document& jsonDocument, const ExtensionSerializer& extensionSerializer)
	{
		if (image.URI.empty())
		{
			if (image.BufferViewId.empty() || image.MimeType.empty())
			{
				throw error::InvalidGLTFException("Invalid image: " + image.ID + ". Images must have either a uri or a bufferView and a mimeType.");
			}
		}
		else if (!image.BufferViewId.empty())
		{
			throw error::InvalidGLTFException("Invalid image: " + image.ID + ". Images can only have a uri or a bufferView, but not both.");
		}

		rapidjson::Document::AllocatorType& a = jsonDocument.GetAllocator();
		rapidjson::Value imageValue(rapidjson::kObjectType);

		RapidJsonUtils::AddOptionalMember("name", imageValue, image.Name, a);
		RapidJsonUtils::AddOptionalMember("uri", imageValue, image.URI, a);
		RapidJsonUtils::AddOptionalMemberIndex("bufferView", imageValue, image.BufferViewId, gltfDocument.BufferViews, a);
		RapidJsonUtils::AddOptionalMember("mimeType", imageValue, image.MimeType, a);

		SerializeProperty(gltfDocument, image, imageValue, a, extensionSerializer);

		return imageValue;
	}
	/****************************************************************************
	*                       SerializeMaterial
	*************************************************************************//**
	*  @fn        rapidjson::Value SerializeMaterial(const GLTFMaterial& material, const GLTFDocument& gltfDocument, rapidjson::Document& jsonDocument, const ExtensionSerializer& extensionSerializer)
	*  @brief     Serialize Material
	*  @param[in] GLTFMaterial
	*  @param[in] GLTFDocument& document
	*  @param[in,out] rapidjson::Document
	*  @param[in] ExtensionSerializer& extensionSerializer
	*  @return 　　rapidjson::Value
	*****************************************************************************/
	rapidjson::Value SerializeMaterial(const GLTFMaterial& material, const GLTFDocument& gltfDocument, rapidjson::Document& jsonDocument, const ExtensionSerializer& extensionSerializer)
	{
		/*-------------------------------------------------------------------
		-                     Metallic and Roughness
		---------------------------------------------------------------------*/
		rapidjson::Document::AllocatorType& a = jsonDocument.GetAllocator();
		rapidjson::Value materialValue(rapidjson::kObjectType);
		{
			rapidjson::Value pbrMetallicRoughness(rapidjson::kObjectType);
			{
				if (material.MetallicRoughness.BaseColorFactor != Color4(1.0f, 1.0f, 1.0f, 1.0f))
				{
					pbrMetallicRoughness.AddMember("baseColorFactor", RapidJsonUtils::ToJsonArray(material.MetallicRoughness.BaseColorFactor, a), a);
				}

				if (!material.MetallicRoughness.BaseColorTexture.TextureID.empty())
				{
					rapidjson::Value baseColorTexture(rapidjson::kObjectType);
					SerializeTextureInfo(gltfDocument, material.MetallicRoughness.BaseColorTexture, baseColorTexture, a, gltfDocument.Textures, extensionSerializer);
					pbrMetallicRoughness.AddMember("baseColorTexture", baseColorTexture, a);
				}

				if (material.MetallicRoughness.MetallicFactor != 1.0f)
				{
					pbrMetallicRoughness.AddMember("metallicFactor", material.MetallicRoughness.MetallicFactor, a);
				}

				if (material.MetallicRoughness.RoughnessFactor != 1.0f)
				{
					pbrMetallicRoughness.AddMember("roughnessFactor", material.MetallicRoughness.RoughnessFactor, a);
				}

				if (!material.MetallicRoughness.MetallicRoughnessTexture.TextureID.empty())
				{
					rapidjson::Value metallicRoughnessTexture(rapidjson::kObjectType);
					SerializeTextureInfo(gltfDocument, material.MetallicRoughness.MetallicRoughnessTexture, metallicRoughnessTexture, a, gltfDocument.Textures, extensionSerializer);
					pbrMetallicRoughness.AddMember("metallicRoughnessTexture", metallicRoughnessTexture, a);
				}
			}

			materialValue.AddMember("pbrMetallicRoughness", pbrMetallicRoughness, a);

			/*-------------------------------------------------------------------
			-                    Normal
			---------------------------------------------------------------------*/
			if (!material.NormalTexture.TextureID.empty())
			{
				rapidjson::Value normalTexture(rapidjson::kObjectType);
				SerializeTextureInfo(gltfDocument, material.NormalTexture, normalTexture, a, gltfDocument.Textures, extensionSerializer);
				if (material.NormalTexture.Scale != 1.0f)
				{
					normalTexture.AddMember("scale", material.NormalTexture.Scale, a);
				}
				materialValue.AddMember("normalTexture", normalTexture, a);
			}

			/*-------------------------------------------------------------------
			-                    Occlusion
			---------------------------------------------------------------------*/
			if (!material.OcclusionTexture.TextureID.empty())
			{
				rapidjson::Value occlusionTexture(rapidjson::kObjectType);
				SerializeTextureInfo(gltfDocument, material.OcclusionTexture, occlusionTexture, a, gltfDocument.Textures, extensionSerializer);
				if (material.OcclusionTexture.Strength != 1.0f)
				{
					occlusionTexture.AddMember("strength", material.OcclusionTexture.Strength, a);
				}
				materialValue.AddMember("occlusionTexture", occlusionTexture, a);
			}

			/*-------------------------------------------------------------------
			-                    Emissive Texture
			---------------------------------------------------------------------*/
			if (!material.EmissiveTexture.TextureID.empty())
			{
				rapidjson::Value emissiveTexture(rapidjson::kObjectType);
				SerializeTextureInfo(gltfDocument, material.EmissiveTexture, emissiveTexture, a, gltfDocument.Textures, extensionSerializer);
				materialValue.AddMember("emissiveTexture", emissiveTexture, a);
			}

			/*-------------------------------------------------------------------
			-                    Emissive Factor
			---------------------------------------------------------------------*/
			if (material.EmissiveFactor != Color3(0.0f, 0.0f, 0.0f))
			{
				materialValue.AddMember("emissiveFactor", RapidJsonUtils::ToJsonArray(material.EmissiveFactor, a), a);
			}

			/*-------------------------------------------------------------------
			-                    Alpha Mode : do not serialize default value (opaque) or currently-unsupported value (unknown)
			---------------------------------------------------------------------*/
			if (material.AlphaMode != GLTFAlphaMode::Opaque && material.AlphaMode != GLTFAlphaMode::Alpha_Unknown)
			{
				materialValue.AddMember("alphaMode", RapidJsonUtils::ToStringValue(AlphaModeToString(material.AlphaMode), a), a);
			}

			/*-------------------------------------------------------------------
			-                   Alpha Cutoff
			---------------------------------------------------------------------*/
			if (material.AlphaCutoff != 0.5f)
			{
				materialValue.AddMember("alphaCutoff", material.AlphaCutoff, a);
			}

			/*-------------------------------------------------------------------
			-                   Name
			---------------------------------------------------------------------*/
			RapidJsonUtils::AddOptionalMember("name", materialValue, material.Name, a);

			/*-------------------------------------------------------------------
			-                   Double Sided
			---------------------------------------------------------------------*/
			if (material.DoubleSided)
			{
				materialValue.AddMember("doubleSided", material.DoubleSided, a);
			}

			SerializeProperty(gltfDocument, material, materialValue, a, extensionSerializer);
		}

		return materialValue;
	}
	/****************************************************************************
	*                       SerializeTarget
	*************************************************************************//**
	*  @fn        rapidjson::Value SerializeTarget(const GLTFMorphTarget& target, const GLTFDocument& gltfDocument, rapidjson::Document& jsonDocument, const ExtensionSerializer& extensionSerializer)
	*  @brief     Serialize Target
	*  @param[in] GLTFMorphTarget
	*  @param[in] GLTFDocument& document
	*  @param[in,out] rapidjson::Document
	*  @param[in] ExtensionSerializer& extensionSerializer
	*  @return 　　rapidjson::Value
	*****************************************************************************/
	rapidjson::Value SerializeTarget(const GLTFMorphTarget& target, const GLTFDocument& gltfDocument, rapidjson::Document& jsonDocument)
	{
		rapidjson::Value targetValue(rapidjson::kObjectType);
		rapidjson::Document::AllocatorType& a = jsonDocument.GetAllocator();

		RapidJsonUtils::AddOptionalMemberIndex(ACCESSOR_POSITION, targetValue, target.PositionsAccessorID, gltfDocument.Accessors, a);
		RapidJsonUtils::AddOptionalMemberIndex(ACCESSOR_NORMAL, targetValue, target.NormalsAccessorID, gltfDocument.Accessors, a);
		RapidJsonUtils::AddOptionalMemberIndex(ACCESSOR_TANGENT, targetValue, target.TangentsAccessorID, gltfDocument.Accessors, a);

		return targetValue;
	}
	/****************************************************************************
	*                       SerializeMaterial
	*************************************************************************//**
	*  @fn        rapidjson::Value SerializeMaterial(const GLTFMeshPrimitive primitive, const GLTFDocument& gltfDocument, rapidjson::Document& jsonDocument, const ExtensionSerializer& extensionSerializer)
	*  @brief     Serialize Material
	*  @param[in] GLTFMeshPrimitve
	*  @param[in,out] rapidjson::Value& primitiveValue
	*  @param[in] GLTFDocument& document
	*  @param[in,out] rapidjson::Document
	*  @return 　　void
	*****************************************************************************/
	void SerializeTargets(const GLTFMeshPrimitive& primitive, rapidjson::Value& primitiveValue, const GLTFDocument& gltfDocument, rapidjson::Document& jsonDocument)
	{
		if (!primitive.Targets.empty())
		{
			rapidjson::Document::AllocatorType& a = jsonDocument.GetAllocator();
			rapidjson::Value targets(rapidjson::kArrayType);
			for (const auto& morphTarget : primitive.Targets)
			{
				rapidjson::Value targetValue = SerializeTarget(morphTarget, gltfDocument, jsonDocument);
				targets.PushBack(targetValue, a);
			}
			primitiveValue.AddMember("targets", targets, a);
		}
	}
	/****************************************************************************
	*                       SerializeMesh
	*************************************************************************//**
	*  @fn        rapidjson::Value SerializeMesh(const GLTFMesh& mesh, const GLTFDocument& gltfDocument, rapidjson::Document& jsonDocument, const ExtensionSerializer& extensionSerializer)
	*  @brief     Serialize Mesh
	*  @param[in] GLTFMesh
	*  @param[in] GLTFDocument& document
	*  @param[in,out] rapidjson::Document
	*  @param[in] ExtensionSerializer& extensionSerializer
	*  @return 　　rapidjson::Value
	*****************************************************************************/
	rapidjson::Value SerializeMesh(const GLTFMesh& mesh, const GLTFDocument& gltfDocument, rapidjson::Document& jsonDocument, const ExtensionSerializer& extensionSerializer)
	{
		rapidjson::Document::AllocatorType& a = jsonDocument.GetAllocator();

		rapidjson::Value meshValue(rapidjson::kObjectType);
		rapidjson::Value primitiveValues(rapidjson::kArrayType);

		for (const auto& primitive : mesh.Primitives)
		{
			rapidjson::Value attributes(rapidjson::kObjectType);

			for (const auto& attribute : primitive.Attributes)
			{
				attributes.AddMember(RapidJsonUtils::ToStringValue(attribute.first, a), rapidjson::Value(ToKnownSizeType(gltfDocument.Accessors.GetIndex(attribute.second))), a);
			}

			rapidjson::Value primitiveValue(rapidjson::kObjectType);

			primitiveValue.AddMember("attributes", attributes, a);
			RapidJsonUtils::AddOptionalMemberIndex("indices", primitiveValue, primitive.IndicesAccessorID, gltfDocument.Accessors, a);
			RapidJsonUtils::AddOptionalMemberIndex("material", primitiveValue, primitive.MaterialID, gltfDocument.Materials, a);

			if (primitive.MeshMode != GLTFMeshMode::Triangles)
			{
				primitiveValue.AddMember("mode", primitive.MeshMode, a);
			}

			SerializeTargets(primitive, primitiveValue, gltfDocument, jsonDocument);

			SerializeProperty(gltfDocument, primitive, primitiveValue, a, extensionSerializer);

			primitiveValues.PushBack(primitiveValue, a);
		}

		RapidJsonUtils::AddArrayMember(meshValue, "weights", mesh.Weights, a);
		RapidJsonUtils::AddOptionalMember("name", meshValue, mesh.Name, a);
		meshValue.AddMember("primitives", primitiveValues, a);

		SerializeProperty(gltfDocument, mesh, meshValue, a, extensionSerializer);

		return meshValue;
	}
	/****************************************************************************
	*                       SerializeNode
	*************************************************************************//**
	*  @fn        rapidjson::Value SerializeNode(const GLTFNode& node, const GLTFDocument& gltfDocument, rapidjson::Document& jsonDocument, const ExtensionSerializer& extensionSerializer)
	*  @brief     Serialize Node
	*  @param[in] GLTFNode
	*  @param[in] GLTFDocument& document
	*  @param[in,out] rapidjson::Document
	*  @param[in] ExtensionSerializer& extensionSerializer
	*  @return 　　rapidjson::Value
	*****************************************************************************/
	rapidjson::Value SerializeNode(const GLTFNode& node, const GLTFDocument& gltfDocument, rapidjson::Document& jsonDocument, const ExtensionSerializer& extensionSerializer)
	{
		rapidjson::Document::AllocatorType& a = jsonDocument.GetAllocator();

		rapidjson::Value nodeValue(rapidjson::kObjectType);

		if (!node.Children.empty())
		{
			rapidjson::Value nodeChildren(rapidjson::kArrayType);
			{
				for (const auto& childId : node.Children)
				{
					nodeChildren.PushBack(ToKnownSizeType(gltfDocument.Nodes.GetIndex(childId)), a);
				}
			}
			nodeValue.AddMember("children", nodeChildren, a);
		}

		if (!node.HasValidTransformType())
		{
			throw error::DocumentException("Node " + node.ID + " doesn't have a valid transform type");
		}

		if (node.GetTransformationType() == TransformationType::Matrix)
		{
			std::array<float, 16> matrix = {};
			for (int i = 0; i < 16; ++i)
			{
				matrix[i] = node.Matrix(i / 4, i % 4);
			}
			nodeValue.AddMember("matrix", RapidJsonUtils::ToJsonArray<float, 16>(matrix, a), a);
		}
		else if (node.GetTransformationType() == TransformationType::TRS)
		{
			if (node.Translation != gm::Float3(0,0,0))
			{
				nodeValue.AddMember("translation", RapidJsonUtils::ToJsonArray(node.Translation, a), a);
			}
			if (node.Rotation != gm::Float4(0,0,0,1))
			{
				nodeValue.AddMember("rotation", RapidJsonUtils::ToJsonArray(node.Rotation, a), a);
			}
			if (node.Scale != gm::Float3(1, 1, 1))
			{
				nodeValue.AddMember("scale", RapidJsonUtils::ToJsonArray(node.Scale, a), a);
			}
		}

		RapidJsonUtils::AddOptionalMemberIndex("mesh", nodeValue, node.MeshID, gltfDocument.Meshes, a);
		RapidJsonUtils::AddOptionalMemberIndex("skin", nodeValue, node.SkinID, gltfDocument.Skins, a);

		if (!node.CameraID.empty())
		{
			nodeValue.AddMember("camera", ToKnownSizeType(gltfDocument.Cameras.GetIndex(node.CameraID)), a);
		}

		RapidJsonUtils::AddArrayMember(nodeValue, "weights", node.Weights, a);

		RapidJsonUtils::AddOptionalMember("name", nodeValue, node.Name, a);

		SerializeProperty(gltfDocument, node, nodeValue, a, extensionSerializer);

		return nodeValue;
	}
	/****************************************************************************
	*                       SerializeCamera
	*************************************************************************//**
	*  @fn        rapidjson::Value SerializeCamera(const GLTFCamera& camera, const GLTFDocument& gltfDocument, rapidjson::Document& jsonDocument, const ExtensionSerializer& extensionSerializer)
	*  @brief     Serialize Camera
	*  @param[in] GLTFCamera
	*  @param[in] GLTFDocument& document
	*  @param[in,out] rapidjson::Document
	*  @param[in] ExtensionSerializer& extensionSerializer
	*  @return 　　rapidjson::Value
	*****************************************************************************/
	rapidjson::Value SerializeCamera(const GLTFCamera& camera, const GLTFDocument& gltfDocument, rapidjson::Document& document, const ExtensionSerializer& extensionSerializer)
	{
		rapidjson::Document::AllocatorType& a = document.GetAllocator();
		rapidjson::Value cameraValue(rapidjson::kObjectType);
		rapidjson::Value projectionValue(rapidjson::kObjectType);

		const ProjectionType projectionType = camera.Projection->GetProjectionType();
		/*-------------------------------------------------------------------
		-                       Perspective
		---------------------------------------------------------------------*/
		if (projectionType == ProjectionType::Projection_Perspective)
		{
			const auto& perspective = camera.GetPerspective();

			projectionValue.AddMember("znear", RapidJsonUtils::ToFloatValue(perspective.ZNear), a);
			projectionValue.AddMember("yfov", RapidJsonUtils::ToFloatValue(perspective.YFov), a);

			if (perspective.ZFar)
			{
				projectionValue.AddMember("zfar", RapidJsonUtils::ToFloatValue(perspective.ZFar.Get()), a);
			}

			if (perspective.AspectRatio)
			{
				projectionValue.AddMember("aspectRatio", RapidJsonUtils::ToFloatValue(perspective.AspectRatio.Get()), a);
			}

			SerializeProperty(gltfDocument, perspective, projectionValue, a, extensionSerializer);

			cameraValue.AddMember("perspective", projectionValue, a);
			cameraValue.AddMember("type", RapidJsonUtils::ToStringValue("perspective", a), a);
		}
		/*-------------------------------------------------------------------
		-                       Orthographic
		---------------------------------------------------------------------*/
		else if (projectionType == ProjectionType::Projection_Orthographic)
		{
			const auto& orthographic = camera.GetOrthographic();

			projectionValue.AddMember("xmag", RapidJsonUtils::ToFloatValue(orthographic.XMagnification), a);
			projectionValue.AddMember("ymag", RapidJsonUtils::ToFloatValue(orthographic.YMagnification), a);
			projectionValue.AddMember("znear", RapidJsonUtils::ToFloatValue(orthographic.ZNear), a);
			projectionValue.AddMember("zfar", RapidJsonUtils::ToFloatValue(orthographic.ZFar), a);

			SerializeProperty(gltfDocument, orthographic, projectionValue, a, extensionSerializer);

			cameraValue.AddMember("orthographic", projectionValue, a);
			cameraValue.AddMember("type", RapidJsonUtils::ToStringValue("orthographic", a), a);
		}
		else
		{
			throw error::DocumentException("Camera " + camera.ID + " doesn't have a valid projection type");
		}

		SerializeProperty(gltfDocument, camera, cameraValue, a, extensionSerializer);

		RapidJsonUtils::AddOptionalMember("name", cameraValue, camera.Name, a);

		return cameraValue;
	}
	/****************************************************************************
	*                       SerializeSampler
	*************************************************************************//**
	*  @fn        rapidjson::Value SerializeSampler(const GLTFSampler& sampler, const GLTFDocument& gltfDocument, rapidjson::Document& jsonDocument, const ExtensionSerializer& extensionSerializer)
	*  @brief     Serialize Sampler
	*  @param[in] GLTFSampler
	*  @param[in] GLTFDocument& document
	*  @param[in,out] rapidjson::Document
	*  @param[in] ExtensionSerializer& extensionSerializer
	*  @return 　　rapidjson::Value
	*****************************************************************************/
	rapidjson::Value SerializeSampler(const GLTFSampler& sampler, const GLTFDocument& gltfDocument, rapidjson::Document& document, const ExtensionSerializer& extensionSerializer)
	{
		rapidjson::Document::AllocatorType& a = document.GetAllocator();
		rapidjson::Value samplerValue(rapidjson::kObjectType);

		{
			RapidJsonUtils::AddOptionalMember("name", samplerValue, sampler.Name, a);

			if (sampler.MagFilter)
			{
				samplerValue.AddMember("magFilter", sampler.MagFilter.Get(), a);
			}

			if (sampler.MinFilter)
			{
				samplerValue.AddMember("minFilter", sampler.MinFilter.Get(), a);
			}

			if (sampler.TextureAddressingU != WrapMode::Repeat)
			{
				samplerValue.AddMember("wrapS", sampler.TextureAddressingU, a);
			}

			if (sampler.TextureAddressingV != WrapMode::Repeat)
			{
				samplerValue.AddMember("wrapT", sampler.TextureAddressingV, a);
			}

			SerializeProperty(gltfDocument, sampler, samplerValue, a, extensionSerializer);
		}

		return samplerValue;
	}
	/****************************************************************************
	*                       SerializeScene
	*************************************************************************//**
	*  @fn        rapidjson::Value SerializeScene(const GLTFScene& scene, const GLTFDocument& gltfDocument, rapidjson::Document& jsonDocument, const ExtensionSerializer& extensionSerializer)
	*  @brief     Serialize Scene
	*  @param[in] GLTFScene
	*  @param[in] GLTFDocument& document
	*  @param[in,out] rapidjson::Document
	*  @param[in] ExtensionSerializer& extensionSerializer
	*  @return 　　rapidjson::Value
	*****************************************************************************/
	rapidjson::Value SerializeScene(const GLTFScene& scene, const GLTFDocument& gltfDocument, rapidjson::Document& jsonDocument, const ExtensionSerializer& extensionSerializer)
	{
		rapidjson::Document::AllocatorType& a = jsonDocument.GetAllocator();
		rapidjson::Value sceneValue(rapidjson::kObjectType);

		if (!scene.Nodes.empty())
		{
			rapidjson::Value nodesArray(rapidjson::kArrayType);
			for (const auto& nodeId : scene.Nodes)
			{
				nodesArray.PushBack(ToKnownSizeType(gltfDocument.Nodes.GetIndex(nodeId)), a);
			}
			sceneValue.AddMember("nodes", nodesArray, a);
		}

		RapidJsonUtils::AddOptionalMember("name", sceneValue, scene.Name, a);

		SerializeProperty(gltfDocument, scene, sceneValue, a, extensionSerializer);

		return sceneValue;
	}
	/****************************************************************************
	*                       SerializeSkin
	*************************************************************************//**
	*  @fn        rapidjson::Value SerializeSkin(const GLTFSkin& skin, const GLTFDocument& gltfDocument, rapidjson::Document& jsonDocument, const ExtensionSerializer& extensionSerializer)
	*  @brief     Serialize Skin
	*  @param[in] GLTFSkin
	*  @param[in] GLTFDocument& document
	*  @param[in,out] rapidjson::Document
	*  @param[in] ExtensionSerializer& extensionSerializer
	*  @return 　　rapidjson::Value
	*****************************************************************************/
	rapidjson::Value SerializeSkin(const GLTFSkin& skin, const GLTFDocument& gltfDocument, rapidjson::Document& document, const ExtensionSerializer& extensionSerializer)
	{
		rapidjson::Document::AllocatorType& a = document.GetAllocator();
		rapidjson::Value skinValue(rapidjson::kObjectType);
		{
			RapidJsonUtils::AddOptionalMemberIndex("inverseBindMatrices", skinValue, skin.InverseBindMatricesAccessorID, gltfDocument.Accessors, a);
			RapidJsonUtils::AddOptionalMemberIndex("skeleton", skinValue, skin.SkeletonID, gltfDocument.Nodes, a);

			if (!skin.JointIDs.empty())
			{
				rapidjson::Value jointIds(rapidjson::kArrayType);
				{
					for (const auto& jointId : skin.JointIDs)
					{
						jointIds.PushBack(ToKnownSizeType(gltfDocument.Nodes.GetIndex(jointId)), a);
					}
				}
				skinValue.AddMember("joints", jointIds, a);
			}

			RapidJsonUtils::AddOptionalMember("name", skinValue, skin.Name, a);

			SerializeProperty(gltfDocument, skin, skinValue, a, extensionSerializer);
		}
		return skinValue;
	}
	/****************************************************************************
	*                       SerializeTexture
	*************************************************************************//**
	*  @fn        rapidjson::Value SerializeTexture(const GLTFTexture& texture, const GLTFDocument& gltfDocument, rapidjson::Document& jsonDocument, const ExtensionSerializer& extensionSerializer)
	*  @brief     Serialize Texture
	*  @param[in] GLTFTexture
	*  @param[in] GLTFDocument& document
	*  @param[in,out] rapidjson::Document
	*  @param[in] ExtensionSerializer& extensionSerializer
	*  @return 　　rapidjson::Value
	*****************************************************************************/
	rapidjson::Value SerializeTexture(const GLTFTexture& texture, const GLTFDocument& gltfDocument, rapidjson::Document& jsonDocument, const ExtensionSerializer& extensionSerializer)
	{
		rapidjson::Document::AllocatorType& a = jsonDocument.GetAllocator();
		rapidjson::Value textureValue(rapidjson::kObjectType);

		RapidJsonUtils::AddOptionalMember("name", textureValue, texture.Name, a);
		RapidJsonUtils::AddOptionalMemberIndex("sampler", textureValue, texture.SamplerID, gltfDocument.Samplers, a);
		RapidJsonUtils::AddOptionalMemberIndex("source", textureValue, texture.ImageID, gltfDocument.Images, a);

		SerializeProperty(gltfDocument, texture, textureValue, a, extensionSerializer);

		return textureValue;
	}
	/****************************************************************************
	*                       SerializeExtensions
	*************************************************************************//**
	*  @fn        void SerializeExtensions(const GLTFDocument& gltfDocument, rapidjson::Document& jsonDocument, const ExtensionSerializer& extensionSerializer)
	*  @brief     Serialize Camera
	*  @param[in] GLTFDocument& document
	*  @param[in,out] rapidjson::Document
	*  @param[in] ExtensionSerializer& extensionSerializer
	*  @return 　　void
	*****************************************************************************/
	void SerializeExtensions(const GLTFDocument& gltfDocument, rapidjson::Document& jsonDocument, const ExtensionSerializer& extensionSerializer)
	{
		rapidjson::Document::AllocatorType& a = jsonDocument.GetAllocator();
		rapidjson::Value extensionValue(rapidjson::kObjectType);

		SerializePropertyExtensions(gltfDocument, gltfDocument, extensionValue, a, extensionSerializer);

		if (extensionValue.HasMember("extensions"))
		{
			auto& value = extensionValue.FindMember("extensions")->value;
			jsonDocument.AddMember("extensions", value, a);
		}
	}
	/****************************************************************************
	*                       SerializeStringSet
	*************************************************************************//**
	*  @fn        void SerializeStringSet(const std::string& key, const std::unordered_set<std::string> set, rapidjson::Document& document)
	*  @brief     Serialize Camera
	*  @param[in] std::string key
	*  @param[in] std::unordered_set<std::string> set
	*  @param[in,out] rapidjson::Document
	*  @return 　　void
	*****************************************************************************/
	void SerializeStringSet(const std::string& key, const std::unordered_set<std::string> set, rapidjson::Document& document)
	{
		if (!set.empty())
		{
			rapidjson::Document::AllocatorType& a = document.GetAllocator();
			rapidjson::Value extensions(rapidjson::kArrayType);
			for (const auto& element : set)
			{
				extensions.PushBack(RapidJsonUtils::ToStringValue(element, a), a);
			}
			document.AddMember(RapidJsonUtils::ToStringValue(key, a), extensions, a);
		}
	}
	/****************************************************************************
	*                       SerializeExtensionsUsed
	*************************************************************************//**
	*  @fn        void SerializeExtensionsUsed(const GLTFDocument& gltfDocument, rapidjson::Document& jsonDocument, const ExtensionSerializer& extensionSerializer)
	*  @brief     Serialize ExtensionUsed
	*  @param[in] GLTFDocument& document
	*  @param[in,out] rapidjson::Document
	*  @param[in] ExtensionSerializer& extensionSerializer
	*  @return 　　void
	*****************************************************************************/
	void SerializeExtensionsUsed(const GLTFDocument& gltfDocument, rapidjson::Document& document)
	{
		SerializeStringSet("extensionsUsed", gltfDocument.ExtensionsUsed, document);
	}
	/****************************************************************************
	*                       SerializeExtensionsIs
	*************************************************************************//**
	*  @fn        void SerializeExtensionsUsed(const GLTFDocument& gltfDocument, rapidjson::Document& jsonDocument, const ExtensionSerializer& extensionSerializer)
	*  @brief     Serialize ExtensionRequired
	*  @param[in] GLTFDocument& document
	*  @param[in,out] rapidjson::Document
	*  @param[in] ExtensionSerializer& extensionSerializer
	*  @return 　　void
	*****************************************************************************/
	void SerializeExtensionsRequired(const GLTFDocument& gltfDocument, rapidjson::Document& document)
	{
		for (auto& extensionName : gltfDocument.ExtensionsRequired)
		{
			if (gltfDocument.ExtensionsUsed.find(extensionName) == gltfDocument.ExtensionsUsed.end())
			{
				throw error::GLTFException("required extension '" + extensionName + "' not present in extensionsUsed.");
			}
		}

		SerializeStringSet("extensionsRequired", gltfDocument.ExtensionsRequired, document);
	}
	/****************************************************************************
	*                       CreateJsonDocument
	*************************************************************************//**
	*  @fn        rapidjson::Document CreateJsonDocument(const GLTFDocument& gltfDocument, const ExtensionSerializer& extensionSerializer)
	*  @brief     Create json document
	*  @param[in] GLTFDocument& document
	*  @param[in] ExtensionSerializer& extensionSerializer
	*  @return 　　rapidjson::Document
	*****************************************************************************/
	rapidjson::Document CreateJsonDocument(const GLTFDocument& gltfDocument, const ExtensionSerializer& extensionSerializer)
	{
		rapidjson::Document document(rapidjson::kObjectType);

		SerializeAsset(gltfDocument, document, extensionSerializer);

		SerializeIndexedContainer<GLTFAccessor>  ("accessors"  , gltfDocument.Accessors  , gltfDocument, document, extensionSerializer, SerializeAccessor);
		SerializeIndexedContainer<GLTFAnimation> ("animations" , gltfDocument.Animations , gltfDocument, document, extensionSerializer, SerializeAnimation);
		SerializeIndexedContainer<GLTFBufferView>("bufferViews", gltfDocument.BufferViews, gltfDocument, document, extensionSerializer, SerializeBufferView);
		SerializeIndexedContainer<GLTFBuffer>    ("buffers"    , gltfDocument.Buffers    , gltfDocument, document, extensionSerializer, SerializeBuffer);
		SerializeIndexedContainer<GLTFCamera>    ("cameras"    , gltfDocument.Cameras    , gltfDocument, document, extensionSerializer, SerializeCamera);
		SerializeIndexedContainer<GLTFImage>     ("images"     , gltfDocument.Images     , gltfDocument, document, extensionSerializer, SerializeImage);
		SerializeIndexedContainer<GLTFMaterial>  ("materials"  , gltfDocument.Materials  , gltfDocument, document, extensionSerializer, SerializeMaterial);
		SerializeIndexedContainer<GLTFMesh>      ("meshes"     , gltfDocument.Meshes     , gltfDocument, document, extensionSerializer, SerializeMesh);
		SerializeIndexedContainer<GLTFNode>      ("nodes"      , gltfDocument.Nodes      , gltfDocument, document, extensionSerializer, SerializeNode);
		SerializeIndexedContainer<GLTFSampler>   ("samplers"   , gltfDocument.Samplers   , gltfDocument, document, extensionSerializer, SerializeSampler);
		SerializeIndexedContainer<GLTFScene>     ("scenes"     , gltfDocument.Scenes     , gltfDocument, document, extensionSerializer, SerializeScene);
		SerializeIndexedContainer<GLTFSkin>      ("skins"      , gltfDocument.Skins      , gltfDocument, document, extensionSerializer, SerializeSkin);
		SerializeIndexedContainer<GLTFTexture>   ("textures"   , gltfDocument.Textures   , gltfDocument, document, extensionSerializer, SerializeTexture);

		SerializeDefaultScene(gltfDocument, document);

		SerializeExtensions(gltfDocument, document, extensionSerializer);

		SerializeExtensionsUsed(gltfDocument, document);
		SerializeExtensionsRequired(gltfDocument, document);

		return document;
	}
	bool HasFlag(SerializeFlags flags, SerializeFlags flag)
	{
		return ((flags & flag) == flag);
	}
}
std::string gltf::Serialize(const GLTFDocument& gltfDocument, gltf::SerializeFlags flags)
{
	return Serialize(gltfDocument, ExtensionSerializer(), flags);
}
std::string gltf::Serialize(const GLTFDocument& gltfDocument, const ExtensionSerializer& extensionSerializer, gltf::SerializeFlags flags)
{
	auto doc = CreateJsonDocument(gltfDocument, extensionSerializer);

	rapidjson::StringBuffer stringBuffer;
	if (HasFlag(flags, SerializeFlags::Pretty))
	{
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(stringBuffer);
		doc.Accept(writer);
	}
	else
	{
		rapidjson::Writer<rapidjson::StringBuffer> writer(stringBuffer);
		doc.Accept(writer);
	}

	return stringBuffer.GetString();
}

gltf::SerializeFlags  gltf::operator| (gltf::SerializeFlags lhs, gltf::SerializeFlags rhs)
{
	const auto result =
		static_cast<std::underlying_type_t<SerializeFlags>>(lhs) |
		static_cast<std::underlying_type_t<SerializeFlags>>(rhs);

	return static_cast<SerializeFlags>(result);
}
gltf::SerializeFlags& gltf::operator|=(gltf::SerializeFlags& lhs, gltf::SerializeFlags rhs)
{
	lhs = lhs | rhs;
	return lhs;
}
gltf::SerializeFlags  gltf::operator& (gltf::SerializeFlags lhs, gltf::SerializeFlags rhs)
{
	const auto result =
		static_cast<std::underlying_type_t<SerializeFlags>>(lhs) &
		static_cast<std::underlying_type_t<SerializeFlags>>(rhs);

	return static_cast<SerializeFlags>(result);
}
gltf::SerializeFlags& gltf::operator&=(gltf::SerializeFlags& lhs, gltf::SerializeFlags rhs)
{
	lhs = lhs & rhs;
	return lhs;
}