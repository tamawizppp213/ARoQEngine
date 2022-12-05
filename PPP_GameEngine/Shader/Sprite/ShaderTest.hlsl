//////////////////////////////////////////////////////////////////////////////////
//              Title:  ShaderDefault.hlsl
//            Content:  Create Skybox
//             Author:  Toide Yutaro
//             Create:  2020_04_24
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_TEST_HLSL
#define SHADER_TEST_HLSL
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Core/ShaderVertexType.hlsli"

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
    result.Position = vertexIn.Position;
    result.UV       = vertexIn.UV;
    result.Normal   = vertexIn.Normal;
    result.Color    = vertexIn.Color;
    
    return result;
}

float4 PSTest(VertexOut input) : SV_Target
{
    return input.Color;
}
#endif