//////////////////////////////////////////////////////////////////////////////////
///  @file   RHIPixelFormat.hpp
///  @brief  画像データを扱うためのクラスです. 
///  @author toide
///  @date   2024/04/24 22:50:59
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_PIXEL_FORMAT_HPP
#define RHI_PIXEL_FORMAT_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUType.hpp"
#include "GameUtility/Base/Include/GUEnumClassFlags.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	/****************************************************************************
	*				  			PixelFormat
	*************************************************************************//**
	/*  @brief  描画で使用するピクセルフォーマット
	*****************************************************************************/
	enum class PixelFormat : gu::uint8
	{
		Unknown            = 0,  //!< 使用用途が不明のフォーマットです.

		R32G32B32A32_FLOAT   = 1,  //!< RGBAコンポーネントで32bitのFloat値
		R32G32B32A32_UINT    = 2,  //!< RGBAコンポーネントで32bitのUnsigned int値
		R32G32B32A32_SINT    = 3,  //!< RGBAコンポーネントで32bitのSigned int値
		
		R16G16B16A16_FLOAT   = 4,  //!< RGBAコンポーネントで16bitのFloat値,
		R16G16B16A16_UINT    = 5,  //!< RGBAコンポーネントで16bitのUnsigned int値
		R16G16B16A16_SINT    = 6,  //!< RGBAコンポーネントで16bitのSigned int値
		R16G16B16A16_UNORM   = 7,  //!< RGBAコンポーネントで16bitの符号なし正規化. 0～1で区切ります
		R16G16B16A16_SNORM   = 8,  //!< RGBAコンポーネントで16bitの符号あり正規化.  -1～1で区切ります
		
		R8G8B8A8_UINT        = 9,   //!< RGBAコンポーネントで8bitのUnsigned int値. 
		R8G8B8A8_SINT        = 10,  //!< RGBAコンポーネントで8bitのSigned int値 . 
		R8G8B8A8_UNORM       = 11,  //!< RGBAコンポーネントで8bitの符号なし正規化.  0～1で区切ります
		R8G8B8A8_SNORM       = 12,  //!< RGBAコンポーネントで8bitの符号あり正規化 .  -1～1で区切ります
		R8G8B8A8_UNORM_SRGB  = 13,  //!< RGBAコンポーネントでSRGBテクスチャに使用. 0～1で区切ります,

		B8G8R8A8_UNORM       = 14,  //!< BGRAコンポーネントで8bitの符号なし正規化.  0～1で区切ります 
		B8G8R8A8_UNORM_SRGB  = 15,  //!< BGRAコンポーネントでSRGBテクスチャに使用. 8bitの符号なし正規化.  0～1で区切ります 

		R32G32B32_FLOAT      = 16,  //!< RGBコンポーネントで32bitのFloat値
		R32G32B32_UINT       = 17,  //!< RGBコンポーネントで32bitのUnsigned int値.
		R32G32B32_SINT       = 18,  //!< RGBコンポーネントで32bitのSigned int値
		                               
		R32G32_FLOAT         = 19,  //!< RGコンポーネントで32bitのFloat値
		R32G32_UINT          = 20,  //!< RGコンポーネントで32bitのUnsigned int値
		R32G32_SINT          = 21,  //!< RGコンポーネントで32bitのSigned int値
		
		R16G16_FLOAT         = 22,  //!< RGコンポーネントで16bitのFloat値
		R16G16_UINT          = 23,  //!< RGコンポーネントで16bitのUnsigned int値
		R16G16_SINT          = 24,  //!< RGコンポーネントで16bitのSigned int値

		R8G8_UINT            = 25,  //!< RGコンポーネントで8bitのUnsigned int値. 
		R8G8_SINT            = 26,  //!< RGコンポーネントで8bitのSigned int値 . 
		R8G8_UNORM           = 27,  //!< RGコンポーネントで8bitの符号なし正規化.  0～1で区切ります
		R8G8_SNORM           = 28,  //!< RGコンポーネントで8bitの符号あり正規化 .  -1～1で区切ります

		D32_FLOAT            = 29,  //!< Depthバッファ用で32bitのFloat値
		R32_FLOAT            = 30,  //!< Rコンポーネントで32bitのFloat値
		R32_UINT             = 31,  //!< Rコンポーネントで32bitのUnsigned int値
		R32_SINT             = 32,  //!< Rコンポーネントで32bitのSigned int値
		
		D16_UNORM            = 33,  //!< Depthバッファ用で16bitのFloat値
		R16_FLOAT            = 34,  //!< Rコンポーネントで16bitのFloat値
		R16_UINT             = 35,  //!< Rコンポーネントで16bitのUnsigned int値
		R16_SINT             = 36,  //!< Rコンポーネントで16bitのSigned int値
		R16_UNORM            = 37,  //!< Rコンポーネントで16bitの符号なし正規化.  0～1で区切ります
		R16_SNORM            = 38,  //!< Rコンポーネントで16bitの符号あり正規化 .  -1～1で区切ります

		R8_UINT              = 39,  //!< Rコンポーネントで8bitのUnsigned int値
		R8_SINT              = 40,  //!< Rコンポーネントで8bitのSigned int値
		R8_UNORM             = 41,  //!< Rコンポーネントで8bitの符号なし正規化.  0～1で区切ります
		R8_SNORM             = 42,  //!< Rコンポーネントで8bitの符号あり正規化 .  -1～1で区切ります
		A8_UNORM             = 43,  //!< Aコンポーネントで8bitの符号なし正規化.  0～1で区切ります

		R1_UNORM             = 44,  //!< Rコンポーネントで1bitの符号なし正規化.  0～1で区切ります.

		D32_FLOAT_S8X24_UINT = 45, //!< 32bitの深度コンポーネントFloat値で設定し, Stencilコンポーネントに8bit, 未使用のコンポーネント24bitを使用します
		D24_UNORM_S8_UINT    = 46, //!< Depth用の24bitで0～1の符号なし正規化を行い, Stencil用のUnsigned int値を8bitで指定します.
		R10G10B10A2_UNORM    = 47, //!< RGBコンポーネントが10bitずつ, Aコンポーネントが2bitの符号なし正規化. 0～1で区切ります
		R10G10B10A2_UINT     = 48, //!< RGBコンポーネントが10bitずつ, Aコンポーネントが2bitのUnsigned int値

		// Block compression format (画像を4x4ピクセル単位のブロックに分割して, それぞれのブロックごとに圧縮を行う方式)
		// https://www.webtech.co.jp/blog/optpix_labs/format/6993/
		BC1_UNORM            = 49, //!< 2 byte RGB(5,6,5bit) + 2 byte RGB(5,6,5bit) + 4 byte(2 bit * 4 pixel * 4 pixelで, 2bitには3色+透明か4色の組み合わせを格納)で0～1のLinearテクスチャ 
		BC1_UNORM_SRGB       = 50, //!< 2 byte RGB(5,6,5bit) + 2 byte RGB(5,6,5bit) + 4 byte(2 bit * 4 pixel * 4 pixelで, 2bitには3色+透明か4色の組み合わせを格納)で0～1のSRGBテクスチャ 
		BC2_UNORM            = 51, //!< 8 byte (4bitでの各ピクセル毎のアルファ値) + 2 byte RGB(5,6,5bit) + 2 byte RGB(5,6,5bit) + 4 byte(2 bit * 4 pixel * 4 pixelで, 2bitには3色+透明か4色の組み合わせを格納)で0～1のLinearテクスチャ
		BC2_UNORM_SRGB       = 52, //!< 8 byte (4bitでの各ピクセル毎のアルファ値) + 2 byte RGB(5,6,5bit) + 2 byte RGB(5,6,5bit) + 4 byte(2 bit * 4 pixel * 4 pixelで, 2bitには3色+透明か4色の組み合わせを格納)で0～1のSRGBテクスチャ
		BC3_UNORM            = 53, //!< 1 byte (アルファの代表値0) + 1 byte (アルファの代表値1) + 6 byte (ピクセル毎のインデックス各3bitを使って線形補間) + 2 byte(RGB 5,6,5bit) + 2 byte(RGB 5,6,5bit) + 4byte(ピクセル毎のインデックス(各2bit)) 
		BC3_UNORM_SRGB       = 54, //!< 1 byte (アルファの代表値0) + 1 byte (アルファの代表値1) + 6 byte (ピクセル毎のインデックス各3bitを使って線形補間) + 2 byte(RGB 5,6,5bit) + 2 byte(RGB 5,6,5bit) + 4byte(ピクセル毎のインデックス(各2bit)) 
		BC4_UNORM            = 55, //!< 1チャンネルの情報だけを格納する(輝度とかハイトマップとか)1 byte(代表値 0), 1 byte (代表値1) + 6 byte(各ピクセル毎のインデックス各3bit) 0.0～1.0
		BC4_SNORM            = 56, //!< 1チャンネルの情報だけを格納する(輝度とかハイトマップとか)1 byte(代表値 0), 1 byte (代表値1) + 6 byte(各ピクセル毎のインデックス各3bit) -1.0～1.0
		BC5_UNORM            = 57, //!< 2チャンネルの情報を格納します(法線マップのxy成分を格納してzは計算で出す等) 1 byte(成分1の代表値0) + 1 byte(成分1の代表値1) + 6 byte成分1の各ピクセル毎のインデックス(3bit)が成分2も同じだけ格納 0.0～1.0
		BC5_SNORM            = 58, //!< 2チャンネルの情報を格納します(法線マップのxy成分を格納してzは計算で出す等) 1 byte(成分1の代表値0) + 1 byte(成分1の代表値1) + 6 byte成分1の各ピクセル毎のインデックス(3bit)が成分2も同じだけ格納 -1.0～1.0
		BC7_UNORM            = 59, //!< BC1～BC3よりも高品質な画像データをRGB, RGBA画像を格納します. テクスチャはLinear. コメントには書ききれないため, 詳細はhttps://www.webtech.co.jp/blog/optpix_labs/format/7006/を参照
		BC7_UNORM_SRGB       = 60, //!< BC1～BC3よりも高品質な画像データをRGB, RGBA画像を格納します. テクスチャはSRGB. コメントには書ききれないため, 詳細はhttps://www.webtech.co.jp/blog/optpix_labs/format/7006/を参照
		BC6H_UFLOAT16        = 61, //!< 4x4ピクセルを16byteに圧縮:HDRのRGB画像でアルファチャンネルはサポートしていない. モードビットで処理が分岐するためコメントには書ききれない. 詳細はhttps://www.webtech.co.jp/blog/optpix_labs/format/7006/を参照. 符号なし
		BC6H_SFLOAT16        = 62, //!< 4x4ピクセルを16byteに圧縮:HDRのRGB画像でアルファチャンネルはサポートしていない. モードビットで処理が分岐するためコメントには書ききれない. 詳細はhttps://www.webtech.co.jp/blog/optpix_labs/format/7006/を参照. 符号あり


		CountOfPixelFormat
	};

	/****************************************************************************
	*				  		PixelChannelFlags
	*************************************************************************//**
	/*  @brief  どのピクセルのチャンネルが使用できるか
	*****************************************************************************/
	enum class PixelChannelFlags : gu::uint8
	{
		None = 0,            //!< 利用不可
		R    = 1 << 0,       //!< Red Channel
		G    = 1 << 1,       //!< Green Channel
		B    = 1 << 2,       //!< Blue Channel
		A    = 1 << 3,       //!< Alpha Channel

		RG   = R | G,        //!< Red, Green Channel
		RGB  = R | G | B,    //!< Red, Green, Blue Channel
		RGBA = R | G | B | A //!< Red, Green, Blue, Alpha Channel 
	};

	/****************************************************************************
	*				  		PixelFormatCapabilities
	*************************************************************************//**
	/*  @brief  対象ピクセルフォーマットに対して, 実現可能なGPUリソースの使い方について定義しています. 
	*****************************************************************************/
	enum class PixelFormatCapabilities : gu::uint32
	{
		None = 0, //!< 利用不可
		Texture1D        = 1ull << 1,  //!< 1次元のテクスチャが利用可能
		Texture2D        = 1ull << 2,  //!< 2次元のテクスチャが利用可能
		Texture3D        = 1ull << 3,  //!< 3次元のテクスチャが利用可能
		TextureCube      = 1ull << 4,  //!< キューブマップテクスチャが利用可能
		RenderTarget     = 1ull << 5,  //!< レンダーターゲットが利用可能
		DepthStencil     = 1ull << 6,  //!< デプスステンシルが利用可能
		TextureMipmap    = 1ull << 7,  //!< ミップマップのテクスチャが利用可能
		TextureLoad      = 1ull << 8,  //!< ロード操作が可能
		TextureSample    = 1ull << 9,  //!< シェーダーからテクスチャのピクセル値を直接サンプリングすることが可能です.
		TextureGather    = 1ull << 10, //!< テクスチャのサンプリング時にBilinear補間に使用される4つのサンプルを取得します. 
		TextureAtomics   = 1ull << 11, //!< テクスチャがアトミック操作 (複数スレッドの書き込み競合状態を避けるための操作)が可能です
		TextureBlendable = 1ull << 12, //!< テクスチャのブレンディングが可能です.
		TextureStore     = 1ull << 13, //!< テクスチャのピクセル値の書き込みが可能です. 

		Buffer           = 1ull << 14, //!< バッファとして利用可能
		VertexBuffer     = 1ull << 15, //!< 頂点バッファとしても利用可能
		IndexBuffer      = 1ull << 16, //!< インデックスバッファとしても利用可能
		BufferLoad       = 1ull << 17, //!< シェーダーからバッファの特定の値を一度に読み込むことが可能です
		BufferStore      = 1ull << 18, //!< シェーダーからバッファの特定の値を書き込むことが可能です. 
		BufferAtomics    = 1ull << 19, //!< バッファがアトミック操作 (複数スレッドの書き込み競合状態を避けるための操作)が可能です

		UAV              = 1ull << 20, //!< Unordered access viewとしてピクセルの読み書きが可能
		UAVLoad          = 1ull << 21, //!< シェーダーからUAVの特定の値を一度に読み込むことが可能です
		UAVStore         = 1ull << 22, //!< シェーダーからUAVの特定の値を書き込むことが可能です.

		/// テクスチャとして利用可能
		AnyTexture = Texture1D | Texture2D | Texture3D | TextureCube,

		/// 全てのテクスチャリソースが使用可能
		AllTexture = AnyTexture | RenderTarget | DepthStencil | TextureMipmap | TextureLoad | TextureSample | TextureGather | TextureAtomics | TextureBlendable | TextureStore,

		/// 全てのバッファとして利用可能
		AllBuffer  = Buffer | VertexBuffer | IndexBuffer | BufferLoad | BufferStore | BufferAtomics,

		/// 全てのUnordered access viewとして利用可能
		AllUAV     = UAV | UAVLoad | UAVStore,

		/// 全てのリソースフラグが利用可能
		AllResource = AllTexture | AllBuffer | AllUAV
	};

	ENUM_CLASS_FLAGS(PixelFormatCapabilities);

	/****************************************************************************
	*				  			  PixelFormatInfo
	*************************************************************************//**
	/*  @brief  対象ピクセルフォーマットに沿った情報を格納しています. @n
	*           基本的に直接扱うというよりはStatic関数から対象のピクセルフォーマット情報を取得してください.
	*****************************************************************************/
	struct PixelFormatInfo
	{
	public:
		#pragma region Static Function
		/*!**********************************************************************
		*  @brief    ピクセルフォーマット情報を取得 (極力Constを使うようにしてください. 範囲外の場合は即時終了します)
		*  @return   ピクセルフォーマット : Globalなフォーマットから取得するため, 値を変える場合は慎重に行ってください. 
		*************************************************************************/
		static PixelFormatInfo& Get(const PixelFormat format);

		/*!**********************************************************************
		*  @brief    ピクセルフォーマットをConstの値で取得
		*  @return   ピクセルフォーマット情報
		*************************************************************************/
		static const PixelFormatInfo& GetConst(const PixelFormat format);
		#pragma endregion

		#pragma region Public Function

		#pragma region Format Check
		/*!**********************************************************************
		*  @brief    整数フォーマットか
		*  @return   整数フォーマットUnsigned intとSigned intであればtrue
		*************************************************************************/
		bool IsInteger() const;

		/*!**********************************************************************
		*  @brief    Floatフォーマットか
		*  @return   Floatフォーマットである場合はtrue
		*************************************************************************/
		bool IsFloat() const ;

		/*!**********************************************************************
		*  @brief    HDRで使用されるフォーマットか
		*  @return   HDRで使用できるフォーマット (1.0以上の値を格納出来るフォーマット)であればtrue
		*************************************************************************/
		bool IsHDR() const;

		/*!**********************************************************************
		*  @brief    DepthかStencilで使用されているフォーマットか
		*  @return   DepthStencilで使用できるフォーマットの場合true
		*************************************************************************/
		bool IsDepthStencil() const;

		/*!**********************************************************************
		*  @brief    Stencilのみ使用されているフォーマットか
		*  @return   Stencilで使用できる (uint8のようなもの)場合true
		*************************************************************************/
		bool IsStencil() const;

		/*!**********************************************************************
		*  @brief    ブロック圧縮のフォーマットであるかどうか
		*  @return   テクスチャのピクセルをブロックごとに分割して圧縮するフォーマットの場合true
		*************************************************************************/
		bool IsBlockCompressed() const;
		#pragma endregion

		#pragma endregion 

		#pragma region Public Member Variables
		/*! @brief ブロック圧縮データに主に使用します. 圧縮のピクセル個数です(幅方向)*/
		gu::int32 BlockSizeX = 0;

		/*! @brief ブロック圧縮データに主に使用します. 圧縮のピクセル個数です(高さ方向)*/
		gu::int32 BlockSizeY = 0;

		/*! @brief ブロック圧縮データに主に使用します. 圧縮のピクセル個数です(奥行方向)*/
		gu::int32 BlockSizeZ = 0;

		/*! @brief ブロック圧縮データによるバイトサイズ. もしくは1ピクセルに必要なバイトサイズ*/
		gu::int32 BlockBytes = 0;

		/*! @brief チャンネルのカウントです. (RGBAの場合, 4を入れることになります)*/
		gu::int32 ChannelCount = 0;

		/*! @brief 対象ピクセルフォーマットに対して, 実現可能なGPUリソースの使い方を定義*/
		PixelFormatCapabilities Capabilities = PixelFormatCapabilities::None;

		/*! @brief プラットフォームで設定するピクセルフォーマット (DirectX12やVulkanのEnum管理に使用します.)*/
		gu::uint32 PlatformFormat = 0;

		/*! @brief 画像データの設定情報 ()*/
		PixelFormat Format = PixelFormat::Unknown;

		/*! @brief 今回のプラットフォームとレンダリングの組み合わせでサポートされているフォーマットか*/
		bool Supported = true;

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief デフォルトコンストラクタ*/
		PixelFormatInfo() = default;

		/*! @brief 初期化に使用します*/
		PixelFormatInfo(const PixelFormat format,
			const gu::int32 blockSizeX, const gu::int32 blockSizeY, const gu::int32 blockSizeZ,
			const gu::int32 blockBytes, const gu::int32 channelCount, const bool supported) :
			Format(format), BlockSizeX(blockSizeX), BlockSizeY(blockSizeY), BlockSizeZ(blockSizeZ),
			BlockBytes(blockBytes), ChannelCount(channelCount), Supported(supported) {};
		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor

		#pragma endregion 

		#pragma region Protected Function

		#pragma endregion 

		#pragma region Protected Member Variables

		#pragma endregion

	private:
		#pragma region Private Constructor and Destructor

		#pragma endregion

		#pragma region Private Function

		#pragma endregion 

		#pragma region Private Member Variables

		#pragma endregion 

	};

}
#endif