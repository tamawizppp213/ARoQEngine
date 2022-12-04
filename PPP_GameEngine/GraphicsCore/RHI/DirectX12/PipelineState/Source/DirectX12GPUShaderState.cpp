//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12GPUShaderState.cpp
///             @brief  Shader State
///             @author Toide Yutaro
///             @date   2022_07_02
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/PipelineState/Include/DirectX12GPUShaderState.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12EnumConverter.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Core.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Debug.hpp"
#include "GameUtility/File/Include/UnicodeUtility.hpp"
#include "GameUtility/File/Include/FileSystem.hpp"
#include <sstream>
#include <fstream>
#include <d3dcompiler.h>
#include <dxcapi.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////

/****************************************************************************
*                           Compile
*************************************************************************//**
*  @fn        void GPUShaderState::Compile(const core::ShaderType type, const std::wstring& fileName, const std::wstring& entryPoint, const float version)
*  @brief     Online Compile 
*  @param[in] core::ShaderType type
*  @param[in] std::wstring& fileName : filePath
*  @param[in] std::wstring& entryPoint (Main Shader Function Name)
*  @param[in] float version (current newest version : 6.6f)
*  @return 　　void
*****************************************************************************/
void GPUShaderState::Compile(const core::ShaderType type, const std::wstring& fileName, const std::wstring& entryPoint, const float version, const std::vector<std::wstring>& includeDirectories)
{
	assert(0.0f < version && version <= NEWEST_VERSION);
	_shaderType = type; _version = version;

	// Set target Name ex) vs_6.0, ps_6.1...
	std::wstring target = GetShaderTypeName(type) + L"_" + Format(version);
	/*-------------------------------------------------------------------
	-          Select Compile Mode Based on Shader Version
	---------------------------------------------------------------------*/
	if (version >= 6.0f)
	{
		_dxBlob = DxCompile(fileName, entryPoint, target, includeDirectories);
	}
	else
	{
		_dxBlob = DxCompile(fileName, nullptr, entryPoint, target);
	}

	_blobData.BufferPointer = _dxBlob->GetBufferPointer();
	_blobData.BufferSize    = _dxBlob->GetBufferSize();
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
	BlobComPtr blob;
	ThrowIfFailed(D3DCreateBlob(size, blob.GetAddressOf()));
	/*-------------------------------------------------------------------
	-          Read and Close
	---------------------------------------------------------------------*/
	fin.read((char*)blob->GetBufferPointer(), size);
	fin.close();

	_blobData.BufferPointer = blob->GetBufferPointer();
	_blobData.BufferSize    = size;
	_dxBlob = blob;
}

#pragma region DxCompile
BlobComPtr GPUShaderState::DxCompile(const std::wstring& fileName, const std::wstring& entryPoint, const std::wstring& target, const std::vector<std::wstring>& includeDirectories)
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
	ComPtr<IDxcCompiler> dxcCompiler = nullptr;
	ThrowIfFailed(DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler)));
	/*-------------------------------------------------------------------
	-                  Create Blob data of the source code
	---------------------------------------------------------------------*/
	ComPtr<IDxcBlobEncoding> sourceBlob = nullptr;
	UINT32 codePage = CP_UTF8;
	ThrowIfFailed(dxcLibrary->CreateBlobFromFile(fileName.c_str(), &codePage, &sourceBlob));

	ComPtr<IDxcIncludeHandler> dxcIncludeHandler = nullptr;
	dxcLibrary->CreateIncludeHandler(&dxcIncludeHandler);

	/*-------------------------------------------------------------------
	-                  Create Blob data of the source code
	---------------------------------------------------------------------*/
	std::vector<LPCWSTR> arguments = {};
	for (const auto& directory : includeDirectories)
	{
		arguments.push_back(L"-I");
		arguments.push_back(directory.c_str());
	}

	ComPtr<IDxcOperationResult> result = nullptr;
	HRESULT hresult = dxcCompiler->Compile(
		sourceBlob.Get(),
		fileName.c_str(),
		entryPoint.c_str(),
		target.c_str(),
		arguments.data(), static_cast<std::uint32_t>(arguments.size()),
		nullptr, 0,
		dxcIncludeHandler.Get(),
		&result
	);

	BlobComPtr byteCode = nullptr;
	if (SUCCEEDED(hresult)) { result->GetStatus(&hresult); }
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

	return byteCode;
}

BlobComPtr GPUShaderState::DxCompile(const std::wstring& fileName, const D3D_SHADER_MACRO* defines, const std::wstring& entryPoint, const std::wstring& target)
{
	UINT compileFlags = 0;
#if _DEBUG
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	HRESULT hresult = S_OK;

	BlobComPtr byteCode = nullptr;
	BlobComPtr errors;
	hresult = D3DCompileFromFile(fileName.c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		unicode::ToUtf8String(entryPoint).c_str(), unicode::ToUtf8String(target).c_str(), compileFlags, 0, &byteCode, &errors);

	if (errors != nullptr)
	{
		throw std::runtime_error((char*)errors->GetBufferPointer());
	}

	ThrowIfFailed(hresult);

	return byteCode;
}
#pragma endregion DxCompile