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
	****************************************************************************/
	/* @class     RHIResourceLayout
	*  @brief     GPU resource root signature. shader binding setting.
	*****************************************************************************/
	class RHIResourceLayout : public  rhi::core::RHIResourceLayout
	{
	public:
		#pragma region Public Function
		#pragma endregion

		#pragma region Public Property
		/*----------------------------------------------------------------------
		*  @brief :  DirectX12のリソースレイアウトを返します. 
		*----------------------------------------------------------------------*/
		RootSignatureComPtr GetRootSignature() const noexcept { return _rootSignature; }

		/*----------------------------------------------------------------------
		*  @brief :   For directX ray tracing resource layout
		*----------------------------------------------------------------------*/
		D3D12_GLOBAL_ROOT_SIGNATURE GetGlobalRootSignature() const { return D3D12_GLOBAL_ROOT_SIGNATURE(_rootSignature.Get()); }

		/*----------------------------------------------------------------------
		*  @brief :   レイトレーシング用のローカルルートシグネチャを返します. 
		*----------------------------------------------------------------------*/
		D3D12_LOCAL_ROOT_SIGNATURE GetLocalRootSignature() const { return D3D12_LOCAL_ROOT_SIGNATURE(_rootSignature.Get()); }

		/*----------------------------------------------------------------------
		*  @brief :   Bindに必要な総数を返します. 
		*----------------------------------------------------------------------*/
		gu::uint8 GetElementsCount() const noexcept { return _elementsCount; }

		/*----------------------------------------------------------------------
		*  @brief :   32bit用のConstant buffer中の要素数を返します
		*----------------------------------------------------------------------*/
		gu::uint8 GetConstant32BitsCount() const noexcept { return _constant32BitsCount; }
		
		/*!**********************************************************************
		*  @brief     Unordered Access Viewがあるかを返します
		*  @param[in] void
		*  @return    bool Unordered Access Viewがあるか
		*************************************************************************/
		__forceinline bool HasUAV() const { return _uavCount > 0; }

		/*!**********************************************************************
		*  @brief     Shader Resource Viewがあるかを返します
		*  @param[in] void
		*  @return    bool Shader Resource Viewがあるか
		*************************************************************************/
		__forceinline bool HasSRV() const { return _srvCount > 0; }

		/*!**********************************************************************
		*  @brief     Constant Buffer Viewがあるかを返します
		*  @param[in] void
		*  @return    bool Constant Buffer Viewがあるか
		*************************************************************************/
		__forceinline bool HasCBV() const { return _cbvCount > 0; }

		/*!**********************************************************************
		*  @brief     Descriptor Tableがあるかを返します
		*  @param[in] void
		*  @return    bool Descriptor Tableがあるか
		*************************************************************************/
		__forceinline bool HasTableResource() const { return HasCBV() || HasSRV() || HasUAV(); }

		/*!**********************************************************************
		*  @brief     Static Sampler stateがあるかを返します
		*  @param[in] void
		*  @return    bool Static Sampler stateがあるか
		*************************************************************************/
		__forceinline bool HasStaticSampler() const { return _staticSamplerCount > 0; }

		/*!**********************************************************************
		*  @brief     Dynamic Sampler stateがあるかを返します
		*  @param[in] void
		*  @return    bool Dynamic Sampler stateがあるか
		*************************************************************************/
		__forceinline bool HasDynamicSampler() const { return _dynamicSamplerCount > 0; }

		/*!**********************************************************************
		*  @brief     デバッグ表示名を設定します
		*  @param[in] const gu::tstring& name : デバッグ表示名
		*  @return    void
		*************************************************************************/
		void SetName(const gu::tstring& name) override;

		#pragma endregion

		#pragma region Public Constructor and Destructor
		/*! @brief デフォルトコンストラクタ*/
		RHIResourceLayout() = default;
		
		/*! @brief デストラクタ*/
		~RHIResourceLayout();
		
		/*! @brief 作成する際の基本となるコンストラクタ*/
		explicit RHIResourceLayout(const gu::SharedPointer<core::RHIDevice>& device, const gu::DynamicArray<core::ResourceLayoutElement>& elements = {}, const gu::DynamicArray<core::SamplerLayoutElement>& samplers = {}, const gu::Optional<core::Constant32Bits>& = {}, const gu::tstring& name = SP("ResourceLayout"));
		
		/*! @brief 単一のResourceElementで初期化*/
		explicit RHIResourceLayout(const gu::SharedPointer<core::RHIDevice>& device, const core::ResourceLayoutElement& element, const core::SamplerLayoutElement& sampler, const gu::Optional<core::Constant32Bits>& constant32Bits = {}, const gu::tstring& name = SP("ResourceLayout"));

		/*! @brief Descriptorを使って初期化*/
		explicit RHIResourceLayout(const gu::SharedPointer<core::RHIDevice>& device, const rhi::core::RHIResourceLayoutDesc& desc, const gu::tstring& name = SP("ResourceLayout"));

		#pragma endregion
	protected:
		#pragma region Protected Constructor and Destructor
		#pragma endregion

		#pragma region Protected Function

		#pragma endregion

		#pragma region Protected Property
		/*! @brief DirectX12で使用するRootSignature*/
		RootSignatureComPtr _rootSignature = nullptr;

		/*! @brief RootSignatureのBindingレベル*/
		D3D12_RESOURCE_BINDING_TIER _bindingTier = D3D12_RESOURCE_BINDING_TIER_1;

		/*! @brief Descriptor Tableの要素数*/
		gu::uint8 _elementsCount = 0;

		/*! @brief constant32Bitsの要素数*/
		gu::uint8 _constant32BitsCount = 0;

		/*! @brief UAVが存在するか*/
		gu::uint8 _uavCount = 0;

		/*! @brief SRVが存在するか*/
		gu::uint8 _srvCount = 0;

		/*! @brief CBVが存在するか*/
		gu::uint8 _cbvCount = 0;

		/*! @brief Static Samplerが存在するか*/
		gu::uint8 _staticSamplerCount = 0;

		/*! @brief DynamicSamplerが存在するか*/
		gu::uint8 _dynamicSamplerCount = 0;
		#pragma endregion


	private:
		#pragma region Private Constructor and Destructor
		#pragma endregion

		#pragma region Private Function
		/*!**********************************************************************
		*  @brief     RootSignatureの作成まで一通り行います.
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void SetUp();

		/*----------------------------------------------------------------------
		*  @brief :   RootSignatureの作成をVersionに合わせて行います.
		*----------------------------------------------------------------------*/
		HRESULT SerializeVersionedRootSignature(const D3D12_VERSIONED_ROOT_SIGNATURE_DESC& desc,
			ID3DBlob** rootSignatureBlob, ID3DBlob** errrorBlob);
		#pragma endregion

		#pragma region Private Property
		#pragma endregion
	};
}
#endif