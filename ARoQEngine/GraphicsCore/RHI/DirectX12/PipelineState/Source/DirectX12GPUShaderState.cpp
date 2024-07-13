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
#include "GameUtility/Base/Include/GUAssert.hpp"
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
#pragma warning (disable: 6387)
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////

/*!**********************************************************************
*  @brief     HLSL�t�@�C�������A���^�C���ɃR���p�C�����܂�. ����ɂ��, �V�F�[�_�[�R�[�h����������܂�.
*  @param[in] const ShaderCompilerOption& option : �V�F�[�_�[�R���p�C�����̐ݒ荀��
*  @return    void
*************************************************************************/
void GPUShaderState::Compile(const core::ShaderCompilerOption& option)
{
	/*-------------------------------------------------------------------
	-         �V�F�[�_�[�o�[�W�����̃`�F�b�N
	---------------------------------------------------------------------*/
	Check(0.0f < option.Version && option.Version <= NEWEST_VERSION);
	
	_version = option.Version;

	/*-------------------------------------------------------------------
	-         �V�F�[�_�[��ʂ̃`�F�b�N
	---------------------------------------------------------------------*/
	Check(IsValidShaderType());

	_shaderType = option.Type; 

	/*-------------------------------------------------------------------
	-         �V�F�[�_�[�^�[�Q�b�g��ݒ� ex) vs_6.0, ps_6.1...
	---------------------------------------------------------------------*/
	gu::tstring target = GetShaderTypeName(option.Type) + SP("_") + gu::tstring(Format(option.Version).CString());

	/*-------------------------------------------------------------------
	-      �V�F�[�_�[�o�[�W�����ɔ����ăR���p�C�����@��ύX
	---------------------------------------------------------------------*/
	if (option.Version >= 6.0f)
	{
		_dxBlob = DxilCompile(option.FileName, option.EntryPoint, target, option.IncludeDirectories, option.Defines);
	}
	else
	{
		gu::DynamicArray<D3D_SHADER_MACRO> dxMacros(option.Defines.Size());
		gu::DynamicArray<std::string> nameList(option.Defines.Size()); //�_���������O�΍�
		for (int i = 0; i < option.Defines.Size(); ++i)
		{
			const auto temp = std::wstring(option.Defines[i].CString());
			nameList[i]            = unicode::ToUtf8String(temp);
			dxMacros[i].Name       = nameList[i].c_str();
			dxMacros[i].Definition = nullptr; // ��X
		}

		_dxBlob = DxCompile(option.FileName,dxMacros.Data(), option.EntryPoint, target);
	}

	_blobData.Pointer = _dxBlob->GetBufferPointer();
	_blobData.ByteSize    = _dxBlob->GetBufferSize();
}
/****************************************************************************
*							LoadBinary
****************************************************************************/
/* @fn        void GPUShaderState::LoadBinary(const core::ShaderType type, const gu::tstring& fileName)
* 
*  @brief     Load Binary Data (Offline Compile)
* 
*  @param[in] core::ShaderType type
* 
*  @param[in] gu::tstring& fileName : filePath
* 
*  @return �@�@void
*****************************************************************************/
void GPUShaderState::LoadBinary(const core::ShaderType type, const gu::tstring& fileName)
{
	_shaderType = type;

	/*-------------------------------------------------------------------
	-          Open file
	---------------------------------------------------------------------*/
	const auto stdFileName = std::wstring(fileName.CString());
	std::ifstream fin(stdFileName, std::ios::binary);

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

	_blobData.Pointer = blob->GetBufferPointer();
	_blobData.ByteSize    = size;
	_dxBlob = blob;
}

#pragma region DxCompile
BlobComPtr GPUShaderState::DxilCompile(const gu::tstring& fileName, const gu::tstring& entryPoint, const gu::tstring& target, const gu::DynamicArray<gu::tstring>& includeDirectories, const gu::DynamicArray<gu::tstring>& defines)
{
	/*-------------------------------------------------------------------
	-         DXC���C�u�������쐬��, �V�F�[�_�[�R�[�h�̓ǂݍ��݂�R���p�C�����s��
	---------------------------------------------------------------------*/
	ComPtr<IDxcLibrary> dxcLibrary = nullptr;
	ThrowIfFailed(DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(dxcLibrary.GetAddressOf())));
	
	/*-------------------------------------------------------------------
	-          DXC�R���p�C�����쐬
	---------------------------------------------------------------------*/
	ComPtr<IDxcCompiler> dxcCompiler = nullptr;
	ThrowIfFailed(DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler)));

	/*-------------------------------------------------------------------
	-			�\�[�X�R�[�h�̏��쐬
	---------------------------------------------------------------------*/
	ComPtr<IDxcBlobEncoding> sourceBlob = nullptr;
	UINT32 codePage = CP_UTF8;
	ThrowIfFailed(dxcLibrary->CreateBlobFromFile(fileName.CString(), &codePage, &sourceBlob));

	/*-------------------------------------------------------------------
	-			Includer�̍쐬
	---------------------------------------------------------------------*/
	ComPtr<IDxcIncludeHandler> dxcIncludeHandler = nullptr;
	dxcLibrary->CreateIncludeHandler(&dxcIncludeHandler);

	/*-------------------------------------------------------------------
	-                  Create Blob data of the source code
	---------------------------------------------------------------------*/
	gu::DynamicArray<const gu::wchar*> arguments = {};
	arguments.Reserve(includeDirectories.Size());
	for (const auto& directory : includeDirectories)
	{
		arguments.Push(L"-I");
		arguments.Push(directory.CString());
	}

	gu::DynamicArray<DxcDefine> dxcDefines(defines.Size());
	for (size_t i = 0; i < defines.Size(); ++i)
	{
		dxcDefines[i].Name = defines[i].CString();
	}
	dxcDefines.Push({ .Name = L"DIRECTX" }); // default setting
#ifdef _DEBUG
	dxcDefines.Push({ .Name = L"_DEBUG" });  // debug
#endif
	
	ComPtr<IDxcOperationResult> result = nullptr;
	HRESULT hresult = dxcCompiler->Compile(
		sourceBlob.Get(),
		fileName.CString(),
		entryPoint.CString(),
		target.CString(),
		arguments.Data(), static_cast<std::uint32_t>(arguments.Size()),
		dxcDefines.IsEmpty() ? nullptr : dxcDefines.Data(),
		dxcDefines.IsEmpty() ? 0 : (UINT32)dxcDefines.Size(),
		dxcIncludeHandler.Get(),
		&result
	);

	BlobComPtr byteCode = nullptr;
	if (SUCCEEDED(hresult))
	{
		result->GetStatus(&hresult);
	}
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

BlobComPtr GPUShaderState::DxCompile(const gu::tstring& fileName, const D3D_SHADER_MACRO* defines, const gu::tstring& entryPoint, const gu::tstring& target)
{
	UINT compileFlags = 0;
#if _DEBUG
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	HRESULT hresult = S_OK;

	BlobComPtr byteCode = nullptr;
	BlobComPtr errors;
	hresult = D3DCompileFromFile(fileName.CString(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		unicode::ToUtf8String(std::wstring(entryPoint.CString())).c_str(), unicode::ToUtf8String(std::wstring(target.CString())).c_str(), compileFlags, 0, &byteCode, &errors);

	if (errors != nullptr)
	{
		throw std::runtime_error((char*)errors->GetBufferPointer());
	}

	ThrowIfFailed(hresult);

	return byteCode;
}
#pragma endregion DxCompile