//////////////////////////////////////////////////////////////////////////////////
///  @file   SFScene.hpp
///  @brief  temp
///  @author toide
///  @date   2024/07/21 23:18:03
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SF_SCENE_HPP
#define SF_SCENE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "SFSceneNode.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace engine::file::sf
{
	/****************************************************************************
	*				  			   SFSceneFlag
	****************************************************************************/
	/* @brief  シーン情報を作成する時のフラグ
	*****************************************************************************/
	enum class SFSceneFlag : gu::uint8
	{
		None = 0,		//!< 何もしない

	};

	/****************************************************************************
	*				  			   SFScene
	****************************************************************************/
	/* @brief  シーン情報を一時的に格納するための構造体. 
	*****************************************************************************/
	struct SFScene
	{
	public:
		#pragma region Public Function
		
		#pragma endregion 

		#pragma region Public Property
		/*! @brief Root. Importが成功した場合, 常にRootNodeは存在します. */
		SFSceneNode* RootSceneNode = nullptr;

		/*! @brief 3次元メッシュ[マテリアルの数分だけ作成されます.*/
		gu::DynamicArray<SFMesh> Meshes = {};

		/*! @brief マテリアル*/
		gu::DynamicArray<SFMaterial> Materials = {};

		/*! @brief テクスチャ*/
		gu::DynamicArray<SFTexture> Textures = {};

		/*! @brief シーン生成フラグ*/
		SFSceneFlag Flag = SFSceneFlag::None;
		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief デフォルトコンストラクタ*/
		SFScene() = default;

		/*! @brief デストラクタ*/
		~SFScene() = default;
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