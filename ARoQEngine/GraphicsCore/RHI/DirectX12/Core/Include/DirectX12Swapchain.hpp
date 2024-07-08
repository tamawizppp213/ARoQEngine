//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12Swapchain.hpp
///             @brief  Update frame buffer image
///             @author Toide Yutaro
///             @date   2022_06_24
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_SWAP_CHAIN_HPP
#define DIRECTX12_SWAP_CHAIN_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHISwapchain.hpp"
#include "DirectX12Core.hpp"
#include <dxgi1_6.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::directX12
{

	/****************************************************************************
	*				  			RHISwapchain
	****************************************************************************/
	/* @brief Update frame buffer image
	*****************************************************************************/
	class RHISwapchain : public  rhi::core::RHISwapchain
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     NextImage�������o�����Ƃ�, Signal�𔭍s���Ď��̃t���[���C���f�b�N�X��Ԃ��܂�
		*  @param[in] const gu::SharedPointer<core::RHIFence>& fence : �t�F���X
		*  @param[in] const gu::uint64 signalValue : �����������Ƀt�F���X�ɏ������܂��V�O�i���l
		*  @return    gu::uint32 ���̃t���[���C���f�b�N�X
		*************************************************************************/
		virtual gu::uint32 PrepareNextImage(const gu::SharedPointer<core::RHIFence>& fence, const gu::uint64 signalValue) override;
		
		/*!**********************************************************************
		*  @brief     �o�b�N�o�b�t�@�����t���[���̃t�����g�o�b�t�@�ɂ��܂� 
		*  @param[in] const gu::SharedPointer<core::RHIFence>& fence : �t�F���X
		*  @param[in] const gu::uint64 waitValue : �ҋ@����l
		*  @return    void
		*************************************************************************/
		virtual void Present(const gu::SharedPointer<core::RHIFence>& fence, gu::uint64 waitValue) override ;
		
		/*!**********************************************************************
		*  @brief     ��ʃT�C�Y��ύX���܂�. ��x���ׂĂ��č\�z���A�ēx�A�b�v�f�[�g���܂�. @n
		* �@�@�@�@�@�@�@ ���O�ɃR�}���h�̎��s���I�����Ă��邱�Ƃ��m�F���Ă�������.
		*  @param[in] const gu::uint32 width : ��ʂ̕�
		*  @param[in] const gu::uint32 height : ��ʂ̍���
		*  @return    void
		*************************************************************************/
		virtual void Resize(const gu::uint32 width, const gu::uint32 height) override;

		/*!**********************************************************************
		*  @brief     �t���X�N���[�����[�h�Ǝw�肳�ꂽ�𑜓x�̃X�N���[�����[�h��؂�ւ���
		* �@�@�@�@�@�@  (isOn : true->�t���X�N���[�����[�h�Ɉڍs����. false : window���[�h�Ɉڍs����)
		*  @param[in] const bool isOn : true->�t���X�N���[�����[�h�Ɉڍs����. false : window���[�h�Ɉڍs����
		*  @return    void
		*************************************************************************/
		virtual void SwitchFullScreenMode(const bool isOn) override;
		
		/*!**********************************************************************
		*  @brief     HDR���[�h��SDR���[�h��؂�ւ���.
		*  @param[in] const bool enableHDR : true -> HDR, false -> SDR
		*  @return    void
		*************************************************************************/
		virtual void SwitchHDRMode(const bool enableHDR) override;

		/*!**********************************************************************
		*  @brief     ���݂̃f�B�X�v���C�o�͂�HDR�@�\���T�|�[�g���Ă��邩�𒲂ׂ�
		*  @param[in] void
		*  @return    bool HDR���T�|�[�g����Ă��邩
		*************************************************************************/
		bool IsSupportedHDRInCurrentDisplayOutput();

		/*!**********************************************************************
		*  @brief     HDR��ł̃��j�^�[�ݒ���o�͏�ɕ\�����܂�. 
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void LogHDROutput() const;

		#pragma endregion 

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     ���ݕ`��R�}���h���l�ߍ��݂����o�b�N�o�b�t�@�̃C���f�b�N�X��Ԃ��܂�
		*  @param[in] void
		*  @return    gu::uint32 �X���b�v�`�F�C���̕`�悷�镝
		*************************************************************************/
		virtual gu::uint8 GetCurrentBufferIndex() const override;

		/*!**********************************************************************
		*  @brief     DirectX12�Ŏg�p����Swapchain��Comptr��Ԃ��܂�.
		*  @param[in] void
		*  @return    SwapchainComPtr Swapchain��Com�|�C���^
		*************************************************************************/
		SwapchainComPtr GetSwapchain() const noexcept { return _swapchain; }

		#pragma endregion
		
		#pragma region Public Constructor and Destructor

		/*! @brief �f�X�g���N�^*/
		~RHISwapchain();
		
		/*! @brief ���������*/
		explicit RHISwapchain(
			const gu::SharedPointer<rhi::core::RHIDevice>& device,
			const gu::SharedPointer<rhi::core::RHICommandQueue>& queue,
			const rhi::core::WindowInfo& windowInfo,
			const rhi::core::PixelFormat& piexlFormat,
			const gu::uint8 frameBufferCount = 3, const gu::uint8 vsync = 0,
			const bool isValidHDR = true,
			const bool isFullScreen = false
			);

		/*! @brief Descriptor�ł܂Ƃ߂ď�����*/
		explicit RHISwapchain(
			const gu::SharedPointer<rhi::core::RHIDevice>& device,
			const core::SwapchainDesc& desc);

		#pragma endregion

	protected:
		#pragma region Protected Constructor and Destructor
		#pragma endregion

		#pragma region Protected Function
		/*!**********************************************************************
		*  @brief     Swapchain�̏�����
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void SetUp();

		#pragma endregion

		#pragma region Protected Property
		// @brief : swapchain
		SwapchainComPtr      _swapchain = nullptr;

		// @brief : swapchain�̐ݒ�
		DXGI_SWAP_CHAIN_FLAG _swapchainFlag;

		// @brief : �s�N�Z���̐F�t�H�[�}�b�g
		DXGI_FORMAT          _backBufferFormat; // color format

		// @brief : �F���
		DXGI_COLOR_SPACE_TYPE _colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709;

		/*----------------------------------------------------------------------
		*  @brief : VSync��0�̂Ƃ�����, AllowTearing���T�|�[�g����Ă���Ȃ�,�@
		*           �X���b�v�̏������o����܂ő҂悤�ɂ��܂�.
		*----------------------------------------------------------------------*/
		bool _useAllowTearing = false;

		/*! @brief �t���X�N���[����Ԃ��������ꂽ��*/
		bool _isFullScreenLost = false;

	#if DXGI_MAX_SWAPCHAIN_INTERFACE < 3
		gu::uint32 _currentBackBufferIndex = 0;
	#endif
		#pragma endregion Protected Property

	private:
		#pragma region Private Function
		/*!**********************************************************************
		*  @brief     �s�N�Z���t�H�[�}�b�g�ɏ]���ĐF��Ԃ�ݒ肵�܂�.
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void EnsureSwapChainColorSpace();
		/*!**********************************************************************
		*  @brief     �蓮�ŐF��Ԃ�ݒ肵�܂�.
		*  @param[in] const core::DisplayColorGamut colorGamut : �F���
		*  @param[in] const core::DisplayOutputFormat displayFormat : �o�̓t�H�[�}�b�g
		*  @return    void
		*************************************************************************/
		void EnsureSwapChainColorSpace(const core::DisplayColorGamut colorGamut, const core::DisplayOutputFormat displayFormat);

		void SetHDRMetaData();
		#pragma endregion
	};
}
#endif