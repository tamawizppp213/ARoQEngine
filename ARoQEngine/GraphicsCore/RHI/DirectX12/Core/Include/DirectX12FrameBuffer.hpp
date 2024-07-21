//////////////////////////////////////////////////////////////////////////////////
///  @file   DirectX12FrameBuffer.hpp
///  @brief  �����_�[�^�[�Q�b�g�ƃf�v�X�X�e���V���̃e�N�X�`����ResourceView���g�p����N���X�ł�.
///  @author Toide Yutaro
///  @date   2024_07_21
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_FRAME_BUFFER_HPP
#define DIRECTX12_FRAME_BUFFER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIFrameBuffer.hpp"
#include "DirectX12Core.hpp"
#include <dxgi1_6.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::directX12
{
	class RHIDescriptorHeap;
	/****************************************************************************
	*				  			RHIFrameBuffer
	****************************************************************************/
	/* @brief  �����_�[�^�[�Q�b�g�ƃf�v�X�X�e���V���̃e�N�X�`����ResourceView���g�p����N���X�ł�.
	*****************************************************************************/
	class RHIFrameBuffer : public rhi::core::RHIFrameBuffer
	{
	public:
		#pragma region Public Function
		#pragma endregion

		#pragma region Public Property

		/*!**********************************************************************
		*  @brief     Render Target View���i�[����DirectX12��DescriptorHeap��Ԃ��܂�.
		*  @param[in] void
		*  @return    DescriptorHeapComPtr Render Target View���i�[����DirectX12��DescriptorHeap
		*************************************************************************/
		DescriptorHeapComPtr GetRTVHeap() const noexcept;

		/*!**********************************************************************
		*  @brief     Depth Stencil View���i�[����DirectX12��DescriptorHeap��Ԃ��܂�.
		*  @param[in] void
		*  @return    DescriptorHeapComPtr Depth Stencil View���i�[����DirectX12��DescriptorHeap
		*************************************************************************/
		DescriptorHeapComPtr GetDSVHeap() const noexcept;

		/*!**********************************************************************
		*  @brief     Render Target View�̃o�C�g�T�C�Y��Ԃ��܂�.
		*  @param[in] void
		*  @return    gu::uint32 Render Target View�̃o�C�g�T�C�Y
		*************************************************************************/
		gu::uint32  GetRTVByteSize() const noexcept { return static_cast<gu::uint32>(_rtvByteSize); }

		/*!**********************************************************************
		*  @brief     Depth Stencil View�̃o�C�g�T�C�Y��Ԃ��܂�.
		*  @param[in] void
		*  @return    gu::uint32Depth Stencil View�̃o�C�g�T�C�Y
		*************************************************************************/
		gu::uint32  GetDSVByteSize() const noexcept { return static_cast<gu::uint32>(_dsvByteSize); }

		#pragma endregion

		#pragma region Public Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		RHIFrameBuffer() = default;

		/*! @brief �f�X�g���N�^*/
		~RHIFrameBuffer();

		/*! @brief �����_�[�^�[�Q�b�g�ƃf�v�X�X�e���V�����g����������*/
		explicit RHIFrameBuffer(const gu::SharedPointer<core::RHIDevice>& device, const gu::SharedPointer<core::RHIRenderPass>& renderPass,  const gu::SharedPointer<core::GPUTexture>& renderTarget, const gu::SharedPointer<core::GPUTexture>& depthStencil);
		
		/*! @brief �ꖇ�̃����_�[�^�[�Q�b�g�ƃf�v�X�X�e���V�����g����������*/
		explicit RHIFrameBuffer(const gu::SharedPointer<core::RHIDevice>& device, const gu::SharedPointer<core::RHIRenderPass>& renderPass, const gu::DynamicArray<gu::SharedPointer<core::GPUTexture>>&renderTargets, const gu::SharedPointer<core::GPUTexture>& depthStencil);

		#pragma endregion
	
	protected:
		#pragma region Protected Function

		#pragma endregion

		#pragma region Protected Property
		/*! @brief Render Target View���i�[����DirectX12��DescriptorHeap*/
		gu::SharedPointer<directX12::RHIDescriptorHeap> _renderTargetHeap = nullptr;

		/*! @brief Depth Stencil View���i�[����DirectX12��DescriptorHeap*/
		gu::SharedPointer<directX12::RHIDescriptorHeap> _depthStencilHeap = nullptr;

		/*! @brief Render Target View�̃o�C�g�T�C�Y*/
		gu::uint32 _rtvByteSize = 0;

		/*! @brief Depth Stencil View�̃o�C�g�T�C�Y*/
		gu::uint32 _dsvByteSize = 0;
		#pragma endregion

	private: 
		#pragma region Private Function
		/*!**********************************************************************
		*  @brief     Descriptor Heap�Ɗe�r���[���쐬���܂�. 
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void Prepare();
		#pragma endregion

	};
}
#endif