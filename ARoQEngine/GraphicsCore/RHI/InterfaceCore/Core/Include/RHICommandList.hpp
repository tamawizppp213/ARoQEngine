//////////////////////////////////////////////////////////////////////////////////
///  @file   RHICommandList.hpp
///  @brief  GPU�̕`�施�߂��܂Ƃ߂��N���X. BeginRecording��EndRecording��1�t���[���̊J�n�ƏI�����ɌĂ�ł�������. 
///  @author Toide Yutaro
///  @date   2024_04_06
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_COMMANDLIST_HPP
#define RHI_COMMANDLIST_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "RHICommonState.hpp"
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
	class RHICommandAllocator;
	class RHIRenderPass;
	class RHIFrameBuffer;
	class RHIResourceLayout;
	class RHIQuery;
	struct QueryResultLocation;
	class GPUGraphicsPipelineState;
	class GPUComputePipelineState;
	class GPUResource;
	class GPUTexture;
	class GPUBuffer;
	class RHIDescriptorHeap;

	/****************************************************************************
	*				  			RHICommandList
	****************************************************************************/
	/* @class     RHICommandList
	*  @brief     GPU�̕`�施�߂��܂Ƃ߂��N���X. BeginRecording��EndRecording��1�t���[���̊J�n�ƏI�����ɌĂ�ł�������. 
	*****************************************************************************/
	class RHICommandList : public gu::NonCopyable
	{
	public:
		#pragma region Public Function
		#pragma region Main Draw Frame 
		/*!**********************************************************************
		*  @brief     �R�}���h���X�g���L�^��ԂɕύX���܂�. �����Draw�֐��̍ŏ��Ɏg�p���܂� @n
		*  @param[in] const bool stillMidFrame : 
		*             �`��t���[�����ɌĂ΂��ꍇ�ɃR�}���h�A���P�[�^�̒��g��Reset���邩�����肷��bool�l.@n
		*             �`��t���[�����ɌĂԂ̂�, �R�}���h���X�g��؂�ւ���ۂɎg�p�����\�������邽�߂ł�. 
		*************************************************************************/
		virtual void BeginRecording(const bool stillMidFrame = false) = 0;

		/*!**********************************************************************
		*  @brief     �R�}���h���X�g���L�^��Ԃ�����s�\��ԂɕύX���܂�. �����Draw�֐��̍Ō�Ɏg�p���܂�
		*************************************************************************/
		virtual void EndRecording  () = 0; // Call end function at end

		/*----------------------------------------------------------------------
		*  @brief : Start the render pass. This function basically called at draw function at start
		*----------------------------------------------------------------------*/
		virtual void BeginRenderPass(const gu::SharedPointer<RHIRenderPass>& renderPass, const gu::SharedPointer<RHIFrameBuffer>& frameBuffer) = 0;
		
		/*----------------------------------------------------------------------
		*  @brief : Finish the render pass. This function basically called at draw function at end
		*----------------------------------------------------------------------*/
		virtual void EndRenderPass() = 0;

		/*!**********************************************************************
		*  @brief     �R�}���h���X�g���L�^��ԂɕύX���܂�.�܂��R�}���h�A���P�[�^���̃R�}���h�o�b�t�@�̓��e��擪�ɖ߂��܂�.
		*  @param[in] �R�}���h�A���P�[�^��ύX���邩�ǂ���
		*  @attention ��{�I�ɂ�, Reset�ł͂Ȃ�BeginRecording���g�p���Ă�������.
		*************************************************************************/
		virtual void Reset(const gu::SharedPointer<RHICommandAllocator>& changeAllocator = nullptr) = 0;

		/*!**********************************************************************
		*  @brief     �R�}���h���X�g���A���P�[�^�ɐV�K�ɋl�ߍ��߂��Ԃɂ��邩���m�F���܂�
		*  @param[in] void
		*  @return    bool : �V�K�ɋl�ߍ��߂�ꍇ��true�ɂȂ�
		*************************************************************************/
		bool IsOpen() const { return _isOpen; }

		/*!**********************************************************************
		*  @brief     �R�}���h���X�g�����ĐV�K�ɃA���P�[�^�ɋl�ߍ��߂Ȃ���Ԃł��邩���m�F���܂�. @n
		*             ������ԂɂȂ�����, �R�}���h�L���[��Execute�֐����Ăяo����GPU���߂����s���Ă�������. 
		*  @param[in] void
		*  @return    bool : �R�}���h���X�g��������Ԃ�true��Ԃ�
		*************************************************************************/
		bool IsClosed() const { return !_isOpen; }

		#pragma endregion Main Draw Frame
		/*-------------------------------------------------------------------
		-               Common command
		---------------------------------------------------------------------*/
		virtual void SetResourceLayout(const gu::SharedPointer<RHIResourceLayout>& resourceLayout) = 0;
		virtual void SetDescriptorHeap(const gu::SharedPointer<RHIDescriptorHeap>& heap) = 0;
		//virtual void SetConstant32Bits(gu::DynamicArray<Value32Bit>& values) = 0;
		//virtual void CopyBuffer(const gu::SharedPointer<GPUBuffer>& source, const gu::SharedPointer<GPUBuffer>& destination, const size_t size, const size_t sourceOffset = 0, const size_t destinationOffset = 0) = 0;*/
		//virtual void TransitLayout(const gu::SharedPointer<GPUTexture>& texture, const ResourceLayout& newLayout) = 0;
		//virtual void TransitLayout(const gu::SharedPointer<GPUBuffer>& buffer, const ResourceLayout& newLayout) = 0;

		#pragma region Query
		/*----------------------------------------------------------------------
		*  @brief :  Starts the query to get GPU information.
		*----------------------------------------------------------------------*/
		virtual void BeginQuery(const QueryResultLocation& location) = 0;

		/*----------------------------------------------------------------------
		*  @brief :  End the query to get GPU information.
		*----------------------------------------------------------------------*/
		virtual void EndQuery(const QueryResultLocation& location) = 0;
		#pragma endregion Query

		#pragma region Graphics Command Function
		/*!**********************************************************************
		*  @brief     �[�x���w��͈̔͂ɓ����Ă��邩���e�X�g��, �͈͓��Ȃ�΃s�N�Z���V�F�[�_�[�𓮍삳���܂�.
		*  @param[in] const float : �ŏ��̐[�x���
		*  @param[in] const float : �ő�̐[�x���
		*  @return    void
		*************************************************************************/
		virtual void SetDepthBounds(const float minDepth, const float maxDepth) = 0;

		/*!**********************************************************************
		*  @brief     ���_���̂Ȃ�����ݒ肵�܂�.
		*  @param[in] �v���~�e�B�u�̃g�|���W�[���
		*************************************************************************/
		virtual void SetPrimitiveTopology(const PrimitiveTopology topology) = 0;

		/*!**********************************************************************
		*  @brief     �r���[�|�[�g�ɂ���ĕ`��̈��ݒ肵�܂�. �V�U�[��`��Viewport�ɍ��킹�Ď����Őݒ肵�܂�
		*  @param[in] const core::Viewport& : �`��̈�������P��̃r���[�|�[�g
		*************************************************************************/
		virtual void SetViewport(const core::Viewport& viewport) = 0;

		/*!**********************************************************************
		*  @brief     �r���[�|�[�g�̔z��(�A�h���X)�����ĕ`��̈��ݒ肵�܂�. �V�U�[��`��Viewport�ɍ��킹�Ď����Őݒ肵�܂�
		*  @param[in] const core::Viewport* : �`��̈���L�q�����z��, �������͒P���Viewport�̃A�h���X
		*  @param[in] const gu::uint32 : �r���[�|�[�g�̔z��
		*************************************************************************/
		virtual void SetViewport (const Viewport* viewport, const gu::uint32 numViewport = 1) = 0;
		
		/*!**********************************************************************
		*  @brief     VR�̂悤�ȗ��̎����s�����ɐݒ肷��`��̈�ł�. �V�U�[��`��Viewport�ɍ��킹�Ď����Őݒ肵�܂�
		*  @param[in] const core::Viewport& �����̎���������`��̈�
		*  @param[in] const core::Viewport& �E���̎���������`��̈�
		*************************************************************************/
		virtual void SetStereoViewport(const Viewport& leftView, const Viewport& rightView) = 0;

		/*!**********************************************************************
		*  @brief     �r���[�|�[�g���Ŏ��ۂɕ`�悳���̈�𐧌����邽�߂̃V�U�[��`���蓮�Őݒ肵�܂�. 
		*  @param[in] const core::ScissorRect* : �`��̈�𐧌����邽�߂̃V�U�[��`�̔z��
		*  @param[in] const gu::uint32 : �V�U�[��`�̔z��
		*************************************************************************/
		virtual void SetScissor (const ScissorRect* rect , const gu::uint32 numRect = 1) = 0;
		
		/*!**********************************************************************
		*  @brief     �`��̈�������r���[�|�[�g��, ���̒��Ŏ��ۂɕ`�悳���͈͂��w�肷��V�U�[��`�����ꂼ��蓮�Őݒ肵�܂�.
		*  @param[in] const core::Viewport& �`��̈�������r���[�|�[�g
		*  @param[in] const core::ScissorRect& ���ۂɕ`�悳���͈͂������V�U�[��`
		*************************************************************************/
		virtual void SetViewportAndScissor(const Viewport& viewport, const ScissorRect& rect)       = 0;
		
		virtual void SetVertexBuffer(const gu::SharedPointer<GPUBuffer>& buffer) = 0;
		
		virtual void SetVertexBuffers     (const gu::DynamicArray<gu::SharedPointer<GPUBuffer>>& buffers, const size_t startSlot = 0) = 0;
		
		virtual void SetIndexBuffer       (const gu::SharedPointer<GPUBuffer>& buffer, const PixelFormat indexType = PixelFormat::R32_UINT) = 0;
		
		virtual void SetGraphicsPipeline  (const gu::SharedPointer<GPUGraphicsPipelineState>& pipeline) = 0;
		
		virtual void DrawIndexed          (gu::uint32 indexCount, gu::uint32 startIndexLocation = 0, gu::uint32 baseVertexLocation = 0) = 0;
		
		/*----------------------------------------------------------------------
		*  @brief : �C���f�b�N�X�����Ă��郂�f���ł���, �C���X�^���V���O�`�悪�K�v�ƂȂ�v���~�e�B�u��`�悵�܂�.
		*           indexCountPerInstance : �C���X�^���X���ɕK�v�ƂȂ�C���f�b�N�X�̑���
		*           instance Count        : �C���X�^���X�̐�
		*           startIndexLocation    : �C���f�b�N�X��ǂݎ��n�߂�, �C���f�b�N�X�o�b�t�@���̔z��v�f��
		* �@�@�@�@�@�@ baseVertexLocation    : ���_�o�b�t�@�[���璸�_��ǂݎ��O��, �e�C���f�b�N�X�ɒǉ�����l
		*           startInstanceLocation : �`����s���ŏ��̃C���X�^���X�ԍ�
		*----------------------------------------------------------------------*/
		virtual void DrawIndexedInstanced (gu::uint32 indexCountPerInstance, gu::uint32 instanceCount, gu::uint32 startIndexLocation = 0, gu::uint32 baseVertexLocation = 0, gu::uint32 startInstanceLocation = 0) = 0;
		
		/*-------------------------------------------------------------------
		-                Compute Command
		---------------------------------------------------------------------*/
		virtual void SetComputeResourceLayout(const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout) = 0;
		
		virtual void SetComputePipeline(const gu::SharedPointer<GPUComputePipelineState>& pipeline) = 0;
		
		virtual void Dispatch(gu::uint32 threadGroupCountX  = 1, gu::uint32 threadGroupCountY = 1, gu::uint32 threadGroupCountZ = 1) = 0;
		
		/*----------------------------------------------------------------------
		*  @brief :�C���f�b�N�X�o�b�t�@�������f���ɑ΂���, �����o�b�t�@��GPU�Őݒ�, �`������s�o����֐��ł�
		*----------------------------------------------------------------------*/
		virtual void DrawIndexedIndirect(const gu::SharedPointer<core::GPUBuffer>& argumentBuffer, const gu::uint32 drawCallCount) = 0;

		/*----------------------------------------------------------------------
		*  @brief :Mesh shader�Ŏg�p����`��֐��ł�.
		*----------------------------------------------------------------------*/
		virtual void DispatchMesh(const gu::uint32 threadGroupCountX = 1, const gu::uint32 threadGroupCountY = 1, const gu::uint32 threadGroupCountZ = 1) = 0;
		
		/*-------------------------------------------------------------------
		-                RayTracing Command
		---------------------------------------------------------------------*/
		
		#pragma region Copy
		/*!**********************************************************************
		*  @brief     �e�N�X�`���̗̈��S�Ă�Subresource���݂ŕʂ̃e�N�X�`���ɃR�s�[����
		*  @param[in] const gu::SharedPointer<core::GPUTexture> : �R�s�[��̃e�N�X�`��
		*  @param[in] const gu::SharedPointer<core::GPUTexture> : �R�s�[���̃e�N�X�`��
		*  @return void
		*************************************************************************/
		virtual void CopyResource(const gu::SharedPointer<GPUTexture>& dest, const gu::SharedPointer<GPUTexture>& source) = 0;
		
		/*!**********************************************************************
		*  @brief     GPU�o�b�t�@�̗̈������GPU�|�C���^����ʂ�GPU�|�C���^�ɃR�s�[���s��. GPU�ł�memcpy
		*  @param[in] const gu::SharedPointer<core::GPUBuffer> : �R�s�[��̃o�b�t�@
		*  @param[in] const gu::uint64 : �R�s�[��̏����������|�C���^�����炷offset byte
		*  @param[in] const gu::SharedPointer<core::GPUBuffer> : �R�s�[���̃o�b�t�@
		*  @param[in] const gu::uint64 : �R�s�[���̏����ǂݎ��|�C���^�����炷offset byte
		*  @param[in] const gu::uint64 : �R�s�[���s���͈� (byteSize�Ŏw��)
		*  @return void
		*************************************************************************/
		virtual void CopyBufferRegion(const gu::SharedPointer<GPUBuffer>& dest, const gu::uint64 destOffset, const gu::SharedPointer<GPUBuffer>& source, const gu::uint64 sourceOffset, const gu::uint64 copyByteSize) = 0;
		
		/*!**********************************************************************
		*  @brief     GPU�e�N�X�`���̗̈������GPU�|�C���^����ʂ�GPU�|�C���^�ɃR�s�[���s��. GPU�ł�memcpy
		*  @param[in] const gu::SharedPointer<core::GPUTexture> : �R�s�[��̃e�N�X�`��
		*  @param[in] const gu::SharedPointer<core::GPUTexture> : �R�s�[���̃e�N�X�`��
		*  @param[in] const core::GPUTextureCopyInfo : �R�s�[����ۂ̏��
		*  @return void
		*************************************************************************/
		virtual void CopyTextureRegion(const gu::SharedPointer<core::GPUTexture>& destination, const gu::SharedPointer<core::GPUTexture>& source, const core::GPUTextureCopyInfo& copyInfo) = 0;
		#pragma endregion Copys

		#pragma region Resource Barrier
		/*!**********************************************************************
		*  @brief  �P�Ƃ�GPU���\�[�X�̏�ԑJ�ڂ������܂�. @n
		*          ���\�[�X�̎g�������ς��^�C�~���O�ŌĂяo���܂�.
		*  @param[in] const gu::SharedPointer<core::GPUResource> GPU���\�[�X
		*  @param[in] const core::ResourceState �J�ڑO�̃��\�[�X���
		*  @param[in] const core::ResourceState �J�ڌ�̃��\�[�X���
		*  @param[in] const gu::uint32 �T�u���\�[�X�������C���f�b�N�X (�f�t�H���g�͑S�ẴT�u���\�[�X�������܂�) 0xfffffff�̏ꍇ�͑S�ẴC���f�b�N�X�ŗL��������܂�
		*  @return    void
		*************************************************************************/
		virtual void PushTransitionBarrier(const gu::SharedPointer<core::GPUResource>& buffer  , const core::ResourceState after, const gu::uint32 subresource = 0xffffffff) = 0;
		
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
		virtual void PushAliasingBarrier(const gu::SharedPointer<core::GPUResource>& before  , const gu::SharedPointer<core::GPUResource>& after) = 0;
		
		/*!**********************************************************************
		*  @brief     Unordered access view��p�̏�ԃo���A @n
		*             UAV�̓ǂݏ������ɂق���UAV���Ώۃ��\�[�X��ǂݏ�������`��R�}���h�̎��s��h�����Ƃ�ړI�Ƃ��܂� @n
		*  @param[in] const gu::SharedPointer<core::GPUTexture> Unordered access view������GPU�o�b�t�@
		*  @return    void
		*************************************************************************/
		virtual void PushUAVBarrier(const gu::SharedPointer<core::GPUResource>& texture) = 0;

		/*!**********************************************************************
		*  @brief  �R�}���h���X�g���g����ResourceBarrier���܂Ƃ߂ČĂяo���܂�.
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		virtual void FlushResourceBarriers() = 0;
		
		#pragma endregion Resource Barrier

		#pragma endregion 
		
		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     �R�}���h���X�g�̃��������i�[����R�}���h�A���P�[�^�̃|�C���^��Ԃ��܂�
		*  @return    gu::SharedPointer<RHICommandAllocator> �R�}���h�A���P�[�^�̃|�C���^
		*************************************************************************/
		gu::SharedPointer<RHICommandAllocator> GetCommandAllocator() const noexcept { return _commandAllocator; }
		
		/*!**********************************************************************
		*  @brief     ���݂̃R�}���h���X�g�̎�ނ�Ԃ��܂�. 
		*  @return    CommandListType �R�}���h���X�g�̎�� [graphics, copy, compute]
		*************************************************************************/
		CommandListType GetType() const { return _commandListType; }
		
		/*!**********************************************************************
		*  @brief     ���̃R�}���h���X�g����肷�邽�߂̌ŗLID�ł�. ���t���[���R�}���h���X�g��Reset���ꂽ�Ƃ��Ă��c��܂�.�@���݂͓��Ɏg�p���Ă��Ȃ�
		*  @return    �R�}���h���X�g����肷��ŗLID
		*************************************************************************/
		gu::uint64 GetID() const { return _commandListID; }

		/*!**********************************************************************
		*  @brief     �_���f�o�C�X��ݒ肵�܂�. 
		*************************************************************************/
		void SetDevice(gu::SharedPointer<RHIDevice> device) { _device = device; }

		/*!**********************************************************************
		*  @brief     �f�o�b�O����ݒ肵�܂�
		*************************************************************************/
		virtual void SetName(const gu::tstring& name) = 0;

		#pragma endregion

		#pragma region Public Constructor and Destructor 

		#pragma endregion
	protected:
		#pragma region Protected Constructor and Destructor 

		/*! @brief �f�t�H���g�R���X�g���N�^*/
		RHICommandList() = default;

		/*! @brief �f�X�g���N�^*/
		virtual ~RHICommandList() = default;

		/*! @brief �_���f�o�C�X�ƃR�}���h�𒙂߂�A���P�[�^���g���Đ�������R���X�g���N�^*/
		explicit RHICommandList(
			const gu::SharedPointer<RHIDevice>& device,
			const gu::SharedPointer<RHICommandAllocator>& commandAllocator)
			: _device(device), _commandAllocator(commandAllocator) { };

		#pragma endregion
		
		#pragma region Protected Function
		#pragma endregion

		#pragma region Protected Property

		/*! @brief �_���f�o�C�X*/
		gu::SharedPointer<RHIDevice> _device = nullptr;

		/*! @brief �R�}���h���X�g�𒙂߂Ă����A���P�[�^*/
		gu::SharedPointer<RHICommandAllocator> _commandAllocator = nullptr;

		/*! @brief �`��p�̃����_�[�p�X*/
		gu::SharedPointer<core::RHIRenderPass> _renderPass = nullptr;

		/*! @brief �t���[���o�b�t�@*/
		gu::SharedPointer<core::RHIFrameBuffer>_frameBuffer = nullptr;

		/*! @brief �R�}���h���X�g����肷��ŗLID*/
		gu::uint64 _commandListID = 0;

		/*! @brief �R�}���h���X�g�̎��*/
		core::CommandListType _commandListType = CommandListType::Unknown;

		/*! @brief : �R�}���h���X�g���l�ߍ��߂��Ԃɂ��邩���m�F���܂�*/
		bool _isOpen = false;

		/*! @brief : �����_�[�p�X�����s���Ă��邩*/
		bool _beginRenderPass = false;

		#pragma endregion
		
	};
}
#endif