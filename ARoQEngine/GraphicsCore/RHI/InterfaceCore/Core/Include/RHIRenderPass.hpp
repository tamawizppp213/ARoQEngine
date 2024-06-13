//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIRenderPass.hpp
///             @brief  Render Pass (for use vulkan)
///             @author Toide Yutaro
///             @date   2022_06_23
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_RENDER_PASS_HPP
#define RHI_RENDER_PASS_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
#include "GameUtility/Base/Include/GUOptional.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class RHIDevice;
	class RHIFrameBuffer;
	/****************************************************************************
	*				  			RHIRenderPass
	****************************************************************************/
	/* @class     RHIRenderPass
	*  @brief     Render pass:  Use before rendering
	*****************************************************************************/
	class RHIRenderPass : public gu::NonCopyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		void SetClearValue(const gu::Optional<ClearValue>& color, const gu::Optional<ClearValue>& depth = {}, const std::uint32_t index = 0)
		{
			if (color.HasValue())
			{
				_colorClearValues[index] = color.Value();
			}
			_depthClearValue     = depth; // optional
		}

		void SetClearValue(const gu::DynamicArray<ClearValue>& colors, const gu::Optional<ClearValue>& depth = {})
		{
			_colorClearValues = colors;
			_depthClearValue  = depth;
		}

		bool Compatible(const gu::SharedPointer<RHIFrameBuffer>& frameBuffer) const;
		
		/* @brief : Return multi sample count*/
		MultiSample GetMaxSample() const noexcept { return _maxSample; }
		
		/* @brief : Return clear color value*/
		const gu::DynamicArray<ClearValue>& GetClearColor() const noexcept { return _colorClearValues; }
		
		/* @brief : Return clear depth color value*/
		gu::Optional<ClearValue> GetDepthClear() const noexcept { return _depthClearValue; }
		
		/* @brief : Return Get color attachment*/
		gu::Optional<Attachment> GetColorAttachment(const size_t index = 0) const { return _colorAttachments[index]; }
		
		/* @brief : Return Get Depth attachment*/
		gu::Optional<Attachment> GetDepthAttachment() const noexcept { return _depthAttachment; }
		
		/* @brief : Return color attachment size*/
		size_t GetColorAttachmentSize() const noexcept { return _colorAttachments.Size(); }

		virtual void SetName(const gu::tstring& name) = 0;
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		RHIRenderPass() = default;

		virtual ~RHIRenderPass()
		{
			_depthClearValue.Reset();
			_depthAttachment.Reset();
			_colorClearValues.Clear(); _colorClearValues.ShrinkToFit();
			_colorAttachments.Clear(); _colorAttachments.ShrinkToFit();

			if (_device) { _device.Reset(); }
		}

		explicit RHIRenderPass(const gu::SharedPointer<RHIDevice>& device, const gu::DynamicArray<Attachment>& colors, const gu::Optional<Attachment>& depth = {});
		
		explicit RHIRenderPass(const gu::SharedPointer<RHIDevice>& device, const Attachment& color, const gu::Optional<Attachment>& depth = {});
		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		gu::SharedPointer<RHIDevice> _device = nullptr;

		/* Render Target Render Pass*/
		gu::DynamicArray<Attachment>    _colorAttachments = {};
		gu::DynamicArray<ClearValue>    _colorClearValues = {};

		/* Depth Stencil Render pass*/
		gu::Optional<Attachment> _depthAttachment;
		gu::Optional<ClearValue> _depthClearValue;

		MultiSample _maxSample = MultiSample::Count1;
	};
}
#endif