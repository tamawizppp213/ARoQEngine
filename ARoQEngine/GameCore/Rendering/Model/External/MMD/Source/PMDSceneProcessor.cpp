//////////////////////////////////////////////////////////////////////////////////
///  @file   PMDSceneProcessor.cpp
///  @brief  拡張子が.pmdのファイルから, 共通のSceneFileMeshに相互変換するクラス. Importがファイル読み込み, Exportが書き出し
///  @author toide
///  @date   2024/07/23 0:43:27
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/MMD/Include/PMDSceneProcessor.hpp"
#include "GameCore/Rendering/Model/External/MMD/Include/PMDParser.hpp"
#include "GameCore/Rendering/Model/Private/SceneFile/Include/SFScene.hpp"
#include "Platform/Core/Include/CoreFileSystem.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace engine::file::pmd;
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
gu::SharedPointer<sf::SFScene> PMDSceneProcessor::Import(const gu::tstring& filePath)
{
	/*-------------------------------------------------------------------
	-       PMDファイルを読み込む
	---------------------------------------------------------------------*/
	const auto file = gu::MakeShared<PMDFile>();
	
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
		scene->Meshes[i].Name = platform::core::IFileSystem::Get()->GetFileName(file->Materials[i].TexturePath);

		indexStart += indexCount;
	}

	/*-------------------------------------------------------------------
	-       マテリアルの取得
	---------------------------------------------------------------------*/
	scene->Materials.Resize(file->Materials.Size(), false);
	for (uint32 i = 0; i < file->Materials.Size(); ++i)
	{
		const auto& pmdMaterial = file->Materials[i];

		scene->Materials[i].Name          = platform::core::IFileSystem::Get()->GetFileName(file->Materials[i].TexturePath);
		scene->Materials[i].Diffuse       = pmdMaterial.Diffuse;
		scene->Materials[i].Ambient       = pmdMaterial.Ambient;
		scene->Materials[i].Specular      = pmdMaterial.Specular;
		scene->Materials[i].SpecularPower = pmdMaterial.SpecularPower;

		// テクスチャの読み込み
		if (!pmdMaterial.TexturePath.IsEmpty())
		{
			scene->Materials[i].DiffuseMap = gu::Optional<sf::SFTexture>(file->Materials[i].TexturePath);
		}
	}

	return scene;
}

/*!**********************************************************************
*  @brief     各拡張子に基づいてシーンを保存します
*  @param[in] const gu::tstring& ファイルパス
*  @return    bool
*************************************************************************/
void PMDSceneProcessor::Export([[maybe_unused]]const gu::tstring& filePath)
{
	
}
#pragma endregion Public Function

#pragma region Private Function
#pragma region Private Function
/*!**********************************************************************
*  @brief     メッシュを作成します.
*  @param[in] const gu::SharedPointer<PMXFile> モデルデータ
*  @param[out] const sf::SFMesh& メッシュ
*  @param[in] const gu::uint64 開始Index
*  @param[in] const gu::uint64 Indexの個数
*  @return    void
*************************************************************************/
void PMDSceneProcessor::CreateMesh(const gu::SharedPointer<PMDFile>& file, engine::file::sf::SFMesh& mesh, const gu::uint64 indexStart, const gu::uint64 indexCount)
{
	mesh.Positions    .Resize(indexCount, false);
	mesh.Normals      .Resize(indexCount, false);
	mesh.TextureCoords.Resize(indexCount, false);

	for (uint64 i = 0; i < indexCount; ++i)
	{
		const auto  index = file->Indices[indexStart + i];

		const auto& pmxVertex = file->Vertices[index];
		mesh.Positions    [i] = pmxVertex.Position;
		mesh.Normals      [i] = pmxVertex.Normal;
		mesh.TextureCoords[i] = gm::Float3(pmxVertex.UV.x, pmxVertex.UV.y, 0.0f);
	}
}

#pragma endregion Private Function