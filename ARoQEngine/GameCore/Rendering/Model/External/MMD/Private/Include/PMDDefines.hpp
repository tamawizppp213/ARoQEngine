//////////////////////////////////////////////////////////////////////////////////
///  @file   PMDDefines.hpp
///  @brief  PMDで使用する定義をまとめたヘッダファイル.
///  @author toide
///  @date   2024/05/11 21:52:31
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef PMD_DEFINES_HPP
#define PMD_DEFINES_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUString.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
#include "GameUtility/Math/Include/GMVector.hpp"
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
namespace gc::file::pmd::details
{
	/****************************************************************************
	*				  		PMDHeader
	*************************************************************************//**
	/*  @brief   モデルデータのヘッダ情報. モデル名, バージョン, コメントなどが入っています. @n
	*            モデル名は日本語と英語の2種類があります. 最大20文字@n
	*            バージョンは1.00などのように小数点以下2桁までの値が入ります.@n
	*            コメントは日本語と英語の2種類があります. コメントは256文字まで入ります. @n
	*            本来はstring型で扱うべきですが, EngineやFileSystemの仕様に合わせてtstring型を使用しています.
	*****************************************************************************/
	struct PMDHeader
	{
		/*! @brief ファイル識別子 (Pmd)*/
		gu::char8   Signature[3]    = {};

		/*! @brief バージョン番号 (1.00など)*/
		gu::float32 Version         = 0;

		/*! @brief モデル名(日本語)*/
		gu::tstring NameJapanese    = SP("");

		/*! @brief モデル名(英語)*/
		gu::tstring NameEnglish     = SP("");

		/*! @brief 著作権情報のようなコメント(日本語) */
		gu::tstring CommentJapanese = SP("");

		/*! @brief 著作権情報のようなコメント(英語) */
		gu::tstring CommentEnglish  = SP("");

		/*!**********************************************************************
		*  @brief     正しいファイルヘッダになっているか (Pmdというヘッダ名になっているか)
		*  @param[in] void
		*  @return    bool : Pmdというヘッダ名になっている場合にtrueを返します. 
		*************************************************************************/
		bool IsValid() const;

		/*!**********************************************************************
		*  @brief     拡張以外のデータ(English)を読み込む
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);
		
		/*!**********************************************************************
		*  @brief     拡張データ(English)を読み込む
		*  @param[in] const gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @return    void
		*************************************************************************/
		void ReadExtension(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);
	};

	/****************************************************************************
	*				  		PMDVertex
	*************************************************************************//**
	/*  @brief   モデルデータの頂点情報, 位置, 法線, UV, ボーン情報, エッジの可視化情報が入っています. @n
	*            最大頂点数は65536です.
	*****************************************************************************/
	struct PMDVertex
	{
		/*! @brief 頂点の位置情報 */
		gm::Float3 Position;

		/*! @brief 頂点の法線 */
		gm::Float3 Normal;

		/*! @brief テクスチャマッピングに使用されるUV座標 */
		gm::Float2 UV;

		/*! @brief 頂点が影響を受ける近接ボーン番号 */
		gu::uint16 BoneIndex[2];

		/*! @brief 2つのボーンに対する影響度合い. 値が大きいほどBoneIndex[0]に近い. 0～100で指定します */
		gu::uint8  BoneWeight;

		/*! @brief 輪郭線が不可視であるかを示すフラグ */
		gu::uint8  EdgeInvisible;

		/*! @brief ファイルハンドルを使った初期化*/
		PMDVertex(const gu::SharedPointer<platform::core::file::IFileHandle>& handle)
		{
			Read(handle);
		}

		/*!**********************************************************************
		*  @brief     頂点データを読み込みます.
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);
	};

	/****************************************************************************
	*				  		PMDMaterial
	*************************************************************************//**
	/*  @brief   モデルデータのマテリアル情報 @n
	*            マテリアルは拡散色, 光沢の強さ, 光沢色, 環境色, トゥーンテクスチャ番号, 輪郭線フラグ, 頂点インデックス数, テクスチャ名, SphereMap名が入っています.
	*****************************************************************************/
	struct PMDMaterial
	{
		/*! @brief 拡散色を示すRGBA値. 各要素は0.0～1.0の範囲で定義されます */
		gm::Float4 Diffuse;

