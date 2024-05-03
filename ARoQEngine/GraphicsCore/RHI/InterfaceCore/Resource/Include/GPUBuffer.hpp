//////////////////////////////////////////////////////////////////////////////////
///  @file   GPUBuffer.hpp
///  @brief  �e�N�X�`���ȊO�̃o�b�t�@. ���_, �C���f�b�N�X�o�b�t�@�Ȃǂ̍쐬�Ɏg�p���܂�. @n
///          �o�b�t�@�̊m�ۂ��s���ꍇ�͎��O��GPUBufferMetaData���g���ă������̈���m�ۂ��Ă���@n
///          CPU����GPU�Ƀf�[�^���A�b�v���[�h���܂�. Upload�֐���2��ނ���, @n
///          UploadIndex : �z��̃C���f�b�N�X���w�肷�邱�ƂŊm�ۂ������ @n
/// �@�@�@�@�@ UploadByte : �o�C�g���𒼐ڎw�肷�邱�ƂŊm�ۂ������������܂�. 
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
	/* @class     GPUBuffer
	*  @brief     �e�N�X�`���ȊO�̃o�b�t�@. ���_, �C���f�b�N�X�o�b�t�@�Ȃǂ̍쐬�Ɏg�p���܂�.
	*****************************************************************************/
	class GPUBuffer : public GPUResource, public gu::EnableSharedFromThis<GPUBuffer>
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief  �@�@GPU�Ƀ�������z�u���܂�. �Z�ʂ������悤��byte�P�ʂŎw�肵�܂�. 
		*  @param[in] const void* : GPU�ɃA�b�v���[�h������CPU���̃������z��
		*  @param[in] const gu::uint64 �������̊m�ۂ���o�C�g�T�C�Y
		*  @param[in] const gu::uint64 ���������m�ۂ��鏉���I�t�Z�b�g [byte]
		*  @param[in] const gu::SharedPointer<RHICommandList> Graphics��Copy�̃R�}���h���X�g
		*  @param[in] const bool �蓮�Ń}�b�v���s����
		*  @return    void
		*************************************************************************/
		virtual void UploadByte(const void* data, const gu::uint64 allocateByteSize, const gu::uint64 offsetByte = 0, const gu::SharedPointer<RHICommandList>& commandList = nullptr, const bool useMapManually = false) = 0;
		 
		/*!**********************************************************************
		*  @brief  �@�@�z��̗v�f���w�肷��C���f�b�N�X���g����CPU����GPU�Ƀ�������z�u���܂�. 
		*  @note      �ÖٓI�ɓ����o�C�g���̕��т����݂��邱�Ƃ����߂��܂�
		*  @param[in] const void* : GPU�ɃA�b�v���[�h������CPU���̃������z��
		*  @param[in] const gu::uint64 : �z��̗v�f��
		*  @param[in] const gu::uint64 : ���������m�ۂ��鏉���C���f�b�N�X
		*  @param[in] const gu::SharedPointer<RHICommandList> Graphics��Copy�̃R�}���h���X�g
		*  @param[in] const bool �蓮�Ń}�b�v���s����
		*  @return    void
		*************************************************************************/
		virtual void UploadIndex(const void* data, const gu::uint64 elementCount, const gu::uint64 offsetIndex = 0, const gu::SharedPointer<RHICommandList>& commandList = nullptr, const bool useMapManually = false) = 0;

		/*!**********************************************************************
		*  @brief     �蓮��CPU����GPU�Ƀf�[�^���A�b�v���[�h���鏀���Ƃ��Ďg�p���܂�.
		*  @attention subresource�̃C���f�b�N�X�̓o�b�t�@�Ƃ��Ă̗��p�����l���Ă��Ȃ�����, 0���������܂�
		*************************************************************************/
		virtual void Map() = 0;

		/*!**********************************************************************
		*  @brief     CPU����GPU�Ƀf�[�^���A�b�v���[�h����̂��~�߂�ꍇ�Ɏg�p���܂�. 
		*  @attention 1�t���[���œ��ꃊ�\�[�X�ɉ����map, unmap���Ă΂Ȃ��悤�ɂ��Ă�������. (�������ׂ̊ϓ_��)
		*************************************************************************/
		virtual void Unmap() = 0;
		#pragma endregion 
		#pragma region Public Property
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

		/*!**********************************************************************
		*  @brief     GPUResource�̎�ނ��w�肵�܂�.(Buffer�̏ꍇ��Buffer��RaytracingAccelerationStructure���w�肷�邱�ƂɂȂ�܂�) 
		*  @param[in] void
		*  @return    ResourceType GPU���\�[�X�̎��
		*************************************************************************/
		__forceinline ResourceType  GetResourceType() const { return _metaData.ResourceType; }
		
		/*!**********************************************************************
		*  @brief     GPU���\�[�X�̏�Ԃ��`���܂�. ���\�[�X��Ԃ�ύX����ۂ�, TransitionResourceState(�R�}���h���X�g)���g�p���Ă�������.  
		*  @return    ResourceState : GPU���\�[�X�̏��
		*************************************************************************/
		ResourceState GetResourceState() const noexcept override { return _metaData.State; }
		
		/*!**********************************************************************
		*  @brief     Buffer�̎g�p���@ (Vertex, Index, or Constant Buffer)���`����Enum��Ԃ��܂� 
		*************************************************************************/
		__forceinline BufferCreateFlags GetUsage() const { return _metaData.Usage; }
		
		/*!**********************************************************************
		*  @brief   GPU�̃��������i�[����q�[�v�̈�̎�ނ����肷��Enum��Ԃ��܂� (Default, Upload, Readback, Custom)
		*************************************************************************/
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

		/*!**********************************************************************
		*  @brief     �o�b�t�@�̏��𒼐ڊi�[���Ă��郁�^�f�[�^���擾���܂�
		*  @return    GPUBufferMetaData ���^�f�[�^
		*************************************************************************/
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

		#pragma region Protected Function
		
		#pragma endregion 

		#pragma region Protected Property

		/*! @brief �o�b�t�@�̏����i�[���Ă���\����*/
		GPUBufferMetaData _metaData = {};

		#pragma endregion 

		
	};
}


#endif