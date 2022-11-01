//////////////////////////////////////////////////////////////////////////////////
//              @file   VulkanShader.cpp
///             @brief  Shader Compile 
///             @author Toide Yutaro // Reference : https://github.com/KhronosGroup/Vulkan-Guide/blob/master/chapters/hlsl.adoc
///             @date   2022_10_31
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanShader.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanDevice.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Debug.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Core.hpp"
#include <sstream>
#include <fstream>
#include <d3dcompiler.h>
#include <dxcapi.h>
// https://github.com/KhronosGroup/Vulkan-Guide/blob/master/chapters/hlsl.adoc
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::vulkan;
using namespace rhi;

//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
RHIShader::~RHIShader()
{
	const auto vkDevice = std::static_pointer_cast<vulkan::RHIDevice>(_device)->GetDevice();
	if (_shaderModule)
	{
		vkDestroyShaderModule(vkDevice, _shaderModule, nullptr);
	}
}
#pragma endregion Constructor and Destructor
void RHIShader::Compile(
	const std::wstring& fileName,
	const std::wstring& entryPoint,
	const std::wstring& target)
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
	-       Select target profile based on shader file extension
	---------------------------------------------------------------------*/
	// HLSLのみを使用した想定のため, 現段階では未実装.

	/*-------------------------------------------------------------------
	-      Configure the compiler arguments for compiling the HLSL shader to SPIR-V
	---------------------------------------------------------------------*/
	std::vector<LPCWSTR> arguments = {
		fileName.c_str(), // (optional) name of the shader file to be displayed e.g. in an error message
		L"-E", entryPoint.c_str(), // shader main entry point
		L"-T", target.c_str(),     // shader target profile
		L"-spirv"                  // compile to SPIRV
	};

	/*-------------------------------------------------------------------
	-                Compile Shader
	---------------------------------------------------------------------*/
	DxcBuffer buffer = {};
	buffer.Encoding = DXC_CP_ACP;
	buffer.Ptr      = sourceBlob->GetBufferPointer();
	buffer.Size     = sourceBlob->GetBufferSize();
	ComPtr<IDxcResult> result = nullptr;
	HRESULT hresult = dxcCompiler->Compile(
		&buffer,
		arguments.data(),
		static_cast<std::uint32_t>(arguments.size()),
		dxcIncludeHandler.Get(), IID_PPV_ARGS(result.GetAddressOf())
	);

	/*-------------------------------------------------------------------
	-              Output error if compilation failed
	---------------------------------------------------------------------*/
	if (SUCCEEDED(hresult)) { result->GetStatus(&hresult); }

	BlobComPtr byteCode = nullptr;
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
		result->GetResult(reinterpret_cast<IDxcBlob**>(byteCode.GetAddressOf()));
	}

	/*-------------------------------------------------------------------
	-      Create a Vulkan shader module from the compilation result 
	---------------------------------------------------------------------*/
	const auto vkDevice = std::static_pointer_cast<vulkan::RHIDevice>(_device)->GetDevice();
	VkShaderModuleCreateInfo shaderModuleInfo = {};
	shaderModuleInfo.sType    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModuleInfo.codeSize = byteCode->GetBufferSize();
	shaderModuleInfo.pCode    = (std::uint32_t*)byteCode->GetBufferPointer();
	shaderModuleInfo.pNext    = nullptr;
	if (vkCreateShaderModule(vkDevice, &shaderModuleInfo, nullptr, &_shaderModule) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create vk shader module");
	}
	
	_bufferPointer  = byteCode->GetBufferPointer();
	_bufferByteSize = byteCode->GetBufferSize();

}
void RHIShader::LoadBinary(const std::wstring& fileName)
{
	std::ifstream fin(fileName, std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	std::ifstream::pos_type size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);

	BlobComPtr blob;
	ThrowIfFailed(D3DCreateBlob(size, blob.GetAddressOf()));

	fin.read((char*)blob->GetBufferPointer(), size);
	fin.close();

	_bufferPointer = blob->GetBufferPointer();
	_bufferByteSize = size;
}
#pragma endregion RHIShader