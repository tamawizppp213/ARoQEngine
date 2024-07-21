//////////////////////////////////////////////////////////////////////////////////
///  @file   PMXDefines.hpp
///  @brief  temp
///  @author toide
///  @date   2024/05/31 0:33:40
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef PMX_DEFINES_HPP
#define PMX_DEFINES_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUString.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
#include "GameUtility/Math/Include/GMVector.hpp"
#include "GameUtility/Base/Include/GUEnumClassFlags.hpp"

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

namespace engine::file::pmx
{
	/****************************************************************************
	*				  			PMXEncode
	****************************************************************************/
	/*  @brief  文字コードのエンコーディングの種類を指定します. 
	*****************************************************************************/
	enum class PMXEncode : gu::uint8
	{
		UTF16 = 0,
		UTF8  = 1
	};

	/****************************************************************************
	*				  		PMXHeader
	****************************************************************************/
	/*  @brief   モデルデータのヘッダ情報, ファイルのバージョン情報を格納します.
	*****************************************************************************/
	struct PMXHeader
	{
		/*! @brief ファイル識別子 (PMX というヘッダ名になっているか). ASCIIで0x50,0x4d,0x58,0x20*/
		char Signature[4] = {};

		/*! @brief バージョン番号 (2.0 か 2.1 か). */
		gu::float32 Version = 0.0f;

		/*! @brief 後続のデータ列(ヘッダ情報)のバイトサイズ*/
		gu::uint8 SubsequentDataSize = 0;

		/*! @brief エンコード方式 (0:UTF16, 1:UTF8) */
		PMXEncode Encode = PMXEncode::UTF16;

		/*! @brief 追加のUV数 (0～4までの範囲)*/
		gu::uint8 AdditionalUVCount = 0;

		/*! @brief 頂点インデックスサイズ (1, 2, 4のいずれか)*/
		gu::uint8 VertexIndexSize = 0;

		/*! @brief テクスチャインデックスサイズ (1, 2, 4のいずれか)*/
		gu::uint8 TextureIndexSize = 0;

		/*! @brief マテリアルインデックスサイズ (1, 2, 4のいずれか)*/
		gu::uint8 MaterialIndexSize = 0;

		/*! @brief ボーンインデックスサイズ (1, 2, 4のいずれか)*/
		gu::uint8 BoneIndexSize = 0;

		/*! @brief モーフインデックスサイズ (1, 2, 4のいずれか)*/
		gu::uint8 MorphIndexSize = 0;

		/*! @brief 剛体インデックスサイズ (1, 2, 4のいずれか)*/
		gu::uint8 RigidBodyIndexSize = 0;

		/*! @brief OtherData*/
		gu::DynamicArray<gu::uint8> OtherData = {};

		/*! @brief モデル名(日本語)*/
		gu::tstring NameJapanese    = SP("");

		/*! @brief モデル名(英語)*/
		gu::tstring NameEnglish     = SP("");

		/*! @brief 著作権情報のようなコメント(日本語) */
		gu::tstring CommentJapanese = SP("");

		/*! @brief 著作権情報のようなコメント(英語) */
		gu::tstring CommentEnglish  = SP("");

		/*!**********************************************************************
		*  @brief     正しいファイルヘッダになっているか (Pmx というヘッダ名になっているか)
		*  @param[in] void
		*  @return    bool : Pmx というヘッダ名になっている場合にtrueを返します.
		*************************************************************************/
		bool IsValid() const;

		/*!**********************************************************************
		*  @brief     pmxファイルをもとにデータ読み込み
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);
	};

	/****************************************************************************
	*				  			PMXVertexWeightType
	****************************************************************************/
	/*  @brief  ボーンウェイトの格納情報について定義します. 
	*****************************************************************************/
	enum class PMXBoneWeightType : gu::uint8
	{
		BDEF1 = 0, //!< ボーンのみ
		BDEF2 = 1, //!< ボーン2つとボーン1のウェイト値 (PMD方式)
		BDEF4 = 2, //!< ボーン4つとそれぞれのウェイト値. ウェイト合計が1.0であるとする保証は無い
		SDEF  = 3, //!< BDEF2と同じだが, SDEF用のfloat3が3つ追加されている. 実際の計算では補正値の算出も必要
		QDEF  = 4, //!< boneIndices[0-3] + boneWeights[0-3])
	};

	/****************************************************************************
	*				  		PMXVertex
	****************************************************************************/
	/*  @brief   PMXファイルの頂点情報を格納します.
	*****************************************************************************/
	struct PMXVertex
	{
		/*! @brief 頂点の位置情報 */
		gm::Float3 Position = {};

		/*! @brief 頂点の法線 */
		gm::Float3 Normal = {};

		/*! @brief テクスチャマッピングに使用されるUV座標 */
		gm::Float2 UV = {};

		/*! @brief 追加のUV座標 */
		gm::Float4 AdditionalUV[4] = {};

		/*! @brief 頂点のウェイトタイプ */
		PMXBoneWeightType WeightType = PMXBoneWeightType::BDEF1;

