//////////////////////////////////////////////////////////////////////////////////
///             @file   VMDParser.hpp
///             @brief  VMDParser
///             @author Toide Yutaro
///             @date   2022_03_11
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VMD_PARSER_HPP
#define VMD_PARSER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/MMD/Private/Include/VMDDefines.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc::file::vmd
{
	/****************************************************************************
	*				  			VMDFile
	*************************************************************************//**
	*   @brief  モーションデータを記録するクラスです. vmdファイルを読み込むことができます.
	*****************************************************************************/
	struct VMDFile : public gu::NonCopyable
	{
	public:
		#pragma region Public Function

		/*!**********************************************************************
		*  @brief     VMDファイルを読み込む関数
		*  @param[in] gu::tsring& ファイルパス
		*  @return    bool
		*************************************************************************/
		bool Read(const gu::tstring& filePath);

		/*!**********************************************************************
		*  @brief     VMDファイルを書き込む関数
		*  @param[in] gu::tsring& ファイルパス
		*  @return    void
		*************************************************************************/
		bool Write(const gu::tstring& filePath);

		#pragma endregion

		#pragma region Public Property
		
		/*! @brief モーションデータのヘッダ情報. モデル名が入っています*/
		VMDHeader Header = {};

		/*! @brief ボーンのキーフレーム情報*/
		gu::DynamicArray<VMDBoneKeyFrame> BoneFrames = {};

		/*! @brief 表情のキーフレーム情報*/
		gu::DynamicArray<VMDFaceKeyFrame> FaceFrames = {};

		/*! @brief カメラのキーフレーム情報*/
		gu::DynamicArray<VMDCameraKeyFrame> CameraFrames = {};

		/*! @brief 照明のキーフレーム情報*/
		gu::DynamicArray<VMDLightKeyFrame> LightFrames = {};

		/*! @brief セルフシャドウのキーフレーム情報 */
		gu::DynamicArray<VMDSelfShadowKeyFrame> SelfShadowFrames = {};

		/*! @brief IKのキーフレーム情報 */
		gu::DynamicArray<VMDIKKeyFrame> IKFrames = {};

		/*! @brief VMDファイルが格納されている親ディレクトリ*/
		gu::tstring Directory = SP("");

		#pragma endregion

		#pragma region Public Constructor and Destructor

		/*! @brief デフォルトコンストラクタ*/
		VMDFile() = default;

		/*! @brief デストラクタ*/
		~VMDFile();

		#pragma endregion

	private:
		#pragma region Private Function
		#pragma endregion

		#pragma region Private Property
		#pragma endregion

	};

}

#endif