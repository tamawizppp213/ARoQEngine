//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12ResourceLayout.cpp
///             @brief  Resource Layout
///             @author Toide Yutaro
///             @date   2022_08_09
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/DirectX12ResourceLayout.hpp"
#include "../Include/DirectX12Device.hpp"
#include "../Include/DirectX12Debug.hpp"
#include "../Include/DirectX12EnumConverter.hpp"
#include "../../Resource/Include/DirectX12GPUSampler.hpp"
#include <d3d12.h>
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;
using namespace Microsoft::WRL;
using namespace gu;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
RHIResourceLayout::~RHIResourceLayout()
{
	if (_rootSignature) { _rootSignature.Reset(); }
}
RHIResourceLayout::RHIResourceLayout(const gu::SharedPointer<core::RHIDevice>& device, const std::vector<core::ResourceLayoutElement>& elements, const std::vector<core::SamplerLayoutElement>& samplers, const std::optional<core::Constant32Bits>& constant32Bits, const std::wstring& name)
	: core::RHIResourceLayout(device, elements, samplers, constant32Bits)
{
	SetUp();
	SetName(name);
}
RHIResourceLayout::RHIResourceLayout(const gu::SharedPointer<core::RHIDevice>& device, const core::ResourceLayoutElement& element, const core::SamplerLayoutElement& sampler, const std::optional<core::Constant32Bits>& constant32Bits, const std::wstring& name)
	: core::RHIResourceLayout(device, element, sampler, constant32Bits)
{
	SetUp();
	SetName(name);
}

RHIResourceLayout::RHIResourceLayout(const gu::SharedPointer<core::RHIDevice>& device, const rhi::core::RHIResourceLayoutDesc& desc, const std::wstring& name)
	: core::RHIResourceLayout(device, desc)
{
	SetUp();
	SetName(name);
}

