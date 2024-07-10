//////////////////////////////////////////////////////////////////////////////////
///  @file   DirectX12GPUSampler.hpp
///  @brief  �e�N�X�`���T���v�����O�̕��@�̐ݒ荀�ڂ��L�q����N���X�ł�.
///  @author Toide Yutaro
///  @date   2024_07_09
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_GPU_SAMPLER_HPP
#define DIRECTX12_GPU_SAMPLER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUSampler.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Core.hpp"
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
	*				  			GPUSampler
	****************************************************************************/
	/* @brief  �e�N�X�`���T���v�����O�̕��@�̐ݒ荀�ڂ��L�q����N���X�ł�.
	*****************************************************************************/
	class GPUSampler : public core::GPUSampler
	{
	public:
		#pragma region Public Function
		#pragma endregion

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     DirectX12�̃T���v���[����Ԃ��܂�.
		*  @param[in] void
		*  @return    D3D12_STATIC_SAMPLER_DESC �T���v���[���
		*************************************************************************/
		__forceinline D3D12_STATIC_SAMPLER_DESC& GetSamplerDesc() { return _samplerDesc; }
		/*!**********************************************************************
	    *  @brief     DirectX12�̃T���v���[����Ԃ��܂�.
	    *  @param[in] void
	    *  @return    const D3D12_STATIC_SAMPLER_DESC �T���v���[���
	    *************************************************************************/
		__forceinline const D3D12_STATIC_SAMPLER_DESC& GetSamplerDesc() const noexcept { return _samplerDesc; }

		#pragma endregion

		#pragma region Public Constructor and Destructor
		
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		GPUSampler() = default;

		/*! @brief �f�X�g���N�^*/
		~GPUSampler() = default;

		/*! @brief �_���f�o�C�X�ƃT���v���[���ŏ�����*/
		explicit GPUSampler(const gu::SharedPointer<core::RHIDevice>& device, const core::SamplerInfo& samplerInfo);

		#pragma endregion
	
	protected:
		#pragma region Protected Constructor and Destructor
		#pragma endregion

		#pragma region Protected Function
		#pragma endregion

		#pragma region Protected Property

		/*! @brief �T���v���[���*/
		D3D12_STATIC_SAMPLER_DESC _samplerDesc = {};
		#pragma endregion	
	};
}
#endif