//////////////////////////////////////////////////////////////////////////////////
///  @file   SFTexture.hpp
///  @brief  テクスチャ読み込み情報の取得. 実際のテクスチャロードは別途行う.
///  @author toide
///  @date   2024/07/22 0:54:48
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SF_TEXTURE_HPP
#define SF_TEXTURE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
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
	*				  		SFTextureOperator
	****************************************************************************/
	/* @brief  異なるテクスチャ同士をブレンドする際の演算方法
	*****************************************************************************/
	enum class SFTextureOperator
	{
		None,		    //!< 何もしない
		Add,		    //!< 加算
		Subtract,	    //!< 減算
		Multiply,	    //!< 乗算
		Divide,		    //!< 除算
		SmoothAdd,	    //!< スムース加算 (Left + Right) - (Left * Right)
		SignedAdd,	    //!< 符号付き加算 (Left + Right) - 0.5
	};

	/****************************************************************************
	*				  		SFTextureAddressingMode
	****************************************************************************/
	/* @brief  テクスチャのアドレッシングモード (0...1]の範囲外のUV座標の扱い方を定義します)
	*****************************************************************************/
	enum class SFTextureAddressingMode
	{
		Wrap,		    //!< テクスチャの繰り返し
		Mirror,		    //!< テクスチャのミラーリング
		Clamp,		    //!< テクスチャのクランプ
		Border,		    //!< テクスチャの境界
	};

	/****************************************************************************
	*				  		SFTextureMapping
	****************************************************************************/
	/* @brief  テクスチャのマッピング方法
	*****************************************************************************/
	enum class SFTextureMapping
	{
		UV,       //!< UV座標
		Sphere,   //!< 球面座標
		Cube,     //!< 立方体座標
		Cylinder, //!< 円柱座標
		Plane,    //!< 平面座標
		Other,    //!< その他
	};

	/****************************************************************************
	*				  			   SFTexture
	****************************************************************************/
	/* @brief  テクスチャの情報を一時的に格納するための構造体.
	*****************************************************************************/
	struct SFTexture
	{
	public:
		#pragma region Public Function

		#pragma endregion 

		#pragma region Public Property

		/*! @brief テクスチャのファイルパス*/
		gu::tstring FilePath = SP("");

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief デフォルトコンストラクタ*/
		SFTexture() = default;

		/*! @brief デストラクタ*/
		~SFTexture() = default;

		/*! @brief ファイルパスの指定*/
		SFTexture(const gu::tstring& filePath) : FilePath(filePath) {};
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