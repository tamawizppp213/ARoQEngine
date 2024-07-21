//////////////////////////////////////////////////////////////////////////////////
///             @file   LowLevelGraphicsEngine.hpp
///             @brief  Low level API grapchis engine. 
///             How To: �@ Call StartUp function at first
///                     �A Create Draw frame, call beginDrawFrame and EndDrawFrame() ...loop....
///                     �B Finally, you should call shutdown when you wanna finish to render
///             @author Toide Yutaro
///             @date   2022_03_11
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef LOW_LEVEL_GRAPHICS_ENGINE_HPP
#define LOW_LEVEL_GRAPHICS_ENGINE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHITypeCore.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIDevice.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommandList.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////

/****************************************************************************
*				  			LowLevelGraphicsEngine
****************************************************************************/
/* @class     LowLevelGraphicsEngine
*  @brief     LowLevelGraphicsEngine
*****************************************************************************/
class LowLevelGraphicsEngine final : public gu::NonCopyAndMove
{
public:
	#pragma region Static Const
	static constexpr gu::uint32 FRAME_BUFFER_COUNT = 3;

	static constexpr gu::uint32 VSYNC = 1; // 0: don't wait, 1:wait(60fps)
	#pragma endregion

	#pragma region Public Function
	/*!**********************************************************************
	*  @brief     Graphics Engine���N����, �`��t���[�����X�^�[�g���܂�. 
	*  @param[in] const rhi::core::GraphicsAPI 
	*  @param[in] void* window handle
	*  @param[in] void* instance Handle
	*  @return    void
	*************************************************************************/
	void StartUp(const rhi::core::GraphicsAPI apiVersion, void* hwnd, void* hInstance);

	/*!**********************************************************************
	*  @brief     CommandList���J��, �`��t���[�����X�^�[�g���܂�.
	*  @param[in] void
	*  @return    void
	*************************************************************************/
	void BeginDrawFrame();

	/*!**********************************************************************
	*  @brief     �o�b�N�o�b�t�@�ɏ������ނ��߂�RenderPass���J�n���܂�.
	*  @param[in] void
	*  @return    void
	*************************************************************************/
	void BeginSwapchainRenderPass();
	
	/*!**********************************************************************
	*  @brief     CommandList���, �`��t���[�����I����, �o�b�N�o�b�t�@��؂�ւ��܂�.
	*  @param[in] void
	*  @return    void
	*************************************************************************/
	void EndDrawFrame();
	
	/*!**********************************************************************
	*  @brief     �O���t�B�N�X�G���W�����I�����܂�. 
	*  @param[in] void
	*  @return    void
	*************************************************************************/
	void ShutDown();

	/*!**********************************************************************
	*  @brief     Swapchain�����T�C�Y���܂�.
	*  @param[in] const gu::uint32 �V������
	*  @param[in] const gu::uint32 �V��������
	*  @return    void
	*************************************************************************/
	void OnResize(const gu::uint32 newWidth, const gu::uint32 newHeight);

	/*!**********************************************************************
	*  @brief     �Ώۂ̃R�}���h���X�g�����s��, �t�F���X�̃V�O�i���l��Ԃ��܂�.
	*  @param[in] const rhi::core::CommandListType �R�}���h���X�g�̎��
	*  @param[in] const bool �܂��t���[�������ǂ���
	*  @return    gu::uint64 �t�F���X�̃V�O�i���l
	*************************************************************************/
	gu::uint64 FlushGPUCommands(const rhi::core::CommandListType type, const bool stillMidFrame = false);

	/*!**********************************************************************
	*  @brief     �R�}���h�L���[���Ăяo���đO�܂ł̏�������������܂�GPU��ҋ@���܂�. �K�v�ɉ�����CPU���ҋ@���܂�.
	*  @param[in] const rhi::core::CommandListType �R�}���h���X�g�̎��
	*  @param[in] const gu::uint64 �ҋ@����t�F���X�̒l
	*  @param[in] const bool CPU���ҋ@���邩�ǂ���
	*  @return    void
	*************************************************************************/
	void WaitExecutionGPUCommands(const rhi::core::CommandListType type, const gu::uint64 waitValue, const bool stopCPU);
	
	#pragma endregion

	#pragma region Public Property
	/*!**********************************************************************
	*  @brief     �_���f�o�C�X���擾���܂�. 
	*  @param[in] void
	*  @return    DevicePtr : �_���f�o�C�X��SharedPointer
	*************************************************************************/
	__forceinline RHIDevicePtr GetDevice() const noexcept { return _device; }

	/*!**********************************************************************
	*  @brief     ComandList���擾���܂�.
	*  @param[in] const rhi::core::CommandListType �R�}���h���X�g�̎��
	*  @return    CommandListPtr : CommandList��SharedPointer
	*************************************************************************/
	__forceinline RHICommandListPtr GetCommandList(const rhi::core::CommandListType type) const noexcept { return _commandLists.At(type); }
	
