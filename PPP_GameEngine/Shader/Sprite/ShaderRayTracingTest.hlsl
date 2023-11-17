/////////////////////////////////////////////////////////////////////////////////
//              Title:  ShaderRayTracingTest.hlsl
//            Content:  Draw RayTracing rect 
//             Author:  Toide Yutaro
//             Create:  2022_11_25
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_RAYTRACING_TEST_HLSL
#define SHADER_RAYTRACING_TEST_HLSL
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
//https://microsoft.github.io/DirectX-Specs/d3d/Raytracing.html
//////////////////////////////////////////////////////////////////////////////////
//                             Define
/////////////////////////////////////////////////////////////////////////////////
RaytracingAccelerationStructure Scene : register(t0);
RWTexture2D<float4> Output            : register(u0);
struct Payload
{
    float3 Color;
};
struct MyAttribute
{
    float2 Barys;
};

//////////////////////////////////////////////////////////////////////////////////
//                            Implement
//////////////////////////////////////////////////////////////////////////////////
/****************************************************************************
*				  		RayGeneration
*************************************************************************//**
*  @fn        void RayGeneration()
*  @brief     Shot ray 
*  @param[in] void
*****************************************************************************/
[shader("raygeneration")]
void RayGeneration()
{
    /*-------------------------------------------------------------------
	-             Shot ray from current pixel to -Z.  
	---------------------------------------------------------------------*/
    // Determine each render pixel location (width and height)
    const uint2 launchIndex = DispatchRaysIndex().xy;
    // Acquire width and height values
    const float2 dimensions = float2(DispatchRaysDimensions().xy);
    const float2 d = (launchIndex.xy + 0.5f) / dimensions.xy * 2.0f - 1.0f;

    // Set up ray descriptor
    RayDesc rayDescriptor;
    rayDescriptor.Origin    = float3(d.x, -d.y, 1);
    rayDescriptor.Direction = float3(0, 0, -1);      // shot ray -Z
    rayDescriptor.TMax      = 10000.0f;
    rayDescriptor.TMin      = 0.0f;
    // pay load initialize
    Payload payload;
    payload.Color = float3(0, 0, 0);
    
    //TraceRay(Scene, 0, 0xFF, 0, 1, 0, rayDescriptor, payload);
}

/****************************************************************************
*				  		Miss
*************************************************************************//**
*  @fn        void RayGeneration()
*  @brief     Shot ray 
*  @param[in] void
*****************************************************************************/
[shader("miss")]
void Miss(inout Payload payload)
{
    payload.Color = float3(0.4f, 0.8f, 0.9f);
}
/****************************************************************************
*				  		ClosestHit
*************************************************************************//**
*  @fn        void ClosestHit(inout Payload payload, MyAttribute attribute)
*  @brief     Closest object ray hit function
*  @param[in] void
*****************************************************************************/
[shader("closesthit")]
void ClosestHit(inout Payload payload, MyAttribute attribute)
{
    float3 color = 0;
    color.rg = attribute.Barys;
    color.b  = 1.0f - color.r - color.g;
    payload.Color = color;
}

float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}

#endif