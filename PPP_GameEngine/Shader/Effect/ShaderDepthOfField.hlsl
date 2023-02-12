//////////////////////////////////////////////////////////////////////////////////
//              Title:  ShaderDepthOfField.hlsl
//            Content:  Draw Depth Of Field Effect (compute shader)
//             Author:  Toide Yutaro
//             Create:  2022_05_01
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_DEPTH_OF_FIELD_HPP
#define SHADER_DEPTH_OF_FIELD_HPP
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Define
//////////////////////////////////////////////////////////////////////////////////
cbuffer BlurEffect : register(b0) // 0
{
    float2 TextureSize;
    float  Radius;
    float  Padding0;
}
cbuffer ClipSize : register(b1) // 1
{
    float  NearClip;
    float  FarClip;
    float2 Padding1;
}
// common
RWTexture2D<float4> DestCommon  : register(u0); // 5
RWTexture2D<float4> DestCommon2 : register(u1); // 6
// vertical blur
Texture2D<float4> SourceVertical : register(t0); // 2
// rhomboid blur
Texture2D<float4> SourceRhomboid0 : register(t0); // 2
Texture2D<float4> SourceRhomboid1 : register(t1); // 3
// final blur
Texture2D<float4> BlurTexture  : register(t0); // 2
Texture2D<float4> DepthTexture : register(t1); // 3
Texture2D<float4> SourceFinal  : register(t2); //4
SamplerState SamplerLinearWrap : register(s0); 
//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////
[numthreads(16, 16, 1)]
void VerticalBlur(uint3 id : SV_DispatchThreadID)
{
    float2 uv = float2((float2) (id.xy) / TextureSize);
    float blurStepLength = Radius / 4.0f;
    /*-------------------------------------------------------------------
	-        Calculate uvoffset step size
	---------------------------------------------------------------------*/
    float2 uvOffset = float2(0.0f, 1.0f / TextureSize.y);
    uvOffset       *= blurStepLength;
     /*-------------------------------------------------------------------
	-        Vertical blur (ë´ÇµçáÇÌÇπÇΩÇ∆Ç´Ç…0Å`1Ç…êÿÇÁÇÍÇƒÇ©ÇÁ1/4Ç≥ÇÍÇÈÉoÉO)
	---------------------------------------------------------------------*/
    float4 outputVertical = float4(0, 0, 0, 0);
    outputVertical += SourceVertical.SampleLevel(SamplerLinearWrap, uv + uvOffset * 1, 0);
    outputVertical += SourceVertical.SampleLevel(SamplerLinearWrap, uv + uvOffset * 2, 0);
    outputVertical += SourceVertical.SampleLevel(SamplerLinearWrap, uv + uvOffset * 3, 0);
    outputVertical += SourceVertical.SampleLevel(SamplerLinearWrap, uv + uvOffset * 4, 0);
    outputVertical /= 4.0f;
    DestCommon[id.xy] = outputVertical;
    /*-------------------------------------------------------------------
	-        Diagonal blur
	---------------------------------------------------------------------*/
    uvOffset.x = 0.86602f / TextureSize.x; // sin 60.
    uvOffset.y = -0.5f / TextureSize.y;
    uvOffset *= blurStepLength;
    
    float4 outputDiagonal = float4(0, 0, 0, 0);
    outputDiagonal += SourceVertical.SampleLevel(SamplerLinearWrap, uv + uvOffset * 1, 0);
    outputDiagonal += SourceVertical.SampleLevel(SamplerLinearWrap, uv + uvOffset * 2, 0);
    outputDiagonal += SourceVertical.SampleLevel(SamplerLinearWrap, uv + uvOffset * 3, 0);
    outputDiagonal += SourceVertical.SampleLevel(SamplerLinearWrap, uv + uvOffset * 4, 0);
    outputDiagonal += SourceVertical.SampleLevel(SamplerLinearWrap, uv, 0);
    outputDiagonal /= 5.0f;
    
    float4 outputTotal = DestCommon[id.xy] + outputDiagonal;
    outputTotal /= 2.0f;
    DestCommon2[id.xy] = outputTotal;
}

[numthreads(16,16,1)]
void RhomboidBlur(uint3 id : SV_DispatchThreadID)
{
    float2 uv            = float2((float2) (id.xy) / TextureSize);
    float blurStepLength = Radius / 4.0f;
    /*-------------------------------------------------------------------
	-        Calculate uvoffset step size
	---------------------------------------------------------------------*/
    float2 uvOffset;
    uvOffset.x = 0.86602f / TextureSize.x; // sin 60.
    uvOffset.y = -0.5f / TextureSize.y;
    uvOffset  *= blurStepLength;
     /*-------------------------------------------------------------------
	-        left down
	---------------------------------------------------------------------*/
    float4 output = float4(0, 0, 0, 0);
    output += SourceRhomboid0.SampleLevel(SamplerLinearWrap, uv + uvOffset * 1, 0);
    output += SourceRhomboid0.SampleLevel(SamplerLinearWrap, uv + uvOffset * 2, 0);
    output += SourceRhomboid0.SampleLevel(SamplerLinearWrap, uv + uvOffset * 3, 0);
    output += SourceRhomboid0.SampleLevel(SamplerLinearWrap, uv + uvOffset * 4, 0);
    /*-------------------------------------------------------------------
	-        right down
	---------------------------------------------------------------------*/
    uvOffset.x = -0.86602f / TextureSize.x * blurStepLength;
    output += SourceRhomboid1.SampleLevel(SamplerLinearWrap, uv + uvOffset * 1, 0);
    output += SourceRhomboid1.SampleLevel(SamplerLinearWrap, uv + uvOffset * 2, 0);
    output += SourceRhomboid1.SampleLevel(SamplerLinearWrap, uv + uvOffset * 3, 0);
    output += SourceRhomboid1.SampleLevel(SamplerLinearWrap, uv + uvOffset * 4, 0);
    output += SourceRhomboid1.SampleLevel(SamplerLinearWrap, uv + uvOffset * 0, 0);
    output /= 9.0f;
    DestCommon[id.xy] = output;
}

[numthreads(16,16,1)]
void FinalRender(uint3 id : SV_DispatchThreadID)
{
    float2 uv    = float2((float2) (id.xy) / TextureSize);
    float  depth = DepthTexture.SampleLevel(SamplerLinearWrap, uv, 0).x;
    float4 blur  = BlurTexture.SampleLevel(SamplerLinearWrap, uv, 0);
    
    [branch]
    if (depth < NearClip)
    {
        blur.a = 0.0f;
    }
    else
    {
        blur.a = min(1.0f, (depth - NearClip) / (FarClip - NearClip));
    }
    
    DestCommon[id.xy] = blur.a * blur + (1.0f - blur.a) * SourceFinal[id.xy];
}
#endif