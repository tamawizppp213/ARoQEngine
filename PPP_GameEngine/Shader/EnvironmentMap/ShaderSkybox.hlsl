//////////////////////////////////////////////////////////////////////////////////
//              Title:  ShaderSkybox.hlsli
//            Content:  Create Skybox
//             Author:  Toide Yutaro
//             Create:  2020_04_25
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_SKYBOX_HLSL
#define SHADER_SKYBOX_HLSL
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Core/ShaderConstantBuffer3D.hlsli"
#include "../Core/ShaderColor.hlsli"

//////////////////////////////////////////////////////////////////////////////////
//                             Define
//////////////////////////////////////////////////////////////////////////////////
struct VertexIn
{
    float3 Position : POSITION;
    float3 Normal   : NORMAL;
    float2 UV       : TEXCOORD;
    float4 Color    : COLOR;
};
struct VertexOut
{
    float4 Position      : SV_Position;
    float3 LookDirection : POSITION;
};
TextureCube  CubeMap : register(t0);

//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////
VertexOut VSMain(VertexIn vertexIn)
{
    VertexOut result;
    /*-------------------------------------------------------------------
	-         Use local vertex position as cubemap lookup vector
	---------------------------------------------------------------------*/
    result.LookDirection = vertexIn.Position.xyz;
     /*-------------------------------------------------------------------
	-        transform to world space 
	---------------------------------------------------------------------*/
    float4 positionWorld = mul(World, float4(vertexIn.Position.xyz, 1.0f));
    positionWorld.xyz   += EyePosition;
    /*-------------------------------------------------------------------
	-        Skybox always on far plane
	---------------------------------------------------------------------*/
    result.Position = mul(ProjectionView, positionWorld).xyww;
    return result;
}

float4 PSMain(VertexOut input) : SV_TARGET
{
    float4 result = float4(CubeMap.Sample(SamplerLinearWrap, input.LookDirection));
    //result.rgb    = SRGBToLinear(result.rgb); // linear‹óŠÔ‚É’¼‚µ‚Ä‚¢‚é
    return result;
}
#endif