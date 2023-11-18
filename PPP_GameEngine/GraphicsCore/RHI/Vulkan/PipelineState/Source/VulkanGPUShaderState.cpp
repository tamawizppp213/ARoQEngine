//////////////////////////////////////////////////////////////////////////////////
//              @file   VulkanShaderState.cpp
///             @brief  Rasterizer State
///             @author Toide Yutaro https://github.com/KhronosGroup/Vulkan-Guide/blob/master/chapters/hlsl.adoc
///             @date   2022_07_04
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/Vulkan/PipelineState/Include/VulkanGPUShaderState.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanEnumConverter.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanDevice.hpp"
#include "GameUtility/File/Include/UnicodeUtility.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Core.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Debug.hpp"
#include <d3dcompiler.h>
#include <dxc/dxcapi.h>
#include <sstream>
#include <fstream>
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::vulkan;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
GPUShaderState::~GPUShaderState()
{
	const auto vkDevice = gu::StaticPointerCast<vulkan::RHIDevice>(_device)->GetDevice();
	if (_module)
	{
		vkDestroyShaderModule(vkDevice, _module, nullptr);
	}
}

void GPUShaderState::Compile(const core::ShaderType type, const std::wstring& fileName, const std::wstring& entryPoint, const float version, const std::vector<std::wstring>& includeDirectories, const std::vector<std::wstring>& defines)
{
#if __DEBUG
	assert(0.0f < version && version <= NEWEST_VERSION);
#endif
	_shaderType = type; _version = version;

	// Set target Name ex) vs_6.0, ps_6.1...
	std::wstring target = GetShaderTypeName(type) + L"_" + Format(_version);

	VkCompile(fileName, entryPoint, target, includeDirectories);

}
/****************************************************************************
*                       Compile Shader
*************************************************************************//**
*  @fn        BlobComPtr CompileShader( const std::wstring& fileName, const std::wstring& entryPoint, const std::wstring& target)
*  @brief     Compile shader
*  @param[in] test
*  @return 　　void
*****************************************************************************/
void GPUShaderState::VkCompile(const std::wstring& fileName, const std::wstring& entryPoint, const std::wstring& target, const std::vector<std::wstring>& includeDirectories)
{
	/*-------------------------------------------------------------------
	-            Create blob data from shader text file.
	-            Initialize DXC library
	---------------------------------------------------------------------*/
	ComPtr<IDxcLibrary> dxcLibrary = nullptr;
	ThrowIfFailed(DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&dxcLibrary)));

	/*-------------------------------------------------------------------
	-                  Create dxc compliler
	---------------------------------------------------------------------*/
	ComPtr<IDxcCompiler3> dxcCompiler = nullptr;
	ThrowIfFailed(DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler)));
	
	/*-------------------------------------------------------------------
	-                  Create dxc utility
	---------------------------------------------------------------------*/
	ComPtr<IDxcUtils> utils = nullptr;
	ThrowIfFailed(DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&utils)));
	
	/*-------------------------------------------------------------------
	-                  Create Blob data of the source code
	---------------------------------------------------------------------*/
	ComPtr<IDxcBlobEncoding> sourceBlob = nullptr;
	UINT32 codePage = DXC_CP_ACP;
	ThrowIfFailed(utils->LoadFile(fileName.c_str(), &codePage, &sourceBlob));

	ComPtr<IDxcIncludeHandler> dxcIncludeHandler = nullptr;
	dxcLibrary->CreateIncludeHandler(&dxcIncludeHandler);
	/*-------------------------------------------------------------------
	-                    Set Source Code Buffer
	---------------------------------------------------------------------*/
	DxcBuffer buffer = {};
	buffer.Encoding = DXC_CP_ACP;
	buffer.Ptr      = sourceBlob->GetBufferPointer();
	buffer.Size     = sourceBlob->GetBufferSize();

	/*-------------------------------------------------------------------
	-      Configure the compiler arguments for compiling the HLSL shader to SPIR-V
	---------------------------------------------------------------------*/
	std::vector<LPCWSTR> arguments = {
		fileName.c_str(),          // (optional) name of the shader file to be displayed e.g. in an error message
		L"-E", entryPoint.c_str(), // shader main entry point
		L"-T", target.c_str(),     // shader target profile,
		std::wstring(L"-fspv-target-env=" + _apiVersion).c_str(),// Vulkan version
		L"-spirv",                  // compile to SPIRV,
		L"-all-resources-bound",   // nVidia: This allows for the compiler to do a better job at optimizing texture accesses. We have seen frame rate improvements of > 1% when toggling this flag on.
		L"-enable-16bit-types",    // VK_KHR_shader_float16_int8
		L"-fvk-use-dx-layout",     // memory layout for resources
		L"-D", L"VULKAN"
		// useful extensions
		//L"-fspv-extension=SPV_GOOGLE_hlsl_functionality1",
		//L"-fspv-extension=SPV_GOOGLE_user_type",
		//L"-fspv-reflect"
	};

	for (const auto& directory : includeDirectories)
	{
		arguments.push_back(L"-I");
		arguments.push_back(directory.c_str());
	}

