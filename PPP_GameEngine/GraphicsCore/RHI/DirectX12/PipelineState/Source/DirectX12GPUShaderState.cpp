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
#pragma region Static Function
namespace
{
	BlobComPtr CompileShader(const std::wstring& fileName, const _D3D_SHADER_MACRO* defines, const std::string& entryPoint, const std::string& target)
	{
		UINT compileFlags = 0;
#if _DEBUG
		compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

		HRESULT hresult = S_OK;

		BlobComPtr byteCode = nullptr;
		BlobComPtr errors;
		hresult = D3DCompileFromFile(fileName.c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entryPoint.c_str(), target.c_str(), compileFlags, 0, &byteCode, &errors);

		if (errors != nullptr)
		{
			OutputDebugStringA((char*)errors->GetBufferPointer());
		}

		ThrowIfFailed(hresult);

		return byteCode;
	}
	/****************************************************************************
	*                       Compile Shader
	*************************************************************************//**
	*  @fn        BlobComPtr CompileShader( const std::wstring& fileName, const std::wstring& entryPoint, const std::wstring& target)
	*  @brief     Compile shader
	*  @param[in] test
	*  @return 　　void
	*****************************************************************************/
	BlobComPtr CompileShader( const std::wstring& fileName, const std::wstring& entryPoint, const std::wstring& target)
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
		ThrowIfFailed(DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&dxcLibrary)));

		ComPtr<IDxcIncludeHandler> includeHandler = nullptr;
		ThrowIfFailed(dxcLibrary->CreateIncludeHandler(&includeHandler));
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
		ComPtr<IDxcOperationResult> result = nullptr;
		HRESULT hresult = dxcCompiler->Compile(
			sourceBlob.Get(),
			fileName.c_str(),
			entryPoint.c_str(),
			target.c_str(),
			nullptr, 0,
			nullptr, 0,
			dxcIncludeHandler.Get(),
			&result
		);

		if (SUCCEEDED(hresult)) { result->GetStatus(&hresult); }
		BlobComPtr data = nullptr;
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

		return byteCode;
	}
	/****************************************************************************
	*							LoadBinary
	*************************************************************************//**
	*  @fn        BlobComPtr LoadBinary(const std::wstring& fileName)
	*  @brief     Load Binary Data (Done Compile)
	*  @param[in] std::wstring& fileName : filePath
	*  @return 　　BlobComPtr
	*****************************************************************************/
	BlobComPtr LoadBinary(const std::string& filename)
	{
		std::ifstream fin(filename, std::ios::binary);

		fin.seekg(0, std::ios_base::end);
		std::ifstream::pos_type size = (int)fin.tellg();
		fin.seekg(0, std::ios_base::beg);

		BlobComPtr blob;
		ThrowIfFailed(D3DCreateBlob(size, blob.GetAddressOf()));

		fin.read((char*)blob->GetBufferPointer(), size);
		fin.close();

		return blob;
	}
}
#pragma endregion Static Function
GPUShaderState::GPUShaderState(
	const std::shared_ptr<core::RHIDevice>& device,
	const core::ShaderType shaderType,
	const std::string& fileName,
	const std::string& entryPoint,
	const std::string& shaderVersion
) : core::GPUShaderState(device, shaderType, fileName, entryPoint, shaderVersion)
{
	float version = std::stof(shaderVersion);

	BlobComPtr blobData = nullptr;
	if (version >= 6.0f)
	{
		blobData = CompileShader(unicode::ToWString(fileName), unicode::ToWString(entryPoint), unicode::ToWString(GetShaderTypeName(shaderType) + "_" + shaderVersion));
	}
	else
	{
		blobData = CompileShader(unicode::ToWString(fileName), nullptr, entryPoint, GetShaderTypeName(shaderType) + "_" + shaderVersion);
	}

	_shaderCode.BytecodeLength  = blobData->GetBufferSize();
	_shaderCode.pShaderBytecode = blobData->GetBufferPointer();
	_blobData.BufferSize        = blobData->GetBufferSize();
	_blobData.BufferPointer     = blobData->GetBufferPointer();
	
}