		/*! @brief ボーンインデックス */
		gu::int32 BoneIndex[4] = {};

		/*! @brief ボーンウェイト値 */
		gu::float32 BoneWeight[4] = {};

		/*! @brief エッジ倍率 */
		gu::float32 EdgeScale = 0.0f;

		/*! @brief SDEFで使用する定数値*/
		gm::Float3 SDEF_C = 0.0f;

		/*! @brief SDEFで使用するR0*/
		gm::Float3 SDEF_R0 = {};

		/*! @brief SDEFで使用するR1*/
		gm::Float3 SDEF_R1 = {};

		/*! @brief ファイルハンドルを使った初期化*/
		PMXVertex(const gu::SharedPointer<platform::core::file::IFileHandle>& handle, const PMXHeader& header)
		{
			Read(handle, header);
		}

		/*!**********************************************************************
		*  @brief     pmxファイルをもとにデータ読み込み
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @param[in] const PMXHeader ヘッダ情報
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header);
	};

	/****************************************************************************
	*				  		 PMXDrawModeFlags
	****************************************************************************/
	/*  @brief  DrawModeのフラグを定義します.
	*****************************************************************************/
	enum class PMXDrawModeFlags : gu::uint8
	{
		None           = 0x00, //!< フラグ無効
		BothSurface    = 0x01, //!< 両面描画
		GroundShadow   = 0x02, //!< 地面影
		CastSelfShadow = 0x04, //!< セルフシャドウマップへの描画
		ReceiveShadow  = 0x08, //!< セルフシャドウマップを受け取ってキャラ自身に描画する
		Edge           = 0x10, //!< エッジ描画
		VetexColor     = 0x20, //!< 頂点カラー (2.1拡張)
		Point          = 0x40, //!< ポイント描画, 点群描画 (2.1拡張)
		Line           = 0x80, //!< 線描画 (2.1拡張)
	};
	ENUM_CLASS_FLAGS(PMXDrawModeFlags);

	/****************************************************************************
	*				  		 PMXSphereMapOperator
	****************************************************************************/
	/*  @brief  スフィアマップ表示時の計算方法を指定します. 
	*****************************************************************************/
	enum class PMXSphereMapOperator : gu::uint8
	{
		Invalid    = 0, //!< 無効
		Multiply   = 1, //!< 乗算
		Add        = 2, //!< 加算
		SubTexture = 3, //!< サブテクスチャ
	};

	/****************************************************************************
	*				  		 PMXToonTextureMode
	****************************************************************************/
	/*  @brief  トゥーンテクスチャの種類を指定します.
	*****************************************************************************/
	enum class PMXToonTextureMode : gu::uint8
	{
		Separate = 0, //!< 個別テクスチャ
		Common   = 1, //!< 共有テクスチャ
	};

	/****************************************************************************
	*				  		PMXMaterial
	****************************************************************************/
	/*  @brief   PMXファイルのマテリアル情報を格納します.
	*****************************************************************************/
	struct PMXMaterial
	{
		/*! @brief マテリアル名(日本語)*/
		gu::tstring NameJapanese = SP("");

		/*! @brief マテリアル名(英語)*/
		gu::tstring NameEnglish = SP("");

		/*! @brief メモ*/
		gu::tstring Memo = SP("");

		/*! @brief マテリアルの拡散色 */
		gm::Float4 Diffuse = {};

		/*! @brief マテリアルの反射色 */
		gm::Float3 Specular = {};

		/*! @brief マテリアルの反射強度 */
		gu::float32 SpecularPower = 0.0f;

		/*! @brief マテリアルの環境色 */
		gm::Float3 Ambient = {};

		/*! @brief マテリアルの描画フラグ */
		PMXDrawModeFlags DrawMode = PMXDrawModeFlags::None;

		/*! @brief マテリアルのエッジ色 */
		gm::Float4 EdgeColor = {};

		/*! @brief マテリアルのエッジサイズ */
		gu::float32 EdgeSize = 0.0f;

		/*! @brief マテリアルのテクスチャインデックス */
		gu::int32 DiffuseTextureIndex = 0;

		/*! @brief マテリアルのスフィアマップテクスチャインデックス */
		gu::int32 SphereMapTextureIndex = 0;

		/*! @brief スフィアマップの計算方法 */
		PMXSphereMapOperator SphereMapMode = PMXSphereMapOperator::Invalid;

		/*! @brief トゥーンテクスチャの種類 */
		PMXToonTextureMode ToonTextureMode = PMXToonTextureMode::Separate;

		/*! @brief トゥーンテクスチャのインデックス */
		gu::int32 ToonTextureIndex = 0;

		/*! @brief 面の個数 (必ず3の倍数になります)*/
		gu::int32 FaceCount = 0;

