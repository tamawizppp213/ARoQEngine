//////////////////////////////////////////////////////////////////////////////////
//              Title:  ShaderTonemapType.hlsl
//            Content:  HDR texture scale down to LDR
//             Author:  Toide Yutaro
//             Create:  2020_04_30
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_TONEMAP_TYPE_HLSLI
#define SHADER_TONEMAP_TYPE_HLSLI
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
cbuffer TonemapConfig : register(b0)
{
    int   TonemapType;
    int   ColorSpaceType;
    float BaseLuminance; // [nit]
    float MaxLuminance; // [nit]
};
//////////////////////////////////////////////////////////////////////////////////
//                             Define
/////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                            Implement
//////////////////////////////////////////////////////////////////////////////////
/****************************************************************************
*				  			Reinhard
****************************************************************************/
/* @fn        float3 Reinhard(float3 color)
*  @brief     Extended reinherd tonemapping
*  @param[in] float3 color
*  @return    float3
*****************************************************************************/
float3 Reinhard(float3 color)
{
    const float Lz = MaxLuminance / BaseLuminance;
    const float k = BaseLuminance * Lz / (BaseLuminance - Lz);
    return color.rgb * k / (color.rgb + float3(k,k,k));
}
/****************************************************************************
*				  			GT
****************************************************************************/
/* @fn        float3 GT(float3 color)
*  @brief     GT tonemapping
*  @param[in] float3 color
*  @return    float3
*****************************************************************************/
float3 GT(float3 color)
{
    const float k = MaxLuminance / BaseLuminance;
    
    // Control parameters
    const float P = 1.0f;  // monitor prominence 100 nit = 1.0
    const float a = 1.0f;  // contrast parameter
    const float m = 0.22f; // LERP start value
    const float l = 0.4f;  // LERP length
    const float c = 1.33f; // black tightness
    const float b = 0.0f;  // black tightness 2
    
    const float3 x = color.rgb;
    const float l0 = ((P - m) * l) / a;
    const float L0 = m - (m / a);
    const float L1 = m + ((1.0f - m) / a);
    
    const float S0 = m + l0;
    const float S1 = m + a * l0;
    const float C2 = (a * P) / (P - S1);
    const float CP = -C2 / P;
    
    const float3 w0 = 1.0f - smoothstep(0.0f, m, x);
    const float3 w2 = step(m + l0, x);
    const float3 w1 = 1.0f - w0 - w2;
    
    const float3 T = m * pow(x / m, c) + b;
    const float3 S = P - (P - S1) * exp(CP * (x - S0));
    const float3 L = m + a * (x - m);
    
    return T * w0 + L * w1 + S * w2;
}
#endif