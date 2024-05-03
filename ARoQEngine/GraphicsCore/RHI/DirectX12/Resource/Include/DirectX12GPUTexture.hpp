//////////////////////////////////////////////////////////////////////////////////
///  @file   DirectX12GPUTexture.hpp
///  @brief  �e�N�X�`���Ƃ��Ďg�p����ꍇ��GPU���\�[�X�ł�.
///  @author Toide Yutaro
///  @date   2024_04_21
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_GPU_TEXTURE_HPP
#define DIRECTX12_GPU_TEXTURE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUTexture.hpp"
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
	*				  			GPUTexture
	*************************************************************************//**
	/*  @brief     �e�N�X�`���Ƃ��Ďg�p����ꍇ��GPU���\�[�X�ł�.
	*****************************************************************************/
	class GPUTexture: public core::GPUTexture
	{
	public:
		#pragma region Public Function
		void Load(const gu::tstring& filePath, const gu::SharedPointer<core::RHICommandList>& commandList) override;
		
		void Save(const gu::tstring& filePath, const gu::SharedPointer<core::RHICommandList>& commandList, const gu::SharedPointer<core::RHICommandQueue>& commandQueue)override;

		void Write(const gu::SharedPointer<core::RHICommandList>& commandList, const gm::RGBA* pixel) override;

		#pragma endregion

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     �o�b�t�@�Ƃ��Ă�GPU���\�[�X���ǂ����𔻒肵�܂�.
		*************************************************************************/
		__forceinline virtual bool IsBuffer() const override { return false; }

		/*!**********************************************************************
		*  @brief     �e�N�X�`���Ƃ��Ă�GPU���\�[�X���ǂ����𔻒肵�܂�.
		*************************************************************************/
		__forceinline virtual bool IsTexture() const override { return true; }

		/*!**********************************************************************
		*  @brief     DirectX12�Ŏg�p����GPU���\�[�X
		*  @return    ResourceComptr�@ComPtr��GPU���\�[�X
		*************************************************************************/
		ResourceComPtr GetResource () const noexcept { return _resource; }
		
		/*!**********************************************************************
		*  @brief     DirectX12�Ŏg�p����GPU���\�[�X�̐��|�C���^
		*************************************************************************/
		Resource* GetResourcePtr() const noexcept { return _resource.Get(); }

		/*!**********************************************************************
		*  @brief     DirectX12�Ŏg�p����GPU�A�h���X
		*  @return    D3D12_GPU_VIRTUAL_ADDRESS : uint64�^�ŕ\�������GPU�A�h���X
		*************************************************************************/
		D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() { return _resource->GetGPUVirtualAddress(); }
		
		/*!**********************************************************************
		*  @brief     �f�o�b�O�\������ݒ肵�܂�.
		*  @param[in] const gu::tstring �f�o�b�O�\����
		*************************************************************************/
		void SetName(const gu::tstring& name) override;
		
		/*!**********************************************************************
		*  @brief     �����_��GPUResource�̈����� (IndexBuffer�Ƃ��Ďg�p����Ȃ�...)��ݒ肵�܂�
		*  @attention �蓮�ł̐؂�ւ��͊�{�I�ɍs��Ȃ��ł�������. (���̊֐��̓o���A�̎g�p��ړI�Ƃ��Ďg�p���܂�.)
		*  @return    void
		*************************************************************************/
		__forceinline virtual void SetResourceState(const core::ResourceState state) override { _metaData.State = state; }

		#pragma endregion

		#pragma region Public Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		GPUTexture() = default;

		/*! @brief �f�X�g���N�^*/
		~GPUTexture();

		/*! @brief �_���f�o�C�X���g���ăf�t�H���g��ԂŃe�N�X�`�����쐬���܂�*/
		explicit GPUTexture(const gu::SharedPointer<core::RHIDevice>& device, const gu::tstring& name = SP(""));

		/*! @brief �_���f�o�C�X�ƃe�N�X�`���쐬�����g�����R���X�g���N�^*/
		explicit GPUTexture(const gu::SharedPointer<core::RHIDevice>& device, const core::GPUTextureMetaData& metaData, const gu::tstring& name = SP(""));

		/*! @brief DirectX12��p�̃R���X�g���N�^�ł�*/
		explicit GPUTexture(const gu::SharedPointer<core::RHIDevice>& device, const ResourceComPtr& texture, const core::GPUTextureMetaData& metaData, const gu::tstring& name = SP(""));
		#pragma endregion
	
	protected:
		#pragma region Protected Constructor and Destructor
		#pragma endregion 

		#pragma region Protected Function
		
		#pragma endregion

		#pragma region Protected Property
		/*! @brief DirectX12�Ŏg�p����GPU���\�[�X*/
		ResourceComPtr _resource = nullptr;

		/*! @brief GPU�ɃA�b�v���[�h����ۂɎg�p���钆�ԃo�b�t�@*/
		ResourceComPtr _stagingBuffer = nullptr; // �K�v�Ȃ��Ȃ����^�C�~���O�Ŏ̂Ă���
		
		/*! @brief DirectX12�̃��\�[�X��Ԃ��i�[���Ă��܂�.*/
		D3D12_RESOURCE_STATES _usageState = D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON;
		
		bool _hasAllocated = false;
		#pragma endregion

	private:
		#pragma region Private Function

		void AllocateGPUTextureBuffer(const D3D12_RESOURCE_DESC& resourceDesc, bool isDiscreteGPU );

		void ConvertDxMetaData       (D3D12_RESOURCE_DESC& resourceDesc);

		#pragma endregion
		
	};
}
#endif