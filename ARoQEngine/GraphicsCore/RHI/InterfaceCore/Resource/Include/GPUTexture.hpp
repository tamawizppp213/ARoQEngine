//////////////////////////////////////////////////////////////////////////////////
///  @file   GPUTexture.hpp
///  @brief  テクスチャとして使用する場合のGPUリソースです.
///  @author Toide Yutaro
///  @date   2024_04_21
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GPU_TEXTURE_HPP
#define GPU_TEXTURE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GPUResource.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gm
{
	struct RGBA;
}

namespace rhi::core
{
	class RHIDevice;
	class RHICommandList;
	class RHICommandQueue;
	class GPUBuffer;

	/****************************************************************************
	*				  			GPUTexture
	****************************************************************************/
	/*  @brief  テクスチャとして使用する場合のGPUリソースです.
	*****************************************************************************/
	class GPUTexture : public GPUResource, public gu::EnableSharedFromThis<GPUTexture>
	{
	public:
		#pragma region Public Function
		virtual void Load(const gu::tstring& filePath, const gu::SharedPointer<RHICommandList>& commandList) = 0;

		virtual void Save(const gu::tstring& filePath, const gu::SharedPointer<RHICommandList>& commandList, const gu::SharedPointer<RHICommandQueue>& commandQueue) = 0;

		virtual void Write(const gu::SharedPointer<RHICommandList>& commandList, const gm::RGBA* pixel) = 0;
		#pragma endregion
		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     テクスチャの幅をpixel単位で指定します. その際, ミップマップレベルを指定すればその値だけサイズを半分にします.
		*  @param[in] const gu::uint8 テクスチャのミップマップレベル
		*  @return    gu::uint32 テクスチャの幅(pixel)
		*************************************************************************/
		__forceinline gu::uint32 GetWidth(const gu::uint8 mipLevel = 0) const noexcept
		{
			return _metaData.Width > 1u << mipLevel ? 1u : _metaData.Width >> mipLevel;
		}

		/*!**********************************************************************
		*  @brief     テクスチャの高さをpixel単位で指定します. その際, ミップマップレベルを指定すればその値だけサイズを半分にします.
		*  @param[in] const gu::uint8 テクスチャのミップマップレベル
		*  @return    gu::uint32 テクスチャの高さ(pixel)
		*************************************************************************/
		__forceinline gu::uint32 GetHeight(const gu::uint8 mipLevel = 0) const noexcept
		{
			return _metaData.Height > 1u << mipLevel ? 1u : _metaData.Height >> mipLevel;
		}

		/*!**********************************************************************
		*  @brief     テクスチャのDepthをpixel単位で指定します. その際, ミップマップレベルを指定すればその値だけサイズを半分にします.
		*  @param[in] const gu::uint8 テクスチャのミップマップレベル
		*  @return    gu::uint16 テクスチャのDepth(pixel)
		*************************************************************************/
		gu::uint16 GetDepth (const gu::uint8 mipLevel = 0) const noexcept;

		/*!**********************************************************************
		*  @brief     テクスチャのWidth, Height, Depthをpixel単位で指定します
		*  @param[in] const gu::uint8 テクスチャのミップマップレベル
		*  @return    gm::Vector3i<uint32>
		*************************************************************************/
		__forceinline gm::Vector3i<gu::uint32> GetTexelSize(const gu::uint8 mipLevel = 0) const noexcept
		{
			return gm::Vector3i<gu::uint32>(GetWidth(mipLevel), GetHeight(0), static_cast<gu::uint32>(GetDepth()));
		}

		/*!**********************************************************************
		*  @brief     テクスチャ全体のバイトサイズを計算します. 
		*  @param[in] const gu::uint8 テクスチャのミップマップレベル
		*  @return    gu::uint64 テクスチャのバイトサイズ
		*************************************************************************/
		gu::uint64 GetByteSize(const gu::uint8 mipLevel = 0) const noexcept;