		/*!**********************************************************************
		*  @brief     pmxファイルをもとにデータ読み込み
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @param[in] const PMXHeader ヘッダ情報
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header);

	};

	/****************************************************************************
	*				  		PMXIKLink
	****************************************************************************/
	/*  @brief   IKリンク情報を格納します.
	*****************************************************************************/
	struct PMXIKLink
	{
		/*! @brief IKリンクのボーンインデックス */
		gu::int32 TargetBoneIndex = 0;

		/*! @brief IKリンクの有効性 */
		gu::uint8 UseAngleLimit = 0;

		/*! @brief IKリンクの角度制限の下限 */
		gm::Float3 AngleLimitLower = {};

		/*! @brief IKリンクの角度制限の上限 */
		gm::Float3 AngleLimitUpper = {};

		PMXIKLink() = default;

		/*!**********************************************************************
		*  @brief     pmxファイルをもとにデータ読み込み
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @param[in] const PMXHeader ヘッダ情報
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header);
	};

	/****************************************************************************
	*				  		PMXBoneFlags
	****************************************************************************/
	/*  @brief   Boneのフラグを定義します.
	*****************************************************************************/
	enum class PMXBoneFlags : gu::uint16
	{
		None               = 0x0000, //!< フラグ無効
		Connect            = 0x0001, //!< 接続先ボーンあり（0：座標でオフセット指定、1：ボーン指定）
		AllowRotation      = 0x0002, //!< 回転可能
		AllowTranslate     = 0x0004, //!< 移動可能
		Visible            = 0x0008, //!< 表示
		AllowControl       = 0x0010, //!< 操作可能
		IK                 = 0x0020, //!< IK
		AppendLocal        = 0x0080, //!< ローカル付与 | 付与対象 0:ユーザー変形値／IKリンク／多重付与 1:親のローカル変形量
		AppendRotate       = 0x0100, //!< 回転付与
		AppendTranslate    = 0x0200, //!< 移動付与
		FixedAxis          = 0x0400, //!< 固定軸
		LocalAxis          = 0x0800, //!< ローカル軸
		DeformAfterPhysics = 0x1000, //!< 物理後変形
		DeformOuterParent  = 0x2000, //!< 外部親変形
	};

	ENUM_CLASS_FLAGS(PMXBoneFlags);

	/****************************************************************************
	*				  		PMXBone
	****************************************************************************/
	/*  @brief   ボーン情報を格納します.
	*****************************************************************************/
	struct PMXBone
	{
		/*! @brief ボーン名(日本語)*/
		gu::tstring NameJapanese = SP("");

		/*! @brief ボーン名(英語)*/
		gu::tstring NameEnglish = SP("");

		/*! @brief ボーンの位置 */
		gm::Float3 Position = {};

		/*! @brief 親ボーンのインデックス */
		gu::int32 ParentBoneIndex = 0;

		/*! @brief ボーンの変形階層 */
		gu::int32 DeformDepth = 0;

		/*! @brief ボーンの変形フラグ */
		PMXBoneFlags BoneFlag = PMXBoneFlags::None;

		/*! @brief 位置のオフセット*/
		gm::Float3 PositionOffset = {};

		/*! @brief ボーンの接続先 */
		gu::int32 TargetBoneIndex = 0;

		/*! @brief 付与親ボーンのインデックス*/
		gu::int32 AppendBoneIndex = 0;

		/*! @brief 付与親ボーンのウェイト */
		gu::float32 AppendWeight = 0;

		/*! @brief ボーンの回転可能軸 (PMXBoneFlag: FixedAxisが有効な場合に使用可能)*/
		gm::Float3 FixedRotationAxis = {};

		/*! @brief ボーンのローカルX軸 (PMXBoneFlag: LocalAxisが有効な場合に使用可能)*/
		gm::Float3 LocalXAxis = {};

		/*! @brief ボーンのローカルZ軸 (PMXBoneFlag: LocalAxisが有効な場合に使用可能)*/
		gm::Float3 LocalZAxis = {};

		/*! @brief キー (PMXBoneFlag: DeformOuterParentが有効な場合に使用可能)*/
		gu::int32 Key = 0;

		/*! @brief IKを使用するときの接続先のボーンのインデックス(PMXBoneFlags: IKBoneが有効な場合に使用可能) */
		gu::int32 IKTargetBoneIndex = 0;

		/*! @brief IKの試行回数(PMXBoneFlags: IKBoneが有効な場合に使用可能) : 最大は255回までを上限とする*/
		gu::int32 IKIteration = 0;

		/*! @brief IKの一回の試行当たりの角度制限. ラジアン角であり, PMDのIK値とは4倍異なるので注意(PMXBoneFlags: IKBoneが有効な場合に使用可能) */
		gu::float32 IKLoopAngleLimit = 0.0f;

		/*! @brief IKリンク*/
		gu::DynamicArray<PMXIKLink> IKLinks = {};