#pragma region SetUp Function
/****************************************************************************
*                     SetUp
*************************************************************************//**
*  @fn        void RHIResourceLayout::SetUp(const std::vector<core::ResourceLayoutElement>& elements, const std::vector<core::SamplerLayoutElement>& samplers, const std::optional<core::Constant32Bits>& constant32Bits)
* 
*  @brief     Set up rootsignature
* 
*  @param[in] void
* 
*  @return    void
*****************************************************************************/
void RHIResourceLayout::SetUp()
{
	const auto rhiDevice  = gu::StaticPointerCast<directX12::RHIDevice>(_device);
	DeviceComPtr dxDevice = gu::StaticPointerCast<directX12::RHIDevice>(_device)->GetDevice();
	
	// Binding Tier
	_bindingTier = rhiDevice->GetResourceBindingTier();

	/*-------------------------------------------------------------------
	-                   Prepare
	---------------------------------------------------------------------*/
	std::vector<D3D12_DESCRIPTOR_RANGE>    ranges(_desc.Elements.size());
	std::vector<D3D12_SHADER_VISIBILITY>   visibilities(_desc.Elements.size());
	std::vector<D3D12_STATIC_SAMPLER_DESC> staticSamplerArrays = {};

	/*-------------------------------------------------------------------
	-                   Set resource layout state
	---------------------------------------------------------------------*/
	std::vector<int32> shaderStageCounter((int32)core::ShaderVisibleFlag::CountOfPipeline);
	const core::ShaderVisibleFlag shaderStageFlags[] =
	{
		core::ShaderVisibleFlag::Vertex, 
		core::ShaderVisibleFlag::Hull, 
		core::ShaderVisibleFlag::Domain,
		core::ShaderVisibleFlag::Geometry,
		core::ShaderVisibleFlag::Pixel,
		core::ShaderVisibleFlag::Amplification,
		core::ShaderVisibleFlag::Mesh
	};

	for (size_t i = 0; i < _desc.Elements.size(); ++i)
	{
		const auto visibilityValue = (int16)_desc.Elements[i].Visibility;
		const bool hasSingleIndex = visibilityValue != 0 && ((visibilityValue & (visibilityValue - 1)) == 0);
		// フラグが複数ある場合は必ずALLに設定した後で, 後に続くDeny flagによってパイプラインに流さない設定を行う.
		visibilities[i]                             = hasSingleIndex ? EnumConverter::Convert(_desc.Elements[i].Visibility) : D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_ALL;
		ranges[i].RangeType                         = EnumConverter::Convert1(_desc.Elements[i].DescriptorType);
		ranges[i].NumDescriptors                    = 1;
		ranges[i].BaseShaderRegister                = static_cast<UINT>(_desc.Elements[i].Binding);
		ranges[i].RegisterSpace                     = static_cast<UINT>(_desc.Elements[i].RegisterSpace);
		ranges[i].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

		// 各描画パイプラインのステージでの個数を取得
		for (size_t j = 0; j < shaderStageCounter.size(); ++j)
		{
			if (visibilityValue & (int16)shaderStageFlags[j])
			{
				shaderStageCounter[j]++;
			}
		}
	}

	/*-------------------------------------------------------------------
	-                   Set static sampler states
	---------------------------------------------------------------------*/
	for (auto& sampler : _desc.Samplers)
	{
		auto& samplerInfo = static_cast<rhi::directX12::GPUSampler*>(sampler.Sampler.Get())->GetSamplerDesc();
		samplerInfo.ShaderVisibility = EnumConverter::Convert(sampler.Visibility);
		samplerInfo.ShaderRegister   = static_cast<UINT>(sampler.Binding);
		samplerInfo.RegisterSpace    = static_cast<UINT>(sampler.RegisterSpace);
		staticSamplerArrays.emplace_back(samplerInfo);
	}

	/*-------------------------------------------------------------------
	-                   Set Root Parameter
	---------------------------------------------------------------------*/
	std::vector<D3D12_ROOT_PARAMETER> parameters = {};
	if (!_desc.Elements.empty())
	{
		_elementsCount = _desc.Elements.size();
		parameters.resize(_elementsCount);
		for (size_t i = 0; i < _elementsCount; ++i)
		{
			D3D12_ROOT_PARAMETER parameter = {};
			parameter.ParameterType                       = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			parameter.ShaderVisibility                    = visibilities[i];
			parameter.DescriptorTable.NumDescriptorRanges = 1;
			parameter.DescriptorTable.pDescriptorRanges   = &ranges[i];
			parameters[i] = parameter;
		}
	}
	

	/*-------------------------------------------------------------------
	-                   Constant 32 bits
	---------------------------------------------------------------------*/
	if (_desc.Constant32Bits.has_value())
	{
		_constant32BitsCount = parameters.size();

		D3D12_ROOT_PARAMETER parameter = {};
		parameter.ParameterType            = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		parameter.ShaderVisibility         = EnumConverter::Convert(_desc.Constant32Bits->Visibility);
		parameter.Constants.Num32BitValues = static_cast<UINT>(_desc.Constant32Bits->Count);
		parameter.Constants.ShaderRegister = static_cast<UINT>(_desc.Constant32Bits->Binding);
		parameter.Constants.RegisterSpace  = static_cast<UINT>(_desc.Constant32Bits->RegisterSpace);
		parameters.push_back(parameter);
	}

	/*-------------------------------------------------------------------
	-                   Root signature flags
	---------------------------------------------------------------------*/
	D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_NONE;

	if (_desc.UseDirectlyIndexedResourceHeap)
	{
		flags |= D3D12_ROOT_SIGNATURE_FLAG_CBV_SRV_UAV_HEAP_DIRECTLY_INDEXED;
	}

	if (_desc.UseDirectlyIndexedSamplerHeap)
	{
		flags |= D3D12_ROOT_SIGNATURE_FLAG_SAMPLER_HEAP_DIRECTLY_INDEXED;
	}

	if (_device->IsSupportedDxr())
	{
		if (_desc.ResourceLayoutType == core::RootSignatureType::RayTracingLocal)
		{
			flags = D3D12_ROOT_SIGNATURE_FLAG_LOCAL_ROOT_SIGNATURE;
		}
		else if (_desc.ResourceLayoutType == core::RootSignatureType::RayTracingGlobal)
		{
			flags |= D3D12_ROOT_SIGNATURE_FLAG_NONE;
		}
	}
	if (_desc.ResourceLayoutType == core::RootSignatureType::Rasterize)
	{
		// Input layoutをOnにする
		if (_desc.UseIAInputLayout)
		{
			flags |= D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		}

		if (!_desc.Elements.empty())
		{
			// Visibility部分はD3D12_ROOT_SIGNATURE_FLAGSと同じ値を採用している
			// 各描画パイプライン上のDeny flagを立てる
			
			for (size_t i = 0; i < shaderStageCounter.size(); ++i)
			{
				// 何もShader stageにバインドするものが無ければ次に進む
				if (shaderStageCounter[i] != 0) { continue; }

				// Deny flagを設定する
				flags |= (D3D12_ROOT_SIGNATURE_FLAGS)shaderStageFlags[i];
			}
		}
	}


	/*-------------------------------------------------------------------
	-                   Set Rootsignature descriptor
	---------------------------------------------------------------------*/
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	if (_desc.ResourceLayoutType == core::RootSignatureType::RayTracingLocal)
	{
		// Local root signatureの場合はstatic samplerのレジスタ範囲がGlobal root signatureと
		// 同じになってしまうため, static sampler stateは使用しないようにします
		rootSignatureDesc.NumStaticSamplers = 0;
		rootSignatureDesc.NumParameters     = static_cast<UINT>(parameters.size());
		rootSignatureDesc.pParameters       = parameters.data();
		rootSignatureDesc.pStaticSamplers   = nullptr;
		rootSignatureDesc.Flags             = flags;
	}
	else
	{
		if (_bindingTier > D3D12_RESOURCE_BINDING_TIER_1)
		{
			rootSignatureDesc.NumStaticSamplers = static_cast<UINT>(staticSamplerArrays.size());
			rootSignatureDesc.NumParameters     = static_cast<UINT>(parameters.size());
			rootSignatureDesc.pParameters       = parameters.data();
			rootSignatureDesc.pStaticSamplers   = staticSamplerArrays.data();
			rootSignatureDesc.Flags             = flags;
		}
		else
		{
			rootSignatureDesc.NumStaticSamplers = 0;
			rootSignatureDesc.NumParameters     = static_cast<UINT>(parameters.size());
			rootSignatureDesc.pParameters       = parameters.data();
			rootSignatureDesc.pStaticSamplers   = nullptr;
			rootSignatureDesc.Flags             = flags;
		}
	}
	
	/*-------------------------------------------------------------------
	-                   Create RootSignature
	---------------------------------------------------------------------*/
	BlobComPtr rootSigBlob = nullptr;
	BlobComPtr errorBlob   = nullptr;

	HRESULT hresult = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, rootSigBlob.GetAddressOf(), errorBlob.GetAddressOf());

	if (errorBlob != nullptr)
	{
		_RPTN(_CRT_WARN, "%s", (char*)errorBlob->GetBufferPointer());
	}
	if (FAILED(hresult))
	{
		_RPT0(_CRT_WARN, "Result False\n");
	}

	if (FAILED(dxDevice->CreateRootSignature(_device->GetGPUMask().Value(), rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&_rootSignature))))
	{
		_RPT0(_CRT_WARN, "Failed to create rootsignature\n");
		return;
	};

	_rootSignature->SetName(L"RootSignature");
	
}

void RHIResourceLayout::SetName(const std::wstring& name)
{
	_rootSignature->SetName(name.c_str());
}
#pragma endregion SetUp Function