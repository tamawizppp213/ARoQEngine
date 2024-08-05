//////////////////////////////////////////////////////////////////////////////////
///  @file   SFScene.hpp
///  @brief  temp
///  @author toide
///  @date   2024/07/21 23:18:03
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SF_SCENE_NODE_HPP
#define SF_SCENE_NODE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "SFMesh.hpp"
#include "SFMaterial.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace engine::file::sf
{
	/****************************************************************************
	*				  			   SFScene
	****************************************************************************/
	/* @brief  シーン情報を一時的に格納するための構造体. 
	*****************************************************************************/
	struct SFSceneNode
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     対象の名前のSceneNode確認
		*  @param[in] const gu::tstring& メッシュ名
		*  @return    SFSceneNode*
		*************************************************************************/
		SFSceneNode* FindNode(const gu::tstring& name);

		/*!**********************************************************************
		*  @brief     対象の名前のSceneNode確認
		*  @param[in] const gu::tstring& メッシュ名
		*  @return    SFSceneNode*
		*************************************************************************/
		const SFSceneNode* FindNode(const gu::tstring& name) const;

		#pragma endregion 

		#pragma region Public Property
		/*! @brief 親ノード*/
		SFSceneNode* Parent = nullptr;

		/*! @brief 子ノード*/
		gu::DynamicArray<SFSceneNode*> Children = {};

		/*! @brief ノード名*/
		gu::tstring Name = SP("");

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief デフォルトコンストラクタ*/
		SFSceneNode() = default;

		/*! @brief デストラクタ*/
		~SFSceneNode() = default;

		explicit SFSceneNode(const gu::tstring& name);
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