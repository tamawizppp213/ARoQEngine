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
#include "GameUtility/Base/Include/GUMemory.hpp"
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
		
		/* @brief : Return filter option which specifies sampling method for image enlargement/shirinkage*/
		FilterOption       GetFilterOption () const noexcept { return _samplerInfo.Filter; }
		
		/* @brief : Return texture addressing mode in the U direction*/
		SamplerAddressMode GetAddressModeU () const noexcept { return _samplerInfo.AddressModeU; }
		
		/* @brief : Return texture addressing mode in the V direction.*/
		SamplerAddressMode GetAddressModeV () const noexcept { return _samplerInfo.AddressModeV; }
		
		/* @brief : Return texture addressing mode in the W direction.*/
		SamplerAddressMode GetAddressModeW () const noexcept { return _samplerInfo.AddressModeW; }
		
		/* @brief : Return max anisotropy*/
		size_t             GetMaxAnisotropy() const noexcept { return _samplerInfo.MaxAnisotropy; }
		
		/* @brief : Return border color*/
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
		
		explicit GPUSampler(const gu::SharedPointer<RHIDevice>& device, const SamplerInfo& samplerInfo) : _device(device), _samplerInfo(samplerInfo) {};
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		gu::SharedPointer<RHIDevice> _device = nullptr;
		SamplerInfo _samplerInfo = {};
	};
}


#endif