		/*! @brief 光沢の強さ. 範囲制限は特にありません*/
		gu::float32 SpecularPower;

		/*! @brief 光沢色(RGB). 各要素は0.0～1.0の範囲で定義されます*/
		gm::Float3 Specular;

		/*! @brief 環境色. 周囲の環境から受ける光のオフセット値. (真っ黒になるのを防ぐ)*/
		gm::Float3 Ambient;

		/*! @brief トゥーンテクスチャの番号*/
		gu::uint8  ToonTextureID;

		/*! @brief 輪郭線フラグ (1で有効化)*/
		gu::uint8  EdgeFlag;

		/*! @brief このマテリアルに関連付けられた頂点インデックスの数を表す値*/
		gu::uint32 IndexCount;

		/*! @brief このマテリアルに関連付けられているテクスチャ名*/
		gu::tstring TexturePath = SP("");

		/*! @brief このマテリアルに関連付けられているSphereMap(反射マップ)の名前*/
		gu::tstring SphereMapPath = SP("");

		/*! @brief デフォルトコンストラクタ*/
		PMDMaterial() = default;

		/*! @brief ファイルハンドルを使った初期化*/
		PMDMaterial(const gu::SharedPointer<platform::core::file::IFileHandle>& handle, const gu::tstring& directory) 
		{
			Read(handle, directory);
		}

		/*!**********************************************************************
		*  @brief     マテリアルを読み込みます
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @param[in] const gu::tstring& ディレクトリ
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle, const gu::tstring& directory);
	};

	/****************************************************************************
	*				  		PMDBoneType
	*************************************************************************//**
	/*  @brief   モデルデータのボーンの種類
	*****************************************************************************/
	enum class PMDBoneType : gu::uint8
	{
		Rotation          = 0,  //!< 回転
		RotationAndMove   = 1,  //!< 回転移動
		IK                = 2,  //!< IK
		Unknown           = 3,  //!< 不明
		IKRotation        = 4,  //!< IK影響下の回転
		RotationEffectable= 5,  //!< 回転影響下
		IKTarget          = 6,  //!< IKの接続先
		Hide              = 7,  //!< 非表示
		Twist             = 8,  //!< 捩り
		RotationMotion    = 9   //!< 回転運動
	};

	/****************************************************************************
	*				  		PMDBone
	*************************************************************************//**
	/*  @brief   モデルデータのボーン情報 @n
	*            ボーンの名前, 親ボーン番号, 子ボーン番号, ボーンの種類, IKボーン番号, ボーンの先頭位置が入っています.
	*****************************************************************************/
	struct PMDBone
	{
		/*! @brief ボーンの日本語名*/
		gu::tstring NameJapanese = SP("");

		/*! @brief ボーンの英語名*/
		gu::tstring NameEnglish = SP("");

		/*! @brief ボーンの親ボーン番号. 親がいない場合は-1*/
		gu::uint16 ParentBoneID = 0xFFFF;

		/*! @brief ボーンの子ボーン番号*/
		gu::uint16 ChildBoneID = 0;

		/*! @brief ボーンの種類. 0:回転, 1:回転移動, 2:IK, 3:不明, 4:IK影響下の回転, 5:IK影響下の回転移動, 6:非表示, 7:捩り, 8:回転運動*/
		PMDBoneType BoneType = PMDBoneType::Unknown;

		/*! @brief IKボーンの番号 (無い場合は0)*/
		gu::uint16 IKBoneID = 0;

		/*! @brief ボーンの先頭位置*/
		gm::Float3 HeadPosition = {};

		/*!**********************************************************************
		*  @brief     ボーンを読み込みます
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @param[in] const gu::tstring& ディレクトリ
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);

		/*!**********************************************************************
		*  @brief     拡張データ(English)を読み込む
		*  @param[in] const gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @return    void
		*************************************************************************/
		void ReadExtension(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);
	};