		/*!**********************************************************************
		*  @brief     pmxファイルをもとにデータ読み込み
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @param[in] const PMXHeader ヘッダ情報
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header);
	};

	/****************************************************************************
	*				  		PMXMorphType
	****************************************************************************/
	/*  @brief   表情モーフの種類を指定します.
	*****************************************************************************/
	enum class PMXMorphType : gu::uint8
	{
		Group         = 0,  //!< グループ
		Vertex        = 1,  //!< 頂点
		Bone          = 2,  //!< ボーン
		UV            = 3,  //!< UV
		AdditionalUV1 = 4,  //!< 追加UV1
		AdditionalUV2 = 5,  //!< 追加UV2
		AdditionalUV3 = 6,  //!< 追加UV3
		AdditionalUV4 = 7,  //!< 追加UV4
		Material      = 8,  //!< マテリアル
		Flip          = 9,  //!< フリップ
		Impulse       = 10, //!< インパルス
	};

	/****************************************************************************
	*				  		PMXFacePart
	****************************************************************************/
	/*  @brief  表情の部位を指定します.
	*****************************************************************************/
	enum class PMXFacePart : gu::uint8
	{
		Base    = 0, //!< 基本
		Eye     = 1, //!< 目
		EyeBrow = 2, //!< 眉
		Mouth   = 3, //!< 口
		Other   = 4, //!< その他
	};

	/****************************************************************************
	*				  		PMXPositionMorph
	****************************************************************************/
	/*  @brief 座標のオフセットを指定するモーフです.
	*****************************************************************************/
	struct PMXPositionMorph
	{
		/*! @brief 対象の頂点インデックス */
		gu::int32 VertexIndex = 0;

		/*! @brief 移動量 */
		gm::Float3 Offset = {};

		/*!**********************************************************************
		*  @brief     pmxファイルをもとにデータ読み込み
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @param[in] const PMXHeader ヘッダ情報
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header);
	};

	/****************************************************************************
	*				  		PMXUVMorph
	****************************************************************************/
	/*  @brief UV座標のオフセットを指定するモーフです.
	*****************************************************************************/
	struct PMXUVMorph
	{
		/*! @brief 対象の頂点インデックス */
		gu::int32 VertexIndex = 0;

		/*! @brief UV値の移動量 */
		gm::Float4 Offset = {};

		/*!**********************************************************************
		*  @brief     pmxファイルをもとにデータ読み込み
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @param[in] const PMXHeader ヘッダ情報
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header);
	};

	/****************************************************************************
	*				  		PMXBoneMorph
	****************************************************************************/
	/*  @brief ボーンの位置と回転量を調整するモーフです.
	*****************************************************************************/
	struct PMXBoneMorph
	{
		/*! @brief 対象のボーンインデックス */
		gu::int32 BoneIndex = 0;

		/*! @brief 移動量 */
		gm::Float3 PositionOffset = {};

		/*! @brief 回転量 */
		gm::Float4 RotationOffset = {};

		/*!**********************************************************************
		*  @brief     pmxファイルをもとにデータ読み込み
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @param[in] const PMXHeader ヘッダ情報
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header);
	};

	/****************************************************************************
	*				  		PMXMaterialOperationType
	****************************************************************************/
	/*  @brief　マテリアルの演算種別を指定します
	*****************************************************************************/
	enum class PMXMaterialOperationType : gu::uint8
	{
		Mul = 0, //!< 乗算
		Add = 1, //!< 加算
	};

	/****************************************************************************
	*				  		PMXMaterialMorph
	****************************************************************************/
	/*  @brief　マテリアルの色を調整するモーフです.
	*****************************************************************************/
	struct PMXMaterialMorph
	{
		/*! @brief 対象のマテリアルインデックス */
		gu::int32 MaterialIndex = 0;

		/*! @brief マテリアルの操作フラグ */
		PMXMaterialOperationType OperationType = PMXMaterialOperationType::Mul;

		/*! @brief マテリアルの拡散色 */
		gm::Float4 Diffuse = {};

		/*! @brief マテリアルの反射色 */
		gm::Float3 Specular = {};

		/*! @brief マテリアルの反射強度 */
		gu::float32 SpecularPower = 0.0f;

		/*! @brief マテリアルの環境色 */
		gm::Float3 Ambient = {};

		/*! @brief マテリアルのエッジ色 */
		gm::Float4 EdgeColor = {};

		/*! @brief マテリアルのエッジサイズ */
		gu::float32 EdgeSize = 0.0f;

		/*! @brief テクスチャ係数*/
		gm::Float4 TextureCoefficient = {};

		/*! @brief スフィアマップ係数*/
		gm::Float4 SphereMapCoefficient = {};

		/*! @brief トゥーンテクスチャ係数*/
		gm::Float4 ToonTextureCoefficient = {};

		/*!**********************************************************************
		*  @brief     pmxファイルをもとにデータ読み込み
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @param[in] const PMXHeader ヘッダ情報
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header);
	};

	/****************************************************************************
	*				  		PMXGroupMorph
	****************************************************************************/
	/*  @brief グループモーフ
	*****************************************************************************/
	struct PMXGroupMorph
	{
		/*! @brief 対象のグループインデックス */
		gu::int32 GroupIndex = 0;

		/*! @brief グループの移動量 */
		gu::float32 Weight = 0.0f;

