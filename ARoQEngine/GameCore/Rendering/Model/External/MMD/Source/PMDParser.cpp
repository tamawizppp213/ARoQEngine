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
#include "Platform/Core/Include/CoreFileSystem.hpp"
#include "GameUtility/Base/Include/GUStringConverter.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gc::file::pmd;
using namespace platform::core;
using namespace gu;

namespace
{
	gu::tstring ReadString(const gu::SharedPointer<file::IFileHandle>& fileHandle, const gu::uint64 length)
	{
		// �����񂪖��ʂɃo�b�t�@���g���̂�
		gu::DynamicArray<char> byteArray(length, true);

		fileHandle->Read(byteArray.Data(), sizeof(char) * length);

		// �����񃁃������ב΍�
		gu::string result(byteArray.Data());

		return StringConverter::ConvertStringToTString(result);
	}
}

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
PMDFile::~PMDFile()
{
	Vertices           .Clear(); Vertices           .ShrinkToFit();
	Indices            .Clear(); Indices            .ShrinkToFit();
	Materials          .Clear(); Materials          .ShrinkToFit();
	Bones              .Clear(); Bones              .ShrinkToFit();
	IKs                .Clear(); IKs                .ShrinkToFit();
	FaceExpressions    .Clear(); FaceExpressions    .ShrinkToFit();
	FaceLabelIndices   .Clear(); FaceLabelIndices   .ShrinkToFit();
	BoneDisplayNameList.Clear(); BoneDisplayNameList.ShrinkToFit();
	BoneNameTable      .Clear(); BoneNameTable      .ShrinkToFit();
	RigidBodies        .Clear(); RigidBodies        .ShrinkToFit();
	Joints             .Clear(); Joints             .ShrinkToFit();
	ToonTextureList    .Clear(); ToonTextureList    .ShrinkToFit();
}

#pragma region PMDFileReadFunction
/*!**********************************************************************
*  @brief     PMD�t�@�C����ǂݍ��ފ֐�
*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& �t�@�C������̃n���h��
*  @return    void
*************************************************************************/
bool PMDFile::Read(const gu::tstring& filePath)
{
	/*-------------------------------------------------------------------
	-             �t�@�C���n���h���̎擾
	---------------------------------------------------------------------*/
	const auto fileHandle = IFileSystem::Get()->OpenRead(filePath.CString());

	// �t�@�C�����J���Ȃ������ꍇ��false��Ԃ�
	if (!fileHandle)
	{
		return false;
	}

	// �f�B���N�g���̎擾
	Directory = IFileSystem::Get()->GetDirectory(filePath.CString());

	/*-------------------------------------------------------------------
	-            �w�b�_���̓ǂݍ���
	---------------------------------------------------------------------*/
	Header.Read(fileHandle);
	
	if(!Header.IsValid()) 
	{
		throw "Invalid PMD Header";
	}

	/*-------------------------------------------------------------------
	-            ���_���̓ǂݍ���
	---------------------------------------------------------------------*/
	uint32 vertexCount = 0;
	fileHandle->Read(&vertexCount, sizeof(uint32));

	Vertices.Reserve(vertexCount); // �R���X�g���N�^�Ăяo���ŏ������d���Ȃ邽�߁AReserve�Ń������m��
	for (uint32 i = 0; i < vertexCount; ++i)
	{
		Vertices.Push(details::PMDVertex(fileHandle));
	}

	/*-------------------------------------------------------------------
	-            �C���f�b�N�X���̓ǂݍ���
	---------------------------------------------------------------------*/
	uint32 indexCount = 0;
	fileHandle->Read(&indexCount, sizeof(uint32));

	Indices.Resize(indexCount);
	fileHandle->Read(Indices.Data(), Indices.Size() * sizeof(uint16));

	/*-------------------------------------------------------------------
	-            �}�e���A�����̓ǂݍ���
	---------------------------------------------------------------------*/
	uint32 materialCount = 0;
	fileHandle->Read(&materialCount, sizeof(uint32));

	Materials.Resize(materialCount);
	for(uint32 i = 0; i < materialCount; ++i)
	{
		Materials[i] = (details::PMDMaterial(fileHandle, Directory));
	}

	/*-------------------------------------------------------------------
	-            �{�[�����̓ǂݍ���
	---------------------------------------------------------------------*/
	uint16 boneCount = 0;
	fileHandle->Read(&boneCount, sizeof(uint16));

	Bones.Resize(boneCount); // �{�[���͌��������Ȃ�����, Resize�Ń������m�� (������������Ȓl���܂܂�Ă��邱�Ƃ�����, ���̂悤�ȑΉ�)
	for (auto& bone : Bones) 
	{
		bone.Read(fileHandle); 
	}

	/*-------------------------------------------------------------------
	-            �{�[��IK���̓ǂݍ���
	---------------------------------------------------------------------*/
	uint16 ikCount = 0;
	fileHandle->Read(&ikCount, sizeof(uint16));

	IKs.Resize(ikCount);
	for (uint8 i = 0; i < ikCount; ++i)
	{
		IKs[i].Read(fileHandle);
	};

	/*-------------------------------------------------------------------
	-           �@�\��̓ǂݍ���
	---------------------------------------------------------------------*/
	uint16 faceCount = 0;
	fileHandle->Read(&faceCount, sizeof(uint16));

	FaceExpressions.Resize(faceCount);
	for (auto& face : FaceExpressions) 
	{
		face.Read(fileHandle); 
	}

	uint8 faceLabelIndexCount = 0;
	fileHandle->Read(&faceLabelIndexCount, sizeof(uint8));

	FaceLabelIndices.Resize(faceLabelIndexCount);
	fileHandle->Read(FaceLabelIndices.Data(), FaceLabelIndices.Size() * sizeof(uint16));

	/*-------------------------------------------------------------------
	-           �@�{�[���\�����̓ǂݍ���
	---------------------------------------------------------------------*/
	uint8 boneDisplayNameCount = 0;
	fileHandle->Read(&boneDisplayNameCount, sizeof(uint8));

	BoneDisplayNameList.Resize(boneDisplayNameCount);
	for (auto& name : BoneDisplayNameList) 
	{
		name.Read(fileHandle);
	}

	/*-------------------------------------------------------------------
	-           �@�{�[���\�����X�g
	---------------------------------------------------------------------*/
	uint32 boneNameTableCount = 0;
	fileHandle->Read(&boneNameTableCount, sizeof(uint32));

	BoneNameTable.Resize(boneNameTableCount);
	for (auto& name : BoneNameTable)
	{
		name.Read(fileHandle);
	}

	/*-------------------------------------------------------------------
	-           �@���[�J���C�Y
	---------------------------------------------------------------------*/
	ReadLocalizeData(fileHandle);

	/*-------------------------------------------------------------------
	-           �@�g�D�[���e�N�X�`��
	---------------------------------------------------------------------*/
	ReadToonTextures(fileHandle);

	/*-------------------------------------------------------------------
	-           �@����
	---------------------------------------------------------------------*/
	ReadPhysics(fileHandle);

	return true;
}


