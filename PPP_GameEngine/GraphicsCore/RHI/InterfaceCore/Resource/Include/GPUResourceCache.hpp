#//////////////////////////////////////////////////////////////////////////////////
///             @file   GPUResource.hpp
///             @brief  GPU Resource 
///             @author Toide Yutaro
///             @date   2022_07_08
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GPU_RESOURCE_CACHE_HPP
#define GPU_RESOURCE_CACHE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include <memory>
#include <string>
#include <map>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////

namespace rhi::core
{
	class RHIDevice;
	class RHICommandList;
	class GPUTexture;
	class GPUResourceView;
	class RHIDescriptorHeap;

	/****************************************************************************
	*				  			GPUResourceCache
	*************************************************************************//**
	*  @class     GPUResourceCache
	*  @brief     GPUResource loader cache for game application
	*****************************************************************************/
	class GPUResourceCache : public NonCopyable
	{
		using GPUResourceViewPtr   = std::shared_ptr<core::GPUResourceView>;
		using DescriptorID         = std::uint32_t;
		using ShaderResourceViewID = std::uint32_t;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		GPUResourceViewPtr Load(const std::wstring& filePath);

		bool Find(const std::wstring& filePath);

		//DescriptorID Regist(const std::shared_ptr<core::GPUResourceView>& view); 
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUResourceCache(const std::shared_ptr<core::RHIDevice>& device, const std::shared_ptr<core::RHICommandList>& graphicsCommandList, 
			const std::shared_ptr<core::RHIDescriptorHeap>& customHeap = nullptr) :
			_device(device), _commandList(graphicsCommandList), _customHeap(customHeap) { };

		~GPUResourceCache() { _resourceViews.clear(); }
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::shared_ptr<core::RHIDevice>      _device      = nullptr;
		std::shared_ptr<core::RHICommandList> _commandList = nullptr;
		std::shared_ptr<RHIDescriptorHeap> _customHeap = nullptr;
		std::map<std::uint64_t, GPUResourceViewPtr> _resourceViews;
	};
}


#endif