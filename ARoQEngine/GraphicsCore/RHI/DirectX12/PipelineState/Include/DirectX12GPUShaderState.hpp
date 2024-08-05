//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12GPUShaderState.hpp
///             @brief  DirectX12GPUShaderState.hpp
///             @author Toide Yutaro
///             @date   2022_07_02
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_GPU_SHADER_STATE_HPP
#define DIRECTX12_GPU_SHADER_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUShaderState.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Core.hpp"
#include <d3d12.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
struct IDxcBlob;

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::directX12
{

	/****************************************************************************
	*				  			GPUShaderState
	****************************************************************************/
	/*  @brief �V�F�[�_�[�R�[�h���i�[����N���X�ł�.
	*****************************************************************************/
	class GPUShaderState : public rhi::core::GPUShaderState
	{
	public:	
		#pragma region Public Function
		
		/*!**********************************************************************
		*  @brief     HLSL�t�@�C�������A���^�C���ɃR���p�C�����܂�. ����ɂ��, �V�F�[�_�[�R�[�h����������܂�.
		*  @param[in] const ShaderCompilerOption& option : �V�F�[�_�[�R���p�C�����̐ݒ荀��
		*  @return    void
		*************************************************************************/
		virtual void Compile(const core::ShaderCompilerOption& option) override;

		// @brief : Offline Compile, already compiled fileName(filePath)
		void LoadBinary(const core::ShaderType type, const gu::tstring& fileName) override ;
		
		#pragma endregion

		#pragma region Public Property

		/*!**********************************************************************
		*  @brief     DirectX12�̃\�[�X�R�[�h��Blob�f�[�^��Ԃ��܂�.
		*  @param[in] void
		*  @return    BlobComPtr : DirectX12�̃\�[�X�R�[�h
		*************************************************************************/
		BlobComPtr GetSourceBlob() const noexcept { return _sourceBlob; }

		/*!**********************************************************************
		*  @brief     DirectX12��Reflection��Blob�f�[�^��Ԃ��܂�.
		*  @param[in] void
		*  @return    BlobComPtr : DirectX12��Reflection��Blob
		*************************************************************************/
		BlobComPtr GetReflectionBlob() const noexcept { return _reflectionBlob; }

		/*!**********************************************************************
		*  @brief     DirectX12�Ŏg�p����ShaderByteCode��Ԃ��܂�.
		*  @param[in] void
		*  @return    D3D12_SHADER_BYTECODE
		*************************************************************************/
		D3D12_SHADER_BYTECODE GetDxShader() const { return D3D12_SHADER_BYTECODE(_blobData.Pointer, _blobData.ByteSize); }

		#pragma endregion

		#pragma region Public Constructor and Destructor

		/*! @brief �f�t�H���g�R���X�g���N�^*/
		GPUShaderState() = default;

		/*! @brief �f�X�g���N�^*/
		~GPUShaderState() = default;

		/*! @brief �_���f�o�C�X�ŏ���������R���X�g���N�^*/
		explicit GPUShaderState(
			const gu::SharedPointer<core::RHIDevice>& device) : rhi::core::GPUShaderState(device) {};

		#pragma endregion

	protected:
		#pragma region Protected Constructor and Destructor
		#pragma endregion

		#pragma region Protected Function
		/*!**********************************************************************
		*  @brief     ShaderModel6.0�ȏ�œ��삵�܂�.DXIL�Ƃ������Ԍ���ɃR���p�C�����܂� 
		*  @param[in] const core::ShaderCompilerOption	
		*  @return    void
		*************************************************************************/
		void DXILCompile(const core::ShaderCompilerOption& option);
		
		/*!**********************************************************************
		*  @brief     ShaderModel6.0�ȏ�œ��삵�܂�. https://simoncoenen.com/blog/programming/graphics/DxcCompiling
		*  @param[in] const core::ShaderCompilerOption& 
		*  @param[out]gu::DynamicArray<const gu::tchar*>& �R�}���h���C���̈��� 
		*  @return    void
		*************************************************************************/
		void SetupDXILArguments(const core::ShaderCompilerOption& option, gu::DynamicArray<const gu::tchar*>& arguments);

		BlobComPtr DxCompile(const gu::tstring& fileName, const D3D_SHADER_MACRO* defines, const gu::tstring& entryPoint, const gu::tstring& target);
		
		#pragma endregion

		#pragma region Protected Property
		/*! @brief �\�[�X�R�[�h�̃o�b�t�@*/
		BlobComPtr _sourceBlob = nullptr;

		/*! @brief ���t���N�V�����f�[�^��ێ�����o�b�t�@*/
		BlobComPtr _reflectionBlob = nullptr;
 
		#pragma endregion
	};
}
#endif