//////////////////////////////////////////////////////////////////////////////////
///  @file   SFTexture.hpp
///  @brief  テクスチャ読み込み情報の取得. 実際のテクスチャロードは別途行う.
///  @author toide
///  @date   2024/07/22 0:54:48
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SF_MATERIAL_HPP
#define SF_MATERIAL_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "SFTexture.hpp"
#include "GameUtility/Base/Include/GUOptional.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace engine::file::sf
{
	/****************************************************************************
	*				  			   SFMaterial
	****************************************************************************/
	/* @brief  テクスチャの情報を一時的に格納するための構造体.
	*****************************************************************************/
	struct SFMaterial
	{
	public:
		#pragma region Public Function

		#pragma endregion 

		#pragma region Public Property
		/*! @brief マテリアル名*/
		gu::tstring Name = SP("");

		/*! @brief マテリアルの拡散色*/
		gm::Float4 Diffuse = gm::Float4(1.0f, 1.0f, 1.0f, 1.0f);

		/*! @brief マテリアルの反射色*/
		gm::Float3 Specular = gm::Float3(1.0f, 1.0f, 1.0f);

		/*! @brief マテリアルの反射強度*/
		gu::float32 SpecularPower = 1.0f;

		/*! @brief マテリアルの環境色*/
		gm::Float3 Ambient = gm::Float3(1.0f, 1.0f, 1.0f);

		/*! @brief DiffuseMapのテクスチャ*/
		gu::Optional<SFTexture> DiffuseMap = {};
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