	/*!**********************************************************************
	*  @brief     ComandQueue���擾���܂�.
	*  @param[in] const rhi::core::CommandListType �R�}���h���X�g�̎��
	*  @return    CommandQueuePtr : CommandQueue��SharedPointer
	*************************************************************************/
	__forceinline RHICommandQueuePtr GetCommandQueue(const rhi::core::CommandListType type) const noexcept { return _commandQueues.At(type); }

	/*!**********************************************************************
	*  @brief     �f�t�H���g��RenderPass�̎擾. �����_�[�p�X�J�n����, ��ʂ̃N���A���s���܂�.  
	*  @param[in] const rhi::core::CommandListType �R�}���h���X�g�̎��
	*  @return    gu::SharedPointer<rhi::core::RHIRenderPass>
	*************************************************************************/
	__forceinline gu::SharedPointer<rhi::core::RHIRenderPass> GetDrawClearRenderPass() const noexcept { return _renderPass; }

	/*!**********************************************************************
	*  @brief     RenderPass�̎擾. �����_�[�p�X�J�n����, ��ʂ̃N���A�͍s���܂���.
	*  @param[in] const rhi::core::CommandListType �R�}���h���X�g�̎��
	*  @return    gu::SharedPointer<rhi::core::RHIRenderPass>
	*************************************************************************/
	__forceinline gu::SharedPointer<rhi::core::RHIRenderPass> GetDrawContinueRenderPass() const noexcept { return _drawContinueRenderPass; }

	/*!**********************************************************************
	*  @brief     �����_�[�^�[�Q�b�g�ƃf�v�X�X�e���V���̃e�N�X�`����ResourceView���g�p����N���X�ł�.
	*  @param[in] const gu::uint32 Frame�̃C���f�b�N�X
	*  @return    gu::SharedPointer<rhi::core::RHIFrameBuffer>
	*************************************************************************/
	__forceinline gu::SharedPointer<rhi::core::RHIFrameBuffer> GetFrameBuffer(const gu::uint32 frameIndex) const noexcept { return _frameBuffers[frameIndex]; }

	/*!**********************************************************************
	*  @brief     ���ݏ������ݒ��̃t���[���ԍ����擾���܂�. 
	*  @param[in] void
	*  @return    gu::uint32 �t���[���ԍ�
	*************************************************************************/
	__forceinline gu::uint32 GetCurrentFrameIndex() const { return _currentFrameIndex; }

	/*!**********************************************************************
	*  @brief     �o�b�N�o�b�t�@�̃s�N�Z���t�H�[�}�b�g���擾���܂�
	*  @param[in] void
	*  @return    rhi::core::PixelFormat
	*************************************************************************/
	__forceinline rhi::core::PixelFormat GetBackBufferFormat() const { return _pixelFormat; }

	/*!**********************************************************************
	*  @brief     �`��o�b�t�@�𖈃t���[���������邽�߂�Swapchain�̃|�C���^��Ԃ��܂�
	*  @param[in] void
	*  @return    RHISwapchainPtr
	*************************************************************************/
	__forceinline RHISwapchainPtr GetSwapchain() const noexcept 
	{
		return _swapchain; 
	}
	
	/*!**********************************************************************
	*  @brief     GPU�v����QueryHeap��Ԃ��܂�
	*  @param[in] const rhi::core::QueryHeapType QueryHeap�̎��
	*  @return    RHIQueryPtr
	*************************************************************************/
	__forceinline RHIQueryPtr GetQuery(const rhi::core::QueryHeapType queryType) 
	{
		return _queryHeaps.At(queryType); 
	}
	#pragma endregion

	#pragma region Public Constructor and Destructor
	/*! @brief �f�t�H���g�R���X�g���N�^*/
	LowLevelGraphicsEngine() = default;

	/*! @brief �f�X�g���N�^*/
	~LowLevelGraphicsEngine();

	#pragma endregion

protected:
	#pragma region Protected Function
	void SetFrameBuffers(const int width, const int height, 
		const rhi::core::ClearValue& clearColor = rhi::core::ClearValue(0.0f, 0.3f, 0.3f, 1.0f),
		const rhi::core::ClearValue& clearDepthColor = rhi::core::ClearValue(0.0f, 0.0f, 0.0f, 1.0f));
	#pragma endregion 

	#pragma region Protected Property
	/*! @brief Graphics API �o�[�W���� (���݂�DirectX12 or Vulkan)*/
	rhi::core::GraphicsAPI _apiVersion = rhi::core::GraphicsAPI::Unknown;

	/* @brief : graphics API instance (select graphics api)*/
	RHIInstancePtr _instance = nullptr;