void PMDFile::ReadLocalizeData(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle)
{
	bool useEnglish; 
	fileHandle->Read(&useEnglish, sizeof(bool));

	if (!useEnglish) { return; }
	/*-------------------------------------------------------------------
	-                        Header
	---------------------------------------------------------------------*/
	Header.ReadExtension(fileHandle);

	/*-------------------------------------------------------------------
	-                        Bone
	---------------------------------------------------------------------*/
	for (auto& bone : Bones)
	{ 
		bone.ReadExtension(fileHandle); 
	}
	/*-------------------------------------------------------------------
	-                        FaceExpressions
	---------------------------------------------------------------------*/
	for (auto& face : FaceExpressions)
	{
		if (face.Part == details::PMDFacePart::Base) { continue; }
		face.ReadExtension(fileHandle);
	}
	/*-------------------------------------------------------------------
	-                        DisplayNameList
	---------------------------------------------------------------------*/
	for (auto& name : BoneDisplayNameList) 
	{
		name.ReadExtension(fileHandle); 
	}
	
}

void PMDFile::ReadToonTextures(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle)
{
	if(fileHandle->Tell() == fileHandle->Size()) { return; }

	ToonTextureList.Resize(10);

	/*-------------------------------------------------------------------
	-             Load Toon Texture Names (extended)
	---------------------------------------------------------------------*/
	for (auto& toonTextureName : ToonTextureList)
	{
		const auto temp = ReadString(fileHandle, 100);
		
		tstring filePath = Directory;
		filePath += temp;
		toonTextureName = filePath;
	}
	
}
void PMDFile::ReadPhysics(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle)
{
	if (fileHandle->Tell() == fileHandle->Size()) { return; }

	/*-------------------------------------------------------------------
	-             Read RigidBody
	---------------------------------------------------------------------*/
	uint32 rigidBodyCount = 0;
	fileHandle->Read(&rigidBodyCount, sizeof(uint32));

	RigidBodies.Resize(rigidBodyCount);
	for (auto& rigidBody : RigidBodies) 
	{
		rigidBody.Read(fileHandle); 
	}
	/*-------------------------------------------------------------------
	-             Read Joints
	---------------------------------------------------------------------*/
	uint32 jointCount = 0;
	fileHandle->Read(&jointCount, sizeof(uint32));

	Joints.Resize(jointCount);
	for (auto& joint : Joints) 
	{
		joint.Read(fileHandle);
	}
}
#pragma endregion PMDFileReadFunction
