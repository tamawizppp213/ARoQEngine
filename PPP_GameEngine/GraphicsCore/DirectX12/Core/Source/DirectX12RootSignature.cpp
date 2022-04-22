//////////////////////////////////////////////////////////////////////////////////
//              @file   a.cpp
///             @brief  a
///             @author Toide Yutaro
///             @date   -
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/DirectX12/Core/Include/DirectX12RootSignature.hpp"
#include "GameUtility/Math/Include/GMHash.hpp"
#include <thread>
#include <map>
#include <mutex>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
static std::map<size_t, RootSignatureComPtr> g_RootSignatureHashMap;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region RootSignature
void RootSignature::DestroyAll()
{
	g_RootSignatureHashMap.clear();
}
void RootSignature::SetStaticSampler(const D3D12_STATIC_SAMPLER_DESC& sampler)
{
	assert(_numInitializedStaticSamplers < _numStaticSampler);
	D3D12_STATIC_SAMPLER_DESC& desc = _staticSamplers[_numInitializedStaticSamplers++];
	desc = sampler;
}
void RootSignature::SetStaticSampler(SamplerType type)
{
	assert(_numInitializedStaticSamplers < _numStaticSampler);
	_staticSamplers[_numInitializedStaticSamplers] = GetStaticSampler(type, _numInitializedStaticSamplers);
	_numInitializedStaticSamplers++;
}

void RootSignature::Reset(UINT numParameter, UINT numStaticSampler)
{
	/*-------------------------------------------------------------------
	-                      Define root parameter size
	---------------------------------------------------------------------*/
	if (numParameter > 0) { _rootParameters.reset(new RootParameter[numParameter]); }
	else                  { _rootParameters = nullptr; }
	_numRootParameter = numParameter;
	/*-------------------------------------------------------------------
	-                      Define static sampler desc size
	---------------------------------------------------------------------*/
	if (numStaticSampler > 0){ _staticSamplers.reset(new D3D12_STATIC_SAMPLER_DESC[numStaticSampler]); }
	else                     { _staticSamplers = nullptr; }
	_numStaticSampler             = numStaticSampler;
	_numInitializedStaticSamplers = 0;
	_hasFinalized = false;
}

void RootSignature::Create(IDevice* device, const std::wstring& name, D3D12_ROOT_SIGNATURE_FLAGS flags)
{
	if (_hasFinalized) { return; }

	/*-------------------------------------------------------------------
	-               Set Root Signature Desc
	---------------------------------------------------------------------*/
	size_t hashCode = 0;
	D3D12_ROOT_SIGNATURE_DESC rootDescriptor = {};
	rootDescriptor.NumParameters     = _numRootParameter;
	rootDescriptor.pParameters       = (const D3D12_ROOT_PARAMETER*)_rootParameters.get();
	rootDescriptor.NumStaticSamplers = _numStaticSampler;
	rootDescriptor.pStaticSamplers   = (const D3D12_STATIC_SAMPLER_DESC*)_staticSamplers.get();
	rootDescriptor.Flags             = flags;
	
	hashCode = gm::HashState(&rootDescriptor.Flags);
	hashCode = gm::HashState(&rootDescriptor.pStaticSamplers, _numStaticSampler, hashCode);
	/*-------------------------------------------------------------------
	-               Set Root Signature Desc
	---------------------------------------------------------------------*/
	_descriptorTableBitMap = 0;
	_samplerTableBitMap    = 0;
	for (UINT parameter = 0; parameter < _numRootParameter; ++parameter)
	{
		const D3D12_ROOT_PARAMETER& rootParameter = rootDescriptor.pParameters[parameter];
		_descriptorTableSize[parameter] = 0;

		if (rootParameter.ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
		{
			assert(rootParameter.DescriptorTable.pDescriptorRanges != nullptr);
			hashCode = gm::HashState(rootParameter.DescriptorTable.pDescriptorRanges, rootParameter.DescriptorTable.NumDescriptorRanges, hashCode);
			
			// We keep track of sampler descriptor tables separately from CBV_SRV_UAV descriptor tables
			if (rootParameter.DescriptorTable.pDescriptorRanges->RangeType == D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER)
			{
				_samplerTableBitMap |= (1 << parameter);
			}
			else
			{
				_descriptorTableBitMap |= (1 << parameter);
			}

			for (UINT tableRange = 0; tableRange < rootParameter.DescriptorTable.NumDescriptorRanges; ++tableRange)
			{
				_descriptorTableSize[parameter] += rootParameter.DescriptorTable.pDescriptorRanges[tableRange].NumDescriptors;
			}
		}
		else
		{
			hashCode = gm::HashState(&rootParameter, 1, hashCode);
		}
	}
	/*-------------------------------------------------------------------
	-                Find RootSignature From Hashmap
	---------------------------------------------------------------------*/
	IRootSignature** rootSignatureReference = nullptr;
	bool firstComplile = false;
	{
		static std::mutex hashMapMutex;
		std::lock_guard<std::mutex> lockGuard(hashMapMutex);

		auto iterator = g_RootSignatureHashMap.find(hashCode);
		if (iterator == g_RootSignatureHashMap.end())
		{
			g_RootSignatureHashMap[hashCode].GetAddressOf();
			firstComplile = true;
		}
		else
		{
			rootSignatureReference = iterator->second.GetAddressOf();
		}
	}

	/*-------------------------------------------------------------------
	-                      Set rootSignature
	---------------------------------------------------------------------*/
	if (firstComplile)
	{
		BlobComPtr rootSigBlob = nullptr;
		BlobComPtr errorBlob   = nullptr;

		HRESULT hresult = D3D12SerializeRootSignature(&rootDescriptor, D3D_ROOT_SIGNATURE_VERSION_1, &rootSigBlob, &errorBlob);

		if (errorBlob != nullptr)
		{
			::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		}
		if (FAILED(hresult))
		{
			::OutputDebugStringA("Result False");
		}

		if (FAILED(device->CreateRootSignature( 0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&_rootSignature))))
		{
			::OutputDebugStringA("Failed to create rootsignature");
		};

		_rootSignature->SetName(name.c_str());
		g_RootSignatureHashMap[hashCode].Attach(_rootSignature);
	}
	else
	{
		while (*rootSignatureReference == nullptr) { std::this_thread::yield(); }
		_rootSignature = *rootSignatureReference;
	}
	_hasFinalized = true;
}

#pragma endregion RootSignature