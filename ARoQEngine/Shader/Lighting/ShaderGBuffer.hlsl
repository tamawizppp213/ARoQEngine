//////////////////////////////////////////////////////////////////////////////////
//              Title:  ShaderGBuffer.hlsl
//            Content:  GBuffer
//             Author:  Toide Yutaro
//             Create:  2023_02_07
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_GBUFFER_HLSL
#define SHADER_GBUFFER_HLSL
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Core/ShaderVertexType.hlsli"
#include "../Core/ShaderConstantBuffer3D.hlsli"

//////////////////////////////////////////////////////////////////////////////////
//                             Define
/////////////////////////////////////////////////////////////////////////////////
Texture2D    DiffuseMap        : register(t0);
Texture2D    SpecularMap       : register(t1);
Texture2D    NormalMap         : register(t2);

struct PSIn
{
	float4 Position     : SV_POSITION;
	float3 WorldNormal  : NORMAL0;
	float2 UV           : TEXCOORD;
};

struct PSOut
{
	float4 Albedo      : SV_Target0;
	float4 WorldNormal : SV_Target1;
};
//////////////////////////////////////////////////////////////////////////////////
//                            Implement
//////////////////////////////////////////////////////////////////////////////////
PSIn VSMain( VSInputSkinVertex vertexIn ) 
{
	PSIn result;
	
	/*-------------------------------------------------------------------
	-        transform to world space 
	---------------------------------------------------------------------*/
	float4 positionWorld = mul(World, vertexIn.Position);
	result.Position      = mul(ProjectionView, positionWorld);
	result.WorldNormal   = normalize(mul(World, vertexIn.Normal).xyz);
	result.UV            = vertexIn.UV;
	return result;
}

PSOut PSMain(PSIn input)
{
	PSOut result;
	
	/*-------------------------------------------------------------------
	-        Set texture config
	---------------------------------------------------------------------*/
	const float4 albedo = DiffuseMap.Sample(SamplerLinearWrap, input.UV);
    const float4 normal = float4(float3((input.WorldNormal * 0.5f) + 0.5f).xyz, 1.0f);
	
	/*-------------------------------------------------------------------
	-        Set result
	---------------------------------------------------------------------*/
	result.Albedo      = albedo;
	result.WorldNormal = normal;
	return result;
}
#endif