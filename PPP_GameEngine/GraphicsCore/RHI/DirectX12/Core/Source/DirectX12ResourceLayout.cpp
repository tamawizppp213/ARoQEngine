//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12ResourceLayout.cpp
///             @brief  Resource Layout
/// 　　　　　　　　　　　　　　https://zenn.dev/piffett/scraps/b6ce0c751487b1
///                     https://shikihuiku.wordpress.com/2017/11/30/d3d12_rootsignature_1_1/
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

#pragma warning(disable: 6001)
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
RHIResourceLayout::~RHIResourceLayout()
{
	if (_rootSignature) { _rootSignature.Reset(); }
}
RHIResourceLayout::RHIResourceLayout(const gu::SharedPointer<core::RHIDevice>& device, const gu::DynamicArray<core::ResourceLayoutElement>& elements, const gu::DynamicArray<core::SamplerLayoutElement>& samplers, const std::optional<core::Constant32Bits>& constant32Bits, const gu::tstring& name)
	: core::RHIResourceLayout(device, elements, samplers, constant32Bits)
{
	SetUp();
	SetName(name);
}
RHIResourceLayout::RHIResourceLayout(const gu::SharedPointer<core::RHIDevice>& device, const core::ResourceLayoutElement& element, const core::SamplerLayoutElement& sampler, const std::optional<core::Constant32Bits>& constant32Bits, const gu::tstring& name)
	: core::RHIResourceLayout(device, element, sampler, constant32Bits)
{
	SetUp();
	SetName(name);
}

RHIResourceLayout::RHIResourceLayout(const gu::SharedPointer<core::RHIDevice>& device, const rhi::core::RHIResourceLayoutDesc& desc, const gu::tstring& name)
	: core::RHIResourceLayout(device, desc)
{
	SetUp();
	SetName(name);
}

