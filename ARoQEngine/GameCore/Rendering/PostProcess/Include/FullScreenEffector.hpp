//////////////////////////////////////////////////////////////////////////////////
///  @file   FullScreenEffector.hpp
///  @brief  ���̃N���X�͎�Ƀ|�X�g�E�G�t�F�N�g�̂��߂Ɏg���� @n
///          �t���X�N���[���̃|���S����`�悷�邽�߂̃C���^�[�t�F�[�X.
///  @author Toide Yutaro
///  @date   2024_07_15
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef FULL_SCREEN_EFFECTOR_HPP
#define FULL_SCREEN_EFFECTOR_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
#include "GameUtility/Base/Include/GUString.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class LowLevelGraphicsEngine;
namespace rhi::core
{
	class RHIResourceLayout;
	class GPUResourceView;
	class GPUGraphicsPipelineState;
	class GPUBuffer;
	class GPUTexture;
}
//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace engine
{
	/****************************************************************************
	*				  			IFullScreenEffector
	****************************************************************************/
	/* @brief  �t���X�N���[���̃|���S����`�悷�邽�߂̃C���^�[�t�F�[�X. 
	*****************************************************************************/
	class IFullScreenEffector : public gu::NonCopyable
	{
	protected:
		using VertexBufferPtr           = gu::SharedPointer<rhi::core::GPUBuffer>;
		using IndexBufferPtr            = gu::SharedPointer<rhi::core::GPUBuffer>;
		using ResourceLayoutPtr         = gu::SharedPointer<rhi::core::RHIResourceLayout>;
		using ResourceViewPtr           = gu::SharedPointer<rhi::core::GPUResourceView>;
		using PipelineStatePtr          = gu::SharedPointer<rhi::core::GPUGraphicsPipelineState>;
		using LowLevelGraphicsEnginePtr = gu::SharedPointer<LowLevelGraphicsEngine>;

	public:
		#pragma region Public Function

		/*!**********************************************************************
		*  @brief     ��ʂ̃T�C�Y��ύX���܂�. 
		*  @param[in] const gu::uint32 �V������
		*  @param[in] const gu::uint32 �V��������
		*  @return    void
		*************************************************************************/
		virtual void OnResize(const gu::uint32 newWidth, const gu::uint32 newHeight) = 0;

		/*!**********************************************************************
		*  @brief     �R�}���h���X�g�ɕ`����e��o�^���܂�
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		virtual void Draw() = 0;

		#pragma endregion

	    #pragma region Public Property

		#pragma endregion

	protected:
		#pragma region Protected Constructor and Destructor 

		/*! @brief �f�t�H���g�R���X�g���N�^*/
		IFullScreenEffector();

		/*! @brief �f�X�g���N�^*/
		virtual ~IFullScreenEffector();

		/*! @brief �O���t�B�N�X�G���W�����g���ď�����*/
		IFullScreenEffector(const LowLevelGraphicsEnginePtr& engine);

		#pragma endregion

		#pragma region Protected Function
		/*!**********************************************************************
		*  @brief     Resize�\���𔻒肵�܂�
		*  @param[in] const gu::uint32 ��
		*  @param[in] const gu::uint32 ����
		*  @return    bool
		*************************************************************************/
		__forceinline bool EnableResize(const gu::uint32 newWidth, const gu::uint32 newHeight) const 
		{
			return newWidth != _width || newHeight != _height;
		}

		/*!**********************************************************************
		*  @brief     �p�C�v���C���X�e�[�g���������܂�.
		*  @param[in] const gu::tstring& �f�o�b�O�\����
		*  @return    void
		*************************************************************************/
		virtual void PreparePipelineState(const gu::tstring& addName) = 0;

		virtual void PrepareResourceView() = 0;
		
		void PrepareVertexAndIndexBuffer(const gu::tstring& addName);
		
		/*!**********************************************************************
		*  @brief     �f�o�b�O�\�������������܂�. ������, �f�t�H���g�l��FSEffector::���ǉ�����܂�.
		*  @param[in] const gu::tstring& �f�o�b�O�\����
		*  @return    void
		*************************************************************************/
		gu::tstring DefineDebugName(const gu::tstring& addName = SP(""))
		{
			gu::tstring name = SP(""); if (addName != SP("")) { name += addName; name += SP("::"); }
			name += SP("FSEffector::"); // ��Ɋe���\�[�X���̂��ǉ�����܂�.
			return name;
		}

		#pragma endregion

		#pragma region Protected Property

		/* @brief : frame resources*/
		gu::DynamicArray<VertexBufferPtr> _vertexBuffers = {};
		
		gu::DynamicArray<IndexBufferPtr>  _indexBuffers = {};
		
		PipelineStatePtr  _pipeline       = nullptr;
		
		ResourceLayoutPtr _resourceLayout = nullptr;
		
		gu::DynamicArray<ResourceViewPtr> _resourceViews = {};

		gu::uint32 _width  = 0;
		gu::uint32 _height = 0;

		/* @brief : device and command list*/
		LowLevelGraphicsEnginePtr _engine = nullptr;

		#pragma endregion
	};
}
#endif