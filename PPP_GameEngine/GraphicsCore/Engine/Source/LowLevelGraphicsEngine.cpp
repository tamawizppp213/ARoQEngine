//////////////////////////////////////////////////////////////////////////////////
//              @file   GraphicsCoreEngine.cpp
///             @brief  Graphics Low Level Engine
///             @author Toide Yutaro
///             @date   -
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
// DirectX12
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Device.hpp"
#include <d3d12.h>
#include <dxgi1_6.h>
// Vulkan
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanDevice.hpp"
#include <Windows.h>
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::core;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
void LowLevelGraphicsEngine::StartUp(APIVersion apiVersion, HWND hwnd, HINSTANCE hInstance)
{
	_hwnd = hwnd; _hInstance = hInstance;
	SelectDevice(apiVersion);
}
void LowLevelGraphicsEngine::OnResize()
{

}
void LowLevelGraphicsEngine::ShutDown()
{

}

#pragma region Private Function
void LowLevelGraphicsEngine::SelectDevice(APIVersion version)
{
	switch (version)
	{
		case APIVersion::DirectX12: { _device = std::make_shared<rhi::directX12::RHIDevice>(); break; }
		case APIVersion::Vulkan:    { _device = std::make_shared<rhi::vulkan   ::RHIDevice>(); break; }
		default: { throw std::runtime_error("Unknown API."); break; }
	}
	
}
#pragma endregion Private Function
