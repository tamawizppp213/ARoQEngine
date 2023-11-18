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
#include "GameUtility/Base/Include/GUSharedPointer.hpp"
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
		using GPUResourceViewPtr   = gu::SharedPointer<core::GPUResourceView>;
		using DescriptorID         = std::uint32_t;
		using ShaderResourceViewID = std::uint32_t;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		GPUResourceViewPtr Load(const std::wstring& filePath);

		bool Find(const std::wstring& filePath);

		//DescriptorID Regist(const gu::SharedPointer<core::GPUResourceView>& view); 
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUResourceCache(const gu::SharedPointer<core::RHIDevice>& device, const gu::SharedPointer<core::RHICommandList>& graphicsCommandList, 
			const gu::SharedPointer<core::RHIDescriptorHeap>& customHeap = nullptr) :
			_device(device), _commandList(graphicsCommandList), _customHeap(customHeap) { };

		virtual ~GPUResourceCache() { _resourceViews.clear(); }

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		gu::SharedPointer<core::RHIDevice>      _device      = nullptr;

		gu::SharedPointer<core::RHICommandList> _commandList = nullptr;

		gu::SharedPointer<RHIDescriptorHeap> _customHeap = nullptr;

		std::map<std::uint64_t, GPUResourceViewPtr> _resourceViews;
	};
}


#endif