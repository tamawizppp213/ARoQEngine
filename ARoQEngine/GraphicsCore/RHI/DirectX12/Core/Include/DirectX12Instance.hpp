//////////////////////////////////////////////////////////////////////////////////
///  @file   DirectX12RHIInstance.hpp
///  @brief  GraphicsAPI��I����, �ȍ~��GraphicsAPI�ɑΉ�����RenderResource���Ăяo�����߂̃N���X@n
///          ���̂ق�, �����f�o�C�X�̑I����CPU, GPU�f�o�b�K�̗L���Ȃǂ��o�^���邱�Ƃ��o���܂�.@n 
/// �@�@�@�@�@�@DirectX12AgilitySDK�̃o�[�W�������ŐV�łɂ��邽�߂ɂ�, cpp���D3D12SDKVersion��ݒ肵�Ă����K�v������܂�
///  @author Toide Yutaro
///  @date   2022_09_05
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_RHI_INSTANCE_HPP
#define DIRECTX12_RHI_INSTANCE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIInstance.hpp"
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
	*				  			RHIInstance
	*************************************************************************//**
	/* @class  RHIInstance
	*  @brief  GraphicsAPI��I����, �ȍ~��GraphicsAPI�ɑΉ�����RenderResource���Ăяo�����߂̃N���X@n
    *          ���̂ق�, �����f�o�C�X�̑I����CPU, GPU�f�o�b�K�̗L���Ȃǂ��o�^���邱�Ƃ��o���܂�.@n 
    * �@�@�@�@�@�@DirectX12AgilitySDK�̃o�[�W�������ŐV�łɂ��邽�߂ɂ�, cpp���D3D12SDKVersion��ݒ肵�Ă����K�v������܂�
	*****************************************************************************/
	class RHIInstance : public core::RHIInstance, public gu::EnableSharedFromThis<RHIInstance>
	{
	public:
		#pragma region Public Function

		/*!**********************************************************************
		*  @brief     �ł����\�����������f�o�C�X�������őI�肵�܂�. ��������xGPU(�O��GPU), dGPU(discrete GPU), iGPU (integrated GPU)�̏��ɗD�悳��܂�
		*  @note      DirectX12�ł͊O��GPU, �f�B�X�N���[�gGPU, ����GPU�̏��ɑI������܂�.�@@n
		*  @return gu::SharedPointer<RHIDisplayAdapter> DisplayAdapter�̃|�C���^
		*************************************************************************/
		virtual gu::SharedPointer<core::RHIDisplayAdapter> SearchHighPerformanceAdapter() override;
		
		/*!**********************************************************************
		*  @brief     �ł����\���Ⴂ(����d�͂��Ⴂ)�����f�o�C�X�������őI�肵�܂�. ��������iGPU (integrated GPU), dGPU(discrete GPU), xGPU(�O��GPU)�̏��ɗD�悳��܂�
		*  @note      DirectX12�ł͓���GPU, �f�B�X�N���[�gGPU, �O��GPU�̏��ɑI������܂�.�@@n
		*  @return gu::SharedPointer<RHIDisplayAdapter> DisplayAdapter�̃|�C���^
		*************************************************************************/
		virtual gu::SharedPointer<core::RHIDisplayAdapter> SearchMinimumPowerAdapter ()override;
		
		/*!**********************************************************************
		*  @brief     �S�Ă̗��p�\�ȕ����f�o�C�X��z��̌`�ŕԂ��܂�.
		* @attention ���̊֐���, �z��̏��Ԃ����Ɏw�肪����܂���. 
		*  @return    gu::DynamicArray<gu::SharedPointer<RHIDisplayAdapter>> : �����f�o�C�X�̔z��
		*************************************************************************/
		virtual gu::DynamicArray<gu::SharedPointer<core::RHIDisplayAdapter>> EnumrateAdapters() override;
		
		/*!**********************************************************************
		*  @brief     �o�͗��ɑS�Ă̕����f�o�C�X���L�����܂�
		*************************************************************************/
		void LogAdapters() override;

		#pragma endregion Public Function

		#pragma region Public Member Function
		/*!**********************************************************************
		*  @brief     Factory�|�C���^��Ԃ��܂�
		*************************************************************************/
		FactoryComPtr GetFactory() const noexcept { return _factory; }

		/*!**********************************************************************
		*  @brief     DirectXAgilitySDK�����[�h�ς݂���Ԃ��܂�
		*************************************************************************/
		#if D3D12_CORE_ENABLED
		bool HasLoadedDirectXAgilitySDK() const;
		#endif

		/*IDREDSettings*  GetDREDSettings () const noexcept { return _useDRED ? _dredSettings.Get() : nullptr; }
		IDREDSettings1* GetDREDSettings1() const noexcept { return _useDREDContext ? _dredSettings1.Get() : nullptr; }*/
		
		#pragma endregion

		#pragma region Public Constructor and Destructor

		/*! @brief : �f�t�H���g�R���X�g���N�^*/
		RHIInstance() = default;

		/*! @brief : Debugger���g���R���X�g���N�^*/
		RHIInstance(const core::RHIDebugCreateInfo& debugCreateInfo); 
		
		~RHIInstance();

		#pragma endregion

	protected:
		#pragma region Protected Function
		/*! @brief : CPU debugger*/
		void EnabledDebugLayer();            // debug mode only use

		/* @brief : GPU debugger*/
		void EnabledShaderBasedValidation(); // It has a significant impact on the frame rate.
		
		/* @brief : Acquire the debug information when the Device is removed.*/
		void EnabledGPUClashDebuggingModes(); 

		/*!**********************************************************************
		*  @brief     �����\�܂��͍ŏ��d�͂�����Adapter��I���i�i���jxGPU�AdGPU iGPU�i��j
		*  @return    gu::SharedPointer<RHIDisplayAdapter> : �����f�o�C�X�̃|�C���^
		*************************************************************************/
		gu::SharedPointer<core::RHIDisplayAdapter> SearchAdapter(const DXGI_GPU_PREFERENCE preference) const;

		#pragma endregion

		#pragma region Protected Member Variables
		/*! @brief �R�}���h���C���Ŏw�肳��Ă���ꍇ, WARP�f�o�C�X�ō��悤�ɂ���
		*   @note  �R�}���h���C���ł�"warp"�R�}���h��ł��Ă�������*/
		static bool SHOULD_CREATE_WITH_WARP;

		/*! @brief �R�}���h���C���Ŏw�肳��Ă���ꍇ, �\�t�g�E�F�A�����_�����O��������
		*   @note  �R�}���h���C���ł�, "allowSoftwareRendering��ł��Ă�������"*/
		static bool ALLOW_SOFTWARE_RENDERING;

		FactoryComPtr _factory = nullptr;

		bool _useDRED        = false;
		bool _useDREDContext = false;
		bool _useLightWeightDRED = false;

		#pragma endregion

	};
}

#endif