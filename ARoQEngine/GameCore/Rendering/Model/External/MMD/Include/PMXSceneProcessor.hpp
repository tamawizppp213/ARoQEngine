//////////////////////////////////////////////////////////////////////////////////
///  @file   PMXModelImporter.hpp
///  @brief  temp
///  @author toide
///  @date   2024/07/22 1:26:19
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef PMX_SCENE_IMPORTER_HPP
#define PMX_SCENE_IMPORTER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/Private/SceneFile/Include/SFSceneProcessor.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
namespace engine::file::sf
{
	struct SFMesh;
	struct SFMaterial;
}
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace engine::file::pmx
{
	struct PMXFile;

	/****************************************************************************
	*				  			   MMDModelImporter
	****************************************************************************/
	/* @brief  temp
	*****************************************************************************/
	class PMXSceneProcessor : public sf::ISceneProcessor
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     �e�g���q�Ɋ�Â��ăV�[����ǂݍ��݂܂�
		*  @param[in] const gu::tstring& �t�@�C���p�X
		*  @return    gu::SharedPointer<SFScene>
		*************************************************************************/
		virtual gu::SharedPointer<sf::SFScene> Import(const gu::tstring& filePath) override;

		/*!**********************************************************************
		*  @brief     �e�g���q�Ɋ�Â��ăV�[����ۑ����܂�
		*  @param[in] const gu::tstring& �t�@�C���p�X
		*  @return    bool
		*************************************************************************/
		virtual void Export([[maybe_unused]] const gu::tstring& filePath) override;

		#pragma endregion 

		#pragma region Public Property

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor

		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor

		#pragma endregion 

		#pragma region Protected Function

		#pragma endregion 

		#pragma region Protected Property

		#pragma endregion

	private:
		#pragma region Private Constructor and Destructor
		/*!**********************************************************************
		*  @brief     ���b�V�����쐬���܂�. 
		*  @param[in]  const gu::SharedPointer<PMXFile> ���f���f�[�^
		*  @param[out] const sf::SFMesh& ���b�V��
		*  @param[in]  const gu::uint64 �J�nIndex
		*  @param[in]  const gu::uint64 Index�̌�
		*  @return     void
		*************************************************************************/
		void CreateMesh(const gu::SharedPointer<PMXFile>& file, sf::SFMesh& mesh, const gu::uint64 indexStart, const gu::uint64 indexCount);

		#pragma endregion

		#pragma region Private Function

		#pragma endregion 

		#pragma region Private Property

		#pragma endregion 

	};
}


#endif