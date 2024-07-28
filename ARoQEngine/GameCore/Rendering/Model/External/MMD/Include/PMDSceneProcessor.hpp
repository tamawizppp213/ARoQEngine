//////////////////////////////////////////////////////////////////////////////////
///  @file   PMDSceneProcessor.hpp
///  @brief  拡張子が.pmdのファイルから, 共通のSceneFileMeshに相互変換するクラス. Importがファイル読み込み, Exportが書き出し
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
namespace engine::file::sf
{
	struct SFMesh;
	struct SFMaterial;
}
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace engine::file::pmd
{
	struct PMDFile;

	/****************************************************************************
	*				  			  PMDSceneProcessor
	****************************************************************************/
	/* @brief  拡張子が.pmdのファイルから, 共通のSceneFileMeshに相互変換するクラス. Importがファイル読み込み, Exportが書き出し
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
		/*!**********************************************************************
		*  @brief     メッシュを作成します.
		*  @param[in]  const gu::SharedPointer<PMXFile> モデルデータ
		*  @param[out] const sf::SFMesh& メッシュ
		*  @param[in]  const gu::uint64 開始Index
		*  @param[in]  const gu::uint64 Indexの個数
		*  @return     void
		*************************************************************************/
		void CreateMesh(const gu::SharedPointer<PMDFile>& file, sf::SFMesh& mesh, const gu::uint64 indexStart, const gu::uint64 indexCount);
		#pragma endregion 

		#pragma region Private Property

		#pragma endregion 

	};
}

#endif