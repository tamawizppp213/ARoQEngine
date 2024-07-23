//////////////////////////////////////////////////////////////////////////////////
/// @file   RHIInstance.cpp
/// @brief  GraphicsAPI��I����, �ȍ~��GraphicsAPI�ɑΉ�����RenderResource���Ăяo�����߂̃N���X@n
///         ���̂ق�, �����f�o�C�X�̑I����CPU, GPU�f�o�b�K�̗L���Ȃǂ��o�^���邱�Ƃ��o���܂�.@n 
/// @author Toide Yutaro
/// @date   2022_09_05
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIMacro.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIInstance.hpp"
// DirectX12
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Instance.hpp"
// Vulkan
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanInstance.hpp"
#include "GameUtility/Base/Include/GUParse.hpp"
#include "GameUtility/Base/Include/GUCommandLine.hpp"
#include "stdio.h"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::core;
using namespace gu;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Public Function
/*!**********************************************************************
*  @brief     �蓮��Graphics API��I�����܂�. @n
*             ���̂ق�CPU, GPU�f�o�b�K���g�p���邩���I���ł��܂���, �����[�X���[�h�ł͎g�p���邱�Ƃ��o���܂���
*  @param[in] const core::GraphicsAPI �O���t�B�N�XAPI
*  @param[in] const RHIInstanceCreateInfo&  CPU, GPU�f�o�b�K�̗L�����w�肷��\����
*  @return gu::SharedPointer<RHIInstance> RHIInstance�̃|�C���^
*************************************************************************/
gu::SharedPointer<RHIInstance> rhi::core::RHIInstance::CreateInstance(const core::GraphicsAPI graphicsAPI, const RHIDebugCreateInfo& debugCreateInfo)
{
	switch (graphicsAPI)
	{
		case GraphicsAPI::DirectX12: 
		{
			printf("DirectX12\n"); 
			return gu::MakeShared<rhi::directX12::RHIInstance>(debugCreateInfo);
		}
		case GraphicsAPI::Vulkan: 
		{
			printf("Vulkan\n"); 
			return gu::MakeShared<rhi::vulkan::RHIInstance>(debugCreateInfo); 
		}
		default:
		{
			Confirmf(false, "Unknown API");
			return nullptr;
		}
	}
}

/*!**********************************************************************
*  @brief     �v���b�g�t�H�[���ɍ��킹�Ď�����Graphics API��I�����܂�. @n
*             ���̂ق�CPU, GPU�f�o�b�K���g�p���邩���I���ł��܂���, �����[�X���[�h�ł͎g�p���邱�Ƃ��o���܂���
*  @param[in] const RHIDebugCreateInfo& CPU, GPU�f�o�b�K�̗L�����w�肷��\����
*  @return gu::SharedPointer<RHIInstance> RHIInstance�̃|�C���^
*************************************************************************/
gu::SharedPointer<RHIInstance> rhi::core::RHIInstance::CreateInstance(const core::RHIDebugCreateInfo& debugCreateInfo)
{
	return gu::MakeShared<DEFAULT_GRAPHICS_API::RHIInstance>(debugCreateInfo);
}

/*!**********************************************************************
*  @brief     �����f�o�C�X�̍D�݂�����ꍇ��, �R�}���h���C���̌��ʂɊ�Â���DisplayAdapterVenderType��Ԃ� @n
*             �R�}���h���C���̕������, prefer_(DisplayAdapterVenderType�Ɏw�肳���x���_�[��)�Ŏw�肵�Ă�������
*  @details   �R�}���h�ꗗ @n
*             prefer_AMD, prefer_Intel, prefer_Nvidia, prefer_Microsoft
*************************************************************************/
DisplayAdapterVendorType rhi::core::RHIInstance::GetPreferredAdapterVendor() const
{
	if(Parse::Contains(CommandLine::Get(), SP("prefer_AMD")))
	{
		return DisplayAdapterVendorType::Amd;
	}

	if (Parse::Contains(CommandLine::Get(), SP("prefer_Intel")))
	{
		return DisplayAdapterVendorType::Intel;
	}

	if (Parse::Contains(CommandLine::Get(), SP("prefer_Nvidia")))
	{
		return DisplayAdapterVendorType::Nvidia;
	}

	if (Parse::Contains(CommandLine::Get(), SP("prefer_Microsoft")))
	{
		return DisplayAdapterVendorType::Microsoft;
	}

	return DisplayAdapterVendorType::Unknown;
}
#pragma endregion Public Function