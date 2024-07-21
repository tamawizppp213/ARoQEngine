//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIDevice.hpp
///             @brief  Device
///             @author Toide Yutaro
///             @date   2022_06_09
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_COMMANDLIST_HPP
#define DIRECTX12_COMMANDLIST_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommandList.hpp"
#include "DirectX12Core.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////

namespace rhi::directX12
{
	class RHIDevice;
	class RHICommandAllocator;
	class RHIRenderPass;
	class RHIFrameBuffer;
	class GPUBarrierBatcher;
	/****************************************************************************
	*				  			RHICommandList
	****************************************************************************/
	/* @brief     GPU�̕`�施�߂��܂Ƃ߂��N���X. BeginRecording��EndRecording��1�t���[���̊J�n�ƏI�����ɌĂ�ł�������.
	*****************************************************************************/
	class RHICommandList : public rhi::core::RHICommandList, public gu::EnableSharedFromThis<RHICommandList>
	{
	public:
		#pragma region Public Function
		#pragma region Main Draw Frame 
		/*!**********************************************************************
		*  @brief     �R�}���h���X�g���L�^��ԂɕύX���܂�. �����Draw�֐��̍ŏ��Ɏg�p���܂� @n
		*  @param[in] �`��t���[�����ɌĂ΂��ꍇ�ɃR�}���h�A���P�[�^�̒��g��Reset���邩�����肷��bool�l.@n
		*             �`��t���[�����ɌĂԂ̂�, �R�}���h���X�g��؂�ւ���ۂɎg�p�����\�������邽�߂ł�.
		*  @return    void
		*************************************************************************/
		virtual void BeginRecording(const bool stillMidFrame) override;

		/*!**********************************************************************
		*  @brief     �R�}���h���X�g���L�^��Ԃ�����s�\��ԂɕύX���܂�. �����Draw�֐��̍Ō�Ɏg�p���܂�
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		virtual void EndRecording () override;

		/*!**********************************************************************
		*  @brief     �V����RenderPass���J�n���܂�. RenderPass�̎w��J���[���g�����������Ȃǂ��s���܂�.  
		*  @param[in] const gu::SharedPointer<core::RHIRenderPass>& RenderPass : �J�n����RenderPass
		*  @param[in] const gu::SharedPointer<core::RHIFrameBuffer>& FrameBuffer : �J�n����FrameBuffer
		*  @return    void
		*************************************************************************/
		virtual void BeginRenderPass(const gu::SharedPointer<core::RHIRenderPass>& renderPass, const gu::SharedPointer<core::RHIFrameBuffer>& frameBuffer) override;
		
		/*!**********************************************************************
		*  @brief     ������RenderPass���I�����܂�.
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		virtual void EndRenderPass() override;

		/*!**********************************************************************
		*  @brief     �R�}���h���X�g���L�^��ԂɕύX���܂�.�܂��R�}���h�A���P�[�^���̃R�}���h�o�b�t�@�̓��e��擪�ɖ߂��܂�.
		*  @param[in] �R�}���h�A���P�[�^��ύX���邩�ǂ���
		*  @attention ��{�I�ɂ�, Reset�ł͂Ȃ�BeginRecording���g�p���Ă�������.
		*************************************************************************/
		virtual void Reset(const gu::SharedPointer<core::RHICommandAllocator>& changeAllocator = nullptr) override;

		/*-------------------------------------------------------------------
		-               Common command
		---------------------------------------------------------------------*/
		void SetDescriptorHeap(const gu::SharedPointer<core::RHIDescriptorHeap>& heap) override;

		#pragma endregion Main Draw Frame
        #pragma region Query
		/*----------------------------------------------------------------------
		*  @brief : GPU�����擾���邽�߂̃N�G�����J�n���܂�
		*----------------------------------------------------------------------*/
		void BeginQuery(const core::QueryResultLocation& location) override;

		/*----------------------------------------------------------------------
		*  @brief : GPU�����擾���邽�߂̃N�G�����I�����܂�
		*----------------------------------------------------------------------*/
		void EndQuery(const core::QueryResultLocation& location) override;

