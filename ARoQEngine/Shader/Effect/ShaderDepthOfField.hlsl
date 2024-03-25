//////////////////////////////////////////////////////////////////////////////////
//              Title:  ShaderDepthOfField.hlsl
//            Content:  Draw Depth Of Field Effect (compute shader)
//             Author:  Toide Yutaro
//             Create:  2023_02_19
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_DEPTH_OF_FIELD_HLSL
#define SHADER_DEPTH_OF_FIELD_HLSL
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Define
//////////////////////////////////////////////////////////////////////////////////
static const uint Thread = 16;

/****************************************************************************
*				  		BlurEffect
*************************************************************************//**
*  @class     BlurEffect
*  @brief     Blur effect setting
*****************************************************************************/
cbuffer BlurEffect : register(b0) // 0
{
    float2 TextureSize; // (in pixel size)
    float  Radius;      //  Blur radius. According to the value, the rhomboid blur size is getting bigger.
    float  Padding0;
}

/****************************************************************************
*				  		ClipSize
*************************************************************************//**
*  @class     ClipSize
*  @brief     Clip size
*****************************************************************************/
cbuffer ClipSize : register(b1) // 1
{
    float  NearClip;  // basically, this value is set 0.2.
    float  FarClip;   // basically, this value is set 0.8.
    float2 Padding1;
}

SamplerState SamplerLinearWrap : register(s0); 
//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////
/*-------------------------------------------------------------------
-         Vertical Blur Resource
---------------------------------------------------------------------*/
Texture2D<float4>   OriginalTexture    : register(t0); // 2
RWTexture2D<float4> VerticalBlurBuffer : register(u0); // 5
RWTexture2D<float4> DiagonalBlurBuffer : register(u1); // 6

/*-------------------------------------------------------------------
-         Vertical Blur Render Function
---------------------------------------------------------------------*/
[numthreads(Thread, Thread, 1)]
void VerticalBlur(uint3 id : SV_DispatchThreadID)
{
    const float2 uv = float2((float2) (id.xy) / TextureSize);
 
    const uint  stepCount      = 4; // êÆêîÇæÇØÇ«èúéZëŒçÙÇ≈float
    const float blurStepLength = (float)Radius / stepCount; 
    
    /*-------------------------------------------------------------------
	-        Calculate uvoffset step size
	---------------------------------------------------------------------*/
    float2 uvOffset = float2(0.0f, 1.0f / TextureSize.y);
    uvOffset       *= blurStepLength; // uv displacement offset to proceed by radius of hexagon (vertical direction.)
    
     /*-------------------------------------------------------------------
	-        Vertical blur 
	---------------------------------------------------------------------*/
    float4 outputVertical = float4(0, 0, 0, 0);
    
    [unroll]
    for (uint i = 0; i < stepCount; ++i)
    {
        outputVertical += OriginalTexture.SampleLevel(SamplerLinearWrap, uv + uvOffset * i + 1, 0);
    }
    outputVertical /= 4.0f; // averaging 

    VerticalBlurBuffer[id.xy] = outputVertical;
    
    /*-------------------------------------------------------------------
	-        Diagonal blur
	---------------------------------------------------------------------*/
    uvOffset.x = 0.86602f / TextureSize.x; // sin 60.
    uvOffset.y = -0.5f / TextureSize.y;
    uvOffset *= blurStepLength;
    
    float4 outputDiagonal = float4(0, 0, 0, 0);
    
    [unroll]
    for(uint j = 0; j <= 4; ++j)
    {
        outputDiagonal += OriginalTexture.SampleLevel(SamplerLinearWrap, uv + uvOffset * j, 0);
    }
    outputDiagonal /= 5.0f;
    
    float4 outputTotal = outputVertical + outputDiagonal;
    outputTotal /= 2.0f;
    DiagonalBlurBuffer[id.xy] = outputTotal;
}


/*-------------------------------------------------------------------
-         Rhomboid Blur Resource
---------------------------------------------------------------------*/
Texture2D<float4> VerticalBlurTexture : register(t0); // 2
Texture2D<float4> DiagonalBlurTexture : register(t1); // 3
RWTexture2D<float4> RhomboidBlurBuffer : register(u0); //5

/*-------------------------------------------------------------------
-         Rhomboid Blur Render Function
---------------------------------------------------------------------*/
[numthreads(Thread,Thread,1)]
void RhomboidBlur(uint3 id : SV_DispatchThreadID)
{
    const float2 uv            = float2((float2) (id.xy) / TextureSize);
    const uint  stepCount      = 4;
    const float blurStepLength = (float) Radius / stepCount;
    
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
    
    [unroll]
    for (uint i = 1; i <= stepCount; ++i)
    {
        output += VerticalBlurTexture.SampleLevel(SamplerLinearWrap, uv + uvOffset * i, 0);
    }
    
    /*-------------------------------------------------------------------
	-        right down
	---------------------------------------------------------------------*/
    uvOffset.x = -0.86602f / TextureSize.x * blurStepLength;
    
    [unroll]
    for (uint j = 0; j <= stepCount; ++j)
    {
        output += VerticalBlurTexture.SampleLevel(SamplerLinearWrap, uv + uvOffset * j, 0);
    }
    
    output /= 9.0f;
    RhomboidBlurBuffer[id.xy] = output;
}

/*-------------------------------------------------------------------
-         Final Blur Render Resource
---------------------------------------------------------------------*/
Texture2D<float4> RhomboidBlurTexture : register(t1); // 3
Texture2D<float4> DepthTexture        : register(t2); // 4
RWTexture2D<float4> FinalBuffer      : register(u0);

/*-------------------------------------------------------------------
-         Final Blur Render Function
---------------------------------------------------------------------*/
[numthreads(Thread, Thread,1)]
void FinalRender(uint3 id : SV_DispatchThreadID)
{
    const float2 uv    = float2((float2) (id.xy) / TextureSize);
    const float  depth = DepthTexture       .SampleLevel(SamplerLinearWrap, uv, 0).x;
    float4 blur  = RhomboidBlurTexture.SampleLevel(SamplerLinearWrap, uv, 0);
    
    [branch]
    if (depth < NearClip)
    {
        blur.a = 0.0f;
    }
    else
    {
        blur.a = min(1.0f, (depth - NearClip) / (FarClip - NearClip));
    }
    
    FinalBuffer[id.xy] = blur.a * blur + (1.0f - blur.a) * OriginalTexture[id.xy];
}
#endif