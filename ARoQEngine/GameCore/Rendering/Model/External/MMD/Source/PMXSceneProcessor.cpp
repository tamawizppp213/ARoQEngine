//////////////////////////////////////////////////////////////////////////////////
///  @file   PMXSceneProcessor.cpp
///  @brief  �g���q��.pmx�̃t�@�C������, ���ʂ�SceneFileMesh�ɑ��ݕϊ�����N���X. Import���t�@�C���ǂݍ���, Export�������o��
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
*  @brief     �e�g���q�Ɋ�Â��ăV�[����ǂݍ��݂܂�
*  @param[in] const gu::tstring& �t�@�C���p�X
*  @return    gu::SharedPointer<SFScene>
*************************************************************************/
gu::SharedPointer<sf::SFScene> PMXSceneProcessor::Import(const gu::tstring& filePath)
{
	/*-------------------------------------------------------------------
	-       PMX�t�@�C����ǂݍ���
	---------------------------------------------------------------------*/
	const auto file = gu::MakeShared<PMXFile>();

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
		scene->Meshes[i].Name = file->Materials[i].NameJapanese;

		indexStart += indexCount;
	}

	/*-------------------------------------------------------------------
	-       �}�e���A���̎擾
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

		// �e�N�X�`���̓ǂݍ���
		if (pmxMaterial.DiffuseTextureIndex >= 0)
		{
			scene->Materials[i].DiffuseMap = gu::Optional<sf::SFTexture>(file->TexturePathList[pmxMaterial.DiffuseTextureIndex]);
		}
	}

	return scene;
}

/*!**********************************************************************
*  @brief     �e�g���q�Ɋ�Â��ăV�[����ۑ����܂�
*  @param[in] const gu::tstring& �t�@�C���p�X
*  @return    bool
*************************************************************************/
void PMXSceneProcessor::Export([[maybe_unused]] const gu::tstring& filePath)
{

}
#pragma endregion Public Function

#pragma region Private Function
/*!**********************************************************************
*  @brief     ���b�V�����쐬���܂�.
*  @param[in] const gu::SharedPointer<PMXFile> ���f���f�[�^
*  @param[out] const sf::SFMesh& ���b�V��
*  @param[in] const gu::uint64 �J�nIndex
*  @param[in] const gu::uint64 Index�̌�
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