		/*!**********************************************************************
		*  @brief     pmxファイルをもとにデータ読み込み
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @param[in] const PMXHeader ヘッダ情報
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header);
	};

	/****************************************************************************
	*				  		PMXFlipMorph
	****************************************************************************/
	/*  @brief フリップモーフは複数登録されたモーフのうち, 一つだけが有効になるモーフです.
	*****************************************************************************/
	struct PMXFlipMorph
	{
		/*! @brief 対象のモーフインデックス */
		gu::int32 MorphIndex = 0;

		/*! @brief モーフの移動量 */
		gu::float32 Weight = 0.0f;

		/*!**********************************************************************
		*  @brief     pmxファイルをもとにデータ読み込み
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @param[in] const PMXHeader ヘッダ情報
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header);
	};

	/****************************************************************************
	*				  		PMXImpulseMorph
	****************************************************************************/
	/*  @brief インパルスモーフ (物理演算によるモーフ) です.
	*****************************************************************************/
	struct PMXImpulseMorph
	{
		/*! @brief 対象のボーンインデックス */
		gu::int32 RigidBodyIndex = 0;

		/*! @brief ローカルフラグ*/
		gu::uint8 LocalFlag = 0;

		/*! @brief 移動速度*/
		gm::Float3 TranslateVelocity = {};

		/*! @brief 回転速度*/
		gm::Float3 RotateTorque = {};

		/*!**********************************************************************
		*  @brief     pmxファイルをもとにデータ読み込み
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @param[in] const PMXHeader ヘッダ情報
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header);
	};

	/****************************************************************************
	*				  		PMXMorph
	****************************************************************************/
	/*  @brief 表情差分のように, デフォルトのモデルから変形を加えるモーフです.
	*****************************************************************************/
	struct PMXMorph
	{
		/*! @brief モーフ名(日本語)*/
		gu::tstring NameJapanese = SP("");

		/*! @brief モーフ名(英語)*/
		gu::tstring NameEnglish = SP("");

		/*! @brief モーフの部位 */
		PMXFacePart FacePart = PMXFacePart::Base;

		/*! @brief モーフの種類 */
		PMXMorphType MorphType = PMXMorphType::Group;

		/*! @brief 位置座標のオフセットを指定するモーフ */
		gu::DynamicArray<PMXPositionMorph> PositionMorphs = {};

		/*! @brief UV座標のオフセットを指定するモーフ */
		gu::DynamicArray<PMXUVMorph> UVMorphs = {};

		/*! @brief モーフのオフセット */
		gu::DynamicArray<PMXBoneMorph> BoneMorphs = {};

		/*! @brief モーフのオフセット */
		gu::DynamicArray<PMXMaterialMorph> MaterialMorphs = {};

		/*! @brief モーフのオフセット */
		gu::DynamicArray<PMXGroupMorph> GroupMorphs = {};

		/*! @brief モーフのオフセット */
		gu::DynamicArray<PMXFlipMorph> FlipMorphs = {};

		/*! @brief モーフのオフセット */
		gu::DynamicArray<PMXImpulseMorph> ImpulseMorphs = {};

		/*!**********************************************************************
		*  @brief     pmxファイルをもとにデータ読み込み
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @param[in] const PMXHeader ヘッダ情報
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header);

		~PMXMorph()
		{
			PositionMorphs.Clear(); PositionMorphs.ShrinkToFit();
			UVMorphs      .Clear(); UVMorphs      .ShrinkToFit();
			BoneMorphs    .Clear(); BoneMorphs    .ShrinkToFit();
			MaterialMorphs.Clear(); MaterialMorphs.ShrinkToFit();
			GroupMorphs   .Clear(); GroupMorphs   .ShrinkToFit();
			FlipMorphs    .Clear(); FlipMorphs    .ShrinkToFit();
			ImpulseMorphs .Clear(); ImpulseMorphs .ShrinkToFit();

		}
	};

	/****************************************************************************
	*				  		PMXDisplayFrameFlag
	****************************************************************************/
	/*  @brief 通常枠か特殊枠か : 特殊枠とは, Root用とPMD互換用の枠です.
	*****************************************************************************/
	enum class PMXDisplayFrameFlag : gu::uint8
	{
		Default = 0, //!< 通常枠
		Special = 1, //!< 特殊枠
	};

	enum class PMXDisplayFrameType : gu::uint8
	{
		Bone  = 0, //!< ボーン
		Morph = 1, //!< モーフ
	};

	/****************************************************************************
	*				  		PMXDisplayFrameElement
	****************************************************************************/
	/*  @brief 　表示枠の要素を指定します.
	*****************************************************************************/
	struct PMXDisplayFrameElement 
	{
		/*! @brief 表示枠の種類 */
		PMXDisplayFrameType DisplayFrameType = PMXDisplayFrameType::Bone;

		/*! @brief 表示枠のインデックス */
		gu::int32 Index = 0;