		/*!**********************************************************************
		*  @brief     1ピクセルの型定義を返します
		*  @return    PixelFormat 1ピクセルの型定義
		*************************************************************************/
		__forceinline PixelFormat GetPixelFormat() const noexcept { return _metaData.PixelFormat; }

		/*!**********************************************************************
		*  @brief     サンプリングの数を指定します. 基本的にはCount1を使用し, MSAAなどを使用するときはCount4を主に使用します.
		*  @return    MultiSample : サンプリングの数
		*************************************************************************/
		__forceinline MultiSample GetMultiSample() const noexcept { return _metaData.Sample; }

		/*!**********************************************************************
		*  @brief     テクスチャの次元数を設定します.
		*  @return    ResourceDimension : テクスチャの次元数
		*************************************************************************/
		__forceinline ResourceDimension GetDimension() const noexcept { return _metaData.Dimension; }

		/*!**********************************************************************
		*  @brief     テクスチャの使用方法を取得します @n
		*             ResourceTypeはTexture2Dなどキューブマップとしての使用などを定義するものです. @n
		* 　　　　　　　 TextureCreateFlagsはレンダーターゲットとしてGPUリソースを使用可能などを定義するものです.
		*  @return    ResourceType テクスチャの使用方法
		*************************************************************************/
		__forceinline ResourceType GetResourceType() const noexcept { return _metaData.ResourceType; }

		/*!**********************************************************************
		*  @brief     テクスチャのGPUリソースとしての使用方法を取得します. @n
		*             ResourceTypeはTexture2Dなどキューブマップとしての使用などを定義し, @n
		* 　　　　　　　 これはレンダーターゲットとしてGPUリソースを使用可能などを定義するものです.
		*  @return    TextureCreateFlags テクスチャ作成方法
		*************************************************************************/
		__forceinline TextureCreateFlags GetUsage() const noexcept { return _metaData.Usage; }

		/*!**********************************************************************
		*  @brief     テクスチャ初期化時に初期化される色を指定します. @n
		*             レンダーターゲットやデプスステンシルを使用するときに使用し, デフォルトは白色です
		*  @return    ClearValue : クリアカラー
		*************************************************************************/
		__forceinline ClearValue GetClearValue () const noexcept { return _metaData.ClearColor; }

		/*!**********************************************************************
		*  @brief     GPUリソースのバリア状態を定義します.
		*  @return    core::ResourceState GPUリソースバリア状態
		*************************************************************************/
		__forceinline core::ResourceState GetResourceState() const noexcept override { return _metaData.State; }

		/*!**********************************************************************
		*  @brief     ミップマップの段階数 (2を指定すると, ミップマップの値が0と1のものを使用するという意味です)
		*  @return    gu::uint8 ミップマップの段階数
		*************************************************************************/
		__forceinline gu::uint8 GetMipMapCount() const noexcept { return _metaData.MipMapLevels; }

		/*!**********************************************************************
		*  @brief     ミップマップの設定可能な最大レベル (2を指定すると, ミップマップの値が0と1のものを使用するという意味です)
		*  @return    gu::uint8 ミップマップの最大レベル
		*************************************************************************/
		__forceinline gu::uint8 GetMaxMipMapLevel() const noexcept { return _metaData.MipMapLevels >= 1? _metaData.MipMapLevels - 1 : 0; }

		/*!**********************************************************************
		*  @brief     ミップマップの設定可能な最小レベル (現在は0が前提です)
		*  @return    gu::uint8 ミップマップの最小レベル
		*************************************************************************/
		__forceinline gu::uint8 GetMinMipMapLevel() const noexcept { return 0; }

		/*!**********************************************************************
		*  @brief     テクスチャの配列長を返します
		*  @return    gu::uint16 配列長
		*************************************************************************/
		__forceinline gu::uint16 GetArrayLength() const noexcept { return _metaData.Dimension == ResourceDimension::Texture3D ?  1 : _metaData.DepthOrArraySize; }

