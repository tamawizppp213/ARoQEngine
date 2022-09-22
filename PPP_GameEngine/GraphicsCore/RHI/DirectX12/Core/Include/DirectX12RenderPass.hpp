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
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIRenderPass() = default;
		~RHIRenderPass() = default;
		explicit RHIRenderPass(const std::shared_ptr<core::RHIDevice>& device, const std::vector<core::Attachment>& colors, const std::optional<core::Attachment>& depth = std::nullopt) : 
			core::RHIRenderPass(device, colors, depth){}
		explicit RHIRenderPass(const std::shared_ptr<core::RHIDevice>& device, const core::Attachment& color, const std::optional<core::Attachment>& depth = std::nullopt) :
			core::RHIRenderPass(device, color, depth){};
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
	};
}
#endif