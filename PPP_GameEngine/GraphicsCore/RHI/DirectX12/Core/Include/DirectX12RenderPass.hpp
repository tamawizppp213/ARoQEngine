//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12RenderPass.hpp
///             @brief  Render pass
///             @author Toide Yutaro
///             @date   2022_08_02
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_RENDER_PASS_HPP
#define DIRECTX12_RENDER_PASS_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIRenderPass.hpp"
#include "DirectX12Core.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::directX12
{

	/****************************************************************************
	*				  			RHIRenderPass
	*************************************************************************//**
	*  @class     RHIRenderPass
	*  @brief     Render pass (not use in directX12)
	*****************************************************************************/
	class RHIRenderPass : public  rhi::core::RHIRenderPass
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/* @brief : set debugging name. */
		void SetName(const gu::tstring& name) { _name = name; };

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIRenderPass() = default;

		~RHIRenderPass() = default;

		explicit RHIRenderPass(const gu::SharedPointer<core::RHIDevice>& device, const gu::DynamicArray<core::Attachment>& colors, const gu::Optional<core::Attachment>& depth = {}) : 
			core::RHIRenderPass(device, colors, depth){}

		explicit RHIRenderPass(const gu::SharedPointer<core::RHIDevice>& device, const core::Attachment& color, const gu::Optional<core::Attachment>& depth = {}) :
			core::RHIRenderPass(device, color, depth){};

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		gu::tstring _name = L"";
	};
}
#endif