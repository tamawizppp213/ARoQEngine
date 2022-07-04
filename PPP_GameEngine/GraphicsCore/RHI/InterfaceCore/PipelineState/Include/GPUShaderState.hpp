//////////////////////////////////////////////////////////////////////////////////
///             @file   GPUBlendState.hpp
///             @brief  Blend State
///             @author Toide Yutaro
///             @date   2022_06_28
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GPU_SHADER_STATE_HPP
#define GPU_SHADER_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GPUState.hpp"
#include <vector>

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
	*				  			RHIPipelineState
	*************************************************************************//**
	*  @class     RHIPipelineState
	*  @brief     PipelineState
	*****************************************************************************/
	class GPUShaderState : public GPUState
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		BlobData           GetBlobData     () const noexcept { return _blobData; }
		std::string        GetFileName     () const noexcept { return _fileName; }
		std::string        GetEntryPoint   () const noexcept { return _entryPoint; }
		std::string        GetShaderVersion() const noexcept { return _shaderVersion; }
		ShaderType         GetShaderType   () const noexcept { return _shaderType; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		
	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUShaderState() = default;
		~GPUShaderState() = default;
		explicit GPUShaderState(
			const std::shared_ptr<core::RHIDevice>& device,
			const ShaderType shaderType,
			const std::string& fileName,
			const std::string& entryPoint    = "main",
			const std::string& shaderVersion = "6.6"
		) : GPUState(device),  _fileName(fileName), _entryPoint(entryPoint), _shaderType(shaderType), _shaderVersion(shaderVersion)
		{
		};
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		std::string GetShaderTypeName(ShaderType shaderType);
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		BlobData    _blobData;
		std::string _fileName      = "";
		std::string _entryPoint    = "main";
		std::string _shaderVersion = "6.0";
		ShaderType  _shaderType = ShaderType::Vertex;
	};

}
#endif