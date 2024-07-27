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
		*  @brief     メッシュを作成します. 
		*  @param[in]  const gu::SharedPointer<PMXFile> モデルデータ
		*  @param[out] const sf::SFMesh& メッシュ
		*  @param[in]  const gu::uint64 開始Index
		*  @param[in]  const gu::uint64 Indexの個数
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