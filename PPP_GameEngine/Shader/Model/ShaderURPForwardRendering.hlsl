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

//////////////////////////////////////////////////////////////////////////////////
//                             Define
/////////////////////////////////////////////////////////////////////////////////
SamplerState SamplerLinearWrap : register(s0);
Texture2D    DiffuseMap        : register(t0);
Texture2D    SpecularMap       : register(t1);
Texture2D    NormalMap         : register(t2);

struct PSInput
{
	float4 Position    : SV_POSITION;
    float4 WorldNormal : NORMAL0;
	float4 ViewNormal  : NORMAL1;
    float2 UV          : TEXCOORD;
};

//////////////////////////////////////////////////////////////////////////////////
//                             Implement
/////////////////////////////////////////////////////////////////////////////////
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
	
	return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
	/*-------------------------------------------------------------------
	-        Set texture config
	---------------------------------------------------------------------*/
    const float4 albedo   = DiffuseMap.Sample(SamplerLinearWrap, input.UV);
	
	/*-------------------------------------------------------------------
	-        Set BRDF Surface Config
	---------------------------------------------------------------------*/
	BRDFSurface surface;
	surface.Diffuse           = albedo;
	surface.Specular          = Specular;
	surface.EmissiveColor     = EmissiveColor;
	surface.EmissiveIntensity = EmissiveIntensity;
	surface.Roughness         = Roughness;
	surface.Metalness         = Metalness;
	surface.Normal            = input.WorldNormal.xyz;
	
	float3 result = surface.EmissiveColor * surface.EmissiveIntensity / 100.0f;
    for (int i = 0; i < MAX_DIRECTIONAL_LIGHT; ++i)
    {
		
    }
	return albedo; 
}

#endif