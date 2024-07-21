//////////////////////////////////////////////////////////////////////////////////
///  @file   RHIFrameBuffer.hpp
///  @brief  �����_�[�^�[�Q�b�g�ƃf�v�X�X�e���V���̃e�N�X�`����ResourceView���g�p����N���X�ł�.
///  @author Toide Yutaro
///  @date   2024_07_21
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_FRAME_BUFFER_HPP
#define RHI_FRAME_BUFFER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////

namespace rhi::core
{
	class RHIDevice;
	class GPUTexture;
	class RHIRenderPass;
	class GPUResourceView;
	/****************************************************************************
	*				  			RHIFrameBuffer
	****************************************************************************/
	/* @brief   �����_�[�^�[�Q�b�g�ƃf�v�X�X�e���V���̃e�N�X�`����ResourceView���g�p����N���X�ł�.
	*****************************************************************************/
	class RHIFrameBuffer : public gu::NonCopyable
	{
	protected:
		using TexturePtr      = gu::SharedPointer<GPUTexture>;
		using ResourceViewPtr = gu::SharedPointer<GPUResourceView>;
	public:
		#pragma region Public Function
		#pragma endregion

		#pragma region Public Property

		/*!**********************************************************************
		*  @brief     �����_�[�^�[�Q�b�g�̕`�文�����w�肵�܂�. 
		*  @param[in] void
		*  @return    gu::uint32 �����_�[�^�[�Q�b�g�̖���
		*************************************************************************/
		__forceinline gu::uint32 GetRenderTargetSize() const { return static_cast<gu::uint32>(_renderTargets.Size()); };

		/*!**********************************************************************
		*  @brief     �����_�[�^�[�Q�b�g�̃|�C���^��Ԃ��܂�.
		*  @param[in] const gu::uint32 �����_�[�^�[�Q�b�g�̃C���f�b�N�X
		*  @return    TexturePtr �����_�[�^�[�Q�b�g�̃|�C���^
		*************************************************************************/
		__forceinline TexturePtr GetRenderTarget(const gu::uint32 index = 0) const { return _renderTargets[index]; };

		/*!**********************************************************************
		*  @brief     �S�Ẵ����_�[�^�[�Q�b�g�z���Ԃ��܂�.
		*  @param[in] void
		*  @return    gu::DynamicArray<TexturePtr>& �����_�[�^�[�Q�b�g�̔z��
		*************************************************************************/
		__forceinline const gu::DynamicArray<TexturePtr>& GetRenderTargets() const { return _renderTargets; }

		/*!**********************************************************************
		*  @brief     DepthStencil�̃|�C���^��Ԃ��܂�.
		*  @param[in] void
		*  @return    TexturePtr DepthStencil�̃|�C���^
		*************************************************************************/
		__forceinline TexturePtr GetDepthStencil() const noexcept { return _depthStencil; };

		/*!**********************************************************************
		*  @brief     �����_�[�^�[�Q�b�g���ݒ肵�Ă���r���[�|�[�g���擾���܂�. 
		*  @param[in] const gu::uint32 index �����_�[�^�[�Q�b�g�̃C���f�b�N�X
		*  @return    Viewport �r���[�|�[�g
		*************************************************************************/
		Viewport    GetFullViewport(const gu::uint32 index = 0) const noexcept;

		/*!**********************************************************************
		*  @brief     �����_�[�^�[�Q�b�g���ݒ肵�Ă���V�U�[��`���擾���܂�.
		*  @param[in] const gu::uint32 index �����_�[�^�[�Q�b�g�̃C���f�b�N�X
		*  @return    ScissorRect �V�U�[��`
		*************************************************************************/
		ScissorRect GetFullScissorRect(const gu::uint32 index = 0) const noexcept;

		/*!**********************************************************************
		*  @brief     �����_�[�^�[�Q�b�g���ݒ肵�Ă���Render Target View�z����擾���܂�.
		*  @param[in] void
		*  @return    const gu::DynamicArray<ResourceViewPtr>& RTV�z��
		*************************************************************************/
		__forceinline const gu::DynamicArray<ResourceViewPtr>& GetRenderTargetViews() const { return _renderTargetViews; }

		/*!**********************************************************************
		*  @brief     �����_�[�^�[�Q�b�g���ݒ肵�Ă���Render Target View���擾���܂�.
		*  @param[in] void
		*  @return    const gu::DynamicArray<ResourceViewPtr>& RTV�z��
		*************************************************************************/
		__forceinline ResourceViewPtr GetRenderTargetView(const gu::uint32 index = 0) const { return _renderTargetViews[index]; }

		/*!**********************************************************************
		*  @brief     �����_�[�^�[�Q�b�g���ݒ肵�Ă���Shader Resource View (�e�N�X�`���Ƃ��Ďg�p����ꍇ�ȂǂɎ擾)���擾���܂�.
		*  @param[in] const gu::uint32 index �����_�[�^�[�Q�b�g�̃C���f�b�N�X
		*  @return    ResourceViewPtr Shader Resource View
		*************************************************************************/
		__forceinline ResourceViewPtr GetRenderTargetSRV(const gu::uint32 index = 0) const noexcept { return _renderTargetSRVs[index]; }

