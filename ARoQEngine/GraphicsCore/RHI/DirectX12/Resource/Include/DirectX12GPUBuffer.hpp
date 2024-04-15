//////////////////////////////////////////////////////////////////////////////////
///  @file   DirectX12GPUBuffer.hpp
///  @brief  �e�N�X�`���ȊO�̃o�b�t�@. ���_, �C���f�b�N�X�o�b�t�@�Ȃǂ̍쐬�Ɏg�p���܂�. 
///  @author Toide Yutaro
///  @date   2022_07_08
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_GPU_BUFFER_HPP
#define DIRECTX12_GPU_BUFFER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUBuffer.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Core.hpp"
#include <d3d12.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::directX12
{
	/****************************************************************************
	*				  			GPUBuffer
	*************************************************************************//**
	*  @class     GPUBuffer
	*  @brief     �e�N�X�`���ȊO�̃o�b�t�@. ���_, �C���f�b�N�X�o�b�t�@�Ȃǂ̍쐬�Ɏg�p���܂�.
	*****************************************************************************/
	class GPUBuffer : public core::GPUBuffer
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
		virtual void Upload(const void* data, const gu::uint64 allocateByteSize, const gu::uint64 offsetByte, const gu::SharedPointer<core::RHICommandList>& commandList) override;

		/*----------------------------------------------------------------------
		*  @brief :  Call at once in each frame (If you need). CopyStart + CopyTotalData + CopyEnd.
		/*----------------------------------------------------------------------*/
		virtual void Update(const void* data, const gu::uint64 dataLength) override
		{
			CopyStart();
			CopyTotalData(data, dataLength);
		}

		/*----------------------------------------------------------------------
		*  @brief : �@Map�֐����Ăяo��
		/*----------------------------------------------------------------------*/
		void CopyStart() override;

		/*----------------------------------------------------------------------
		*  @brief :  �w�肳�ꂽ�C���f�b�N�X��offset�ɂ���������, �w��̔z�񐔕������R�s�[����
		/*----------------------------------------------------------------------*/
		void CopyTotalData(const void* data, const gu::uint64 dataLength, const gu::uint64 indexOffset = 0) override;
		
		/*----------------------------------------------------------------------
		*  @brief :  Unmap���Ăяo���Ƃ��Ɏg�p���܂�
		/*----------------------------------------------------------------------*/
		void CopyEnd() override ;

		
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		ResourceComPtr GetResource   () const noexcept { return _resource; }
		
		Resource*      GetResourcePtr() const noexcept { return _resource.Get(); }
		
		Resource**     GetAddressOf  () { return _resource.GetAddressOf();}

		D3D12_GPU_VIRTUAL_ADDRESS GetDxGPUVirtualAddress() const { return _resource->GetGPUVirtualAddress(); }

		void SetName(const gu::tstring& name) override;
		
		/*!**********************************************************************
		*  @brief     Map�ɓo�^�����CPU�������̍ŏ��̃|�C���^��Ԃ��܂�
		*  @return    gu::uint8* �}�b�v�����擪�|�C���^
		*************************************************************************/
		virtual gu::uint8* GetCPUMappedAddress() override { return _mappedData; };

		/*!**********************************************************************
		*  @brief     GPU�̃A�h���X��gu::uint64�Ƃ����`�ŉ��z�I�ɕ\���������̂�Ԃ��܂�.
		*  @return    gu::uint64 GPU�̃A�h���X�l
		*************************************************************************/
		virtual gu::uint64 GetGPUVirtualAddress() override { return _resource->GetGPUVirtualAddress(); }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUBuffer() = default;
		
		~GPUBuffer();
		
		explicit GPUBuffer(const gu::SharedPointer<core::RHIDevice>& device, const core::GPUBufferMetaData& metaData, const gu::tstring& name = SP("Buffer"));
	protected:
		#pragma region Protected Function
		#pragma endregion 

		#pragma region Protected Member Variables
		/*! @brief DirectX12�Ŏg�p����GPU���\�[�X*/
		ResourceComPtr _resource = nullptr;

		/*! @brief GPU�ɃA�b�v���[�h����ۂɎg�p���钆�ԃo�b�t�@*/
		ResourceComPtr _intermediateBuffer = nullptr; // for default buffer

		/*! @brief Map���s���ۂɎg�p����CPU���̃o�b�t�@�̈�*/
		gu::uint8* _mappedData = nullptr;

		/*! @brief CPU����GPU��Upload�����s�����ǂ���(Map�����ɌĂ΂�Ă��邩�ǂ���)*/
		bool _useCPUMapped = false;
		#pragma endregion Protected Member Variables
	};
}
#endif