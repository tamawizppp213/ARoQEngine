//////////////////////////////////////////////////////////////////////////////////
//              Title:  ShaderColor.hlsli
//            Content:  Shader Color
//             Author:  Toide Yutaro
//             Create:  2020_03_20
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_COLOR_HLSLI
#define SHADER_COLOR_HLSLI
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Define
//////////////////////////////////////////////////////////////////////////////////
/****************************************************************************
*				  			xyYToRec709
*************************************************************************//**
*  @class     float3 xyYToRec709(float2 xy, float Y = 1.0f)
*  @brief     Convert xy color space and luminance Y To LMS
*  @param[in] float3 color
*****************************************************************************/
float3 xyYToLMS(float2 xy, float Y = 1.0)
{
    static const float3x3 XYZtoLMS = 
    {
        0.7328, 0.4296, -0.1624,
        -0.7036, 1.6975, 0.0061,
        0.0030, 0.0136, 0.9834
    };
    
    float3 XYZ = Y * float3(xy.x / xy.y, 1.0, (1.0 - xy.x - xy.y) / xy.y);
    float3 LMS = mul(XYZtoLMS, XYZ);
    return LMS;
}

/****************************************************************************
*				  			xyYToRec709
*************************************************************************//**
*  @class     float3 xyYToRec709(float2 xy, float Y = 1.0f)
*  @brief     Convert xy color space and luminance Y To Rec709
*  @param[in] float3 color
*****************************************************************************/
float3 xyYToRec709(float2 xy, float Y = 1.0)
{
    static const float3x3 XYZtoRGB =
    {
        3.2409699419, -1.5373831776, -0.4986107603,
        -0.9692436363, 1.8759675015, 0.0415550574,
        0.0556300797, -0.2039769589, 1.0569715142
    };
    
    float3 XYZ = Y * float3(xy.x / xy.y, 1.0, (1.0 - xy.x - xy.y) / xy.y);
    float3 RGB = mul(XYZtoRGB, XYZ);
    float maxChannel = max(RGB.r, max(RGB.g, RGB.b));
    return RGB / max(maxChannel, 1.0);
}

/****************************************************************************
*				  			xyYToRec2020
*************************************************************************//**
*  @class     float3 xyYToRec2020(float2 xy, float Y = 1.0)
*  @brief     Convert xy color space and luminance Y To Rec2020 
*  @param[in] float3 color
*****************************************************************************/
float3 xyYToRec2020(float2 xy, float Y = 1.0)
{
    static const float3x3 XYZtoRGB =
    {
        1.7166511880, -0.3556707838, -0.2533662814,
        -0.6666843518, 1.6164812366, 0.0157685458,
        0.0176398574, -0.0427706133, 0.9421031212
    };
    
    float3 XYZ = Y * float3(xy.x / xy.y, 1.0, (1.0 - xy.x - xy.y) / xy.y);
    float3 RGB = mul(XYZtoRGB, XYZ);
    float maxChannel = max(RGB.r, max(RGB.g, RGB.b));
    return RGB / max(maxChannel, 1.0);
}

/****************************************************************************
*				  			LinearToSRGB
*************************************************************************//**
*  @class     float3 LinearToSRGB(float3 color)
*  @brief     Convert Linear To SRGB 
*  @param[in] float3 color
*****************************************************************************/
float3 LinearToSRGB(float3 color)
{
    // Approximately pow(color, 1.0 / 2.2)
    return color < 0.0031308 ? 12.92 * color : 1.055 * pow(abs(color), 1.0 / 2.4) - 0.055;
}

/****************************************************************************
*				  			SRGBToLinear
*************************************************************************//**
*  @class     float3 SRGBToLinear(float3 color)
*  @brief     Convert SRGB To Linear 
*  @param[in] float3 color
*****************************************************************************/
float3 SRGBToLinear(float3 color)
{
    // Approximately pow(color, 2.2)
    return color < 0.04045 ? color / 12.92 : pow(abs(color + 0.055) / 1.055, 2.4);
}

/****************************************************************************
*				  			Rec709ToRec2020
*************************************************************************//**
*  @class     float3 Rec2020ToRec709(float3 color)
*  @brief     Convert Rec709 To Rec2020 
*  @param[in] float3 color
*****************************************************************************/
float3 Rec709ToRec2020(float3 color)
{
    static const float3x3 conversion =
    {
        0.627402, 0.329292, 0.043306,
        0.069095, 0.919544, 0.011360,
        0.016394, 0.088028, 0.895578
    };
    return mul(conversion, color);
}

/****************************************************************************
*				  			Rec2020ToRec709
*************************************************************************//**
*  @class     float3 Rec2020ToRec709(float3 color)
*  @brief     Convert Rec2020 To Rec709 
*  @param[in] float3 color
*****************************************************************************/
float3 Rec2020ToRec709(float3 color)
{
    static const float3x3 conversion =
    {
        1.660496, -0.587656, -0.072840,
        -0.124547, 1.132895, -0.008348,
        -0.018154, -0.100597, 1.118751
    };
    return mul(conversion, color);
}

/****************************************************************************
*				  			LinearToST2084
*************************************************************************//**
*  @class     float3 LinearToST2084(float3 color)
*  @brief     Convert Linear Scale To ST2084 
*  @param[in] float3 color
*****************************************************************************/
float3 LinearToST2084(float3 color)
{
    float m1 = 2610.0 / 4096.0 / 4;
    float m2 = 2523.0 / 4096.0 * 128;
    float c1 = 3424.0 / 4096.0;
    float c2 = 2413.0 / 4096.0 * 32;
    float c3 = 2392.0 / 4096.0 * 32;
    float3 cp = pow(abs(color), m1);
    return pow((c1 + c2 * cp) / (1 + c3 * cp), m2);
}

float3 LinearToLMS(float3 color)
{
    static const float3x3 conversion =
    {
        3.90405e-1, 5.49941e-1, 8.92632e-3,
        7.08416e-2, 9.63172e-1, 1.35775e-3,
        2.31082e-2, 1.28021e-1, 9.36245e-1
    };
    
    return mul(conversion, color);
}

float3 LMSToLinear(float3 color)
{
    static const float3x3 conversion =
    {
        2.85847e+0, -1.62879e+0, -2.48910e-2,
        -2.10182e-1, 1.15820e+0, 3.24281e-4,
        -4.18120e-2, -1.18169e-1, 1.06867e+0
    };
    
    return mul(conversion, color);
}

#endif