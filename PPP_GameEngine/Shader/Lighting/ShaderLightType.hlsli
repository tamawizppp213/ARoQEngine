//////////////////////////////////////////////////////////////////////////////////
//              Title:  ShaderLightType.hlsli
//            Content:  Define Shader LightType
//             Author:  Toide Yutaro
//             Create:  
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_LIGHTTYPE_HLSLI
#define SHADER_LIGHTTYPE_HLSLI
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Define
//////////////////////////////////////////////////////////////////////////////////
// 32 bytes
struct PointLight
{
    float3 Position;
    float  Range;
    float3 Color;
    float  Brightness;
    
};

// 64 bytes
struct SpotLight
{
    float3 Position;
    float  Range;
    float3 Color;
    float  Brightness;
    float3 Direction;
    float  InnerConeAngle;
    float  OuterConeAngle;
    float3 Dummy;
};

// 32 bytes
struct DirectionalLight
{
    float3 Direction;
    float  Brightness;
    float3 Color;
    float  Padding;
};

#define NUM_DIRECTIONAL_LIGHT (4)
#define NUM_POINT_LIGHT (100)
#define NUM_SPOT_LIGHT  (20)

cbuffer SceneLights : register(b3)
{
    DirectionalLight DirectionalLights[NUM_DIRECTIONAL_LIGHT];
    PointLight       PointLights      [NUM_POINT_LIGHT];
    SpotLight        SpotLights       [NUM_SPOT_LIGHT];
}
#endif