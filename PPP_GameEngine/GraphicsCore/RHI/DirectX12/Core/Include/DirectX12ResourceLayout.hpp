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
		/*----------------------------------------------------------------------
		*  @brief :  DirectX12�̃��\�[�X���C�A�E�g��Ԃ��܂�. 
		/*----------------------------------------------------------------------*/
		RootSignatureComPtr GetRootSignature() const noexcept { return _rootSignature; }

		/*----------------------------------------------------------------------
		*  @brief :   For directX ray tracing resource layout
		/*----------------------------------------------------------------------*/
		D3D12_GLOBAL_ROOT_SIGNATURE GetGlobalRootSignature() const { return D3D12_GLOBAL_ROOT_SIGNATURE(_rootSignature.Get()); }

		/*----------------------------------------------------------------------
		*  @brief :   ���C�g���[�V���O�p�̃��[�J�����[�g�V�O�l�`����Ԃ��܂�. 
		/*----------------------------------------------------------------------*/
		D3D12_LOCAL_ROOT_SIGNATURE GetLocalRootSignature() const { return D3D12_LOCAL_ROOT_SIGNATURE(_rootSignature.Get()); }

		/*----------------------------------------------------------------------
		*  @brief :   Bind�ɕK�v�ȑ�����Ԃ��܂�. 
		/*----------------------------------------------------------------------*/
		size_t GetElementsCount() const noexcept { return _elementsCount; }

		/*----------------------------------------------------------------------
		*  @brief :   32bit�p��Constant buffer���̗v�f����Ԃ��܂�
		/*----------------------------------------------------------------------*/
		size_t GetConstant32BitsCount() const noexcept { return _constant32BitsCount; }
		
		/*----------------------------------------------------------------------
		*  @brief :  Unordered Access View�����邱�Ƃ�Ԃ��܂�
		/*----------------------------------------------------------------------*/
		bool HasUAV() const { return _hasUAV; }

		/*----------------------------------------------------------------------
		*  @brief :  Shader Resource View�����邱�Ƃ�Ԃ��܂�
		/*----------------------------------------------------------------------*/
		bool HasSRV() const { return _hasSRV; }

		/*----------------------------------------------------------------------
		*  @brief :  Constant Buffer View�����邱�Ƃ�Ԃ��܂�
		/*----------------------------------------------------------------------*/
		bool HasCBV() const { return _hasCBV; }

		/*----------------------------------------------------------------------
		*  @brief :  Descriptor Table�����邱�Ƃ�Ԃ��܂�
		/*----------------------------------------------------------------------*/
		bool HasTableResource() const { return _hasCBV || _hasSRV || _hasUAV; }

		/*----------------------------------------------------------------------
		*  @brief :  Static Sampler state �����邱�Ƃ�Ԃ��܂�
		/*----------------------------------------------------------------------*/
		bool HasStaticSampler() const { return _hasStaticSampler; }

		/*----------------------------------------------------------------------
		*  @brief :  Dynamic Sampler state�����邱�Ƃ�Ԃ��܂�
		/*----------------------------------------------------------------------*/
		bool HasDynamicSampler() const { return _hasDynamicSampler; }

		void SetName(const std::wstring& name) override;

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIResourceLayout() = default;
		
		~RHIResourceLayout();
		
		explicit RHIResourceLayout(const gu::SharedPointer<core::RHIDevice>& device, const std::vector<core::ResourceLayoutElement>& elements = {}, const std::vector<core::SamplerLayoutElement>& samplers = {}, const std::optional<core::Constant32Bits>& = std::nullopt, const std::wstring& name = L"ResourceLayout");
		
		explicit RHIResourceLayout(const gu::SharedPointer<core::RHIDevice>& device, const core::ResourceLayoutElement& element, const core::SamplerLayoutElement& sampler, const std::optional<core::Constant32Bits>& constant32Bits = std::nullopt, const std::wstring& name = L"ResourceLayout");

		explicit RHIResourceLayout(const gu::SharedPointer<core::RHIDevice>& device, const rhi::core::RHIResourceLayoutDesc& desc, const std::wstring& name = L"ResourceLayout");

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

		D3D12_RESOURCE_BINDING_TIER _bindingTier = D3D12_RESOURCE_BINDING_TIER_1;

		/*----------------------------------------------------------------------
		*        Resource view�����݂��邩
		/*----------------------------------------------------------------------*/
		bool _hasUAV = false;
		bool _hasSRV = false;
		bool _hasCBV = false;
		bool _hasStaticSampler = false;
		bool _hasDynamicSampler = false;

	private:
		/*----------------------------------------------------------------------
		*  @brief :   RootSignature�̍쐬�܂ň�ʂ�s���܂�.
		/*----------------------------------------------------------------------*/
		void SetUp();

		/*----------------------------------------------------------------------
		*  @brief :   RootSignature�̍쐬��Version�ɍ��킹�čs���܂�.
		/*----------------------------------------------------------------------*/
		HRESULT SerializeVersionedRootSignature(const D3D12_VERSIONED_ROOT_SIGNATURE_DESC& desc, 
			ID3DBlob** rootSignatureBlob, ID3DBlob** errrorBlob);
	};
}
#endif