//////////////////////////////////////////////////////////////////////////////////
///  @file   PMXDefines.cpp
///  @brief  temp
///  @author toide
///  @date   2024/05/31 0:33:56
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/MMD/Private/Include/PMXDefines.hpp"
#include "Platform/Core/Include/CoreFileSystem.hpp"
#include "GameUtility/Base/Include/GUStringConverter.hpp"
#include "GameUtility/Base/Include/GUCharacterCodeConverter.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gc::file::pmx;
using namespace platform::core::file;
using namespace gu;

namespace
{
	int32 ReadIndex(const gu::SharedPointer<IFileHandle>& fileHandle, const uint32 size)
	{
		switch (size)
		{
			case 1:
			{
				gu::int8 value = 0;
				fileHandle->Read(&value, sizeof(gu::int8));
				return static_cast<int32>(value);
			}
			case 2:
			{
				gu::int16 value = 0;
				fileHandle->Read(&value, sizeof(gu::int16));
				return static_cast<int32>(value);
			}
			case 4:
			{
				gu::int32 value = 0;
				fileHandle->Read(&value, sizeof(gu::int32));
				return value;
			}
			default:
			{
				throw "Invalid index size.";
			}
		}
	}

	gu::tstring ReadString(const gu::SharedPointer<IFileHandle>& fileHandle,const PMXEncode encode)
	{
		int32 bufferSize = 0;
		fileHandle->Read(&bufferSize, sizeof(int32));

		if (bufferSize == 0)
		{
			return SP("");
		}

		/*-------------------------------------------------------------------
		-             UTF16のときはそのままTStringに変換
		---------------------------------------------------------------------*/
		if (encode == PMXEncode::UTF16)
		{
			gu::DynamicArray<gu::uint8> tempBuffer(bufferSize);

			fileHandle->Read(tempBuffer.Data(), bufferSize);

			CharacterCodeConverter converter(CharacterCodeType::UTF16, CharacterCodeType::ANSI);
			auto& utf8Buffer = converter.Convert(tempBuffer.Data(), tempBuffer.Size());
			string utf8String(reinterpret_cast<char*>(utf8Buffer.Data()), utf8Buffer.Size());
			
			return StringConverter::ConvertStringToTString(utf8String);
		}
		/*-------------------------------------------------------------------
		-             UTF8のときはそのままTStringに変換
		---------------------------------------------------------------------*/
		else
		{
			// 文字列が無駄にバッファを使うので
			gu::DynamicArray<char> byteArray(bufferSize);

			fileHandle->Read(byteArray.Data(), sizeof(char) * bufferSize);

			// 文字列メモリ負荷対策
			gu::string result(byteArray.Data());

			return StringConverter::ConvertStringToTString(result);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor

#pragma endregion Constructor and Destructor

#pragma region Public Function
/*!**********************************************************************
*  @brief     正しいファイルヘッダになっているか (Pmx というヘッダ名になっているか)
*  @param[in] void
*  @return    bool : Pmx というヘッダ名になっている場合にtrueを返します.
*************************************************************************/
bool PMXHeader::IsValid() const
{
	return Signature[0] == 'P' && Signature[1] == 'M' && Signature[2] == 'X' && Signature[3] == ' ' 
		&& (Version >= 2.0f);
}

/*!**********************************************************************
*  @brief     pmxファイルをもとにデータ読み込み
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @return    void
*************************************************************************/
void PMXHeader::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle)
{
	fileHandle->Read(Signature, sizeof(char) * _countof(Signature));
	fileHandle->Read(&Version, sizeof(float32));

	if(!IsValid())
	{
		throw "Invalid PMX file.";
	}

	fileHandle->Read(&SubsequentDataSize, sizeof(uint8));
	if (SubsequentDataSize >= 1) { fileHandle->Read(&Encode           , sizeof(uint8)); }
	if (SubsequentDataSize >= 2) { fileHandle->Read(&AdditionalUVCount, sizeof(uint8)); }
	if (SubsequentDataSize >= 3) { fileHandle->Read(&VertexIndexSize  , sizeof(uint8)); }
	if (SubsequentDataSize >= 4) { fileHandle->Read(&TextureIndexSize , sizeof(uint8)); }
	if (SubsequentDataSize >= 5) { fileHandle->Read(&MaterialIndexSize, sizeof(uint8)); }
	if (SubsequentDataSize >= 6) { fileHandle->Read(&BoneIndexSize    , sizeof(uint8)); }
	if (SubsequentDataSize >= 7) { fileHandle->Read(&MorphIndexSize   , sizeof(uint8)); }
	if (SubsequentDataSize >= 8) { fileHandle->Read(&RigidBodyIndexSize, sizeof(uint8)); }
	
	// 8バイト以降のデータを読み込む
	if (SubsequentDataSize > 8)
	{
		OtherData.Resize(static_cast<uint64>(SubsequentDataSize) - 8);
		fileHandle->Read(OtherData.Data(), OtherData.Size());
	}

	NameJapanese    = ReadString(fileHandle, Encode);
	NameEnglish     = ReadString(fileHandle, Encode);
	CommentJapanese = ReadString(fileHandle, Encode);
	CommentEnglish  = ReadString(fileHandle, Encode);
}

/*!**********************************************************************
*  @brief     pmxファイルをもとにデータ読み込み
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @param[in] const PMXHeader ヘッダ情報
*  @return    void
*************************************************************************/
void PMXVertex::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header)
{
	fileHandle->Read(&Position, sizeof(gm::Float3));
	fileHandle->Read(&Normal  , sizeof(gm::Float3));
	fileHandle->Read(&UV      , sizeof(gm::Float2));

	for (uint8 i = 0; i < header.AdditionalUVCount; i++)
	{
		fileHandle->Read(&AdditionalUV[i], sizeof(gm::Float4));
	}

	fileHandle->Read(&WeightType, sizeof(uint8));
	switch (WeightType)
	{
		/*-------------------------------------------------------------------
		-             Weight: BDEF 1 (boneIndices[0])
		---------------------------------------------------------------------*/
		case gc::file::pmx::PMXBoneWeightType::BDEF1:
		{
			BoneIndex[0] = ReadIndex(fileHandle, header.BoneIndexSize);
			break;
		}
		/*-------------------------------------------------------------------
		-             Weight: BDEF 2 (boneIndices[0-1] + boneWeights[0]  (PMD Format weight(0.0-1.0))
		---------------------------------------------------------------------*/
		case gc::file::pmx::PMXBoneWeightType::BDEF2:
		{
			BoneIndex[0] = ReadIndex(fileHandle, header.BoneIndexSize);
			BoneIndex[1] = ReadIndex(fileHandle, header.BoneIndexSize);
			fileHandle->Read(&BoneWeight, sizeof(float32));
			break;
		}
		/*-------------------------------------------------------------------
		-             Weight: BDEF 4 (boneIndices[0-3] + boneWeights[0-3] (may not weight total = 1.0))
		---------------------------------------------------------------------*/
		case gc::file::pmx::PMXBoneWeightType::BDEF4:
		{
			BoneIndex[0] = ReadIndex(fileHandle, header.BoneIndexSize);
			BoneIndex[1] = ReadIndex(fileHandle, header.BoneIndexSize);
			BoneIndex[2] = ReadIndex(fileHandle, header.BoneIndexSize);
			BoneIndex[3] = ReadIndex(fileHandle, header.BoneIndexSize);
			fileHandle->Read(&BoneWeight, sizeof(float32) * 4);
			break;
		}
		/*-------------------------------------------------------------------
		-             Weight: SDEF (boneIndices[0-1] + boneWeights[0] + SDefC + SDefR0 + SDefR1)
		---------------------------------------------------------------------*/
		case gc::file::pmx::PMXBoneWeightType::SDEF:
		{
			BoneIndex[0] = ReadIndex(fileHandle, header.BoneIndexSize);
			BoneIndex[1] = ReadIndex(fileHandle, header.BoneIndexSize);
			fileHandle->Read(&BoneWeight[0], sizeof(float32));
			fileHandle->Read(&SDEF_C, sizeof(gm::Float3));
			fileHandle->Read(&SDEF_R0, sizeof(gm::Float3));
			fileHandle->Read(&SDEF_R1, sizeof(gm::Float3));
			break;
		}
		/*-------------------------------------------------------------------
		-             Weight: QDEF (boneIndices[0-3] + boneWeights[0-3])
		---------------------------------------------------------------------*/
		case gc::file::pmx::PMXBoneWeightType::QDEF:
		{
			BoneIndex[0] = ReadIndex(fileHandle, header.BoneIndexSize);
			BoneIndex[1] = ReadIndex(fileHandle, header.BoneIndexSize);
			BoneIndex[2] = ReadIndex(fileHandle, header.BoneIndexSize);
			BoneIndex[3] = ReadIndex(fileHandle, header.BoneIndexSize);
			fileHandle->Read(&BoneWeight, sizeof(float32) * 4);
			break;
		}
		default:
		{
			throw "Invalid weight type.";
		}
	}

	fileHandle->Read(&EdgeScale, sizeof(float32));
}

/*!**********************************************************************
*  @brief     pmxファイルをもとにデータ読み込み
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @param[in] const PMXHeader ヘッダ情報
*  @return    void
*************************************************************************/
void PMXMaterial::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header)
{
	NameJapanese = ReadString(fileHandle, header.Encode);
	NameEnglish  = ReadString(fileHandle, header.Encode);

	fileHandle->Read(&Diffuse      , sizeof(gm::Float4));
	fileHandle->Read(&Specular     , sizeof(gm::Float3));
	fileHandle->Read(&SpecularPower, sizeof(float32));
	fileHandle->Read(&Ambient      , sizeof(gm::Float3));
	fileHandle->Read(&DrawMode     , sizeof(uint8));
	fileHandle->Read(&EdgeColor    , sizeof(gm::Float4));
	fileHandle->Read(&EdgeSize     , sizeof(float32));

	DiffuseTextureIndex   = ReadIndex(fileHandle, header.TextureIndexSize);
	SphereMapTextureIndex = ReadIndex(fileHandle, header.TextureIndexSize);

	fileHandle->Read(&SphereMapMode  , sizeof(uint8));
	fileHandle->Read(&ToonTextureMode, sizeof(uint8));

	switch(ToonTextureMode)
	{
		case PMXToonTextureMode::Separate:
		{
			ToonTextureIndex = ReadIndex(fileHandle, header.TextureIndexSize);
			break;
		}
		case PMXToonTextureMode::Common:
		{
			uint8 toonIndex = 0;
			fileHandle->Read(&toonIndex, sizeof(uint8));
			ToonTextureIndex = static_cast<int32>(toonIndex);
			break;
		}
	}

	Memo = ReadString(fileHandle, header.Encode); // Memo
	fileHandle->Read(&FaceCount, sizeof(int32));
}

