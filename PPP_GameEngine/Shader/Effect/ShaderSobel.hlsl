//////////////////////////////////////////////////////////////////////////////////
//              Title:  ShaderSobel.hlsl
//            Content:  Detection of outline
//             Author:  Toide Yutaro
//             Create:  2023_03_26
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_SOBEL_HLSL
#define SHADER_SOBEL_HLSL
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Define
/////////////////////////////////////////////////////////////////////////////////
static const uint THREAD = 16;
Texture2D           inputImage  : register(t0);
RWTexture2D<float4> outputImage : register(u0);

//////////////////////////////////////////////////////////////////////////////////
//                             Implement
/////////////////////////////////////////////////////////////////////////////////
[numthreads(THREAD, THREAD, 1)]
void ExecuteSobel(const uint3 dispatchThreadID : SV_DispatchThreadID) // threadIDをグループスレッドから順に数え上げときのスレッドID (groupID * numThreadCount + threadIDInGroup)
{
    /*-------------------------------------------------------------------
	-        Acquire Neighborhood Pixel
	---------------------------------------------------------------------*/
	float3 pixels[9];
	int    k = 0;
    for (int y = -1; y <= 1; ++y)
    {
        for (int x = -1; x <= 1; ++x, ++k)
        {
            const int2 centerPixel = int2(dispatchThreadID.xy);
            pixels[k] = inputImage[centerPixel + int2(x, y)].rgb; 
        }
    }
    
    /*-------------------------------------------------------------------
	-        Calculate Filter
	---------------------------------------------------------------------*/
    float3 sobelHorizontal;
    float3 sobelVertical;
    sobelHorizontal = pixels[0] * -1 + pixels[2] *  1 + pixels[3] * -2 + pixels[5] * 2 + pixels[6] * -1 + pixels[8] * 1;
    sobelVertical   = pixels[0] * -1 + pixels[1] * -2 + pixels[2] * -1 + pixels[6] * 1 + pixels[7] * 2 + pixels[8] * 1;
    
    /*-------------------------------------------------------------------
	-        Display pixel
	---------------------------------------------------------------------*/
    const bool   isRender = sqrt(sobelVertical * sobelVertical + sobelHorizontal * sobelHorizontal) > 0; // 0は閾値にしてもいいかも
    const float4 color    = isRender ? float4(1,1,1,1) : float4(0,0,0,0);
    outputImage[dispatchThreadID.xy] = color;
}
#endif