//////////////////////////////////////////////////////////////////////////////////
//              @file   PMXParser.cpp
///             @brief  PMX File Parser
///             @author Toide Yutaro
///             @date   2022_05_05
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/MMD/Include/PMXParser.hpp"
#include "GameUtility/File/Include/UnicodeUtility.hpp"
#include "GameUtility/File/Include/FileSystem.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace pmx;
bool ReadPMXString(FILE* filePtr, std::string* string, PMXEncode encode);
bool ReadPMXIndex(FILE* filePtr, INT32* pmxIndex, UINT8 indexSize);
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
bool PMXFile::Load(const std::wstring& filePath)
{
	/*-------------------------------------------------------------------
	-             Open File
	---------------------------------------------------------------------*/
	if (file::FileSystem::GetExtension(filePath) != L"pmx") 
	{ 
		OutputDebugStringA("pmx error: wrong extension type");
		return false; 
	};

	FILE* filePtr = file::FileSystem::OpenFile(filePath);
	if (filePtr == nullptr) 
	{ 
		OutputDebugStringA("failed to open file");
		return false; 
	}

	Directory     = file::FileSystem::GetDirectory(unicode::ToUtf8String(filePath));
	/*-------------------------------------------------------------------
	-             Read Data
	---------------------------------------------------------------------*/
	Setting   .Read(filePtr);
	Infomation.Read(filePtr, &Setting);
	ReadVertices     (filePtr);
	ReadIndices      (filePtr);
	ReadTextureList  (filePtr);
	ReadMaterials    (filePtr);
	ReadBones        (filePtr);
	ReadMorphs       (filePtr);
	ReadDisplayFrames(filePtr);
	ReadRigidBodies  (filePtr);
	ReadJoints       (filePtr);
	ReadSoftBodies   (filePtr);

	return true;
}
PMXFile::~PMXFile()
{
	Vertices       .clear(); Vertices       .shrink_to_fit();
	Indices        .clear(); Indices        .shrink_to_fit();
	TexturePathList.clear(); TexturePathList.shrink_to_fit();
	Materials      .clear(); Materials      .shrink_to_fit();
	Bones          .clear(); Bones          .shrink_to_fit();
	Morphs         .clear(); Morphs         .shrink_to_fit();
	DisplayFrames  .clear(); DisplayFrames  .shrink_to_fit();
	RigidBodies    .clear(); RigidBodies    .shrink_to_fit();
	Joints         .clear(); Joints         .shrink_to_fit();
}
#pragma region PMXFileFunction
void PMXFile::ReadVertices     (FILE* filePtr)
{
	INT32 vertexCount = 0;
	fread_s(&vertexCount, sizeof(vertexCount), sizeof(INT32), 1, filePtr);

	Vertices.resize(vertexCount);
	for (auto& vertex : Vertices) { vertex.Read(filePtr, &Setting); }
}
void PMXFile::ReadIndices      (FILE* filePtr)
{
	/*-------------------------------------------------------------------
	-             Load Face Count
	---------------------------------------------------------------------*/
	INT32 indexCount = 0;
	fread_s(&indexCount, sizeof(indexCount), sizeof(INT32), 1, filePtr);

	/*-------------------------------------------------------------------
	-             Load Face Data
	---------------------------------------------------------------------*/
	Indices.resize(indexCount);
	switch (Setting.VertexIndexSize)
	{
		case 1:
		{
			std::vector<UINT8> indices(indexCount);
			fread_s(indices.data(), sizeof(UINT8) * indices.size(), sizeof(UINT8), indices.size(), filePtr);
			for (int i = 0; i < indexCount; ++i)
			{
				Indices[i] = indices[i];
			}
			break;
		}
		case 2:
		{
			std::vector<UINT16> indices(indexCount);
			fread_s(indices.data(), sizeof(UINT16) * indices.size(), sizeof(UINT16), indices.size(), filePtr);
			for (int i = 0; i < indexCount; ++i)
			{
				Indices[i] = indices[i];
			}
			break;
		}
		case 4:
		{
			std::vector<UINT32> indices(indexCount);
			fread_s(indices.data(), sizeof(UINT32) * indices.size(), sizeof(UINT32), indices.size(), filePtr);
			Indices = std::move(indices);
			break;
		}
		default:
		{
			return;
		}
	}
}
void PMXFile::ReadTextureList  (FILE* filePtr)
{
	/*-------------------------------------------------------------------
	-             Load Texture Count
	---------------------------------------------------------------------*/
	INT32 textureCount = 0;
	fread_s(&textureCount, sizeof(textureCount), sizeof(INT32), 1, filePtr);

	/*-------------------------------------------------------------------
	-             Load Texture Data
	---------------------------------------------------------------------*/
	TexturePathList.resize(textureCount);
	for (auto& texture : TexturePathList)
	{
		ReadPMXString(filePtr, &texture, Setting.Encode);
		texture = Directory + "/" + texture;
	}
}
void PMXFile::ReadMaterials    (FILE* filePtr)
{
	/*-------------------------------------------------------------------
	-             Load Material Count
	---------------------------------------------------------------------*/
	INT32 materialCount = 0;
	fread_s(&materialCount, sizeof(materialCount), sizeof(INT32), 1, filePtr);

	/*-------------------------------------------------------------------
	-             Load Material Data
	---------------------------------------------------------------------*/
	Materials.resize(materialCount);
	for (auto& material : Materials)
	{
		material.Read(filePtr, &Setting);
	}
}
void PMXFile::ReadBones        (FILE* filePtr)
{
	/*-------------------------------------------------------------------
	-             Load Bone Count
	---------------------------------------------------------------------*/
	INT32 boneCount = 0;
	fread_s(&boneCount, sizeof(boneCount), sizeof(INT32), 1, filePtr);

	/*-------------------------------------------------------------------
	-             Load Bone Data
	---------------------------------------------------------------------*/
	Bones.resize(boneCount);
	for (auto& bone : Bones)
	{
		bone.Read(filePtr, &Setting);
	}
}
void PMXFile::ReadMorphs       (FILE* filePtr)
{
	/*-------------------------------------------------------------------
	-             Load Morph Count
	---------------------------------------------------------------------*/
	INT32 morphCount = 0;
	fread_s(&morphCount, sizeof(morphCount), sizeof(INT32), 1, filePtr);
	/*-------------------------------------------------------------------
	-             Load Morph Data
	---------------------------------------------------------------------*/
	Morphs.resize(morphCount);
	for (auto& morph : Morphs)
	{
		morph.Read(filePtr, &Setting);
	}
}
void PMXFile::ReadDisplayFrames(FILE* filePtr)
{
	/*-------------------------------------------------------------------
	-             Load display frame count
	---------------------------------------------------------------------*/
	INT32 displayFrameCount = 0;
	fread_s(&displayFrameCount, sizeof(displayFrameCount), sizeof(INT32), 1, filePtr);

	/*-------------------------------------------------------------------
	-             Load display frame data
	---------------------------------------------------------------------*/
	DisplayFrames.resize(displayFrameCount);
	for (auto& frame : DisplayFrames)
	{
		frame.Read(filePtr, &Setting);
	}
}
void PMXFile::ReadRigidBodies  (FILE* filePtr)
{
	/*-------------------------------------------------------------------
	-             Load RigidBody Count
	---------------------------------------------------------------------*/
	INT32 rigidBodyCount = 0;
	fread_s(&rigidBodyCount, sizeof(rigidBodyCount), sizeof(INT32), 1, filePtr);

	/*-------------------------------------------------------------------
	-             Load rigid body Config
	---------------------------------------------------------------------*/
	RigidBodies.resize(rigidBodyCount);
	for (auto& rigidBody : RigidBodies)
	{
		rigidBody.Read(filePtr, &Setting);
	}
}
void PMXFile::ReadJoints       (FILE* filePtr)
{
	/*-------------------------------------------------------------------
	-             Load joint count
	---------------------------------------------------------------------*/
	INT32 jointCount = 0;
	fread_s(&jointCount, sizeof(jointCount), sizeof(INT32), 1, filePtr);

	/*-------------------------------------------------------------------
	-             Load joint data
	---------------------------------------------------------------------*/
	Joints.resize(jointCount);
	for (auto& joint : Joints)
	{
		joint.Read(filePtr, &Setting);
	}
}
void PMXFile::ReadSoftBodies   (FILE* filePtr)
{
	/*-------------------------------------------------------------------
	-             Load soft body count
	---------------------------------------------------------------------*/
	INT32 softBodyCount = 0;
	fread_s(&softBodyCount, sizeof(softBodyCount), sizeof(INT32), 1, filePtr);

	/*-------------------------------------------------------------------
	-             Load Soft Body Data
	---------------------------------------------------------------------*/
	SoftBodies.resize(softBodyCount);
	for (auto& softBody : SoftBodies)
	{
		softBody.Read(filePtr, &Setting);
	}
}
#pragma endregion  PMXFileFunction
#pragma region EachReadFunction
void PMXSetting     ::Read(FILE* filePtr)
{
	fread_s(this, sizeof(PMXSetting), sizeof(PMXSetting), 1, filePtr);
}
bool PMXInfo        ::Read(FILE* filePtr, const PMXSetting* setting)
{
	if (!ReadPMXString(filePtr, &ModelName, setting->Encode))        { return false; };
	if (!ReadPMXString(filePtr, &EngliseModelName, setting->Encode)) { return false; };
	if (!ReadPMXString(filePtr, &Comment, setting->Encode))          { return false; };
	if (!ReadPMXString(filePtr, &EnglishComment, setting->Encode))   { return false; };
	return true;
}
bool PMXVertex      ::Read(FILE* filePtr, const PMXSetting* setting)
{
	/*-------------------------------------------------------------------
	-             Load Position , Normal, UV, and Add UV
	---------------------------------------------------------------------*/
	fread_s(&Position, sizeof(Position), sizeof(Float3), 1, filePtr);
	fread_s(&Normal  , sizeof(Normal  ), sizeof(Float3), 1, filePtr);
	fread_s(&UV      , sizeof(UV      ), sizeof(Float2), 1, filePtr);

	for (UINT8 i = 0; i < setting->AddUVCount; ++i)
	{
		fread_s(&AddUV[i], sizeof(AddUV[i]), sizeof(Float4), 1, filePtr);
	}

	fread_s(&WeightType, sizeof(WeightType), sizeof(UINT8), 1, filePtr);
	switch (WeightType)
	{
		/*-------------------------------------------------------------------
		-             Weight: BDEF 1 (boneIndices[0])
		---------------------------------------------------------------------*/
		case PMXVertexWeight::BDEF1:
		{
			ReadPMXIndex(filePtr, &BoneIndices[0], setting->BoneIndexSize);
			break;
		}
		/*-------------------------------------------------------------------
		-             Weight: BDEF 2 (boneIndices[0-1] + boneWeights[0]  (PMD Format weight(0.0-1.0))
		---------------------------------------------------------------------*/
		case PMXVertexWeight::BDEF2:
		{
			ReadPMXIndex(filePtr, &BoneIndices[0], setting->BoneIndexSize);
			ReadPMXIndex(filePtr, &BoneIndices[1], setting->BoneIndexSize);
			fread_s(&BoneWeights[0], sizeof(BoneWeights), sizeof(float), 1, filePtr);
			break;
		}
		/*-------------------------------------------------------------------
		-             Weight: BDEF 4 (boneIndices[0-3] + boneWeights[0-3] (may not weight total = 1.0))
		---------------------------------------------------------------------*/
		case PMXVertexWeight::BDEF4:
		{
			ReadPMXIndex(filePtr, &BoneIndices[0], setting->BoneIndexSize);
			ReadPMXIndex(filePtr, &BoneIndices[1], setting->BoneIndexSize);
			ReadPMXIndex(filePtr, &BoneIndices[2], setting->BoneIndexSize);
			ReadPMXIndex(filePtr, &BoneIndices[3], setting->BoneIndexSize);
			fread_s(&BoneWeights, sizeof(BoneWeights), sizeof(float), 4, filePtr);
			break;
		}
		/*-------------------------------------------------------------------
		-             Weight: SDEF (boneIndices[0-1] + boneWeights[0] + SDefC + SDefR0 + SDefR1)
		---------------------------------------------------------------------*/
		case PMXVertexWeight::SDEF:
		{
			ReadPMXIndex(filePtr, &BoneIndices[0], setting->BoneIndexSize);
			ReadPMXIndex(filePtr, &BoneIndices[1], setting->BoneIndexSize);
			fread_s(&BoneWeights[0], sizeof(BoneWeights[0]), sizeof(float)   , 1, filePtr);
			fread_s(&SDefC         , sizeof(SDefC)         , sizeof(Float3), 1, filePtr);
			fread_s(&SDefR0        , sizeof(SDefR0)        , sizeof(Float3), 1, filePtr);
			fread_s(&SDefR1        , sizeof(SDefR1)        , sizeof(Float3), 1, filePtr);
			break;
		}
		/*-------------------------------------------------------------------
		-             Weight: QDEF (boneIndices[0-3] + boneWeights[0-3])
		---------------------------------------------------------------------*/
		case PMXVertexWeight::QDEF:
		{
			ReadPMXIndex(filePtr, &BoneIndices[0], setting->BoneIndexSize);
			ReadPMXIndex(filePtr, &BoneIndices[1], setting->BoneIndexSize);
			ReadPMXIndex(filePtr, &BoneIndices[2], setting->BoneIndexSize);
			ReadPMXIndex(filePtr, &BoneIndices[3], setting->BoneIndexSize);
			fread_s(BoneWeights, sizeof(BoneWeights), sizeof(float), 4, filePtr);
			break;
		}
		default:
		{
			return false;
		}
	}
	fread_s(&EdgeMagnitude, sizeof(EdgeMagnitude), sizeof(float), 1, filePtr);
	return true;
}
void PMXMaterial    ::Read(FILE* filePtr, const PMXSetting* setting)
{
	/*-------------------------------------------------------------------
	-             Material Name
	---------------------------------------------------------------------*/
	ReadPMXString(filePtr, &MaterialName, setting->Encode);
	ReadPMXString(filePtr, &EnglishName, setting->Encode);

	/*-------------------------------------------------------------------
	-             Material Config
	---------------------------------------------------------------------*/
	fread_s(&Diffuse      , sizeof(Diffuse)      , sizeof(Float4), 1, filePtr);
	fread_s(&Specular     , sizeof(Specular)     , sizeof(Float3), 1, filePtr);
	fread_s(&SpecularPower, sizeof(SpecularPower), sizeof(float) , 1, filePtr);
	fread_s(&Ambient      , sizeof(Ambient)      , sizeof(Float3), 1, filePtr);
	fread_s(&DrawMode     , sizeof(DrawMode)     , sizeof(UINT8) , 1, filePtr);
	fread_s(&EdgeColor    , sizeof(EdgeColor)    , sizeof(Float4), 1, filePtr);
	fread_s(&EdgeSize     , sizeof(EdgeSize)     , sizeof(float) , 1, filePtr);

	ReadPMXIndex(filePtr, &TextureIndex, setting->TextureIndexSize);
	ReadPMXIndex(filePtr, &SphereMapTextureIndex, setting->TextureIndexSize);

	fread_s(&SphereMapMode , sizeof(SphereMapMode), sizeof(UINT8), 1, filePtr);
	fread_s(&ToonTextureMode, sizeof(ToonTextureMode), sizeof(UINT8), 1, filePtr);
	switch (ToonTextureMode)
	{
		case PMXToonTextureMode::Separate:
		{
			ReadPMXIndex(filePtr, &ToonTextureIndex, setting->TextureIndexSize);
			break;
		}
		case PMXToonTextureMode::Common:
		{
			UINT8 toonIndex;
			fread_s(&toonIndex, sizeof(toonIndex), sizeof(UINT8), 1, filePtr);
			ToonTextureIndex = static_cast<INT32>(toonIndex);
			break;
		}
		default:
		{
			return;
		}
	}

	/*-------------------------------------------------------------------
	-             Material Comment
	---------------------------------------------------------------------*/
	ReadPMXString(filePtr, &Memo, setting->Encode);

	/*-------------------------------------------------------------------
	-             Face Index Count
	---------------------------------------------------------------------*/
	fread_s(&FaceIndicesCount, sizeof(FaceIndicesCount), sizeof(INT32), 1, filePtr);
}
void PMXIKLink      ::Read(FILE* filePtr, const PMXSetting* setting)
{
	ReadPMXIndex(filePtr, &LinkTarget, setting->BoneIndexSize);
	fread_s(&EnableLimit, sizeof(EnableLimit), sizeof(unsigned char), 1, filePtr);

	if (EnableLimit != 0)
	{
		fread_s(&AngleMin, sizeof(AngleMin), sizeof(Float3), 1, filePtr);
		fread_s(&AngleMax, sizeof(AngleMax), sizeof(Float3), 1, filePtr);
	}
}
void PMXBone        ::Read(FILE* filePtr, const PMXSetting* setting)
{
	/*-------------------------------------------------------------------
	-             Bone Name
	---------------------------------------------------------------------*/
	ReadPMXString(filePtr, &BoneName, setting->Encode);
	ReadPMXString(filePtr, &EnglishName, setting->Encode);

	/*-------------------------------------------------------------------
	-             Bone Config
	---------------------------------------------------------------------*/
	fread_s(&Position, sizeof(Position), sizeof(Float3), 1, filePtr);
	ReadPMXIndex(filePtr, &ParentBoneIndex, setting->BoneIndexSize);
	fread_s(&DeformDepth, sizeof(DeformDepth), sizeof(INT32), 1, filePtr);
	fread_s(&BoneFlag, sizeof(BoneFlag), sizeof(PMXBoneFlag), 1, filePtr);
	/*-------------------------------------------------------------------
	-             Bone Flag: TargetShowMode
	---------------------------------------------------------------------*/
	if (((UINT16)BoneFlag & (UINT16)PMXBoneFlag::TargetShowMode) == 0)
	{
		fread_s(&PositionOffset, sizeof(PositionOffset), sizeof(Float3), 1, filePtr);
	}
	else
	{
		ReadPMXIndex(filePtr, &LinkBoneIndex, setting->BoneIndexSize);
	}

	/*-------------------------------------------------------------------
	-             Bone Flag: AppendRotate , Translate
	---------------------------------------------------------------------*/
	if (((UINT16)BoneFlag & (UINT16)PMXBoneFlag::AppendRotate) ||
		((UINT16)BoneFlag & (UINT16)PMXBoneFlag::AppendTranslate))
	{
		ReadPMXIndex(filePtr, &AppendBoneIndex, setting->BoneIndexSize);
		fread_s(&AppendWeight, sizeof(AppendWeight), sizeof(float), 1, filePtr);
	}

	/*-------------------------------------------------------------------
	-             Bone Flag: FixedAxis
	---------------------------------------------------------------------*/
	if ((UINT16)BoneFlag & (UINT16)PMXBoneFlag::FixedAxis)
	{
		fread_s(&FixedAxis, sizeof(FixedAxis), sizeof(Float3), 1, filePtr);
	}

	/*-------------------------------------------------------------------
	-             Bone Flag: LocalAxis
	---------------------------------------------------------------------*/
	if ((UINT16)BoneFlag & (UINT16)PMXBoneFlag::LocalAxis)
	{
		fread_s(&LocalAxis_X, sizeof(LocalAxis_X), sizeof(Float3), 1, filePtr);
		fread_s(&LocalAxis_Z, sizeof(LocalAxis_Z), sizeof(Float3), 1, filePtr);
	}

	/*-------------------------------------------------------------------
	-             Bone Flag: DeformOuterParent
	---------------------------------------------------------------------*/
	if ((UINT16)BoneFlag & (UINT16)PMXBoneFlag::DeformOuterParent)
	{
		fread_s(&KeyValue, sizeof(KeyValue), sizeof(INT32), 1, filePtr);
	}

	/*-------------------------------------------------------------------
	-             Bone Flag: IKBone
	---------------------------------------------------------------------*/
	if ((UINT16)BoneFlag & (UINT16)PMXBoneFlag::IKBone)
	{
		ReadPMXIndex(filePtr, &IKTargetBoneIndex, setting->BoneIndexSize);
		fread_s(&IKIterationCount, sizeof(IKIterationCount), sizeof(INT32), 1, filePtr);
		fread_s(&IKAngleLimit    , sizeof(IKAngleLimit    ), sizeof(float), 1, filePtr);

		/*-------------------------------------------------------------------
		-             Load IKLink Count
		---------------------------------------------------------------------*/
		INT32 linkCount = 0;
		fread_s(&linkCount, sizeof(linkCount), sizeof(INT32), 1, filePtr);

		/*-------------------------------------------------------------------
		-             Load IKLink
		---------------------------------------------------------------------*/
		IKLinks.resize(linkCount);
		for (auto& ikLink : IKLinks)
		{
			ikLink.Read(filePtr, setting);
		}
	}
}
void PMXMorph       ::Read(FILE* filePtr, const PMXSetting* setting)
{
	/*-------------------------------------------------------------------
	-             Load Name
	---------------------------------------------------------------------*/
	ReadPMXString(filePtr, &Name, setting->Encode);
	ReadPMXString(filePtr, &EnglishName, setting->Encode);

	Name = unicode::ToUtf8String(unicode::ToWString(Name));
		
	fread_s(&FacePart , sizeof(FacePart ), sizeof(pmx::PMXFacePart ), 1, filePtr);
	fread_s(&MorphType, sizeof(MorphType), sizeof(pmx::PMXMorphType), 1, filePtr);
		
	/*-------------------------------------------------------------------
	-             Load morph data count
	---------------------------------------------------------------------*/
	INT32 dataCount = 0;
	fread_s(&dataCount, sizeof(dataCount), sizeof(INT32), 1, filePtr);

	switch (MorphType)
	{
		/*-------------------------------------------------------------------
		-             MorphType: Position
		---------------------------------------------------------------------*/
		case PMXMorphType::Position:
		{
			PositionMorphs.resize(dataCount);
			for (auto& positionMorph : PositionMorphs)
			{
				ReadPMXIndex(filePtr, &positionMorph.VertexIndex, setting->VertexIndexSize);
				fread_s(&positionMorph.Position, sizeof(positionMorph.Position), sizeof(Float3), 1, filePtr);
			}
			break;
		}
		/*-------------------------------------------------------------------
		-             MorphType: UV
		---------------------------------------------------------------------*/
		case PMXMorphType::UV:
		case PMXMorphType::AddUV1:
		case PMXMorphType::AddUV2:
		case PMXMorphType::AddUV3:
		case PMXMorphType::AddUV4:
		{
			UVMorphs.resize(dataCount);
			for (auto& uvMorph : UVMorphs)
			{
				ReadPMXIndex(filePtr, &uvMorph.VertexIndex, setting->VertexIndexSize);
			}
			break;
		}
		/*-------------------------------------------------------------------
		-             MorphType: Bone
		---------------------------------------------------------------------*/
		case PMXMorphType::Bone:
		{
			BoneMorphs.resize(dataCount);
			for (auto& boneMorph : BoneMorphs)
			{
				ReadPMXIndex(filePtr, &boneMorph.BoneIndex, setting->BoneIndexSize);
				fread_s(&boneMorph.Position  , sizeof(boneMorph.Position)  , sizeof(Float3), 1, filePtr);
				fread_s(&boneMorph.Quaternion, sizeof(boneMorph.Quaternion), sizeof(Float4), 1, filePtr);
			}
			break;
		}
		/*-------------------------------------------------------------------
		-             MorphType: Material
		---------------------------------------------------------------------*/
		case PMXMorphType::Material:
		{
			MaterialMorphs.resize(dataCount);
			for (auto& materialMorph : MaterialMorphs)
			{
				ReadPMXIndex(filePtr, &materialMorph.MaterialIndex, setting->MaterialIndexSize);
				fread_s(&materialMorph.OpType           , sizeof(materialMorph.OpType)           , sizeof(UINT8)   , 1, filePtr);
				fread_s(&materialMorph.Diffuse          , sizeof(materialMorph.Diffuse)          , sizeof(Float4), 1, filePtr);
				fread_s(&materialMorph.Specular         , sizeof(materialMorph.Specular)         , sizeof(Float3), 1, filePtr);
				fread_s(&materialMorph.SpecularPower    , sizeof(materialMorph.SpecularPower)    , sizeof(float)   , 1, filePtr);
				fread_s(&materialMorph.Ambient          , sizeof(materialMorph.Ambient)          , sizeof(Float3), 1, filePtr);
				fread_s(&materialMorph.EdgeColor        , sizeof(materialMorph.EdgeColor)        , sizeof(Float4), 1, filePtr);
				fread_s(&materialMorph.EdgeSize         , sizeof(materialMorph.EdgeSize)         , sizeof(float)   , 1, filePtr);
				fread_s(&materialMorph.TextureFactor    , sizeof(materialMorph.TextureFactor)    , sizeof(Float4), 1, filePtr);
				fread_s(&materialMorph.SphereMapFactor  , sizeof(materialMorph.SphereMapFactor)  , sizeof(Float4), 1, filePtr);
				fread_s(&materialMorph.ToonTextureFactor, sizeof(materialMorph.ToonTextureFactor), sizeof(Float4), 1, filePtr);
			}
			break;
		}
		/*-------------------------------------------------------------------
		-             MorphType: Group
		---------------------------------------------------------------------*/
		case PMXMorphType::Group:
		{
			GroupMorphs.resize(dataCount);
			for (auto& groupMorph : GroupMorphs)
			{
				ReadPMXIndex(filePtr, &groupMorph.MorphIndex, setting->FaceIndexSize);
				fread_s(&groupMorph.Weight, sizeof(groupMorph.Weight), sizeof(float), 1, filePtr);
			}
			break;
		}
		/*-------------------------------------------------------------------
		-             MorphType: Flip
		---------------------------------------------------------------------*/
		case PMXMorphType::Flip:
		{
			FlipMorphs.resize(dataCount);
			for (auto& flipMorph : FlipMorphs)
			{
				ReadPMXIndex(filePtr, &flipMorph.MorphIndex, setting->FaceIndexSize);
				fread_s(&flipMorph.Weight, sizeof(flipMorph.Weight), sizeof(float), 1, filePtr);
			}
			break;
		}
		/*-------------------------------------------------------------------
		-             MorphType: Impulse
		---------------------------------------------------------------------*/
		case PMXMorphType::Impulse:
		{
			ImpulseMorphs.resize(dataCount);
			for (auto& impulseMorph : ImpulseMorphs)
			{
				ReadPMXIndex(filePtr, &impulseMorph.RigidBodyIndex, setting->RigidBodyIndexSize);
				fread_s(&impulseMorph.LocalFlag        , sizeof(impulseMorph.LocalFlag)        , sizeof(UINT8)   , 1, filePtr);
				fread_s(&impulseMorph.TranslateVelocity, sizeof(impulseMorph.TranslateVelocity), sizeof(Float3), 1, filePtr);
				fread_s(&impulseMorph.RotateTorque     , sizeof(impulseMorph.RotateTorque)     , sizeof(Float3), 1, filePtr);
			}
			break;
		}
		default:
		{
			return;
		}
			
	}

}
void PMXDisplayFrame::Read(FILE* filePtr, const PMXSetting* setting)
{
	/*-------------------------------------------------------------------
	-             Load display frame name
	---------------------------------------------------------------------*/
	ReadPMXString(filePtr, &Name, setting->Encode);
	ReadPMXString(filePtr, &EnglishName, setting->Encode);

	/*-------------------------------------------------------------------
	-             Load display frame flag
	---------------------------------------------------------------------*/
	fread_s(&Flag, sizeof(Flag), sizeof(UINT8), 1, filePtr);

	/*-------------------------------------------------------------------
	-             Load target count
	---------------------------------------------------------------------*/
	INT32 targetCount = 0;
	fread_s(&targetCount, sizeof(targetCount), sizeof(INT32), 1, filePtr);

	/*-------------------------------------------------------------------
	-             Load display frame targets data
	---------------------------------------------------------------------*/
	Targets.resize(targetCount);
	for (auto& target : Targets)
	{
		fread_s(&target.Type, sizeof(target.Type), sizeof(UINT8), 1, filePtr);
		switch (target.Type)
		{
			case PMXDisplayFrame::TargetType::BoneIndex:
			{
				ReadPMXIndex(filePtr, &target.Index, setting->BoneIndexSize);
				break;
			}
			case PMXDisplayFrame::TargetType::MorphIndex:
			{
				ReadPMXIndex(filePtr, &target.Index, setting->FaceIndexSize);
				break;
			}
			default:
			{
				return;
			}
		}
	}
	
}
void PMXRigidBody   ::Read(FILE* filePtr, const PMXSetting* setting)
{
	ReadPMXString(filePtr, &Name, setting->Encode);
	ReadPMXString(filePtr, &EnglishName, setting->Encode);
	Name        = unicode::ToUtf8String(unicode::ToWString(Name));
	EnglishName = unicode::ToUtf8String(unicode::ToWString(EnglishName));
	ReadPMXIndex(filePtr, &BoneIndex, setting->BoneIndexSize);
	fread_s(&Group             , sizeof(Group)             , sizeof(UINT8)   , 1, filePtr);
	fread_s(&CollisionGroup    , sizeof(CollisionGroup)    , sizeof(UINT16)  , 1, filePtr);
	fread_s(&Shape             , sizeof(Shape)             , sizeof(UINT8)   , 1, filePtr);
	fread_s(&ShapeSize         , sizeof(ShapeSize)         , sizeof(Float3)  , 1, filePtr);
	fread_s(&Translation       , sizeof(Translation)       , sizeof(Float3)  , 1, filePtr);
	fread_s(&Rotation          , sizeof(Rotation)          , sizeof(Float3)  , 1, filePtr);
	fread_s(&Mass              , sizeof(Mass)              , sizeof(float)   , 1, filePtr);
	fread_s(&DampingTranslation, sizeof(DampingTranslation), sizeof(float)   , 1, filePtr);
	fread_s(&DampingRotation   , sizeof(DampingRotation)   , sizeof(float)   , 1, filePtr);
	fread_s(&Repulsion         , sizeof(Repulsion)         , sizeof(float)   , 1, filePtr);
	fread_s(&Friction          , sizeof(Friction)          , sizeof(float)   , 1, filePtr);
	fread_s(&RigidBodyCalcType , sizeof(RigidBodyCalcType) , sizeof(UINT8)   , 1, filePtr);
}
void PMXJoint       ::Read(FILE* filePtr, const PMXSetting* setting)
{
	ReadPMXString(filePtr, &Name, setting->Encode);
	ReadPMXString(filePtr, &EnglishName, setting->Encode);
	Name        = unicode::ToUtf8String(unicode::ToWString(Name));
	EnglishName = unicode::ToUtf8String(unicode::ToWString(EnglishName));
	fread_s(&JointType              , sizeof(JointType)              , sizeof(UINT8)   , 1, filePtr);
	ReadPMXIndex(filePtr, &RigidBodyIndex_A, setting->RigidBodyIndexSize);
	ReadPMXIndex(filePtr, &RigidBodyIndex_B, setting->RigidBodyIndexSize);
	fread_s(&Translation            , sizeof(Translation)            , sizeof(Float3), 1, filePtr);
	fread_s(&Rotation               , sizeof(Rotation)               , sizeof(Float3), 1, filePtr);
	fread_s(&TranslationMin         , sizeof(TranslationMin)         , sizeof(Float3), 1, filePtr);
	fread_s(&TranslationMax         , sizeof(TranslationMax)         , sizeof(Float3), 1, filePtr);
	fread_s(&RotationMin            , sizeof(RotationMin)            , sizeof(Float3), 1, filePtr);
	fread_s(&RotationMax            , sizeof(RotationMax)            , sizeof(Float3), 1, filePtr);
	fread_s(&SpringTranslationFactor, sizeof(SpringTranslationFactor), sizeof(Float3), 1, filePtr);
	fread_s(&SpringRotationFactor   , sizeof(SpringRotationFactor)   , sizeof(Float3), 1, filePtr);
}
void PMXSoftBodyAnchorRigidBody::Read(FILE* filePtr, const PMXSetting* setting)
{
	ReadPMXIndex(filePtr, &RigidBodyIndex, setting->RigidBodyIndexSize);
	ReadPMXIndex(filePtr, &VertexIndex, setting->VertexIndexSize);
	fread_s(&NearMode, sizeof(NearMode), sizeof(UINT8), 1, filePtr);
}
void PMXSoftBody    ::Read(FILE* filePtr, const PMXSetting* setting)
{
	/*-------------------------------------------------------------------
	-             Load Soft Body Name
	---------------------------------------------------------------------*/
	ReadPMXString(filePtr, &Name, setting->Encode);
	ReadPMXString(filePtr, &EnglishName, setting->Encode);

	/*-------------------------------------------------------------------
	-             Load SoftBodyType
	---------------------------------------------------------------------*/
	fread_s(&SoftBodyType, sizeof(SoftBodyType), sizeof(UINT8), 1, filePtr);

	/*-------------------------------------------------------------------
	-             Load Material Index
	---------------------------------------------------------------------*/
	ReadPMXIndex(filePtr, &MaterialIndex, setting->MaterialIndexSize);

	/*-------------------------------------------------------------------
	-             Load Material Config
	---------------------------------------------------------------------*/
	fread_s(&Group          , sizeof(Group          ), sizeof(UINT8 ), 1, filePtr);
	fread_s(&CollisionGroup , sizeof(CollisionGroup ), sizeof(UINT16), 1, filePtr);
	fread_s(&MaskFlag       , sizeof(MaskFlag       ), sizeof(UINT8 ), 1, filePtr);
	fread_s(&BoneLinkLength , sizeof(BoneLinkLength ), sizeof(INT32 ), 1, filePtr);
	fread_s(&ClustersCount  , sizeof(ClustersCount  ), sizeof(INT32 ), 1, filePtr);
	fread_s(&TotalMass      , sizeof(TotalMass      ), sizeof(float ), 1, filePtr);
	fread_s(&CollisionMargin, sizeof(CollisionMargin), sizeof(float ), 1, filePtr);
	fread_s(&AeroModel      , sizeof(AeroModel      ), sizeof(PMXAeroModel        ), 1, filePtr);
	fread_s(&Config         , sizeof(Config         ), sizeof(PMXSoftBodyConfig   ), 1, filePtr);
	fread_s(&Cluster        , sizeof(Cluster        ), sizeof(PMXSoftBodyCluster  ), 1, filePtr);
	fread_s(&Iteration      , sizeof(Iteration      ), sizeof(PMXSoftBodyIteration), 1, filePtr);
	fread_s(&Material       , sizeof(Material       ), sizeof(PMXSoftBodyMaterial ), 1, filePtr);

	/*-------------------------------------------------------------------
	-             Load Anchor Count
	---------------------------------------------------------------------*/
	INT32 anchorCount = 0;
	fread_s(&anchorCount, sizeof(anchorCount), sizeof(INT32), 1, filePtr);

	/*-------------------------------------------------------------------
	-             Load SoftBody Count
	---------------------------------------------------------------------*/
	Anchor.resize(anchorCount);

	/*-------------------------------------------------------------------
	-             Load Anchor
	---------------------------------------------------------------------*/
	for (auto& anchor : Anchor)
	{
		anchor.Read(filePtr, setting);
	}

	/*-------------------------------------------------------------------
	-             Load vertex count
	---------------------------------------------------------------------*/
	INT32 vertexCount = 0;
	fread_s(&vertexCount, sizeof(vertexCount), sizeof(INT32), 1, filePtr);

	/*-------------------------------------------------------------------
	-             Load vertex count
	---------------------------------------------------------------------*/
	VertexIndices.resize(vertexCount);
	for (auto& index : VertexIndices)
	{
		ReadPMXIndex(filePtr, &index, setting->VertexIndexSize);
	}
}
#pragma endregion EachReadFunction
/****************************************************************************
*							ReadPMXString
*************************************************************************//**
*  @fn            bool PMXData::ReadPMXString(FILE* filePtr, std::string* string)
*  @brief         Load PMX Header
*  @param[in,out] FILE* filePtr
*  @return Å@    Å@bool
*****************************************************************************/
bool ReadPMXString(FILE* filePtr, std::string* string, PMXEncode encode)
{
	using namespace pmx;

	INT32 bufferSize = -1;
	fread_s(&bufferSize, sizeof(bufferSize), sizeof(INT32), 1, filePtr);

	/*-------------------------------------------------------------------
	-             Error Check
	---------------------------------------------------------------------*/
	if (bufferSize < 0)
	{
		::OutputDebugString(L"cannot read buffer size.");
		return false;
	}

	/*-------------------------------------------------------------------
	-             Load String
	---------------------------------------------------------------------*/
	switch (encode)
	{
		case PMXEncode::UTF16:
		{
			std::u16string utf16String(bufferSize / 2, u'\0');
			fread_s(utf16String.data(), sizeof(char16_t) * utf16String.size(), sizeof(char16_t), utf16String.size(), filePtr);
			if (!unicode::ConvertU16ToU8(utf16String, *string)) { return false; }
			break;
		}
		case PMXEncode::UTF8:
		{
			std::string utf8String(bufferSize, '\0');
			fread_s(utf8String.data(), sizeof(char8_t) * utf8String.size(), sizeof(char8_t), utf8String.size(), filePtr);
			*string = utf8String;
			break;
		}
		default:
		{
			::OutputDebugString(L"don't substitute PMX header encoder.");
			return false;
		}
	}

	return true;
}
bool ReadPMXIndex(FILE* filePtr, INT32* pmxIndex, UINT8 indexSize)
{
	switch (indexSize)
	{
		case 1:
		{
			UINT8 index;
			fread_s(&index, sizeof(index), sizeof(UINT8), 1, filePtr);
			if (index != 0xFF)
			{
				*pmxIndex = (INT32)index;
			}
			else
			{
				*pmxIndex = -1;
			}
			break;
		}
		case 2:
		{
			UINT16 index;
			fread_s(&index, sizeof(index), sizeof(UINT16), 1, filePtr);
			if (index != 0xFFFF)
			{
				*pmxIndex = (INT32)index;
			}
			else
			{
				*pmxIndex = -1;
			}
			break;
		}
		case 4:
		{
			UINT32 index;
			fread_s(&index, sizeof(index), sizeof(UINT32), 1, filePtr);
			*pmxIndex = (INT32)index;
			break;
		}
		default:
		{
			return false;
		}
	}
	return true;
}