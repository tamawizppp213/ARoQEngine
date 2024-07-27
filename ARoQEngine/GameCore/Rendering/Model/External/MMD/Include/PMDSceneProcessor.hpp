//////////////////////////////////////////////////////////////////////////////////
///  @file   MMDModelImporter.hpp
///  @brief  temp
///  @author toide
///  @date   2024/07/21 23:20:08
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef PMD_SCENE_IMPORTER_HPP
#define PMD_SCENE_IMPORTER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/Private/SceneFile/Include/SFSceneProcessor.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace engine::file::pmd
{
	/****************************************************************************
	*				  			   MMDModelImporter
	****************************************************************************/
	/* @brief  temp
	*****************************************************************************/
	class PMDSceneProcessor : public sf::ISceneProcessor
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     各拡張子に基づいてシーンを読み込みます
		*  @param[in] const gu::tstring& ファイルパス
		*  @return    gu::SharedPointer<SFScene>
		*************************************************************************/
		virtual gu::SharedPointer<sf::SFScene> Import(const gu::tstring& filePath) override;

		/*!**********************************************************************
		*  @brief     各拡張子に基づいてシーンを保存します
		*  @param[in] const gu::tstring& ファイルパス
		*  @return    bool
		*************************************************************************/
		virtual void Export([[maybe_unused]]const gu::tstring& filePath) override;

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

		#pragma endregion

		#pragma region Private Function

		#pragma endregion 

		#pragma region Private Property

		#pragma endregion 

	};
}

#endif