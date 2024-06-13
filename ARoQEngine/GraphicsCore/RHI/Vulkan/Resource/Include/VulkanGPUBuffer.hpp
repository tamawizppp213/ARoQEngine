//////////////////////////////////////////////////////////////////////////////////
///             @file   VulkanGPUSampler.hpp
///             @brief  Sampler State 
///             @author Toide Yutaro
///             @date   2022_07_08
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VULKAN_GPU_BUFFER_HPP
#define Vulkan_GPU_BUFFER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUBuffer.hpp"
#include <vulkan/vulkan.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::vulkan
{
	/****************************************************************************
	*				  			GPUBuffer
	****************************************************************************/
	/* @class     GPUBuffer
	*  @brief     Buffer
	*****************************************************************************/
	class GPUBuffer : public core::GPUBuffer
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*!**********************************************************************
		*  @brief  　　GPUにメモリを配置します. 融通が効くようにbyte単位で指定します.
		*  @param[in] const void* : GPUにアップロードしたCPU側のメモリ配列
		*  @param[in] const gu::uint64 メモリの確保するバイトサイズ
		*  @param[in] const gu::uint64 メモリを確保する初期オフセット [byte]
		*  @param[in] const gu::SharedPointer<RHICommandList> GraphicsかCopyのコマンドリスト
		*  @param[in] const bool : 手動でマップを行うか
		*  @return    void
		*************************************************************************/
		virtual void UploadByte([[maybe_unused]]const void* data, [[maybe_unused]] const gu::uint64 allocateByteSize, [[maybe_unused]] const gu::uint64 offsetByte, [[maybe_unused]] const gu::SharedPointer<core::RHICommandList>& commandList, [[maybe_unused]] const bool useMapManually) override {};

		/*!**********************************************************************
		*  @brief  　　配列の要素を指定するインデックスを使ってCPUからGPUにメモリを配置します.
		*  @note      暗黙的に同じバイト数の並びが存在することが求められます
		*  @param[in] const void* : GPUにアップロードしたいCPU側のメモリ配列
		*  @param[in] const gu::uint64 : 配列の要素数
		*  @param[in] const gu::uint64 : メモリを確保する初期インデックス
		*  @param[in] const gu::SharedPointer<RHICommandList> GraphicsかCopyのコマンドリスト
		*  @param[in] const bool : 手動でマップを行うか
		*  @return    void
		*************************************************************************/
		virtual void UploadIndex([[maybe_unused]] const void* data, [[maybe_unused]] const gu::uint64 elementCount, [[maybe_unused]] const gu::uint64 offsetIndex, [[maybe_unused]] const gu::SharedPointer<core::RHICommandList>& commandList, [[maybe_unused]] const bool useMapManually) override;

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		VkBuffer GetBuffer() const noexcept { return _buffer; }

		void SetName(const gu::tstring& name) override;

		/*!**********************************************************************
		*  @brief     バッファとしてのGPUリソースかどうかを判定します.
		*************************************************************************/
		__forceinline virtual bool IsBuffer() const override { return true; }

		/*!**********************************************************************
		*  @brief     テクスチャとしてのGPUリソースかどうかを判定します.
		*************************************************************************/
		__forceinline virtual bool IsTexture() const override { return false; }

		/*!**********************************************************************
		*  @brief     Mapに登録されるCPUメモリの最初のポインタを返します
		*  @return    gu::uint8* マップした先頭ポインタ
		*************************************************************************/
		virtual gu::uint8* GetCPUMappedAddress() override { return _mappedData; }

		/*!**********************************************************************
		*  @brief     GPUのアドレスをgu::uint64という形で仮想的に表現したものを返します.
		*  @return    gu::uint64 GPUのアドレス値
		*************************************************************************/
		virtual gu::uint64 GetGPUVirtualAddress()override { return 0; };

		/*!**********************************************************************
		*  @brief     現時点のGPUResourceの扱い方 (IndexBufferとして使用するなど...)を設定します
		*  @attention 手動での切り替えは基本的に行わないでください. (この関数はバリアの使用を目的として使用します.)
		*  @return    void
		*************************************************************************/
		__forceinline virtual void SetResourceState(const core::ResourceState state) override { _metaData.State = state; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUBuffer() = default;

		~GPUBuffer();

		explicit GPUBuffer(const gu::SharedPointer<core::RHIDevice>& device, const core::GPUBufferMetaData& metaData, const gu::tstring& name = SP("Buffer"));
		
	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		/*!**********************************************************************
		*  @brief     手動でCPUからGPUにデータをアップロードする準備として使用します.
		*  @attention subresourceのインデックスはバッファとしての利用しか考えていないため, 0が代入されます
		*************************************************************************/
		virtual void Map() override;

		/*!**********************************************************************
		*  @brief     CPUからGPUにデータをアップロードするのを止める場合に使用します.
		*  @attention 1フレームで同一リソースに何回もmap, unmapを呼ばないようにしてください. (処理負荷の観点で) @n
		*             subresourceのインデックスはバッファとしての利用しか考えていないため, 0が代入されます
		*************************************************************************/
		virtual void Unmap() override;

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		// 本体
		VkDeviceMemory _memory = nullptr;

		VkBuffer       _buffer = nullptr;

		// GPUだけしかアクセスできないやつにコピー等を行うため中間バッファ
		VkBuffer _stagingBuffer = nullptr;

		VkDeviceMemory _stagingMemory = nullptr;

		std::uint8_t* _stagingMappedData = nullptr;
		std::uint8_t* _mappedData = nullptr;
	private:
		void Prepare(VkBuffer& buffer, VkDeviceMemory& memory, VkMemoryPropertyFlags flags);
	};


}
#endif