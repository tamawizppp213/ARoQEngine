//////////////////////////////////////////////////////////////////////////////////
///  @file   PMXParser.hpp
///  @brief  PMXファイルを読み込むクラス
///  @author Toide Yutaro
///  @date   2024_06_02
/////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef PMX_PARSER_HPP
#define PMX_PARSER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/MMD/Private/Include/PMXDefines.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Class
//////////////////////////////////////////////////////////////////////////////////
namespace engine::file::pmx
{
	
	/****************************************************************************
	*				  			PMXFile
	****************************************************************************/
	/*  @brief  PMXのファイルを読み込むクラスです.
	*****************************************************************************/
	struct PMXFile : public gu::NonCopyable
	{
	public:
		#pragma region Public Function

		/*!**********************************************************************
		*  @brief     PMXファイルを読み込む関数
		*  @param[in] const gu::tstring& ファイルパス
		*  @param[in] const bool 非同期で読み込むかどうか
		*  @return    bool
		*************************************************************************/
		bool Read(const gu::tstring& filePath, const bool useAsync = false);

		#pragma endregion

		#pragma region Public Property
		/*! @brief モデルデータのヘッダ情報, ファイルのバージョン情報を格納します.*/
		PMXHeader Header = {};

		/*! @brief モデルの頂点情報*/
		gu::DynamicArray<PMXVertex> Vertices = {};

		/*! @brief モデルのインデックス情報*/
		gu::DynamicArray<gu::uint32> Indices = {};

		/*! @brief モデルのマテリアル情報*/
		gu::DynamicArray<PMXMaterial> Materials = {};

		/*! @brief モデルのボーン情報*/
		gu::DynamicArray<PMXBone> Bones = {};

		/*! @brief モデルのモーフ情報*/
		gu::DynamicArray<PMXMorph> Morphs = {};

		/*! @brief モデルの表示枠情報*/
		gu::DynamicArray<PMXDisplayFrame> DisplayFrames = {};

		/*! @brief モデルの剛体情報*/
		gu::DynamicArray<PMXRigidBody> RigidBodies = {};

		/*! @brief モデルのジョイント情報*/
		gu::DynamicArray<PMXJoint> Joints = {};

		/*! @brief モデルのソフトボディ情報*/
		gu::DynamicArray<PMXSoftBody> SoftBodies = {};

		/*! @brief モデルのテクスチャリスト*/
		gu::DynamicArray<gu::tstring> TexturePathList = {};

		/*! @brief PMXファイルが格納されている親ディレクトリ*/
		gu::tstring Directory = SP("");
		#pragma endregion

		#pragma region Public Constructor and Destructor
		
		/*! @brief デフォルトコンストラクタ*/
		PMXFile() = default;

		/*! @brief デストラクタ*/
		~PMXFile();

		#pragma endregion

	private:
		#pragma region Private Function
		#pragma endregion

		#pragma region Private Property
		#pragma endregion
	};

}

#endif