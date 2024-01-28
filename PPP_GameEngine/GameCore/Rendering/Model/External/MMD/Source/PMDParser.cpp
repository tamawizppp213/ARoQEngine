//////////////////////////////////////////////////////////////////////////////////
//              @file   PMDParser.cpp
///             @brief  PMD File Parser
///             @author Toide Yutaro
///             @date   2022_05_05
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/MMD/Include/PMDParser.hpp"
#include "GameUtility/File/Include/UnicodeUtility.hpp"
#include "GameUtility/File/Include/FileSystem.hpp"
#include <sstream>
#include <iomanip>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace pmd;
using namespace file;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
void ReadPMDString(FILE* filePtr, gu::string* string, UINT32 bufferSize);
bool PMDFile::Load(const gu::tstring& filePath)
{
	/*-------------------------------------------------------------------
	-             Open File
	---------------------------------------------------------------------*/
	FILE* filePtr = file::FileSystem::OpenFile(std::wstring(filePath.CString()));
	if (filePtr == nullptr) { return false; }
	Directory = gu::string(file::FileSystem::GetDirectory(unicode::ToUtf8String(std::wstring(filePath.CString()))).c_str());
	/*-------------------------------------------------------------------
	-             Read Data
	---------------------------------------------------------------------*/
	Header.Read(filePtr);
	ReadVertices           (filePtr);
	ReadIndices            (filePtr);
	ReadMaterials          (filePtr);
	ReadBones              (filePtr);
	ReadBoneIKs            (filePtr);
	ReadFaceExpressions    (filePtr);
	ReadBoneDisplayNameList(filePtr);
	ReadBoneDisplayList    (filePtr);
	ReadLocalizeData       (filePtr);
	ReadToonTextures       (filePtr);
	ReadPhysics            (filePtr);

	return true;
}
PMDFile::~PMDFile()
{
	Vertices           .clear(); Vertices           .shrink_to_fit();
	Indices            .clear(); Indices            .shrink_to_fit();
	Materials          .clear(); Materials          .shrink_to_fit();
	Bones              .clear(); Bones              .shrink_to_fit();
	BoneIKs            .clear(); BoneIKs            .shrink_to_fit();
	FaceExpressions    .clear(); FaceExpressions    .shrink_to_fit();
	FaceLabelIndices   .clear(); FaceLabelIndices   .shrink_to_fit();
	BoneDisplayNameList.clear(); BoneDisplayNameList.shrink_to_fit();
	BoneDisplayList    .clear(); BoneDisplayList    .shrink_to_fit();
	RigidBodies        .clear(); RigidBodies        .shrink_to_fit();
	Joints             .clear(); Joints             .shrink_to_fit();
	ToonTextureList    .clear(); ToonTextureList    .shrink_to_fit();
}
#pragma region PMDFileReadFunction
void PMDFile::ReadVertices           (FILE* filePtr)
{
	UINT32 vertexCount = 0;
	fread_s(&vertexCount, sizeof(vertexCount), sizeof(UINT32), 1, filePtr);

	Vertices.resize(vertexCount);
	for (auto& vertex : Vertices) { vertex.Read(filePtr); }
}
void PMDFile::ReadIndices            (FILE* filePtr)
{
	UINT32 count;
	fread_s(&count, sizeof(count), sizeof(UINT32), 1, filePtr);

	Indices.resize(count);
	fread_s(Indices.data(), Indices.size() * sizeof(UINT16), sizeof(UINT16), Indices.size(), filePtr);

}
void PMDFile::ReadMaterials          (FILE* filePtr)
{
	UINT32 count;
	fread_s(&count, sizeof(count), sizeof(UINT32), 1, filePtr);

	Materials.resize(count);
	for (auto& material : Materials)
	{
		material.Read(filePtr, Directory);
	}
}
void PMDFile::ReadBones              (FILE* filePtr)
{
	UINT16 boneCount;
	fread_s(&boneCount, sizeof(boneCount), sizeof(UINT16), 1, filePtr);

	Bones.resize(boneCount);
	for (auto& bone : Bones) { bone.Read(filePtr); }
}
void PMDFile::ReadBoneIKs            (FILE* filePtr)
{
	UINT16 ikCount;
	fread_s(&ikCount, sizeof(ikCount), sizeof(UINT16), 1, filePtr);

	BoneIKs.resize(ikCount);
	for (auto& ik : BoneIKs) { ik.Read(filePtr); }
}
void PMDFile::ReadFaceExpressions    (FILE* filePtr)
{
	UINT16 faceCount;
	fread_s(&faceCount, sizeof(faceCount), sizeof(UINT16), 1, filePtr);

	FaceExpressions.resize(faceCount);
	for (auto& face : FaceExpressions) { face.Read(filePtr); }

	UINT8 faceLabelIndexCount;
	fread_s(&faceLabelIndexCount, sizeof(faceLabelIndexCount), sizeof(UINT8), 1, filePtr);

	FaceLabelIndices.resize(faceLabelIndexCount);
	for (auto& index : FaceLabelIndices)
	{
		fread_s(&index, sizeof(index), sizeof(UINT16), 1, filePtr);
	}
}
void PMDFile::ReadBoneDisplayNameList(FILE* filePtr)
{
	UINT8 boneDisplayNameCount;
	fread_s(&boneDisplayNameCount, sizeof(boneDisplayNameCount), sizeof(UINT8), 1, filePtr);

	BoneDisplayNameList.resize(boneDisplayNameCount);
	for (auto& name : BoneDisplayNameList) { name.Read(filePtr); }
}
void PMDFile::ReadBoneDisplayList    (FILE* filePtr)
{
	UINT32 boneDisplayCount;
	fread_s(&boneDisplayCount, sizeof(boneDisplayCount), sizeof(UINT32), 1, filePtr);

	BoneDisplayList.resize(boneDisplayCount);
	for (auto& display : BoneDisplayList) { display.Read(filePtr); }
}
void PMDFile::ReadLocalizeData       (FILE* filePtr)
{
	bool useEnglish; 
	fread_s(&useEnglish, sizeof(useEnglish), sizeof(bool), 1, filePtr);

	if (!useEnglish) { return; }
	/*-------------------------------------------------------------------
	-                        Header
	---------------------------------------------------------------------*/
	Header.ReadExtension(filePtr);
	/*-------------------------------------------------------------------
	-                        Bone
	---------------------------------------------------------------------*/
	for (auto& bone : Bones          ) { bone.ReadExtension(filePtr); }
	/*-------------------------------------------------------------------
	-                        FaceExpressions
	---------------------------------------------------------------------*/
	for (auto& face : FaceExpressions)
	{
		if (face.FaceExpressionType == FacePart::Base) { continue; }
		face.ReadExtension(filePtr);
	}
	/*-------------------------------------------------------------------
	-                        DisplayNameList
	---------------------------------------------------------------------*/
	for (auto& name : BoneDisplayNameList) { name.ReadExtension(filePtr); }
	
}
void PMDFile::ReadToonTextures       (FILE* filePtr)
{
	size_t toonTextureIndex = 1;

	ToonTextureList.resize(10);
	if (feof(filePtr) == NULL)
	{
		/*-------------------------------------------------------------------
		-             Load Toon Texture Names (default Name)
		---------------------------------------------------------------------*/
		for (auto& name : ToonTextureList)
		{
			std::stringstream stringStream;
			stringStream << "toon" << std::setfill('0') << std::setw(2) << toonTextureIndex << ".bmp";

			std::string fileName = std::string(Directory.CString()) + "toon/" + stringStream.str();
			name = gu::string(fileName.c_str());
			toonTextureIndex++;
		}
	}
	else
	{
		/*-------------------------------------------------------------------
		-             Load Toon Texture Names (extended)
		---------------------------------------------------------------------*/
		for (auto& toonTextureName : ToonTextureList)
		{
			char temp[100];
			fread_s(&temp, sizeof(temp), sizeof(char), sizeof(temp), filePtr);

			std::string filePath = std::string(Directory.CString()) + "/";
			filePath += std::string(temp, '\0');
			toonTextureName = gu::string(filePath.c_str());
		}
	}
	
}
void PMDFile::ReadPhysics            (FILE* filePtr)
{
	if (feof(filePtr) == NULL) 
	{
		RigidBodies.clear(); RigidBodies.shrink_to_fit(); 
		Joints.clear(); Joints.shrink_to_fit();
		return;
	}
	/*-------------------------------------------------------------------
	-             Read RigidBody
	---------------------------------------------------------------------*/
	UINT32 rigidBodyCount;
	fread_s(&rigidBodyCount, sizeof(rigidBodyCount), sizeof(UINT32), 1, filePtr);

	RigidBodies.resize(rigidBodyCount);
	for (auto& rigidBody : RigidBodies) { rigidBody.Read(filePtr); }
	/*-------------------------------------------------------------------
	-             Read Joints
	---------------------------------------------------------------------*/
	UINT32 jointCount;
	fread_s(&jointCount, sizeof(jointCount), sizeof(UINT32), 1, filePtr);

	Joints.resize(jointCount);
	for (auto& joint : Joints) { joint.Read(filePtr); }
}
#pragma endregion PMDFileReadFunction
#pragma region EachReadFunction
void PMDHeader         ::Read(FILE* filePtr)
{
	fread_s(Signature, sizeof(Signature), sizeof(char), _countof(Signature), filePtr);
	fread_s(&Version , sizeof(Version  ), sizeof(float), 1, filePtr);
	ReadPMDString(filePtr, &ModelName, 20);
	ReadPMDString(filePtr, &ModelComment, 256);
}
void PMDVertex         ::Read(FILE* filePtr)
{
	fread_s(this, sizeof(pmd::PMDVertex), sizeof(pmd::PMDVertex), 1, filePtr);
}
void PMDMaterial       ::Read(FILE* filePtr, const gu::string& directory)
{
	fread_s(&Diffuse      , sizeof(Diffuse      ), sizeof(Float4), 1, filePtr);
	fread_s(&SpecularPower, sizeof(SpecularPower), sizeof(float ), 1, filePtr);
	fread_s(&Specular     , sizeof(Specular     ), sizeof(Float3), 1, filePtr);
	fread_s(&Ambient      , sizeof(Ambient      ), sizeof(Float3), 1, filePtr);
	fread_s(&ToonID       , sizeof(ToonID       ), sizeof(UINT8 ), 1, filePtr);
	fread_s(&EdgeFlag     , sizeof(EdgeFlag     ), sizeof(UINT8 ), 1, filePtr);
	fread_s(&IndexCount   , sizeof(IndexCount   ), sizeof(UINT32), 1, filePtr);
	gu::string textureName; ReadPMDString(filePtr, &textureName, 20);
	ReadTextureName(directory, textureName);
}
void PMDBone           ::Read(FILE* filePtr)
{
	ReadPMDString(filePtr, &BoneName, 20);
	fread_s(&ParentBoneID    , sizeof(ParentBoneID)    , sizeof(UINT16)    , 1, filePtr);
	fread_s(&ChildBoneID     , sizeof(ChildBoneID)     , sizeof(UINT16)    , 1, filePtr);
	fread_s(&BoneType        , sizeof(BoneType)        , sizeof(UINT8)     , 1, filePtr);
	fread_s(&IKBoneID        , sizeof(IKBoneID)        , sizeof(UINT16)    , 1, filePtr);
	fread_s(&BoneHeadPosition, sizeof(BoneHeadPosition), sizeof(gm::Float3), 1, filePtr);
}
void PMDBoneIK         ::Read(FILE* filePtr)
{
	fread_s(&IKBoneID      , sizeof(IKBoneID)      , sizeof(UINT16), 1, filePtr);
	fread_s(&IKTargetBoneID, sizeof(IKTargetBoneID), sizeof(UINT16), 1, filePtr);
	fread_s(&IKChainLength , sizeof(IKChainLength) , sizeof(UINT8) , 1, filePtr);
	fread_s(&IterationCount, sizeof(IterationCount), sizeof(UINT16), 1, filePtr);
	fread_s(&AngleLimit    , sizeof(AngleLimit)    , sizeof(float) , 1, filePtr);

	Chains.resize(IKChainLength);
	for (auto& chain : Chains)
	{
		fread_s(&chain, sizeof(chain), sizeof(UINT16), 1, filePtr);
	}
}
void PMDFaceExpression ::Read(FILE* filePtr)
{
	ReadPMDString(filePtr, &FaceExpressionName, 20);
	fread_s(&VertexNum         , sizeof(VertexNum)         , sizeof(UINT32)       , 1 , filePtr);
	fread_s(&FaceExpressionType, sizeof(FaceExpressionType), sizeof(pmd::FacePart), 1 , filePtr);
	Vertices.resize(VertexNum);
	Indices .resize(VertexNum);
	for (UINT i = 0; i < VertexNum; ++i)
	{
		switch (FaceExpressionType)
		{
		case pmd::FacePart::Base: // index, vertex Position
			fread_s(&Vertices[i], sizeof(Vertices[i]), sizeof(Float3), 1, filePtr);
			fread_s(&Indices [i], sizeof(Indices[i]) , sizeof(UINT32)  , 1, filePtr);
			break;
		default: // index => index about FacePart::Base, vertex => offset Position
			fread_s(&Vertices[i], sizeof(Vertices[i]), sizeof(Float3), 1, filePtr);
			fread_s(&Indices [i], sizeof(Indices[i] ), sizeof(UINT32)  , 1, filePtr);
			break;
		}
	}
}
void PMDBoneDisplayName::Read(FILE* filePtr)
{
	ReadPMDString(filePtr, &BoneDisplayName, 50);
}
void PMDBoneDisplay    ::Read(FILE* filePtr)
{
	fread_s(&BoneIndex, sizeof(BoneIndex), sizeof(UINT16), 1, filePtr);
	fread_s(&BoneDisplayIndex, sizeof(BoneDisplayIndex), sizeof(UINT8), 1, filePtr);
}
void PMDRigidBody      ::Read(FILE* filePtr)
{
	ReadPMDString(filePtr, &RigidBodyName, 20);
	fread_s(&RelationBoneIndex , sizeof(RelationBoneIndex ), sizeof(UINT16), 1, filePtr);
	fread_s(&GroupIndex        , sizeof(GroupIndex        ), sizeof(UINT8 ), 1, filePtr);
	fread_s(&GroupTarget       , sizeof(GroupTarget       ), sizeof(UINT16), 1, filePtr);
	fread_s(&RigidBodyShapeType, sizeof(RigidBodyShapeType), sizeof(UINT8 ), 1, filePtr);
	fread_s(&BodyShape         , sizeof(BodyShape         ), sizeof(Float3), 1, filePtr);
	fread_s(&Position          , sizeof(Position          ), sizeof(Float3), 1, filePtr);
	fread_s(&Rotation          , sizeof(Rotation          ), sizeof(Float3), 1, filePtr);
	fread_s(&Mass              , sizeof(Mass              ), sizeof(float ), 1, filePtr);
	fread_s(&DampingTranslate  , sizeof(DampingTranslate  ), sizeof(float ), 1, filePtr);
	fread_s(&DampingRotation   , sizeof(DampingRotation   ), sizeof(float ), 1, filePtr);
	fread_s(&Elasticity        , sizeof(Elasticity        ), sizeof(float ), 1, filePtr);
	fread_s(&Friction          , sizeof(Friction          ), sizeof(float ), 1, filePtr);
	fread_s(&RigidBodyCalcType , sizeof(RigidBodyCalcType ), sizeof(UINT8 ), 1, filePtr);
}
void PMDJoint          ::Read(FILE* filePtr)
{
	ReadPMDString(filePtr, &JointName, 20);
	fread_s(&RigidBodyA, sizeof(RigidBodyA), sizeof(UINT32), 1, filePtr);
	fread_s(&RigidBodyB, sizeof(RigidBodyB), sizeof(UINT32), 1, filePtr);
	fread_s(&JointTranslation       , sizeof(JointTranslation       ), sizeof(Float3), 1, filePtr);
	fread_s(&JointRotation          , sizeof(JointRotation          ), sizeof(Float3), 1, filePtr);
	fread_s(&TranslationMin         , sizeof(TranslationMin         ), sizeof(Float3), 1, filePtr);
	fread_s(&TranslationMax         , sizeof(TranslationMax         ), sizeof(Float3), 1, filePtr);
	fread_s(&RotationMin            , sizeof(RotationMin            ), sizeof(Float3), 1, filePtr);
	fread_s(&RotationMax            , sizeof(RotationMax            ), sizeof(Float3), 1, filePtr);
	fread_s(&SpringTranslationFactor, sizeof(SpringTranslationFactor), sizeof(Float3), 1, filePtr);
	fread_s(&SpringRotationFactor   , sizeof(SpringRotationFactor   ), sizeof(Float3), 1, filePtr);
}
void PMDHeader         ::ReadExtension(FILE* filePtr)
{
	ReadPMDString(filePtr, &ModelEnglishName   , 20);
	ReadPMDString(filePtr, &ModelEnglishComment, 256);
}
void PMDBone           ::ReadExtension(FILE* filePtr)
{
	ReadPMDString(filePtr, &EnglishBoneName, 20);
}
void PMDFaceExpression ::ReadExtension(FILE* filePtr)
{
	ReadPMDString(filePtr, &FaceExpressionEnglishName, 20);
}
void PMDBoneDisplayName::ReadExtension(FILE* filePtr)
{
	ReadPMDString(filePtr, &BoneDisplayEnglishName, 50);
}