		#pragma endregion Query
		#pragma region Graphics Command Function
		/*!**********************************************************************
		*  @brief     �[�x���w��͈̔͂ɓ����Ă��邩���e�X�g��, �͈͓��Ȃ�΃s�N�Z���V�F�[�_�[�𓮍삳���܂�.
		*  @param[in] const gu::float32 : �ŏ��̐[�x���
		*  @param[in] const gu::float32 : �ő�̐[�x���
		*  @return    void
		*************************************************************************/
		virtual void SetDepthBounds(const float minDepth, const float maxDepth) override;

		/*!**********************************************************************
		*  @brief     Variable Rate Shading��Graphics Pipeline��ŗL�������܂�. PerTile, PerPrimitive���g�p����ꍇ��SetShadingRateImage���g�p���Ă�������
		*  @note      https://sites.google.com/site/monshonosuana/directx%E3%81%AE%E8%A9%B1/directx%E3%81%AE%E8%A9%B1-%E7%AC%AC168%E5%9B%9E
		*  @oaram[in] �`��s�N�Z���̒P��
		*  @param[in] const gu::DynamicArray<core::ShadingRateCombiner>& Image�̌������@
		*  @return    void
		*************************************************************************/
		virtual void SetShadingRate(const core::ShadingRate shadingRate, const gu::DynamicArray<core::ShadingRateCombiner>& combiners) override;

		/*!**********************************************************************
		*  @brief     VariableRateShading : �s�N�Z���V�F�[�_�[�̋N����1�s�N�Z�����Ƃł͂Ȃ�, �����s�N�Z�������킹��1��̃V�F�[�_�[�N���ŏ������邽�߂̃C���[�W��ݒ�
		*  @param[in] const gu::SharedPointer<core::GPUTexture> : VariableRateShading��K�p����e�N�X�`��@n
		*             �P��̃~�b�v������2D�e�N�X�`����, DXGI_FORMAT_R8_UINT�ł���K�v������. (D3D12_TEXTURE_LAYOUT_UNKNOWN)
		*  @note      https://learn.microsoft.com/en-us/windows/win32/api/d3d12/nf-d3d12-id3d12graphicscommandlist5-rssetshadingrateimage
		*  @return    void
		*************************************************************************/
		virtual void SetShadingRateImage(const gu::SharedPointer<core::GPUTexture>& texture) override;

		/*!**********************************************************************
		*  @brief     ���_���̂Ȃ�����ݒ肵�܂�. 
		*  @param[in] const core::PrimitiveTopology : �v���~�e�B�u�̃g�|���W�[���
		*  @return    void
		*************************************************************************/
		virtual void SetPrimitiveTopology(const core::PrimitiveTopology topology) override;

		/*!**********************************************************************
		*  @brief     �r���[�|�[�g�ɂ���ĕ`��̈��ݒ肵�܂�. �V�U�[��`��Viewport�ɍ��킹�Ď����Őݒ肵�܂�
		*  @param[in] const core::Viewport& : �`��̈�������P��̃r���[�|�[�g
		*  @return    void
		*************************************************************************/
		virtual void SetViewport(const core::Viewport& viewport) override;

		/*!**********************************************************************
		*  @brief     �r���[�|�[�g�̔z��(�A�h���X)�����ĕ`��̈��ݒ肵�܂�. �V�U�[��`��Viewport�ɍ��킹�Ď����Őݒ肵�܂�
		*  @param[in] const core::Viewport* : �`��̈���L�q�����z��, �������͒P���Viewport�̃A�h���X
		*  @param[in] const gu::uint32 : �r���[�|�[�g�̔z�� (Default��1)
		*  @return    void
		*************************************************************************/
		virtual void SetViewport(const core::Viewport* viewport, const gu::uint32 numViewport) override;
		