		/*!**********************************************************************
		*  @brief     pmxファイルをもとにデータ読み込み
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @param[in] const PMXHeader ヘッダ情報
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header);
	};

	/****************************************************************************
	*				  		PMXDisplayFrame
	****************************************************************************/
	/*  @brief ボーン / モーフの表示枠を定義します.
	*****************************************************************************/
	struct PMXDisplayFrame
	{
		/*! @brief 表示枠名(日本語)*/
		gu::tstring NameJapanese = SP("");

		/*! @brief 表示枠名(英語)*/
		gu::tstring NameEnglish = SP("");

		/*! @brief 表示枠の特殊枠か*/
		PMXDisplayFrameFlag FrameFlag = PMXDisplayFrameFlag::Default;

		/*! @brief 表示枠の要素*/
		gu::DynamicArray<PMXDisplayFrameElement> Elements = {};

		/*!**********************************************************************
		*  @brief     pmxファイルをもとにデータ読み込み
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @param[in] const PMXHeader ヘッダ情報
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header);

		~PMXDisplayFrame()
		{
			Elements.Clear(); Elements.ShrinkToFit();
		}
	};

	/****************************************************************************
	*				  		PMXRigidBodyShape
	****************************************************************************/
	/*  @brief 剛体の形状
	*****************************************************************************/
	enum class PMXRigidBodyShape : gu::uint8
	{
		Sphere  = 0, //!< 球
		Box     = 1, //!< 箱
		Capsule = 2, //!< カプセル
	};

	/****************************************************************************
	*				  		PMXRigidBodyCalculationType
	****************************************************************************/
	/*  @brief 剛体の計算方法について
	*****************************************************************************/
	enum class PMXRigidBodyCalculationType : gu::uint8
	{
		Static              = 0, //!< 静的
		Dynamic             = 1, //!< 動的
		DynamicAndBoneMerge = 2, //!< 物理演算
	};

	/****************************************************************************
	*				  		PMXRigidBody
	****************************************************************************/
	/*  @brief 剛体
	*****************************************************************************/
	struct PMXRigidBody
	{
		/*! @brief 剛体名(日本語)*/
		gu::tstring NameJapanese = SP("");

		/*! @brief 剛体名(英語)*/
		gu::tstring NameEnglish = SP("");

		/*! @brief 剛体の関連ボーンインデックス*/
		gu::int32 RelatedBoneIndex = 0;

		/*! @brief グループ*/
		gu::uint8 Group = 0;

		/*! @brief マスク (非衝突グループフラグ)*/
		gu::uint16 Mask = 0;

		/*! @brief 形状*/
		gu::uint8 Shape = 0;

		/*! @brief サイズ*/
		gm::Float3 Size = {};

		/*! @brief 位置*/
		gm::Float3 Position = {};

		/*! @brief 回転*/
		gm::Float3 Rotation = {};

		/*! @brief 質量*/
		gu::float32 Mass = 0.0f;

		/*! @brief 移動減衰*/
		gu::float32 TranslateDamping = 0.0f;

		/*! @brief 回転減衰*/
		gu::float32 RotationDamping = 0.0f;

		/*! @brief 反発力*/
		gu::float32 Repulsion = 0.0f;

		/*! @brief 摩擦力*/
		gu::float32 Friction = 0.0f;

		/*! @brief 物理演算の種類*/
		PMXRigidBodyCalculationType CalculationType = PMXRigidBodyCalculationType::Static;

		/*!**********************************************************************
		*  @brief     pmxファイルをもとにデータ読み込み
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @param[in] const PMXHeader ヘッダ情報
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header);
	};

	/****************************************************************************
	*				  		PMXJointType
	****************************************************************************/
	/*  @brief Jointの種類 jointとは剛体同士を接続するための接続方法のことです
	*****************************************************************************/
	enum class PMXJointType : gu::uint8
	{
		GenericSpring6DOF = 0, //!< 6自由度バネ付きジョイント (PMX2.0およびPMDと同様) : 対応btGeneric6DofSpringConstraint
		Generic6DOF       = 1, //!< 6自由度ジョイント                            : 対応btGeneric6DofConstraint
		PointToPoint      = 2, //!< 各制限 / ばね定数が無効                       : 対応btPoint2PointConstraint
		ConeTwist         = 3, //!< 円錐範囲指定　　　　　　　　　　　               : 対応btConeTwistConstraint
		Slider            = 4, //!< スライダージョイント (軸移動ジョイント) 		     : 対応btSliderConstraint
		Hinge             = 5, //!< ヒンジジョイント   (回転ジョイント) 		     : 対応btHingeConstraint
	};

	/****************************************************************************
	*				  		PMXJoint
	****************************************************************************/
	/*  @brief Joint
	*****************************************************************************/
	struct PMXJoint
	{
		/*! @brief Joint名(日本語)*/
		gu::tstring NameJapanese = SP("");

		/*! @brief Joint名(英語)*/
		gu::tstring NameEnglish = SP("");

		/*! @brief Jointの種類*/
		PMXJointType JointType = PMXJointType::GenericSpring6DOF;

