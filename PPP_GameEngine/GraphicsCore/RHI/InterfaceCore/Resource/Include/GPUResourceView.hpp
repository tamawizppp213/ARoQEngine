//////////////////////////////////////////////////////////////////////////////////
///             @file   GPUResource.hpp
///             @brief  GPU Resource 
///             @author Toide Yutaro
///             @date   2022_07_08
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GPU_RESOURCE_VIEW_HPP
#define GPU_RESOURCE_VIEW_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////

namespace rhi::core
{
	class RHIDevice;
	class GPUBuffer;
	class GPUTexture;
	class RHIDescriptorHeap;
	/****************************************************************************
	*				  			GPUResourceView
	*************************************************************************//**
	*  @class     GPUResourceView
	*  @brief     Resource View(å„Ç≈NoncopyableÇ…ïœçXÇ∑ÇÈ)
	*****************************************************************************/
	class GPUResourceView : public NonCopyable
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

	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUResourceView() = default;
		virtual ~GPUResourceView() = default;
		explicit GPUResourceView(const std::shared_ptr<RHIDevice>& device, const core::ResourceViewType type) : _device(device), _resourceViewType(type) {};
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::shared_ptr<RHIDevice>  _device = nullptr;
		std::shared_ptr<GPUBuffer>  _buffer = nullptr;
		std::shared_ptr<GPUTexture> _texture = nullptr;
		core::ResourceViewType      _resourceViewType = core::ResourceViewType::Unknown;
	};
}


#endif