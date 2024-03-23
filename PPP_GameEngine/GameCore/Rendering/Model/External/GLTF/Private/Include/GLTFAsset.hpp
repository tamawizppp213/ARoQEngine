//////////////////////////////////////////////////////////////////////////////////
///             @file   GLTFAsset.hpp
///             @brief  Asset 
///             @author 
///             @date   2022_05_12  
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GLTF_ASSET_HPP
#define GLTF_ASSET_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFProperty.hpp"
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFOptional.hpp"
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFIndexedContainer.hpp"
#include "GameUtility/Math/Include/GMMatrix.hpp"
#include "GLTFColor.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gltf
{
	namespace detail
	{
		namespace asset
		{
#pragma region AssetType
			enum BufferViewTarget
			{
				Array_Buffer = 34962,
				Element_Array_Buffer = 34963
			};
			enum AccessorType
			{
				Type_Unknown = 0,
				Type_Scalar,
				Type_Vec2,
				Type_Vec3,
				Type_Vec4,
				Type_Mat2,
				Type_Mat3,
				Type_Mat4,
			};
			enum ComponentType
			{
				Component_Unknown        = 0,
				Component_Byte           = 5120,
				Component_Unsigned_Byte  = 5121,
				Component_Short          = 5122,
				Component_Unsigned_Short = 5123,
				Component_Unsigned_Int   = 5125,
				Component_Float          = 5126
			};
			const std::unordered_map<std::string, AccessorType> AccessorTypes =
			{
				{ define::TYPE_NAME_SCALAR, AccessorType::Type_Scalar },
				{ define::TYPE_NAME_VEC2,   AccessorType::Type_Vec2 },
				{ define::TYPE_NAME_VEC3,   AccessorType::Type_Vec3 },
				{ define::TYPE_NAME_VEC4,   AccessorType::Type_Vec4 },
				{ define::TYPE_NAME_MAT2,   AccessorType::Type_Mat2 },
				{ define::TYPE_NAME_MAT3,   AccessorType::Type_Mat3 },
				{ define::TYPE_NAME_MAT4,   AccessorType::Type_Mat4}
			};
			enum GLTFMeshMode
			{
				Points         = 0,
				Lines          = 1,
				Line_Loop      = 2,
				Line_Strip     = 3,
				Triangles      = 4,
				Triangle_Strip = 5,
				Triangle_Fan   = 6
			};
			enum GLTFAlphaMode
			{
				Alpha_Unknown = 0,
				Opaque  = 1,
				Blend   = 2,
				Mask    = 3
			};
			enum TargetPath
			{
				Target_Unknown     = 0,
				Translation = 1,
				Rotation    = 2,
				Scale       = 3,
				Weight      = 4
			};
			enum InterpolationType
			{
				Interpolation_Unknown     = 0,
				Interpolation_Linear      = 1,
				Interpolation_Step        = 2,
				Interpolation_CubicSpline = 3
			};
			enum TransformationType
			{
				Indentity = 0,
				Matrix    = 1,
				TRS       = 2, // ç∂éËånÇ…ÇµÇƒSRTÇ≈Ç‚ÇËÇΩÇ¢ 
			};
			enum ProjectionType
			{
				Projection_Perspective,
				Projection_Orthographic
			};
			enum TextureType
			{
				BaseColor,
				MetallicRoughness,
				Normal,
				Occlusion,
				Emissive
			};
			enum MagFilterMode
			{
				MagFilter_Nearest = 9728,
				MagFilter_Linear  = 9729
			};
			enum MinFilterMode
			{
				MinFilter_Nearest = 9728,
				MinFilter_Linear = 9729,
				MinFilter_Nearest_Mipmap_Nearest = 9984,
				MinFilter_Linear_Mipmap_Nearset  = 9985,
				MinFilter_Nearest_Mipmap_Linear = 9986,
				MinFilter_Linear_Mipmap_Linear  = 9987
			};
			enum WrapMode
			{
				Repeat = 10497,
				Clamp  = 33071,
				Mirrored_Repeat = 33648
			};

			inline GLTFAlphaMode  ParseAlphaMode(const std::string& alphaMode)
			{
				if (alphaMode == define::ALPHAMODE_NAME_OPAQUE) { return GLTFAlphaMode::Opaque; }
				if (alphaMode == define::ALPHAMODE_NAME_BLEND)  { return GLTFAlphaMode::Blend; }
				if (alphaMode == define::ALPHAMODE_NAME_MASK)   { return GLTFAlphaMode::Mask;}
				return GLTFAlphaMode::Alpha_Unknown;
			}
			inline TargetPath ParseTargetPath(const std::string& targetPath)
			{
				if (targetPath == define::TARGETPATH_NAME_TRANSLATION) { return TargetPath::Translation; }
				if (targetPath == define::TARGETPATH_NAME_ROTATION) { return TargetPath::Rotation; }
				if (targetPath == define::TARGETPATH_NAME_SCALE)    { return TargetPath::Scale; }
				if (targetPath == define::TARGETPATH_NAME_WEIGHTS)  { return TargetPath::Weight; }
				return TargetPath::Target_Unknown;
			}
			inline InterpolationType ParseInterpolationType(const std::string& interpolationType)
			{
				if (interpolationType == define::INTERPOLATIONTYPE_NAME_LINEAR)      { return InterpolationType::Interpolation_Linear; }
				if (interpolationType == define::INTERPOLATIONTYPE_NAME_STEP)        { return InterpolationType::Interpolation_Step; }
				if (interpolationType == define::INTERPOLATIONTYPE_NAME_CUBICSPLINE) { return InterpolationType::Interpolation_CubicSpline; }

				return InterpolationType::Interpolation_Unknown;
			}
#pragma endregion AssetType
#pragma region Asset
			/****************************************************************************
			*				  			Asset
			*************************************************************************//**
			*  @struct    Asset
			*  @brief     temp
			*****************************************************************************/
			struct GLTFAccessor : public GLTFChildOfRootProperty
			{
			public:
				/****************************************************************************
				**                Public Define Struct
				*****************************************************************************/
				struct GLTFSparse
				{
					size_t        Count = 0;                  // Number of deviating accessor values stored in the sparcse array
					std::string   IndicesBufferViewID  = "";   // The index of the buffer view 
					size_t        IndicesByteOffset    = 0;   // The offset relative to the start of the buffer view in bytes
					ComponentType IndicesComponentType = ComponentType::Component_Unknown; // The datatype of the accessor's components
					std::string   ValuesBufferViewID   = "";   // The index of the buffer view with sparse values
					size_t        ValuesByteOffset     = 0;   // The offset relatice to the start of the bufferView in bytes

					bool operator==(const GLTFSparse& rhs) const
					{
						return this->Count == rhs.Count
							&& this->IndicesBufferViewID  == rhs.IndicesBufferViewID
							&& this->IndicesComponentType == rhs.IndicesComponentType
							&& this->IndicesByteOffset    == rhs.IndicesByteOffset
							&& this->ValuesBufferViewID   == rhs.ValuesBufferViewID
							&& this->ValuesByteOffset     == rhs.ValuesByteOffset;
					}
					bool operator!=(const GLTFSparse & rhs) const { return !operator==(rhs); }
				};

				/****************************************************************************
				**                Public Function
				*****************************************************************************/
				static AccessorType ParseType(const std::string& type);
				static std::string  GetAccessorTypeName(AccessorType type);
				static uint8_t      GetTypeCount       (AccessorType type);

				static ComponentType GetComponentType(uint32_t value);
				static std::string   GetComponentTypeName(ComponentType componentType);
				static uint8_t       GetComponentTypeSize(ComponentType componentType);

				size_t GetByteLength() const { return Count * GetComponentTypeSize(ComponentDataType) * GetTypeCount(AccessorDataType); }
				/****************************************************************************
				**                Public Member Variables
				*****************************************************************************/
				std::string   BufferViewID      = ""; // The index of the buffer view
				size_t        ByteOffset        = 0U; // The offset relative to the start of the buffer view in bytes
				ComponentType ComponentDataType  = ComponentType::Component_Unknown;
				bool          IsNormalized      = false; // Specifies whether integer data values are normalized before usage
				size_t        Count             = 0U; // The number of elements referenced by this accessor
				AccessorType  AccessorDataType  = AccessorType::Type_Unknown;
				std::vector<float> MaxValue;
				std::vector<float> MinValue;
				GLTFSparse         Sparse;

				/****************************************************************************
				**                Constructor and Destructor
				*****************************************************************************/
				bool operator==(const GLTFAccessor& rhs) const;
				bool operator!=(const GLTFAccessor& rhs) const { return !operator==(rhs); }
			
			private:
				/****************************************************************************
				**                Private Function
				*****************************************************************************/

				/****************************************************************************
				**                Private Member Variables
				*****************************************************************************/
			};

			/****************************************************************************
			*				  			Asset
			*************************************************************************//**
			*  @struct    Asset
			*  @brief     Has Copyright, Generator, Version, MinVersion Info.
			*****************************************************************************/
			struct GLTFAsset : public GLTFProperty
			{
			public:
				/****************************************************************************
				**                Public Member Variables
				*****************************************************************************/
				std::string CopyRight; // A copyright message suitable for display to credit the content creator
				std::string Generator; // Tool that generated this GLTF model. Useful for debugging.
				std::string Version;   // The glTF Version in the form of <major>.<minor> that this asset targets.^[0-9]+\.[0-9]+$
				std::string MinVersion;// The minimum glTF version in the form of <major><minor> that this asset targets^[0-9]+\.[0-9]+$

				/****************************************************************************
				**                Constructor and Destructor
				*****************************************************************************/
				GLTFAsset() : Version(define::GLTF_VERSION_2_0){}
				bool operator==(const GLTFAsset& rhs) const;
				bool operator!=(const GLTFAsset& rhs) const { return !operator==(rhs); }
			};
#pragma endregion    Asset
#pragma region Buffer
			/****************************************************************************
			*				  			GLTFBufferView
			*************************************************************************//**
			*  @struct    GLTFBufferView
			*  @brief     A view into a buffer generally representing a subset of the buffer
			*****************************************************************************/
			struct GLTFBufferView : public GLTFChildOfRootProperty
			{
			public:
				/****************************************************************************
				**                Public Member Variables
				*****************************************************************************/
				std::string                BufferID;         // The index of buffer
				size_t                     ByteOffset = 0U;  // The offset into the buffer in bytes
				size_t                     ByteLength = 0U;  // THe length of the buffer view in bytes
				Optional<size_t>           ByteStride;       // The stride in bytes
				Optional<BufferViewTarget> Target;           // The hint representing the intented GPU buffer type to use with this buffer view

				/****************************************************************************
				**                Constructor and Destructor
				*****************************************************************************/
				bool operator==(const GLTFBufferView& rhs) const
				{
					return GLTFChildOfRootProperty::Equals(*this, rhs)
						&& this->BufferID   == rhs.BufferID
						&& this->ByteOffset == rhs.ByteOffset
						&& this->ByteLength == rhs.ByteLength
						&& this->ByteStride == rhs.ByteStride
						&& this->Target     == rhs.Target;
				}
				bool operator!=(const GLTFBufferView& rhs) const { return !operator==(rhs); }
			};
			/****************************************************************************
			*				  			GLTFBuffer
			*************************************************************************//**
			*  @struct    GLTFBuffer
			*  @brief     A Buffer points to binary geometry, animation, or skins
			*****************************************************************************/
			struct GLTFBuffer : public GLTFChildOfRootProperty
			{
			public:
				/****************************************************************************
				**                Public Member Variables
				*****************************************************************************/
				std::string URI;        // The URI of the buffer. Relative oath are relatiave to the current glTF asset.
				size_t ByteLength = 0U; // The length of the buffer in bytes.

				/****************************************************************************
				**                Constructor and Destructor
				*****************************************************************************/
				bool operator==(const GLTFBuffer& rhs) const
				{
					return GLTFChildOfRootProperty::Equals(*this, rhs)
						&& this->URI        == rhs.URI
						&& this->ByteLength == rhs.ByteLength;
				}
				bool operator!=(const GLTFBuffer& rhs) const { return !operator==(rhs); }
			};
#pragma endregion   Buffer
#pragma region Camera
			/****************************************************************************
			*				  			GLTFProjection
			*************************************************************************//**
			*  @struct    GLTFProjection
			*  @brief     Camera 
			*****************************************************************************/
			struct GLTFProjection : public GLTFProperty
			{
			public:
				/****************************************************************************
				**                Public Member Variables
				*****************************************************************************/
				virtual ProjectionType      GetProjectionType() const = 0;
				virtual std::unique_ptr<GLTFProjection> Clone() const = 0;
				virtual bool IsValid() const = 0;

				bool operator== (const GLTFProjection& rhs) const { return IsEqual(rhs); }
				bool operator!= (const GLTFProjection& rhs) const { return !operator==(rhs); }
				/****************************************************************************
				**                Constructor and Destructor
				*****************************************************************************/
			protected:
				/****************************************************************************
				**                Constructor and Destructor
				*****************************************************************************/
				virtual bool IsEqual(const GLTFProjection& rhs) const
				{
					return GLTFProperty::Equals(*this, rhs);
				}
			};
			/****************************************************************************
			*				  			GLTFProjection
			*************************************************************************//**
			*  @struct    GLTFOrthographic
			*  @brief     Orthographic Camera
			*****************************************************************************/
			struct GLTFOrthographic : GLTFProjection
			{
			public:
				/****************************************************************************
				**                Public Function
				*****************************************************************************/
				bool IsValid()                     const override { return (ZFar > ZNear) && (YMagnification != 0.0f) && (XMagnification != 0.0f); }
				ProjectionType GetProjectionType() const override { return ProjectionType::Projection_Orthographic; }
				std::unique_ptr<GLTFProjection> Clone() const override { return std::make_unique<GLTFOrthographic>(*this); }
				bool IsEqual(const GLTFProjection& rhs) const override;
				/****************************************************************************
				**                Public Member Variables
				*****************************************************************************/
				float XMagnification; // The floating-point horizontal magnification of the view. 
				float YMagnification; // The floating-point vertical magnification of the view.
				float ZFar;           // The floating-point distance to the far clipping plane. T
				float ZNear;          // The floating-point distance to the near clipping plane.
				/****************************************************************************
				**                Constructor and Destructor
				*****************************************************************************/
				GLTFOrthographic(float zfar, float znear, float xmag, float ymag) : ZNear(znear), XMagnification(xmag), YMagnification(ymag), ZFar(zfar) {};

			protected:
				/****************************************************************************
				**                Constructor and Destructor
				*****************************************************************************/
				
			};

			/****************************************************************************
			*				a  			GLTFProjection
			*************************************************************************//**
			*  @struct    GLTFOrthographic
			*  @brief     Orthographic Camera
			*****************************************************************************/
			struct GLTFPerspective : public GLTFProjection
			{
			public:
				/****************************************************************************
				**                Public Function
				*****************************************************************************/
				inline bool IsValid () const override             { return !ZFar.HasValue() || (ZFar.Get() > ZNear); }
				inline bool IsFinite() const                      { return ZFar.HasValue(); }
				inline bool HasCustomAspectRatio() const          { return AspectRatio.HasValue(); }
				ProjectionType      GetProjectionType() const override { return ProjectionType::Projection_Perspective; }
				std::unique_ptr<GLTFProjection> Clone() const override { return std::make_unique<GLTFPerspective>(*this); }
				bool IsEqual(const GLTFProjection& rhs) const override;
				
				/****************************************************************************
				**                Public Member Variables
				*****************************************************************************/
				Optional<float> AspectRatio;
				float YFov;
				float ZNear;
				Optional<float> ZFar;
				/****************************************************************************
				**                Constructor and Destructor
				*****************************************************************************/
				GLTFPerspective(float znear, float yfov) : ZNear(znear), AspectRatio(), YFov(yfov), ZFar(){};
				GLTFPerspective(float zfar, float znear, float aspectRatio, float yfov) : ZNear(znear), ZFar(zfar), AspectRatio(aspectRatio), YFov(yfov) {};

			protected:
				/****************************************************************************
				**                Constructor and Destructor
				*****************************************************************************/

			};
			/****************************************************************************
			*				  			GLTFProjection
			*************************************************************************//**
			*  @struct    GLTFOrthographic
			*  @brief     Orthographic Camera
			*****************************************************************************/
			struct GLTFCamera : public GLTFChildOfRootProperty
			{
			public:
				/****************************************************************************
				**                Public Function
				*****************************************************************************/
				const GLTFPerspective&  GetPerspective() const;
				const GLTFOrthographic& GetOrthographic() const;

				/****************************************************************************
				**                Public Member Variables
				*****************************************************************************/
				std::unique_ptr<GLTFProjection> Projection;
				/****************************************************************************
				**                Constructor and Destructor
				*****************************************************************************/
				explicit GLTFCamera(std::unique_ptr<GLTFProjection> projection);
				GLTFCamera(std::string id, std::string name, std::unique_ptr<GLTFProjection> projection);
				GLTFCamera(const GLTFCamera& rhs) : GLTFChildOfRootProperty(rhs), Projection(rhs.Projection->Clone()) {};

				bool operator==(const GLTFCamera& rhs) const;
				bool operator!=(const GLTFCamera& rhs) const { return !operator==(rhs); }
			protected:
				/****************************************************************************
				**                Constructor and Destructor
				*****************************************************************************/

			};
#pragma endregion   Camera
#pragma region Texture
			/****************************************************************************
			*				  			GLTFProjection
			*************************************************************************//**
			*  @struct    GLTFOrthographic
			*  @brief     Orthographic Camera
			*****************************************************************************/
			struct GLTFImage : public GLTFChildOfRootProperty
			{
			public:
				/****************************************************************************
				**                Public Function
				*****************************************************************************/

				/****************************************************************************
				**                Public Member Variables
				*****************************************************************************/
				std::string URI;           // The uri of the image
				std::string MimeType;      // The image's media type. image/png
				std::string BufferViewId;  //  The index of the bufferView that contains image (ñ{ìñÇÕint)

				/****************************************************************************
				**                Constructor and Destructor
				*****************************************************************************/
				bool operator==(const GLTFImage& rhs) const;
				bool operator!=(const GLTFImage& rhs) const { return !operator==(rhs); }
			protected:
				/****************************************************************************
				**                Constructor and Destructor
				*****************************************************************************/

			};

			/****************************************************************************
			*				  			GLTFTextureInfo
			*************************************************************************//**
			*  @struct    GLTFTextureInfo
			*  @brief     Texture infomation
			*****************************************************************************/
			struct GLTFTextureInfo : public GLTFProperty
			{
			public:
				/****************************************************************************
				**                Public Function
				*****************************************************************************/

				/****************************************************************************
				**                Public Member Variables
				*****************************************************************************/
				std::string TextureID = ""; // idÇÕñ{óàintÇæÇ™, ñºëOÇ≈ä«óù
				size_t      TexCoord = 0;

				/****************************************************************************
				**                Constructor and Destructor
				*****************************************************************************/
				bool operator==(const GLTFTextureInfo& rhs) const { return GLTFTextureInfo::Equals(*this, rhs); }
				bool operator!=(const GLTFTextureInfo& rhs) const { return !operator==(rhs); }
			protected:
				/****************************************************************************
				**                Constructor and Destructor
				*****************************************************************************/
				static bool Equals(const GLTFTextureInfo& lhs, const GLTFTextureInfo& rhs)
				{
					return lhs.TextureID == rhs.TextureID
						&& lhs.TexCoord == rhs.TexCoord
						&& GLTFProperty::Equals(lhs, rhs);
				}
			};
			/****************************************************************************
			*				  			GLTFSampler
			*************************************************************************//**
			*  @struct    GLTFSampler
			*  @brief     Sampler Mode
			*****************************************************************************/
			struct GLTFSampler : public GLTFChildOfRootProperty
			{
			public:
				/****************************************************************************
				**                Public Function
				*****************************************************************************/
				static MinFilterMode GetMinFilterMode (size_t readValue);
				static MagFilterMode GetMagFilterMode (size_t readValue);
				static WrapMode      GetAddressingMode(size_t readValue);

				/****************************************************************************
				**                Public Member Variables
				*****************************************************************************/
				Optional<MagFilterMode> MagFilter;
				Optional<MinFilterMode> MinFilter;
				WrapMode TextureAddressingU = WrapMode::Repeat;
				WrapMode TextureAddressingV = WrapMode::Repeat;

				/****************************************************************************
				**                Constructor and Destructor
				*****************************************************************************/
				bool operator==(const GLTFSampler& rhs) const;
				bool operator!=(const GLTFSampler& rhs) const { return !operator==(rhs); }
			protected:
				/****************************************************************************
				**                Constructor and Destructor
				*****************************************************************************/
				
			};
			/****************************************************************************
			*				  			GLTFTexture
			*************************************************************************//**
			*  @struct    GLTFTexture
			*  @brief     Textures that references an Image.
			*****************************************************************************/
			struct GLTFTexture : public GLTFChildOfRootProperty
			{
			public:
				/****************************************************************************
				**                Public Function
				*****************************************************************************/

				/****************************************************************************
				**                Public Member Variables
				*****************************************************************************/
				std::string SamplerID; // The index of the sampler used by this texture
				std::string ImageID;   // The index of the image by this textyre
				/****************************************************************************
				**                Constructor and Destructor
				*****************************************************************************/
				bool operator==(const GLTFTexture& rhs) const
				{
					return GLTFChildOfRootProperty::Equals(*this, rhs)
						&& this->SamplerID == rhs.SamplerID
						&& this->ImageID   == rhs.ImageID;
				}
				bool operator!=(const GLTFTexture& rhs) const { return !operator==(rhs); }
			protected:
				/****************************************************************************
				**                Constructor and Destructor
				*****************************************************************************/

			};
#pragma endregion  Texture
#pragma region Material
			/****************************************************************************
			*				  		 GLTFMaterial
			*************************************************************************//**
			*  @struct    GLTFMaterial
			*  @brief     Material
			*****************************************************************************/
			struct GLTFMaterial : public GLTFChildOfRootProperty
			{
			public:
				/****************************************************************************
				**                Define Struct 
				*****************************************************************************/
				struct PBRMetallicRoughness : GLTFProperty
				{
					Color4      BaseColorFactor;          // The factors for the base color of the material
					GLTFTextureInfo BaseColorTexture;         // The base color texture
					float           MetallicFactor;           // The factor for the metalness of the material
					float           RoughnessFactor;          // The factor for the roughness of the material
					GLTFTextureInfo MetallicRoughnessTexture; // The metallic and roughness texture
					PBRMetallicRoughness() : BaseColorFactor(1.0f, 1.0f, 1.0f, 1.0f), MetallicFactor(1.0f), RoughnessFactor(1.0f){}

					bool operator==(const PBRMetallicRoughness& rhs) const
					{
						return this->BaseColorFactor          == rhs.BaseColorFactor
							&& this->BaseColorTexture         == rhs.BaseColorTexture
							&& this->MetallicFactor           == rhs.MetallicFactor
							&& this->RoughnessFactor          == rhs.RoughnessFactor
							&& this->MetallicRoughnessTexture == rhs.MetallicRoughnessTexture;
					}
					bool operator!=(const PBRMetallicRoughness& rhs) const { return !operator==(rhs); }
				};
				struct NormalTextureInfo : public GLTFTextureInfo
				{
					float Scale = 1.0f;
					NormalTextureInfo() : Scale(1.0f){}
					bool operator==(const NormalTextureInfo& rhs) const
					{
						return GLTFTextureInfo::Equals(*this, rhs)
							&& this->Scale == rhs.Scale;
					}
					bool operator!=(const NormalTextureInfo& rhs) const { return !operator==(rhs); }
				};
				struct OcculusionTextureInfo : public GLTFTextureInfo
				{
					float Strength; // A scalar multiplier controlling the amount of occulusion
					OcculusionTextureInfo() : Strength(1.0f){}
					bool operator==(const OcculusionTextureInfo& rhs) const 
					{
						return GLTFTextureInfo::Equals(*this, rhs)
							&& this->Strength == rhs.Strength;
					}
					bool operator!=(const OcculusionTextureInfo& rhs) const { return !operator==(rhs); }
				};
				/****************************************************************************
				**                Public Function
				*****************************************************************************/
				std::vector<std::pair<std::string, TextureType>> GetTextures() const
				{
					return {
					{ MetallicRoughness.BaseColorTexture.TextureID, TextureType::BaseColor },
					{ MetallicRoughness.MetallicRoughnessTexture.TextureID, TextureType::MetallicRoughness },
					{ NormalTexture   .TextureID, TextureType::Normal },
					{ OcclusionTexture.TextureID, TextureType::Occlusion },
					{ EmissiveTexture .TextureID, TextureType::Emissive }
					};
				}
				/****************************************************************************
				**                Public Member Variables
				*****************************************************************************/
				PBRMetallicRoughness  MetallicRoughness;
				NormalTextureInfo     NormalTexture;
				OcculusionTextureInfo OcclusionTexture;
				GLTFTextureInfo       EmissiveTexture;
				Color3               EmissiveFactor;
				GLTFAlphaMode         AlphaMode;
				float                 AlphaCutoff;
				bool                  DoubleSided; // Specifies whether the material is double sided
				/****************************************************************************
				**                Constructor and Destructor
				*****************************************************************************/
				GLTFMaterial() : EmissiveFactor(0.0f, 0.0f, 0.0f), AlphaMode(GLTFAlphaMode::Opaque), AlphaCutoff(0.5f), DoubleSided(false){}
				bool operator==(const GLTFMaterial& rhs) const
				{
					return GLTFChildOfRootProperty::Equals(*this, rhs)
						&& this->MetallicRoughness == rhs.MetallicRoughness
						&& this->NormalTexture     == rhs.NormalTexture
						&& this->OcclusionTexture  == rhs.OcclusionTexture
						&& this->EmissiveTexture   == rhs.EmissiveTexture
						&& this->EmissiveFactor    == rhs.EmissiveFactor
						&& this->AlphaMode         == rhs.AlphaMode
						&& this->AlphaCutoff       == rhs.AlphaCutoff
						&& this->DoubleSided       == rhs.DoubleSided;
				}
				bool operator!=(const GLTFMaterial& rhs) const { return !operator==(rhs); }
			};
#pragma endregion Material
#pragma region Scene
			/****************************************************************************
			*				  			GLTFScene
			*************************************************************************//**
			*  @struct    GLTFScene
			*  @brief     The root nodes of a scene
			*****************************************************************************/
			struct GLTFScene : public GLTFChildOfRootProperty
			{
			public:
				/****************************************************************************
				**                Public Function
				*****************************************************************************/

				/****************************************************************************
				**                Public Member Variables
				*****************************************************************************/
				std::vector<std::string> Nodes;
				/****************************************************************************
				**                Constructor and Destructor
				*****************************************************************************/
				bool operator==(const GLTFScene& rhs) const
				{
					return GLTFChildOfRootProperty::Equals(*this, rhs)
						&& this->Nodes == rhs.Nodes;
				}
				bool operator!=(const GLTFScene& rhs) const { return !operator==(rhs); }
			};
#pragma endregion    Scene
#pragma region Skeleton
			/****************************************************************************
			*				  			GLTFMorphTarget
			*************************************************************************//**
			*  @struct    GLTFMorphTarget
			*  @brief     Morph Target
			*****************************************************************************/
			struct GLTFMorphTarget
			{
			public:
				/****************************************************************************
				**                Public Function
				*****************************************************************************/

				/****************************************************************************
				**                Public Member Variables
				*****************************************************************************/
				std::string PositionsAccessorID;
				std::string NormalsAccessorID;
				std::string TangentsAccessorID;
				/****************************************************************************
				**                Constructor and Destructor
				*****************************************************************************/
				bool operator==(const GLTFMorphTarget& rhs) const
				{
					return this->PositionsAccessorID == rhs.PositionsAccessorID
						&& this->NormalsAccessorID   == rhs.NormalsAccessorID
						&& this->TangentsAccessorID == rhs.TangentsAccessorID;
				}
				bool operator!=(const GLTFMorphTarget& rhs) const { return !operator==(rhs); }
			};

			/****************************************************************************
			*				  			GLTFMorphTarget
			*************************************************************************//**
			*  @struct    GLTFMorphTarget
			*  @brief     Morph Target
			*****************************************************************************/
			struct GLTFMeshPrimitive : public GLTFProperty
			{
			public:
				/****************************************************************************
				**                Public Function
				*****************************************************************************/
				bool HasAttribute(const std::string& name) const { return Attributes.find(name) != Attributes.end(); }
				const std::string& GetAttributeAccessorID(const std::string& name) const;
				bool TryGetAttributeAccessorID(const std::string& name, std::string& accessorID) const;
				/****************************************************************************
				**                Public Member Variables
				*****************************************************************************/
				std::unordered_map<std::string, std::string> Attributes; // A plain JSON objects where each key corresponds to a mesh attribute semantic
				std::string                  IndicesAccessorID;
				std::string                  MaterialID;
				GLTFMeshMode                 MeshMode = GLTFMeshMode::Triangles;
				std::vector<GLTFMorphTarget> Targets;
				/****************************************************************************
				**                Constructor and Destructor
				*****************************************************************************/
				bool operator==(const GLTFMeshPrimitive& rhs) const
				{
					return GLTFProperty::Equals(*this, rhs)
						&& this->Attributes       == rhs.Attributes
						&& this->IndicesAccessorID == rhs.IndicesAccessorID
						&& this->MaterialID        == rhs.MaterialID
						&& this->MeshMode          == rhs.MeshMode
						&& this->Targets           == rhs.Targets;
				}
				bool operator!=(const GLTFMeshPrimitive& rhs) const { return !operator==(rhs); }
			};

			/****************************************************************************
			*				  			GLTFMesh
			*************************************************************************//**
			*  @struct    GLTFMesh
			*  @brief     Mesh
			*****************************************************************************/
			struct GLTFMesh : public GLTFChildOfRootProperty
			{
			public:
				/****************************************************************************
				**                Public Function
				*****************************************************************************/

				/****************************************************************************
				**                Public Member Variables
				*****************************************************************************/
				std::vector<GLTFMeshPrimitive> Primitives;
				std::vector<float> Weights;
				/****************************************************************************
				**                Constructor and Destructor
				*****************************************************************************/
				bool operator==(const GLTFMesh& rhs) const
				{
					return GLTFChildOfRootProperty::Equals(*this, rhs)
						&& this->Primitives == rhs.Primitives
						&& this->Weights    == rhs.Weights;
				}
				bool operator!=(const GLTFMesh& rhs) const { return !operator==(rhs); }
			protected:
				/****************************************************************************
				**                Constructor and Destructor
				*****************************************************************************/

			};

			/****************************************************************************
			*				  			GLTFNode
			*************************************************************************//**
			*  @struct    GLTFNode
			*  @brief     Node in the node hierarchy
			*****************************************************************************/
			struct GLTFNode : public GLTFChildOfRootProperty
			{
			public:
				/****************************************************************************
				**                Public Function
				*****************************************************************************/
				bool IsEmpty() const { return Children.empty() && MeshID.empty() && SkinID.empty(); }
				bool HasValidTransformType() const { return gm::Matrix4f(Matrix) == gm::Matrix4f() || HasIdentityTRS(); }
				TransformationType GetTransformationType() const
				{
					if (gm::Matrix4f(Matrix) != gm::Matrix4f()) { return TransformationType::Matrix; }
					if (!HasIdentityTRS()) { return TransformationType::TRS; }
					return TransformationType::Indentity;
				}
				bool HasIdentityTRS() const
				{
					return Translation == gm::Float3(0, 0, 0)
						&& Rotation    == gm::Float4(0, 0, 0, 1)
						&& Scale       == gm::Float3(1, 1, 1);
				}
				/****************************************************************************
				**                Public Member Variables
				*****************************************************************************/
				std::string              CameraID; // The index of the camera referenced by this node
				std::vector<std::string> Children; // The indices of this node's children
				std::string              SkinID;   // The index of the skin referenced by this node
				gm::Float4x4             Matrix    = gm::Float4x4();   // Transformation matrix
				std::string              MeshID;
				gm::Float4               Rotation    = gm::Float4(0,0,0,1);
				gm::Float3               Scale       = gm::Float3(1,1,1);
				gm::Float3               Translation = gm::Float3(0,0,0);
				std::vector<float>       Weights;
				/****************************************************************************
				**                Constructor and Destructor
				*****************************************************************************/
				bool operator==(const GLTFNode& rhs) const
				{
					return GLTFChildOfRootProperty::Equals(*this, rhs)
						&& this->CameraID    == rhs.CameraID
						&& this->Children    == rhs.Children
						&& this->SkinID      == rhs.SkinID
						&& gm::Matrix4f(this->Matrix) == gm::Matrix4f(rhs.Matrix)
						&& this->MeshID      == rhs.MeshID
						&& this->Rotation    == rhs.Rotation
						&& this->Scale       == rhs.Scale
						&& this->Translation == rhs.Translation
						&& this->Weights == rhs.Weights;
				}
				bool operator!=(const GLTFNode& rhs) const
				{
					return !operator==(rhs);
				}
			protected:
				/****************************************************************************
				**                Constructor and Destructor
				*****************************************************************************/

			};

			/****************************************************************************
			*				  			GLTFSkin
			*************************************************************************//**
			*  @struct    GLTFSkin
			*  @brief     Joints and matrices defining a skin
			*****************************************************************************/
			struct GLTFSkin : public GLTFChildOfRootProperty
			{
			public:
				/****************************************************************************
				**                Public Function
				*****************************************************************************/

				/****************************************************************************
				**                Public Member Variables
				*****************************************************************************/
				std::string InverseBindMatricesAccessorID; // The index of the accessor containing the floating-point 4x4 inverse bind matrices
				std::string SkeletonID;            // The index of the node used as a skeleton root.
				std::vector<std::string> JointIDs; // Indices of skeleton nodes, used as joint in this skin.


				/****************************************************************************
				**                Constructor and Destructor
				*****************************************************************************/
				bool operator==(const GLTFSkin& rhs) const;
				bool operator!=(const GLTFSkin& rhs) const { return !operator==(rhs); }
			protected:
				/****************************************************************************
				**                Constructor and Destructor
				*****************************************************************************/

			};
#pragma endregion Skeleton
#pragma region Animation
			/****************************************************************************
			*				  			GLTFAnimationTarget
			*************************************************************************//**
			*  @struct    GLTFAnimationTarget
			*  @brief     Animation Target Node
			*****************************************************************************/
			struct GLTFAnimationTarget : public GLTFProperty
			{
			public:
				/****************************************************************************
				**                Public Member Variables
				*****************************************************************************/
				std::string NodeID;
				TargetPath  Path = Target_Unknown;
				/****************************************************************************
				**                Constructor and Destructor
				*****************************************************************************/
				bool operator==(const GLTFAnimationTarget& rhs) const
				{
					return GLTFProperty::Equals(*this, rhs)
						&& this->NodeID == rhs.NodeID
						&& this->Path    == rhs.Path;
				}
				bool operator!=(const GLTFAnimationTarget& rhs) const { return !operator==(rhs); }
			};
			/****************************************************************************
			*				  			GLTFAnimationChannel
			*************************************************************************//**
			*  @struct    GLTFAnimationChannel
			*  @brief     Animation Channel 
			*****************************************************************************/
			struct GLTFAnimationChannel : public GLTFProperty
			{
			public:
				/****************************************************************************
				**                Public Member Variables
				*****************************************************************************/
				std::string ID;
				std::string SampleID;
				GLTFAnimationTarget  Target;
				/****************************************************************************
				**                Constructor and Destructor
				*****************************************************************************/
				bool operator==(const GLTFAnimationChannel& rhs) const
				{
					return GLTFProperty::Equals(*this, rhs)
						&& this->ID == rhs.ID
						&& this->SampleID == rhs.SampleID
						&& this->Target   == rhs.Target;
				}
				bool operator!=(const GLTFAnimationChannel& rhs) const { return !operator==(rhs); }
			};
			/****************************************************************************
			*				  			GLTFAnimationSampler
			*************************************************************************//**
			*  @struct    GLTFAnimationSampler
			*  @brief     Animation Interpolation 
			*****************************************************************************/
			struct GLTFAnimationSampler : public GLTFProperty
			{
			public:
				/****************************************************************************
				**                Public Member Variables
				*****************************************************************************/
				std::string ID;
				std::string InputAccessorID;
				InterpolationType Interpolation = InterpolationType::Interpolation_Linear;
				std::string       OutputAccessorID;
				/****************************************************************************
				**                Constructor and Destructor
				*****************************************************************************/
				bool operator==(const GLTFAnimationSampler& rhs) const
				{
					return GLTFProperty::Equals(*this, rhs)
						&& this->ID == rhs.ID
						&& this->InputAccessorID  == rhs.InputAccessorID
						&& this->Interpolation    == rhs.Interpolation
						&& this->OutputAccessorID == rhs.OutputAccessorID;
				}
				bool operator!=(const GLTFAnimationSampler& rhs) const { return !operator==(rhs); }
			};

			/****************************************************************************
			*				  			GLTFAnimation
			*************************************************************************//**
			*  @struct    GLTFAnimation
			*  @brief     Animation
			*****************************************************************************/
			struct GLTFAnimation : public GLTFChildOfRootProperty
			{
			public:

				/****************************************************************************
				**                Public Member Variables
				*****************************************************************************/
				IndexedContainer<const GLTFAnimationChannel> Channels;
				IndexedContainer<const GLTFAnimationSampler> Samplers;

				/****************************************************************************
				**                Constructor and Destructor
				*****************************************************************************/
				bool operator==(const GLTFAnimation& rhs) const
				{
					return GLTFChildOfRootProperty::Equals(*this, rhs)
						&& this->Channels == rhs.Channels
						&& this->Samplers == rhs.Samplers;
				}
				bool operator!=(const GLTFAnimation& rhs) const { return !operator==(rhs); }
			};
#pragma endregion Animation
		}
	}

}
#endif