/*!**********************************************************************
*  @brief     pmxファイルをもとにデータ読み込み
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @param[in] const PMXHeader ヘッダ情報
*  @return    void
*************************************************************************/
void PMXIKLink::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header)
{
	TargetBoneIndex = ReadIndex(fileHandle, header.BoneIndexSize);
	fileHandle->Read(&UseAngleLimit, sizeof(uint8));

	if (UseAngleLimit == 1)
	{
		fileHandle->Read(&AngleLimitLower, sizeof(gm::Float3));
		fileHandle->Read(&AngleLimitUpper, sizeof(gm::Float3));
	}
}

/*!**********************************************************************
*  @brief     pmxファイルをもとにデータ読み込み
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @param[in] const PMXHeader ヘッダ情報
*  @return    void
*************************************************************************/
void PMXBone::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header)
{
	/*-------------------------------------------------------------------
	-             Bone Name
	---------------------------------------------------------------------*/
	NameJapanese = ReadString(fileHandle, header.Encode);
	NameEnglish  = ReadString(fileHandle, header.Encode);

	/*-------------------------------------------------------------------
	-             Bone Config
	---------------------------------------------------------------------*/
	fileHandle->Read(&Position, sizeof(gm::Float3));
	ParentBoneIndex = ReadIndex(fileHandle, header.BoneIndexSize);
	fileHandle->Read(&DeformDepth, sizeof(int32));
	fileHandle->Read(&BoneFlag   , sizeof(uint16));

	/*-------------------------------------------------------------------
	-             Bone Flags
	---------------------------------------------------------------------*/
	if (gu::HasAnyFlags(BoneFlag, PMXBoneFlags::Connect))
	{
		TargetBoneIndex = ReadIndex(fileHandle, header.BoneIndexSize);
	}
	else
	{
		fileHandle->Read(&PositionOffset, sizeof(gm::Float3));
	}
	/*-------------------------------------------------------------------
	-             Bone Flag: AppendRotate , Translate
	---------------------------------------------------------------------*/
	if (gu::HasAnyFlags(BoneFlag, PMXBoneFlags::AppendRotate | PMXBoneFlags::AppendTranslate))
	{
		AppendBoneIndex = ReadIndex(fileHandle, header.BoneIndexSize);
		fileHandle->Read(&AppendWeight, sizeof(float32));
	}
	/*-------------------------------------------------------------------
	-             Bone Flag: FixedAxis
	---------------------------------------------------------------------*/
	if (gu::HasAnyFlags(BoneFlag, PMXBoneFlags::FixedAxis))
	{
		fileHandle->Read(&FixedRotationAxis, sizeof(gm::Float3));
	}
	/*-------------------------------------------------------------------
	-             Bone Flag: LocalAxis
	---------------------------------------------------------------------*/
	if (gu::HasAnyFlags(BoneFlag, PMXBoneFlags::LocalAxis))
	{
		fileHandle->Read(&LocalXAxis, sizeof(gm::Float3));
		fileHandle->Read(&LocalZAxis, sizeof(gm::Float3));
	}
	/*-------------------------------------------------------------------
	-             Bone Flag: DeformOuterParent
	---------------------------------------------------------------------*/
	if (gu::HasAnyFlags(BoneFlag, PMXBoneFlags::DeformOuterParent))
	{
		fileHandle->Read(&Key, sizeof(int32));
	}
	/*-------------------------------------------------------------------
	-             Bone Flag: IKBone
	---------------------------------------------------------------------*/
	if (gu::HasAnyFlags(BoneFlag, PMXBoneFlags::IK))
	{
		IKTargetBoneIndex = ReadIndex(fileHandle, header.BoneIndexSize);
		fileHandle->Read(&IKIteration, sizeof(int32));
		fileHandle->Read(&IKLoopAngleLimit, sizeof(float32));

		int32 ikLinkCount = 0;
		fileHandle->Read(&ikLinkCount, sizeof(int32));
		
		IKLinks.Resize(ikLinkCount);
		for (int32 i = 0; i < ikLinkCount; i++)
		{
			IKLinks[i].Read(fileHandle, header);
		}
	}
}

