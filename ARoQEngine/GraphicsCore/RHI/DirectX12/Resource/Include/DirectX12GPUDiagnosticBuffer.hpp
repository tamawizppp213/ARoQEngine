//////////////////////////////////////////////////////////////////////////////////
///  @file   DirectX12GPUDiagnosticBuffer.hpp
///  @brief  �R�}���h�L���[��GPU�̐i���󋵂��g���b�L���O�����p�o�b�t�@ (DirectX12�݂̂Ŏg�p���܂�.)
///  @author toide
///  @date   2024/04/06 19:05:15
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_GPU_DIAGNOSTIC_BUFFER_HPP
#define DIRECTX12_GPU_DIAGNOSTIC_BUFFER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "DirectX12GPUBuffer.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace rhi::directX12
{
	/****************************************************************************
	*                GPUDiagnosticBuffer
	*************************************************************************//**
	/*  @class     GPUDiagnosticBuffer
	*   @brief     �R�}���h�L���[��GPU�̐i���󋵂��g���b�L���O�����p�o�b�t�@ (DirectX12�݂̂Ŏg�p���܂�.)
	*****************************************************************************/
	class GPUDiagnosticBuffer : public GPUBuffer
	{
	public:
		#pragma region Public Function

		#pragma endregion 

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief  �R���e�L�X�g�̃o�C�g�T�C�Y���擾���܂�
		*************************************************************************/
		__forceinline gu::uint32 GetBreadCrumbsContextByteSize() const { return _breadCrumbsContextByteSize; }

		/*!**********************************************************************
		*  @brief  �p���������X�g��Offset [Byte]���擾���܂�
		*************************************************************************/
		__forceinline gu::uint32 GetBreadCrumbsOffset() const { return _breadCrumbsOffset; }

		/*!**********************************************************************
		*  @brief  �p���������X�g���̂̃o�C�g�T�C�Y���擾���܂�
		*************************************************************************/
		__forceinline gu::uint32 GetBreadCrumbsByteSize() const { return _breadCrumbsByteSize; }

		/*!**********************************************************************
		*  @brief  �f�f����Offset [Byte]���擾���܂�
		*************************************************************************/
		__forceinline gu::uint32 GetDiagnosticOffset() const { return _diagnosticOffset; }

		/*!**********************************************************************
		*  @brief  �f�f��񎩑̂̃o�C�g�T�C�Y���擾���܂�
		*************************************************************************/
		__forceinline gu::uint32 GetDiagnosticByteSize() const { return _diagnosticByteSize; }

		/*!**********************************************************************
		*  @brief  �R���e�L�X�g�̃o�C�g�T�C�Y��ݒ肵�܂�
		*************************************************************************/
		__forceinline void SetBreadCrumbsContextByteSize(const gu::uint32 context) { _breadCrumbsContextByteSize = context; }

		/*!**********************************************************************
		*  @brief  �p���������X�g��Offset [Byte]��ݒ肵�܂�
		*************************************************************************/
		__forceinline void SetBreadCrumbsOffset(const gu::uint32 offset) { _breadCrumbsOffset = offset; }

		/*!**********************************************************************
		*  @brief  �p���������X�g���̂̃o�C�g�T�C�Y���擾���܂�
		*************************************************************************/
		__forceinline void SetBreadCrumbsByteSize(const gu::uint32 byteSize) { _breadCrumbsByteSize = byteSize; }

		/*!**********************************************************************
		*  @brief  �f�f����Offset [Byte]��ݒ肵�܂�
		*************************************************************************/
		__forceinline void SetDiagnosticOffset(const gu::uint32 offset) { _diagnosticOffset = offset; }

		/*!**********************************************************************
		*  @brief  �f�f��񎩑̂̃o�C�g�T�C�Y��ݒ肵�܂�
		*************************************************************************/
		__forceinline void SetDiagnosticByteSize(const gu::uint32 byteSize) { _diagnosticByteSize = byteSize; }
		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		GPUDiagnosticBuffer();

		/*! @brief �f�X�g���N�^*/
		~GPUDiagnosticBuffer();

		/*! @brief �f�o�C�X�ƃo�b�t�@�������Ƃɍ쐬�����o�b�t�@*/
		GPUDiagnosticBuffer(const gu::SharedPointer<core::RHIDevice>& device, const core::GPUBufferMetaData& metaData, const gu::tstring& name);
		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor

		#pragma endregion 

		#pragma region Protected Function

		#pragma endregion 

		#pragma region Protected Property
		/*! @brief �R���e�L�X�g�̃o�C�g�T�C�Y*/
		gu::uint32 _breadCrumbsContextByteSize = 0;

		/*! @brief �p���������X�g��Offset [Byte]*/
		gu::uint32 _breadCrumbsOffset = 0;

		/*! @brief �p���������X�g���̂�ByteSize*/
		gu::uint32 _breadCrumbsByteSize = 0;

		/*! @brief �f�f����Offset byte*/
		gu::uint32 _diagnosticOffset = 0;

		/*! @brief �f�f��񎩑̂�BytreSize*/
		gu::uint32 _diagnosticByteSize = 0;
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