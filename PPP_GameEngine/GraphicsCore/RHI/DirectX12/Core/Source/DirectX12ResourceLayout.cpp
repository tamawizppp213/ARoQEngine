//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12ResourceLayout.cpp
///             @brief  Resource Layout
///             @author Toide Yutaro
///             @date   2022_08_09
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12ResourceLayout.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Device.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Debug.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12EnumConverter.hpp"
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUSampler.hpp"
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
RHIResourceLayout::~RHIResourceLayout()
{
	if (_rootSignature) { _rootSignature.Reset(); }
}
RHIResourceLayout::RHIResourceLayout(const std::shared_ptr<core::RHIDevice>& device, const std::vector<core::ResourceLayoutElement>& elements, const std::vector<core::SamplerLayoutElement>& samplers, const std::optional<core::Constant32Bits>& constant32Bits, const std::wstring& name)
	: core::RHIResourceLayout(device, elements, samplers, constant32Bits)
{
	SetUp();
	SetName(name);
}
RHIResourceLayout::RHIResourceLayout(const std::shared_ptr<core::RHIDevice>& device, const core::ResourceLayoutElement& element, const core::SamplerLayoutElement& sampler, const std::optional<core::Constant32Bits>& constant32Bits, const std::wstring& name)
	: core::RHIResourceLayout(device, element, sampler, constant32Bits)
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
	DeviceComPtr dxDevice = std::static_pointer_cast<directX12::RHIDevice>(_device)->GetDevice();

	std::vector<D3D12_DESCRIPTOR_RANGE>    ranges(_elements.size());
	std::vector<D3D12_STATIC_SAMPLER_DESC> staticSamplerArrays = {};
	
	/*-------------------------------------------------------------------
	-                   Set resource layout state
	---------------------------------------------------------------------*/
	for (size_t i = 0; i < _elements.size(); ++i)
	{
		ranges[i].RangeType                         = EnumConverter::Convert1(_elements[i].DescriptorType);
		ranges[i].NumDescriptors                    = 1;
		ranges[i].BaseShaderRegister                = static_cast<UINT>(_elements[i].Binding);
		ranges[i].RegisterSpace                     = static_cast<UINT>(_elements[i].RegisterSpace);
		ranges[i].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
	}

	/*-------------------------------------------------------------------
	-                   Set static sampler states
	---------------------------------------------------------------------*/
	for (auto& sampler : _samplers)
	{
		auto& samplerInfo = static_cast<rhi::directX12::GPUSampler*>(sampler.Sampler.get())->GetSamplerDesc();
		samplerInfo.ShaderVisibility = EnumConverter::Convert(sampler.Visibility);
		samplerInfo.ShaderRegister   = static_cast<UINT>(sampler.Binding);
		samplerInfo.RegisterSpace    = static_cast<UINT>(sampler.RegisterSpace);
		staticSamplerArrays.push_back(samplerInfo);
	}

	/*-------------------------------------------------------------------
	-                   Set Root Parameter
	---------------------------------------------------------------------*/
	std::vector<D3D12_ROOT_PARAMETER> parameters = {};
	if (!_elements.empty())
	{
		_elementsCount = _elements.size();
		for (size_t i = 0; i < _elementsCount; ++i)
		{
			D3D12_ROOT_PARAMETER parameter = {};
			parameter.ParameterType                       = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			parameter.ShaderVisibility                    = D3D12_SHADER_VISIBILITY_ALL;
			parameter.DescriptorTable.NumDescriptorRanges = 1;
			parameter.DescriptorTable.pDescriptorRanges   = &ranges[i];
			parameters.push_back(parameter);
		}
	}
	

	/*-------------------------------------------------------------------
	-                   Constant 32 bits
	---------------------------------------------------------------------*/
	if (_constant32Bits.has_value())
	{
		_constant32BitsCount = parameters.size();

		D3D12_ROOT_PARAMETER parameter = {};
		parameter.ParameterType            = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		parameter.ShaderVisibility         = EnumConverter::Convert(_constant32Bits->Visibility);
		parameter.Constants.Num32BitValues = static_cast<UINT>(_constant32Bits->Count);
		parameter.Constants.ShaderRegister = static_cast<UINT>(_constant32Bits->Binding);
		parameter.Constants.RegisterSpace  = static_cast<UINT>(_constant32Bits->RegisterSpace);
		parameters.push_back(parameter);
	}
	/*-------------------------------------------------------------------
	-                   Set Rootsignature descriptor
	---------------------------------------------------------------------*/
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.NumStaticSamplers = static_cast<UINT>(staticSamplerArrays.size());
	rootSignatureDesc.NumParameters     = static_cast<UINT>(parameters.size());
	rootSignatureDesc.pParameters       = parameters.data();
	rootSignatureDesc.pStaticSamplers   = staticSamplerArrays.data();
	rootSignatureDesc.Flags             = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	
	/*-------------------------------------------------------------------
	-                   Create RootSignature
	---------------------------------------------------------------------*/
	BlobComPtr rootSigBlob = nullptr;
	BlobComPtr errorBlob   = nullptr;

	HRESULT hresult = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, rootSigBlob.GetAddressOf(), errorBlob.GetAddressOf());

	if (errorBlob != nullptr)
	{
		::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}
	if (FAILED(hresult))
	{
		::OutputDebugStringA("Result False");
	}

	if (FAILED(dxDevice->CreateRootSignature(1, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&_rootSignature))))
	{
		::OutputDebugStringA("Failed to create rootsignature");
	};

	_rootSignature->SetName(L"RootSignature");
	
}

void RHIResourceLayout::SetName(const std::wstring& name)
{
	_rootSignature->SetName(name.c_str());
}
#pragma endregion SetUp Function