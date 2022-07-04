//////////////////////////////////////////////////////////////////////////////////
//              @file   VulkanShaderState.cpp
///             @brief  Rasterizer State
///             @author Toide Yutaro
///             @date   2022_07_04
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/Vulkan/PipelineState/Include/VulkanGPUShaderState.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanEnumConverter.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanDevice.hpp"
#include "GameUtility/File/Include/UnicodeUtility.hpp"
#include <wrl/client.h>
#include <dxcapi.h>
#include <sstream>
#include <fstream>
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::vulkan;
using Microsoft::WRL::ComPtr;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
GPUShaderState::GPUShaderState(
	const std::shared_ptr<core::RHIDevice>& device,
	const core::ShaderType shaderType,
	const std::string& fileName,
	const std::string& entryPoint,
	const std::string& shaderVersion
) : core::GPUShaderState(device, shaderType, fileName, entryPoint, shaderVersion)
{
	const auto vkDevice = std::static_pointer_cast<rhi::vulkan::RHIDevice>(_device)->GetDevice();
	CompileShader(unicode::ToWString(fileName), unicode::ToWString(entryPoint), unicode::ToWString(GetShaderTypeName(shaderType) + "_" + shaderVersion));
	/*-------------------------------------------------------------------
	-                  Create shader module
	---------------------------------------------------------------------*/
	_stage.pNext               = nullptr;
	_stage.flags               = 0;
	_stage.module              = _module;
	_stage.pName               = _fileName.c_str();
	_stage.pSpecializationInfo = nullptr;
	_stage.stage               = EnumConverter::Convert(_shaderType);
}
/****************************************************************************
*                       Compile Shader
*************************************************************************//**
*  @fn        BlobComPtr CompileShader( const std::wstring& fileName, const std::wstring& entryPoint, const std::wstring& target)
*  @brief     Compile shader
*  @param[in] test
*  @return Å@Å@void
*****************************************************************************/
void GPUShaderState::CompileShader(const std::wstring& fileName, const std::wstring& entryPoint, const std::wstring& target)
{
	/*-------------------------------------------------------------------
	-                  Open shader file
	---------------------------------------------------------------------*/
	std::ifstream shaderFile(fileName, std::ios::binary);
	if (shaderFile.good() == false)
	{
		std::wstring errorMessage = L"Failed to open your shader file. " + fileName;
		MessageBoxW(nullptr, errorMessage.c_str(), L"Error", MB_OK);
		std::abort();
	}

	std::stringstream stringStream(std::ios::binary);
	stringStream << shaderFile.rdbuf();
	std::string shader = stringStream.str();

	/*-------------------------------------------------------------------
	-                  Create blob data from shader text file.
	---------------------------------------------------------------------*/
	ComPtr<IDxcLibrary> dxcLibrary = nullptr;
	if (FAILED(DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&dxcLibrary)))) { throw std::runtime_error("Could not init DXC library"); };

	ComPtr<IDxcIncludeHandler> dxcIncludeHandler = nullptr;
	if (FAILED(dxcLibrary->CreateIncludeHandler(&dxcIncludeHandler))) { throw std::runtime_error("Could not init Include handler"); };
	/*-------------------------------------------------------------------
	-                  Create dxc compliler
	---------------------------------------------------------------------*/
	ComPtr<IDxcCompiler> dxcCompiler = nullptr;
	if (FAILED(DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler)))) { throw std::runtime_error("Could not init DXC Compiler"); };
	/*-------------------------------------------------------------------
	-                  Create Blob data of the source code
	---------------------------------------------------------------------*/
	ComPtr<IDxcBlobEncoding> sourceBlob = nullptr;
	UINT32 codePage = CP_UTF8;
	if (FAILED(dxcLibrary->CreateBlobFromFile(fileName.c_str(), &codePage, sourceBlob.GetAddressOf()))) { throw std::runtime_error("Could not load shader file."); };
	/*-------------------------------------------------------------------
	-       Tell the compiler to be passed to the shader compiler
	---------------------------------------------------------------------*/
	std::vector<LPCWSTR> arguments;
	arguments.push_back(L"-spirv");
	// nVidia: This allows for the compiler to do a better job at optimizing texture accesses. We have seen frame rate improvements of > 1% when toggling this flag on.
	arguments.push_back(L"-all-resources-bound");
	// VK_KHR_shader_float16_int8
	arguments.push_back(L"-enable-16bit-types");
	// memory layout for resources
	arguments.push_back(L"-fvk-use-dx-layout");
	// Vulkan version
	std::wstring name = L"-fspv-target-env=" + _apiVersion;
	LPCWSTR   apiName = name.c_str();
	arguments.push_back(apiName);
	// useful extensions
	arguments.push_back(L"-fspv-extension=SPV_GOOGLE_hlsl_functionality1");
	arguments.push_back(L"-fspv-extension=SPV_GOOGLE_user_type");
	arguments.push_back(L"-fspv-reflect");

	/*-------------------------------------------------------------------
	-                  Create Blob data of the source code
	---------------------------------------------------------------------*/
	ComPtr<IDxcOperationResult> result = nullptr;
	HRESULT hresult = dxcCompiler->Compile(
		sourceBlob.Get(),
		fileName.c_str(),
		entryPoint.c_str(),
		target.c_str(),
		arguments.data(), arguments.size(),
		nullptr, 0,
		dxcIncludeHandler.Get(),
		&result
	);

	if (SUCCEEDED(hresult)) { result->GetStatus(&hresult); }
	core::BlobData blob;
	ComPtr<IDxcBlob> code = nullptr;

	if (FAILED(hresult))
	{
		if (result)
		{
			ComPtr <IDxcBlobEncoding> errorBlob;
			hresult = result->GetErrorBuffer(&errorBlob);
			if (SUCCEEDED(hresult) && errorBlob)
			{
				std::string errorMessage = "Compilation failed with errors: \n%hs\n";
				errorMessage += (const char*)errorBlob->GetBufferPointer();
				MessageBoxA(nullptr, errorMessage.c_str(), "Error", MB_OK);
			}
		}
	}
	else
	{
		result->GetResult(reinterpret_cast<IDxcBlob**>(code.GetAddressOf()));
	}


	const auto vkDevice = std::static_pointer_cast<rhi::vulkan::RHIDevice>(_device);

	/*-------------------------------------------------------------------
	-                  Create shader module
	---------------------------------------------------------------------*/
	VkShaderModuleCreateInfo shaderModuleCI{};
	shaderModuleCI.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModuleCI.codeSize = code->GetBufferSize();
	shaderModuleCI.pCode    = (uint32_t*)code->GetBufferPointer();
	shaderModuleCI.pNext    = nullptr;
	vkCreateShaderModule(vkDevice->GetDevice(), &shaderModuleCI, nullptr, &_module);

	_blobData.BufferPointer = code->GetBufferPointer();
	_blobData.BufferSize    = code->GetBufferSize();
	
}