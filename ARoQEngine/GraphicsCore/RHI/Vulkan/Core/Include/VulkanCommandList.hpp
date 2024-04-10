//////////////////////////////////////////////////////////////////////////////////
///             @file   VulkanCommandList.hpp
///             @brief  CommandList
///             @author Toide Yutaro
///             @date   2022_06_09
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VULKAN_COMMANDLIST_HPP
#define VULKAN_COMMANDLIST_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommandList.hpp"
#include <vulkan/vulkan.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#pragma warning(disable : 4100)
//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::vulkan
{
	class RHIResourceLayout;
	class GPUGraphicsPipelineState;
	/****************************************************************************
	*				  			TemplateClass
	*************************************************************************//**
	*  @class     TemplateClass
	*  @brief     temp
	*****************************************************************************/
	class RHICommandList : public rhi::core::RHICommandList, public gu::EnableSharedFromThis<RHICommandList>
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : This function must be called at draw function initially (stillMidFrame = false).
					If still mid frame is set false, this function clears the command allocator.*/
		void BeginRecording(const bool stillMidFrame) override; 
		
		void EndRecording () override;  // end call   at draw frame
		
		void BeginRenderPass(const gu::SharedPointer<core::RHIRenderPass>& renderPass, const gu::SharedPointer<core::RHIFrameBuffer>& frameBuffer) override;
		
		void EndRenderPass() override;
		
		/* @brief : Proceed to the record state.*/
		void Reset(const gu::SharedPointer<core::RHICommandAllocator>& changeAllocator) override {};

#pragma region Query
		/*----------------------------------------------------------------------
		*  @brief : GPU�����擾���邽�߂̃N�G�����J�n���܂�
		/*----------------------------------------------------------------------*/
		void BeginQuery(const core::QueryResultLocation& location) override {};

		/*----------------------------------------------------------------------
		*  @brief : GPU�����擾���邽�߂̃N�G�����I�����܂�
		/*----------------------------------------------------------------------*/
		void EndQuery(const core::QueryResultLocation& location) override {};

#pragma endregion Query
		/*-------------------------------------------------------------------
		-               Graphic Pipeline command
		---------------------------------------------------------------------*/
		void SetDepthBounds(const float minDepth, const float maxDepth) override {};

		void SetDescriptorHeap(const gu::SharedPointer<core::RHIDescriptorHeap>& heap) override {};
		
		void SetResourceLayout(const gu::SharedPointer<core::RHIResourceLayout>& layout) override;
		
		void SetGraphicsPipeline(const gu::SharedPointer<core::GPUGraphicsPipelineState>& pipeline);
		
		void SetPrimitiveTopology(const core::PrimitiveTopology topology) override;
		
		/*!**********************************************************************
		*  @brief     �r���[�|�[�g�ɂ���ĕ`��̈��ݒ肵�܂�. �V�U�[��`��Viewport�ɍ��킹�Ď����Őݒ肵�܂�
		*  @param[in] const core::Viewport& : �`��̈�������P��̃r���[�|�[�g
		*************************************************************************/
		virtual void SetViewport(const core::Viewport& viewport) override {  };

		/*!**********************************************************************
		*  @brief     �r���[�|�[�g�̔z��(�A�h���X)�����ĕ`��̈��ݒ肵�܂�. �V�U�[��`��Viewport�ɍ��킹�Ď����Őݒ肵�܂�
		*  @param[in] const core::Viewport* : �`��̈���L�q�����z��, �������͒P���Viewport�̃A�h���X
		*  @param[in] const gu::uint32 : �r���[�|�[�g�̔z�� (Default��1)
		*************************************************************************/
		void SetViewport (const core::Viewport* viewport, const gu::uint32 numViewport = 1)override;
		
		/*!**********************************************************************
		*  @brief     VR�̂悤�ȗ��̎����s�����ɐݒ肷��`��̈�ł�
		*  @param[in] const core::Viewport& �����̎���������`��̈�
		*  @param[in] const core::Viewport& �E���̎���������`��̈�
		*************************************************************************/
		virtual void SetStereoViewport(const core::Viewport& leftView, const core::Viewport& rightView) override {};

		/*!**********************************************************************
		*  @brief     �r���[�|�[�g���Ŏ��ۂɕ`�悳���̈�𐧌����邽�߂̃V�U�[��`���蓮�Őݒ肵�܂�.
		*  @param[in] const core::ScissorRect* : �`��̈�𐧌����邽�߂̃V�U�[��`�̔z��
		*  @param[in] const gu::uint32 : �V�U�[��`�̔z��
		*************************************************************************/
		virtual void SetScissor(const core::ScissorRect* rect, const gu::uint32 numRect = 1) override;

		/*!**********************************************************************
		*  @brief     �`��̈�������r���[�|�[�g��, ���̒��Ŏ��ۂɕ`�悳���͈͂��w�肷��V�U�[��`�����ꂼ��蓮�Őݒ肵�܂�.
		*  @param[in] const core::Viewport& �`��̈�������r���[�|�[�g
		*  @param[in] const core::ScissorRect& ���ۂɕ`�悳���͈͂������V�U�[��`
		*************************************************************************/
		virtual void SetViewportAndScissor(const core::Viewport& viewport, const core::ScissorRect& rect) override;

		/*-------------------------------------------------------------------
		-                Graphics Command
		---------------------------------------------------------------------*/
		void SetComputePipeline(const gu::SharedPointer<core::GPUComputePipelineState>& pipeline) override {};
		
		void SetVertexBuffer (const gu::SharedPointer<core::GPUBuffer>& buffer) override;
		
		void SetVertexBuffers(const gu::DynamicArray<gu::SharedPointer<core::GPUBuffer>>& buffers, const size_t startSlot = 0) override;
		
		void SetIndexBuffer  (const gu::SharedPointer<core::GPUBuffer>& buffer, const core::IndexType indexType = core::IndexType::UInt32) override;
		
		void DrawIndexed(gu::uint32 indexCount, gu::uint32 startIndexLocation = 0, gu::uint32 baseVertexLocation = 0)override;
		
		void DrawIndexedInstanced(gu::uint32 indexCountPerInstance, gu::uint32 instanceCount, gu::uint32 startIndexLocation = 0, gu::uint32 baseVertexLocation = 0, gu::uint32 startInstanceLocation = 0)override;
		
		/*----------------------------------------------------------------------
		*  @brief :�C���f�b�N�X�o�b�t�@�������f���ɑ΂���, �����o�b�t�@��GPU�Őݒ�, �`������s�o����֐��ł�
		/*----------------------------------------------------------------------*/
		void DrawIndexedIndirect(const gu::SharedPointer<core::GPUBuffer>& argumentBuffer, const gu::uint32 drawCallCount) override {};

		/*----------------------------------------------------------------------
		*  @brief :Mesh shader�Ŏg�p����`��֐��ł�.
		/*----------------------------------------------------------------------*/
		void DispatchMesh(const gu::uint32 threadGroupCountX = 1, const gu::uint32 threadGroupCountY = 1, const gu::uint32 threadGroupCountZ = 1) override {};
		/*-------------------------------------------------------------------
		-                Compute Command
		---------------------------------------------------------------------*/
		void SetComputeResourceLayout(const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout) override{};
		
		void Dispatch(gu::uint32 threadGroupCountX = 1, gu::uint32 threadGroupCountY = 1, gu::uint32 threadGroupCountZ = 1)override;
		/*-------------------------------------------------------------------
		-                Transition Resource State
		---------------------------------------------------------------------*/
		void TransitionResourceState(const gu::SharedPointer<core::GPUTexture>& texture, core::ResourceState after) override;
		
		void TransitionResourceStates(const gu::uint32 numStates, const gu::SharedPointer<core::GPUTexture>* textures, core::ResourceState* afters) override;
		
		void CopyResource(const gu::SharedPointer<core::GPUTexture>& dest, const gu::SharedPointer<core::GPUTexture>& source) override {};;
		
		/*----------------------------------------------------------------------
		*  @brief : �o�b�t�@�̗̈�����郊�\�[�X����ʂ̃��\�[�X�ɃR�s�[����. GPU��memcpy
		/*----------------------------------------------------------------------*/
		void CopyBufferRegion(const gu::SharedPointer<core::GPUBuffer>& dest, const gu::uint64 destOffset, const gu::SharedPointer<core::GPUBuffer>& source, const gu::uint64 sourceOffset, const gu::uint64 copyByteSize) override {};

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		VkCommandBuffer GetCommandList() { return _commandBuffer; }
		
		void SetName(const gu::tstring& name);
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHICommandList() = default;
		
		~RHICommandList();
		
		explicit RHICommandList(
			const gu::SharedPointer<core::RHIDevice>& device,
			const gu::SharedPointer<core::RHICommandAllocator>& allocator,
			const gu::tstring& name);
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		VkCommandBuffer _commandBuffer = nullptr;
		
		gu::SharedPointer<RHIResourceLayout> _resourceLayout = nullptr;
	
		bool _isFirstFrame = true;
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		VkAccessFlags SelectVkAccessFlag(const VkImageLayout imageLayout);
	};
	inline void rhi::vulkan::RHICommandList::DrawIndexed(gu::uint32 indexCount, gu::uint32 startIndexLocation, gu::uint32 baseVertexLocation)
	{
		vkCmdDrawIndexed(_commandBuffer, indexCount, 1, startIndexLocation, baseVertexLocation, 0);
	}
	inline void rhi::vulkan::RHICommandList::DrawIndexedInstanced(gu::uint32 indexCountPerInstance, gu::uint32 instanceCount, gu::uint32 startIndexLocation, gu::uint32 baseVertexLocation, gu::uint32 startInstanceLocation)
	{
		vkCmdDrawIndexed(_commandBuffer, indexCountPerInstance, instanceCount, startIndexLocation, baseVertexLocation, startIndexLocation);
	}
	inline void rhi::vulkan::RHICommandList::Dispatch(gu::uint32 threadGroupCountX, gu::uint32 threadGroupCountY, gu::uint32 threadGroupCountZ)
	{
		vkCmdDispatch(_commandBuffer, threadGroupCountX, threadGroupCountY, threadGroupCountZ);
	}
}
#endif