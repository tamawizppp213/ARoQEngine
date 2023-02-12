//////////////////////////////////////////////////////////////////////////////////
///             @file   GPUSampler.hpp
///             @brief  GPU Sampler 
///             @author Toide Yutaro
///             @date   2022_07_19
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GPU_SAMPLER_HPP
#define GPU_SAMPLER_HPP

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
	/****************************************************************************
	*				  			GPUResource
	*************************************************************************//**
	*  @class     GPUResource
	*  @brief     Resource
	*****************************************************************************/
	class GPUSampler : public NonCopyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		const SamplerInfo& GetSamplerInfo  () const noexcept { return _samplerInfo; }
		
		FilterOption       GetFilterOption () const noexcept { return _samplerInfo.Filter; }
		
		SamplerAddressMode GetAddressModeU () const noexcept { return _samplerInfo.AddressModeU; }
		
		SamplerAddressMode GetAddressModeV () const noexcept { return _samplerInfo.AddressModeV; }
		
		SamplerAddressMode GetAddressModeW () const noexcept { return _samplerInfo.AddressModeW; }
		
		size_t             GetMaxAnisotropy() const noexcept { return _samplerInfo.MaxAnisotropy; }
		
		BorderColor        GetBorderColor  () const noexcept { return _samplerInfo.Border; }
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUSampler() = default;
		
		virtual ~GPUSampler() = default;
		
		explicit GPUSampler(const std::shared_ptr<RHIDevice>& device, const SamplerInfo& samplerInfo) : _device(device), _samplerInfo(samplerInfo) {};
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::shared_ptr<RHIDevice> _device = nullptr;
		SamplerInfo _samplerInfo = {};
	};
}


#endif