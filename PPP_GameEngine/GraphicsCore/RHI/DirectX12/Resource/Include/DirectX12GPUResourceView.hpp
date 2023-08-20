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
	*************************************************************************//**
	*  @class     GPUResourceView
	*  @brief     Resource view 
	*****************************************************************************/
	class GPUResourceView : public core::GPUResourceView
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Bind resource layout array index to the command list.
					index : resource layout array index*/
		void Bind(const std::shared_ptr<core::RHICommandList>& commandList, const std::uint32_t index, const std::shared_ptr<core::RHIResourceLayout>& layout = nullptr) override;
		
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandler();

		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandler();

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUResourceView() = default;

		~GPUResourceView();

		explicit GPUResourceView(const std::shared_ptr<core::RHIDevice>& device, const core::ResourceViewType type, const std::shared_ptr<core::GPUBuffer>& buffer, const std::shared_ptr<core::RHIDescriptorHeap>& customHeap = nullptr);
		
		explicit GPUResourceView(const std::shared_ptr<core::RHIDevice>& device, const core::ResourceViewType type, const std::shared_ptr<core::GPUTexture>& texture, const std::shared_ptr<core::RHIDescriptorHeap>& customHeap = nullptr);
	
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		void CreateView(const std::shared_ptr<directX12::RHIDescriptorHeap>& heap); // According to view type flag, create each view. 
		void CreateSRV(const std::shared_ptr<directX12::RHIDescriptorHeap>& heap); // shader resource view
		void CreateRAS(const std::shared_ptr<directX12::RHIDescriptorHeap>& heap); // raytracing acceleration structure
		void CreateUAV(const std::shared_ptr<directX12::RHIDescriptorHeap>& heap); // unordered access view
		void CreateRTV(const std::shared_ptr<directX12::RHIDescriptorHeap>& heap); // render target view
		void CreateDSV(const std::shared_ptr<directX12::RHIDescriptorHeap>& heap); // depth stencil view
		void CreateCBV(const std::shared_ptr<directX12::RHIDescriptorHeap>& heap); // constant buffer view

		const std::shared_ptr<directX12::RHIDescriptorHeap> SelectDescriptorHeap(const core::ResourceViewType type);
		
		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		std::pair<core::DescriptorHeapType, std::uint32_t> _heapOffset = {};
		bool _hasCreated = false;
	};
}
#endif