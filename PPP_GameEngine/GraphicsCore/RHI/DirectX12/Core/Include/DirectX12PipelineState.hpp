//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12PipelineState.hpp
///             @brief  Pipeline State
///             @author Toide Yutaro
///             @date   2022_04_14
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
#include <string>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class RootSignature;
class GraphicsDeviceDirectX12;
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

	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/
	inline IPipelineState*      GetPipelineState() const { return _pipelineState; }
	inline const RootSignature& GetRootSignature() const { return *_rootSignature; }
	void SetRootSignature(RootSignature& rootSignature) 
	{ 
		_rootSignature = &rootSignature; 
	}
	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	PipelineState() = default;
	~PipelineState();
protected:
	/****************************************************************************
	**                Private Function
	*****************************************************************************/

	/****************************************************************************
	**                Private Member Variables
	*****************************************************************************/
	RootSignature* _rootSignature = nullptr;
	IPipelineState* _pipelineState = nullptr;
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
	void CompleteSetting(IDevice* device, const std::wstring& name = L"Unnamed Graphics PSO");
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/
	void SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE type);
	void SetBlendState         (const D3D12_BLEND_DESC& blendStateDesc);
	void SetRasterizerState    (const D3D12_RASTERIZER_DESC& rasterizerDesc);
	void SetDepthStencilState  (const D3D12_DEPTH_STENCIL_DESC& depthStencilDesc);
	void SetSampleMask         (UINT sampleMask);
	void SetDepthTargetFormat  (DXGI_FORMAT dsvFormat, UINT msaaCount = 1, UINT msaaQuality = 0);
	void SetRenderTargetFormat (DXGI_FORMAT rtvFormat, DXGI_FORMAT dsvFormat,UINT msaaCount = 1, UINT msaaQuality = 0);
	void SetRenderTargetFormats(UINT numRTVs     , const DXGI_FORMAT* rtvFormats, DXGI_FORMAT dsvFormat, UINT msaaCount = 1, UINT msaaQuality = 0);
	void SetInputLayouts       (const D3D12_INPUT_LAYOUT_DESC& desc);
	

	void SetVertexShader       (const D3D12_SHADER_BYTECODE& byteCode);
	void SetPixelShader        (const D3D12_SHADER_BYTECODE& byteCode);
	void SetGeometryShader     (const D3D12_SHADER_BYTECODE& byteCode);
	void SetHullShader         (const D3D12_SHADER_BYTECODE& byteCode);
	void SetDomainShader       (const D3D12_SHADER_BYTECODE& byteCode);

	void SetVertexShader       (const void* bufferPointer, size_t bufferSize);
	void SetPixelShader        (const void* bufferPointer, size_t bufferSize);
	void SetGeometryShader     (const void* bufferPointer, size_t bufferSize);
	void SetHullShader         (const void* bufferPointer, size_t bufferSize);
	void SetDomainShader       (const void* bufferPointer, size_t bufferSize);

	void SetVertexShader       (BlobComPtr blobData);
	void SetPixelShader        (BlobComPtr blobData);
	void SetGeometryShader     (BlobComPtr blobData);
	void SetHullShader         (BlobComPtr blobData);
	void SetDomainShader       (BlobComPtr blobData);
	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	// Start with empty state
	GraphicsPipelineState();
	~GraphicsPipelineState();
protected:
	/****************************************************************************
	**                Protected Function
	*****************************************************************************/

	/****************************************************************************
	**                Protected Member Variablesg_GraphicsPSOHashMap[hashCode]
	*****************************************************************************/
	D3D12_GRAPHICS_PIPELINE_STATE_DESC _psoDescriptor;
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
	void CompleteSetting(IDevice* device, const std::wstring& name = L"Unnamed Compute PSO");
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/
	void SetComputeShader(const D3D12_SHADER_BYTECODE& byteCode);
	void SetComputeShader(const void* bufferPointer, size_t bufferSize);
	void SetComputeShader(BlobComPtr blobData);
	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	ComputePipelineState ();