#ifdef _DEBUG
	arguments.push_back(L"-D");
	arguments.push_back(L"_DEBUG");
#endif

	/*-------------------------------------------------------------------
	-                Compile Shader
	---------------------------------------------------------------------*/
	ComPtr<IDxcResult> result = nullptr;
	HRESULT hresult = TRUE;
	dxcCompiler->Compile(
		&buffer,
		arguments.data(),
		static_cast<std::uint32_t>(arguments.size()),
		dxcIncludeHandler.Get(), IID_PPV_ARGS(result.GetAddressOf())
	);

	/*-------------------------------------------------------------------
	-              Output error if compilation failed
	---------------------------------------------------------------------*/
	if (SUCCEEDED(hresult)) { result->GetStatus(&hresult); }

	ComPtr<IDxcBlob> byteCode = nullptr;
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
		result->GetResult(byteCode.GetAddressOf());
	}

	/*-------------------------------------------------------------------
	-      Create a Vulkan shader module from the compilation result 
	---------------------------------------------------------------------*/
	const auto vkDevice = gu::StaticPointerCast<vulkan::RHIDevice>(_device)->GetDevice();
	const VkShaderModuleCreateInfo shaderModuleInfo = 
	{
		.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.pNext    = nullptr,
		.flags    = 0,
		.codeSize = byteCode->GetBufferSize(),
		.pCode    = (std::uint32_t*)byteCode->GetBufferPointer()
	};

	if (vkCreateShaderModule(vkDevice, &shaderModuleInfo, nullptr, &_module) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create vk shader module");
	}
	
	/*-------------------------------------------------------------------
	-      Create a Vulkan shader module from the compilation result
	---------------------------------------------------------------------*/
	_name = unicode::ToUtf8String(entryPoint);
	_stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	_stage.flags = 0;
	_stage.module = _module;
	_stage.pNext = nullptr;
	_stage.pSpecializationInfo = nullptr;
	_stage.stage = EnumConverter::Convert(_shaderType);
	_stage.pName = _name.c_str();
	

	_blobData.BufferPointer= byteCode->GetBufferPointer();
	_blobData.BufferSize   = byteCode->GetBufferSize();
	
}

/****************************************************************************
*							LoadBinary
*************************************************************************//**
*  @fn        void GPUShaderState::LoadBinary(const core::ShaderType type, const std::wstring& fileName)
*  @brief     Load Binary Data (Offline Compile)
*  @param[in] core::ShaderType type
*  @param[in] std::wstring& fileName : filePath
*  @return 　　void
*****************************************************************************/
void GPUShaderState::LoadBinary(const core::ShaderType type, const std::wstring& fileName)
{
	_shaderType = type;

	/*-------------------------------------------------------------------
	-          Open file
	---------------------------------------------------------------------*/
	std::ifstream fin(fileName, std::ios::binary);
	/*-------------------------------------------------------------------
	-          Calculate buffer byte size
	---------------------------------------------------------------------*/
	fin.seekg(0, std::ios_base::end);
	std::ifstream::pos_type size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);
	/*-------------------------------------------------------------------
	-          Create blob (bytecode) data
	---------------------------------------------------------------------*/
	ComPtr<ID3DBlob> blob;
	ThrowIfFailed(D3DCreateBlob(size, blob.GetAddressOf()));
	/*-------------------------------------------------------------------
	-          Read and Close
	---------------------------------------------------------------------*/
	fin.read((char*)blob->GetBufferPointer(), size);
	fin.close();

	_blobData.BufferPointer = blob->GetBufferPointer();
	_blobData.BufferSize = size;
}