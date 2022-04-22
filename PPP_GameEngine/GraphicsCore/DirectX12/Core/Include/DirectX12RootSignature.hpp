//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12RootSignature.hpp
///             @brief  RootSignature
///             @author Toide Yutaro
///             @date   2022_04_17
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_ROOT_SIGNATURE_HPP
#define DIRECTX12_ROOT_SIGNATURE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "DirectX12Core.hpp"
#include "DirectX12StaticSampler.hpp"
#include <memory>
#include <string>

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
class RootParameter
{
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	void Clear();
	void InitAsConstants(UINT shaderRegister, UINT num32BitValues, UINT registerSpace = 0, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL);
	void InitAsCBV      (UINT shaderRegister,                      UINT registerSpace = 0, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL);
	void InitAsSRV      (UINT shaderRegister                     , UINT registerSpace = 0, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL);
	void InitAsUAV      (UINT shaderRegister                     , UINT registerSpace = 0, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL);
	// one descriptor range 
	void InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE type, UINT shaderRegister, UINT descriptorCount, UINT registerSpace = 0, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL);
	// multi descriptor range
	void InitAsDescriptorTable(UINT descriptorRangeCount, D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_ALL);
	void SetTableRange   (UINT rangeIndex, D3D12_DESCRIPTOR_RANGE_TYPE type, UINT shaderRegister, UINT descriptorCount, UINT registerSpace = 0);
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/

	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	RootParameter(){ _rootParameter.ParameterType = (D3D12_ROOT_PARAMETER_TYPE)0xFFFFFFFF; }
	explicit RootParameter(const D3D12_ROOT_PARAMETER& rootParameter) { _rootParameter = rootParameter; }
	virtual ~RootParameter() { Clear(); }

	const D3D12_ROOT_PARAMETER& operator()(void) const { return _rootParameter; }
protected:
	/****************************************************************************
	**                Protected Function
	*****************************************************************************/

	/****************************************************************************
	**                Protected Member Variables
	*****************************************************************************/
	D3D12_ROOT_PARAMETER _rootParameter;
};

/****************************************************************************
*				  			TemplateClass
*************************************************************************//**
*  @class     TemplateClass
*  @brief     temp
*****************************************************************************/
class RootSignature
{
	using RootParameterPtr = std::unique_ptr<RootParameter[]>;
	using StaticSamplerPtr = std::unique_ptr<D3D12_STATIC_SAMPLER_DESC[]>;
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	static void DestroyAll();
	void SetStaticSampler(SamplerType type);
	void SetStaticSampler(const D3D12_STATIC_SAMPLER_DESC& sampler);
	void Reset(UINT numParameter = 0, UINT numStaticSampler = 0);
	void Create(IDevice* device, const std::wstring& name, D3D12_ROOT_SIGNATURE_FLAGS flags = D3D12_ROOT_SIGNATURE_FLAG_NONE);
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/
	IRootSignature* GetSignature() const { return _rootSignature; }
	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	RootSignature(UINT numParameter = 0, UINT numStaticSampler = 0) { Reset(numParameter, numStaticSampler); }
	~RootSignature() = default;
	RootParameter& operator[] (size_t entryIndex)
	{
		return _rootParameters.get()[entryIndex];
	}
	const RootParameter& operator[](size_t entryIndex) const
	{
		return _rootParameters.get()[entryIndex];
	}
protected:
	/****************************************************************************
	**                Protected Function
	*****************************************************************************/

	/****************************************************************************
	**                Protected Member Variables
	*****************************************************************************/
	bool            _hasFinalized    = false;
	UINT            _numStaticSampler = 0;
	UINT            _numRootParameter = 0;
	UINT            _numInitializedStaticSamplers = 0;
	UINT32          _descriptorTableBitMap; // One bit is set for root parameters that are non-sampler descriptor tables
	UINT32          _samplerTableBitMap;
	UINT32          _descriptorTableSize[16];
	IRootSignature*  _rootSignature  = nullptr;
	RootParameterPtr _rootParameters = nullptr;
	StaticSamplerPtr _staticSamplers = nullptr;
};

