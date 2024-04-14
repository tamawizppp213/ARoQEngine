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
	*************************************************************************//**
	*  @class     GPUBuffer
	*  @brief     Buffer
	*****************************************************************************/
	class GPUBuffer : public core::GPUBuffer
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		// @brief : Basically for Default Buffer Initialize. Total Buffer Copy
		//          Create temp upload buffer and copy this to default buffer
		void Pack(const void* data, const gu::SharedPointer<core::RHICommandList>& copyCommandList) override;
		
		// @brief : Begin Map Function
		void CopyStart() override;
		
		// @brief : GPU copy the specified range
		void CopyTotalData(const void* data, const size_t dataLength, const size_t indexOffset = 0) override;
		
		/*----------------------------------------------------------------------
		*  @brief :  Call at once in each frame (If you need). CopyStart + CopyTotalData + CopyEnd.
		/*----------------------------------------------------------------------*/
		virtual void Update(const void* data, const gu::uint64 dataLength) override {};

		virtual void Upload(const void* data, const gu::uint64 allocateByteSize, const gu::uint64 offsetByte = 0, const gu::SharedPointer<core::RHICommandList>& commandList = nullptr) override {}

		// @brief : Unmap Function
		void CopyEnd() override;

		/****************************************************************************
		**                Public Member Variables
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

		/****************************************************************************
		**                Protected Member Variables
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