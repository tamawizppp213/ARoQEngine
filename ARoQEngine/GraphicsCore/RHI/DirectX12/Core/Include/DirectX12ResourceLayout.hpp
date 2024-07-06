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
		*  @brief :  DirectX12�̃��\�[�X���C�A�E�g��Ԃ��܂�. 
		*----------------------------------------------------------------------*/
		RootSignatureComPtr GetRootSignature() const noexcept { return _rootSignature; }

		/*----------------------------------------------------------------------
		*  @brief :   For directX ray tracing resource layout
		*----------------------------------------------------------------------*/
		D3D12_GLOBAL_ROOT_SIGNATURE GetGlobalRootSignature() const { return D3D12_GLOBAL_ROOT_SIGNATURE(_rootSignature.Get()); }

		/*----------------------------------------------------------------------
		*  @brief :   ���C�g���[�V���O�p�̃��[�J�����[�g�V�O�l�`����Ԃ��܂�. 
		*----------------------------------------------------------------------*/
		D3D12_LOCAL_ROOT_SIGNATURE GetLocalRootSignature() const { return D3D12_LOCAL_ROOT_SIGNATURE(_rootSignature.Get()); }

		/*----------------------------------------------------------------------
		*  @brief :   Bind�ɕK�v�ȑ�����Ԃ��܂�. 
		*----------------------------------------------------------------------*/
		gu::uint8 GetElementsCount() const noexcept { return _elementsCount; }

		/*----------------------------------------------------------------------
		*  @brief :   32bit�p��Constant buffer���̗v�f����Ԃ��܂�
		*----------------------------------------------------------------------*/
		gu::uint8 GetConstant32BitsCount() const noexcept { return _constant32BitsCount; }
		
		/*!**********************************************************************
		*  @brief     Unordered Access View�����邩��Ԃ��܂�
		*  @param[in] void
		*  @return    bool Unordered Access View�����邩
		*************************************************************************/
		__forceinline bool HasUAV() const { return _uavCount > 0; }

		/*!**********************************************************************
		*  @brief     Shader Resource View�����邩��Ԃ��܂�
		*  @param[in] void
		*  @return    bool Shader Resource View�����邩
		*************************************************************************/
		__forceinline bool HasSRV() const { return _srvCount > 0; }

		/*!**********************************************************************
		*  @brief     Constant Buffer View�����邩��Ԃ��܂�
		*  @param[in] void
		*  @return    bool Constant Buffer View�����邩
		*************************************************************************/
		__forceinline bool HasCBV() const { return _cbvCount > 0; }

		/*!**********************************************************************
		*  @brief     Descriptor Table�����邩��Ԃ��܂�
		*  @param[in] void
		*  @return    bool Descriptor Table�����邩
		*************************************************************************/
		__forceinline bool HasTableResource() const { return HasCBV() || HasSRV() || HasUAV(); }

		/*!**********************************************************************
		*  @brief     Static Sampler state�����邩��Ԃ��܂�
		*  @param[in] void
		*  @return    bool Static Sampler state�����邩
		*************************************************************************/
		__forceinline bool HasStaticSampler() const { return _staticSamplerCount > 0; }

		/*!**********************************************************************
		*  @brief     Dynamic Sampler state�����邩��Ԃ��܂�
		*  @param[in] void
		*  @return    bool Dynamic Sampler state�����邩
		*************************************************************************/
		__forceinline bool HasDynamicSampler() const { return _dynamicSamplerCount > 0; }

		/*!**********************************************************************
		*  @brief     �f�o�b�O�\������ݒ肵�܂�
		*  @param[in] const gu::tstring& name : �f�o�b�O�\����
		*  @return    void
		*************************************************************************/
		void SetName(const gu::tstring& name) override;

		#pragma endregion

		#pragma region Public Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		RHIResourceLayout() = default;
		
		/*! @brief �f�X�g���N�^*/
		~RHIResourceLayout();
		
		/*! @brief �쐬����ۂ̊�{�ƂȂ�R���X�g���N�^*/
		explicit RHIResourceLayout(const gu::SharedPointer<core::RHIDevice>& device, const gu::DynamicArray<core::ResourceLayoutElement>& elements = {}, const gu::DynamicArray<core::SamplerLayoutElement>& samplers = {}, const gu::Optional<core::Constant32Bits>& = {}, const gu::tstring& name = SP("ResourceLayout"));
		
		/*! @brief �P���ResourceElement�ŏ�����*/
		explicit RHIResourceLayout(const gu::SharedPointer<core::RHIDevice>& device, const core::ResourceLayoutElement& element, const core::SamplerLayoutElement& sampler, const gu::Optional<core::Constant32Bits>& constant32Bits = {}, const gu::tstring& name = SP("ResourceLayout"));

		/*! @brief Descriptor���g���ď�����*/
		explicit RHIResourceLayout(const gu::SharedPointer<core::RHIDevice>& device, const rhi::core::RHIResourceLayoutDesc& desc, const gu::tstring& name = SP("ResourceLayout"));

		#pragma endregion
	protected:
		#pragma region Protected Constructor and Destructor
		#pragma endregion

		#pragma region Protected Function

		#pragma endregion

		#pragma region Protected Property
		/*! @brief DirectX12�Ŏg�p����RootSignature*/
		RootSignatureComPtr _rootSignature = nullptr;

		/*! @brief RootSignature��Binding���x��*/
		D3D12_RESOURCE_BINDING_TIER _bindingTier = D3D12_RESOURCE_BINDING_TIER_1;

		/*! @brief Descriptor Table�̗v�f��*/
		gu::uint8 _elementsCount = 0;

		/*! @brief constant32Bits�̗v�f��*/
		gu::uint8 _constant32BitsCount = 0;

		/*! @brief UAV�����݂��邩*/
		gu::uint8 _uavCount = 0;

		/*! @brief SRV�����݂��邩*/
		gu::uint8 _srvCount = 0;

		/*! @brief CBV�����݂��邩*/
		gu::uint8 _cbvCount = 0;

		/*! @brief Static Sampler�����݂��邩*/
		gu::uint8 _staticSamplerCount = 0;

		/*! @brief DynamicSampler�����݂��邩*/
		gu::uint8 _dynamicSamplerCount = 0;
		#pragma endregion


	private:
		#pragma region Private Constructor and Destructor
		#pragma endregion

		#pragma region Private Function
		/*!**********************************************************************
		*  @brief     RootSignature�̍쐬�܂ň�ʂ�s���܂�.
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void SetUp();

		/*----------------------------------------------------------------------
		*  @brief :   RootSignature�̍쐬��Version�ɍ��킹�čs���܂�.
		*----------------------------------------------------------------------*/
		HRESULT SerializeVersionedRootSignature(const D3D12_VERSIONED_ROOT_SIGNATURE_DESC& desc,
			ID3DBlob** rootSignatureBlob, ID3DBlob** errrorBlob);
		#pragma endregion

		#pragma region Private Property
		#pragma endregion
	};
}
#endif