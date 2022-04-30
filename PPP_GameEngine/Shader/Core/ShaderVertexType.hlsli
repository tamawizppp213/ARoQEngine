//////////////////////////////////////////////////////////////////////////////////
//              Title:  BasicShaderStruct.hlsli
//            Content:  Basic Vertex 
//             Author:  Toide Yutaro
//             Create:  2020_12_08
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_VERTEX_TYPE_HLSLI
#define SHADER_VERTEX_TYPE_HLSLI

struct VSInputPosition
{
    float4 Position : POSITION;
};

struct VSInputPositionColor
{
    float4 Position : POSITION;
    float4 Color    : COLOR;
};

struct VSInputPositionTexture
{
    float4 Position : POSITION;
    float2 UV : TEXCOORD0;
};

struct VSInputPositionColorTexture
{
    float4 Position : POSITION;
    float4 Color    : COLOR;
    float2 UV       : TEXCOORD0;
};

struct VSInputPositionNormal
{
    float4 Position : POSITION;
    float4 Normal   : NORMAL;
    
};

struct VSInputPositionNormalColor
{
    float4 Position : POSITION;
    float4 Normal   : NORMAL;
    float4 Color    : COLOR;
};

struct VSInputPositionNormalTexture
{
    float4 Position : POSITION;
    float4 Normal   : NORMAL;
    float2 UV       : TEXCOORD0;
};

struct VSInputPositionNormalColorTexture
{
    float4 Position : POSITION;
    float4 Normal   : NORMAL;
    float4 Color    : COLOR;
    float2 UV       : TEXCOORD0;
};

struct VSInputPositionDualTexture
{
    float4 Position : POSITION;
    float2 UV0      : TEXCOORD0;
    float2 UV1      : TEXCOORD1;
};

// VSOutput
struct VSOutputPosition
{
    float4 Position : SV_POSITION;
};

struct VSOutputPositionColor
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
};

struct VSOutputPositionTexture
{
    float4 Position : SV_POSITION;
    float2 UV : TEXCOORD0;
};

struct VSOutputPositionColorTexture
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD0;
};

struct VSOutputPositionNormal
{
    float4 Position : SV_POSITION;
    float4 Normal : NORMAL;
    
};

struct VSOutputPositionNormalColor
{
    float4 Position : SV_POSITION;
    float4 Normal : NORMAL;
    float4 Color : COLOR;
};

struct VSOutputPositionNormalTexture
{
    float4 Position : SV_POSITION;
    float4 Normal   : NORMAL;
    float2 UV       : TEXCOORD0;
};

struct VSOutputPositionNormalColorTexture
{
    float4 Position : SV_POSITION;
    float4 Normal : NORMAL;
    float4 Color : COLOR;
    float2 UV : TEXCOORD0;
};

struct VSOutputPositionDualTexture
{
    float4 Position : SV_POSITION;
    float2 UV0 : TEXCOORD0;
    float2 UV1 : TEXCOORD1;
};
#endif