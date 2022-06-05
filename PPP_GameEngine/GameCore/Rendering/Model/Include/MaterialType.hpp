//////////////////////////////////////////////////////////////////////////////////
///             @file   MaterialType.hpp
///             @brief  Material type
///             @author Toide Yutaro
///             @date   2022_06_06
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef MATERIAL_TYPE_HPP
#define MATERIAL_TYPE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Math/Include/GMVector.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
struct PBRMaterial
{
	gm::Float4 Diffuse;
	gm::Float3 Specular;
	float      SpecularIntensity;
	gm::Float3 EmissiveColor;
	float      EmissiveIntensity;
	gm::Float3 Ambient;
	float      Metalness;
	float      Roughness;
	gm::Float3 Padding;
	
	PBRMaterial()
	{
		this->Diffuse           = gm::Float4(1, 1, 1, 1);
		this->Specular          = gm::Float3(1, 1, 1);
		this->SpecularIntensity = 1.0f;
		this->Roughness         = 0.5f;
		this->EmissiveColor     = gm::Float3(1, 1, 1);
		this->EmissiveIntensity = 0.0f;
		this->Ambient           = gm::Float3(0.5f, 0.5f, 0.5f);
		this->Metalness         = 0.0f;
		this->Padding           = 0.0f;
	}
	~PBRMaterial()                             = default;
	PBRMaterial(const PBRMaterial&)            = default;
	PBRMaterial& operator=(const PBRMaterial&) = default;
	PBRMaterial(PBRMaterial&&)                 = default;
	PBRMaterial& operator=(PBRMaterial&&)      = default;
};
#endif