/*!**********************************************************************
*  @brief     pmxファイルをもとにデータ読み込み
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @param[in] const PMXHeader ヘッダ情報
*  @return    void
*************************************************************************/
void PMXPositionMorph::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header)
{
	VertexIndex = ReadIndex(fileHandle, header.VertexIndexSize);
	fileHandle->Read(&Offset, sizeof(gm::Float3));
}

/*!**********************************************************************
*  @brief     pmxファイルをもとにデータ読み込み
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @param[in] const PMXHeader ヘッダ情報
*  @return    void
*************************************************************************/
void PMXUVMorph::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header)
{
	VertexIndex = ReadIndex(fileHandle, header.VertexIndexSize);
	fileHandle->Read(&Offset, sizeof(gm::Float4));
}

/*!**********************************************************************
*  @brief     pmxファイルをもとにデータ読み込み
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @param[in] const PMXHeader ヘッダ情報
*  @return    void
*************************************************************************/
void PMXBoneMorph::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header)
{
	BoneIndex = ReadIndex(fileHandle, header.BoneIndexSize);
	fileHandle->Read(&PositionOffset, sizeof(gm::Float3));
	fileHandle->Read(&RotationOffset, sizeof(gm::Float4));
}

/*!**********************************************************************
*  @brief     pmxファイルをもとにデータ読み込み
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @param[in] const PMXHeader ヘッダ情報
*  @return    void
*************************************************************************/
void PMXMaterialMorph::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header)
{
	MaterialIndex = ReadIndex(fileHandle, header.MaterialIndexSize);
	fileHandle->Read(&OperationType         , sizeof(uint8));
	fileHandle->Read(&Diffuse               , sizeof(gm::Float4));
	fileHandle->Read(&Specular              , sizeof(gm::Float3));
	fileHandle->Read(&SpecularPower         , sizeof(float32));
	fileHandle->Read(&Ambient               , sizeof(gm::Float3));
	fileHandle->Read(&EdgeColor             , sizeof(gm::Float4));
	fileHandle->Read(&EdgeSize              , sizeof(float32));
	fileHandle->Read(&TextureCoefficient    , sizeof(gm::Float4));
	fileHandle->Read(&SphereMapCoefficient  , sizeof(gm::Float4));
	fileHandle->Read(&ToonTextureCoefficient, sizeof(gm::Float4));
}

