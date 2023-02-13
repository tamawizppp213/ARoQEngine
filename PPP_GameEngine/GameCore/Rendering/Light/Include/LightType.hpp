//////////////////////////////////////////////////////////////////////////////////
///             @file   LightType.hpp
///             @brief  LightType
///             @author Toide Yutaro
///             @date   2022_04_16
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef LIGHTING_TYPE_HPP
#define LIGHTING_TYPE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/HLSLUtility.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Light
//////////////////////////////////////////////////////////////////////////////////
namespace gc::rendering
{
	struct PointLightData
	{
		hlsl::float3 Position = hlsl::float3(0, 0, 0);
		hlsl::float1 Range    = 1.0f;
		hlsl::float3 Color      = hlsl::float3(1, 1, 1);
		hlsl::float1 Brightness = 1.0f;
	};

	struct SpotLightData
	{
		hlsl::float3 Position       = hlsl::float3(0, 0, 0);
		hlsl::float1 Range          = 1.0f;
		hlsl::float3 Color          = hlsl::float3(0, 0, 0);
		hlsl::float1 Brightness     = 1.0f;
		hlsl::float3 Direction      = hlsl::float3(0, 0, 0);
		hlsl::float1 InnerConeAngle = gm::GM_PI / 6.0f;
		hlsl::float1 OuterConeAngle = gm::GM_PI / 4.0f;
	};

	struct DirectionalLightData
	{
		hlsl::float3 Direction  = hlsl::float3(0, 0, 0);
		hlsl::float1 Brightness = 1.0f;
		hlsl::float3 Color      = hlsl::float3(1, 1, 1);
		hlsl::float1 Padding    = 1.0f;
	};
}
#endif