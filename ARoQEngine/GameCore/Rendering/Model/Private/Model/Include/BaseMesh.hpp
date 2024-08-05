//////////////////////////////////////////////////////////////////////////////////
///  @file   BaseMesh.hpp
///  @brief  ���b�V���̊��N���X�ł�. GPU�o�b�t�@�̍쐬, ���b�V������肷�邽�߂̋��ʏ�����񋟂��܂�.
///  @author toide
///  @date   2024/07/27 22:46:33
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef BASE_MESH_HPP
#define BASE_MESH_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHITypeCore.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
#include "GameUtility/Base/Include/GUString.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class LowLevelGraphicsEngine;

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace engine
{
	/****************************************************************************
	*				  	BaseMeshSetting
	****************************************************************************/
	/* @brief  BaseMesh���쐬����ۂ̐ݒ�
	*****************************************************************************/
	struct BaseMeshSetting
	{
		/*! @brief VB��CPU�A�N�Z�X�\�� (�f�t�H���g��false). ���̃t���O�ɂ��, ���I��CPU�����璸�_�f�[�^��ύX���邱�Ƃ��o���܂�.*/
		bool IsCPUAccessibleVB = false;

		/*! @brief IB��CPU�A�N�Z�X�\�� (�f�t�H���g��false). ���̃t���O�ɂ��, ���I��CPU������C���f�b�N�X�f�[�^��ύX���邱�Ƃ��o���܂�.*/
		bool IsCPUAccessibleIB = false;

		/*! @brief 32bit�̃C���f�b�N�X�o�b�t�@��*/
		bool Is32BitIndex = false;
	};

	/****************************************************************************
	*				  			   BaseMesh
	****************************************************************************/
	/* @brief  ���b�V���̊��N���X�ł�. GPU�o�b�t�@�̍쐬, ���b�V������肷�邽�߂̋��ʏ�����񋟂��܂�.
	*****************************************************************************/
	class IBaseMesh : public gu::NonCopyable
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     �`����s���܂�. 
		*  @param[in] const RHICommandListPtr& �R�}���h���X�g
		*  @return    void
		*************************************************************************/
		virtual void Draw(const RHICommandListPtr& commandList) = 0;

		#pragma endregion 

		#pragma region Public Property

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		IBaseMesh() = default;

		/*! @brief �f�X�g���N�^*/
		virtual ~IBaseMesh() = default;

		IBaseMesh(const gu::SharedPointer<LowLevelGraphicsEngine>& engine, const BaseMeshSetting& setting, const gu::tstring& name);

		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor

		#pragma endregion 

		#pragma region Protected Function
		/*!**********************************************************************
		*  @brief     ���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���쐬���܂�.
		*  @param[in] const gu::uint32 VertexBuffer��Stride (1���_�ɕK�v�ȃo�C�g�T�C�Y)
		*  @param[in] const gu::uint32 VertexBuffer�̒��_��
		*  @param[in] const gu::uint32 IndexBuffer�̌�
		*  @param[in] const void* VertexBuffer�ɏ������ݎn�߂�擪�|�C���^
		*  @param[in] const void* IndexBuffer�������ݎn�߂�擪�|�C���^
		*  @return    void
		*************************************************************************/
		void SetupVertexBufferAndIndexBuffer(const gu::uint32 vbStride, const gu::uint32 vbCount, const gu::uint32 ibCount, 
			const void* vbData, const void* ibData);

		/*!**********************************************************************
		*  @brief     ���_�o�b�t�@���擾���܂�. CPU�A�N�Z�X�\�̏ꍇ�͌��������݃t���[���̃o�b�t�@��Ԃ��܂�.
		*  @param[in] void
		*  @return    GPUBufferPtr
		*************************************************************************/
		GPUBufferPtr GetVertexBuffer() const noexcept;

		/*!**********************************************************************
		*  @brief     �C���f�b�N�X�o�b�t�@���擾���܂�. CPU�A�N�Z�X�\�̏ꍇ�͌��������݃t���[���̃o�b�t�@��Ԃ��܂�.
		*  @param[in] void
		*  @return    GPUBufferPtr
		*************************************************************************/
		GPUBufferPtr GetIndexBuffer() const noexcept;
		#pragma endregion 

		#pragma region Protected Property

		/*! @brief �`��p�̃O���t�B�N�X�G���W��*/
		gu::SharedPointer<LowLevelGraphicsEngine> _engine = nullptr;

		/*! @brief ���O*/
		gu::tstring _name = SP("");

		/*! @brief ���_�o�b�t�@. CPU���A�N�Z�X�\�̏ꍇ�̓t���[����GPU�o�b�t�@������܂�*/
		gu::DynamicArray<GPUBufferPtr> _vertexBuffer = {};

		/*! @brief �C���f�b�N�X�o�b�t�@. CPU���A�N�Z�X�\�̏ꍇ��, �t���[����GPU�o�b�t�@������܂�.*/
		gu::DynamicArray<GPUBufferPtr> _indexBuffer = {};

		/*! @brief �ݒ�*/
		BaseMeshSetting _baseMeshSetting = {};

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