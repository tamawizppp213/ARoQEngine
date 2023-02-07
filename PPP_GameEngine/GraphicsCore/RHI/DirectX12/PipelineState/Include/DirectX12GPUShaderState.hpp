//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12GPUShaderState.hpp
///             @brief  DirectX12GPUShaderState.hpp
///             @author Toide Yutaro
///             @date   2022_07_02
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_GPU_SHADER_STATE_HPP
#define DIRECTX12_GPU_SHADER_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUShaderState.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Core.hpp"
#include <d3d12.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
struct _D3D_SHADER_MACRO;
//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::directX12
{

	/****************************************************************************
	*				  			GPUShaderState
	*************************************************************************//**
	*  @class     GPUShaderState
	*  @brief     Shader State
	*****************************************************************************/
	class GPUShaderState : public rhi::core::GPUShaderState
	{
		using D3D_SHADER_MACRO = _D3D_SHADER_MACRO;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		// @brief: Online Compile, fileName(filePath), entryPoint(Main Function Name), version (current version <= 6.6f )
		void Compile(const core::ShaderType type, const std::wstring& fileName, const std::wstring& entryPoint = L"main", const float version = 6.0f, const std::vector<std::wstring>& includeDirectories = {}) override;
		// @brief : Offline Compile, already compiled fileName(filePath)
		void LoadBinary(const core::ShaderType type, const std::wstring& fileName) override ;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		BlobComPtr GetDxBlob() const noexcept { return _dxBlob; }

		D3D12_SHADER_BYTECODE GetShader() const { return D3D12_SHADER_BYTECODE(_blobData.BufferPointer, _blobData.BufferSize); }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUShaderState() = default;
		~GPUShaderState() = default;
		explicit GPUShaderState(
			const std::shared_ptr<core::RHIDevice>& device) : rhi::core::GPUShaderState(device) {};
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		BlobComPtr DxCompile(const std::wstring& fileName, const std::wstring& entryPoint, const std::wstring& target, const std::vector<std::wstring>& includeDirectories);
		BlobComPtr DxCompile(const std::wstring& fileName, const D3D_SHADER_MACRO* defines, const std::wstring& entryPoint, const std::wstring& target);
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		BlobComPtr _dxBlob = nullptr;
	};
}
#endif