protected:
	/****************************************************************************
	**                Protected Function
	*****************************************************************************/

	/****************************************************************************
	**                Protected Member Variables
	*****************************************************************************/
	D3D12_COMPUTE_PIPELINE_STATE_DESC _psoDescriptor;
};
inline void GraphicsPipelineState::SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE type)
{
	_psoDescriptor.PrimitiveTopologyType = type;
}
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
inline void GraphicsPipelineState::SetVertexShader(const void* bufferPointer, size_t bufferSize)
{
	D3D12_SHADER_BYTECODE byteCode = { bufferPointer, bufferSize };
	_psoDescriptor.VS = byteCode;
}
inline void GraphicsPipelineState::SetPixelShader(const void* bufferPointer, size_t bufferSize)
{
	D3D12_SHADER_BYTECODE byteCode = { bufferPointer, bufferSize };
	_psoDescriptor.PS = byteCode;
}
inline void GraphicsPipelineState::SetGeometryShader(const void* bufferPointer, size_t bufferSize)
{
	D3D12_SHADER_BYTECODE byteCode = { bufferPointer, bufferSize };
	_psoDescriptor.GS = byteCode;
}
inline void GraphicsPipelineState::SetHullShader(const void* bufferPointer, size_t bufferSize)
{
	D3D12_SHADER_BYTECODE byteCode = { bufferPointer, bufferSize };
	_psoDescriptor.HS = byteCode;
}
inline void GraphicsPipelineState::SetDomainShader(const void* bufferPointer, size_t bufferSize)
{
	D3D12_SHADER_BYTECODE byteCode = { bufferPointer, bufferSize };
	_psoDescriptor.DS = byteCode;
}

inline void GraphicsPipelineState::SetVertexShader(BlobComPtr blobData)
{
	D3D12_SHADER_BYTECODE byteCode = { reinterpret_cast<BYTE*>(blobData->GetBufferPointer()), blobData->GetBufferSize()};
	_psoDescriptor.VS = byteCode;

}
inline void GraphicsPipelineState::SetPixelShader(BlobComPtr blobData)
{
	D3D12_SHADER_BYTECODE byteCode = { reinterpret_cast<BYTE*>(blobData->GetBufferPointer()), blobData->GetBufferSize()};
	_psoDescriptor.PS = byteCode;
}
inline void GraphicsPipelineState::SetGeometryShader(BlobComPtr blobData)
{
	D3D12_SHADER_BYTECODE byteCode = { reinterpret_cast<BYTE*>(blobData->GetBufferPointer()), blobData->GetBufferSize() };
	_psoDescriptor.GS = byteCode;
}
inline void GraphicsPipelineState::SetHullShader(BlobComPtr blobData)
{
	D3D12_SHADER_BYTECODE byteCode = { reinterpret_cast<BYTE*>(blobData->GetBufferPointer()), blobData->GetBufferSize() };
	_psoDescriptor.HS = byteCode;
}
inline void GraphicsPipelineState::SetDomainShader(BlobComPtr blobData)
{
	D3D12_SHADER_BYTECODE byteCode = { reinterpret_cast<BYTE*>(blobData->GetBufferPointer()), blobData->GetBufferSize() };
	_psoDescriptor.DS = byteCode;
}
inline void ComputePipelineState::SetComputeShader(const D3D12_SHADER_BYTECODE& byteCode)
{
	_psoDescriptor.CS = byteCode;
}
inline void ComputePipelineState::SetComputeShader(const void* bufferPointer, size_t bufferSize)
{
	D3D12_SHADER_BYTECODE byteCode = { bufferPointer, bufferSize };
	_psoDescriptor.CS = byteCode;
}
inline void ComputePipelineState::SetComputeShader(BlobComPtr blobData)
{
	D3D12_SHADER_BYTECODE byteCode = { reinterpret_cast<BYTE*>(blobData->GetBufferPointer()), blobData->GetBufferSize() };
	_psoDescriptor.CS = byteCode;
}
#endif