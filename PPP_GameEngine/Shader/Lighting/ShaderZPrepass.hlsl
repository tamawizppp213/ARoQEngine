//////////////////////////////////////////////////////////////////////////////////
//              Title:  ShaderZPrepass.hlsl
//            Content:  ZPrepass
//             Author:  Toide Yutaro
//             Create:  2023_01_16
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_ZPREPASS_HLSL
#define SHADER_ZPREPASS_HLSL
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Core/ShaderVertexType.hlsli"
#include "../Core/ShaderConstantBuffer3D.hlsli"

//////////////////////////////////////////////////////////////////////////////////
//                             Define
/////////////////////////////////////////////////////////////////////////////////
cbuffer ObjectConstants : register(b1)
{
    matrix World;
}

struct PSIn
{
    float4 Position : SV_POSITION;
    float  Depth     : TEXCOORD0;
};

//////////////////////////////////////////////////////////////////////////////////
//                            Implement
//////////////////////////////////////////////////////////////////////////////////
PSIn VSMain( VSInputSkinVertex vertexIn)
{
	PSIn result;
	
	/*-------------------------------------------------------------------
	-        transform to world space 
	---------------------------------------------------------------------*/
	float4 positionWorld = mul(World, vertexIn.Position);
	result.Position      = mul(ProjectionView, positionWorld);
	result.Depth    = result.Position.z / result.Position.w; // normalize
	return result;
}

float4 PSMain(PSIn psIn) : SV_TARGET
{
	return float4(psIn.Depth, psIn.Depth, psIn.Depth, 1.0f);
}
#endif