		/*!**********************************************************************
		*  @brief     �����_�[�^�[�Q�b�g���ݒ肵�Ă���Unordered Access View (�e�N�X�`���̓ǂݏ������s���ꍇ�ȂǂɎ擾)���擾���܂�.
		*  @param[in] const gu::uint32 index �����_�[�^�[�Q�b�g�̃C���f�b�N�X
		*  @return    ResourceViewPtr Unordered Access View
		*************************************************************************/
		__forceinline ResourceViewPtr GetRenderTargetUAV(const gu::uint32 index = 0) const noexcept { return _renderTargetUAVs[index]; }

		/*!**********************************************************************
		*  @brief     �f�v�X�X�e���V�����ݒ肵�Ă���DSV���擾���܂�.
		*  @param[in] void
		*  @return    ResourceViewPtr DSV
		*************************************************************************/
		__forceinline ResourceViewPtr GetDepthStencilView() const noexcept { return _depthStencilView; }

		/*!**********************************************************************
		*  @brief     �f�v�X�X�e���V�����ݒ肵�Ă���Shader Resource View���擾���܂�.
		*  @param[in] void
		*  @return    ResourceViewPtr SRV
		*************************************************************************/
		__forceinline ResourceViewPtr GetDepthStencilSRV() const noexcept { return _depthStencilSRV; }

		/*!**********************************************************************
		*  @brief     RenderTarget��ݒ肵�܂�. 
		*  @param[in] const gu::DynamicArray<TexturePtr>& textures �����_�[�^�[�Q�b�g�̔z��
		*  @return    void
		*************************************************************************/
		void SetRenderTargets(const gu::DynamicArray<TexturePtr>& textures);

		/*!**********************************************************************
		*  @brief     RenderTarget��ݒ肵�܂�.
		*  @param[in] const TexturePtr& texture �����_�[�^�[�Q�b�g
		*  @param[in] const gu::uint32 index �����_�[�^�[�Q�b�g�̃C���f�b�N�X
		*  @return    void
		*************************************************************************/
		void SetRenderTarget(const TexturePtr& texture, const gu::uint32 index = 0);

		/*!**********************************************************************
		*  @brief     �f�v�X�X�e���V����ݒ肵�܂�.
		*  @param[in] const TexturePtr& texture �f�v�X�X�e���V��
		*  @return    void
		*************************************************************************/
		void SetDepthStencil(const TexturePtr& texture);

		#pragma endregion

		#pragma region Public Constructor and Destructor
		#pragma endregion


	protected:
		#pragma region Protected Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		RHIFrameBuffer() = default;

		/*! @brief �f�X�g���N�^*/
		virtual ~RHIFrameBuffer();

		/*! @brief �_���f�o�C�X, �����_�[�p�X, �����̃����_�[�^�[�Q�b�g, �f�v�X�X�e���V�����g���ď�����*/
		explicit RHIFrameBuffer(const gu::SharedPointer<RHIDevice>& device, const gu::SharedPointer<core::RHIRenderPass>& renderPass, 
			const gu::DynamicArray<gu::SharedPointer<GPUTexture>>& renderTargets, const gu::SharedPointer<GPUTexture>& depthStencil = nullptr);

		/*! @brief �_���f�o�C�X, �����_�[�p�X, �����_�[�^�[�Q�b�g, �f�v�X�X�e���V�����g���ď�����*/
		explicit RHIFrameBuffer(const gu::SharedPointer<RHIDevice>& device, const gu::SharedPointer<core::RHIRenderPass>& renderPass, 
			const gu::SharedPointer<GPUTexture>& renderTarget, const gu::SharedPointer<GPUTexture>& depthStencil = nullptr);
		#pragma endregion 

		#pragma region Protected Function
		#pragma endregion

		#pragma region Protected Property
		/*! @brief �_���f�o�C�X*/
		gu::SharedPointer<RHIDevice>     _device     = nullptr;

		/*! @brief �����_�[�p�X*/
		gu::SharedPointer<RHIRenderPass> _renderPass = nullptr;

		/*! @brief �����_�[�^�[�Q�b�g*/
		gu::DynamicArray<TexturePtr>      _renderTargets     = {nullptr};

		/*! @brief �����_�[�^�[�Q�b�g�r���[*/
		gu::DynamicArray<ResourceViewPtr> _renderTargetViews = {nullptr};

		/*! @brief �����_�[�^�[�Q�b�g�V�F�[�_�[���\�[�X�r���[*/
		gu::DynamicArray<ResourceViewPtr> _renderTargetSRVs = { nullptr };

		/*! @brief �����_�[�^�[�Q�b�g�A���I�[�_�[�A�N�Z�X�r���[*/
		gu::DynamicArray<ResourceViewPtr> _renderTargetUAVs = { nullptr };

		/*! @brief �f�v�X�X�e���V��*/
		TexturePtr      _depthStencil     = nullptr;

		/*! @brief �f�v�X�X�e���V���r���[*/
		ResourceViewPtr _depthStencilView = nullptr;

		/*! @brief �f�v�X�X�e���V���V�F�[�_�[���\�[�X�r���[*/
		ResourceViewPtr _depthStencilSRV  = nullptr;
		#pragma endregion

	private:
		#pragma region Private Function
		void CheckResourceFormat();
		#pragma endregion
	};
}
#endif