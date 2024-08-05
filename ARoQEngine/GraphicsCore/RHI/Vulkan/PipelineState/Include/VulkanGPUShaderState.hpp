//////////////////////////////////////////////////////////////////////////////////
///             @file   VulkanGPUShaderState.hpp
///             @brief  VulkanGPUShaderState.hpp
///             @author Toide Yutaro
///             @date   2022_07_02
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VULKAN_GPU_SHADER_STATE_HPP
#define VULKAN_GPU_SHADER_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUShaderState.hpp"
#include <vulkan/vulkan.h>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::vulkan
{

	/****************************************************************************
	*				  			GPUBlendState
	****************************************************************************/
	/* @class     GPUBlendState
	*  @brief     BlendState
	*****************************************************************************/
	class GPUShaderState : public rhi::core::GPUShaderState
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		// @brief: Online Compile, fileName(filePath), entryPoint(Main Function Name), version (current version <= 6.6f )
		void Compile(const core::ShaderCompilerOption& option) override;
		
		// @brief : Offline Compile, already compiled fileName(filePath)
		void LoadBinary(const core::ShaderType type, const gu::tstring& fileName) override;
		
		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		VkShaderModule GetModule() const noexcept { return _module; }
		
		const VkPipelineShaderStageCreateInfo& GetStage() const{ return _stage; }
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUShaderState() = default;
		
		~GPUShaderState();
		
		explicit GPUShaderState(
			const gu::SharedPointer<core::RHIDevice>& device) : core::GPUShaderState(device){};
	
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		
		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		VkPipelineShaderStageCreateInfo _stage = {};

		VkShaderModule _module     = nullptr;

		gu::tstring   _apiVersion = SP("vulkan1.3");

		gu::string   _name = "";

	private:
		void VkCompile(const gu::tstring& fileName, const gu::tstring& entryPoint, const gu::tstring& target, const gu::DynamicArray<gu::tstring>& includeDirectories);
	};
}
#endif