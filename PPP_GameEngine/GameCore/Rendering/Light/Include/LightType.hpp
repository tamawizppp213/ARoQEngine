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
	enum class LightType
	{
		Directional,
		Point,
		Spot,
		CountOf
	};

	struct LightData{};

	struct DirectionalLightData : public LightData
	{
		hlsl::float3 Direction  = hlsl::float3(0, 0, 0);
		hlsl::float1 Brightness = 1.0f;
		hlsl::float3 Color      = hlsl::float3(1, 1, 1);
		int          IsUse      = 0.0f;
	};

	struct PointLightData : public LightData
	{
		hlsl::float3 Position   = hlsl::float3(0, 0, 0);
		hlsl::float1 Range      = 1.0f;
		hlsl::float3 Color      = hlsl::float3(1, 1, 1);
		hlsl::float1 Brightness = 1.0f;
		int          IsUse      = false;
		hlsl::float3 Dummy      = hlsl::float3(0, 0, 0);
	};

	struct SpotLightData : public LightData
	{
		hlsl::float3 Position       = hlsl::float3(0, 0, 0);
		hlsl::float1 Range          = 1.0f;
		hlsl::float3 Color          = hlsl::float3(0, 0, 0);
		hlsl::float1 Brightness     = 1.0f;
		hlsl::float3 Direction      = hlsl::float3(0, 0, 0);
		hlsl::float1 InnerConeAngle = gm::GM_PI / 6.0f;
		hlsl::float1 OuterConeAngle = gm::GM_PI / 4.0f;
		int          IsUse          = false;
		hlsl::float2 Dummy          = hlsl::float2(0, 0);
	};

	struct RectLightData : public LightData
	{
		hlsl::float3 Position   = hlsl::float3(0, 0, 0);
		hlsl::float1 Range      = 1.0f;
		hlsl::float3 Color      = hlsl::float3(0, 0, 0);
		hlsl::float1 Brightness = 1.0f;
		hlsl::float2 RectSize   = (1.0f, 1.0f);
		hlsl::float2 Padding    = (0.0f, 0.0f);
	};
}
#endif