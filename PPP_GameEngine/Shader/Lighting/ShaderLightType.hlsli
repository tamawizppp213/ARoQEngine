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
// 48 bytes
struct PointLight
{
    float3 Position;
    float  Range;
    float3 Color;
    float  Brightness;
    int    IsUse;
    float3 Dummy;
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
    int    IsUse;
    float2 Dummy;
};

// 32 bytes
struct DirectionalLight
{
    float3 Direction;
    float  Brightness;
    float3 Color;
    int    IsUse;
};

#ifndef MAX_DIRECTIONAL_LIGHT // default count
#define MAX_DIRECTIONAL_LIGHT (4)
#endif

#ifndef MAX_POINT_LIGHT // default count
#define MAX_POINT_LIGHT (1000)
#endif

#ifndef MAX_SPOT_LIGHT // default count
#define MAX_SPOT_LIGHT  (100)
#endif

cbuffer ScenePointLight : register(b3)
{
    PointLight PointLights [MAX_POINT_LIGHT];
}

cbuffer SceneSpotLight : register(b4)
{
    SpotLight SpotLights[MAX_SPOT_LIGHT];
}

cbuffer SceneDirectionalLight : register(b5)
{
    DirectionalLight DirectionalLights[MAX_DIRECTIONAL_LIGHT];
}
#endif