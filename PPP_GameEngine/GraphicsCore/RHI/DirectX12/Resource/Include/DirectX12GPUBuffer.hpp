//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12GPUBuffer.hpp
///             @brief  GPU Buffer 
///             @author Toide Yutaro
///             @date   2022_07_08
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
	*  @brief     Buffer
	*****************************************************************************/
	class GPUBuffer : public core::GPUBuffer
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		// @brief : Basically for Default Buffer Initialize. Total Buffer Copy
		//          Create temp upload buffer and copy this to default buffer
		void Pack(const void* data, const std::shared_ptr<rhi::core::RHICommandList>& copyCommandList = nullptr) override;
		// @brief : Begin Map Function
		void CopyStart() override;
		// @brief : GPU copy to one element 
		void CopyData(const void* data, const size_t elementIndex) override ;
		/* @brief : GPU copy the specified range*/
		void CopyTotalData(const void* data, const size_t dataLength, const size_t indexOffset = 0) override;
		// @brief : Unmap Function
		void CopyEnd() override ;

		
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		ResourceComPtr GetResource   () const noexcept { return _resource; }
		Resource*      GetResourcePtr() const noexcept { return _resource.Get(); }
		Resource**     GetAddressOf  () { return _resource.GetAddressOf(); }

		void SetName(const std::wstring& name) override;
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUBuffer() = default;
		~GPUBuffer();
		explicit GPUBuffer(const std::shared_ptr<core::RHIDevice>& device, const core::GPUBufferMetaData& metaData);
	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		ResourceComPtr _resource = nullptr;
		ResourceComPtr _intermediateBuffer = nullptr; // for default buffer
	};
}
#endif