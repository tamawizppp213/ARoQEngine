//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12GraphicsDevice.cpp
///             @brief  Graphics Device
///             @author Toide Yutaro
///             @date   -
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/DirectX12/Core/Include/DirectX12GraphicsDevice.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Public Function
/****************************************************************************
*							Initialize
*************************************************************************//**
*  @fn        void DirectX12::Initialize(void)
*  @brief     Initialize Back Screen
*  @param[in] HWND hwnd
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::Initialize(HWND hwnd)
{
	if (_hasInitialized) { return; }
	SetHWND(hwnd);
	LoadPipeline();
	LoadAssets();
	_hasInitialized = true;
}
void GraphicsDeviceDirectX12::OnResize()
{

}
#pragma endregion Public Function
#pragma region Private Function
/****************************************************************************
*                     LoadPipeLine
*************************************************************************//**
*  @fn        void DirectX12::LoadPipeLine(void)
*  @brief     Load DirectX Basic PipeLine (for initialize)
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::LoadPipeline()
{
#if _DEBUG

#endif
}
void GraphicsDeviceDirectX12::LoadAssets()
{

}

#pragma region Debug Function
/****************************************************************************
*                     EnabledDebugLayer
*************************************************************************//**
*  @fn        void DirectX12::EnabledDebugLayer(void)
*  @brief     Enabled CPU debug layer
*  @param[in] void
*  @return 　　void @n
*  @details   it must be called before the D3D12 device is created.
*****************************************************************************/
void GraphicsDeviceDirectX12::EnabledDebugLayer()
{
	
}

/****************************************************************************
*                   EnabledGPUBasedValiation
*************************************************************************//**
*  @fn        void DirectX12::EnabledGPUBasedValidation(void)
*  @brief     Enabled GPU debugger
*  @param[in] void
*  @return 　　void
*  @details   GPU-based valiation helps to identify the following errors.@n
*             1. Use of uninitialized or incompatible descriptors in a shader.@n
*             2. Use of descriptors referencing deleted Resources in a shader.@n
*             3. Validation of promoted resource states and resource state decay.@n
*             4. Indexing beyond the end of the descriptor heap in a shader.@n
*             5. Shader accesses of resources in incompatible state.@n
*             6. Use of uninitialized or incompatible Samplers in a shader.@n
*****************************************************************************/
void GraphicsDeviceDirectX12::EnabledGPUBasedValidation()
{
	

}

/****************************************************************************
*                     LogAdapters
*************************************************************************//**
*  @fn        void DirectX12::LogAdapters(void)
*  @brief     Show all adapter name (GeForce... )
*  @param[in] Adapter* adapter
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::LogAdapters()
{
	
}

/****************************************************************************
*                     LogAdapterOutputs
*************************************************************************//**
*  @fn        void DirectX12::LogAdapterOutputs(Adapter* adapter)
*  @brief     Show all display output name
*  @param[in] Adapter* adapter
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::LogAdapterOutputs(Adapter* adapter)
{
	
}

/****************************************************************************
*                     LogOutputDisplayModes
*************************************************************************//**
*  @fn        void DirectX12::LogOutputDisplayModes(Output* output, DXGI_FORMAT format)
*  @brief     Show display modes (output screen width, height and refresh rates)
*  @param[in] Output* output: for display modelist
*  @param[in] DXGI_FORMAT   : format
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::LogOutputDisplayModes(Output* output, DXGI_FORMAT format)
{
	

}
#pragma endregion Debug Function
#pragma endregion Private Function