//////////////////////////////////////////////////////////////////////////////////
//              Title:  ShaderLightCulling.hlsl
//            Content:  Shader light culling (compute shader)
//             Author:  Toide Yutaro 
//             Create:  2021_08_06
//////////////////////////////////////////////////////////////////////////////////
// reference: https://github.com/shoeisha-books/hlsl-grimoire-sample/blob/main/Sample_16_04/Sample_16_04/Assets/shader/lightCulling.fx
// reference: https://mobile.sites.google.com/site/monshonosuana/directx%E3%81%AE%E8%A9%B1/directx%E3%81%AE%E8%A9%B1-%E7%AC%AC125%E5%9B%9E

#ifndef SHADER_LIGHT_CULLING_HPP
#define SHADER_LIGHT_CULLING_HPP
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Lighting/ShaderLightType.hlsli"
#include "../Core/ShaderConstantBuffer3D.hlsli"

//////////////////////////////////////////////////////////////////////////////////
//                             Define
//////////////////////////////////////////////////////////////////////////////////
#define TILE_WIDTH  (16)
#define TILE_HEIGHT (16)
#define TILE_SIZE (TILE_WIDTH * TILE_HEIGHT)

Texture2D<float> DepthTexture             : register(t0);
RWStructuredBuffer<uint> LightIndices     : register(u0);
RWStructuredBuffer<uint> SpotLightIndices : register(u1);
//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////
[numthreads(TILE_WIDTH, TILE_HEIGHT, 1)]
void LightCulling( uint3 groupID          : SV_GroupID,
                   uint3 dispatchThreadID : SV_DispatchThreadID,
                   uint3 groupThreadID    : SV_GroupThreadID)
{
    
}
#endif