//////////////////////////////////////////////////////////////////////////////////
///  @file   SFMesh.hpp
///  @brief  temp
///  @author toide
///  @date   2024/07/21 23:18:28
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SF_MESH_HPP
#define SF_MESH_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Math/Include/GMVector.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
#include "GameUtility/Container/Include/GUHashMap.hpp"
#include "GameUtility/Base/Include/GUString.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace engine::file::sf
{
	/****************************************************************************
	*				  			   SFMesh
	****************************************************************************/
	/* @brief  外部ファイルから読み込む際の共通メッシュデータ. GameEngineに直接格納するのではなく, @n 
	*          あくまでモデルデータの一時的な保存を目的とする. 
	*****************************************************************************/
	struct SFMesh
	{
	public:
		#pragma region Public Function

		#pragma endregion 

		#pragma region Public Property
		/*! @brief 3次元位置 [メッシュには常に格納される]*/
		gu::DynamicArray<gm::Float3> Positions = {};

		/*! @brief 3次元の正規化された法線ベクトル. 点と線だけのメッシュは法線ベクトルは持てない. */
		gu::DynamicArray<gm::Float3> Normals = {};

		/*! @brief テクスチャのX軸方向を指す. 正規化された接線ベクトル*/
		gu::DynamicArray<gm::Float3> Tangents = {};

		/*! @brief テクスチャのY軸方向を指す. 正規化された接線ベクトル*/
		gu::DynamicArray<gm::Float3> Bitangents = {};

		/*! @brief 頂点カラー*/
		gu::DynamicArray<gm::Float4> Colors = {};

		/*! @brief テクスチャのUVW値*/
		gu::DynamicArray<gm::Float3> TextureCoords = {};

		/*! @brief このメッシュに適用されるマテリアルIndex*/
		gu::uint32 MaterialIndex = 0;

		/*! @brief メッシュの名前*/
		gu::tstring Name = SP("");

		/*!**********************************************************************
		*  @brief     3次元位置を取得しているか
		*  @param[in] void
		*  @return    bool 3次元位置を取得している場合はtrue, そうでない場合はfalse
		*************************************************************************/
		__forceinline bool HasPositions() const noexcept { return !Positions.IsEmpty(); }

		/*!**********************************************************************
		*  @brief     法線ベクトルを取得しているか
		*  @param[in] void
		*  @return    bool 法線ベクトルを取得している場合はtrue, そうでない場合はfalse
		*************************************************************************/
		__forceinline bool HasNormals() const noexcept { return !Normals.IsEmpty(); }

		/*!**********************************************************************
		*  @brief     接線ベクトルを取得しているか
		*  @param[in] void
		*  @return    bool 接線ベクトルを取得している場合はtrue, そうでない場合はfalse
		*************************************************************************/
		__forceinline bool HasTangents() const noexcept { return !Tangents.IsEmpty(); }

		/*!**********************************************************************
		* @brief     接線ベクトルを取得しているか
		* @param[in] void
		* @return    bool 接線ベクトルを取得している場合はtrue, そうでない場合はfalse
		*************************************************************************/
		__forceinline bool HasBitangents() const noexcept { return !Bitangents.IsEmpty(); }

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