		/*!**********************************************************************
		*  @brief     テクスチャ配列として使用されているか
		*  @return    bool テクスチャ配列として使用している場合はtrueを返します. (3次元テクスチャはfalseを指定します)
		*************************************************************************/
		__forceinline bool IsArray() const noexcept {  return GetArrayLength() != 1; }

		/*!**********************************************************************
		*  @brief     サブリソースIDを取得します 
		*  @note      https://learn.microsoft.com/ja-jp/windows/win32/direct3d12/subresources
		*  @param[in] const gu::uint32 mipSlice   : 取得したいミップマップのレベルを指定します.  
		*  @param[in] const gu::uint32 arraySlice : テクスチャ配列において, テクスチャの種類を指定する操作に対応します
		*  @param[in] const gu::uint32 planeSlice : RGBA以外のテクスチャフォーマットかつ, DepthStencilテクスチャのような複数プレーンを持ったものに使用します.@n
		*                                           基本は0に設定されます. DepthStencilの場合Depthが0でStencilが1を設定することになります.
		*  @return    gu::uint32 サブリソースID   
		*************************************************************************/
		__forceinline gu::uint32 GetSubresourceIndex(const gu::uint32 mipSlice, const gu::uint32 arraySlice, const gu::uint32 planeSlice = 0) const noexcept 
		{
			const gu::uint32 mipmapLevel = GetMaxMipMapLevel();
			return mipSlice + (arraySlice * mipmapLevel) + (planeSlice * mipmapLevel * static_cast<gu::uint32>(GetArrayLength()));
		}

		/*!**********************************************************************
		*  @brief     物理的に確保しているテクスチャサイズを返します. 
		*************************************************************************/
		__forceinline gu::uint64 GetPhysicalSize() const noexcept { return _physicalSize; }

		/*!**********************************************************************
		*  @brief     テクスチャのアラインメントを指定します
		*************************************************************************/
		__forceinline gu::uint64 GetAlignment() const noexcept { return _alignment; }

		/*!**********************************************************************
		*  @brief     Textureの作成情報を一通り管理している構造体を変換
		*  @return    GPUTextureMetaData
		*************************************************************************/
		__forceinline GPUTextureMetaData& GetMetaData() { return _metaData; }

		/*!**********************************************************************
		*  @brief     Textureの作成情報を一通り管理している構造体を変換
		*  @return    GPUTextureMetaData
		*************************************************************************/
		__forceinline const GPUTextureMetaData& GetMetaData() const noexcept { return _metaData; }

		#pragma endregion 

		#pragma region Public Constructor and Destuctor
		#pragma endregion

	protected:
		#pragma region Protected Constructor and Destructor
		/*! @brief デフォルトコンストラクタ*/
		GPUTexture() = default;

		/*! @brief デストラクタ*/
		~GPUTexture() = default;

		/*! @brief 論理デバイスを使って初期化します. */
		explicit GPUTexture(const gu::SharedPointer<RHIDevice>& device, [[maybe_unused]]const gu::tstring& name = SP("Texture")) : core::GPUResource(device) {};
		
		/*! @brief テクスチャ作成情報と論理デバイスを使って初期化*/
		explicit GPUTexture(const gu::SharedPointer<RHIDevice>& device, const GPUTextureMetaData& metaData, [[maybe_unused]] const gu::tstring& name): core::GPUResource(device), _metaData(metaData) {};
		#pragma endregion 
		
		#pragma region Protected Function

		#pragma endregion Protected Function

		#pragma region Protected Property
		/*! @brief テクスチャの作成情報を一通り管理している構造体*/
		GPUTextureMetaData _metaData;

		/*! @brief 物理的に確保しているテクスチャサイズ*/
		gu::uint64 _physicalSize = 0;

		/*! @brief アラインメント*/
		gu::uint64 _alignment = 0;
		#pragma endregion 

	};
}
#endif