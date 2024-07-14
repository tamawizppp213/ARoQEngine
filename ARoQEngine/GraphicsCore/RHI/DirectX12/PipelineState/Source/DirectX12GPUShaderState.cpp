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
#include "Platform/Core/Include/CoreFileSystem.hpp"
#include "GameUtility/File/Include/UnicodeUtility.hpp"
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
	-      �V�F�[�_�[�o�[�W�����ɔ����ăR���p�C�����@��ύX
	---------------------------------------------------------------------*/
	if (option.Version >= 6.0f)
	{
		DXILCompile(option);
	}
	else
	{
		/*-------------------------------------------------------------------
		-         �V�F�[�_�[�^�[�Q�b�g��ݒ� ex) vs_6.0, ps_6.1...
		---------------------------------------------------------------------*/
		gu::tstring target = GetShaderTypeName(option.Type) + SP("_") + gu::tstring(Format(option.Version).CString());

		gu::DynamicArray<D3D_SHADER_MACRO> dxMacros(option.Defines.Size());
		gu::DynamicArray<std::string> nameList(option.Defines.Size()); //�_���������O�΍�
		for (int i = 0; i < option.Defines.Size(); ++i)
		{
			//const auto temp = std::wstring(option.Defines[i].CString());
			//nameList[i]            = unicode::ToUtf8String(temp);
			//dxMacros[i].Name       = nameList[i].c_str();
			//dxMacros[i].Definition = nullptr; // ��X
		}

		_sourceBlob = DxCompile(option.FileName,dxMacros.Data(), option.EntryPoint, target);
	}

	_blobData.Pointer = _sourceBlob->GetBufferPointer();
	_blobData.ByteSize= _sourceBlob->GetBufferSize();
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
	_sourceBlob = blob;
}

#pragma region DxCompile
void GPUShaderState::DXILCompile(const core::ShaderCompilerOption& option)
{
	const auto fileSystem = platform::core::IFileSystem::Get();

	/*-------------------------------------------------------------------
	-         DXC���C�u�������쐬��, �V�F�[�_�[�R�[�h�̓ǂݍ��݂�R���p�C�����s��
	---------------------------------------------------------------------*/
	ComPtr<IDxcLibrary> dxcLibrary = nullptr;
	ThrowIfFailed(DxcCreateInstance(CLSID_DxcLibrary, IID_PPV_ARGS(dxcLibrary.GetAddressOf())));
	
	/*-------------------------------------------------------------------
	-          DXC�R���p�C�����쐬
	---------------------------------------------------------------------*/
	ComPtr<IDxcCompiler3> dxcCompiler = nullptr;
	ThrowIfFailed(DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler)));

	/*-------------------------------------------------------------------
	-			DXC�̃��t���N�V�����p��Util�N���X���쐬
	---------------------------------------------------------------------*/
	ComPtr<IDxcUtils> dxcUtils = nullptr;
	ThrowIfFailed(DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils)));

	/*-------------------------------------------------------------------
	-			�\�[�X�R�[�h�̏��쐬
	---------------------------------------------------------------------*/
	ComPtr<IDxcBlobEncoding> sourceBlob = nullptr;
	UINT32 codePage = CP_UTF8;
	ThrowIfFailed(dxcLibrary->CreateBlobFromFile(option.FileName.CString(), &codePage, &sourceBlob));

	const DxcBuffer sourceBuffer = { sourceBlob->GetBufferPointer(), sourceBlob->GetBufferSize(), codePage };

	/*-------------------------------------------------------------------
	-			Includer�̍쐬
	---------------------------------------------------------------------*/
	ComPtr<IDxcIncludeHandler> dxcIncludeHandler = nullptr;
	dxcLibrary->CreateIncludeHandler(&dxcIncludeHandler);

	/*-------------------------------------------------------------------
	-                  Create Blob data of the source code
	---------------------------------------------------------------------*/
	gu::DynamicArray<const gu::tchar*> arguments = {};
	SetupDXILArguments(option, arguments);

	/*-------------------------------------------------------------------
	-         �V�F�[�_�[�^�[�Q�b�g��ݒ� ex) vs_6.0, ps_6.1... (���[�J���ϐ��ɂ��Ȃ��Ɛ���ɓ����Ȃ����ߐ؂�o���܂���)
	---------------------------------------------------------------------*/
	const gu::tstring target = GetShaderTypeName(option.Type) + SP("_") + gu::tstring(Format(option.Version));
	arguments.Push(L"-T");
	arguments.Push(target.CString());

	/*-------------------------------------------------------------------
	-         �R���p�C���̎��s
	---------------------------------------------------------------------*/
	ComPtr<IDxcResult> compileResult = nullptr;

	ThrowIfFailed(dxcCompiler->Compile(&sourceBuffer, arguments.Data(), (gu::uint32)arguments.Size(), dxcIncludeHandler.Get(),IID_PPV_ARGS(compileResult.GetAddressOf())));

	if (!compileResult)
	{
		return;
	}

	/*-------------------------------------------------------------------
	-         �R���p�C�������������ꍇ��bytecode��Ԃ�
	---------------------------------------------------------------------*/
	HRESULT compileStatus = S_OK;
	compileResult->GetStatus(&compileStatus);

	if (SUCCEEDED(compileStatus))
	{
		/*-------------------------------------------------------------------
		-         �V�F�[�_�[�R�[�h���擾
		---------------------------------------------------------------------*/
		ComPtr<IDxcBlobUtf16> sourceCodeNameBlob = nullptr;
		Check  (compileResult->HasOutput(DXC_OUT_OBJECT));
		ThrowIfFailed(compileResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(_sourceBlob.GetAddressOf()), sourceCodeNameBlob.GetAddressOf()));
		
		/*-------------------------------------------------------------------
		-         �V�F�[�_�[���t���N�V�������擾
		---------------------------------------------------------------------*/
		ComPtr<IDxcBlobUtf16> reflectionNameBlob = nullptr;
		Check  (compileResult->HasOutput(DXC_OUT_REFLECTION));
		ThrowIfFailed(compileResult->GetOutput(DXC_OUT_REFLECTION, IID_PPV_ARGS(_reflectionBlob.GetAddressOf()), reflectionNameBlob.GetAddressOf()));
		
		/*-------------------------------------------------------------------
		-         �t�@�C�����o��
		---------------------------------------------------------------------*/
		if (!option.DebugOutputDirectory.IsEmpty())
		{
			/*const auto      reflectionFileName = option.DebugOutputDirectory + SP("\\") + fileSystem->GetFileName(option.FileName) + SP(".d3dasm");
			const auto      reflectionFile     = fileSystem->OpenWrite(reflectionFileName);
			const DxcBuffer reflectionBuffer   = { _reflectionBlob->GetBufferPointer(), _reflectionBlob->GetBufferSize() };
			ComPtr<ID3D12LibraryReflection> libraryReflection = nullptr;
			dxcUtils->CreateReflection(&reflectionBuffer, IID_PPV_ARGS(&libraryReflection));*/

		}

		return;
	}
	
	if (!compileResult) { return; }

	/*-------------------------------------------------------------------
	-         �R���p�C�����o���Ȃ������ꍇ, �G���[���b�Z�[�W���o��
	---------------------------------------------------------------------*/
	ComPtr <IDxcBlobEncoding> errorBlob = nullptr;
	const auto errorResult = compileResult->GetErrorBuffer(&errorBlob);
	if (SUCCEEDED(errorResult) && errorBlob)
	{
		gu::string errorMessage = "Compilation failed with errors: \n%hs\n";
		errorMessage += (const char*)errorBlob->GetBufferPointer();
		_RPTN(_CRT_ERROR, "%s", errorMessage.CString());
	}
}

