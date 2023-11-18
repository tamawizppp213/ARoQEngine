//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12ResourceLayout.hpp
///             @brief  Resource Layout
///             @author Toide Yutaro
///             @date   2022_08_09
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_RESOURCE_LAYOUT_HPP
#define DIRECTX12_RESOURCE_LAYPUT_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIResourceLayout.hpp"
#include "DirectX12Core.hpp"
#include <d3d12.h>
#include <vector>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::directX12
{
	/****************************************************************************
	*				  			RHIResourceLayout
	*************************************************************************//**
	*  @class     RHIResourceLayout
	*  @brief     GPU resource root signature. shader binding setting.
	*****************************************************************************/
	class RHIResourceLayout : public  rhi::core::RHIResourceLayout
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/* @brief : DirectX12 resource layout. */
		RootSignatureComPtr GetRootSignature() const noexcept { return _rootSignature; }

		/* @brief : For directX ray tracing resource layout.*/
		D3D12_GLOBAL_ROOT_SIGNATURE GetGlobalRootSignature() const { return D3D12_GLOBAL_ROOT_SIGNATURE(_rootSignature.Get()); }

		D3D12_LOCAL_ROOT_SIGNATURE GetLocalRootSignature() const { return D3D12_LOCAL_ROOT_SIGNATURE(_rootSignature.Get()); }

		size_t GetElementsCount() const noexcept { return _elementsCount; }

		size_t GetConstant32BitsCount() const noexcept { return _constant32BitsCount; }
		
		void SetName(const std::wstring& name) override;

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIResourceLayout() = default;
		
		~RHIResourceLayout();
		
		explicit RHIResourceLayout(const gu::SharedPointer<core::RHIDevice>& device, const std::vector<core::ResourceLayoutElement>& elements = {}, const std::vector<core::SamplerLayoutElement>& samplers = {}, const std::optional<core::Constant32Bits>& = std::nullopt, const std::wstring& name = L"ResourceLayout");
		
		explicit RHIResourceLayout(const gu::SharedPointer<core::RHIDevice>& device, const core::ResourceLayoutElement& element, const core::SamplerLayoutElement& sampler, const std::optional<core::Constant32Bits>& constant32Bits = std::nullopt, const std::wstring& name = L"ResourceLayout");
	
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		RootSignatureComPtr _rootSignature = nullptr;
		
		size_t _elementsCount = 0;
		
		size_t _constant32BitsCount = 0;

	private:
		void SetUp();
	};
}
#endif