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
#include "Platform/Core/Include/CoreFileSystem.hpp"
#include "GameUtility/Base/Include/GUStringConverter.hpp"
#include "GameUtility/Base/Include/GUCharacterCodeConverter.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace engine::file::pmx;
using namespace platform::core;
using namespace gu;

namespace
{
	uint32 ReadIndex(const gu::SharedPointer<file::IFileHandle>& fileHandle, const uint32 size)
	{
		switch (size)
		{
			case 1:
			{
				gu::uint8 value = 0;
				fileHandle->Read(&value, sizeof(gu::uint8));
				return static_cast<uint32>(value);
			}
			case 2:
			{
				gu::uint16 value = 0;
				fileHandle->Read(&value, sizeof(gu::uint16));
				return static_cast<uint32>(value);
			}
			case 4:
			{
				gu::uint32 value = 0;
				fileHandle->Read(&value, sizeof(gu::uint32));
				return value;
			}
			default:
			{
				throw "Invalid index size.";
			}
		}
	}

	gu::tstring ReadString(const gu::SharedPointer<file::IFileHandle>& fileHandle,const PMXEncode encode)
	{
		int32 bufferSize = 0;
		fileHandle->Read(&bufferSize, sizeof(int32));

		if (bufferSize == 0)
		{
			return SP("");
		}

		/*-------------------------------------------------------------------
		-             UTF16�̂Ƃ��͂��̂܂�TString�ɕϊ�
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
		-             UTF8�̂Ƃ��͂��̂܂�TString�ɕϊ�
		---------------------------------------------------------------------*/
		else
		{
			// �����񂪖��ʂɃo�b�t�@���g���̂�
			gu::DynamicArray<char> byteArray(bufferSize, true);

			fileHandle->Read(byteArray.Data(), sizeof(char) * bufferSize);

			// �����񃁃������ב΍�
			gu::string result(byteArray.Data());

			return StringConverter::ConvertStringToTString(result);
		}
	}
}
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////



PMXFile::~PMXFile()
{
	Vertices       .Clear(); Vertices       .ShrinkToFit();
	Indices        .Clear(); Indices        .ShrinkToFit();
	TexturePathList.Clear(); TexturePathList.ShrinkToFit();
	Materials      .Clear(); Materials      .ShrinkToFit();
	Bones          .Clear(); Bones          .ShrinkToFit();
	Morphs         .Clear(); Morphs         .ShrinkToFit();
	DisplayFrames  .Clear(); DisplayFrames  .ShrinkToFit();
	RigidBodies    .Clear(); RigidBodies    .ShrinkToFit();
	Joints         .Clear(); Joints         .ShrinkToFit();
}
#pragma region PMXFileFunction
bool PMXFile::Read(const gu::tstring& filePath, const bool useAsync)
{
	/*-------------------------------------------------------------------
	-             �t�@�C���n���h���̎擾
	---------------------------------------------------------------------*/
	const auto fileHandle = IFileSystem::Get()->OpenRead(filePath.CString(), false, useAsync);

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

	if (!Header.IsValid())
	{
		return false;
	}

	/*-------------------------------------------------------------------
	-            ���_���̓ǂݍ���
	---------------------------------------------------------------------*/
	uint32 vertexCount = 0;
	fileHandle->Read(&vertexCount, sizeof(uint32));

	Vertices.Reserve(vertexCount); // �R���X�g���N�^�Ăяo���ŏ������d���Ȃ邽�߁AReserve�Ń������m��
	for (uint32 i = 0; i < vertexCount; ++i)
	{
		Vertices.Push(PMXVertex(fileHandle, Header));
	}

	/*-------------------------------------------------------------------
	-            �C���f�b�N�X���̓ǂݍ���
	---------------------------------------------------------------------*/
	uint32 indexCount = 0;
	fileHandle->Read(&indexCount, sizeof(uint32));

	Indices.Reserve(indexCount); // �R���X�g���N�^�Ăяo���ŏ������d���Ȃ邽�߁AReserve�Ń������m��
	for (uint32 i = 0; i < indexCount; ++i)
	{
		Indices.Push(ReadIndex(fileHandle, Header.VertexIndexSize));
	}

	/*-------------------------------------------------------------------
	-            �e�N�X�`�����̓ǂݍ���
	---------------------------------------------------------------------*/
	uint32 textureCount = 0;
	fileHandle->Read(&textureCount, sizeof(uint32));

	TexturePathList.Resize(textureCount); // �R���X�g���N�^�Ăяo���ŏ������d���Ȃ邽�߁AReserve�Ń������m��
	for (uint32 i = 0; i < textureCount; ++i)
	{
		TexturePathList[i] = Directory + ReadString(fileHandle, Header.Encode);
	}

	/*-------------------------------------------------------------------
	-            �}�e���A�����̓ǂݍ���
	---------------------------------------------------------------------*/
	uint32 materialCount = 0;
	fileHandle->Read(&materialCount, sizeof(uint32));

	Materials.Resize(materialCount);
	for (auto& material : Materials)
	{
		material.Read(fileHandle, Header);
	}

	/*-------------------------------------------------------------------
	-            �}�e���A�����̓ǂݍ���
	---------------------------------------------------------------------*/
	uint32 boneCount = 0;
	fileHandle->Read(&boneCount, sizeof(uint32));

	Bones.Resize(boneCount);
	for (auto& bone : Bones)
	{
		bone.Read(fileHandle, Header);
	}

	/*-------------------------------------------------------------------
	-            ���[�t���̓ǂݍ���
	---------------------------------------------------------------------*/
	uint32 morphCount = 0;
	fileHandle->Read(&morphCount, sizeof(uint32));

	Morphs.Resize(morphCount);
	for (auto& morph : Morphs)
	{
		morph.Read(fileHandle, Header);
	}

	/*-------------------------------------------------------------------
	-            �{�[���\�������̓ǂݍ���
	---------------------------------------------------------------------*/
	uint32 boneDisplayNameCount = 0;
	fileHandle->Read(&boneDisplayNameCount, sizeof(uint32));

	DisplayFrames.Resize(boneDisplayNameCount);
	for (auto& displayFrame : DisplayFrames)
	{
		displayFrame.Read(fileHandle, Header);
	}

	/*-------------------------------------------------------------------
	-            ���̏��̓ǂݍ���
	---------------------------------------------------------------------*/
	uint32 rigidBodyCount = 0;
	fileHandle->Read(&rigidBodyCount, sizeof(uint32));

	RigidBodies.Resize(rigidBodyCount);
	for (auto& rigidBody : RigidBodies)
	{
		rigidBody.Read(fileHandle, Header);
	}

	/*-------------------------------------------------------------------
	-            �W���C���g���̓ǂݍ���
	---------------------------------------------------------------------*/
	uint32 jointCount = 0;
	fileHandle->Read(&jointCount, sizeof(uint32));

	Joints.Resize(jointCount);
	for (auto& joint : Joints)
	{
		joint.Read(fileHandle, Header);
	}

	/*-------------------------------------------------------------------
	-            �\�t�g�{�f�B���̓ǂݍ���
	---------------------------------------------------------------------*/
	if (fileHandle->Tell() == fileHandle->Size()) { return true; }

	uint32 softBodyCount = 0;
	fileHandle->Read(&softBodyCount, sizeof(uint32));
	if(softBodyCount == 0) { return true; }

	SoftBodies.Resize(softBodyCount);
	for (auto& softBody : SoftBodies)
	{
		softBody.Read(fileHandle, Header);
	}

	return true;
}

