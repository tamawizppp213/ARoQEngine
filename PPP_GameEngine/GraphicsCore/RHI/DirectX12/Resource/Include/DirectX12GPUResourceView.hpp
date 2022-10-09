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
	/****************************************************************************
	*				  			GPUBuffer
	*************************************************************************//**
	*  @class     GPUBuffer
	*  @brief     Buffer
	*****************************************************************************/
	class GPUResourceView : public core::GPUResourceView
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUResourceView() = default;
		~GPUResourceView() = default;
		explicit GPUResourceView(const std::shared_ptr<core::RHIDevice>& device, const core::ResourceViewType type, const std::shared_ptr<core::GPUBuffer>& buffer);
		explicit GPUResourceView(const std::shared_ptr<core::RHIDevice>& device, const core::ResourceViewType type, const std::shared_ptr<core::GPUTexture>& texture);
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		void CreateView(); // According to view type flag, create each view. 
		void CreateSRV(); // shader resource view
		void CreateRAS(); // raytracing acceleration structure
		void CreateUAV(); // unordered access view
		void CreateRTV(); // render target view
		void CreateDSV(); // depth stencil view
		void CreateCBV(); // constant buffer view
		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		
	};
}
#endif