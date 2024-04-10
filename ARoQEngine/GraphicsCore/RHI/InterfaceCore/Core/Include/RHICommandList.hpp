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
	*************************************************************************//**
	*  @class     RHICommandList
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
		/*----------------------------------------------------------------------*/
		virtual void BeginRenderPass(const gu::SharedPointer<RHIRenderPass>& renderPass, const gu::SharedPointer<RHIFrameBuffer>& frameBuffer) = 0;
		
		/*----------------------------------------------------------------------
		*  @brief : Finish the render pass. This function basically called at draw function at end
		/*----------------------------------------------------------------------*/
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
		/*----------------------------------------------------------------------*/
		virtual void BeginQuery(const QueryResultLocation& location) = 0;

		/*----------------------------------------------------------------------
		*  @brief :  End the query to get GPU information.
		/*----------------------------------------------------------------------*/
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
		
		virtual void SetIndexBuffer       (const gu::SharedPointer<GPUBuffer>& buffer, const IndexType indexType = IndexType::UInt32) = 0;
		
		virtual void SetGraphicsPipeline  (const gu::SharedPointer<GPUGraphicsPipelineState>& pipeline) = 0;
		
		virtual void DrawIndexed          (gu::uint32 indexCount, gu::uint32 startIndexLocation = 0, gu::uint32 baseVertexLocation = 0) = 0;
		
		/*----------------------------------------------------------------------
		*  @brief : �C���f�b�N�X�����Ă��郂�f���ł���, �C���X�^���V���O�`�悪�K�v�ƂȂ�v���~�e�B�u��`�悵�܂�.
		*           indexCountPerInstance : �C���X�^���X���ɕK�v�ƂȂ�C���f�b�N�X�̑���
		*           instance Count        : �C���X�^���X�̐�
		*           startIndexLocation    : �C���f�b�N�X��ǂݎ��n�߂�, �C���f�b�N�X�o�b�t�@���̔z��v�f��
		* �@�@�@�@�@�@ baseVertexLocation    : ���_�o�b�t�@�[���璸�_��ǂݎ��O��, �e�C���f�b�N�X�ɒǉ�����l
		*           startInstanceLocation : �`����s���ŏ��̃C���X�^���X�ԍ�
		/*----------------------------------------------------------------------*/
		virtual void DrawIndexedInstanced (gu::uint32 indexCountPerInstance, gu::uint32 instanceCount, gu::uint32 startIndexLocation = 0, gu::uint32 baseVertexLocation = 0, gu::uint32 startInstanceLocation = 0) = 0;
		
		/*-------------------------------------------------------------------
		-                Compute Command
		---------------------------------------------------------------------*/
		virtual void SetComputeResourceLayout(const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout) = 0;
		
		virtual void SetComputePipeline(const gu::SharedPointer<GPUComputePipelineState>& pipeline) = 0;
		
		virtual void Dispatch(gu::uint32 threadGroupCountX  = 1, gu::uint32 threadGroupCountY = 1, gu::uint32 threadGroupCountZ = 1) = 0;
		
		/*----------------------------------------------------------------------
		*  @brief :�C���f�b�N�X�o�b�t�@�������f���ɑ΂���, �����o�b�t�@��GPU�Őݒ�, �`������s�o����֐��ł�
		/*----------------------------------------------------------------------*/
		virtual void DrawIndexedIndirect(const gu::SharedPointer<core::GPUBuffer>& argumentBuffer, const gu::uint32 drawCallCount) = 0;

		/*----------------------------------------------------------------------
		*  @brief :Mesh shader�Ŏg�p����`��֐��ł�.
		/*----------------------------------------------------------------------*/
		virtual void DispatchMesh(const gu::uint32 threadGroupCountX = 1, const gu::uint32 threadGroupCountY = 1, const gu::uint32 threadGroupCountZ = 1) = 0;
		
		/*-------------------------------------------------------------------
		-                RayTracing Command
		---------------------------------------------------------------------*/
		
		#pragma region Copy
		/*-------------------------------------------------------------------
		-                Copy Resource
		---------------------------------------------------------------------*/
		virtual void CopyResource(const gu::SharedPointer<GPUTexture>& dest, const gu::SharedPointer<GPUTexture>& source) = 0;
		
		/*----------------------------------------------------------------------
		*  @brief : �o�b�t�@�̗̈�����郊�\�[�X����ʂ̃��\�[�X�ɃR�s�[����. 
		*           GPU��memcpy
		/*----------------------------------------------------------------------*/
		virtual void CopyBufferRegion(const gu::SharedPointer<GPUBuffer>& dest, const gu::uint64 destOffset, const gu::SharedPointer<GPUBuffer>& source, const gu::uint64 sourceOffset, const gu::uint64 copyByteSize) = 0;
		#pragma endregion Copys
		/*-------------------------------------------------------------------
		-                Transition layout
		---------------------------------------------------------------------*/
		virtual void TransitionResourceState (const gu::SharedPointer<core::GPUTexture>& texture, core::ResourceState after) = 0;
		
		virtual void TransitionResourceStates(const gu::uint32 numStates, const gu::SharedPointer<core::GPUTexture>* textures, core::ResourceState* afters) = 0;
		
		#pragma endregion
		#pragma endregion 
		
		#pragma region Public Member Variables
		gu::SharedPointer<RHICommandAllocator> GetCommandAllocator() const noexcept { return _commandAllocator; }
		
		/* @brief : Command list type (graphics, copy, or compute)*/
		CommandListType GetType() const { return _commandListType; }
		
		/* @brief : ���̃R�}���h���X�g����肷�邽�߂̌ŗLID�ł�. ���t���[���R�}���h���X�g��Reset���ꂽ�Ƃ��Ă��c��܂�.*/
		std::uint64_t GetID() const { return _commandListID; }

		/* @brief : �f�o�C�X���Z�b�g���܂�. */
		void SetDevice(gu::SharedPointer<RHIDevice> device) { _device = device; }

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

		#pragma region Protected Member Variables

		/*! @brief �_���f�o�C�X*/
		gu::SharedPointer<RHIDevice> _device = nullptr;

		/*! @brief �R�}���h���X�g�𒙂߂Ă����A���P�[�^*/
		gu::SharedPointer<RHICommandAllocator> _commandAllocator = nullptr;

		gu::SharedPointer<core::RHIRenderPass> _renderPass = nullptr;

		gu::SharedPointer<core::RHIFrameBuffer>_frameBuffer = nullptr;

		gu::uint64 _commandListID = 0;

		core::CommandListType _commandListType = CommandListType::Unknown;

		/* @brief : �R�}���h���X�g���l�ߍ��߂��Ԃɂ��邩���m�F���܂�*/
		bool _isOpen = false;

		bool _beginRenderPass = false;

		#pragma endregion
		
	};
}
#endif