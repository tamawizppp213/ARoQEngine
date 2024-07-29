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
			gu::DynamicArray<char> byteArray(bufferSize, true);

			fileHandle->Read(byteArray.Data(), sizeof(char) * bufferSize);

			// 文字列メモリ負荷対策
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
	-             ファイルハンドルの取得
	---------------------------------------------------------------------*/
	const auto fileHandle = IFileSystem::Get()->OpenRead(filePath.CString(), false, useAsync);

	// ファイルが開けなかった場合はfalseを返す
	if (!fileHandle)
	{
		return false;
	}

	// ディレクトリの取得
	Directory = IFileSystem::Get()->GetDirectory(filePath.CString());

	/*-------------------------------------------------------------------
	-            ヘッダ情報の読み込み
	---------------------------------------------------------------------*/
	Header.Read(fileHandle);

	if (!Header.IsValid())
	{
		return false;
	}

	/*-------------------------------------------------------------------
	-            頂点情報の読み込み
	---------------------------------------------------------------------*/
	uint32 vertexCount = 0;
	fileHandle->Read(&vertexCount, sizeof(uint32));

	Vertices.Reserve(vertexCount); // コンストラクタ呼び出しで処理が重くなるため、Reserveでメモリ確保
	for (uint32 i = 0; i < vertexCount; ++i)
	{
		Vertices.Push(PMXVertex(fileHandle, Header));
	}

	/*-------------------------------------------------------------------
	-            インデックス情報の読み込み
	---------------------------------------------------------------------*/
	uint32 indexCount = 0;
	fileHandle->Read(&indexCount, sizeof(uint32));

	Indices.Reserve(indexCount); // コンストラクタ呼び出しで処理が重くなるため、Reserveでメモリ確保
	for (uint32 i = 0; i < indexCount; ++i)
	{
		Indices.Push(ReadIndex(fileHandle, Header.VertexIndexSize));
	}

	/*-------------------------------------------------------------------
	-            テクスチャ情報の読み込み
	---------------------------------------------------------------------*/
	uint32 textureCount = 0;
	fileHandle->Read(&textureCount, sizeof(uint32));

	TexturePathList.Resize(textureCount); // コンストラクタ呼び出しで処理が重くなるため、Reserveでメモリ確保
	for (uint32 i = 0; i < textureCount; ++i)
	{
		TexturePathList[i] = Directory + ReadString(fileHandle, Header.Encode);
	}

	/*-------------------------------------------------------------------
	-            マテリアル情報の読み込み
	---------------------------------------------------------------------*/
	uint32 materialCount = 0;
	fileHandle->Read(&materialCount, sizeof(uint32));

	Materials.Resize(materialCount);
	for (auto& material : Materials)
	{
		material.Read(fileHandle, Header);
	}

	/*-------------------------------------------------------------------
	-            マテリアル情報の読み込み
	---------------------------------------------------------------------*/
	uint32 boneCount = 0;
	fileHandle->Read(&boneCount, sizeof(uint32));

	Bones.Resize(boneCount);
	for (auto& bone : Bones)
	{
		bone.Read(fileHandle, Header);
	}

	/*-------------------------------------------------------------------
	-            モーフ情報の読み込み
	---------------------------------------------------------------------*/
	uint32 morphCount = 0;
	fileHandle->Read(&morphCount, sizeof(uint32));

	Morphs.Resize(morphCount);
	for (auto& morph : Morphs)
	{
		morph.Read(fileHandle, Header);
	}

	/*-------------------------------------------------------------------
	-            ボーン表示名情報の読み込み
	---------------------------------------------------------------------*/
	uint32 boneDisplayNameCount = 0;
	fileHandle->Read(&boneDisplayNameCount, sizeof(uint32));

	DisplayFrames.Resize(boneDisplayNameCount);
	for (auto& displayFrame : DisplayFrames)
	{
		displayFrame.Read(fileHandle, Header);
	}

	/*-------------------------------------------------------------------
	-            剛体情報の読み込み
	---------------------------------------------------------------------*/
	uint32 rigidBodyCount = 0;
	fileHandle->Read(&rigidBodyCount, sizeof(uint32));

	RigidBodies.Resize(rigidBodyCount);
	for (auto& rigidBody : RigidBodies)
	{
		rigidBody.Read(fileHandle, Header);
	}

	/*-------------------------------------------------------------------
	-            ジョイント情報の読み込み
	---------------------------------------------------------------------*/
	uint32 jointCount = 0;
	fileHandle->Read(&jointCount, sizeof(uint32));

	Joints.Resize(jointCount);
	for (auto& joint : Joints)
	{
		joint.Read(fileHandle, Header);
	}

	/*-------------------------------------------------------------------
	-            ソフトボディ情報の読み込み
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

