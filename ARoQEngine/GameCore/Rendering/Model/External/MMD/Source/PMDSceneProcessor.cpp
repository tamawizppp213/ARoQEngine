//////////////////////////////////////////////////////////////////////////////////
///  @file   PMDSceneProcessor.cpp
///  @brief  �g���q��.pmd�̃t�@�C������, ���ʂ�SceneFileMesh�ɑ��ݕϊ�����N���X. Import���t�@�C���ǂݍ���, Export�������o��
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
*  @brief     �e�g���q�Ɋ�Â��ăV�[����ǂݍ��݂܂�
*  @param[in] const gu::tstring& �t�@�C���p�X
*  @return    gu::SharedPointer<SFScene>
*************************************************************************/
gu::SharedPointer<sf::SFScene> PMDSceneProcessor::Import(const gu::tstring& filePath)
{
	/*-------------------------------------------------------------------
	-       PMD�t�@�C����ǂݍ���
	---------------------------------------------------------------------*/
	const auto file = gu::MakeShared<PMDFile>();
	
	if (!file->Read(filePath))
	{
		return nullptr;
	}

	/*-------------------------------------------------------------------
	-       �V�[���̃C���|�[�g
	---------------------------------------------------------------------*/
	const auto scene = gu::MakeShared<sf::SFScene>();

	/*-------------------------------------------------------------------
	-       ���b�V���f�[�^�̎擾
	---------------------------------------------------------------------*/
	scene->Meshes.Resize(file->Materials.Size());

	uint64 indexStart = 0;
	for (uint64 i = 0; i < scene->Meshes.Size(); ++i)
	{
		// material���ɒ��_���b�V�����擾����
		const auto indexCount = static_cast<uint64>(file->Materials[i].IndexCount);

		CreateMesh(file, scene->Meshes[i], indexStart, indexCount);
		scene->Meshes[i].MaterialIndex = static_cast<uint32>(i);
		scene->Meshes[i].Name = platform::core::IFileSystem::Get()->GetFileName(file->Materials[i].TexturePath);

		indexStart += indexCount;
	}

	/*-------------------------------------------------------------------
	-       �}�e���A���̎擾
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

		// �e�N�X�`���̓ǂݍ���
		if (!pmdMaterial.TexturePath.IsEmpty())
		{
			scene->Materials[i].DiffuseMap = gu::Optional<sf::SFTexture>(file->Materials[i].TexturePath);
		}
	}

	return scene;
}

/*!**********************************************************************
*  @brief     �e�g���q�Ɋ�Â��ăV�[����ۑ����܂�
*  @param[in] const gu::tstring& �t�@�C���p�X
*  @return    bool
*************************************************************************/
void PMDSceneProcessor::Export([[maybe_unused]]const gu::tstring& filePath)
{
	
}
#pragma endregion Public Function

#pragma region Private Function
#pragma region Private Function
/*!**********************************************************************
*  @brief     ���b�V�����쐬���܂�.
*  @param[in] const gu::SharedPointer<PMXFile> ���f���f�[�^
*  @param[out] const sf::SFMesh& ���b�V��
*  @param[in] const gu::uint64 �J�nIndex
*  @param[in] const gu::uint64 Index�̌�
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