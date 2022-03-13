//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12GrahicsDevice.hpp
///             @brief  Grahics Device for DirectX12
///             @author Toide Yutaro
///             @date   2022_03_11
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_GRAPHICS_DEVICE_HPP
#define DIRECTX12_GRAPHICS_DEVICE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/Interface/Include/IGraphicsDevice.hpp"
#include "DirectX12Core.hpp"
#include <dxgiformat.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////

/****************************************************************************
*				  			TemplateClass
*************************************************************************//**
*  @class     TemplateClass
*  @brief     temp
*****************************************************************************/
class GraphicsDeviceDirectX12 : public IGraphicsDevice
{
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	void Initialize        (HWND hwnd) override;
	void OnResize          () override;
	void Finalize          () override;
	void ClearScreen       () override;
	void CompleteInitialize() override;
	void CompleteRendering () override;
	void FlushCommandQueue () override;

	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/

	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
private:
	/****************************************************************************
	**                Private Function
	*****************************************************************************/
	void LoadPipeline();
	void LoadAssets();
	/*-------------------------------------------------------------------
	-                        Initialize
	---------------------------------------------------------------------*/
	void CreateDevice();
	void CreateCommandObject();
	void CreateSwapChain();
	void CreateDescriptorHeap();
	void CreatePSOs();

	/*-------------------------------------------------------------------
	-                        Debug
	---------------------------------------------------------------------*/
	void EnabledDebugLayer();
	void EnabledGPUBasedValidation();
	void LogAdapters     ();
	void LogAdapterOutputs(Adapter* adapter);
	void LogOutputDisplayModes(Output* output, DXGI_FORMAT format);
	void ReportLiveObjects();
	/****************************************************************************
	**                Private Member Variables
	*****************************************************************************/
	/*-------------------------------------------------------------------
	-                        Debug
	---------------------------------------------------------------------*/
	DeviceComPtr    _device;
	FactoryComPtr   _dxgiFactory;
	AdapterComPtr   _useAdapter;
	SwapchainComPtr _swapchain;

	DXGI_FORMAT _backBufferFormat = DXGI_FORMAT_R16G16B16A16_FLOAT;

	bool _isWarpAdapter;
};

#endif