		/*!**********************************************************************
		*  @brief     VR�̂悤�ȗ��̎����s�����ɐݒ肷��`��̈�ł�.�V�U�[��`��Viewport�ɍ��킹�Ď����Őݒ肵�܂�
		*  @param[in] const core::Viewport& �����̎���������`��̈�
		*  @param[in] const core::Viewport& �E���̎���������`��̈�
		*  @return    void
		*************************************************************************/
		virtual void SetStereoViewport(const core::Viewport& leftView, const core::Viewport& rightView) override ;

		/*!**********************************************************************
		*  @brief     �r���[�|�[�g���Ŏ��ۂɕ`�悳���̈�𐧌����邽�߂̃V�U�[��`���蓮�Őݒ肵�܂�.
		*  @param[in] const core::ScissorRect* : �`��̈�𐧌����邽�߂̃V�U�[��`�̔z��
		*  @param[in] const gu::uint32 : �V�U�[��`�̔z��
		*  @return    void
		*************************************************************************/
		virtual void SetScissor (const core::ScissorRect* rect, const gu::uint32 numRect = 1) override;
		
		/*!**********************************************************************
		*  @brief     �`��̈�������r���[�|�[�g��, ���̒��Ŏ��ۂɕ`�悳���͈͂��w�肷��V�U�[��`�����ꂼ��蓮�Őݒ肵�܂�.
		*  @param[in] const core::Viewport& �`��̈�������r���[�|�[�g
		*  @param[in] const core::ScissorRect& ���ۂɕ`�悳���͈͂������V�U�[��`
		*  @return    void
		*************************************************************************/
		virtual void SetViewportAndScissor(const core::Viewport& viewport, const core::ScissorRect& rect) override;
		
		/*!**********************************************************************
		*  @brief     ���\�[�X���C�A�E�g��ݒ肵�܂�. ���\�[�X���C�A�E�g��GPU���\�[�X�̃o�C���h���@��ݒ肵�܂�.
		*  @param[in] const gu::SharedPointer<core::RHIResourceLayout>& ���\�[�X���C�A�E�g
		*  @return    void
		*************************************************************************/
		virtual void SetResourceLayout(const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout) override;
		
		virtual void SetGraphicsPipeline(const gu::SharedPointer<core::GPUGraphicsPipelineState>& pipelineState) override;
		
		virtual void SetVertexBuffer(const gu::SharedPointer<core::GPUBuffer>& buffer) override ;
		
		virtual void SetVertexBuffers(const gu::DynamicArray<gu::SharedPointer<core::GPUBuffer>>& buffers, const size_t startSlot = 0) override;
		
		/*!**********************************************************************
		*  @brief     �C���f�b�N�X�o�b�t�@��ݒ肵�܂�. �C���f�b�N�X�o�b�t�@��GPU�o�b�t�@�̌`�œn����܂�.
		*  @param[in] const gu::SharedPointer<core::GPUBuffer>& �C���f�b�N�X�o�b�t�@
		*  @param[in] const core::PixelFormat �C���f�b�N�X�̌^ (Default��R32_UINT)
		*  @return    void
		*************************************************************************/
		void SetIndexBuffer(const gu::SharedPointer<core::GPUBuffer>& buffer, const core::PixelFormat indexType = core::PixelFormat::R32_UINT) override;

		/*!**********************************************************************
		*  @brief     �C���f�b�N�X�����Ă��郂�f���ł���, �C���X�^���V���O�`�悪�K�v�Ȃ��v���~�e�B�u��`�悵�܂�.
		*  @param[in] indexCount            : �C���f�b�N�X�̑���
		*  @param[in] startIndexLocation    : �C���f�b�N�X��ǂݎ��n�߂�, �C���f�b�N�X�o�b�t�@���̔z��v�f��
		*  @param[in] baseVertexLocation    : ���_�o�b�t�@�[���璸�_��ǂݎ��O��, �e�C���f�b�N�X�ɒǉ�����l
		*  @return    void
		*************************************************************************/
		virtual void DrawIndexed(const gu::uint32 indexCount, const gu::uint32 startIndexLocation = 0, const gu::uint32 baseVertexLocation = 0) override;
		