/*!**********************************************************************
*  @brief     pmxファイルをもとにデータ読み込み
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @param[in] const PMXHeader ヘッダ情報
*  @return    void
*************************************************************************/
void PMXGroupMorph::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header)
{
	GroupIndex = ReadIndex(fileHandle, header.MorphIndexSize);
	fileHandle->Read(&Weight, sizeof(float32));
}

/*!**********************************************************************
*  @brief     pmxファイルをもとにデータ読み込み
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @param[in] const PMXHeader ヘッダ情報
*  @return    void
*************************************************************************/
void PMXFlipMorph::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header)
{
	MorphIndex = ReadIndex(fileHandle, header.MorphIndexSize);
	fileHandle->Read(&Weight, sizeof(float32));
}

/*!**********************************************************************
*  @brief     pmxファイルをもとにデータ読み込み
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @param[in] const PMXHeader ヘッダ情報
*  @return    void
*************************************************************************/
void PMXImpulseMorph::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header)
{
	RigidBodyIndex = ReadIndex(fileHandle, header.RigidBodyIndexSize);
	fileHandle->Read(&LocalFlag        , sizeof(uint8));
	fileHandle->Read(&TranslateVelocity, sizeof(gm::Float3));
	fileHandle->Read(&RotateTorque     , sizeof(gm::Float3));
}

