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
	****************************************************************************/
	/* @class     GPUBuffer
	*  @brief     �e�N�X�`���ȊO�̃o�b�t�@. ���_, �C���f�b�N�X�o�b�t�@�Ȃǂ̍쐬�Ɏg�p���܂�.
	*****************************************************************************/
	class GPUBuffer : public core::GPUBuffer
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
		virtual void UploadByte(const void* data, const gu::uint64 allocateByteSize, const gu::uint64 offsetByte, const gu::SharedPointer<core::RHICommandList>& commandList, const bool useMapManually) override;

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
		virtual void UploadIndex(const void* data, const gu::uint64 elementCount, const gu::uint64 offsetIndex, const gu::SharedPointer<core::RHICommandList>& commandList, const bool useMapManually) override;
		
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

		#pragma region Public Member Varialbles
		/*!**********************************************************************
		*  @brief     DirectX12�Ŏg�p����GPU���\�[�X
		*  @return    ResourceComptr�@ComPtr��GPU���\�[�X
		*************************************************************************/
		ResourceComPtr GetResource   () const noexcept { return _resource; }
		
		/*!**********************************************************************
		*  @brief     DirectX12�Ŏg�p����GPU���\�[�X�̐��|�C���^
		*************************************************************************/
		Resource* GetResourcePtr() const noexcept { return _resource.Get(); }
		
		/*!**********************************************************************
		*  @brief     DirectX12�Ŏg�p����GPU���\�[�X�̃A�h���X
		*************************************************************************/
		Resource** GetAddressOf  () { return _resource.GetAddressOf();}

		/*!**********************************************************************
		*  @brief     DirectX12�Ŏg�p����GPU�A�h���X
		*  @return    D3D12_GPU_VIRTUAL_ADDRESS : uint64�^�ŕ\�������GPU�A�h���X
		*************************************************************************/
		D3D12_GPU_VIRTUAL_ADDRESS GetDxGPUVirtualAddress() const { return _resource->GetGPUVirtualAddress(); }

		/*!**********************************************************************
		*  @brief     �f�o�b�O�\������ݒ肵�܂�.
		*  @param[in] const gu::tstring �f�o�b�O�\����
		*************************************************************************/
		virtual void SetName(const gu::tstring& name) override;
		
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

		#pragma endregion

		#pragma region Public Constructor and Destrcutor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		GPUBuffer() = default;
		
		/*! @brief �f�X�g���N�^*/
		~GPUBuffer();
		
		/*! @brief �f�o�C�X�ƃo�b�t�@�̍쐬�����g���Đ�������R���X�g���N�^*/
		explicit GPUBuffer(const gu::SharedPointer<core::RHIDevice>& device, const core::GPUBufferMetaData& metaData, const gu::tstring& name = SP("Buffer"));
		#pragma endregion 
	protected:
		#pragma region Protected Function
		
		#pragma endregion 

		#pragma region Protected Property
		/*! @brief DirectX12�Ŏg�p����GPU���\�[�X*/
		ResourceComPtr _resource = nullptr;

		/*! @brief GPU�ɃA�b�v���[�h����ۂɎg�p���钆�ԃo�b�t�@*/
		ResourceComPtr _intermediateBuffer = nullptr; // for default buffer

		/*! @brief Map���s���ۂɎg�p����CPU���̃o�b�t�@�̈�*/
		gu::uint8* _mappedData = nullptr;

		/*! @brief CPU����GPU��Upload�����s�����ǂ���(Map�����ɌĂ΂�Ă��邩�ǂ���)*/
		bool _useCPUMapped = false;
		#pragma endregion Protected Property
	};
}
#endif