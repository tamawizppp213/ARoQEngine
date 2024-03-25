//////////////////////////////////////////////////////////////////////////////////
//              Title:  ShaderConstantBuffer.hlsli
//            Content:  Constant Buffer
//             Author:  Toide Yutaro
//             Create:  2020_03_10
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_CONSTANT_BUFFER_3D_HLSLI
#define SHADER_CONSTANT_BUFFER_3D_HLSLI
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Define
//////////////////////////////////////////////////////////////////////////////////
cbuffer SceneConstants : register(b0)
{
    matrix View;
    matrix InverseView;
    matrix Projection;
    matrix InverseProjection;
    matrix ProjectionView;
    matrix InverseProjectionView;
    float3 EyePosition;
    float  Padding1;
    float2 RenderTargetSize;
    float2 InverseRenderTargetSize;
    float  NearZ;
    float  FarZ;
    float  TotalTime;
    float  DeltaTime;
}

cbuffer ObjectConstants : register(b1)
{
    matrix World;
    #ifdef _DEBUG
    float4 DebugColor;
    #endif
}

cbuffer MaterialConstants : register(b2)
{
    float4 Diffuse;
    float3 Specular;
    float  SpecularIntensity;
    float3 EmissiveColor;
    float  EmissiveIntensity;
    float3 Ambient;
    float  Metalness;
    float  Roughness;
    float3 Padding;
}

SamplerState SamplerLinearWrap : register(s0);
#endif