		/*!**********************************************************************DrawIndexedIndirect
		*  @brief     �C���f�b�N�X�����Ă��郂�f���ł���, �C���X�^���V���O�`�悪�K�v�ƂȂ�v���~�e�B�u��`�悵�܂�.
		*  @param[in] indexCountPerInstance : �C���X�^���X���ɕK�v�ƂȂ�C���f�b�N�X�̑���
		*  @param[in] instance Count        : �C���X�^���X�̐�
		*  @param[in] startIndexLocation    : �C���f�b�N�X��ǂݎ��n�߂�, �C���f�b�N�X�o�b�t�@���̔z��v�f��
		*  @param[in] baseVertexLocation    : ���_�o�b�t�@�[���璸�_��ǂݎ��O��, �e�C���f�b�N�X�ɒǉ�����l
		*  @param[in] startInstanceLocation : �`����s���ŏ��̃C���X�^���X�ԍ�
		*  @return    void
		*************************************************************************/
		virtual void DrawIndexedInstanced(const gu::uint32 indexCountPerInstance, const gu::uint32 instanceCount, const gu::uint32 startIndexLocation = 0, const gu::uint32 baseVertexLocation = 0, const gu::uint32 startInstanceLocation = 0) override;

		/*!**********************************************************************
		*  @brief     �C���f�b�N�X�o�b�t�@�������f���ɑ΂���, �����o�b�t�@��GPU�Őݒ�, �`������s�o����֐��ł�.
		*  @param[in] const gu::SharedPointer<core::GPUBuffer>& �����o�b�t�@
		*  @param[in] const gu::uint32 �h���[�R�[���̑���
		*  @return    void
		*************************************************************************/
		virtual void DrawIndexedIndirect(const gu::SharedPointer<core::GPUBuffer>& argumentBuffer, const gu::uint32 drawCallCount) override;

		/*!**********************************************************************
		*  @brief     Mesh shader�Ŏg�p����`��֐��ł�. 
		*  @param[in] const gu::uint32 threadGroupCountX : X�����̃X���b�h�O���[�v��
		*  @param[in] const gu::uint32 threadGroupCountY : Y�����̃X���b�h�O���[�v��
		*  @param[in] const gu::uint32 threadGroupCountZ : Z�����̃X���b�h�O���[�v��
		*  @return    void
		*************************************************************************/
		virtual void DispatchMesh(const gu::uint32 threadGroupCountX = 1, const gu::uint32 threadGroupCountY = 1, const gu::uint32 threadGroupCountZ = 1) override;
        

		#pragma endregion Graphics Command Function
		/*-------------------------------------------------------------------
		-                Compute Command
		---------------------------------------------------------------------*/
		void SetComputeResourceLayout(const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout) override;
		
		void SetComputePipeline(const gu::SharedPointer<core::GPUComputePipelineState>& pipeline) override;
		
		/*!**********************************************************************
		*  @brief     Compute shader�Ŏg�p����`��֐��ł�.
		*  @param[in] const gu::uint32 threadGroupCountX : X�����̃X���b�h�O���[�v��
		*  @param[in] const gu::uint32 threadGroupCountY : Y�����̃X���b�h�O���[�v��
		*  @param[in] const gu::uint32 threadGroupCountZ : Z�����̃X���b�h�O���[�v��
		*  @return    void
		*************************************************************************/
		void Dispatch(gu::uint32 threadGroupCountX = 1, gu::uint32 threadGroupCountY = 1, gu::uint32 threadGroupCountZ = 1) override;
		
		#pragma region Copy Function
		/*!**********************************************************************
		*  @brief     GPU�o�b�t�@�̗̈������GPU�|�C���^����ʂ�GPU�|�C���^�ɃR�s�[���s��. GPU�ł�memcpy
		*  @param[in] const gu::SharedPointer<core::GPUBuffer> : �R�s�[��̃o�b�t�@
		*  @param[in] const gu::uint64 : �R�s�[��̏����������|�C���^�����炷offset byte
		*  @param[in] const gu::SharedPointer<core::GPUBuffer> : �R�s�[���̃o�b�t�@
		*  @param[in] const gu::uint64 : �R�s�[���̏����ǂݎ��|�C���^�����炷offset byte
		*  @param[in] const gu::uint64 : �R�s�[���s���͈� (byteSize�Ŏw��)
		*  @return void
		*************************************************************************/
		virtual void CopyBufferRegion(const gu::SharedPointer<core::GPUBuffer>& dest, const gu::uint64 destOffset, const gu::SharedPointer<core::GPUBuffer>& source, const gu::uint64 sourceOffset, const gu::uint64 copyByteSize) override;

