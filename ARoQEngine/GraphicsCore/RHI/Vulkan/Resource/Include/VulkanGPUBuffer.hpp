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
		*  @brief  �@�@GPU�Ƀ�������z�u���܂�. �Z�ʂ������悤��byte�P�ʂŎw�肵�܂�.
		*  @param[in] const void* : GPU�ɃA�b�v���[�h����CPU���̃������z��
		*  @param[in] const gu::uint64 �������̊m�ۂ���o�C�g�T�C�Y
		*  @param[in] const gu::uint64 ���������m�ۂ��鏉���I�t�Z�b�g [byte]
		*  @param[in] const gu::SharedPointer<RHICommandList> Graphics��Copy�̃R�}���h���X�g
		*  @param[in] const bool : �蓮�Ń}�b�v���s����
		*  @return    void
		*************************************************************************/
		virtual void UploadByte([[maybe_unused]]const void* data, [[maybe_unused]] const gu::uint64 allocateByteSize, [[maybe_unused]] const gu::uint64 offsetByte, [[maybe_unused]] const gu::SharedPointer<core::RHICommandList>& commandList, [[maybe_unused]] const bool useMapManually) override {};

		/*!**********************************************************************
		*  @brief  �@�@�z��̗v�f���w�肷��C���f�b�N�X���g����CPU����GPU�Ƀ�������z�u���܂�.
		*  @note      �ÖٓI�ɓ����o�C�g���̕��т����݂��邱�Ƃ����߂��܂�
		*  @param[in] const void* : GPU�ɃA�b�v���[�h������CPU���̃������z��
		*  @param[in] const gu::uint64 : �z��̗v�f��
		*  @param[in] const gu::uint64 : ���������m�ۂ��鏉���C���f�b�N�X
		*  @param[in] const gu::SharedPointer<RHICommandList> Graphics��Copy�̃R�}���h���X�g
		*  @param[in] const bool : �蓮�Ń}�b�v���s����
		*  @return    void
		*************************************************************************/
		virtual void UploadIndex([[maybe_unused]] const void* data, [[maybe_unused]] const gu::uint64 elementCount, [[maybe_unused]] const gu::uint64 offsetIndex, [[maybe_unused]] const gu::SharedPointer<core::RHICommandList>& commandList, [[maybe_unused]] const bool useMapManually) override;

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		VkBuffer GetBuffer() const noexcept { return _buffer; }

		void SetName(const gu::tstring& name) override;

		/*!**********************************************************************
		*  @brief     �o�b�t�@�Ƃ��Ă�GPU���\�[�X���ǂ����𔻒肵�܂�.
		*************************************************************************/
		__forceinline virtual bool IsBuffer() const override { return true; }

		/*!**********************************************************************
		*  @brief     �e�N�X�`���Ƃ��Ă�GPU���\�[�X���ǂ����𔻒肵�܂�.
		*************************************************************************/
		__forceinline virtual bool IsTexture() const override { return false; }

		/*!**********************************************************************
		*  @brief     Map�ɓo�^�����CPU�������̍ŏ��̃|�C���^��Ԃ��܂�
		*  @return    gu::uint8* �}�b�v�����擪�|�C���^
		*************************************************************************/
		virtual gu::uint8* GetCPUMappedAddress() override { return _mappedData; }

		/*!**********************************************************************
		*  @brief     GPU�̃A�h���X��gu::uint64�Ƃ����`�ŉ��z�I�ɕ\���������̂�Ԃ��܂�.
		*  @return    gu::uint64 GPU�̃A�h���X�l
		*************************************************************************/
		virtual gu::uint64 GetGPUVirtualAddress()override { return 0; };

		/*!**********************************************************************
		*  @brief     �����_��GPUResource�̈����� (IndexBuffer�Ƃ��Ďg�p����Ȃ�...)��ݒ肵�܂�
		*  @attention �蓮�ł̐؂�ւ��͊�{�I�ɍs��Ȃ��ł�������. (���̊֐��̓o���A�̎g�p��ړI�Ƃ��Ďg�p���܂�.)
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
		*  @brief     �蓮��CPU����GPU�Ƀf�[�^���A�b�v���[�h���鏀���Ƃ��Ďg�p���܂�.
		*  @attention subresource�̃C���f�b�N�X�̓o�b�t�@�Ƃ��Ă̗��p�����l���Ă��Ȃ�����, 0���������܂�
		*************************************************************************/
		virtual void Map() override;

		/*!**********************************************************************
		*  @brief     CPU����GPU�Ƀf�[�^���A�b�v���[�h����̂��~�߂�ꍇ�Ɏg�p���܂�.
		*  @attention 1�t���[���œ��ꃊ�\�[�X�ɉ����map, unmap���Ă΂Ȃ��悤�ɂ��Ă�������. (�������ׂ̊ϓ_��) @n
		*             subresource�̃C���f�b�N�X�̓o�b�t�@�Ƃ��Ă̗��p�����l���Ă��Ȃ�����, 0���������܂�
		*************************************************************************/
		virtual void Unmap() override;

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		// �{��
		VkDeviceMemory _memory = nullptr;

		VkBuffer       _buffer = nullptr;

		// GPU���������A�N�Z�X�ł��Ȃ���ɃR�s�[�����s�����ߒ��ԃo�b�t�@
		VkBuffer _stagingBuffer = nullptr;

		VkDeviceMemory _stagingMemory = nullptr;

		std::uint8_t* _stagingMappedData = nullptr;
		std::uint8_t* _mappedData = nullptr;
	private:
		void Prepare(VkBuffer& buffer, VkDeviceMemory& memory, VkMemoryPropertyFlags flags);
	};


}
#endif