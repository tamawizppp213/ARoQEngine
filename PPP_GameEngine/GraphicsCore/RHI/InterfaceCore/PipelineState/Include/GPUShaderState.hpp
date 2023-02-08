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
	*				  			GPUShaderState
	*************************************************************************//**
	*  @class     GPUShaderState
	*  @brief     Shader
	*****************************************************************************/
	class GPUShaderState : public GPUState
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		// @brief: Online Compile, fileName(filePath), entryPoint(Main Function Name), version (current version <= 6.6f )
#ifdef _DEBUG
		virtual void Compile   (const core::ShaderType type, const std::wstring& fileName, const std::wstring& entryPoint = L"main", const float version = 6.0f, const std::vector<std::wstring>& includeDirectories = {}, const std::vector<std::wstring>& defines = {L"_DEBUG"}) = 0;
#else
		virtual void Compile(const core::ShaderType type, const std::wstring& fileName, const std::wstring& entryPoint = L"main", const float version = 6.0f, const std::vector<std::wstring>& includeDirectories = {}, const std::vector<std::wstring>& defines = {}) = 0;
#endif
		// @brief : Offline Compile, already compiled fileName(filePath)
		virtual void LoadBinary(const core::ShaderType type, const std::wstring& fileName) = 0;
		
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		// @brief : Return Buffer pointer and Buffer byte size
		BlobData           GetBlobData      () const noexcept { return _blobData; }
		
		void*              GetBufferPointer () const noexcept { return _blobData.BufferPointer; }
		
		std::uint64_t      GetBufferByteSize() const noexcept { return _blobData.BufferSize; }
		
		ShaderType         GetShaderType    () const noexcept { return _shaderType; }
		
		float              GetShaderVersion () const noexcept { return _version; }
		
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
			const std::shared_ptr<core::RHIDevice>& device) : GPUState(device){};

		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		std::wstring GetShaderTypeName(ShaderType shaderType);

		std::wstring Format(float version);

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		ShaderType  _shaderType = ShaderType::Vertex;
		float       _version = 6.0f;
		BlobData    _blobData;

		static constexpr float NEWEST_VERSION = 6.6f;
	};

}
#endif