		/*!**********************************************************************
		*  @brief     GPU�e�N�X�`���̗̈������GPU�|�C���^����ʂ�GPU�|�C���^�ɃR�s�[���s��. GPU�ł�memcpy
		*  @param[in] const gu::SharedPointer<core::GPUTexture> : �R�s�[��̃e�N�X�`��
		*  @param[in] const gu::SharedPointer<core::GPUTexture> : �R�s�[���̃e�N�X�`��
		*  @param[in] const core::GPUTextureCopyInfo : �R�s�[����ۂ̏��
		*  @return void
		*************************************************************************/
		virtual void CopyTextureRegion(const gu::SharedPointer<core::GPUTexture>& destination, const gu::SharedPointer<core::GPUTexture>& source, const core::GPUTextureCopyInfo& copyInfo) override;

		/*!**********************************************************************
		*  @brief     �e�N�X�`���̗̈��S�Ă�Subresource���݂ŕʂ̃e�N�X�`���ɃR�s�[����
		*  @param[in] const gu::SharedPointer<core::GPUTexture> : �R�s�[��̃e�N�X�`��
		*  @param[in] const gu::SharedPointer<core::GPUTexture> : �R�s�[���̃e�N�X�`��
		*  @return void
		*************************************************************************/
		virtual void CopyResource(const gu::SharedPointer<core::GPUTexture>& dest, const gu::SharedPointer<core::GPUTexture>& source) override;
		
		/*!**********************************************************************
		*  @brief     GPU���\�[�X�̗̈��ʂ�GPU���\�[�X�ɃR�s�[����. (�o�b�t�@�ƃe�N�X�`���̂悤�Ȉَ�Ԃ̃��\�[�X�ł����v�ł�)
		*  @param[in] const gu::SharedPointer<core::GPUResource> : �R�s�[��̃��\�[�X
		*  @param[in] const gu::SharedPointer<core::GPUResource> : �R�s�[���̃��\�[�X
		*  @return void
		*************************************************************************/
		void CopyResource(const gu::SharedPointer<core::GPUResource>& dest, const gu::SharedPointer<core::GPUResource>& source);
		#pragma endregion

		#pragma region Resource Barrier
		/*!**********************************************************************
		*  @brief  �P�Ƃ�GPU���\�[�X�̏�ԑJ�ڂ������܂�. @n
		*          ���\�[�X�̎g�������ς��^�C�~���O�ŌĂяo���܂�.
		*  @param[in] const gu::SharedPointer<core::GPUBuffer> GPU�o�b�t�@
		*  @param[in] const core::ResourceState �J�ڑO�̃��\�[�X���
		*  @param[in] const core::ResourceState �J�ڌ�̃��\�[�X���
		*  @param[in] const gu::uint32 �T�u���\�[�X�������C���f�b�N�X (�f�t�H���g�͑S�ẴT�u���\�[�X�������܂�) 0xfffffff�̏ꍇ�͑S�ẴC���f�b�N�X�ŗL��������܂�
		*  @return    void
		*************************************************************************/
		virtual void PushTransitionBarrier(const gu::SharedPointer<core::GPUResource>& buffer  , const core::ResourceState after, const gu::uint32 subresource = 0xffffffff) override;
		
