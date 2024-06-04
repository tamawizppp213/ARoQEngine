//////////////////////////////////////////////////////////////////////////////////
///  @file   PMDParser.hpp
///  @brief  PMD File Parser
///  @author Toide Yutaro
///  @date   2024_05_29
/////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef PMD_PARSER_HPP
#define PMD_PARSER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/MMD/Private/Include/PMDDefines.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
namespace platform::core::file
{
	class IFileHandle;
}

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc::file::pmd
{
	/****************************************************************************
	*				  			PMDFile
	*************************************************************************//**
	/*  @brief  PMDのファイルを読み込むクラスです. 
	*****************************************************************************/
	struct PMDFile : public gu::NonCopyable
	{
	public:
		#pragma region Public Function

		/*!**********************************************************************
		*  @brief     PMDファイルを読み込む関数
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @return    void
		*************************************************************************/
		bool Read(const gu::tstring& filePath);

		#pragma endregion

		#pragma region Public Property
		/*! @brief モデルデータのヘッダ情報. モデル名, バージョン, コメントなどが入っています*/
		details::PMDHeader Header = {};

		/*! @brief モデルの頂点情報 (頂点の最大数は65536です)*/
		gu::DynamicArray<details::PMDVertex> Vertices = {};

		/*! @brief モデルのインデックス情報 (頂点数の制約により, uint16となっています.) */
		gu::DynamicArray<gu::uint16> Indices = {};

		/*! @brief モデルのマテリアル情報 */
		gu::DynamicArray<details::PMDMaterial> Materials = {};

		/*! @brief ボーンの名前, 親ボーン番号, 子ボーン番号, ボーンの種類, IKボーン番号, ボーンの先頭位置が入っています.*/
		gu::DynamicArray<details::PMDBone> Bones = {};

		/*! @brief IKボーンの情報 */
		gu::DynamicArray<details::PMDBoneIK> IKs = {};

		/*! @brief 表情の名前, 表情の頂点情報, 表情の種類が入っています.*/
		gu::DynamicArray<details::PMDFaceExpression>  FaceExpressions = {};

		/*! @brief 複数の表情が存在する場合にその種類を指定するインデックスです*/
		gu::DynamicArray<gu::uint16> FaceLabelIndices = {};

		/*! @brief ボーンの表示名 */
		gu::DynamicArray<details::PMDBoneDisplayName> BoneDisplayNameList = {};

		/*! @brief ボーンの表示リスト */
		gu::DynamicArray<details::PMDBoneNameTable> BoneNameTable = {};

		/*! @brief 剛体*/
		gu::DynamicArray<details::PMDRigidBody> RigidBodies = {};

		/*! @brief 関節*/
		gu::DynamicArray<details::PMDJoint> Joints = {};

		/*! @brief トゥーンテクスチャを使用する場合のパスが格納されているリスト*/
		gu::DynamicArray<gu::tstring> ToonTextureList = {};

		/*! @brief PMDファイルが格納されている親ディレクトリ*/
		gu::tstring Directory = SP("");

		#pragma endregion

		#pragma region Public Constructor and Destructor
		/*! @brief デフォルトコンストラクタ */
		PMDFile() = default;

		/*! @brief デストラクタ */
		~PMDFile();

		#pragma endregion

	private:
		#pragma region Private Function
		void ReadLocalizeData       (const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);
		void ReadToonTextures       (const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);
		void ReadPhysics            (const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);
		#pragma endregion 
		
		#pragma region Private Property
		#pragma endregion
	};

}
#endif