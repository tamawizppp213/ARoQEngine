//////////////////////////////////////////////////////////////////////////////////
//              @file   GPUShaderState.cpp
///             @brief  Shader State 
///             @author Toide Yutaro
///             @date   2022_07_02
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUShaderState.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIDevice.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::core;
using namespace gu;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
gu::tstring GPUShaderState::GetShaderTypeName(core::ShaderType shaderType)
{
	switch (shaderType)
	{
		case ShaderType::Vertex       : return SP("vs");
		case ShaderType::Pixel        : return SP("ps");
		case ShaderType::Compute      : return SP("cs");
		case ShaderType::Mesh         : return SP("ms");
		case ShaderType::Amplification: return SP("as");
		case ShaderType::Domain       : return SP("ds");
		case ShaderType::Geometry     : return SP("gs");
		case ShaderType::Hull         : return SP("hs");
		case ShaderType::RayGeneration:
		case ShaderType::Intersection:
		case ShaderType::AnyHit:
		case ShaderType::ClosestHit:
		case ShaderType::Miss:         return SP("lib");
		default:
			throw "not supported shader type";
	}
}


gu::tstring GPUShaderState::Format(float version)
{
	tstring number = gu::tstring::FromNumber(version);
	number = number.Left(number.Find(SP(".")) + 2); // +2�Ȃ̂�, �������� + index�̂���

	return number.SubString(0, 1) + SP("_") + number.SubString(2, 1); // main version 5, 6, sub version .1, .2
}

/*!**********************************************************************
*  @brief     �L���ȃV�F�[�_�[�ݒ肩�ǂ�����Ԃ��܂�
*  @param[in] void
*  @return    bool
*************************************************************************/
bool GPUShaderState::IsValidShaderType() const noexcept
{
	// Mesh Shader���T�|�[�g����Ă��Ȃ��ꍇ��, Mesh Shader���g�p�ł��܂���
	if (_shaderType == ShaderType::Mesh && !_device->IsSupportedMeshShading())
	{
		return false;
	}

	// Ray Tracing Shader���T�|�[�g����Ă��Ȃ��ꍇ��, Ray Tracing Shader���g�p�ł��܂���
	if (IsRayTracingShader() && !_device->IsSupportedDxr())
	{
		return false;
	}
	
	// ����ȊO�̏ꍇ��, �L���ȃV�F�[�_�[�ݒ�ł�
	return true;
}

/*!**********************************************************************
*  @brief     �ݒ肳�ꂽ�V�F�[�_�[��RayTracing�Ŏg�p�\�ȃV�F�[�_�[��
*  @param[in] void
*  @return    bool
*************************************************************************/
bool GPUShaderState::IsRayTracingShader() const
{
	switch(_shaderType)
	{
		case ShaderType::RayGeneration:
		case ShaderType::Intersection:
		case ShaderType::AnyHit:
		case ShaderType::ClosestHit:
		case ShaderType::Miss:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}