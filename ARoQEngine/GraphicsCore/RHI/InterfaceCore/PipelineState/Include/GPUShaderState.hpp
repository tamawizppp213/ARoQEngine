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
#include "GameUtility/Container/Include/GUDynamicArray.hpp"

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
	****************************************************************************/
	/* @class     GPUShaderState
	*  @brief     Shader
	*****************************************************************************/
	class GPUShaderState : public GPUState
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		// @brief: Online Compile, fileName(filePath), entryPoint(Main Function Name), version (current version <= 6.6f )
		virtual void Compile(const core::ShaderType type, const gu::tstring& fileName, const gu::tstring& entryPoint = SP("main"), const float version = NEWEST_VERSION, const gu::DynamicArray<gu::tstring>& includeDirectories = {}, const gu::DynamicArray<gu::tstring>& defines = {}) = 0;

		// @brief : Offline Compile, already compiled fileName(filePath)
		virtual void LoadBinary(const core::ShaderType type, const gu::tstring& fileName) = 0;
		
		/****************************************************************************
		**                Public Property
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
			const gu::SharedPointer<core::RHIDevice>& device) : GPUState(device){};

		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		gu::tstring GetShaderTypeName(ShaderType shaderType);

		gu::tstring Format(float version);

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		ShaderType  _shaderType = ShaderType::Vertex;

		float       _version = 6.0f;

		BlobData    _blobData;

		static constexpr float NEWEST_VERSION = 6.6f;
	};

}
#endif