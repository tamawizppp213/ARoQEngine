//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12RenderPass.hpp
///             @brief  Render pass
///             @author Toide Yutaro
///             @date   2022_08_09
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VULKAN_RESOURCE_LAYOUT_HPP
#define VULKAN_RESOURCE_LAYPUT_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIResourceLayout.hpp"
#include <vulkan/vulkan.h>
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::vulkan
{
	/****************************************************************************
	*				  			RHIResourceLayout
	*************************************************************************//**
	*  @class     RHIResourceLayout
	*  @brief     GPU resource root signature. shader binding setting.
	*****************************************************************************/
	class RHIResourceLayout : public  rhi::core::RHIResourceLayout
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		VkPipelineLayout GetLayout() const noexcept { return _pipelineLayout; }
		
		gu::DynamicArray<VkDescriptorSetLayout>& GetDescriptorSetLayouts() { return _descriptorSetLayouts; }
		
		void SetName(const gu::tstring& name) override;
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIResourceLayout() = default;
		
		~RHIResourceLayout();
		
		explicit RHIResourceLayout(const gu::SharedPointer<core::RHIDevice>& device, const gu::DynamicArray<core::ResourceLayoutElement>& elements = {}, const gu::DynamicArray<core::SamplerLayoutElement>& samplers = {}, const std::optional<core::Constant32Bits>& constants = std::nullopt, const gu::tstring& name = SP("ResourceLayout"));
		
		explicit RHIResourceLayout(const gu::SharedPointer<core::RHIDevice>& device, const core::ResourceLayoutElement& layout, const core::SamplerLayoutElement& sampler, const std::optional<core::Constant32Bits>& constant = std::nullopt, const gu::tstring& name = SP("ResourceLayout"));
	
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		VkPipelineLayout _pipelineLayout = nullptr;

		gu::DynamicArray<VkDescriptorSetLayout> _descriptorSetLayouts = {};
	
	private:
		void SetUp();
	};
}
#endif