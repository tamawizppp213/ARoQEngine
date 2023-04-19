//////////////////////////////////////////////////////////////////////////////////
//              Title:  ShaderDefault.hlsl
//            Content:  Create Skybox
//             Author:  Toide Yutaro
//             Create:  2020_04_24
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_SHADOW_MAP_HLSL
#define SHADER_SHADOW_MAP_HLSL
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Core/ShaderVertexType.hlsli"
#include "../Core/ShaderConstantBuffer3D.hlsli"
#define VertexIn VSInputVertex
#define VertexOut VSOutputVertex
//////////////////////////////////////////////////////////////////////////////////
//                             Define
/////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                            Implement
//////////////////////////////////////////////////////////////////////////////////
VertexOut VSMain(VertexIn vertexIn)
{
    VertexOut result;
    
    /*-------------------------------------------------------------------
	-        transform to world space 
	---------------------------------------------------------------------*/
    float4 positionWorld = mul(World, vertexIn.Position);
    result.Position      = mul(ProjectionView, positionWorld);
    result.UV            = vertexIn.UV;
    result.Normal        = vertexIn.Normal;
    result.Color         = vertexIn.Color;
    
    return result;
}

float4 PSMain(VertexOut input) : SV_Target
{
    // input.Position.z == directional light depth
    // input.Position ^2 is used by the VSM.
    return float4(0.5f, 0.5f, 0.5f, 1.0f);
    //return float4(input.Position.z, input.Position.z * input.Position.z, 0.0f, 1.0f);
}

#endif