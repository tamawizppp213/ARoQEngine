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
	/*!**********************************************************************
	*  @brief  GPU���N���b�V�������ۂ̃f�o�b�O�̐ݒ�
	*************************************************************************/
	enum class GPUCrashDebuggingMode
	{
		None = 0x0,
		Dred = 0x1, //!<�f�o�C�X���폜���ꂽ�ۂ̃f�o�b�O���
		All = Dred
	};

	/****************************************************************************
	*				  			RHIInstance
	****************************************************************************/
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
		*  @note      DirectX12�ł͊O��GPU, �f�B�X�N���[�gGPU, ����GPU�̏��ɑI������܂�.
		*  @param[in] void
		*  @return gu::SharedPointer<RHIDisplayAdapter> DisplayAdapter�̃|�C���^
		*************************************************************************/
		virtual gu::SharedPointer<core::RHIDisplayAdapter> SearchHighPerformanceAdapter() override;
		
		/*!**********************************************************************
		*  @brief     �ł����\���Ⴂ(����d�͂��Ⴂ)�����f�o�C�X�������őI�肵�܂�. ��������iGPU (integrated GPU), dGPU(discrete GPU), xGPU(�O��GPU)�̏��ɗD�悳��܂�
		*  @note      DirectX12�ł͓���GPU, �f�B�X�N���[�gGPU, �O��GPU�̏��ɑI������܂�.�@@n
		*  @param[in] void
		*  @return gu::SharedPointer<RHIDisplayAdapter> DisplayAdapter�̃|�C���^
		*************************************************************************/
		virtual gu::SharedPointer<core::RHIDisplayAdapter> SearchMinimumPowerAdapter ()override;
		
		/*!**********************************************************************
		*  @brief     �S�Ă̗��p�\�ȕ����f�o�C�X��z��̌`�ŕԂ��܂�.
		*  @attention ���̊֐���, �z��̏��Ԃ����Ɏw�肪����܂���. 
		*  @param[in] void
		*  @return    gu::DynamicArray<gu::SharedPointer<RHIDisplayAdapter>> : �����f�o�C�X�̔z��
		*************************************************************************/
		virtual gu::DynamicArray<gu::SharedPointer<core::RHIDisplayAdapter>> EnumrateAdapters() const override;
		
		/*!**********************************************************************
		*  @brief     �o�͗��ɑS�Ă̕����f�o�C�X���L�����܂�
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void LogAdapters() const override;

		#pragma endregion Public Function

		#pragma region Public Member Function
		/*!**********************************************************************
		*  @brief     Factory�|�C���^��Ԃ��܂�
		*************************************************************************/
		FactoryComPtr GetFactory() const noexcept { return _factory; }

		/*!**********************************************************************
		*  @brief     GPU���N���b�V�������ۂ̃f�o�b�O�̐ݒ�
		*************************************************************************/
		GPUCrashDebuggingMode GetGPUCrashDebuggingMode() const { return _gpuCrashDebuggingMode; }

		#if D3D12_CORE_ENABLED
		/*!**********************************************************************
		*  @brief     DirectXAgilitySDK�����[�h�ς݂���Ԃ��܂�
		*************************************************************************/
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
		
		/*! @brief �f�X�g���N�^*/
		~RHIInstance();

		#pragma endregion

	protected:
		#pragma region Protected Function
		/*!**********************************************************************
		*  @brief    DirectX12�ɂ�����CPU�̃f�o�b�O�@�\��On�ɂ���
		*  @note     �f�o�b�O���[�h�ɂ����g�p�ł��܂���
		*************************************************************************/
		void EnabledDebugLayer(); // debug mode only use

		/*!**********************************************************************
		*  @brief     GPU�x�[�X�̃o���f�[�V����
		*  @note      FPS�ɋ����e����^���邱�Ƃ���, Release���ɂ͎g�p���Ȃ��悤�ɂ��Ă�������
		*  @details   �ȉ��̂��Ƃ��s���܂�. @n
		*             1. ����������Ă��Ȃ��A�܂��͌݊����̂Ȃ��f�B�X�N���v�^���V�F�[�_�Ŏg�p���� @n
		*             2. �폜���ꂽResource���Q�Ƃ���f�B�X�N���v�^���V�F�[�_�Ŏg�p���� @n
		*             3. ���i�������\�[�X�̏�Ԃƃ��\�[�X�̏�Ԃ̌����̌��� @n
		*             4. �V�F�[�_�[�ɂ�����f�B�X�N���v�^�E�q�[�v�̏I�[�𒴂����C���f�b�N�X�t�� @n
		*             5. �݊����̂Ȃ���ԂŃV�F�[�_�[�����\�[�X�ɃA�N�Z�X���� @n
		*             6. ����������Ă��Ȃ��A�܂��͌݊����̂Ȃ��T���v���[���V�F�[�_�[�Ŏg�p���� @n
		*************************************************************************/
		void EnabledShaderBasedValidation(); // It has a significant impact on the frame rate.
		
		/*!**********************************************************************
		*  @brief     Device���폜���ꂽ�Ƃ��̏����L�q����(DRED)�Ȃǂ̗L����
		*************************************************************************/
		void EnabledGPUClashDebuggingModes(); 

		/*!**********************************************************************
		*  @brief     �����\�܂��͍ŏ��d�͂�����Adapter��I���i�i���jxGPU�AdGPU iGPU�i��j
		*  @param[in] const DXGI_GPU_PREFERENCE �I������GPU�̎��
		*  @return    gu::SharedPointer<RHIDisplayAdapter> : �����f�o�C�X�̃|�C���^
		*************************************************************************/
		gu::SharedPointer<core::RHIDisplayAdapter> SearchAdapter(const DXGI_GPU_PREFERENCE preference) const;

		#pragma endregion

		#pragma region Protected Property
		/*! @brief �R�}���h���C���Ŏw�肳��Ă���ꍇ, WARP�f�o�C�X�ō��悤�ɂ���
		*   @note  �R�}���h���C���ł�"warp"�R�}���h��ł��Ă�������*/
		static bool SHOULD_CREATE_WITH_WARP;

		/*! @brief �R�}���h���C���Ŏw�肳��Ă���ꍇ, �\�t�g�E�F�A�����_�����O��������
		*   @note  �R�}���h���C���ł�, "allowSoftwareRendering��ł��Ă�������"*/
		static bool ALLOW_SOFTWARE_RENDERING;

		/*! @brief Factory�̃|�C���^*/
		FactoryComPtr _factory = nullptr;

		/* @brief GPU���N���b�V�������ہA�\�����Ȃ��f�o�C�X�폜�G���[�����o���ꂽ��Ɍ�������͂ł���悤�ȋ@�\��ǉ����邩*/
		bool _useDRED        = false;

		/* @brief BreadcrumbContext���g�p���邩*/
		bool _useDREDContext = false;

		/* @brief �y�ʔł�DRED���g�p���邩*/
		bool _useLightWeightDRED = false;

		GPUCrashDebuggingMode _gpuCrashDebuggingMode = GPUCrashDebuggingMode::None;
		#pragma endregion

	};
}

#endif