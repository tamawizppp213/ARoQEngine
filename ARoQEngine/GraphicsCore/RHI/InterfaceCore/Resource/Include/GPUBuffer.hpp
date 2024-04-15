//////////////////////////////////////////////////////////////////////////////////
///  @file   GPUBuffer.hpp
///  @brief  �e�N�X�`���ȊO�̃o�b�t�@. ���_, �C���f�b�N�X�o�b�t�@�Ȃǂ̍쐬�Ɏg�p���܂�. 
///  @author Toide Yutaro
///  @date   2024_04_09
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GPU_BUFFER_HPP
#define GPU_BUFFER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GPUResource.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace rhi::core
{
	class RHICommandList;
	/****************************************************************************
	*				  			GPUBuffer
	*************************************************************************//**
	*  @class     GPUBuffer
	*  @brief     �e�N�X�`���ȊO�̃o�b�t�@. ���_, �C���f�b�N�X�o�b�t�@�Ȃǂ̍쐬�Ɏg�p���܂�.
	*****************************************************************************/
	class  GPUBuffer : public GPUResource, public gu::EnableSharedFromThis<GPUBuffer>
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief  �@�@GPU�Ƀ�������z�u���܂�. �Z�ʂ������悤��byte�P�ʂŎw�肵�܂�. 
		*  @param[in] const void* : GPU�ɃA�b�v���[�h����CPU���̃������z��
		*  @param[in] const gu::uint64 �������̊m�ۂ���o�C�g�T�C�Y
		*  @param[in] const gu::uint64 ���������m�ۂ��鏉���I�t�Z�b�g [byte]
		*  @param[in] const gu::SharedPointer<RHICommandList> Graphics��Copy�̃R�}���h���X�g
		*  @return    void
		*************************************************************************/
		virtual void Upload(const void* data, const gu::uint64 allocateByteSize, const gu::uint64 offsetByte = 0, const gu::SharedPointer<RHICommandList>& commandList = nullptr) = 0;
		
		/*----------------------------------------------------------------------
		*  @brief :  Call at once in each frame (If you need). CopyStart + CopyTotalData + CopyEnd. 
		/*----------------------------------------------------------------------*/
		virtual void Update(const void* data, const gu::uint64 dataLength) = 0;
		
		/*----------------------------------------------------------------------
		*  @brief :  Call map function
		/*----------------------------------------------------------------------*/
		virtual void CopyStart() = 0;
		 
		/*----------------------------------------------------------------------
		*  @brief :  GPU copy the specified range
		/*----------------------------------------------------------------------*/
		virtual void CopyTotalData(const void* data, const gu::uint64 dataLength, const gu::uint64 indexOffset = 0) = 0;
		
		/*----------------------------------------------------------------------
		*  @brief :  Call unmap function
		/*----------------------------------------------------------------------*/
		virtual void CopyEnd() = 0;

		#pragma endregion 
		#pragma region Public Member Variables
		/*!**********************************************************************
		*  @brief     �o�b�t�@�Ƃ��Ă�GPU���\�[�X���ǂ����𔻒肵�܂�.
		*************************************************************************/
		__forceinline virtual bool IsBuffer() const override { return true; }

		/*!**********************************************************************
		*  @brief     �e�N�X�`���Ƃ��Ă�GPU���\�[�X���ǂ����𔻒肵�܂�.
		*************************************************************************/
		__forceinline virtual bool IsTexture() const override { return false; }

		/*!**********************************************************************
		*  @brief     �o�b�t�@�Ɋi�[�����v�f�̍ő吔��Ԃ��܂�. 
		*  @param[in] void
		*  @return    gu::uint64 �o�b�t�@�Ɋi�[�����v�f�̍ő��
		*************************************************************************/
		__forceinline gu::uint64 GetElementCount() const { return _metaData.Count; }

		/*!**********************************************************************
		*  @brief     1�v�f�ɕK�v�Ȍ^���̃o�C�g��
		*  @param[in] void
		*  @return    gu::uint64 �^�̃o�C�g��
		*************************************************************************/
		__forceinline gu::uint64 GetElementByteSize() const { return _metaData.Stride; }

		/*!**********************************************************************
		*  @brief     GPUBuffer�Ŏg�p���Ă���S�̂̃o�C�g�����v�Z���܂�. elementCount * elementByteSize�ŕԂ��܂�. 
		*  @param[in] void
		*  @return    gu::uint64 �S�̂̃o�C�g��
		*************************************************************************/
		__forceinline gu::uint64 GetTotalByteSize() const { return _metaData.GetTotalByte(); }

		/*----------------------------------------------------------------------
		*  @brief :  Return GPU Resource Type. (Basically Buffer or RaytracingAccelerationStructure) 
		/*----------------------------------------------------------------------*/
		__forceinline ResourceType  GetResourceType() const { return _metaData.ResourceType; }
		
		/*!**********************************************************************
		*  @brief     GPU���\�[�X�̏�Ԃ��`���܂�. ���\�[�X��Ԃ�ύX����ۂ�, TransitionResourceState(�R�}���h���X�g)���g�p���Ă�������.  
		*  @return    ResourceState : GPU���\�[�X�̏��
		*************************************************************************/
		ResourceState GetResourceState() const noexcept override { return _metaData.State; }
		
		/*----------------------------------------------------------------------
		*  @brief :  Return Buffer Usage Flag. (Vertex, Index, or Constant Buffer)
		/*----------------------------------------------------------------------*/
		__forceinline ResourceUsage GetUsage() const { return _metaData.ResourceUsage; }
		
		/*----------------------------------------------------------------------
		*  @brief :  Return Heap region type (Default, Upload, Readback, Custom)
		/*----------------------------------------------------------------------*/
		__forceinline MemoryHeap GetMemoryHeapType() const { return _metaData.HeapType; }

		/*!**********************************************************************
		*  @brief     Map�ɓo�^�����CPU�������̍ŏ��̃|�C���^��Ԃ��܂�
		*  @return    gu::uint8* �}�b�v�����擪�|�C���^
		*************************************************************************/
		virtual gu::uint8* GetCPUMappedAddress() = 0;

		/*!**********************************************************************
		*  @brief     GPU�̃A�h���X��gu::uint64�Ƃ����`�ŉ��z�I�ɕ\���������̂�Ԃ��܂�. 
		*  @return    gu::uint64 GPU�̃A�h���X�l
		*************************************************************************/
		virtual gu::uint64 GetGPUVirtualAddress() = 0;

		__forceinline GPUBufferMetaData& GetMetaData()                      { return _metaData; }
		__forceinline const GPUBufferMetaData& GetMetaData() const noexcept { return _metaData; }
		
		/*!**********************************************************************
		*  @brief     �����_��GPUResource�̈����� (IndexBuffer�Ƃ��Ďg�p����Ȃ�...)��ݒ肵�܂�
		*  @attention �蓮�ł̐؂�ւ��͊�{�I�ɍs��Ȃ��ł�������. (���̊֐��̓o���A�̎g�p��ړI�Ƃ��Ďg�p���܂�.)
		*  @return    void
		*************************************************************************/
		__forceinline virtual void SetResourceState(const core::ResourceState state) override { _metaData.State = state;}
		#pragma endregion 
		
	protected:
		#pragma region Protected Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		GPUBuffer();

		/*! @brief �f�X�g���N�^*/
		~GPUBuffer() = default;

		/*! @brief �o�b�t�@�̍쐬�����g���R���X�g���N�^*/
		explicit GPUBuffer(const gu::SharedPointer<RHIDevice>& device, const core::GPUBufferMetaData& metaData, const gu::tstring& name);
		
		#pragma endregion
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		GPUBufferMetaData _metaData = {};
	};
}


#endif