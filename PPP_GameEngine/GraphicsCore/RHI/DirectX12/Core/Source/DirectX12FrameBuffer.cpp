//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12FrameBuffer.cpp
///             @brief  Frame Buffer
///             @author Toide Yutaro
///             @date   2022_07_20
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12FrameBuffer.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12CommandQueue.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Device.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Debug.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12EnumConverter.hpp"
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUTexture.hpp"
#include <d3d12.h>
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;
using namespace Microsoft::WRL;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
RHIFrameBuffer::RHIFrameBuffer(const std::shared_ptr<core::RHIDevice>& device, const std::vector<std::shared_ptr<core::GPUTexture>>& renderTargets, const std::shared_ptr<core::GPUTexture>& depthStencil)
	: core::RHIFrameBuffer(device, renderTargets, depthStencil)
{
	Prepare();
}

RHIFrameBuffer::RHIFrameBuffer(const std::shared_ptr<core::RHIDevice>& device, const std::shared_ptr<core::GPUTexture>& renderTarget, const std::shared_ptr<core::GPUTexture>& depthStencil)
	: core::RHIFrameBuffer(device, renderTarget, depthStencil)
{
	Prepare();
}

#pragma region Prepare
void RHIFrameBuffer::Prepare()
{
	const auto dxDevice = static_pointer_cast<RHIDevice>(_device)->GetDevice();

	/*-------------------------------------------------------------------
	-                Create Render Target Heap
	---------------------------------------------------------------------*/
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeapDesc.NumDescriptors = 1;
	rtvHeapDesc.NodeMask       = 0;
	ThrowIfFailed(dxDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(_renderTargetHeap.GetAddressOf())));
	_renderTargetHeap->SetName(L"DirectX12::RenderTargetHeap");
	_rtvSize = dxDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	/*-------------------------------------------------------------------
	-                Create Depth Stencil Heap
	---------------------------------------------------------------------*/
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
	dsvHeapDesc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.NodeMask       = 0;
	ThrowIfFailed(dxDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(_depthStencilHeap.GetAddressOf())));
	_depthStencilHeap->SetName(L"DirectX12::DepthStencilHeap");
	_dsvSize = dxDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);


	/*-------------------------------------------------------------------
	-                Create Render Target View
	---------------------------------------------------------------------*/
	for (size_t i = 0; i < _renderTargets.size(); ++i)
	{
		D3D12_RENDER_TARGET_VIEW_DESC desc = {};
		desc.Format = EnumConverter::Convert(_renderTargets[i]->GetPixelFormat());

		switch (_renderTargets[i]->GetMetaData().ResourceType)
		{
			case core::ResourceType::Texture2D:
			{
				desc.ViewDimension        = D3D12_RTV_DIMENSION_TEXTURE2D;
				desc.Texture2D.MipSlice   = static_cast<UINT>(_renderTargets[i]->GetMipMapLevels());
				desc.Texture2D.PlaneSlice = 0;
				break;
			}
			case core::ResourceType::Texture2DMultiSample:
			{
				desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DMS;
				break;
			}
			case core::ResourceType::Texture2DArray:
			{
				desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
				desc.Texture2DArray.FirstArraySlice = 0;
				desc.Texture2DArray.MipSlice        = static_cast<UINT>(_renderTargets[i]->GetMipMapLevels());
				desc.Texture2DArray.PlaneSlice      = 0;
				desc.Texture2DArray.ArraySize       = static_cast<UINT>(_renderTargets[i]->GetArrayLength());
				break;
			}
			case core::ResourceType::Texture2DArrayMultiSample:
			{
				desc.ViewDimension                    = D3D12_RTV_DIMENSION_TEXTURE2DMSARRAY;
				desc.Texture2DMSArray.ArraySize       = static_cast<UINT>(_renderTargets[i]->GetArrayLength());
				desc.Texture2DMSArray.FirstArraySlice = 0;
				break;
			}
			default:
			{
				throw std::runtime_error("wrong render target resource type (directX12 api)");
			}
		}

		dxDevice->CreateRenderTargetView(
			std::static_pointer_cast<directX12::GPUTexture>(_renderTargets[i])->GetResource().Get(),
			&desc,
			{ _renderTargetHeap->GetCPUDescriptorHandleForHeapStart().ptr + i * _rtvSize }
		);
	}

	/*-------------------------------------------------------------------
	-				 Set Depth / Stencil Descriptor
	---------------------------------------------------------------------*/
	if (_depthStencil != nullptr)
	{
		D3D12_DEPTH_STENCIL_VIEW_DESC desc = {};
		desc.Format = EnumConverter::Convert(_depthStencil->GetPixelFormat());
		desc.Flags  = D3D12_DSV_FLAG_NONE;
		switch (_depthStencil->GetMetaData().ResourceType)
		{
			case core::ResourceType::Texture2D:
			{
				desc.ViewDimension        = D3D12_DSV_DIMENSION_TEXTURE2D;
				desc.Texture2D.MipSlice   = static_cast<UINT>(_depthStencil->GetMipMapLevels());
				break;
			}
			case core::ResourceType::Texture2DMultiSample:
			{
				desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DMS;
				break;
			}
			case core::ResourceType::Texture2DArray:
			{
				desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DARRAY;
				desc.Texture2DArray.FirstArraySlice = 0;
				desc.Texture2DArray.MipSlice        = static_cast<UINT>(_depthStencil->GetMipMapLevels());
				desc.Texture2DArray.ArraySize       = static_cast<UINT>(_depthStencil->GetArrayLength());
				break;
			}
			case core::ResourceType::Texture2DArrayMultiSample:
			{
				desc.ViewDimension                    = D3D12_DSV_DIMENSION_TEXTURE2DMSARRAY;
				desc.Texture2DMSArray.ArraySize       = static_cast<UINT>(_depthStencil->GetArrayLength());
				desc.Texture2DMSArray.FirstArraySlice = 0;
				break;
			}
			default:
			{
				throw std::runtime_error("wrong resource type (directX12 api)");
			}
		}
		dxDevice->CreateDepthStencilView(
			std::static_pointer_cast<directX12::GPUTexture>(_depthStencil)->GetResource().Get(),
			&desc,
			{ _depthStencilHeap->GetCPUDescriptorHandleForHeapStart().ptr }
		);
	}
}
#pragma endregion Prepare