		/*!**********************************************************************
		*  @brief  �����������̈�Ƀ}�b�s���O���ꂽ������GPU���\�[�X�ɑ΂�, �g�p���郊�\�[�X���̂�؂�ւ���ۂɎg�p���܂�. @n
		*          �����Ɏg�p���Ȃ����Ƃ��S�ۂ���Ă��郊�\�[�X�̃�������ߖ񂷂邱�Ƃ��\�ƂȂ�܂�. @n
		*          ������, �{�֐����g�p����ꍇ��,CreateCommittedResource�ł͖���, CreatePlacedResource���g�p�������@�Ńq�[�v�̊m�ۂ��s���Ă�������.@n
		*          (�ʁX�̃q�[�v������Ă��܂����ƂɂȂ�, �����������̈������Ȃ��Ȃ邽��.)
		*  @note   https://logicalbeat.jp/blog/8185/ (AliasingBarrier�̊��p���@�ɂ��Ă̋L�q)
		*  @param[in] const gu::SharedPointer<core::GPUResource> �؂�ւ���O�Ɏg�p���Ă���GPU���\�[�X
		*  @param[in] const gu::SharedPointer<core::GPUResource> �؂�ւ���O�Ɏg�p���Ă���GPU���\�[�X
		*  @return    void
		*************************************************************************/
		virtual void PushAliasingBarrier(const gu::SharedPointer<core::GPUResource>& before  , const gu::SharedPointer<core::GPUResource>& after) override;
		
		/*!**********************************************************************
		*  @brief     Unordered access view��p�̏�ԃo���A @n
		*             UAV�̓ǂݏ������ɂق���UAV���Ώۃ��\�[�X��ǂݏ�������`��R�}���h�̎��s��h�����Ƃ�ړI�Ƃ��܂� @n
		*  @param[in] const gu::SharedPointer<core::GPUResource> Unordered access view������GPU���\�[�X
		*  @return    void
		*************************************************************************/
		virtual void PushUAVBarrier(const gu::SharedPointer<core::GPUResource>& buffer) override;
		
		/*!**********************************************************************
		*  @brief  �R�}���h���X�g���g����ResourceBarrier���܂Ƃ߂ČĂяo���܂�.
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		virtual void FlushResourceBarriers() override;
		
		#pragma endregion Resource Barrier

		#pragma endregion 

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     �R�}���h���X�g��Comptr��Ԃ��܂�
		*  @return CommandListComPtr �R�}���h���X�g
		*************************************************************************/
		CommandListComPtr GetCommandList() const noexcept { return _commandList; }

		/*!**********************************************************************
		*  @brief     �f�o�b�O�\������ݒ肵�܂�
		*  @param[in] const gu::tstring& �\���� 
		*************************************************************************/
		virtual void SetName(const gu::tstring& name) override;
		
		#pragma endregion

		#pragma region Public Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		RHICommandList() = default;

		/*! @brief �f�X�g���N�^*/
		~RHICommandList();

		/*! @brief �f�o�C�X�ƃR�}���h�A���P�[�^�����R���X�g���N�^�ł�*/
		explicit RHICommandList(const gu::SharedPointer<rhi::core::RHIDevice>& device, 
			const gu::SharedPointer<rhi::core::RHICommandAllocator>& commandAllocator, 
			const gu::tstring& name);
		#pragma endregion 

	protected:
		#pragma region Protected Function
		#pragma endregion 

		#pragma region Protected Property

		/*! @brief DirectX12�Ŏg�p����o���A���Ǘ�*/
		gu::SharedPointer<GPUBarrierBatcher> _barrierBatcher = nullptr;

		/*! @brief directX12�Ŏg�p����R�}���h���X�g*/
		CommandListComPtr _commandList = nullptr;

		#pragma endregion Protected Member Variable
		
	private:
		#pragma region Private Function
		void BeginRenderPassImpl(const gu::SharedPointer<directX12::RHIRenderPass>& renderPass, const gu::SharedPointer<directX12::RHIFrameBuffer>& frameBuffer);
		
		void OMSetFrameBuffer   (const gu::SharedPointer<directX12::RHIRenderPass>& renderPass, const gu::SharedPointer<directX12::RHIFrameBuffer>& frameBuffer);
		#pragma endregion Private Function

	};
}
#endif