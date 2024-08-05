//////////////////////////////////////////////////////////////////////////////////
///  @file   SceneImporter.hpp
///  @brief  �O��3D���f��, 3D�V�[���̃f�[�^�t�@�C����ǂݍ��݂܂�.�@
///  @author toide
///  @date   2024/07/21 22:32:34
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef EXTERNAL_SCENE_PROCESSOR_HPP
#define EXTERNAL_SCENE_PROCESSOR_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Base/Include/GUString.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
namespace engine::file::sf
{
	class ISceneProcessor;
	struct SFScene;
}

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace engine
{
	/****************************************************************************
	*				  			 ExternalSceneProcessor
	****************************************************************************/
	/* @brief  �O���t�@�C���̃V�[���ǂݍ��݂��s���N���X
	*****************************************************************************/
	class ExternalSceneProcessor : public gu::NonCopyable
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     �e�g���q�Ɋ�Â��ăV�[����ǂݍ��݂܂�
		*  @param[in] const gu::tstring& �t�@�C���p�X
		*  @return    gu::SharedPointer<file::sf::ISceneProcessor>
		*************************************************************************/
		static gu::SharedPointer<file::sf::ISceneProcessor> Import(const gu::tstring& filePath);

		/*!**********************************************************************
		*  @brief     �e�g���q�Ɋ�Â��ăV�[����ۑ����܂�
		*  @param[in] const gu::tstring& �t�@�C���p�X
		*  @return    bool
		*************************************************************************/
		static void Export(const gu::tstring& filePath);

		#pragma endregion 

		#pragma region Public Property

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		ExternalSceneProcessor() = default;

		/*! @brief �f�X�g���N�^*/
		~ExternalSceneProcessor() = default;
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

		#pragma endregion

		#pragma region Private Function

		#pragma endregion 

		#pragma region Private Property
		
		#pragma endregion 

	};
}

#endif