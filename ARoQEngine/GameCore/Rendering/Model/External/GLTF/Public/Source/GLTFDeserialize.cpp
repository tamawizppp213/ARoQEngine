//////////////////////////////////////////////////////////////////////////////////
//              @file   GLTFDeserialize.cpp
///             @brief  GLTF Deserialize
///             @author Partially edit by Toide
///             @date   2022_05_19
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/GLTF/Public/Include/GLTFDeserialize.hpp"
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFProperty.hpp"
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFRapidJsonUtils.hpp"
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFExtensionHandlers.hpp"
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFConstants.hpp"
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFSchemaValidation.hpp"
#include "GameUtility/File/Include/JsonOld.hpp"
#include <iostream>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gltf;
using namespace gltf::detail;
using namespace gltf::detail::asset;
using namespace gltf::detail::define;

#pragma warning(disable: 4244)
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
namespace
{
	/****************************************************************************
	*                       DeserializeToIndexedContainer
	****************************************************************************/
	/* @fn        template<typename T>
				　 IndexedContainer<const T> DeserializeToIndexedContainer(const char* name, const rapidjson::Value& value)
	*  @brief     Deserialize index container
	*  @param[in] const char* name
	*  @param[in] const rapidjson::Value& value
	*  @return 　　IndexedContainer
	*****************************************************************************/
	template<typename T>
	IndexedContainer<const T> DeserializeToIndexedContainer(const char* name, const rapidjson::Value& value, const ExtensionDeserializer& extensionDeserializer,
		T(*fn)(const rapidjson::Value&, const ExtensionDeserializer& ))
	{
		IndexedContainer<const T> items;

		rapidjson::Value::ConstMemberIterator iterator;
		if (TryFindMember(name, value, iterator))
		{
			size_t index = 0;
			for (auto& valueArray : iterator->value.GetArray())
			{
				try
				{
					const auto& item = items.Append(fn(valueArray, extensionDeserializer), AppendIdPolicy::GenerateOnEmpty);
					const auto& id = item.ID;
					(void)id;
					assert(id == std::to_string(index));
				}
				catch(const error::InvalidGLTFException& e)
				{
					std::cerr << "Could not parse " << name << "[" << index << "]" << e.what() << "\n";
					throw;
				}
				++index;
			}
		}
		return items;
	}
	/****************************************************************************
	*                       ParseExtensions
	****************************************************************************/
	/* @fn        void ParseExtensions(const rapidjson::Value& value, detail::GLTFProperty& node)
	*  @brief     Parse extension data
	*  @param[in] rapidjson::Value& value
	*  @param[in] detail::GLTFProperty& node
	*  @return 　　void
	*****************************************************************************/
	void ParseExtensions(const rapidjson::Value& value, detail::GLTFProperty& node, const ExtensionDeserializer& extensionDeserializer)
	{
		/*-------------------------------------------------------------------
		-                    Find Extension Member 
		---------------------------------------------------------------------*/
		const auto& iterator = value.FindMember("extensions");
		if (iterator != value.MemberEnd())
		{
			/*-------------------------------------------------------------------
			-                    Get Extension Object
			---------------------------------------------------------------------*/
			const rapidjson::Value& extensionsObject = iterator->value;
			/*-------------------------------------------------------------------
			-                    Get Extension Data 
			---------------------------------------------------------------------*/
			for (const auto& entry : extensionsObject.GetObject())
			{
				ExtensionPair extensionPair = { entry.name.GetString(), Serialize(entry.value) };

				if (extensionDeserializer.HasHandler(extensionPair.Name, node) || extensionDeserializer.HasHandler(extensionPair.Name))
				{
					node.SetExtension(extensionDeserializer.DeSerialize(extensionPair, node));
				}
				else
				{
					node.Extensions.emplace(std::move(extensionPair.Name), std::move(extensionPair.Value));
				}
			}
		}
	}
	/****************************************************************************
	*                       ParseExtras
	****************************************************************************/
	/* @fn        void ParseExtras(const rapidjson::Value& value, detail::GLTFProperty& node)
	*  @brief     Parse extra data
	*  @param[in] rapidjson::Value& value
	*  @param[in] detail::GLTFProperty& node
	*  @return 　　void
	*****************************************************************************/
	void ParseExtras(const rapidjson::Value& value, detail::GLTFProperty& node)
	{
		rapidjson::Value::ConstMemberIterator iterator;
		if (TryFindMember("extras", value, iterator))
		{
			const rapidjson::Value& v = iterator->value;
			node.Extras = Serialize(v);
		}
	}
	/****************************************************************************
	*                       ParseProperty
	****************************************************************************/
	/* @fn        void ParseProperty(const rapidjson::Value& value, detail::GLTFProperty& node)
	*  @brief     Parse extra property data
	*  @param[in] rapidjson::Value& value
	*  @param[in] detail::GLTFProperty& node
	*  @return 　　void
	*****************************************************************************/
	void ParseProperty(const rapidjson::Value& value, detail::GLTFProperty& node, const ExtensionDeserializer& extensionDeserializer)
	{
		ParseExtensions(value, node, extensionDeserializer);
		ParseExtras(value, node);
	}
	/****************************************************************************
	*                       ParseTextureInfo
	****************************************************************************/
	/* @fn        void ParseTextureInfo(const rapidjson::Value& value, detail::asset::GLTFTextureInfo textureInfomation)
	*  @brief     Parse TextureInfo data
	*  @param[in] rapidjson::Value& value
	*  @param[in] detail::GLTFTextureInfo textureInfo
	*  @return 　　void
	*****************************************************************************/
	void ParseTextureInfo(const rapidjson::Value& value, GLTFTextureInfo& textureInfomation, const ExtensionDeserializer& extensionDeserializer)
	{
		auto iterator = FindRequiredMember("index", value);
		textureInfomation.TextureID = std::to_string(iterator->value.GetUint());
		textureInfomation.TexCoord = GetMemberValueOrDefault<size_t>(value, "texCoord", 0U);
		ParseProperty(value, textureInfomation, extensionDeserializer);
	}
	/****************************************************************************
	*                       ParseAsset
	****************************************************************************/
	/* @fn        GLTFAsset ParseAsset(const rapidjson::Value& assetValue, const ExtensionDeserializer& extensionDeserializer)
	*  @brief     Parse asset
	*  @param[in] rapidjson::Value& value
	*  @param[in] const ExtensionDeserializer& extensionDeserializer
	*  @return 　　GLTFAsset
	*****************************************************************************/
	GLTFAsset ParseAsset(const rapidjson::Value& assetValue, const ExtensionDeserializer& extensionDeserializer)
	{
		GLTFAsset asset;
		asset.CopyRight  = GetMemberValueOrDefault<std::string>(assetValue, "copyright");
		asset.Generator  = GetMemberValueOrDefault<std::string>(assetValue, "generator");
		asset.Version    = FindRequiredMember("version", assetValue)->value.GetString();
		asset.MinVersion = GetMemberValueOrDefault<std::string>(assetValue, "minVersion");
		ParseProperty(assetValue, asset, extensionDeserializer);
		return asset;
	}
	/****************************************************************************
	*                       ParseAsset
	****************************************************************************/
	/* @fn        GLTFAsset ParseAsset(const rapidjson::Value& assetValue, const ExtensionDeserializer& extensionDeserializer)
	*  @brief     Parse asset
	*  @param[in] rapidjson::Value& value
	*  @param[in] const ExtensionDeserializer& extensionDeserializer
	*  @return 　　GLTFAsset
	*****************************************************************************/
	GLTFAccessor ParseAccessor(const rapidjson::Value& value, const ExtensionDeserializer& extensionDeserializer)
	{
		GLTFAccessor accessor;
		accessor.Name = GetMemberValueOrDefault<std::string>(value, "name");

		rapidjson::Value::ConstMemberIterator iterator;
		/*-------------------------------------------------------------------
		-                    Get sparse data
		---------------------------------------------------------------------*/
		if (TryFindMember("sparse", value, iterator))
		{
			const rapidjson::Value& sparseMember = iterator->value;
			const rapidjson::Value& sparseIndiceMember = FindRequiredMember("indices", sparseMember)->value;
			const rapidjson::Value& sparseValueMember = FindRequiredMember("values", sparseMember)->value;

			accessor.Sparse.Count = GetValue<size_t>(FindRequiredMember("count", sparseMember)->value);
			accessor.Sparse.IndicesBufferViewID  = std::to_string(FindRequiredMember("bufferView", sparseIndiceMember)->value.GetUint());
			accessor.Sparse.IndicesComponentType = GLTFAccessor::GetComponentType(FindRequiredMember("componentType", sparseIndiceMember)->value.GetUint());
			accessor.Sparse.IndicesByteOffset    = GetMemberValueOrDefault<size_t>(sparseIndiceMember, "byteOffset");
			accessor.Sparse.ValuesBufferViewID   = std::to_string(FindRequiredMember("bufferView", sparseValueMember)->value.GetUint());
			accessor.Sparse.ValuesByteOffset     = GetMemberValueOrDefault<size_t>(sparseValueMember, "byteOffset");

			if (TryFindMember("bufferView", value, iterator))
			{
				accessor.BufferViewID = std::to_string(iterator->value.GetUint());
			}
		
		}
		else
		{
			accessor.BufferViewID = GetMemberValueAsString<size_t>(value, "bufferView");
		}

		/*-------------------------------------------------------------------
		-                    Get Default Accessor data
		---------------------------------------------------------------------*/
		accessor.ByteOffset        = GetMemberValueOrDefault<size_t>(value, "byteOffset");
		accessor.ComponentDataType = GLTFAccessor::GetComponentType(FindRequiredMember("componentType", value)->value.GetUint());
		accessor.IsNormalized      = GetMemberValueOrDefault<bool>(value, "normalized", false);
		accessor.Count             = GetValue<size_t>(FindRequiredMember("count", value)->value);
		accessor.AccessorDataType  = GLTFAccessor::ParseType(FindRequiredMember("type", value)->value.GetString());

		if (TryFindMember("min", value, iterator))
		{
			for (rapidjson::Value::ConstValueIterator it = iterator->value.Begin(); it != iterator->value.End(); ++it)
			{
				accessor.MinValue.push_back(static_cast<float>(it->GetDouble()));
			}
		}

		if (TryFindMember("max", value, iterator))
		{
			for (rapidjson::Value::ConstValueIterator it = iterator->value.Begin(); it != iterator->value.End(); ++it)
			{
				accessor.MaxValue.push_back(static_cast<float>(it->GetDouble()));
			}
		}

		ParseProperty(value, accessor, extensionDeserializer);
		return accessor;
	}
	/****************************************************************************
	*                       ParseBufferView
	****************************************************************************/
	/* @fn        GLTFBufferView ParseBufferView(const rapidjson::Value& value, const ExtensionDeserializer& extensionDeserializer)
	*  @brief     Parse Buffer view
	*  @param[in] rapidjson::Value& value
	*  @param[in] const ExtensionDeserializer& extensionDeserializer
	*  @return 　　GLTFBufferView
	*****************************************************************************/
	GLTFBufferView ParseBufferView(const rapidjson::Value& value, const ExtensionDeserializer& extensionDeserializer)
	{
		GLTFBufferView bufferView;

		bufferView.Name       = GetMemberValueOrDefault<std::string>(value, "name");
		bufferView.BufferID   = std::to_string(FindRequiredMember("buffer", value)->value.GetUint());
		bufferView.ByteOffset = GetMemberValueOrDefault<size_t>(value, "byteOffset");
		bufferView.ByteLength = GetValue<size_t>(FindRequiredMember("byteLength", value)->value);

		auto byteStride = value.FindMember("byteStride");
		if (byteStride != value.MemberEnd())
		{
			bufferView.ByteStride = byteStride->value.GetUint();
		}

		auto target = value.FindMember("target");
		if (target != value.MemberEnd())
		{
			bufferView.Target = static_cast<BufferViewTarget>(target->value.GetUint());
		}

		ParseProperty(value, bufferView, extensionDeserializer);
		return bufferView;
	}
	/****************************************************************************
	*                       ParseScene
	****************************************************************************/
	/* @fn        GLTFScene ParseScene(const rapidjson::Value& value, const ExtensionDeserializer& extensionDeserializer)
	*  @brief     Parse Scene
	*  @param[in] rapidjson::Value& value
	*  @param[in] const ExtensionDeserializer& extensionDeserializer
	*  @return 　　GLTFScene
	*****************************************************************************/
	GLTFScene ParseScene(const rapidjson::Value& value, const ExtensionDeserializer& extensionDeserializer)
	{
		GLTFScene scene;

		rapidjson::Value::ConstMemberIterator iterator = value.FindMember("nodes");
		if (iterator != value.MemberEnd())
		{
			const rapidjson::Value& v = iterator->value;
			scene.Nodes.reserve(v.Capacity());
			for (rapidjson::Value::ConstValueIterator vit = v.Begin(); vit != v.End(); ++vit)
			{
				scene.Nodes.push_back(std::to_string(vit->GetUint()));
			}
		}

		ParseProperty(value, scene, extensionDeserializer);
		return scene;
	}
	/****************************************************************************
	*                       ParseMorphTarget
	****************************************************************************/
	/* @fn        GLTFScene ParseMorphTarget(const rapidjson::Value& value)
	*  @brief     Parse MorphTarget
	*  @param[in] rapidjson::Value& value
	*  @return 　　GLTFMorphTarget
	*****************************************************************************/
	GLTFMorphTarget ParseMorphTarget(const rapidjson::Value& value)
	{
		GLTFMorphTarget target;
		target.PositionsAccessorID = GetMemberValueAsString<uint32_t>(value, define::ACCESSOR_POSITION);
		target.NormalsAccessorID   = GetMemberValueAsString<uint32_t>(value, define::ACCESSOR_NORMAL);
		target.TangentsAccessorID = GetMemberValueAsString<uint32_t>(value, define::ACCESSOR_TANGENT);
		return target;
	}
	/****************************************************************************
	*                       ParseMorphTargets
	****************************************************************************/
	/* @fn        GLTFScene ParseMorphTargets(const rapidjson::Value& value, GLTFMeshPrimitive& primitive)
	*  @brief     Parse MorphTargets
	*  @param[in] rapidjson::Value& value
	*  @param[in] GLTFMeshPrimitive& primitive
	*  @return 　　void
	*****************************************************************************/
	void ParseMorphTargets(const rapidjson::Value& value, GLTFMeshPrimitive& primitive)
	{
		rapidjson::Value::ConstMemberIterator iterator = value.FindMember("targets");
		if (iterator != value.MemberEnd())
		{
			const rapidjson::Value& v = iterator->value;
			primitive.Targets.reserve(v.Capacity());
			for (rapidjson::Value::ConstValueIterator vit = v.Begin(); vit != v.End(); ++vit)
			{
				primitive.Targets.push_back(ParseMorphTarget(*vit));
			}
		}
	}
	/****************************************************************************
	*                       ParseMeshPrimitive
	****************************************************************************/
	/* @fn        GLTFMeshPrimitive ParseMeshPrimitive(const rapidjson::Value& value, const ExtensionDeserializer& extensionDeserializer)
	*  @brief     Parse MeshPrimitive
	*  @param[in] rapidjson::Value& value
	*  @param[in] const ExtensionDeserializer& extensionDeserializer
	*  @return 　　GLTFMeshPrimitive
	*****************************************************************************/
	GLTFMeshPrimitive ParseMeshPrimitive(const rapidjson::Value& value, const ExtensionDeserializer& extensionDeserializer)
	{
		GLTFMeshPrimitive primitive;

		rapidjson::Value::ConstMemberIterator iterator = value.FindMember("attributes");
		if (iterator != value.MemberEnd())
		{
			const auto& attributes = iterator->value.GetObject();

			for (const auto& attribute : attributes)
			{
				auto name = attribute.name.GetString();
				primitive.Attributes[name] = std::to_string(attribute.value.Get<uint32_t>());
			}
		}

		primitive.IndicesAccessorID = GetMemberValueAsString<uint32_t>(value, "indices");
		primitive.MaterialID        = GetMemberValueAsString<uint32_t>(value, "material");
		primitive.MeshMode          = static_cast<GLTFMeshMode>(GetMemberValueOrDefault<int>(value, "mode", (int)GLTFMeshMode::Triangles));
		ParseMorphTargets(value, primitive);
		ParseProperty(value, primitive, extensionDeserializer);
		return primitive;
	}
	/****************************************************************************
	*                       ParseMesh
	****************************************************************************/
	/* @fn        GLTFMesh ParseMesh(const rapidjson::Value& value, const ExtensionDeserializer& extensionDeserializer)
	*  @brief     Parse Mesh
	*  @param[in] rapidjson::Value& value
	*  @param[in] const ExtensionDeserializer& extensionDeserializer
	*  @return 　　GLTFMesh
	*****************************************************************************/
	GLTFMesh ParseMesh(const rapidjson::Value& value, const ExtensionDeserializer& extensionDeserializer)
	{
		GLTFMesh mesh;
		mesh.Name = GetMemberValueOrDefault<std::string>(value, "name");

		rapidjson::Value::ConstMemberIterator iterator = value.FindMember("primitives");

		if (iterator != value.MemberEnd())
		{
			const rapidjson::Value& v = iterator->value;
			mesh.Primitives.reserve(v.Capacity());
			for (rapidjson::Value::ConstValueIterator vit = v.Begin(); vit != v.End(); ++vit)
			{
				mesh.Primitives.push_back(ParseMeshPrimitive(*vit, extensionDeserializer));
			}
		}
		mesh.Weights = RapidJsonUtils::ToFloatArray(value, "weights");
		ParseProperty(value, mesh, extensionDeserializer);
		return mesh;
	}
	/****************************************************************************
	*                       ParseNodeScale
	****************************************************************************/
	/* @fn        void ParseNodeScale(const rapidjson::Value& value, GLTFNode& node)
	*  @brief     Parse Node Scale
	*  @param[in] rapidjson::Value& value
	*  @param[in] GLTFNode& node
	*  @return 　　void
	*****************************************************************************/
	void ParseNodeScale(const rapidjson::Value& value, GLTFNode& node)
	{
		const int scaleCapacity = 3;
		auto iterator = value.FindMember("scale");
		if (iterator == value.MemberEnd())
		{
			node.Scale = gm::Float3(1, 1, 1);
			return;
		}

		const rapidjson::Value& v = iterator->value;
		if (v.Capacity() != scaleCapacity)
		{
			throw error::InvalidGLTFException("A node mush have a scale with 3 elements");
		}

		rapidjson::Value::ConstValueIterator vit = v.Begin();
		node.Scale.x = vit++->GetFloat();
		node.Scale.y = vit++->GetFloat();
		node.Scale.z = vit->GetFloat();
	}
	/****************************************************************************
	*                       ParseNodeTranslation
	****************************************************************************/
	/* @fn        void ParseNodeScale(const rapidjson::Value& value, GLTFNode& node)
	*  @brief     Parse Node Translation
	*  @param[in] rapidjson::Value& value
	*  @param[in] GLTFNode& node
	*  @return 　　void
	*****************************************************************************/
	void ParseNodeTranslation(const rapidjson::Value& value, GLTFNode& node)
	{
		const int translationCapacity = 3;
		auto iterator = value.FindMember("translation");
		if (iterator == value.MemberEnd())
		{
			node.Translation = gm::Float3(0, 0, 0);
			return;
		}

		const rapidjson::Value& v = iterator->value;
		if (v.Capacity() != translationCapacity)
		{
			throw error::InvalidGLTFException("A node must have a translation with 3 elements");
		}

		rapidjson::Value::ConstValueIterator vit = v.Begin();
		node.Translation.x = vit++->GetFloat();
		node.Translation.y = vit++->GetFloat();
		node.Translation.z = vit->GetFloat();
	}
	/****************************************************************************
	*                       ParseNodeRotation
	****************************************************************************/
	/* @fn        void ParseNodeRotation(const rapidjson::Value& value, GLTFNode& node)
	*  @brief     Parse Node Rotation
	*  @param[in] rapidjson::Value& value
	*  @param[in] GLTFNode& node
	*  @return 　　void
	*****************************************************************************/
	void ParseNodeRotation(const rapidjson::Value& value, GLTFNode& node)
	{
		const int rotationCapacity = 4;
		auto iterator = value.FindMember("rotation");
		if (iterator == value.MemberEnd())
		{
			node.Rotation = gm::Float4(0, 0, 0, 1);
			return;
		}

		const rapidjson::Value& v = iterator->value;
		if (v.Capacity() != rotationCapacity)
		{
			throw error::InvalidGLTFException("A node must have a rotation with 4 elements");
		}

		rapidjson::Value::ConstValueIterator vit = v.Begin();
		node.Rotation.x = vit++->GetFloat();
		node.Rotation.y = vit++->GetFloat();
		node.Rotation.z = vit++->GetFloat();
		node.Rotation.w = vit->GetFloat();
	}
	/****************************************************************************
	*                       ParseNodeMatrix
	****************************************************************************/
	/* @fn        void ParseNodeMatrix(const rapidjson::Value& value, GLTFNode& node)
	*  @brief     Parse Node Matrix
	*  @param[in] rapidjson::Value& value
	*  @param[in] GLTFNode& node
	*  @return 　　void
	*****************************************************************************/
	void ParseNodeMatrix(const rapidjson::Value& value, GLTFNode& node)
	{
		auto iterator = value.FindMember("matrix");
		if (iterator == value.MemberEnd())
		{
			ParseNodeScale      (value, node);
			ParseNodeTranslation(value, node);
			ParseNodeRotation   (value, node);
			return;
		}

		const rapidjson::Value& v = iterator->value;
		if (v.Capacity() != 16)
		{
			throw error::InvalidGLTFException("A node must have a matrix transform with 16 elements");
		}

		uint8_t index = 0;
		for (rapidjson::Value::ConstValueIterator vit = v.Begin(); vit != v.End(); ++vit)
		{
			node.Matrix(index / 4, index % 4) = static_cast<float>(vit->GetDouble());
			index++;
		}
	}
	/****************************************************************************
	*                       ParseNodeChildren
	****************************************************************************/
	/* @fn        void ParseNodeChildren(const rapidjson::Value& value, GLTFNode& node)
	*  @brief     Parse Node Children
	*  @param[in] rapidjson::Value& value
	*  @param[in] GLTFNode& node
	*  @return 　　void
	*****************************************************************************/
	void ParseNodeChildren(const rapidjson::Value& value, GLTFNode& node)
	{
		rapidjson::Value::ConstMemberIterator iterator = value.FindMember("children");
		if (iterator != value.MemberEnd())
		{
			const rapidjson::Value& v = iterator->value;
			node.Children.reserve(v.Capacity());
			for (rapidjson::Value::ConstValueIterator vit = v.Begin(); vit != v.End(); ++vit)
			{
				node.Children.push_back(std::to_string(vit->GetUint()));
			}
		}
	}
	/****************************************************************************
	*                       ParseCamera
	****************************************************************************/
	/* @fn        GLTFCamera ParseCamera(const rapidjson::Value& value, const ExtensionDeserializer& extensionDeserializer)
	*  @brief     Parse Camera
	*  @param[in] rapidjson::Value& value
	*  @param[in] const ExtensionDeserializer& extensionDeserializer
	*  @return 　　GLTFCamera
	*****************************************************************************/
	GLTFCamera ParseCamera(const rapidjson::Value& value, const ExtensionDeserializer& extensionDeserializer)
	{
		std::unique_ptr<GLTFProjection> projection;
		std::string projectionType = FindRequiredMember("type", value)->value.GetString();

		/*-------------------------------------------------------------------
		-                      Perspective
		---------------------------------------------------------------------*/
		if (projectionType == "perspective")
		{
			auto perspectiveIterator = value.FindMember("perspective");
			if (perspectiveIterator == value.MemberEnd())
			{
				throw error::InvalidGLTFException("Camera perspective projection undefined");
			}

			/*-------------------------------------------------------------------
			-                      AspectRatio
			---------------------------------------------------------------------*/
			Optional<float> aspectRatio;
			auto aspectRatioIterator = perspectiveIterator->value.FindMember("aspectRatio");
			if (aspectRatioIterator != perspectiveIterator->value.MemberEnd())
			{
				aspectRatio = aspectRatioIterator->value.GetFloat();
			}

			/*-------------------------------------------------------------------
			-                      YFov
			---------------------------------------------------------------------*/
			float yfov = GetValue<float>(FindRequiredMember("yfov", perspectiveIterator->value)->value);
			/*-------------------------------------------------------------------
			-                      ZNear
			---------------------------------------------------------------------*/
			float znear = GetValue<float>(FindRequiredMember("znear", perspectiveIterator->value)->value);
			/*-------------------------------------------------------------------
			-                      ZFar
			---------------------------------------------------------------------*/
			Optional<float> zfar;
			auto zfarIterator = perspectiveIterator->value.FindMember("zfar");
			if (zfarIterator != perspectiveIterator->value.MemberEnd())
			{
				zfar = zfarIterator->value.GetFloat();
			}

			auto perspective = std::make_unique<GLTFPerspective>(znear, yfov);
			perspective->AspectRatio = aspectRatio;
			ParseProperty(perspectiveIterator->value, *perspective, extensionDeserializer);

			projection = std::move(perspective);
		}
		/*-------------------------------------------------------------------
		-                      Orthographic
		---------------------------------------------------------------------*/
		else if (projectionType == "orthographic")
		{
			auto orthographicIterator = value.FindMember("orthographic");
			if (orthographicIterator == value.MemberEnd())
			{
				throw error::InvalidGLTFException("Camera orthographic projection undefined");
			}

			float xmag  = GetValue<float>(FindRequiredMember("xmag", orthographicIterator->value)->value);
			float ymag  = GetValue<float>(FindRequiredMember("ymag", orthographicIterator->value)->value);
			float zfar  = GetValue<float>(FindRequiredMember("zfar", orthographicIterator->value)->value);
			float znear = GetValue<float>(FindRequiredMember("znear", orthographicIterator->value)->value);

			projection = std::make_unique<GLTFOrthographic>(zfar, znear, xmag, ymag);
			ParseProperty(orthographicIterator->value, *projection, extensionDeserializer);
		}

		GLTFCamera camera(std::move(projection));
		camera.Name = GetMemberValueOrDefault<std::string>(value, "name");
		
		if (!camera.Projection->IsValid()) { throw error::InvalidGLTFException("Camera's projection is not valid"); }

		ParseProperty(value, camera, extensionDeserializer);
		return camera;
	}
	/****************************************************************************
	*                       ParseNode
	****************************************************************************/
	/* @fn        GLTFNode ParseNode(const rapidjson::Value& value, const ExtensionDeserializer& extensionDeserialzier)
	*  @brief     Parse Node
	*  @param[in] rapidjson::Value& value
	*  @param[in] ExtensionDeserializer& extensionDeserializer
	*  @return 　　GLTFNode
	*****************************************************************************/
	GLTFNode ParseNode(const rapidjson::Value& value, const ExtensionDeserializer& extensionDeserialzier)
	{
		GLTFNode node;
		node.Name = GetMemberValueOrDefault<std::string>(value, "name");

		ParseNodeChildren(value, node);
		node.MeshID   = GetMemberValueAsString<uint32_t>(value, "mesh");
		node.SkinID   = GetMemberValueAsString<uint32_t>(value, "skin");
		node.CameraID = GetMemberValueAsString<uint32_t>(value, "camera");
		ParseNodeMatrix(value, node);
		node.Weights = RapidJsonUtils::ToFloatArray(value, "weights");
		ParseProperty(value, node, extensionDeserialzier);
		return node;
	}
	/****************************************************************************
	*                       ParseBuffer
	****************************************************************************/
	/* @fn        GLTFBuffer ParseNode(const rapidjson::Value& value, const ExtensionDeserializer& extensionDeserialzier)
	*  @brief     Parse Buffer
	*  @param[in] rapidjson::Value& value
	*  @param[in] ExtensionDeserializer& extensionDeserializer
	*  @return 　　GLTFBuffer
	*****************************************************************************/
	GLTFBuffer ParseBuffer(const rapidjson::Value& value, const ExtensionDeserializer& extensionDeserializer)
	{
		GLTFBuffer buffer;
		buffer.ByteLength = GetValue<size_t>(FindRequiredMember("byteLength", value)->value);
		buffer.URI = GetMemberValueOrDefault<std::string>(value, "uri");
		ParseProperty(value, buffer, extensionDeserializer);
		return buffer;
	}
	/****************************************************************************
	*                       ParseSampler
	****************************************************************************/
	/* @fn        GLTFSampler ParseSampler(const rapidjson::Value& value, const ExtensionDeserializer& extensionDeserialzier)
	*  @brief     Parse Sampler
	*  @param[in] rapidjson::Value& value
	*  @param[in] ExtensionDeserializer& extensionDeserializer
	*  @return 　　GLTFSampler
	*****************************************************************************/
	GLTFSampler ParseSampler(const rapidjson::Value& value, const ExtensionDeserializer& extensionDeserializer)
	{
		GLTFSampler sampler;

		sampler.Name = GetMemberValueOrDefault<std::string>(value, "name");
		sampler.TextureAddressingU = GLTFSampler::GetAddressingMode(GetMemberValueOrDefault<unsigned int>(value, "wrapS", static_cast<unsigned int>(WrapMode::Repeat)));
		sampler.TextureAddressingV = GLTFSampler::GetAddressingMode(GetMemberValueOrDefault<unsigned int>(value, "wrapT", static_cast<unsigned int>(WrapMode::Repeat)));

		auto minIterator = value.FindMember("minFilter");
		if (minIterator != value.MemberEnd())
		{
			sampler.MinFilter = GLTFSampler::GetMinFilterMode(minIterator->value.GetUint());
		}

		auto magIterator = value.FindMember("maxFilter");
		if (magIterator != value.MemberEnd())
		{
			sampler.MagFilter = GLTFSampler::GetMagFilterMode(magIterator->value.GetUint());
		}
		ParseProperty(value, sampler, extensionDeserializer);
		return sampler;
	}
	/****************************************************************************
	*                       ParseAnimationTarget
	****************************************************************************/
	/* @fn        GLTFAnimationtarget ParseAnimationTarget(const rapidjson::Value& value, const ExtensionDeserializer& extensionDeserialzier)
	*  @brief     Parse Animation Target
	*  @param[in] rapidjson::Value& value
	*  @param[in] ExtensionDeserializer& extensionDeserializer
	*  @return 　　GLTFAnimationTarget
	*****************************************************************************/
	GLTFAnimationTarget ParseAnimationTarget(const rapidjson::Value& value, const ExtensionDeserializer& extensionDeserializer)
	{
		try
		{
			GLTFAnimationTarget target;
			target.NodeID = GetMemberValueAsString<uint32_t>(value, "node");

			auto iterator = value.FindMember("path");
			if (iterator != value.MemberEnd())
			{
				target.Path = ParseTargetPath(iterator->value.GetString());
			}

			ParseProperty(value, target, extensionDeserializer);
			return target;
		}
		catch (const error::InvalidGLTFException& e)
		{
			std::cerr << "Could not parse animation target\n" << e.what() << "\n";
			throw;
		}
	}
	/****************************************************************************
	*                       ParseAnimationChannel
	****************************************************************************/
	/* @fn        GLTFAnimationChannel ParseAnimationChannel(const rapidjson::Value& value, const ExtensionDeserializer& extensionDeserialzier)
	*  @brief     Parse Animation Channel
	*  @param[in] rapidjson::Value& value
	*  @param[in] ExtensionDeserializer& extensionDeserializer
	*  @return 　　GLTFAnimationChannel
	*****************************************************************************/
	GLTFAnimationChannel ParseAnimationChannel(const rapidjson::Value& value, const ExtensionDeserializer& extensionDeserializer)
	{
		try
		{
			GLTFAnimationChannel channel;
			channel.SampleID = GetMemberValueAsString<uint32_t>(value, "sampler");
			channel.Target   = ParseAnimationTarget(FindRequiredMember("target", value)->value, extensionDeserializer);

			ParseProperty(value, channel, extensionDeserializer);
			return channel;
		}
		catch (const error::InvalidGLTFException& e)
		{
			std::cerr << "Could not parse channel\n" << e.what() << "\n";
			throw;
		}
	}
	/****************************************************************************
	*                       ParseAnimationSampler
	****************************************************************************/
	/* @fn        GLTFAnimationSampler ParseAnimationSampler(const rapidjson::Value& value, const ExtensionDeserializer& extensionDeserialzier)
	*  @brief     Parse Animation Sampler
	*  @param[in] rapidjson::Value& value
	*  @param[in] ExtensionDeserializer& extensionDeserializer
	*  @return 　　GLTFAnimationSampler
	*****************************************************************************/
	GLTFAnimationSampler ParseAnimationSampler(const rapidjson::Value& value, const ExtensionDeserializer& extensionDeserializer)
	{
		GLTFAnimationSampler sampler;
		sampler.InputAccessorID = GetMemberValueAsString<uint32_t>(value, "input");
		sampler.OutputAccessorID = GetMemberValueAsString<uint32_t>(value, "output");

		auto iterator = value.FindMember("interpolation");
		if (iterator != value.MemberEnd())
		{
			sampler.Interpolation = ParseInterpolationType(iterator->value.GetString());
		}
		ParseProperty(value, sampler, extensionDeserializer);
		return sampler;
	}
	/****************************************************************************
	*                       ParseAnimation
	****************************************************************************/
	/* @fn        GLTFAnimation ParseAnimation(const rapidjson::Value& value, const ExtensionDeserializer& extensionDeserialzier)
	*  @brief     Parse Animation
	*  @param[in] rapidjson::Value& value
	*  @param[in] ExtensionDeserializer& extensionDeserializer
	*  @return 　　GLTFAnimation
	*****************************************************************************/
	GLTFAnimation ParseAnimation(const rapidjson::Value& value, const ExtensionDeserializer& extensionDeserializer)
	{
		GLTFAnimation animation;
		animation.Name = GetMemberValueOrDefault<std::string>(value, "name");
		animation.Channels = DeserializeToIndexedContainer<GLTFAnimationChannel>("channels", value, extensionDeserializer, ParseAnimationChannel);
		animation.Samplers = DeserializeToIndexedContainer<GLTFAnimationSampler>("samplers", value, extensionDeserializer, ParseAnimationSampler);
		ParseProperty(value, animation, extensionDeserializer);
		return animation;
	}
	/****************************************************************************
	*                       ParseSkin
	****************************************************************************/
	/* @fn        GLTFSkin ParseSkin(const rapidjson::Value& value, const ExtensionDeserializer& extensionDeserializer)
	*  @brief     Parse Skin
	*  @param[in] rapidjson::Value& value
	*  @param[in] ExtensionDeserializer& extensionDeserializer
	*  @return 　　GLTFSkin
	*****************************************************************************/
	GLTFSkin ParseSkin(const rapidjson::Value& value, const ExtensionDeserializer& extensionDeserializer)
	{
		GLTFSkin skin;
		skin.Name                          = GetMemberValueOrDefault<std::string>(value, "name");
		skin.InverseBindMatricesAccessorID = GetMemberValueAsString<uint32_t>(value, "inverseBindMatrices");
		skin.SkeletonID                    = GetMemberValueAsString<uint32_t>(value, "skeleton");
		rapidjson::Value::ConstMemberIterator iterator = value.FindMember("joints");

		if (iterator != value.MemberEnd())
		{
			const rapidjson::Value& v = iterator->value;
			skin.JointIDs.reserve(v.Capacity());
			for (rapidjson::Value::ConstValueIterator vit = v.Begin(); vit != v.End(); ++vit)
			{
				skin.JointIDs.push_back(std::to_string(vit->GetInt()));
			}
		}
		ParseProperty(value, skin, extensionDeserializer);
		return skin;
	}
	/****************************************************************************
	*                       ParseExtensionUsed
	****************************************************************************/
	/* @fn        void ParseExtesisonUsed(const rapidjson::Document& jsonDocument, GLTFDocument& gltfDocument)
	*  @brief     Parse Extension Used
	*  @param[in] rapidjson::Document& jsonDocument
	*  @param[in] GLTFDocument& gltfDocument
	*  @return 　　void
	*****************************************************************************/
	void ParseExtesisonUsed(const rapidjson::Document& jsonDocument, GLTFDocument& gltfDocument)
	{
		rapidjson::Value::ConstMemberIterator iterator;
		if (TryFindMember("extensionsUsed", jsonDocument, iterator))
		{
			const rapidjson::Value& v = iterator->value;
			for (rapidjson::Value::ConstValueIterator vit = v.Begin(); vit != v.End(); ++vit)
			{
				gltfDocument.ExtensionsUsed.insert(vit->GetString());
			}
		}
	}
	/****************************************************************************
	*                       ParseExtensionRequired
	****************************************************************************/
	/* @fn        void ParseExtensionRequired(const rapidjson::Document& jsonDocument, GLTFDocument& gltfDocument)
	*  @brief     Parse Extension Required
	*  @param[in] rapidjson::Document& jsonDocument
	*  @param[in] GLTFDocument& gltfDocument
	*  @return 　　void
	*****************************************************************************/
	void ParseExtensionRequired(const rapidjson::Document& jsonDocument, GLTFDocument& gltfDocument)
	{
		rapidjson::Value::ConstMemberIterator iterator;
		if (TryFindMember("extensionsRequired", jsonDocument, iterator))
		{
			const rapidjson::Value& v = iterator->value;
			for (rapidjson::Value::ConstValueIterator vit = v.Begin(); vit != v.End(); ++vit)
			{
				gltfDocument.ExtensionsRequired.insert(vit->GetString());
			}
		}
	}
	/****************************************************************************
	*                       ValidateMaterial
	****************************************************************************/
	/* @fn        void ValidateMaterial(GLTFMaterial& material)
	*  @brief     Validate Material
	*  @param[in,out] GLTFMaterial material
	*  @return 　　void
	*****************************************************************************/
	void ValidateMaterial(GLTFMaterial& material)
	{
		if (material.OcclusionTexture.Strength > 1 || material.OcclusionTexture.Strength < 0)
		{
			throw error::InvalidGLTFException("Material" + material.Name + "has invalid occulusionStrength (value out of range [0,1])");

		}
	}
	/****************************************************************************
	*                       ParseMaterial
	****************************************************************************/
	/* @fn        GLTFMaterial ParseMaterial(const rapidjson::Value& value, const ExtensionDeserializer& extensionDeserializer)
	*  @brief     Parse Material
	*  @param[in] rapidjson::Value& value
	*  @param[in] ExtensionDeserializer& extensionDeserializer
	*  @return 　　GLTFMaterial
	*****************************************************************************/
	GLTFMaterial ParseMaterial(const rapidjson::Value& value, const ExtensionDeserializer& extensionDeserializer)
	{
		GLTFMaterial material;

		auto materialIterator = value.FindMember("pbrMetallicRoughness");
		if (materialIterator != value.MemberEnd())
		{
			auto& pbrMaterial = materialIterator->value;

			/*-------------------------------------------------------------------
			-                        Diffuse
			---------------------------------------------------------------------*/
			auto baseColorFactorIterator = pbrMaterial.FindMember("baseColorFactor");
			if (baseColorFactorIterator != pbrMaterial.MemberEnd())
			{
				std::vector<float> baseColorFactor;
				for (rapidjson::Value::ConstValueIterator vit = baseColorFactorIterator->value.Begin(); vit != baseColorFactorIterator->value.End(); ++vit)
				{
					baseColorFactor.push_back(static_cast<float>(vit->GetDouble()));
				}
				material.MetallicRoughness.BaseColorFactor = Color4(baseColorFactor[0], baseColorFactor[1], baseColorFactor[2], baseColorFactor[3]);
			}

			/*-------------------------------------------------------------------
			-                      Color Texture Name
			---------------------------------------------------------------------*/
			auto baseColorTextureIterator = pbrMaterial.FindMember("baseColorTexture");
			if (baseColorTextureIterator != pbrMaterial.MemberEnd())
			{
				ParseTextureInfo(baseColorTextureIterator->value, material.MetallicRoughness.BaseColorTexture, extensionDeserializer);
			}

			material.MetallicRoughness.MetallicFactor  = GetMemberValueOrDefault<float>(pbrMaterial, "metallicFactor", 1.0f);
			material.MetallicRoughness.RoughnessFactor = GetMemberValueOrDefault<float>(pbrMaterial, "roughnessFactor", 1.0f);

			/*-------------------------------------------------------------------
			-                        Metallic and Roughness map
			---------------------------------------------------------------------*/
			auto metallicRoughnessTextureIterator = pbrMaterial.FindMember("metallicRoughnessTexture");
			if (metallicRoughnessTextureIterator != pbrMaterial.MemberEnd())
			{
				ParseTextureInfo(metallicRoughnessTextureIterator->value, material.MetallicRoughness.MetallicRoughnessTexture, extensionDeserializer);
			}

		}

		/*-------------------------------------------------------------------
		-                        Normal map
		---------------------------------------------------------------------*/
		auto normalTextureIterator = value.FindMember("normalTexture");
		if (normalTextureIterator != value.MemberEnd())
		{
			ParseTextureInfo(normalTextureIterator->value, material.NormalTexture, extensionDeserializer);
			material.NormalTexture.Scale = GetMemberValueOrDefault<float>(normalTextureIterator->value, "scale", 1.0f);
		}

		/*-------------------------------------------------------------------
		-                        Occulusion map
		---------------------------------------------------------------------*/
		auto occlusionTextureIterator = value.FindMember("occlusionTexture");
		if (occlusionTextureIterator != value.MemberEnd())
		{
			ParseTextureInfo(occlusionTextureIterator->value, material.OcclusionTexture, extensionDeserializer);
			material.OcclusionTexture.Strength = GetMemberValueOrDefault<float>(occlusionTextureIterator->value, "strength", 1.0f);
		}

		/*-------------------------------------------------------------------
		-                        Emissive map
		---------------------------------------------------------------------*/
		auto emissionTextureIterator = value.FindMember("emissiveTexture");
		if (emissionTextureIterator != value.MemberEnd())
		{
			ParseTextureInfo(emissionTextureIterator->value, material.EmissiveTexture, extensionDeserializer);
		}

		auto emissionFactorIterator = value.FindMember("emissiveFactor");
		if (emissionFactorIterator != value.MemberEnd())
		{
			std::vector<float> emissiveFactor;
			for (rapidjson::Value::ConstValueIterator vit = emissionFactorIterator->value.Begin(); vit != emissionFactorIterator->value.End(); ++vit)
			{
				emissiveFactor.push_back(static_cast<float>(vit->GetDouble()));
			}
			material.EmissiveFactor = Color3(emissiveFactor[0], emissiveFactor[1], emissiveFactor[2]);
		}

		/*-------------------------------------------------------------------
		-                        Alpha mode
		---------------------------------------------------------------------*/
		auto alphaModeIterator = value.FindMember("alphaMode");
		if (alphaModeIterator != value.MemberEnd())
		{
			material.AlphaMode = ParseAlphaMode(alphaModeIterator->value.GetString());
		}

		/*-------------------------------------------------------------------
		-                        Alpha Cutoff
		---------------------------------------------------------------------*/
		material.AlphaCutoff = GetMemberValueOrDefault<float>(value, "alphaCutoff", 0.5f);
		/*-------------------------------------------------------------------
		-                        Name
		---------------------------------------------------------------------*/
		material.Name = GetMemberValueOrDefault<float>(value, "name");
		/*-------------------------------------------------------------------
		-                        Double Sided
		---------------------------------------------------------------------*/
		material.DoubleSided = GetMemberValueOrDefault<bool>(value, "doubleSided", false);
		ParseProperty(value, material, extensionDeserializer);
		ValidateMaterial(material);

		return material;
	}
	/****************************************************************************
	*                       ParseTexture
	****************************************************************************/
	/* @fn        GLTFTexture ParseTexture(const rapidjson::Value& value, const ExtensionDeserializer& extensionDeserializer)
	*  @brief     Parse Texture
	*  @param[in] rapidjson::Value& value
	*  @param[in] ExtensionDeserializer& extensionDeserializer
	*  @return 　　GLTFTexture
	*****************************************************************************/
	GLTFTexture ParseTexture(const rapidjson::Value& value, const ExtensionDeserializer& extensionDeserializer)
	{
		GLTFTexture texture;
		texture.Name      = GetMemberValueOrDefault<std::string>(value, "name");
		texture.ImageID   = GetMemberValueAsString<uint32_t>(value, "source");
		texture.SamplerID = GetMemberValueAsString<uint32_t>(value, "sampler");
		ParseProperty(value, texture, extensionDeserializer);
		return texture;
	}
	/****************************************************************************
	*                       ParseImage
	****************************************************************************/
	/* @fn        GLTFImage ParseImage(const rapidjson::Value& value, const ExtensionDeserializer& extensionDeserializer)
	*  @brief     Parse Image
	*  @param[in] rapidjson::Value& value
	*  @param[in] ExtensionDeserializer& extensionDeserializer
	*  @return 　　GLTFImage
	*****************************************************************************/
	GLTFImage ParseImage(const rapidjson::Value& value, const ExtensionDeserializer& extensionDeserializer)
	{
		GLTFImage image;
		image.Name         = GetMemberValueOrDefault<std::string>(value, "name");
		image.URI          = GetMemberValueOrDefault<std::string>(value, "uri");
		image.BufferViewId = GetMemberValueAsString<uint32_t>(value, "bufferView");
		image.MimeType     = GetMemberValueOrDefault<std::string>(value, "mineType");
		ParseProperty(value, image, extensionDeserializer);
		return image;
	}
	/****************************************************************************
	*                       DeserializerInternal
	****************************************************************************/
	/* @fn        GLTFDocument DeserializerInternal(const rapidjson::Document& document, const ExtensionDeserializer& extensionDeserializer)
	*  @brief     Deserialize
	*  @param[in] rapidjson::Document& document
	*  @param[in] ExtensionDeserializer& extensionDeserializer
	*  @return 　　GLTFDocument
	*****************************************************************************/
	GLTFDocument DeserializerInternal(const rapidjson::Document& document, const ExtensionDeserializer& extensionDeserializer, define::SchemaFlags schemaFlags)
	{
		ValidateDocumentAgainstSchema(document, define::SCHEMA_URI_GLTF, GetDefaultSchemaLocator(schemaFlags));
		GLTFDocument gltfDocument;

		rapidjson::Value::ConstMemberIterator iterator;
		if (TryFindMember("asset", document, iterator))
		{
			gltfDocument.Asset = ParseAsset(iterator->value, extensionDeserializer);
		}

		gltfDocument.Accessors   = DeserializeToIndexedContainer<GLTFAccessor>  ("accessors"  , document, extensionDeserializer, ParseAccessor);
        gltfDocument.Animations  = DeserializeToIndexedContainer<GLTFAnimation> ("animations" , document, extensionDeserializer, ParseAnimation);
        gltfDocument.Buffers     = DeserializeToIndexedContainer<GLTFBuffer>    ("buffers"    , document, extensionDeserializer, ParseBuffer);
        gltfDocument.BufferViews = DeserializeToIndexedContainer<GLTFBufferView>("bufferViews", document, extensionDeserializer, ParseBufferView);
        gltfDocument.Cameras     = DeserializeToIndexedContainer<GLTFCamera>    ("cameras"    , document, extensionDeserializer, ParseCamera);
        gltfDocument.Images      = DeserializeToIndexedContainer<GLTFImage>     ("images"     , document, extensionDeserializer, ParseImage);
        gltfDocument.Materials   = DeserializeToIndexedContainer<GLTFMaterial>  ("materials"  , document, extensionDeserializer, ParseMaterial);
        gltfDocument.Meshes      = DeserializeToIndexedContainer<GLTFMesh>      ("meshes"     , document, extensionDeserializer, ParseMesh);
        gltfDocument.Nodes       = DeserializeToIndexedContainer<GLTFNode>      ("nodes"      , document, extensionDeserializer, ParseNode);
        gltfDocument.Samplers    = DeserializeToIndexedContainer<GLTFSampler>   ("samplers"   , document, extensionDeserializer, ParseSampler);
        gltfDocument.Scenes      = DeserializeToIndexedContainer<GLTFScene>     ("scenes"     , document, extensionDeserializer, ParseScene);
        gltfDocument.Skins       = DeserializeToIndexedContainer<GLTFSkin>      ("skins"      , document, extensionDeserializer, ParseSkin);
        gltfDocument.Textures    = DeserializeToIndexedContainer<GLTFTexture>   ("textures"   , document, extensionDeserializer, ParseTexture);

		ParseProperty(document, gltfDocument, extensionDeserializer);

		if (TryFindMember("scene", document, iterator))
		{
			gltfDocument.DefaultSceneID = std::to_string(iterator->value.GetUint());
		}
		ParseExtesisonUsed(document, gltfDocument);
		ParseExtensionRequired(document, gltfDocument);
		return gltfDocument;
	}
	bool HasFlag(DeserializeFlags flags, DeserializeFlags flag)
	{
		return ((flags & flag) == flag);
	}
}
gltf::GLTFDocument gltf::Deserialize(const std::string& json, gltf::DeserializeFlags flag, gltf::detail::define::SchemaFlags schemaFlags)
{
	return Deserialize(json, ExtensionDeserializer(), flag, schemaFlags);
}
gltf::GLTFDocument gltf::Deserialize(const std::string& json, const gltf::ExtensionDeserializer& extensionDeserializer, gltf::DeserializeFlags flag, detail::define::SchemaFlags schemaFlags)
{
	const rapidjson::Document document = HasFlag(flag, DeserializeFlags::IgnoreByteOrderMark) ?
		RapidJsonUtils::CreateDocumentFromEncodedString(json) :
		RapidJsonUtils::CreateDocumentFromString(json);
	return DeserializerInternal(document, extensionDeserializer, schemaFlags);
}
gltf::GLTFDocument gltf::Deserialize(std::istream& jsonStream, gltf::DeserializeFlags flags, gltf::detail::define::SchemaFlags schemaFlags)
{
	return Deserialize(jsonStream, ExtensionDeserializer(), flags, schemaFlags);
}
gltf::GLTFDocument gltf::Deserialize(std::istream& jsonStream, const gltf::ExtensionDeserializer& extensions, gltf::DeserializeFlags flags, gltf::detail::define::SchemaFlags schemaFlags)
{
	const rapidjson::Document document = HasFlag(flags, DeserializeFlags::IgnoreByteOrderMark) ?
		RapidJsonUtils::CreateDocumentFromEncodedStream(jsonStream) :
		RapidJsonUtils::CreateDocumentFromStream(jsonStream);
	return DeserializerInternal(document, extensions, schemaFlags);
}

gltf::DeserializeFlags  gltf::operator| (gltf::DeserializeFlags lhs, gltf::DeserializeFlags rhs)
{
	const auto result =
		static_cast<std::underlying_type_t<DeserializeFlags>>(lhs) |
		static_cast<std::underlying_type_t<DeserializeFlags>>(rhs);

	return static_cast<DeserializeFlags>(result);
}
gltf::DeserializeFlags& gltf::operator|=(gltf::DeserializeFlags& lhs, gltf::DeserializeFlags rhs)
{
	lhs = lhs | rhs;
	return lhs;
}
gltf::DeserializeFlags  gltf::operator& (gltf::DeserializeFlags lhs, gltf::DeserializeFlags rhs)
{
	const auto result =
		static_cast<std::underlying_type_t<DeserializeFlags>>(lhs) &
		static_cast<std::underlying_type_t<DeserializeFlags>>(rhs);

	return static_cast<DeserializeFlags>(result);
}
gltf::DeserializeFlags& gltf::operator&=(gltf::DeserializeFlags& lhs, gltf::DeserializeFlags rhs)
{
	lhs = lhs & rhs;
	return lhs;
}