	/****************************************************************************
	*				  		PMDBoneIK
	*************************************************************************//**
	/*  @brief   モデルデータのボーンIK情報@n
	*****************************************************************************/
	struct PMDBoneIK
	{
		/*! @brief 自身のIKBoneを取得するID*/
		gu::uint16 SelfIKBoneID = 0;

		/*! @brief この変数に向かってSelfIKBoneや間のChainボーンを動かすことになります. */
		gu::uint16 TargetBoneID = 0;

		/*! @brief IKを実行し, 目標のボーンに近づけるためのループ試行回数*/
		gu::uint16 IterationCount = 0;

		/*! @brief 最大の角度制限*/
		gu::float32 AngleLimit = 0;

		/*! @brief 間のIKボーンに含まれるIDのリスト*/
		gu::DynamicArray<gu::uint16> Chains = gu::DynamicArray<gu::uint16>();

		/*!**********************************************************************
		*  @brief     ボーンIKを読み込みます
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @param[in] const gu::tstring& ディレクトリ
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);

		/*! @brief デフォルトコンストラクタ*/
		PMDBoneIK() = default;

		/*! @brief ファイルハンドルを使った初期化*/
		PMDBoneIK(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle)
		{
			Read(fileHandle);
		}

		/*! @brief デストラクタ*/
		~PMDBoneIK()
		{
			Chains.Clear(); Chains.ShrinkToFit();
		}
	};

	/****************************************************************************
	*				  		PMDFacePart
	*************************************************************************//**
	/*  @brief  顔のパーツの種類を指定します.
	*****************************************************************************/
	enum class PMDFacePart : gu::uint8
	{
		Base    = 0, //!< 基本
		EyeBrow = 1, //!< 眉
		Eye     = 2, //!< 目
		Lip     = 3, //!< 口
		Other   = 4  //!< その他
	};

	/****************************************************************************
	*				  		PMDFaceExpression
	*************************************************************************//**
	/*  @brief  表情による頂点モーフィング情報を格納します. @n
	*           表情名, 表情のパーツ種類, 頂点差分の位置リスト, 頂点のインデックスリストが入っています.
	*****************************************************************************/
	struct PMDFaceExpression
	{
		/*! @brief 表情名 (日本語)*/
		gu::tstring NameJapanese = SP("");

		/*! @brief 表情名 (英語)*/
		gu::tstring NameEnglish = SP("");

		/*! @brief 表情のパーツ種類*/
		PMDFacePart Part = PMDFacePart::Base;

		/*! @brief 表情の頂点差分の位置リスト*/
		gu::DynamicArray<gm::Float3> Vertices = {};

		/*! @brief 頂点のインデックスリスト*/
		gu::DynamicArray<gu::uint32> Indices = {};

		/*!**********************************************************************
		*  @brief     表情を読み込みます
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);

		/*!**********************************************************************
		*  @brief     拡張データ(English)を読み込む
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @return    void
		*************************************************************************/
		void ReadExtension(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);

		/*! @brief デフォルトコンストラクタ*/
		PMDFaceExpression() = default;

		/*! @brief デストラクタ*/
		~PMDFaceExpression()
		{
			Vertices.Clear(); Vertices.ShrinkToFit();
			Indices .Clear(); Indices .ShrinkToFit();
		}
	};

	/****************************************************************************
	*				  		PMDBoneDisplayName
	*************************************************************************//**
	/*  @brief  ボーンの表示名を取得します.
	*****************************************************************************/
	struct PMDBoneDisplayName
	{
		/*! @brief 表情名 (日本語)*/
		gu::tstring NameJapanese = SP("");

		/*! @brief 表情名 (英語)*/
		gu::tstring NameEnglish  = SP("");

		/*!**********************************************************************
		*  @brief     ボーンの表示名を読み込みます
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);

		/*!**********************************************************************
		*  @brief     拡張データ(English)を読み込む
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @return    void
		*************************************************************************/
		void ReadExtension(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);
	};

	/****************************************************************************
	*				  		PMDBoneNameTable
	*************************************************************************//**
	/*  @brief  ボーンの実名と表示名の対応を取るためのインデックスを保存します. 
	*****************************************************************************/
	struct PMDBoneNameTable
	{
		/*! @brief 実ボーンを指定するインデックス*/
		gu::uint16 BoneID = 0; 

