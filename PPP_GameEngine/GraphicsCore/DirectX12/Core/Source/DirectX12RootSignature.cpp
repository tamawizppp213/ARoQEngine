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
static std::map<size_t, RootSignatureComPtr> s_RootSignatureHashMap;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region RootSignature
void RootSignature::DestroyAll()
{
	s_RootSignatureHashMap.clear();
}
void RootSignature::SetStaticSampler(const D3D12_STATIC_SAMPLER_DESC& sampler)
{
	assert(_numInitializedStaticSamplers < _numStaticSampler);
	D3D12_STATIC_SAMPLER_DESC& desc = _staticSamplers[_numInitializedStaticSamplers++];
	desc = sampler;
}
/****************************************************************************
*							SetStaticSampler
*************************************************************************//**
*  @fn        void RootSignature::SetStaticSampler(SamplerType type)
*  @brief     Set static sampler
*  @param[in] SamplerType type
*  @return @ void
*****************************************************************************/
void RootSignature::SetStaticSampler(SamplerType type)
{
	assert(_numInitializedStaticSamplers < _numStaticSampler);
	_staticSamplers[_numInitializedStaticSamplers] = GetStaticSampler(type, _numInitializedStaticSamplers);
	_numInitializedStaticSamplers++;
}
/****************************************************************************
*							Reset
*************************************************************************//**
*  @fn        void RootSignature::Reset(UINT numParameter, UINT numStaticSampler)
*  @brief     Reset root parameter and static sampler
*  @param[in] UINT numParameter
*  @param[in] UINT numStaticSampler
*  @return @  void
*****************************************************************************/
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
/****************************************************************************
*							CompleteSetting
*************************************************************************//**
*  @fn        void RootSignature::CompleteSetting(IDevice* device, const std::wstring& name, D3D12_ROOT_SIGNATURE_FLAGS flags)
*  @brief     Complete Setting
*  @param[in,out] IDevice* device
*  @param[in]     const std::wstring& name
*  @param[in]     D3D12_ROOT_SIGNATURE_FLAG flag
*  @return @ void
*****************************************************************************/
void RootSignature::CompleteSetting(IDevice* device, const std::wstring& name, D3D12_ROOT_SIGNATURE_FLAGS flags)
{
	if (_hasFinalized) { printf("Already complete rootSignature\n"); return; }

	/*-------------------------------------------------------------------
	-               Set Root Signature Desc
	---------------------------------------------------------------------*/
	size_t hashCode = 0;
	D3D12_ROOT_SIGNATURE_DESC rootDescriptor = {};
	rootDescriptor.NumParameters     = _numRootParameter;
	rootDescriptor.pParameters       = _rootParameters.get()->GetAddress();
	rootDescriptor.NumStaticSamplers = _numStaticSampler;
	rootDescriptor.pStaticSamplers   = (const D3D12_STATIC_SAMPLER_DESC*)_staticSamplers.get();
	rootDescriptor.Flags             = flags;
	
	hashCode = gm::HashState(&rootDescriptor.Flags);
	hashCode = gm::HashState(&rootDescriptor.pStaticSamplers, _numStaticSampler, hashCode);
	/*-------------------------------------------------------------------
	-               Set Root Signature Desc
	---------------------------------------------------------------------*/
	for (UINT parameter = 0; parameter < _numRootParameter; ++parameter)
	{
		const D3D12_ROOT_PARAMETER& rootParameter = rootDescriptor.pParameters[parameter];

		if (rootParameter.ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
		{
			assert(rootParameter.DescriptorTable.pDescriptorRanges != nullptr);
			hashCode = gm::HashState(rootParameter.DescriptorTable.pDescriptorRanges, rootParameter.DescriptorTable.NumDescriptorRanges, hashCode);
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

		auto iterator = s_RootSignatureHashMap.find(hashCode);
		if (iterator == s_RootSignatureHashMap.end())
		{
			rootSignatureReference = s_RootSignatureHashMap[hashCode].GetAddressOf();
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

		HRESULT hresult = D3D12SerializeRootSignature(&rootDescriptor, D3D_ROOT_SIGNATURE_VERSION_1, rootSigBlob.GetAddressOf(), errorBlob.GetAddressOf());

		if (errorBlob != nullptr)
		{
			::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		}
		if (FAILED(hresult))
		{
			::OutputDebugStringA("Result False");
		}

		if (FAILED(device->CreateRootSignature( 1, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&_rootSignature))))
		{
			::OutputDebugStringA("Failed to create rootsignature");
		};

		_rootSignature->SetName(name.c_str());
		s_RootSignatureHashMap[hashCode].Attach(_rootSignature);
		assert(*rootSignatureReference == _rootSignature);
	}
	else
	{
		while (*rootSignatureReference == nullptr) { std::this_thread::yield(); }
		_rootSignature = *rootSignatureReference;
	}
	_hasFinalized = true;
}

#pragma endregion RootSignature