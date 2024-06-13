//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12GPUBuffer.hpp
///             @brief  GPU Buffer 
///             @author Toide Yutaro
///             @date   2022_07_08
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_GPU_RESOURCE_VIEW_HPP
#define DIRECTX12_GPU_RESOURCE_VIEW_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUResourceView.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Core.hpp"
#include <d3d12.h>
#include <utility>
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
	*				  			GPUResourceView
	****************************************************************************/
	/* @class     GPUResourceView
	*  @brief     Resource view 
	*****************************************************************************/
	class GPUResourceView : public core::GPUResourceView
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : Resource Layout�̔z��C���f�b�N�X���R�}���h���X�g�ƃo�C���h����
		/*----------------------------------------------------------------------*/
		void Bind(const gu::SharedPointer<core::RHICommandList>& commandList, const gu::uint32 index, [[maybe_unused]]const gu::SharedPointer<core::RHIResourceLayout>& layout = nullptr) override;
		
		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : CPU�̃|�C���^��Ԃ��n���h�����擾���܂�
		/*----------------------------------------------------------------------*/
		D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandler() const;

		/*----------------------------------------------------------------------
		*  @brief : GPU�̃|�C���^��Ԃ��n���h�����擾���܂�
		/*----------------------------------------------------------------------*/
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandler() const;

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUResourceView() = default;

		~GPUResourceView();

		explicit GPUResourceView(const gu::SharedPointer<core::RHIDevice>& device, const core::ResourceViewType type, 
			const gu::SharedPointer<core::GPUBuffer>& buffer, 
			const gu::uint32 mipSlice = 0, const gu::uint32 planeSlice = 0,
			const gu::SharedPointer<core::RHIDescriptorHeap>& customHeap = nullptr);
		
		explicit GPUResourceView(const gu::SharedPointer<core::RHIDevice>& device, const core::ResourceViewType type, const gu::SharedPointer<core::GPUTexture>& texture,
			const gu::uint32 mipSlice = 0, const gu::uint32 planeSlice = 0,
			const gu::SharedPointer<core::RHIDescriptorHeap>& customHeap = nullptr);
	
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		void CreateView(const gu::SharedPointer<directX12::RHIDescriptorHeap>& heap); // According to view type flag, create each view. 
		void CreateSRV(const gu::SharedPointer<directX12::RHIDescriptorHeap>& heap); // shader resource view
		void CreateRAS(const gu::SharedPointer<directX12::RHIDescriptorHeap>& heap); // raytracing acceleration structure
		void CreateUAV(const gu::SharedPointer<directX12::RHIDescriptorHeap>& heap); // unordered access view
		void CreateRTV(const gu::SharedPointer<directX12::RHIDescriptorHeap>& heap); // render target view
		void CreateDSV(const gu::SharedPointer<directX12::RHIDescriptorHeap>& heap); // depth stencil view
		void CreateCBV(const gu::SharedPointer<directX12::RHIDescriptorHeap>& heap); // constant buffer view

		const gu::SharedPointer<directX12::RHIDescriptorHeap> SelectDescriptorHeap(const core::ResourceViewType type);
		
		/****************************************************************************
		**                Private Property
		*****************************************************************************/
		std::pair<core::DescriptorHeapType, gu::uint32> _heapOffset = {};
		bool _hasCreated = false;
	};
}
#endif