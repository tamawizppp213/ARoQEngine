//////////////////////////////////////////////////////////////////////////////////
//              Title:  BasicShaderStruct.hlsli
//            Content:  Basic Vertex 
//             Author:  Toide Yutaro
//             Create:  2020_12_08
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_VERTEX_TYPE_HLSLI
#define SHADER_VERTEX_TYPE_HLSLI


struct VSInputPositionNormalColorTexture
{
    float4 Position : POSITION0;
    float4 Normal   : NORMAL0;
    float4 Color    : COLOR0;
    float2 UV       : TEXCOORD0;
};



// VSOutput
struct VSOutputPositionNormalColorTexture
{
    float4 Position : SV_POSITION;
    float4 Normal : NORMAL0;
    float4 Color : COLOR0;
    float2 UV : TEXCOORD0;
};

#endif