#pragma region SetUp Function
/****************************************************************************
*                     SetUp
*************************************************************************//**
*  @fn        void RHIResourceLayout::SetUp(const gu::DynamicArray<core::ResourceLayoutElement>& elements, const gu::DynamicArray<core::SamplerLayoutElement>& samplers, const std::optional<core::Constant32Bits>& constant32Bits)
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
	gu::DynamicArray<D3D12_DESCRIPTOR_RANGE1>   ranges(_desc.Elements.Size());
	gu::DynamicArray<D3D12_SHADER_VISIBILITY>   visibilities(_desc.Elements.Size());
	gu::DynamicArray<D3D12_STATIC_SAMPLER_DESC> staticSamplerArrays = {};

	/*-------------------------------------------------------------------
	-     リソースバインディング階層に基づいて、ディスクリプタまたはそのデータが静的であるかどうかを判断します
	-  (バインディング階層に基づいて)ディスクリプタテーブルがコピーするディスクリプタの数よりも大きくなることがあるため,これを行います.
	-  https://shikihuiku.wordpress.com/2017/11/30/d3d12_rootsignature_1_1/
	---------------------------------------------------------------------*/
	const D3D12_DESCRIPTOR_RANGE_FLAGS srvDescriptorRangeFlags = 
		(_bindingTier <= D3D12_RESOURCE_BINDING_TIER_1) ?
		D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE :
		D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE |
		D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE;

	const D3D12_DESCRIPTOR_RANGE_FLAGS cbvDescriptorRangeFlags = 
		(_bindingTier <= D3D12_RESOURCE_BINDING_TIER_2) ?
		D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE :
		D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC_WHILE_SET_AT_EXECUTE | 
		D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE;

	const D3D12_DESCRIPTOR_RANGE_FLAGS uavDescriptorRangeFlags = 
		(_bindingTier <= D3D12_RESOURCE_BINDING_TIER_2) ?
		D3D12_DESCRIPTOR_RANGE_FLAG_DATA_VOLATILE :
		D3D12_DESCRIPTOR_RANGE_FLAG_DATA_VOLATILE | 
		D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE;

	const D3D12_DESCRIPTOR_RANGE_FLAGS samplerDescriptorRangeFlags = 
		(_bindingTier <= D3D12_RESOURCE_BINDING_TIER_1) ?
		D3D12_DESCRIPTOR_RANGE_FLAG_NONE :                // 静的
		D3D12_DESCRIPTOR_RANGE_FLAG_DESCRIPTORS_VOLATILE; // 変更がありうるものとして扱う

	//const D3D12_ROOT_DESCRIPTOR_FLAGS cbvRootDescriptorFlags = 
	//	D3D12_ROOT_DESCRIPTOR_FLAG_DATA_STATIC;	// 一度Bindされた時点で変更がないものとして扱われる.
	// We always set the Data in an upload heap before calling Set*RootConstantBufferView.

	/*-------------------------------------------------------------------
	-                   Set resource layout state
	---------------------------------------------------------------------*/
	gu::DynamicArray<int32> shaderStageCounter((int32)core::ShaderVisibleFlag::CountOfPipeline);
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

	for (size_t i = 0; i < _desc.Elements.Size(); ++i)
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

		// flag設定, resource viewを持っていることを設定
		if (_desc.Elements[i].DescriptorType == core::DescriptorHeapType::CBV)
		{
			ranges[i].Flags = cbvDescriptorRangeFlags;
			_hasCBV = true;
		}
		else if (_desc.Elements[i].DescriptorType == core::DescriptorHeapType::SRV)
		{
			ranges[i].Flags = srvDescriptorRangeFlags;
			_hasSRV = true;
		}
		else if (_desc.Elements[i].DescriptorType == core::DescriptorHeapType::UAV)
		{
			ranges[i].Flags = uavDescriptorRangeFlags;
			_hasUAV = true;
		}
		else if (_desc.Elements[i].DescriptorType == core::DescriptorHeapType::SAMPLER)
		{
			ranges[i].Flags = samplerDescriptorRangeFlags;
			_hasDynamicSampler = true;
		}
		
		// 各描画パイプラインのステージでの個数を取得
		for (size_t j = 0; j < shaderStageCounter.Size(); ++j)
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
		staticSamplerArrays.Push(samplerInfo);
	}
	_hasStaticSampler = !staticSamplerArrays.IsEmpty();

	/*-------------------------------------------------------------------
	-                   Set Root Parameter
	---------------------------------------------------------------------*/
	gu::DynamicArray<D3D12_ROOT_PARAMETER1> parameters = {};
	if (!_desc.Elements.IsEmpty())
	{
		_elementsCount = _desc.Elements.Size();
		parameters.Resize(_elementsCount);
		for (size_t i = 0; i < _elementsCount; ++i)
		{
			D3D12_ROOT_PARAMETER1 parameter = {};
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
		_constant32BitsCount = parameters.Size();

		D3D12_ROOT_PARAMETER1 parameter = {};
		parameter.ParameterType            = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		parameter.ShaderVisibility         = EnumConverter::Convert(_desc.Constant32Bits->Visibility);
		parameter.Constants.Num32BitValues = static_cast<UINT>(_desc.Constant32Bits->Count);
		parameter.Constants.ShaderRegister = static_cast<UINT>(_desc.Constant32Bits->Binding);
		parameter.Constants.RegisterSpace  = static_cast<UINT>(_desc.Constant32Bits->RegisterSpace);
		parameters.Push(parameter);
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

		if (!_desc.Elements.IsEmpty())
		{
			// Visibility部分はD3D12_ROOT_SIGNATURE_FLAGSと同じ値を採用している
			// 各描画パイプライン上のDeny flagを立てる
			
			for (size_t i = 0; i < shaderStageCounter.Size(); ++i)
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
	D3D12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.Version = rhiDevice->GetMaxRootSignatureVersion() >= D3D_ROOT_SIGNATURE_VERSION_1_1 ? D3D_ROOT_SIGNATURE_VERSION_1_1 : D3D_ROOT_SIGNATURE_VERSION_1_0;

	if (_desc.ResourceLayoutType == core::RootSignatureType::RayTracingLocal)
	{
		// Local root signatureの場合はstatic samplerのレジスタ範囲がGlobal root signatureと
		// 同じになってしまうため, static sampler stateは使用しないようにします
		rootSignatureDesc.Desc_1_1.NumStaticSamplers = 0;
		rootSignatureDesc.Desc_1_1.NumParameters     = static_cast<UINT>(parameters.Size());
		rootSignatureDesc.Desc_1_1.pParameters       = parameters.Data();
		rootSignatureDesc.Desc_1_1.pStaticSamplers   = nullptr;
		rootSignatureDesc.Desc_1_1.Flags             = flags;
	}
	else
	{
		if (_bindingTier > D3D12_RESOURCE_BINDING_TIER_1)
		{
			rootSignatureDesc.Desc_1_1.NumStaticSamplers = static_cast<UINT>(staticSamplerArrays.Size());
			rootSignatureDesc.Desc_1_1.NumParameters     = static_cast<UINT>(parameters.Size());
			rootSignatureDesc.Desc_1_1.pParameters       = parameters.Data();
			rootSignatureDesc.Desc_1_1.pStaticSamplers   = staticSamplerArrays.Data();
			rootSignatureDesc.Desc_1_1.Flags             = flags;
		}
		else
		{
			rootSignatureDesc.Desc_1_1.NumStaticSamplers = 0;
			rootSignatureDesc.Desc_1_1.NumParameters     = static_cast<UINT>(parameters.Size());
			rootSignatureDesc.Desc_1_1.pParameters       = parameters.Data();
			rootSignatureDesc.Desc_1_1.pStaticSamplers   = nullptr;
			rootSignatureDesc.Desc_1_1.Flags             = flags;
		}
	}
	
	/*-------------------------------------------------------------------
	-                   Create RootSignature
	---------------------------------------------------------------------*/
	BlobComPtr rootSigBlob = nullptr;
	BlobComPtr errorBlob   = nullptr;

	HRESULT hresult = SerializeVersionedRootSignature(rootSignatureDesc, rootSigBlob.GetAddressOf(), errorBlob.GetAddressOf());

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

void RHIResourceLayout::SetName(const gu::tstring& name)
{
	_rootSignature->SetName(name.CString());
}

HRESULT RHIResourceLayout::SerializeVersionedRootSignature(
	const D3D12_VERSIONED_ROOT_SIGNATURE_DESC& desc,
	ID3DBlob** rootSignatureBlob, ID3DBlob** errorBlob)
{
	/*-------------------------------------------------------------------
	-       Error blob reset
	---------------------------------------------------------------------*/
	if (errorBlob != nullptr)
	{
		errorBlob = nullptr;
	}

	/*-------------------------------------------------------------------
	-       Search max root signatrue version from the device
	---------------------------------------------------------------------*/
	const auto device     = gu::StaticPointerCast<directX12::RHIDevice>(_device);
	const auto maxVersion = device->GetMaxRootSignatureVersion();

	// 利用可能なバージョンが1_1以上である場合は常にVersionedRootsignatureを呼び出す
	if (maxVersion >= D3D_ROOT_SIGNATURE_VERSION_1_1)
	{
		return D3D12SerializeVersionedRootSignature(&desc, rootSignatureBlob, errorBlob);
	}
	
	// 利用可能なバージョンが1_0しかない場合は, 1_0によるRootSignatureを再構築します.
	if (desc.Version >= D3D_ROOT_SIGNATURE_VERSION_1_1)
	{
		HRESULT result = S_OK;

		/*-------------------------------------------------------------------
		-       Acquire parameter info from the newest version
		---------------------------------------------------------------------*/
		const auto& desc11            = desc.Desc_1_1;
		const auto  parameterByteSize = sizeof(D3D12_ROOT_PARAMETER) * desc11.NumParameters;
		auto parameters = (parameterByteSize > 0) ? HeapAlloc(GetProcessHeap(), 0, parameterByteSize) : nullptr;

		// パラメータを取得するヒープ領域を確保できたか
		if (parameterByteSize > 0 && parameters == nullptr)
		{
			return E_OUTOFMEMORY;
		}
		if (parameters == nullptr)
		{
			return E_FAIL;
		}

		// 1_0のバージョンに変更する
		auto parameters10 = static_cast<D3D12_ROOT_PARAMETER*>(parameters);

		/*-------------------------------------------------------------------
		-       Convert the root parameter newest -> 1_0
		---------------------------------------------------------------------*/
		for (uint32 i = 0; i < desc11.NumParameters; ++i)
		{
			// 誤検知の抑制
			__analysis_assume(parameterByteSize == sizeof(D3D12_ROOT_PARAMETER) * desc11.NumParameters);

			// 各root parameterの情報変換
			parameters10[i].ParameterType    = desc11.pParameters[i].ParameterType;
			parameters10[i].ShaderVisibility = desc11.pParameters[i].ShaderVisibility;

			switch (desc11.pParameters[i].ParameterType)
			{
				// 32 bit constants
				case D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS:
				{
					parameters10[i].Constants.Num32BitValues = desc11.pParameters[i].Constants.Num32BitValues;
					parameters10[i].Constants.RegisterSpace  = desc11.pParameters[i].Constants.RegisterSpace;
					parameters10[i].Constants.ShaderRegister = desc11.pParameters[i].Constants.ShaderRegister;
					break;
				}

				// cbv, srv, uav
				case D3D12_ROOT_PARAMETER_TYPE_CBV:
				case D3D12_ROOT_PARAMETER_TYPE_SRV:
				case D3D12_ROOT_PARAMETER_TYPE_UAV:
				{
					parameters10[i].Descriptor.RegisterSpace  = desc11.pParameters[i].Descriptor.RegisterSpace;
					parameters10[i].Descriptor.ShaderRegister = desc11.pParameters[i].Descriptor.ShaderRegister;
					break;
				}

				// descriptor table
				case D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE:
				{
					const auto& table11                = desc11.pParameters[i].DescriptorTable;
					const auto descriptorRangeByteSize = sizeof(D3D12_DESCRIPTOR_RANGE) * table11.NumDescriptorRanges;
					
					/*-------------------------------------------------------------------
					-       Acquire descriptor ranges info from the newest version
					---------------------------------------------------------------------*/
					void* descriptorRanges = descriptorRangeByteSize > 0 && SUCCEEDED(result) ? HeapAlloc(GetProcessHeap(), 0, descriptorRangeByteSize) : nullptr;
					if (descriptorRangeByteSize > 0 && descriptorRanges == nullptr)
					{
						result = E_OUTOFMEMORY;
					}

					auto descriptorRanges10 = static_cast<D3D12_DESCRIPTOR_RANGE*>(descriptorRanges);

					/*-------------------------------------------------------------------
					-       Convert descriptor ranges from the newest version to the 1_0 version
					---------------------------------------------------------------------*/
					if (SUCCEEDED(result))
					{
						for (uint32 j = 0; j < table11.NumDescriptorRanges; ++j)
						{
							__analysis_assume(descriptorRangeByteSize == sizeof(D3D12_DESCRIPTOR_RANGE) * table11.NumDescriptorRanges);
							descriptorRanges10[j].BaseShaderRegister                = table11.pDescriptorRanges[j].BaseShaderRegister;
							descriptorRanges10[j].NumDescriptors                    = table11.pDescriptorRanges[j].NumDescriptors;
							descriptorRanges10[j].OffsetInDescriptorsFromTableStart = table11.pDescriptorRanges[j].OffsetInDescriptorsFromTableStart;
							descriptorRanges10[j].RangeType                         = table11.pDescriptorRanges[j].RangeType;
							descriptorRanges10[j].RegisterSpace                     = table11.pDescriptorRanges[j].RegisterSpace;
						}
					}

					/*-------------------------------------------------------------------
					-      Acquire descriptor table
					---------------------------------------------------------------------*/
					auto& table10 = parameters10[i].DescriptorTable;
					table10.NumDescriptorRanges = table11.NumDescriptorRanges;
					table10.pDescriptorRanges   = descriptorRanges10;
					break;
				}
			}
		}

		if (SUCCEEDED(result))
		{
			const D3D12_ROOT_SIGNATURE_DESC desc10 =
			{
				.NumParameters     = desc11.NumParameters,
				.pParameters       = parameters10,
				.NumStaticSamplers = desc11.NumStaticSamplers,
				.pStaticSamplers   = desc11.pStaticSamplers,
				.Flags             = desc11.Flags
			};
			result = D3D12SerializeRootSignature(&desc10, D3D_ROOT_SIGNATURE_VERSION_1, rootSignatureBlob, errorBlob);
		}

		/*-------------------------------------------------------------------
		-       Terminate process
		---------------------------------------------------------------------*/
		for (uint32 i = 0; i < desc11.NumParameters; ++i)
		{
			if (desc11.pParameters[i].ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
			{
				auto descriptorRanges10 = parameters10[i].DescriptorTable.pDescriptorRanges;
				HeapFree(GetProcessHeap(), 0, reinterpret_cast<void*>(const_cast<D3D12_DESCRIPTOR_RANGE*>(descriptorRanges10)));
			}
			HeapFree(GetProcessHeap(), 0, parameters);
		}

		return result;
	}
	else
	{
		return D3D12SerializeRootSignature(&desc.Desc_1_0, D3D_ROOT_SIGNATURE_VERSION_1, rootSignatureBlob, errorBlob);
	}

}
#pragma endregion SetUp Function