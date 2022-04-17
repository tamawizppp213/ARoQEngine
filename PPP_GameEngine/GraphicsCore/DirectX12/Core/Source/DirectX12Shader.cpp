//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12GraphicsDevice.cpp
///             @brief  Graphics Device
///             @author Toide Yutaro
///             @date   -
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/DirectX12/Core/Include/DirectX12Shader.hpp"
#include "GraphicsCore/DirectX12/Core/Include/DirectX12Include.hpp"
#include "GraphicsCore/DirectX12/Core/Include/DirectX12Debug.hpp"
#include <fstream>
#include <d3dcompiler.h>
#include <dxcapi.h>
#include <filesystem>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
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

BlobComPtr CompileShader(
	const std::wstring& fileName,
	const std::wstring& entryPoint,
	const std::wstring& target)
{
	std::filesystem::path filePath = fileName;
	std::ifstream infile(filePath);
	std::vector<char> sourceData;

	if (!infile) { throw std::runtime_error("shader not found"); }
	sourceData.resize(uint32_t(infile.seekg(0, infile.end).tellg()));
	infile.seekg(0, infile.beg).read(sourceData.data(), sourceData.size());

	// Compile using DXC
	ComPtr<IDxcLibrary>         library;
	ComPtr<IDxcCompiler>        compiler;
	ComPtr<IDxcBlobEncoding>    source;
	ComPtr<IDxcOperationResult> dxcResult;

	DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(&library));
	library->CreateBlobWithEncodingFromPinned(sourceData.data(), UINT(sourceData.size()), CP_ACP, &source);
	DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compiler));

	LPCWSTR compilerFlags[] = {
#if _DEBUG
	L"/Zi", L"/O0",
#else
	L"/O2"
#endif
	};

	HRESULT hresult = S_OK;
	BlobComPtr byteCode = nullptr;
	BlobComPtr errors   = nullptr;

	hresult = compiler->Compile(
		source.Get(),
		filePath.wstring().c_str(),
		entryPoint.c_str(),
		target.c_str(),
		compilerFlags, _countof(compilerFlags),
		nullptr, 0, // Defines
		nullptr,
		&dxcResult);

	if (SUCCEEDED(hresult))
	{
		dxcResult->GetResult(reinterpret_cast<IDxcBlob**>(byteCode.GetAddressOf()));
	}
	else
	{
		dxcResult->GetErrorBuffer(reinterpret_cast<IDxcBlobEncoding**>(errors.GetAddressOf()));
	}
	ThrowIfFailed(hresult);

	return byteCode;

}
/****************************************************************************
*							LoadBinary
*************************************************************************//**
*  @fn        BlobComPtr LoadBinary(const std::wstring& fileName)
*  @brief     Load Binary Data (Done Compile)
*  @param[in] std::wstring& fileName : filePath
*  @return @@BlobComPtr
*****************************************************************************/
BlobComPtr LoadBinary(const std::wstring& filename)
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
