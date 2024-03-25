//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIAdapter.hpp
///             @brief  Physical Device (adapter), Describe gpu information 
///             @author Toide Yutaro
///             @date   2022_09_05
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_RHI_ADAPTER_HPP
#define DIRECTX12_RHI_ADAPTER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIAdapter.hpp"
#include "DirectX12Core.hpp"
#include <dxgi1_6.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::directX12
{
	/****************************************************************************
	*				  			RHIDisplayAdapter
	*************************************************************************//**
	*  @class     RHIDisplayAdapter
	*  @brief     Physical Device (adapter), Describe gpu information
	*****************************************************************************/
	class RHIDisplayAdapter : public core::RHIDisplayAdapter, public gu::EnableSharedFromThis<RHIDisplayAdapter>
	{
	public:

		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*---------------------------------------------------------------
				@brief :  return logical device shared pointer.
						  frame count is used for the command allocators
		-----------------------------------------------------------------*/
		gu::SharedPointer<core::RHIDevice> CreateDevice() override;

		/*---------------------------------------------------------------
				@brief : Describe physical device name and spec(future work)
		-----------------------------------------------------------------*/
		void PrintInfo() override;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		AdapterComPtr GetAdapter() { return _adapter; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIDisplayAdapter(const gu::SharedPointer<core::RHIInstance>& instance, const AdapterComPtr& adapter);

		~RHIDisplayAdapter();

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		AdapterComPtr _adapter = nullptr;
	};
}

#endif