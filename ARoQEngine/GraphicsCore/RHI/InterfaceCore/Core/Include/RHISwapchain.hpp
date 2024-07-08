//////////////////////////////////////////////////////////////////////////////////
///             @file   RHISwapchain.hpp
///             @brief  Update frame buffer image
///             @author Toide Yutaro
///             @date   2022_06_24
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_SWAP_CHAIN_HPP
#define RHI_SWAP_CHAIN_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class RHIDevice;
	class RHICommandQueue;
	class RHIFence;
	class GPUTexture;

	/****************************************************************************
	*				  			SwapchainDesc
	****************************************************************************/
	/* @brief Swapchain�𐶐�����ۂ̏����ݒ�
	*****************************************************************************/
	struct SwapchainDesc
	{
	protected:
		using CommandQueuePtr = gu::SharedPointer<RHICommandQueue>;

	public:
		/*! @brief �R�}���h�����s����L���[. �t�����g�o�b�t�@�ɕ\������ۂɎg�p���܂�.*/
		CommandQueuePtr CommandQueue = nullptr;

		/*! @brief �����t���[���ɂ�����E�B���h�E���*/
		WindowInfo WindowInfo = {};

		/*! @brief �`��Ŏg�p����s�N�Z���t�H�[�}�b�g*/
		PixelFormat PixelFormat = PixelFormat::Unknown; 

		/*! @brief �t���[���o�b�t�@�̐�*/
		gu::uint8 FrameBufferCount = 3;

		/*! @brief VSync�̐ݒ� (0: VSync������, 1: VSync�L��, �ȍ~fps�������ɂȂ�܂�.)*/
		gu::uint8 VSync = 0;

		/*! @brief HDR�̗L����*/
		bool IsValidHDR = true;

		/*! @brief ���̎��̎g�p*/
		bool IsValidStereo = false;

		/*! @brief �t���X�N���[���̎g�p*/
		bool IsFullScreen = false;

		~SwapchainDesc()
		{
			if (CommandQueue) { CommandQueue.Reset(); }
		}
	};

	/****************************************************************************
	*				  			RHISwapchain
	****************************************************************************/
	/* @brief     Update frame buffer image
	*****************************************************************************/
	class RHISwapchain : public gu::NonCopyable
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     NextImage�������o�����Ƃ�, Signal�𔭍s���Ď��̃t���[���C���f�b�N�X��Ԃ��܂�
		*  @param[in] const gu::SharedPointer<core::RHIFence>& fence : �t�F���X
		*  @param[in] const gu::uint64 signalValue : �����������Ƀt�F���X�ɏ������܂��V�O�i���l
		*  @return    gu::uint32 ���̃t���[���C���f�b�N�X
		*************************************************************************/
		virtual gu::uint32 PrepareNextImage(const gu::SharedPointer<RHIFence>& fence, const gu::uint64 signalValue) = 0;
		
		/*!**********************************************************************
		*  @brief     �o�b�N�o�b�t�@�����t���[���̃t�����g�o�b�t�@�ɂ��܂�
		*  @param[in] const gu::SharedPointer<core::RHIFence>& fence : �t�F���X
		*  @param[in] const gu::uint64 waitValue : �ҋ@����l
		*  @return    void
		*************************************************************************/
		virtual void Present(const gu::SharedPointer<RHIFence>& fence, gu::uint64 waitValue) = 0;
		
		/*!**********************************************************************
		*  @brief     ��ʃT�C�Y��ύX���܂�. ��x���ׂĂ��č\�z���A�ēx�A�b�v�f�[�g���܂�. @n
		* �@�@�@�@�@�@�@ ���O�ɃR�}���h�̎��s���I�����Ă��邱�Ƃ��m�F���Ă�������.
		*  @param[in] const gu::uint32 width : ��ʂ̕�
		*  @param[in] const gu::uint32 height : ��ʂ̍���
		*  @return    void
		*************************************************************************/
		virtual void Resize(const gu::uint32 width, const gu::uint32 height) = 0;

		/*!**********************************************************************
		*  @brief     �t���X�N���[�����[�h�Ǝw�肳�ꂽ�𑜓x�̃X�N���[�����[�h��؂�ւ���
		* �@�@�@�@�@�@  (isOn : true->�t���X�N���[�����[�h�Ɉڍs����. false : window���[�h�Ɉڍs����)
		*  @param[in] const bool isOn : true->�t���X�N���[�����[�h�Ɉڍs����. false : window���[�h�Ɉڍs����
		*  @return    void
		*************************************************************************/
		virtual void SwitchFullScreenMode(const bool isOn) = 0;

		/*!**********************************************************************
		*  @brief     HDR���[�h��SDR���[�h��؂�ւ���.
		*  @param[in] const bool enableHDR : true -> HDR, false -> SDR
		*  @return    void
		*************************************************************************/
		virtual void SwitchHDRMode(const bool enableHDR) = 0;
		#pragma endregion

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     ���ݕ`��R�}���h���l�ߍ��݂����o�b�N�o�b�t�@�̃C���f�b�N�X��Ԃ��܂�
		*  @param[in] void
		*  @return    gu::uint32 �X���b�v�`�F�C���̕`�悷�镝
		*************************************************************************/
		virtual gu::uint8 GetCurrentBufferIndex() const = 0;

		/*!**********************************************************************
		*  @brief     �X���b�v�`�F�C���̕`�悷�镝��Ԃ��܂�
		*  @param[in] void
		*  @return    gu::uint32 �X���b�v�`�F�C���̕`�悷�镝
		*************************************************************************/
		__forceinline gu::uint32  GetWidth() const noexcept { return _desc.WindowInfo.Width; }

		/*!**********************************************************************
		*  @brief     �X���b�v�`�F�C���̕`�悷�鍂����Ԃ��܂�
		*  @param[in] void
		*  @return    gu::uint32 �X���b�v�`�F�C���̕`�悷�鍂��
		*************************************************************************/
		__forceinline gu::uint32  GetHeight() const noexcept { return _desc.WindowInfo.Height; }

		/*!**********************************************************************
		*  @brief     �`��Ŏg�p����s�N�Z���t�H�[�}�b�g
		*  @param[in] void
		*  @return    PixelFormat �`��Ŏg�p����s�N�Z���t�H�[�}�b�g
		*************************************************************************/
		__forceinline PixelFormat GetPixelFormat() const noexcept { return _desc.PixelFormat; }

		/*!**********************************************************************
		*  @brief     Window�̕��⍂��, �n���h����Ԃ��܂�
		*  @param[in] void
		*  @return    const WindowInfo& Window�̏��
		*************************************************************************/
		__forceinline const WindowInfo& GetWindowInfo() const noexcept { return _desc.WindowInfo; }

		/*!**********************************************************************
		*  @brief     �o�b�N�o�b�t�@�̎Q�Ƃ�Ԃ��܂�
		*  @param[in] const gu::uint8 �t���[���o�b�t�@�̃C���f�b�N�X
		*  @return    gu::SharedPointer<GPUTexture> �o�b�N�o�b�t�@
		*************************************************************************/
		__forceinline gu::SharedPointer<GPUTexture> GetBuffer(const gu::uint8 index) { return _backBuffers[index]; }
		
		/*!**********************************************************************
		*  @brief     �t���[���o�b�t�@�̑�����Ԃ��܂�. 
		*  @param[in] void
		*  @return    gu::uint8 �t���[���o�b�t�@�̐�
		*************************************************************************/
		__forceinline gu::uint8 GetBufferCount() const noexcept { return _desc.FrameBufferCount; }

		#pragma endregion

		#pragma region Public Constructor and Destructor
		#pragma endregion
		
	protected:
		#pragma region Protected Constructor and Destructor

		/*! @brief �f�t�H���g�R���X�g���N�^*/
		RHISwapchain() = default;

		/*! @brief �f�X�g���N�^*/
		virtual ~RHISwapchain()
		{
			_backBuffers.Clear(); _backBuffers.ShrinkToFit();
			if (_device)       { _device.Reset(); }
		};

		/*! @brief ����ݒ�*/
		explicit RHISwapchain(const gu::SharedPointer<RHIDevice>& device, const gu::SharedPointer<RHICommandQueue>& commandQueue, const WindowInfo& windowInfo, PixelFormat pixelFormat, gu::uint8 frameBufferCount = 3, gu::uint8 vsync = 0, bool isValidHDR = true, bool isFullScreen = false)
		{
			_device = device; _desc.CommandQueue = commandQueue; _desc.WindowInfo = windowInfo; _desc.PixelFormat = pixelFormat; _desc.VSync = vsync; _desc.FrameBufferCount = frameBufferCount; _desc.IsValidHDR = isValidHDR; _desc.IsFullScreen = isFullScreen;
		}

		/*! @brief Swapchain��Descriptor���g���ď�����*/
		explicit RHISwapchain(const gu::SharedPointer<RHIDevice>& device, const SwapchainDesc& desc):
			_device(device), _desc(desc)
		{
			
		}
		#pragma endregion

		#pragma region Protected Function
		#pragma endregion

		#pragma region Protected Property

		/*! @brief �_���f�o�C�X*/
		gu::SharedPointer<RHIDevice> _device = nullptr;
		
		/*! @brief �o�b�N�o�b�t�@*/
		gu::DynamicArray<gu::SharedPointer<GPUTexture>> _backBuffers; 
		
		/*! @brief �X���b�v�`�F�C���̌��݂̐ݒ�*/
		SwapchainDesc _desc = {};

		#pragma endregion
	};
}
#endif