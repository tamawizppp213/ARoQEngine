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
		// @brief: Online Compile, fileName(filePath), entryPoint(Main Function Name), version (current version <= 6.6f )
		void Compile(const core::ShaderType type, const std::wstring& fileName, const std::wstring& entryPoint = L"main", const float version = 6.0f, const std::vector<std::wstring>& includeDirectories = {}, const std::vector<std::wstring>& defines = {}) override;
		
		// @brief : Offline Compile, already compiled fileName(filePath)
		void LoadBinary(const core::ShaderType type, const std::wstring& fileName) override;
		
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		VkShaderModule GetModule() const noexcept { return _module; }
		
		const VkPipelineShaderStageCreateInfo& GetStage() const{ return _stage; }
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUShaderState() = default;
		
		~GPUShaderState();
		
		explicit GPUShaderState(
			const std::shared_ptr<core::RHIDevice>& device) : core::GPUShaderState(device){};
	
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

		std::string   _name = "";

	private:
		void VkCompile(const std::wstring& fileName, const std::wstring& entryPoint, const std::wstring& target, const std::vector<std::wstring>& includeDirectories);
	};
}
#endif