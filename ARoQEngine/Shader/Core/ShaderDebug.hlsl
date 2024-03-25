//////////////////////////////////////////////////////////////////////////////////
//              Title:  ShaderDefault.hlsl
//            Content:  Create Skybox
//             Author:  Toide Yutaro
//             Create:  2020_04_24
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_DEBUG_HLSL
#define SHADER_DEBUG_HLSL
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Core/ShaderVertexType.hlsli"
#include "../Core/ShaderConstantBuffer3D.hlsli"

//////////////////////////////////////////////////////////////////////////////////
//                             Define
/////////////////////////////////////////////////////////////////////////////////
struct PSIn
{
    float4 Position : SV_POSITION;
    #ifdef _DEBUG
    float4 Color : COLOR;
    #endif
};
//////////////////////////////////////////////////////////////////////////////////
//                            Implement
//////////////////////////////////////////////////////////////////////////////////
PSIn VSMain(VSInputSkinVertex vertexIn)
{
    PSIn result;
	
	/*-------------------------------------------------------------------
	-        transform to world space 
	---------------------------------------------------------------------*/
    float4 positionWorld = mul(World, vertexIn.Position);
    result.Position      = mul(ProjectionView, positionWorld);
    
    // color
    #ifdef _DEBUG
    result.Color = DebugColor;
    #endif
    return result;
}

float4 PSMain(PSIn psIn) : SV_TARGET
{
    #ifdef _DEBUG
    return psIn.Color;
    #else
    return float4(1,1,1,1); // Žg—p‚Í‚µ‚Ü‚¹‚ñ
    #endif
}
#endif