		/*! @brief 表示名を指定するインデックス*/
		gu::uint8 DisplayID = 0;

		/*!**********************************************************************
		*  @brief     ボーンの対応表を読み込みます
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);

	};

	/****************************************************************************
	*				  		PMDRigidBodyShape
	*************************************************************************//**
	/*  @brief  剛体の形状を指定します. 基本的な形状は球, 箱, カプセルの3種類です.
	*****************************************************************************/
	enum class PMDRigidBodyShape : gu::uint8
	{
		Sphere  = 0, //!< 球
		Box     = 1, //!< 箱
		Capsule = 2  //!< カプセル
	};

	/****************************************************************************
	*				  		PMDRigidBodyType
	*************************************************************************//**
	/*  @brief  剛体の種類を指定します
	*****************************************************************************/
	enum class PMDRigidBodyType : gu::uint8
	{
		BoneConnected  = 0, //!< ボーンに接続
		Physics        = 1, //!< 物理演算
		PhysicsAndBone = 2  //!< 物理演算とボーンに接続
	};

	/****************************************************************************
	*				  		PMDRigidBody
	*************************************************************************//**
	/*  @brief  剛体の設定を保存します @n
	*****************************************************************************/
	struct PMDRigidBody
	{
		/*! @brief 剛体の名前*/
		gu::tstring Name = SP("");

		/*! @brief 剛体の関連ボーン番号*/
		gu::uint16 BoneID = 0;

		/*! @brief 剛体のグループ番号*/
		gu::uint8 GroupID = 0;

		/*! @brief 剛体のマスク番号*/
		gu::uint16 MaskID = 0;

		/*! @brief 剛体の形状*/
		PMDRigidBodyShape Shape = PMDRigidBodyShape::Sphere;

		/*! @brief 剛体のサイズ*/
		gm::Float3 Size = {};

		/*! @brief 剛体の位置*/
		gm::Float3 Position = {};

		/*! @brief 剛体の方向 (rad)*/
		gm::Float3 Orientation = {};

		/*! @brief 剛体の質量*/
		gu::float32 Mass = 0;

		/*! @brief 剛体の移動減衰*/
		gu::float32 DampingTranslate = 0;

		/*! @brief 剛体の回転減衰*/
		gu::float32 DampingAngular = 0;

		/*! @brief 剛体の反発力*/
		gu::float32 Restitution = 0;

		/*! @brief 剛体の摩擦力*/
		gu::float32 Friction = 0;

		/*! @brief 剛体の種類*/
		PMDRigidBodyType Type = PMDRigidBodyType::BoneConnected;

		/*!**********************************************************************
		*  @brief     剛体の設定を読み込みます
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);
	};

	/****************************************************************************
	*				  		PMDJoint
	*************************************************************************//**
	/*  @brief  関節の設定を保存します
	*****************************************************************************/
	struct PMDJoint
	{
		/*! @brief 関節の名前*/
		gu::tstring Name = SP("");

		/*! @brief 剛体A*/
		gu::uint32 RigidBodyA = 0;

		/*! @brief 剛体B*/
		gu::uint32 RigidBodyB = 0;

		/*! @brief 位置*/
		gm::Float3 Position = {};

		/*! @brief 回転 (rad)*/
		gm::Float3 Orientation = {};

		/*! @brief 移動制限 (最小値)*/
		gm::Float3 TranslationLimitMin = {};

		/*! @brief 移動制限 (最大値)*/
		gm::Float3 TranslationLimitMax = {};

		/*! @brief 回転制限 (最小値)*/
		gm::Float3 OrientationLimitMin = {};

		/*! @brief 回転制限 (最大値)*/
		gm::Float3 OrientationLimitMax = {};

		/*! @brief 移動のばね定数*/
		gm::Float3 SpringTranslation = {};

		/*! @brief 回転のばね定数*/
		gm::Float3 SpringOrientation = {};

		/*!**********************************************************************
		*  @brief     関節の設定を読み込みます
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);
	};
}


#endif