/*!**********************************************************************
*  @brief     ShaderModel6.0�ȏ�œ��삵�܂�.
*  @param[in] const core::ShaderCompilerOption&
*  @param[out]gu::DynamicArray<const gu::tchar*>& �R�}���h���C���̈���
*  @return    void
*************************************************************************/
void GPUShaderState::SetupDXILArguments(const core::ShaderCompilerOption& option, gu::DynamicArray<const gu::tchar*>& arguments)
{
	const auto fileSystem = platform::core::IFileSystem::Get();

	arguments.Reserve(2 * (option.IncludeDirectories.Size() + option.Defines.Size() + 4)); // +4 is for -T, -E -Fc, -Fo
	for (const auto& directory : option.IncludeDirectories)
	{
		arguments.Push(L"-I");
		arguments.Push(directory.CString());
	}

	/*-------------------------------------------------------------------
	-               �f�o�b�O���s������, DXIL�̒��Ԍ�����e�L�X�g�o�͂���
	---------------------------------------------------------------------*/
	if (!option.DebugOutputDirectory.IsEmpty())
	{
		const auto asmFileName  = option.DebugOutputDirectory + SP("\\") + fileSystem->GetFileName(option.FileName) + SP(".d3dasm");
		const auto dxilFileName = option.DebugOutputDirectory + SP("\\") + fileSystem->GetFileName(option.FileName) + SP(".cso");
		arguments.Push(L" -Fc");
		arguments.Push(asmFileName.CString());

		arguments.Push(L" -Fo ");
		arguments.Push(dxilFileName.CString());
	}

	/*-------------------------------------------------------------------
	-        �œK���ݒ�
	---------------------------------------------------------------------*/
	if (gu::HasAllFlags(option.OptimizationFlags, core::ShaderCompilerFlags::Debug))
	{
		arguments.Push(L"-Od");
	}
	else if (gu::HasAllFlags(option.OptimizationFlags, core::ShaderCompilerFlags::LowLevelOptimization))
	{
		arguments.Push(L"-O1");
	}
	else
	{
	#if _DEBUG
		arguments.Push(L"-Od");
	#else
		arguments.Push(L"-O3");
	#endif
	}

	/*-------------------------------------------------------------------
	-        �x��
	---------------------------------------------------------------------*/
	if (gu::HasAllFlags(option.OptimizationFlags, core::ShaderCompilerFlags::WarningAsErrors))
	{
		arguments.Push(L"-WX");
	}
	else if (gu::HasAllFlags(option.OptimizationFlags, core::ShaderCompilerFlags::NoWarnings))
	{
		arguments.Push(L"-no-warnings");
	}

	/*-------------------------------------------------------------------
	-        PDB�t�@�C���𐶐�����
	---------------------------------------------------------------------*/
	if (gu::HasAllFlags(option.OptimizationFlags, core::ShaderCompilerFlags::GenerateDebugSymbol))
	{
		if(gu::HasAllFlags(option.OptimizationFlags, core::ShaderCompilerFlags::DebugNameForSource))
		{
			arguments.Push(L"-Zss");
		}
		else if (gu::HasAllFlags(option.OptimizationFlags, core::ShaderCompilerFlags::DebugNameForBinary))
		{
			arguments.Push(L"-Zsb");
		}
		else
		{
			arguments.Push(L"-Zsb");
		}

		arguments.Push(L"-Zi");
		arguments.Push(L"-Qembed_debug");
		arguments.Push(L"-Fd");
		arguments.Push(L".\\");
	}

	/*-------------------------------------------------------------------
	-        ������BindingSpace�����肷��
	---------------------------------------------------------------------*/
	if(gu::HasAllFlags(option.OptimizationFlags, core::ShaderCompilerFlags::AutoBindingSpace))
	{
		arguments.Push(L"-auto-binding-space");
	}

	/*-------------------------------------------------------------------
	-         �G���g���|�C���g��ݒ�
	---------------------------------------------------------------------*/
	arguments.Push(L"-E");
	arguments.Push(option.EntryPoint.CString());

	/*-------------------------------------------------------------------
	-         �}�N����ݒ�
	---------------------------------------------------------------------*/
	for (const auto& define : option.Defines)
	{
		arguments.Push(L"-D");
		arguments.Push(define.CString());
	}
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