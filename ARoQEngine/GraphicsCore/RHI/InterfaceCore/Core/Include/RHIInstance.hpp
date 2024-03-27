//////////////////////////////////////////////////////////////////////////////////
/// @file   RHIInstance.hpp
/// @brief  GraphicsAPI��I����, �ȍ~��GraphicsAPI�ɑΉ�����RenderResource���Ăяo�����߂̃N���X@n
///         ���̂ق�, �����f�o�C�X�̑I����CPU, GPU�f�o�b�K�̗L���Ȃǂ��o�^���邱�Ƃ��o���܂�.@n 
/// @author Toide Yutaro
/// @date   2024_03_27
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_INSTANCE_HPP
#define RHI_INSTANCE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "RHICommonState.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class RHIDisplayAdapter;
	/****************************************************************************
	*				  			RHIInstance
	*************************************************************************//**
	/*  @class     RHIInstance
	*   @brief     GraphicsAPI��I����, �ȍ~��GraphicsAPI�ɑΉ�����RenderResource���Ăяo�����߂̃N���X@n
	*              ���̂ق�, �����f�o�C�X�̑I����CPU, GPU�f�o�b�K�̗L���Ȃǂ��o�^���邱�Ƃ��o���܂�.@n 
	*****************************************************************************/
	class RHIInstance : public gu::NonCopyable
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     �蓮��Graphics API��I�����܂�. @n
		*             ���̂ق�CPU, GPU�f�o�b�K���g�p���邩���I���ł��܂���, �����[�X���[�h�ł͎g�p���邱�Ƃ��o���܂���
		*  @param[in] const core::GraphicsAPI �O���t�B�N�XAPI
		*  @param[in] const RHIInstanceCreateInfo �f�o�b�K�̎w��p�\����
		*  @return gu::SharedPointer<RHIInstance> RHIInstance�̃|�C���^
		*************************************************************************/
		static gu::SharedPointer<RHIInstance> CreateInstance(const core::GraphicsAPI graphicsAPI, const RHIDebugCreateInfo& debugCreateInfo);
		
		/*!**********************************************************************
		*  @brief     �v���b�g�t�H�[���ɍ��킹�Ď�����Graphics API��I�����܂�. @n
		*             ���̂ق�CPU, GPU�f�o�b�K���g�p���邩���I���ł��܂���, �����[�X���[�h�ł͎g�p���邱�Ƃ��o���܂���
		*  @param[in] const RHIDebugCreateInfo& debugCreateInfo
		*  @return gu::SharedPointer<RHIInstance> RHIInstance�̃|�C���^
		*************************************************************************/
		static gu::SharedPointer<RHIInstance> CreateInstance(const RHIDebugCreateInfo& debugCreateInfo);

		/*!**********************************************************************
		*  @brief     �ł����\�����������f�o�C�X�������őI�肵�܂�. ��������xGPU(�O��GPU), dGPU(discrete GPU), iGPU (integrated GPU)�̏��ɗD�悳��܂�
		*  @note      DirectX12�ł͊O��GPU, �f�B�X�N���[�gGPU, ����GPU�̏��ɑI������܂�.�@@n 
		*             Vulkan�ł͂܂��O��GPU���Ή��ł��Ă��炸, �f�B�X�N���[�gGPU, integrated GPU (�ŏ��ɑI�����ꂽGPU)�̏��ɑI������܂�@n
		*  @return gu::SharedPointer<RHIDisplayAdapter> DisplayAdapter�̃|�C���^
		*************************************************************************/
		virtual gu::SharedPointer<RHIDisplayAdapter> SearchHighPerformanceAdapter() = 0;
		
		/*!**********************************************************************
		*  @brief     �ł����\���Ⴂ(����d�͂��Ⴂ)�����f�o�C�X�������őI�肵�܂�. ��������iGPU (integrated GPU), dGPU(discrete GPU), xGPU(�O��GPU)�̏��ɗD�悳��܂�
		*  @note      DirectX12�ł͓���GPU, �f�B�X�N���[�gGPU, �O��GPU�̏��ɑI������܂�.�@@n
		*             Vulkan�ł͂܂��O��GPU���Ή��ł��Ă��炸, integrated GPU, �f�B�X�N���[�gGPU (�ŏ��ɑI�����ꂽGPU)�̏��ɑI������܂�
		*  @return gu::SharedPointer<RHIDisplayAdapter> DisplayAdapter�̃|�C���^
		*************************************************************************/
		virtual gu::SharedPointer<RHIDisplayAdapter> SearchMinimumPowerAdapter() = 0;
		
		/*!**********************************************************************
		*  @brief     �S�Ă̗��p�\�ȕ����f�o�C�X��z��̌`�ŕԂ��܂�.
		*  @return    gu::DynamicArray<gu::SharedPointer<RHIDisplayAdapter>> : �����f�o�C�X�̔z��
		*************************************************************************/
		virtual gu::DynamicArray<gu::SharedPointer<RHIDisplayAdapter>> EnumrateAdapters() = 0;
		
		/*!**********************************************************************
		*  @brief     �o�͗��ɑS�Ă̕����f�o�C�X���L�����܂�
		*************************************************************************/
		virtual void LogAdapters() = 0;

		#pragma endregion Public Function

		#pragma region Public Member Function
		/*!**********************************************************************
		*  @brief    �G���W���̖��O���擾���܂�
		*************************************************************************/
		const char* GetEngineName() const { return EngineName; }

		/*!**********************************************************************
		*  @brief    GPU�̃u���[�N�|�C���g��ݒ�ł��邩��Ԃ��܂�.
		*************************************************************************/
		bool UseGPUDebugBreak() const { return _debugInfo.EnableGPUDebugBreak; }

		#pragma endregion Public Member Function

		#pragma region Public Constructor and Destructor

		#pragma endregion

	protected:
		#pragma region Protected Constructor and Destructor
		
		/*! @brief : �f�t�H���g�R���X�g���N�^*/
		RHIInstance() = default;

		/*! @brief : Debugger���g���R���X�g���N�^*/
		RHIInstance(const core::RHIDebugCreateInfo& debugCreateInfo) : _debugInfo(debugCreateInfo) {};

		virtual ~RHIInstance() = default;

		#pragma endregion

		#pragma region Protected Member Variables

		/* @ Debugger���w�肷��R���X�g���N�^*/
		core::RHIDebugCreateInfo _debugInfo;

		/*! @brief : �G���W����*/
		const char* EngineName = "ARoQ Engine";

		#pragma endregion 
	};
	
}

#endif