/*!**********************************************************************
*  @brief     pmxファイルをもとにデータ読み込み
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @param[in] const PMXHeader ヘッダ情報
*  @return    void
*************************************************************************/
void PMXMorph::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header)
{
	NameJapanese = ReadString(fileHandle, header.Encode);
	NameEnglish  = ReadString(fileHandle, header.Encode);
	fileHandle->Read(&FacePart , sizeof(uint8));
	fileHandle->Read(&MorphType, sizeof(uint8));

	int32 morphCount = 0;
	fileHandle->Read(&morphCount, sizeof(int32));

	switch (MorphType)
	{
		case PMXMorphType::Group:
		{
			GroupMorphs.Resize(morphCount);
			for (int32 i = 0; i < morphCount; i++)
			{
				GroupMorphs[i].Read(fileHandle, header);
			}
			break;
		}
		case PMXMorphType::Vertex:
		{
			PositionMorphs.Resize(morphCount);
			for (int32 i = 0; i < morphCount; i++)
			{
				PositionMorphs[i].Read(fileHandle, header);
			}
			break;
		}
		case PMXMorphType::Bone:
		{
			BoneMorphs.Resize(morphCount);
			for (int32 i = 0; i < morphCount; i++)
			{
				BoneMorphs[i].Read(fileHandle, header);
			}
			break;
		}
		case PMXMorphType::UV:
		case PMXMorphType::AdditionalUV1:
		case PMXMorphType::AdditionalUV2:
		case PMXMorphType::AdditionalUV3:
		case PMXMorphType::AdditionalUV4:
		{
			UVMorphs.Resize(morphCount);
			for (int32 i = 0; i < morphCount; i++)
			{
				UVMorphs[i].Read(fileHandle, header);
			}
			break;
		}
		case PMXMorphType::Material:
		{
			MaterialMorphs.Resize(morphCount);
			for (int32 i = 0; i < morphCount; i++)
			{
				MaterialMorphs[i].Read(fileHandle, header);
			}
			break;
		}
		case PMXMorphType::Flip:
		{
			FlipMorphs.Resize(morphCount);
			for (int32 i = 0; i < morphCount; i++)
			{
				FlipMorphs[i].Read(fileHandle, header);
			}
			break;
		}
		case PMXMorphType::Impulse:
		{
			ImpulseMorphs.Resize(morphCount);
			for (int32 i = 0; i < morphCount; i++)
			{
				ImpulseMorphs[i].Read(fileHandle, header);
			}
			break;
		}
		default:
		{
			throw "Invalid morph operation type.";
		}
	}
}

/*!**********************************************************************
*  @brief     pmxファイルをもとにデータ読み込み
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @param[in] const PMXHeader ヘッダ情報
*  @return    void
*************************************************************************/
void PMXDisplayFrameElement::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header)
{
	fileHandle->Read(&DisplayFrameType, sizeof(uint8));
	switch (DisplayFrameType)
	{
		case PMXDisplayFrameType::Bone:
		{
			Index = ReadIndex(fileHandle, header.BoneIndexSize);
			break;
		}
		case PMXDisplayFrameType::Morph:
		{
			Index = ReadIndex(fileHandle, header.MorphIndexSize);
			break;
		}
		default:
		{
			throw "Invalid display frame type.";
		}
	}
}