void PMDMaterial::ReadTextureName(const gu::string& directory, const gu::string& textureName)
{
	const auto stdTextureName = std::string(textureName.CString());
	if (textureName.Size() != 0) // Whether the file exists or not
	{
		/*-------------------------------------------------------------------
		-             Exist Check Splitter and Get FilePath Name
		---------------------------------------------------------------------*/
		/*-------------------------------------------------------------------
		-             In case existance splitter
		---------------------------------------------------------------------*/
		if (std::count(stdTextureName.begin(), stdTextureName.end(), '*') > 0)
		{
			auto fileNamePair = FileSystem::Split(stdTextureName);
			/*-------------------------------------------------------------------
			-             Get FilePath Name
			---------------------------------------------------------------------*/
			if (FileSystem::GetExtension(fileNamePair.first) == "sph")
			{
				TextureFileName = directory + gu::string(fileNamePair.second.c_str());
				SphereFileName  = directory + gu::string(fileNamePair.first.c_str());
			}
			else if (FileSystem::GetExtension(fileNamePair.first) == "spa")
			{
				TextureFileName = directory  + gu::string(fileNamePair.second.c_str());
				SphereFileName  = directory  + gu::string(fileNamePair.first.c_str());
			}
			else
			{
				TextureFileName = directory  + gu::string(fileNamePair.first.c_str());
				if (FileSystem::GetExtension(fileNamePair.second) == "sph")
				{
					SphereFileName = directory  + gu::string(fileNamePair.second.c_str());
				}
				else if (FileSystem::GetExtension(fileNamePair.second) == "spa")
				{
					SphereFileName = directory  + gu::string(fileNamePair.second.c_str());
				}
				else
				{
					::OutputDebugString(L"Couldn't read second textures");
					return;
				}
			}
		}
		/*-------------------------------------------------------------------
		-             In case no-existance splitter
		---------------------------------------------------------------------*/
		else
		{
			/*-------------------------------------------------------------------
			-             Get FilePath Name
			---------------------------------------------------------------------*/
			if (FileSystem::GetExtension(stdTextureName) == "sph")
			{
				SphereFileName  = directory + textureName;
				TextureFileName = "";
			}
			else if (FileSystem::GetExtension(stdTextureName) == "spa")
			{
				SphereFileName = directory  + textureName;
				TextureFileName = "";
			}
			else
			{
				TextureFileName = directory + textureName;
			}
		}
	}
}
#pragma endregion EachReadFunction
void ReadPMDString(FILE* filePtr, gu::string* string, UINT32 bufferSize)
{
	std::string utf8String(bufferSize, '\0');
	fread_s(utf8String.data(), sizeof(char8_t) * utf8String.size(), sizeof(char8_t), utf8String.size(), filePtr);
	*string = gu::string(utf8String.c_str());
}