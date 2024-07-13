//////////////////////////////////////////////////////////////////////////////////
///             @file   GPUBlendState.hpp
///             @brief  Blend State
///             @author Toide Yutaro
///             @date   2022_06_28
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GPU_SHADER_STATE_HPP
#define GPU_SHADER_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GPUShaderCompiler.hpp"
#include "GPUState.hpp"


//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class RHIDevice;

	/****************************************************************************
	*				  			GPUShaderState
	****************************************************************************/
	/* @brief  �V�F�[�_�[�R�[�h���i�[����N���X�ł�.
	*****************************************************************************/
	class GPUShaderState : public GPUState
	{
	public:
		/*! @brief �V�F�[�_�[���f���̍ŐV�o�[�W���� (�K�X�X�V���Ă�������)*/
		static constexpr float NEWEST_VERSION = 6.8f;

		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     HLSL�t�@�C�������A���^�C���ɃR���p�C�����܂�. ����ɂ��, �V�F�[�_�[�R�[�h����������܂�.
		*  @param[in] const ShaderCompilerOption& option : �V�F�[�_�[�R���p�C�����̐ݒ荀��
		*  @return    void
		*************************************************************************/
		virtual void Compile(const ShaderCompilerOption& option) = 0;

		// @brief : Offline Compile, already compiled fileName(filePath)
		virtual void LoadBinary(const core::ShaderType type, const gu::tstring& fileName) = 0;
		#pragma endregion

		#pragma region Public Property

		/*!**********************************************************************
		*  @brief     �V�F�[�_�[�R�[�h�̏���Ԃ��܂�. 
		*  @param[in] void
		*  @return    const ShaderCode& : �V�F�[�_�[�R�[�h�̏��
		*************************************************************************/
		__forceinline const ShaderCode& GetBlobData() const noexcept { return _blobData; }
		
		/*!**********************************************************************
		*  @brief     �V�F�[�_�[�R�[�h�̐擪�|�C���^��Ԃ��܂�
		*  @param[in] void
		*  @return    void* : �V�F�[�_�[�R�[�h�̐擪�|�C���^
		*************************************************************************/
		[[nodiscard]] __forceinline void* Get() const noexcept { return _blobData.Pointer; }
		
		/*!**********************************************************************
		*  @brief     �V�F�[�_�[�R�[�h���̂̃o�C�g����Ԃ��܂�. 
		*  @param[in] void
		*  @return    gu::uint64 : �V�F�[�_�[�R�[�h�̃o�C�g��
		*************************************************************************/
		__forceinline gu::uint64 GetByteSize() const noexcept { return _blobData.ByteSize; }
		
		/*!**********************************************************************
		*  @brief     �V�F�[�_�[�̎�ނ��w�肵�܂�. 
		*  @param[in] void
		*  @return    ShaderType : �V�F�[�_�[�̎��
		*************************************************************************/
		__forceinline ShaderType GetShaderType() const noexcept { return _shaderType; }
		
		/*!**********************************************************************
		*  @brief     �V�F�[�_�[�̃o�[�W�������w�肵�܂�. 
		*  @param[in] void
		*  @return    float �o�[�W����
		*************************************************************************/
		__forceinline gu::float32 GetShaderVersion () const noexcept { return _version; }

		/*!**********************************************************************
		*  @brief     �ݒ肳�ꂽ�V�F�[�_�[��RayTracing�Ŏg�p�\�ȃV�F�[�_�[��
		*  @param[in] void
		*  @return    bool 
		*************************************************************************/
		bool IsRayTracingShader() const;

		#pragma endregion

		#pragma region Public Constructor and Destructor

		#pragma endregion

	protected:
		#pragma region Protected Constructor and Destructor

		/*! @brief �f�t�H���g�R���X�g���N�^*/
		GPUShaderState() = default;

		/*! @brief �f�X�g���N�^*/
		~GPUShaderState() = default;

		/*! @brief �f�o�C�X���g���č쐬���܂�. */
		explicit GPUShaderState(
			const gu::SharedPointer<core::RHIDevice>& device) : GPUState(device){};

		#pragma endregion

		#pragma region Protected Function
		gu::tstring GetShaderTypeName(ShaderType shaderType);

		gu::tstring Format(float version);

		/*!**********************************************************************
		*  @brief     �L���ȃV�F�[�_�[�ݒ肩�ǂ�����Ԃ��܂�
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		bool IsValidShaderType() const noexcept;

		#pragma endregion

		#pragma region Protected Property

		/*! @brief �V�F�[�_�[�R�[�h�̏�� */
		ShaderCode _blobData = {};

		/*! @brief �V�F�[�_�[�̌��݂̃o�[�W����*/
		float _version = 6.0f;

		/*! @brief �V�F�[�_�[�̎��*/
		ShaderType  _shaderType = ShaderType::Vertex;

		#pragma endregion
	};
}
#endif