/*!**********************************************************************
*  @brief     pmxファイルをもとにデータ読み込み
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @param[in] const PMXHeader ヘッダ情報
*  @return    void
*************************************************************************/
void PMXDisplayFrame::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header)
{
	NameJapanese = ReadString(fileHandle, header.Encode);
	NameEnglish  = ReadString(fileHandle, header.Encode);

	fileHandle->Read(&FrameFlag, sizeof(uint8));

	int32 elementCount = 0;
	fileHandle->Read(&elementCount, sizeof(int32));

	Elements.Resize(elementCount);
	for (int32 i = 0; i < elementCount; i++)
	{
		Elements[i].Read(fileHandle, header);
	}
}

/*!**********************************************************************
*  @brief     pmxファイルをもとにデータ読み込み
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @param[in] const PMXHeader ヘッダ情報
*  @return    void
*************************************************************************/
void PMXRigidBody::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header)
{
	NameJapanese = ReadString(fileHandle, header.Encode);
	NameEnglish  = ReadString(fileHandle, header.Encode);

	RelatedBoneIndex = ReadIndex(fileHandle, header.BoneIndexSize);
	fileHandle->Read(&Group           , sizeof(uint8));
	fileHandle->Read(&Mask            , sizeof(uint16));
	fileHandle->Read(&Shape           , sizeof(uint8));
	fileHandle->Read(&Size            , sizeof(gm::Float3));
	fileHandle->Read(&Position        , sizeof(gm::Float3));
	fileHandle->Read(&Rotation        , sizeof(gm::Float3));
	fileHandle->Read(&Mass            , sizeof(float32));
	fileHandle->Read(&TranslateDamping, sizeof(float32));
	fileHandle->Read(&RotationDamping , sizeof(float32));
	fileHandle->Read(&Repulsion       , sizeof(float32));
	fileHandle->Read(&Friction        , sizeof(float32));
	fileHandle->Read(&CalculationType , sizeof(uint8));
}

/*!**********************************************************************
*  @brief     pmxファイルをもとにデータ読み込み
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @param[in] const PMXHeader ヘッダ情報
*  @return    void
*************************************************************************/
void PMXJoint::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header)
{
	NameJapanese = ReadString(fileHandle, header.Encode);
	NameEnglish  = ReadString(fileHandle, header.Encode);

	fileHandle->Read(&JointType                , sizeof(uint8));
	RigidBodyIndexA = ReadIndex(fileHandle, header.RigidBodyIndexSize);
	RigidBodyIndexB = ReadIndex(fileHandle, header.RigidBodyIndexSize);
	fileHandle->Read(&Position                 , sizeof(gm::Float3));
	fileHandle->Read(&Rotation                 , sizeof(gm::Float3));
	fileHandle->Read(&TranslationLimitLower    , sizeof(gm::Float3));
	fileHandle->Read(&TranslationLimitUpper    , sizeof(gm::Float3));
	fileHandle->Read(&RotationLimitLower       , sizeof(gm::Float3));
	fileHandle->Read(&RotationLimitUpper       , sizeof(gm::Float3));
	fileHandle->Read(&TranslationSpringConstant, sizeof(gm::Float3));
	fileHandle->Read(&RotationSpringConstant   , sizeof(gm::Float3));
}

/*!**********************************************************************
*  @brief     pmxファイルをもとにデータ読み込み
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @param[in] const PMXHeader ヘッダ情報
*  @return    void
*************************************************************************/
void PMXAnchorRigidBody::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header)
{
	RigidBodyIndex = ReadIndex(fileHandle, header.RigidBodyIndexSize);
	VertexIndex	   = ReadIndex(fileHandle, header.VertexIndexSize);
	uint8 isNearMode = 0;
	fileHandle->Read(&isNearMode, sizeof(uint8));
	IsNearMode = isNearMode != 0;
}

