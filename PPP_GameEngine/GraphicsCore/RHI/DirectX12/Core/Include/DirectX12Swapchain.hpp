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
	*************************************************************************//**
	*  @class     RHISwapchain
	*  @brief     Update frame buffer image
	*****************************************************************************/
	class RHISwapchain : public  rhi::core::RHISwapchain
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief :  NextImage�������o�����Ƃ�, Signal�𔭍s���Ď��̃t���[���C���f�b�N�X��Ԃ��܂�
		/*----------------------------------------------------------------------*/
		gu::uint32 PrepareNextImage(const gu::SharedPointer<core::RHIFence>& fence, const gu::uint64 signalValue) override;
		
		/*----------------------------------------------------------------------
		*  @brief :  �t�����g�o�b�t�@��\�����܂�. 
		/*----------------------------------------------------------------------*/
		void Present(const gu::SharedPointer<core::RHIFence>& fence, std::uint64_t waitValue) override ;
		
		/*----------------------------------------------------------------------
		*  @brief :  ��ʃT�C�Y��ύX����B��x���ׂĂ��č\�z���A�ēx�A�b�v�f�[�g����B
		/*----------------------------------------------------------------------*/
		void Resize(const size_t width, const size_t height) override ;
		
		/* @brief : Return current frame buffer*/
		size_t GetCurrentBufferIndex() const override;

		/*----------------------------------------------------------------------
		*  @brief :  �t���X�N���[�����[�h�Ǝw�肳�ꂽ�𑜓x�̃X�N���[�����[�h��؂�ւ���
		* �@�@�@�@�@�@  (isOn : true->�t���X�N���[�����[�h�Ɉڍs����. false : window���[�h�Ɉڍs����)
		/*----------------------------------------------------------------------*/       
		void SwitchFullScreenMode(const bool isOn) override;
		
		/*----------------------------------------------------------------------
		*  @brief :  HDR���[�h��SDR���[�h��؂�ւ���
		/*----------------------------------------------------------------------*/
		void SwitchHDRMode(const bool enableHDR) override;

		/*----------------------------------------------------------------------
		*  @brief :  ���݂̃f�B�X�v���C�o�͂�HDR�@�\���T�|�[�g���Ă��邩�𒲂ׂ�
		/*----------------------------------------------------------------------*/
		bool IsSupportedHDRInCurrentDisplayOutput();

		/*----------------------------------------------------------------------
		*  @brief :  HDR��ł̃��j�^�[�ݒ���o�͏�ɕ\�����܂�.
		/*----------------------------------------------------------------------*/
		void LogHDROutput();

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		SwapchainComPtr GetSwapchain() const noexcept { return _swapchain; }
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		~RHISwapchain();
		
		explicit RHISwapchain(
			const gu::SharedPointer<rhi::core::RHIDevice>& device,
			const gu::SharedPointer<rhi::core::RHICommandQueue>& queue,
			const rhi::core::WindowInfo& windowInfo,
			const rhi::core::PixelFormat& piexlFormat,
			const size_t frameBufferCount = 3, const std::uint32_t vsync = 0,
			const bool isValidHDR = true,
			const bool isFullScreen = false
			);

		explicit RHISwapchain(
			const gu::SharedPointer<rhi::core::RHIDevice>& device,
			const core::SwapchainDesc& desc);
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void SetUp();

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
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
		/*----------------------------------------------------------------------*/
		bool _useAllowTearing = false;
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		void EnsureSwapChainColorSpace();
		void EnsureSwapChainColorSpace(const core::DisplayColorGamut colorGamut, const core::DisplayOutputFormat displayFormat);

		void SetHDRMetaData();
	};
}
#endif