#pragma region Root Parameter
inline void RootParameter::Clear()
{
	if (_rootParameter.ParameterType == D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE)
	{
		delete[] _rootParameter.DescriptorTable.pDescriptorRanges;
	}
	_rootParameter.ParameterType = (D3D12_ROOT_PARAMETER_TYPE)0xFFFFFFFF;
}
inline void RootParameter::InitAsConstants(UINT shaderRegister, UINT num32BitValues, UINT registerSpace, D3D12_SHADER_VISIBILITY visibility)
{
	_rootParameter.ParameterType            = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	_rootParameter.ShaderVisibility         = visibility;
	_rootParameter.Constants.Num32BitValues = num32BitValues;
	_rootParameter.Constants.RegisterSpace  = registerSpace;
	_rootParameter.Constants.ShaderRegister = shaderRegister;
}
inline void RootParameter::InitAsCBV(UINT shaderRegister, UINT registerSpace, D3D12_SHADER_VISIBILITY visibility)
{
	_rootParameter.ParameterType             = D3D12_ROOT_PARAMETER_TYPE_CBV;
	_rootParameter.ShaderVisibility          = visibility;
	_rootParameter.Descriptor.ShaderRegister = shaderRegister;
	_rootParameter.Descriptor.RegisterSpace  = registerSpace;
}
inline void RootParameter::InitAsSRV(UINT shaderRegister, UINT registerSpace, D3D12_SHADER_VISIBILITY visibility)
{
	_rootParameter.ParameterType             = D3D12_ROOT_PARAMETER_TYPE_SRV;
	_rootParameter.ShaderVisibility          = visibility;
	_rootParameter.Descriptor.ShaderRegister = shaderRegister;
	_rootParameter.Descriptor.RegisterSpace  = registerSpace;
}
inline void RootParameter::InitAsUAV(UINT shaderRegister, UINT registerSpace, D3D12_SHADER_VISIBILITY visibility)
{
	_rootParameter.ParameterType             = D3D12_ROOT_PARAMETER_TYPE_UAV;
	_rootParameter.ShaderVisibility          = visibility;
	_rootParameter.Descriptor.ShaderRegister = shaderRegister;
	_rootParameter.Descriptor.RegisterSpace  = registerSpace;
}
inline void RootParameter::InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE type, UINT shaderRegister, UINT descriptorCount, UINT registerSpace, D3D12_SHADER_VISIBILITY visibility)
{
	InitAsDescriptorTable(1, visibility);
	SetTableRange(0, type, shaderRegister, descriptorCount, registerSpace);
}
inline void RootParameter::InitAsDescriptorTable(UINT descriptorRangeCount, D3D12_SHADER_VISIBILITY visibility)
{
	_rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	_rootParameter.ShaderVisibility = visibility;
	_rootParameter.DescriptorTable.NumDescriptorRanges = descriptorRangeCount;
	_rootParameter.DescriptorTable.pDescriptorRanges   = new D3D12_DESCRIPTOR_RANGE[descriptorRangeCount];
}
inline void RootParameter::SetTableRange(UINT rangeIndex, D3D12_DESCRIPTOR_RANGE_TYPE type, UINT shaderRegister, UINT descriptorCount, UINT registerSpace)
{
	D3D12_DESCRIPTOR_RANGE* range = const_cast<D3D12_DESCRIPTOR_RANGE*>(_rootParameter.DescriptorTable.pDescriptorRanges + rangeIndex);
	range->RangeType          = type;
	range->NumDescriptors     = descriptorCount;
	range->BaseShaderRegister = shaderRegister;
	range->RegisterSpace      = registerSpace;
	range->OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
}
#pragma endregion Root Parameter
#endif