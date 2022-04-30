//////////////////////////////////////////////////////////////////////////////////
//              Title:  ShaderBRDF.hlsli
//            Content:  BRDF function
//             Author:  Toide Yutaro
//             Create:  
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_BRDF_HLSLI
#define SHADER_BRDF_HLSLI
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Define
//////////////////////////////////////////////////////////////////////////////////
#define PI 3.141592653589f
struct BRDFSurface
{
    float3 Normal;
    float  Roughness;
    float3 Diffuse;
    float  Metalness;
    float3 Specular;
    float  EmissiveIntensity;
    float3 EmissiveColor;
    int    lightModel;
};
//////////////////////////////////////////////////////////////////////////////////
//                             BRDF Implement
//////////////////////////////////////////////////////////////////////////////////
/****************************************************************************
*				  			Calculate_F0
*************************************************************************//**
*  @fn        float3 Calculate_F0(float refractive_1, float refractive_2 = 1.0f)
*  @brief     Calculate F0
*  @param[in] float refractive index
*****************************************************************************/
float3 Calculate_F0(float refractive_1, float refractive_2 = 1.0f)
{
    const float3 refVec_1 = float3(refractive_1, refractive_1, refractive_1);
    const float3 refVec_2 = float3(refractive_2, refractive_2, refractive_2);
    const float3 F0 = abs(refVec_1 - refVec_2);
    return F0;
}
/****************************************************************************
*				  			Fresnel_Schlick
*************************************************************************//**
*  @fn        float3 Fresnel_Schlick(float3 F0, float3 normal, float3 lightVector)
*  @brief     Calcurate Reflectance
*  @param[in] float3 F0 : ((n1 - n2) / (n1 + n2))^2 : n(refraction, default n2 = 1 (air))
*  @param[in] float3 normal 
*  @param[in] float3 lightVector
*  @return    float3
*****************************************************************************/
float3 Fresnel_Schlick(float3 F0, float3 normal, float3 lightVector)
{
    const float  normalDotLight = saturate(dot(normal, lightVector));
    const float3 reflectance    = F0 + (float3(1, 1, 1) - F0) * pow(1.0 - normalDotLight, 5.0f);
    return reflectance;
}
float3 Fresnel_Schlick(float normalDotHalf, float3 F0, float3 F90 = 1.0f, float powValue = 5.0f)
{
    return lerp(F0, F90, pow(1.0f - normalDotHalf, powValue));
}
//////////////////////////////////////////////////////////////////////////////////
//                            Diffuse
//////////////////////////////////////////////////////////////////////////////////
/****************************************************************************
*				  		DiffuseBRDF_Lambert
*************************************************************************//**
*  @fn        float3 DiffuseBRDF_Lambert(float3 cDiffuse)
*  @brief     Calcurate Lambert Diffuse
*  @param[in] float cDiffuse
*  @return    float3 
*****************************************************************************/
float3 DiffuseBRDF_Lambert(float3 cDiffuse)
{
    return cDiffuse / PI;
}
//////////////////////////////////////////////////////////////////////////////////
//                            Specular
//////////////////////////////////////////////////////////////////////////////////
/****************************************************************************
*				  			NDF_GGX
*************************************************************************//**
*  @fn        float NDF_GGX(float NormalDotHalf, float roughness)
*  @brief     Calculate Specular D (normal distribution fuction)
*  @param[in] float NormalDotHalf
*  @param[in] float roughness
*****************************************************************************/
float SpecularBRDF_GGX(float normalDotHalf, float roughness)
{
    float result = 0.0f;
    const float alpha  = roughness * roughness;
    const float alpha2 = alpha * alpha;
    const float NDotH2 = pow(normalDotHalf, 2);
    // ‚Ü‚¾
    return result;
}
//////////////////////////////////////////////////////////////////////////////////
//                            Luminance
//////////////////////////////////////////////////////////////////////////////////
/****************************************************************************
*				  	SurfaceEnergy
*************************************************************************//**
*  @class     float3 SurfaceEnergy(in BRDFSurface surface, float3 light, float3 toEye)
*  @brief     Diffuse: Lambert Specular: Cook_Torrence_Model
*  @param[in] BRDFSurface surface
*  @param[in] float3 light
*  @param[in] float3 toEye
*****************************************************************************/
float3 SurfaceEnergy(in BRDFSurface surface, float3 light, float3 toEye)
{
    /*-------------------------------------------------------------------
	-             Get each of vector for surface.  
	---------------------------------------------------------------------*/
    const float3 toEyeNormalize  = normalize(toEye);
    const float3 normal          = normalize(surface.Normal);
    const float3 halfVector      = normalize(toEyeNormalize + surface.Normal);
    const float  normalDotHalf   = saturate(dot(normal, halfVector));
    const float  normalDotToEye  = saturate(dot(normal, toEyeNormalize));
    const float  normalDotLight  = saturate(dot(normal, light));
    
    /*-------------------------------------------------------------------
	-             Surface infomation
	---------------------------------------------------------------------*/
    const float3 albedo    = surface.Diffuse;
    const float  roughness = surface.Roughness;
    const float  metalness = surface.Metalness;
    const float3 F0 = lerp(float3(0.04f, 0.04f, 0.04f), albedo, metalness);
    
    /*-------------------------------------------------------------------
	-             Specular (Fresnel and Cook-Torrence model) 
	---------------------------------------------------------------------*/
    const float3 F = Fresnel_Schlick(F0, halfVector, toEyeNormalize);
    
    /*-------------------------------------------------------------------
	-             Diffuse (Lambert model) 
	---------------------------------------------------------------------*/
    const float3 cDiffuse = (float3(1, 1, 1) - F) * (1.0f - metalness) * albedo;
    const float3 Ld = DiffuseBRDF_Lambert(cDiffuse);
    const float3 Ls = float3(1,1,1);
    const float3 Le = float3(0, 0, 0);
    
    return Le + Ls + Ld; // Luminance emit + specular +  diffuse 
}
#endif