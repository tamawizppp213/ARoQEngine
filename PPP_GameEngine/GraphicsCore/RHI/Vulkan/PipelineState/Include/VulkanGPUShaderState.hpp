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
	*************************************************************************//**
	*  @class     GPUBlendState
	*  @brief     BlendState
	*****************************************************************************/
	class GPUShaderState : public rhi::core::GPUShaderState
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		VkShaderModule GetModule() const noexcept { return _module; }
		const VkPipelineShaderStageCreateInfo& GetStage() const{ return _stage; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUShaderState() = default;
		~GPUShaderState() = default;
		explicit GPUShaderState(
			const std::shared_ptr<core::RHIDevice>& device,
			const core::ShaderType shaderType,
			const std::string& fileName,
			const std::string& entryPoint    = "main",
			const std::string& shaderVersion = "6.6"
		);
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		VkPipelineShaderStageCreateInfo _stage = {};
		VkShaderModule _module     = nullptr;
		std::wstring   _apiVersion = L"vulkan1.3";

	private:
		void CompileShader(const std::wstring& fileName, const std::wstring& entryPoint, const std::wstring& target);
	};
}
#endif