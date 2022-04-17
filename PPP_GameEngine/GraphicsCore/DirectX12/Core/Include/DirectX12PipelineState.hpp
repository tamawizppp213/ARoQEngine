//////////////////////////////////////////////////////////////////////////////////
///             @file   TemplateText.hpp
///             @brief  TemplateText
///             @author Toide Yutaro
///             @date   2022_03_11
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_PIPELINE_STATE_HPP
#define DIRECTX12_PIPELINE_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "DirectX12Core.hpp"
#include <d3d12.h>
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Class
//////////////////////////////////////////////////////////////////////////////////
/****************************************************************************
*				  			PipelineState
*************************************************************************//**
*  @class     PipelineState
*  @brief     PipelineState
*****************************************************************************/
class PipelineState
{
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	static void DestroyAll();
	
	virtual void CompleteSetting(IDevice* device) = 0;
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/
	inline IPipelineState*       GetPipelineState() const { return _pipelineState; }
	inline const IRootSignature& GetRootSignature() const { return *_rootSignature; }
	virtual void SetRootSignature(IRootSignature& rootSignature) { _rootSignature = &rootSignature; }
	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	PipelineState(const wchar_t* name) : _name(name), _rootSignature(nullptr), _pipelineState(nullptr){};
	virtual ~PipelineState();
protected:
	/****************************************************************************
	**                Private Function
	*****************************************************************************/

	/****************************************************************************
	**                Private Member Variables
	*****************************************************************************/
	IRootSignature* _rootSignature = nullptr;
	IPipelineState* _pipelineState = nullptr;
	const wchar_t* _name;
};

/****************************************************************************
*				  			GraphicsPipelineState
*************************************************************************//**
*  @class     Graphics pipelineState
*  @brief     Graphics PSO
*****************************************************************************/
class GraphicsPipelineState : public PipelineState
{
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	void SetGraphicsPipelineStateDescriptor(const D3D12_GRAPHICS_PIPELINE_STATE_DESC& descriptor) { _psoDescriptor = descriptor; }
	void CompleteSetting(IDevice* device) override;
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/
	void SetBlendState         (const D3D12_BLEND_DESC& blendStateDesc);
	void SetRasterizerState    (const D3D12_RASTERIZER_DESC& rasterizerDesc);
	void SetDepthStencilState  (const D3D12_DEPTH_STENCIL_DESC& depthStencilDesc);
	void SetSampleMask         (UINT sampleMask);
	void SetDepthTargetFormat  (DXGI_FORMAT dsvFormat, UINT msaaCount = 1, UINT msaaQuality = 0);
	void SetRenderTargetFormat (DXGI_FORMAT rtvFormat, DXGI_FORMAT dsvFormat,UINT msaaCount = 1, UINT msaaQuality = 0);
	void SetRenderTargetFormats(UINT numRTVs     , const DXGI_FORMAT* rtvFormats, DXGI_FORMAT dsvFormat, UINT msaaCount = 1, UINT msaaQuality = 0);
	void SetInputLayouts       (UINT elementCount, const D3D12_INPUT_ELEMENT_DESC* inputElementDesc);
	void SetVertexShader       (const D3D12_SHADER_BYTECODE& byteCode);
	void SetPixelShader        (const D3D12_SHADER_BYTECODE& byteCode);
	void SetGeometryShader     (const D3D12_SHADER_BYTECODE& byteCode);
	void SetHullShader         (const D3D12_SHADER_BYTECODE& byteCode);
	void SetDomainShader       (const D3D12_SHADER_BYTECODE& byteCode);

	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	// Start with empty state
	GraphicsPipelineState(const wchar_t* name = L"Unnamed Graphics PSO");
	~GraphicsPipelineState() {};
protected:
	/****************************************************************************
	**                Protected Function
	*****************************************************************************/

	/****************************************************************************
	**                Protected Member Variables
	*****************************************************************************/
	D3D12_GRAPHICS_PIPELINE_STATE_DESC _psoDescriptor;
	std::shared_ptr<D3D12_INPUT_ELEMENT_DESC> _inputLayouts;
};

/****************************************************************************
*				  			Compute
*************************************************************************//**
*  @class     ComputePipelineState
*  @brief     Compute PSO
*****************************************************************************/
class ComputePipelineState : public PipelineState
{
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	void CompleteSetting(IDevice* device) override;
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/
	void SetComputeShader(const D3D12_SHADER_BYTECODE& byteCode);
	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	ComputePipelineState (const wchar_t* name = L"Unnamed Compute PSO");
	~ComputePipelineState() = default;
protected:
	/****************************************************************************
	**                Protected Function
	*****************************************************************************/

	/****************************************************************************
	**                Protected Member Variables
	*****************************************************************************/
	D3D12_COMPUTE_PIPELINE_STATE_DESC _psoDescriptor;
};

inline void GraphicsPipelineState::SetBlendState(const D3D12_BLEND_DESC& blendStateDesc)
{
	_psoDescriptor.BlendState = blendStateDesc;
}
inline void GraphicsPipelineState::SetRasterizerState(const D3D12_RASTERIZER_DESC& rasterizerDesc)
{
	_psoDescriptor.RasterizerState = rasterizerDesc;
}
inline void GraphicsPipelineState::SetDepthStencilState(const D3D12_DEPTH_STENCIL_DESC& depthStencilDesc)
{
	_psoDescriptor.DepthStencilState = depthStencilDesc;
}
inline void GraphicsPipelineState::SetSampleMask(UINT sampleMask)
{
	_psoDescriptor.SampleMask = sampleMask;
}
inline void GraphicsPipelineState::SetDepthTargetFormat(DXGI_FORMAT dsvFormat, UINT msaaCount, UINT msaaQuality)
{
	SetRenderTargetFormats(0, nullptr, dsvFormat, msaaCount, msaaQuality);
}
inline void GraphicsPipelineState::SetRenderTargetFormat(DXGI_FORMAT rtvFormat, DXGI_FORMAT dsvFormat, UINT msaaCount, UINT msaaQuality)
{
	SetRenderTargetFormats(1, &rtvFormat, dsvFormat, msaaCount, msaaQuality);
}
inline void GraphicsPipelineState::SetVertexShader(const D3D12_SHADER_BYTECODE& byteCode)
{
	_psoDescriptor.VS = byteCode;
}
inline void GraphicsPipelineState::SetPixelShader(const D3D12_SHADER_BYTECODE& byteCode)
{
	_psoDescriptor.PS = byteCode;
}
inline void GraphicsPipelineState::SetGeometryShader(const D3D12_SHADER_BYTECODE& byteCode)
{
	_psoDescriptor.GS = byteCode;
}
inline void GraphicsPipelineState::SetHullShader(const D3D12_SHADER_BYTECODE& byteCode)
{
	_psoDescriptor.HS = byteCode;
}
inline void GraphicsPipelineState::SetDomainShader(const D3D12_SHADER_BYTECODE& byteCode)
{
	_psoDescriptor.DS = byteCode;
}
inline void ComputePipelineState::SetComputeShader(const D3D12_SHADER_BYTECODE& byteCode)
{
	_psoDescriptor.CS = byteCode;
}
#endif