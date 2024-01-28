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

	/****************************************************************************
	*				  			RHIDevice
	*************************************************************************//**
	*  @class     RHIDevice
	*  @brief     Device interface
	*****************************************************************************/
	class RHICommandList : public rhi::core::RHICommandList, public gu::EnableSharedFromThis<RHICommandList>
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : �R�}���h���X�g���L�^��ԂɕύX���܂�. �����Draw�֐��̍ŏ��Ɏg�p���܂�
		*           stillMidFrame��, �R�}���h�A���P�[�^�[�̒��g��Reset���邩�����肵�܂�.
		/*----------------------------------------------------------------------*/
		void BeginRecording(const bool stillMidFrame) override;

		/*----------------------------------------------------------------------
		*  @brief : �R�}���h���X�g���L�^��Ԃ�����s�\��ԂɕύX���܂�. �����Draw�֐��̍Ō�Ɏg�p���܂�
		/*----------------------------------------------------------------------*/
		void EndRecording  () override;

		/*----------------------------------------------------------------------
		*  @brief : RenderPass���J�n���܂�.��{�I�ɂ͊eDraw�֐���BeginRecording�̌�ɌĂ΂�܂�
		/*----------------------------------------------------------------------*/
		void BeginRenderPass(const gu::SharedPointer<core::RHIRenderPass>& renderPass, const gu::SharedPointer<core::RHIFrameBuffer>& frameBuffer) override;
		
		/*----------------------------------------------------------------------
		*  @brief : RenderPass���I�����܂�.��{�I�ɂ͊eDraw�֐���EndRecording�O�ɌĂ΂�܂�
		/*----------------------------------------------------------------------*/
		void EndRenderPass() override;

		/*----------------------------------------------------------------------
		*  @brief : Proceed to the record state. �R�}���h���X�g���L�^��ԂɕύX���܂�.
		            ��{�I�ɂ�, Reset�ł͂Ȃ�BeginRecording���g�p���Ă�������.
		/*----------------------------------------------------------------------*/
		void Reset(const gu::SharedPointer<core::RHICommandAllocator>& changeAllocator = nullptr) override;

		/*-------------------------------------------------------------------
		-               Common command
		---------------------------------------------------------------------*/
		void SetDescriptorHeap(const gu::SharedPointer<core::RHIDescriptorHeap>& heap) override;

#pragma region Query
		/*----------------------------------------------------------------------
		*  @brief : GPU�����擾���邽�߂̃N�G�����J�n���܂�
		/*----------------------------------------------------------------------*/
		void BeginQuery(const core::QueryResultLocation& location) override;

		/*----------------------------------------------------------------------
		*  @brief : GPU�����擾���邽�߂̃N�G�����I�����܂�
		/*----------------------------------------------------------------------*/
		void EndQuery(const core::QueryResultLocation& location) override;
#pragma endregion Query
#pragma region Graphics Command Function
		/*-------------------------------------------------------------------
		-                Graphics Command
		---------------------------------------------------------------------*/
		/*----------------------------------------------------------------------
		*  @brief : �[�x���w��͈̔͂ɓ����Ă��邩���e�X�g��, �͈͓��Ȃ�΃s�N�Z���V�F�[�_�[�𓮍삳���܂�.
		/*----------------------------------------------------------------------*/
		void SetDepthBounds(const float minDepth, const float maxDepth) override;

		void SetPrimitiveTopology(const core::PrimitiveTopology topology) override;

		void SetViewport(const core::Viewport* viewport, const std::uint32_t numViewport = 1) override;
		
		void SetScissor (const core::ScissorRect* rect, const std::uint32_t numRect = 1) override;
		
		void SetViewportAndScissor(const core::Viewport& viewport, const core::ScissorRect& rect) override;
		
		void SetResourceLayout(const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout) override;
		
		void SetGraphicsPipeline(const gu::SharedPointer<core::GPUGraphicsPipelineState>& pipelineState) override;
		
		void SetVertexBuffer(const gu::SharedPointer<core::GPUBuffer>& buffer) override ;
		
		void SetVertexBuffers(const std::vector<gu::SharedPointer<core::GPUBuffer>>& buffers, const size_t startSlot = 0) override;
		
		void SetIndexBuffer(const gu::SharedPointer<core::GPUBuffer>& buffer, const core::IndexType indexType = core::IndexType::UInt32) override;
		
		/*----------------------------------------------------------------------
		*  @brief : �C���f�b�N�X�����Ă��郂�f���ł���, �C���X�^���V���O�`�悪�K�v�ƂȂ�v���~�e�B�u��`�悵�܂�.
		*           indexCountPerInstance : �C���X�^���X���ɕK�v�ƂȂ�C���f�b�N�X�̑���
		*           instance Count        : �C���X�^���X�̐�
		*           startIndexLocation    : �C���f�b�N�X��ǂݎ��n�߂�, �C���f�b�N�X�o�b�t�@���̔z��v�f��
		* �@�@�@�@�@�@ baseVertexLocation    : ���_�o�b�t�@�[���璸�_��ǂݎ��O��, �e�C���f�b�N�X�ɒǉ�����l
		*           startInstanceLocation : �`����s���ŏ��̃C���X�^���X�ԍ�
		/*----------------------------------------------------------------------*/
		void DrawIndexedInstanced(std::uint32_t indexCountPerInstance, std::uint32_t instanceCount, std::uint32_t startIndexLocation = 0, std::uint32_t baseVertexLocation = 0, std::uint32_t startInstanceLocation = 0) override;
		
		/*----------------------------------------------------------------------
		*  @brief : �C���f�b�N�X�����Ă��郂�f���ł���, �C���X�^���V���O�`�悪�K�v�Ȃ��v���~�e�B�u��`�悵�܂�.
		/*----------------------------------------------------------------------*/
		void DrawIndexed(std::uint32_t indexCount, std::uint32_t startIndexLocation = 0, std::uint32_t baseVertexLocation = 0) override;
		
		/*----------------------------------------------------------------------
		*  @brief :�C���f�b�N�X�o�b�t�@�������f���ɑ΂���, �����o�b�t�@��GPU�Őݒ�, �`������s�o����֐��ł�
		/*----------------------------------------------------------------------*/
		void DrawIndexedIndirect(const gu::SharedPointer<core::GPUBuffer>& argumentBuffer, const std::uint32_t drawCallCount) override;

		/*----------------------------------------------------------------------
		*  @brief :Mesh shader�Ŏg�p����`��֐��ł�. 
		/*----------------------------------------------------------------------*/
		void DispatchMesh(const std::uint32_t threadGroupCountX = 1, const std::uint32_t threadGroupCountY = 1, const std::uint32_t threadGroupCountZ = 1) override;
#pragma endregion Graphics Command Function
		/*-------------------------------------------------------------------
		-                Compute Command
		---------------------------------------------------------------------*/
		void SetComputeResourceLayout(const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout) override;
		
		void SetComputePipeline(const gu::SharedPointer<core::GPUComputePipelineState>& pipeline) override;
		
		void Dispatch(std::uint32_t threadGroupCountX = 1, std::uint32_t threadGroupCountY = 1, std::uint32_t threadGroupCountZ = 1) override;
		
		/*-------------------------------------------------------------------
		-                Transition layout
		---------------------------------------------------------------------*/
		void TransitionResourceState (const gu::SharedPointer<core::GPUTexture>& texture, core::ResourceState after) override ;

		void TransitionResourceStates(const std::uint32_t numStates, const gu::SharedPointer<core::GPUTexture>* textures, core::ResourceState* afters) override ;

		void TransitionResourceStates(const std::vector<gu::SharedPointer<core::GPUResource>>& resources, core::ResourceState* afters);
		
		/*-------------------------------------------------------------------
		-                Copy
		---------------------------------------------------------------------*/
		/*----------------------------------------------------------------------
		*  @brief : �o�b�t�@�̗̈�����郊�\�[�X����ʂ̃��\�[�X�ɃR�s�[����. GPU memcpy
		/*----------------------------------------------------------------------*/
		void CopyBufferRegion(const gu::SharedPointer<core::GPUBuffer>& dest, const gu::uint64 destOffset, const gu::SharedPointer<core::GPUBuffer>& source, const gu::uint64 sourceOffset, const gu::uint64 copyByteSize) override;

		/*----------------------------------------------------------------------
		*  @brief : �e�N�X�`���̗̈���܂Ƃ߂ĕʂ̃��\�[�X�ɃR�s�[����
		/*----------------------------------------------------------------------*/
		void CopyResource(const gu::SharedPointer<core::GPUTexture>& dest, const gu::SharedPointer<core::GPUTexture>& source) override;
		
		/*----------------------------------------------------------------------
		*  @brief : ���郊�\�[�X�̗̈���܂Ƃ߂ĕʂ̃��\�[�X�ɃR�s�[����. 
		*           �g�ݍ��킹�ɉ����Ď����Ńo�b�t�@���e�N�X�`�����𔻒肵�܂�
		/*----------------------------------------------------------------------*/
		void CopyResource(const gu::SharedPointer<core::GPUResource>& dest, const gu::SharedPointer<core::GPUResource>& source);
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		CommandListComPtr GetCommandList() const noexcept { return _commandList; }

		void SetName(const gu::tstring& name) override;
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHICommandList() = default;

		~RHICommandList();

		explicit RHICommandList(const gu::SharedPointer<rhi::core::RHIDevice>& device, const gu::SharedPointer<rhi::core::RHICommandAllocator>& commandAllocator, 
			const gu::tstring& name);
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		CommandListComPtr _commandList = nullptr;
		
	private:
		void BeginRenderPassImpl(const gu::SharedPointer<directX12::RHIRenderPass>& renderPass, const gu::SharedPointer<directX12::RHIFrameBuffer>& frameBuffer);
		
		void OMSetFrameBuffer   (const gu::SharedPointer<directX12::RHIRenderPass>& renderPass, const gu::SharedPointer<directX12::RHIFrameBuffer>& frameBuffer);
	};
}
#endif