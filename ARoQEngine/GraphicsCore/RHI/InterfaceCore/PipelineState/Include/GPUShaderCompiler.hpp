//////////////////////////////////////////////////////////////////////////////////
///  @file   GPUShaderCompiler.hpp
///  @brief  temp
///  @author toide
///  @date   2024/07/13 11:48:22
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GPU_SHADER_COMPILER_HPP
#define GPU_SHADER_COMPILER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace rhi::core
{
	/****************************************************************************
	*				  			ShaderCompilerFlags
	****************************************************************************/
	/* @brief  �R���p�C������ۂ̍œK���t���O. 
	*****************************************************************************/
	enum class ShaderCompilerFlags : gu::uint64
	{
		None                 = 0,
		Debug                = 1 << 0, //!< �R���p�C����, �f�o�b�O�t�@�C��/�s/�^/�V���{�����o�̓R�[�h�ɑ}������悤�Ɏw��
		SkipValidation       = 1 << 1, //!< �V�F�[�_�[�̌��؂𖳌������܂�. 
		LowLevelOptimization = 1 << 2, //!< �R���p�C���͎��s���ԂƃR�[�h�T�C�Y�̃o�����X�����悤�ɍœK��O1)
		WarningAsErrors      = 1 << 3, //!< �x�����G���[�Ƃ��Ĉ����܂�
		NoWarnings           = 1 << 4, //!< �x�����o�͂��Ȃ�
		StrictMode           = 1 << 4, //!< ���i���[�h���g�p���܂�
		PreferFlowControl    = 1 << 5, //!< ��������Ȃǂ̃t���[����\����D�悵�܂�
		AvoidFlowControl     = 1 << 6, //!< ��������Ȃǂ̃t���[����\��������܂�
		GenerateDebugSymbol  = 1 << 7, //!< PDB�o�͂̍ۂɎg�p���܂�. �f�o�b�O�V���{���𐶐����܂�
		DebugNameForSource   = 1 << 8, //!< �f�o�b�OPDB���o�͂���ꍇ, �n�b�V���Ƀ\�[�X�t�@�C���ƃo�C�i�����w�肵�܂�
		DebugNameForBinary   = 1 << 9, //!< �f�o�b�OPDB���o�͂���ꍇ, �n�b�V���Ƀo�C�i���t�@�C�������g�p���܂�.
		AutoBindingSpace     = 1 << 10,//!< ������BindingSpace�����肷��
	};

	ENUM_CLASS_FLAGS(ShaderCompilerFlags);

	/****************************************************************************
	*				  	   ShaderCompileOption
	****************************************************************************/
	/* @brief  �V�F�[�_�[�R���p�C�����̐ݒ荀�� (�A���C�������g�΍���s��������, ����܂ł̌Ăяo���Ɠ������Ԃŏ������ł���悤�ɂ��Ă���)
	*****************************************************************************/
	struct ShaderCompilerOption
	{
	public:
		static constexpr gu::float32 NEWEST_VERSION = 6.6f;
		#pragma region Public Function

		#pragma endregion 

		#pragma region Public Property
		/*! @brief �V�F�[�_�[�̎��*/
		ShaderType Type = ShaderType::Vertex;

		/*! @brief �t�@�C����*/
		gu::tstring FileName = SP("");

		/*! @brief �G���g���|�C���g*/
		gu::tstring EntryPoint = SP("main");

		/*! @brief include���s���f�B���N�g���̃��X�g*/
		gu::DynamicArray<gu::tstring> IncludeDirectories = {};

		/*! @brief define�}�N�����g�p���郊�X�g*/
		gu::DynamicArray<gu::tstring> Defines = {};

		/*! @brief �V�F�[�_�[�̃R���p�C�����̐ݒ荀��*/
		ShaderCompilerFlags OptimizationFlags = ShaderCompilerFlags::None;

		/*! @brief �V�F�[�_�[�o�[�W����*/
		gu::float32 Version = NEWEST_VERSION;

		/*! @brief �f�o�b�O���e���o�͂���f�B���N�g��*/
		gu::tstring DebugOutputDirectory = SP("");


		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		
		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor

		#pragma endregion 

		#pragma region Protected Function

		#pragma endregion 

		#pragma region Protected Property

		#pragma endregion

	private:
		#pragma region Private Constructor and Destructor

		#pragma endregion

		#pragma region Private Function

		#pragma endregion 

		#pragma region Private Property

		#pragma endregion 

	};
}

#endif