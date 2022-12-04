//////////////////////////////////////////////////////////////////////////////////
//              Title:  BasicShaderStruct.hlsli
//            Content:  Basic Vertex 
//             Author:  Toide Yutaro
//             Create:  2020_12_08
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_VERTEX_TYPE_HLSLI
#define SHADER_VERTEX_TYPE_HLSLI


struct VSInputVertex
{
    float4 Position : POSITION;
    float4 Normal   : NORMAL;
    float4 Color    : COLOR;
    float2 UV       : TEXCOORD;
};



// VSOutput
struct VSOutputVertex
{
    float4 Position : SV_POSITION;
    float4 Normal   : NORMAL;
    float4 Color    : COLOR;
    float2 UV       : TEXCOORD;
};

#endif