		/*! @brief 剛体Aのインデックス*/
		gu::int32 RigidBodyIndexA = 0;

		/*! @brief 剛体Bのインデックス*/
		gu::int32 RigidBodyIndexB = 0;

		/*! @brief 位置*/
		gm::Float3 Position = {};

		/*! @brief 回転 (radian)*/
		gm::Float3 Rotation = {};

		/*! @brief 移動制限の下限*/
		gm::Float3 TranslationLimitLower = {};

		/*! @brief 移動制限の上限*/
		gm::Float3 TranslationLimitUpper = {};

		/*! @brief 回転制限の下限 (radian)*/
		gm::Float3 RotationLimitLower = {};

		/*! @brief 回転制限の上限 (radian)*/
		gm::Float3 RotationLimitUpper = {};

		/*! @brief 移動バネ定数*/
		gm::Float3 TranslationSpringConstant = {};

		/*! @brief 回転バネ定数*/
		gm::Float3 RotationSpringConstant = {};

		/*!**********************************************************************
		*  @brief     pmxファイルをもとにデータ読み込み
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @param[in] const PMXHeader ヘッダ情報
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header);
	};

	/****************************************************************************
	*				  		PMXSoftBodyShape
	****************************************************************************/
	/*  @brief SoftBodyの形状 
	*****************************************************************************/
	enum class PMXSoftBodyShape : gu::uint8
	{
		TriangleMesh = 0, //!< 三角形メッシュ
		Rope         = 1, //!< ロープ
	};

	/****************************************************************************
	*				  		PMXSoftBodyFlags
	****************************************************************************/
	/*  @brief SoftBodyのフラグ
	*****************************************************************************/
	enum class PMXSoftBodyFlags : gu::uint8
	{
		None       = 0x00,
		BLink      = 0x01, //!< BLink
		Cluster    = 0x02, //!< クラスタ作成
		HybridLink = 0x04  //!< リンク交雑
	};
	ENUM_CLASS_FLAGS(PMXSoftBodyFlags);

	/****************************************************************************
	*				  		PMXAeroModel
	****************************************************************************/
	/*  @brief AeroModel
	*****************************************************************************/
	enum class PMXAeroModel
	{
		VPoint    = 0, //!< Vポイント 
		VTwoSided = 1, //!< V両面 
		VOneSided = 2, //!< V片面
		FTwoSided = 3, //!< F両面
		FOneSided = 4, //!< F片面
	};

	/****************************************************************************
	*				  		PMXSoftBodyAnchorRigidBody
	****************************************************************************/
	/*  @brief アンカー剛体
	*****************************************************************************/
	struct PMXAnchorRigidBody
	{
		/*! @brief 関連剛体インデックス*/
		gu::int32 RigidBodyIndex = 0;

		/*! @brief 関連頂点インデックス*/
		gu::int32 VertexIndex = 0;

		/*! @brief Nearモード*/
		bool IsNearMode = false;

		/*!**********************************************************************
		*  @brief     pmxファイルをもとにデータ読み込み
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @param[in] const PMXHeader ヘッダ情報
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header);
	};

	/****************************************************************************
	*				  		PMXSoftBodyConfig
	****************************************************************************/
	/*  @brief ソフトボディの設定
	*****************************************************************************/
	struct PMXSoftBodyConfig
	{
		/*! @brief 速度補正係数 (Baumgarte)*/
		gu::float32 VelocityCorrection = 0.0f;

		/*! @brief 減衰係数 [0, 1]*/
		gu::float32 DampingCoefficient = 0.0f;

		/*! @brief 抗力係数 [0,+inf]*/
		gu::float32 DragCoefficient = 0.0f;

		/*! @brief 揚力係数 [0, +inf]*/
		gu::float32 LiftCoefficient = 0.0f;

		/*! @brief 圧力係数 [-inf, +inf]*/
		gu::float32 PressureCoefficient = 0.0f;

		/*! @brief 体積保存係数 [0, +inf]*/
		gu::float32 VolumeConversation = 0.0f;

		/*! @brief 動摩擦係数 [0, 1] */
		gu::float32 DynamicFriction = 0.0f;

		/*! @brief ポーズマッチング係数 [0, 1]*/
		gu::float32 PoseMatchingCoefficient = 0.0f;

		/*! @brief 剛体接触の硬さ [0, 1]*/
		gu::float32 RigidBodyContactHardness = 0.0f;

		/*! @brief 運動接触の硬さ [0, 1]*/
		gu::float32 KineticContactHardness = 0.0f;

		/*! @brief ソフト接触の硬さ [0, 1]*/
		gu::float32 SoftContactHardness = 0.0f;

		/*! @brief アンカー硬さ [0, 1]*/
		gu::float32 AnchorHardness = 0.0f;

		/*!**********************************************************************
		*  @brief     pmxファイルをもとにデータ読み込み
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);

	};

	/****************************************************************************
	*				  		PMXSoftBodyCluster
	****************************************************************************/
	/*  @brief ソフトボディのクラスタ
	*****************************************************************************/
	struct PMXSoftBodyCluster
	{
		/*! @brief ソフトボディ vs 剛体 硬さ [0, 1]*/
		gu::float32 SoftVsRigidBodyHardness = 0.0f;

