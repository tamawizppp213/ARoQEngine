//////////////////////////////////////////////////////////////////////////////////
//              Title:  ShaderURPForwardRendering.hlsli
//            Content:  Tile-based Forward Rendering
//             Author:  Toide Yutaro
//             Create:  2023_03_03
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_URP_FORWARD_RENDERING_HLSL
#define SHADER_URP_FORWARD_RENDERING_HLSL
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Core/ShaderVertexType.hlsli"
#include "../Core/ShaderConstantBuffer3D.hlsli"
#include "../Lighting/ShaderBRDF.hlsli"
#include "../Lighting/ShaderLightType.hlsli"
#include "../Lighting/ShaderShadow.hlsli"

//////////////////////////////////////////////////////////////////////////////////
//                             Define
/////////////////////////////////////////////////////////////////////////////////
Texture2D    DiffuseMap        : register(t0);
Texture2D    SpecularMap       : register(t1);
Texture2D    NormalMap         : register(t2); 

struct PSInput
{
	float4 Position    : SV_POSITION;
    float4 WorldNormal : NORMAL0;
	float4 ViewNormal  : NORMAL1;
    float2 UV          : TEXCOORD;
    //float4 PositionInLVP[SHADOW_MAP_COUNT] : TEXCOORD1; // pixel position in the light view screen space.
};

//////////////////////////////////////////////////////////////////////////////////
//                             Implement
/////////////////////////////////////////////////////////////////////////////////
float3 Calculate_Directional_Light_Illumination(in const DirectionalLight light, in BRDFSurface surface, const in float3 toEye);

PSInput VSMain( VSInputSkinVertex vertexIn)
{
	PSInput result;
	
	/*-------------------------------------------------------------------
	-        transform to world space 
	---------------------------------------------------------------------*/
	float4 positionWorld = mul(World, vertexIn.Position);
	result.Position      = mul(ProjectionView, positionWorld);
	result.WorldNormal   = mul(World, vertexIn.Normal);
	result.ViewNormal    = mul(View, result.WorldNormal);
	result.UV            = vertexIn.UV;
	
	/*-------------------------------------------------------------------
	-        Calculate light view screen space
	---------------------------------------------------------------------*/
    //for (int i = 0; i < SHADOW_MAP_COUNT; ++i)
    //{
    //    result.PositionInLVP[i] = mul(LVPC[i], positionWorld);
    //}
	
	return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    const float3 toEye = normalize(EyePosition - input.Position.xyz);
	
	/*-------------------------------------------------------------------
	-        Set texture config
	---------------------------------------------------------------------*/
    float4 albedo   = DiffuseMap.Sample(SamplerLinearWrap, input.UV);
	
	/*-------------------------------------------------------------------
	-        Set BRDF Surface Config
	---------------------------------------------------------------------*/
	BRDFSurface surface;
	surface.BaseColor         = albedo.rgb * Diffuse.rgb;
	surface.EmissiveColor     = EmissiveColor;
	surface.EmissiveIntensity = EmissiveIntensity;
	surface.Roughness         = Roughness;
    surface.Metalness         = Metalness;
	surface.Normal            = input.WorldNormal.xyz;
	
	float3 result = surface.EmissiveColor * surface.EmissiveIntensity / 100.0f;
    for (int i = 0; i < MAX_DIRECTIONAL_LIGHT; ++i)
    {
		if(!DirectionalLights[i].IsUse){ continue; }
		
		result += Calculate_Directional_Light_Illumination(DirectionalLights[i], surface, toEye);
    }
	
    result += Diffuse.rgb * albedo.rgb * Ambient;
    return float4(result, albedo.a);
}

/****************************************************************************
*				  		Calculate_Directional_Light_Illumination
****************************************************************************/
/* @class     float3 Calculate_Directional_Light_Illumination(in const PointLight light, in BRDFSurface surface, const in float3 surfacePosition, const in float3 toEye )
*  @brief     Calculate directional light illumination
*  @param[in] in const Directional;Light light
*  @param[in] in BRDFSurface surface
*  @param[in] const in float3 surfacePosition 
*  @param[in] const in float3 toEye
*****************************************************************************/
float3 Calculate_Directional_Light_Illumination(in const DirectionalLight light, in BRDFSurface surface, const in float3 toEye)
{
    const float3 toLight       = normalize(-light.Direction); // light vector‚Í•\–Ê‚©‚ç—£‚ê‚é•ûŒü‚Å‚ ‚é‚±‚Æ‚É’ˆÓ‚·‚é.
    const float3 radiance      = light.Brightness;
    const float normalDotLight = saturate(dot(surface.Normal, toLight));
	
    DirectLight directLight = AccumulateSurfaceEnergy(surface, toLight, toEye);
    float3 result = (directLight.Diffuse + directLight.Specular) * radiance * light.Color;
    return result;
}
#endif