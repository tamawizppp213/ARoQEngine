//////////////////////////////////////////////////////////////////////////////////
///  @file   VMDDefines.hpp
///  @brief  VMDで使用する定義をまとめたヘッダファイル.
///  @author toide
///  @date   2024/06/05 23:05:33
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VMD_DEFINES_HPP
#define VMD_DEFINES_HPP

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
namespace gc::file::vmd
{
	/****************************************************************************
	*				  		VMDHeader
	*************************************************************************//**
	/*  @brief  VMDのヘッダ情報を格納する構造体です.
	*****************************************************************************/
	struct VMDHeader
	{
		/*! @brief シグネチャ Vocaloid Motion Data 0002 (30 byte)*/
		gu::tstring Header = SP("");

		/*! @brief モデル名 (20 byte)*/
		gu::tstring ModelName = SP("");

		/*!**********************************************************************
		*  @brief     有効なVMDファイルか
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		bool IsValid() const; 

		/*!**********************************************************************
		*  @brief     VMDファイルをもとにデータ読み込み
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);

		/*!**********************************************************************
		*  @brief     VMDファイルをもとにデータ書き込み
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @return    void
		*************************************************************************/
		void Write(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle) const;
	};

	/****************************************************************************
	*				  		VMDBoneKeyFrame
	*************************************************************************//**
	/*  @brief  ボーンのキーフレーム情報を格納する構造体です.
	*****************************************************************************/
	struct VMDBoneKeyFrame
	{
		/*! @brief ボーン名*/
		gu::tstring Name = SP("");

		/*! @brief フレーム番号*/
		gu::uint32 Frame = 0;

		/*! @brief 位置*/
		gm::Float3 Translation = {};

		/*! @brief 回転*/
		gm::Float4 Quaternion = {};

		/*! @brief ベジエ曲線の補間パラメータ. 0～127*/
		gu::uint8 BazierInterpolation[64] = {};

		/*!**********************************************************************
		*  @brief     VMDファイルをもとにデータ読み込み
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);

		/*!**********************************************************************
		*  @brief     VMDファイルをもとにデータ書き込み
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @return    void
		*************************************************************************/
		void Write(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle) const;
	};

	/****************************************************************************
	*				  		VMDFaceKeyFrame
	*************************************************************************//**
	/*  @brief  表情のキーフレーム情報を格納する構造体です.
	*****************************************************************************/
	struct VMDFaceKeyFrame
	{
		/*! @brief 表情名*/
		gu::tstring Name = SP("");

		/*! @brief フレーム番号*/
		gu::uint32 Frame = 0;

		/*! @brief ウェイト*/
		gu::float32 Weight = 0.0f;

		/*!**********************************************************************
		*  @brief     VMDファイルをもとにデータ読み込み
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);

		/*!**********************************************************************
		*  @brief     VMDファイルをもとにデータ書き込み
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @return    void
		*************************************************************************/
		void Write(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle) const;
	};

	/****************************************************************************
	*				  		VMDCameraKeyFrame
	*************************************************************************//**
	/*  @brief  カメラのキーフレーム情報を格納する構造体です.
	*****************************************************************************/
	struct VMDCameraKeyFrame
	{
		/*! @brief フレーム番号*/
		gu::uint32 Frame = 0;

		/*! @brief 距離*/
		gu::float32 Distance = 0.0f;

		/*! @brief 位置*/
		gm::Float3 Position = {};

		/*! @brief 回転*/
		gm::Float3 Rotation = {};

		/*! @brief 補間パラメータ*/
		gu::uint8 Interpolation[24] = {};

		/*! @brief 視野角*/
		gu::uint32 ViewAngle = 0;

		/*! @brief 透視投影かどうか*/
		bool IsPerspective = 0;

		/*!**********************************************************************
		*  @brief     VMDファイルをもとにデータ読み込み
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);

		/*!**********************************************************************
		*  @brief     VMDファイルをもとにデータ書き込み
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @return    void
		*************************************************************************/
		void Write(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle) const;
	};

	/****************************************************************************
	*				  		VMDLightKeyFrame
	*************************************************************************//**
	/*  @brief  ライトのキーフレーム情報を格納する構造体です.
	*****************************************************************************/
	struct VMDLightKeyFrame
	{
		/*! @brief フレーム番号*/
		gu::uint32 Frame = 0;

		/*! @brief 色*/
		gm::Float3 Color = {};

		/*! @brief 位置*/
		gm::Float3 Position = {};

		/*!**********************************************************************
		*  @brief     VMDファイルをもとにデータ読み込み
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);

		/*!**********************************************************************
		*  @brief     VMDファイルをもとにデータ書き込み
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @return    void
		*************************************************************************/
		void Write(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle) const;
	};

	/****************************************************************************
	*				  		VMDShadowType
	*************************************************************************//**
	/*  @brief  セルフシャドウのつけ方を表す列挙体です.
	*****************************************************************************/
	enum class VMDShadowType : gu::uint8
	{
		Off,    //!< オフ
		Mode1,  //!< モード1
		Mode2   //!< モード2
	};

	/****************************************************************************
	*				  		VMDSelfShadowKeyFrame
	*************************************************************************//**
	/*  @brief  セルフシャドウのキーフレーム情報を格納する構造体です.
	*****************************************************************************/
	struct VMDSelfShadowKeyFrame
	{
		/*! @brief フレーム番号*/
		gu::uint32 Frame = 0;

		/*! @brief シャドウの種類*/
		VMDShadowType ShadowType = VMDShadowType::Off;

		/*! @brief シャドウ距離*/
		gu::float32 Distance = 0.0f;

		/*!**********************************************************************
		*  @brief     VMDファイルをもとにデータ読み込み
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);

		/*!**********************************************************************
		*  @brief     VMDファイルをもとにデータ書き込み
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @return    void
		*************************************************************************/
		void Write(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle) const;
	};

	/****************************************************************************
	*				  		VMDIKEnable
	*************************************************************************//**
	/*  @brief  モデル表示のキーフレーム情報を格納する構造体です.
	*****************************************************************************/
	struct VMDIKEnable
	{
		/*! @brief IK名*/
		gu::tstring Name = SP("");

		/*! 有効化するか*/
		bool IsEnable = false;

		/*!**********************************************************************
		*  @brief     VMDファイルをもとにデータ読み込み
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);

		/*!**********************************************************************
		*  @brief     VMDファイルをもとにデータ書き込み
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @return    void
		*************************************************************************/
		void Write(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle) const;
	};

	/****************************************************************************
	*				  		VMDIKKeyFrame
	*************************************************************************//**
	/*  @brief  IKのキーフレーム情報を格納する構造体です.
	*****************************************************************************/
	struct VMDIKKeyFrame
	{
		/*! @brief フレーム番号*/
		gu::uint32 Frame = 0;

		/*! @brief 表示するか*/
		bool IsDisplay = false;

		/*! @brief IKの有効化情報*/
		gu::DynamicArray<VMDIKEnable> IKEnables = {};

		/*!**********************************************************************
		*  @brief     VMDファイルをもとにデータ読み込み
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @return    void
		*************************************************************************/
		void Read(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle);

		/*!**********************************************************************
		*  @brief     VMDファイルをもとにデータ書き込み
		*  @param[in] gu::SharedPointer<platform::core::file::IFileHandle>& ファイル操作のハンドル
		*  @return    void
		*************************************************************************/
		void Write(const gu::SharedPointer<platform::core::file::IFileHandle>& fileHandle) const;

		~VMDIKKeyFrame()
		{
			IKEnables.Clear(); IKEnables.ShrinkToFit();
		}
	};
}

#endif