		/*! @brief ソフトボディ vs Kinetic 硬さ [0, 1]*/
		gu::float32 SoftVsKineticHardness = 0.0f;

		/*! @brief ソフトボディ vs ソフトボディ 硬さ [0, 1]*/
		gu::float32 SoftVsSoftHardness = 0.0f;

		/*! @brief ソフトボディ vs 剛体 impulse split [0, 1]*/
		gu::float32 SoftVsRigidBodyImpulseSplit = 0.0f;

		/*! @brief ソフトボディ vs Kinetic impulse split [0, 1]*/
		gu::float32 SoftVsKineticImpulseSplit = 0.0f;

		/*! @brief ソフトボディ vs ソフトボディ impulse split [0, 1]*/
		gu::float32 SoftVsSoftImpulseSplit = 0.0f;

		/*!**********************************************************************
		*  @brief     pmxファイルをもとにデータ読み込み
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);
	};

	/****************************************************************************
	*				  		PMXSoftBodyIteration
	****************************************************************************/
	/*  @brief ソフトボディ計算の各反復回数
	*****************************************************************************/
	struct PMXSoftBodyIteration
	{
		/*! @brief 速度Solverの反復回数*/
		gu::int32 VelocitySolver = 0;

		/*! @brief 位置Solverの反復回数*/
		gu::int32 PositionSolver = 0;

		/*! @brief ドリフトSolverの反復回数*/
		gu::int32 DriftSolver = 0;

		/*! @brief クラスタSolverの反復回数*/
		gu::int32 ClusterSolver = 0;

		/*!**********************************************************************
		*  @brief     pmxファイルをもとにデータ読み込み
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);
	};

	/****************************************************************************
	*				  		PMXSoftBodyMaterial
	****************************************************************************/
	/*  @brief ソフトボディの材質設定
	*****************************************************************************/
	struct PMXSoftBodyMaterial
	{
		/*! @brief 線形剛性係数 [0, 1]*/
		gu::float32 LinearStiffness = 0.0f;

		/*! @brief 面積 / 角度剛性係数 [0,1]*/
		gu::float32 AngularStiffness = 0.0f;

		/*! @brief 体積剛性係数 [0,1]*/
		gu::float32 VolumeStiffness = 0.0f;

		/*!**********************************************************************
		*  @brief     pmxファイルをもとにデータ読み込み
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);
	};

	/****************************************************************************
	*				  		PMXSoftBody
	****************************************************************************/
	/*  @brief ソフトボディ
	*****************************************************************************/
	struct PMXSoftBody
	{
		/*! @brief ソフトボディ名(日本語)*/
		gu::tstring NameJapanese = SP("");

		/*! @brief ソフトボディ名(英語)*/
		gu::tstring NameEnglish = SP("");

		/*! @brief 形状*/
		PMXSoftBodyShape Shape = PMXSoftBodyShape::TriangleMesh;

		/*! @brief マテリアルを指定するインデックス*/
		gu::int32 MaterialIndex = 0;

		/*! @brief グループインデックス*/
		gu::uint8 Group = 0;

		/*! @brief 非衝突グループフラグ*/
		gu::uint16 NonCollideGroupFlag = 0;

		/*! @brief ソフトボディフラグ*/
		PMXSoftBodyFlags Flag = PMXSoftBodyFlags::None;

		/*! @brief BLinkの作成距離*/
		gu::int32 BLinkDistance = 0;

		/*! @brief クラスタの作成数*/
		gu::int32 ClusterCount = 0;

		/*! @brief 総質量*/
		gu::float32 TotalMass = 0.0f;

		/*! @brief 衝突マージン*/
		gu::float32 CollisionMargin = 0.0f;

		/*! @brief AeroModel*/
		PMXAeroModel AeroModel = PMXAeroModel::VPoint;

		/*! @brief Config*/
		PMXSoftBodyConfig Config = {};

		/*! @brief Cluster*/
		PMXSoftBodyCluster Cluster = {};

		/*! @brief Iteration*/
		PMXSoftBodyIteration Iteration = {};

		/*! @brief Material*/
		PMXSoftBodyMaterial Material = {};

		/*! @brief アンカー剛体*/
		gu::DynamicArray<PMXAnchorRigidBody> AnchorRigidBodies = {};

		/*! @brief 関連頂点Index*/
		gu::DynamicArray<gu::int32> VertexIndices = {};

		/*!**********************************************************************
		*  @brief     pmxファイルをもとにデータ読み込み
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @param[in] const PMXHeader ヘッダ情報
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const PMXHeader& header);

		~PMXSoftBody()
		{
			AnchorRigidBodies.Clear(); AnchorRigidBodies.ShrinkToFit();
			VertexIndices.Clear(); VertexIndices.ShrinkToFit();
		}
	};

}

#endif