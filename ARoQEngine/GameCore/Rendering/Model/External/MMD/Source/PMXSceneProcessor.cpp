//////////////////////////////////////////////////////////////////////////////////
///  @file   PMXSceneProcessor.cpp
///  @brief  拡張子が.pmxのファイルから, 共通のSceneFileMeshに相互変換するクラス. Importがファイル読み込み, Exportが書き出し
///  @author toide
///  @date   2024/07/23 0:58:33
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/MMD/Include/PMXSceneProcessor.hpp"
#include "GameCore/Rendering/Model/External/MMD/Include/PMXParser.hpp"
#include "GameCore/Rendering/Model/Private/SceneFile/Include/SFScene.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace engine::file::pmx;
using namespace engine::file;
using namespace gu;
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor

#pragma endregion Constructor and Destructor

#pragma region Public Function
/*!**********************************************************************
*  @brief     各拡張子に基づいてシーンを読み込みます
*  @param[in] const gu::tstring& ファイルパス
*  @return    gu::SharedPointer<SFScene>
*************************************************************************/
gu::SharedPointer<sf::SFScene> PMXSceneProcessor::Import(const gu::tstring& filePath)
{
	/*-------------------------------------------------------------------
	-       PMXファイルを読み込む
	---------------------------------------------------------------------*/
	const auto file = gu::MakeShared<PMXFile>();

	if (!file->Read(filePath))
	{
		return nullptr;
	}

	/*-------------------------------------------------------------------
	-       シーンのインポート
	---------------------------------------------------------------------*/
	const auto scene = gu::MakeShared<sf::SFScene>();

	/*-------------------------------------------------------------------
	-       メッシュデータの取得
	---------------------------------------------------------------------*/
	scene->Meshes.Resize(file->Materials.Size());

	uint64 indexStart = 0;
	for (uint64 i = 0; i < scene->Meshes.Size(); ++i)
	{
		// material毎に頂点メッシュを取得する
		const auto indexCount = static_cast<uint64>(file->Materials[i].IndexCount);

		CreateMesh(file, scene->Meshes[i], indexStart, indexCount);
		scene->Meshes[i].MaterialIndex = static_cast<uint32>(i);
		scene->Meshes[i].Name = file->Materials[i].NameJapanese;

		indexStart += indexCount;
	}

	/*-------------------------------------------------------------------
	-       マテリアルの取得
	---------------------------------------------------------------------*/
	scene->Materials.Resize(file->Materials.Size(), false);
	for (uint32 i = 0; i < file->Materials.Size(); ++i)
	{
		const auto& pmxMaterial = file->Materials[i];

		scene->Materials[i].Name          = pmxMaterial.NameJapanese;
		scene->Materials[i].Diffuse       = pmxMaterial.Diffuse;
		scene->Materials[i].Ambient       = pmxMaterial.Ambient;
		scene->Materials[i].Specular      = pmxMaterial.Specular;
		scene->Materials[i].SpecularPower = pmxMaterial.SpecularPower;

		// テクスチャの読み込み
		if (pmxMaterial.DiffuseTextureIndex >= 0)
		{
			scene->Materials[i].DiffuseMap = gu::Optional<sf::SFTexture>(file->TexturePathList[pmxMaterial.DiffuseTextureIndex]);
		}
	}

	return scene;
}

/*!**********************************************************************
*  @brief     各拡張子に基づいてシーンを保存します
*  @param[in] const gu::tstring& ファイルパス
*  @return    bool
*************************************************************************/
void PMXSceneProcessor::Export([[maybe_unused]] const gu::tstring& filePath)
{

}
#pragma endregion Public Function

#pragma region Private Function
/*!**********************************************************************
*  @brief     メッシュを作成します.
*  @param[in] const gu::SharedPointer<PMXFile> モデルデータ
*  @param[out] const sf::SFMesh& メッシュ
*  @param[in] const gu::uint64 開始Index
*  @param[in] const gu::uint64 Indexの個数
*  @return    void
*************************************************************************/
void PMXSceneProcessor::CreateMesh(const gu::SharedPointer<PMXFile>& file, engine::file::sf::SFMesh& mesh, const gu::uint64 indexStart, const gu::uint64 indexCount)
{
	mesh.Positions    .Resize(indexCount, false);
	mesh.Normals      .Resize(indexCount, false);
	mesh.TextureCoords.Resize(indexCount, false);

	for (uint64 i = 0; i < indexCount; ++i)
	{
		const auto  index = file->Indices[indexStart + i];
		//printf("index: %d\n", index);
		const auto& pmxVertex = file->Vertices[index];
		mesh.Positions    [i] = pmxVertex.Position;
		mesh.Normals      [i] = pmxVertex.Normal;
		mesh.TextureCoords[i] = gm::Float3(pmxVertex.UV.x, pmxVertex.UV.y, 0.0f);
	}
}


#pragma endregion Private Function