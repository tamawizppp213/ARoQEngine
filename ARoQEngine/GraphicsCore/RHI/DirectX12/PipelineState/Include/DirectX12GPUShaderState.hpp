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
		*  @brief     DirectX12��Blob��Ԃ��܂�.
		*  @param[in] void
		*  @return    BlobComPtr : DirectX12��Blob
		*************************************************************************/
		BlobComPtr GetDxBlob() const noexcept { return _dxBlob; }

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
		*  @param[in] const gu::tstring& fileName : �t�@�C����
		*  @param[in] const gu::tstring& entryPoint : �G���g���[�|�C���g
		*  @param[in] const gu::tstring& target : �V�F�[�_�[�^�[�Q�b�g
		*  @param[in] include���s���f�B���N�g���̃��X�g
		*  @param[in] define�}�N�����g�p���郊�X�g
		*  @return    BlobComPtr : DirectX12��Blob
		*************************************************************************/
		BlobComPtr DxilCompile(const gu::tstring& fileName, const gu::tstring& entryPoint, const gu::tstring& target, const gu::DynamicArray<gu::tstring>& includeDirectories, const gu::DynamicArray<gu::tstring>& defines);
		
		BlobComPtr DxCompile(const gu::tstring& fileName, const D3D_SHADER_MACRO* defines, const gu::tstring& entryPoint, const gu::tstring& target);
		
		#pragma endregion

		#pragma region Protected Property
		BlobComPtr _dxBlob = nullptr;
		#pragma endregion
	};
}
#endif