/*!**********************************************************************
*  @brief     pmxファイルをもとにデータ読み込み
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @return    void
*************************************************************************/
void PMXSoftBodyConfig::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle)
{
	fileHandle->Read(&VelocityCorrection      , sizeof(float32));
	fileHandle->Read(&DampingCoefficient      , sizeof(float32));
	fileHandle->Read(&DragCoefficient         , sizeof(float32));
	fileHandle->Read(&LiftCoefficient         , sizeof(float32));
	fileHandle->Read(&PressureCoefficient     , sizeof(float32));
	fileHandle->Read(&VolumeConversation      , sizeof(float32));
	fileHandle->Read(&DynamicFriction         , sizeof(float32));
	fileHandle->Read(&PoseMatchingCoefficient , sizeof(float32));
	fileHandle->Read(&RigidBodyContactHardness, sizeof(float32));
	fileHandle->Read(&KineticContactHardness  , sizeof(float32));
	fileHandle->Read(&SoftContactHardness     , sizeof(float32));
	fileHandle->Read(&AnchorHardness		  , sizeof(float32));
}

/*!**********************************************************************
*  @brief     pmxファイルをもとにデータ読み込み
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @param[in] const PMXHeader ヘッダ情報
*  @return    void
*************************************************************************/
void PMXSoftBodyCluster::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle)
{
	fileHandle->Read(&SoftVsRigidBodyHardness       , sizeof(float32));
	fileHandle->Read(&SoftVsKineticHardness         , sizeof(float32));
	fileHandle->Read(&SoftVsSoftHardness            , sizeof(float32));
	fileHandle->Read(&SoftVsRigidBodyImpulseSplit   , sizeof(float32));
	fileHandle->Read(&SoftVsKineticImpulseSplit     , sizeof(float32));
	fileHandle->Read(&SoftVsSoftImpulseSplit        , sizeof(float32));
}

/*!**********************************************************************
*  @brief     pmxファイルをもとにデータ読み込み
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @param[in] const PMXHeader ヘッダ情報
*  @return    void
*************************************************************************/
void PMXSoftBodyIteration::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle)
{
	fileHandle->Read(&VelocitySolver, sizeof(int32));
	fileHandle->Read(&PositionSolver, sizeof(int32));
	fileHandle->Read(&DriftSolver   , sizeof(int32));
	fileHandle->Read(&ClusterSolver , sizeof(int32));
}

/*!**********************************************************************
*  @brief     pmxファイルをもとにデータ読み込み
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @return    void
*************************************************************************/
void PMXSoftBodyMaterial::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle)
{
	fileHandle->Read(&LinearStiffness , sizeof(float32));
	fileHandle->Read(&AngularStiffness, sizeof(float32));
	fileHandle->Read(&VolumeStiffness , sizeof(float32));
}

/*!**********************************************************************
*  @brief     pmxファイルをもとにデータ読み込み
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
*  @param[in] const PMXHeader ヘッダ情報
*  @return    void
*************************************************************************/
void PMXSoftBody::Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header)
{
	NameJapanese = ReadString(fileHandle, header.Encode);
	NameEnglish  = ReadString(fileHandle, header.Encode);

	fileHandle->Read(&Shape, sizeof(uint8));
	MaterialIndex = ReadIndex(fileHandle, header.MaterialIndexSize);
	fileHandle->Read(&Group              , sizeof(uint8));
	fileHandle->Read(&NonCollideGroupFlag, sizeof(uint16));
	fileHandle->Read(&Flag               , sizeof(uint8));
	fileHandle->Read(&BLinkDistance      , sizeof(int32));
	fileHandle->Read(&ClusterCount       , sizeof(int32));
	fileHandle->Read(&TotalMass          , sizeof(float32));
	fileHandle->Read(&CollisionMargin    , sizeof(float32));
	fileHandle->Read(&AeroModel          , sizeof(uint8));
	
	Config   .Read(fileHandle);
	Cluster  .Read(fileHandle);
	Iteration.Read(fileHandle);
	Material .Read(fileHandle);

	int32 anchorCount = 0;
	fileHandle->Read(&anchorCount, sizeof(int32));

	if (anchorCount == 0)
	{
		return;
	}

	AnchorRigidBodies.Resize(anchorCount);
	for (int32 i = 0; i < anchorCount; i++)
	{
		AnchorRigidBodies[i].Read(fileHandle, header);
	}

	int32 vertexCount = 0;
	fileHandle->Read(&vertexCount, sizeof(int32));

	if (vertexCount == 0)
	{
		return;
	}

	VertexIndices.Resize(vertexCount);
	for(int32 i = 0; i < vertexCount; i++)
	{
		VertexIndices[i] = ReadIndex(fileHandle, header.VertexIndexSize);
	}
}
#pragma endregion Public Function