	/* @brief : gpu display adapter (basically discrete gpu adapter)*/
	RHIDisplayAdapterPtr _adapter = nullptr;

	/* @brief : Logical Device*/
	RHIDevicePtr  _device = nullptr;

	/* @ brief : Command queue (graphics, compute, transfer)*/
	gu::SortedMap<rhi::core::CommandListType, RHICommandQueuePtr> _commandQueues;

	/* @brief : Command List*/
	gu::SortedMap<rhi::core::CommandListType, RHICommandListPtr> _commandLists;
	
	/* @brief : Default rendering pass*/
	gu::SharedPointer<rhi::core::RHIRenderPass> _renderPass = { nullptr }; 
	gu::SharedPointer<rhi::core::RHIRenderPass> _drawContinueRenderPass = nullptr;
	gu::DynamicArray<gu::SharedPointer<rhi::core::RHIFrameBuffer>> _frameBuffers = { nullptr };
	
	/* @brief : current frame index*/
	gu::uint32 _currentFrameIndex = 0;

	/*! @brief TimeStamp*/
	rhi::core::GPUTimingCalibrationTimestamp _beginDrawFrameTimeStamp = {};
	rhi::core::GPUTimingCalibrationTimestamp _endDrawFrameTimeStamp = {};
	gu::double64 _gpuTimer = 0.0;
	gu::double64 _cpuTimer = 0.0;

#pragma region Rendering Variables
	/*----------------------------------------------------------------------
	*  @brief : �`��o�b�t�@�𖈃t���[���������邽�߂�Swapchain
	*----------------------------------------------------------------------*/
	gu::SharedPointer<rhi::core::RHISwapchain> _swapchain = nullptr;

	/*----------------------------------------------------------------------
	*  @brief : CPU��GPU�Ԃ̓����E�ҋ@�������s�����߂̃t�F���X
	*----------------------------------------------------------------------*/
	gu::SharedPointer<rhi::core::RHIFence> _fence = nullptr;
	gu::uint64 _fenceValue = 0;  // current frame fence value

	/*----------------------------------------------------------------------
	*  @brief : HDR���g�p���� (true : �g�p, false : �g�p���Ȃ�)
	*----------------------------------------------------------------------*/
	bool _useHDR = false;

	/*----------------------------------------------------------------------
	*  @brief : ���C�g���[�V���O���g�p���� (true : �g�p, false : �g�p���Ȃ�)
	*----------------------------------------------------------------------*/
	bool _useRayTracing = true;

	/*----------------------------------------------------------------------
	*  @brief : Swapchain���Ŏg�p����f�t�H���g�̃s�N�Z���t�H�[�}�b�g
	*           SDR�z���R8G8B8A8_UNORM
	* �@�@�@�@�@�@�@HDR�z���R16G16B16A16_FLOAT���g�p���܂�.
	*----------------------------------------------------------------------*/
	rhi::core::PixelFormat _pixelFormat = rhi::core::PixelFormat::R8G8B8A8_UNORM;

	/*----------------------------------------------------------------------
	*  @brief : �ʏ�`��Ŏg�p����Depth Stencil�t�H�[�}�b�g
	*           �f�t�H���g��Stencil���g�p����, D32_FLOAT���g�p���܂�
	*----------------------------------------------------------------------*/
	rhi::core::PixelFormat _depthStencilFormat = rhi::core::PixelFormat::D32_FLOAT;
		
	/*----------------------------------------------------------------------
	*  @brief : �X�N���[���̏c��
	*----------------------------------------------------------------------*/
	gu::uint32 _width  = 0;
	gu::uint32 _height = 0;

#pragma endregion Rendering Variables

	// shutdown check
	bool _hasCalledShutDown = false;

	bool _hasInitialized = false;

	/*----------------------------------------------------------------------
	*  @brief : GPU���׌v���̂��߂�QueryHeap
	*----------------------------------------------------------------------*/
	gu::SortedMap<rhi::core::QueryHeapType, gu::SharedPointer<rhi::core::RHIQuery>> _queryHeaps = {};

	/****************************************************************************
	**                Heap Config
	*****************************************************************************/
	static constexpr int RTV_DESC_COUNT = 1000;
	static constexpr int DSV_DESC_COUNT = 100;
	static constexpr int CBV_DESC_COUNT = 1024 * 10;
	static constexpr int UAV_DESC_COUNT = 1024 * 10;
	static constexpr int SRV_DESC_COUNT = 1024 * 10;
	static constexpr int MAX_SAMPLER_STATE = 16;

	#pragma endregion 

private:
	#pragma region Private Function
	void SetUpRenderResource();
	void SetUpHeap();
	void SetUpFence();
	void SetUpQuery();
	#pragma endregion

};

#endif