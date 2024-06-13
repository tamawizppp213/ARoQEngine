//////////////////////////////////////////////////////////////////////////////////
//              Title:  ShaderBRDF.hlsli
//            Content:  BRDF function
//                      BRDFとは, 光どの方向にどれだけ反射されるかを表現した式です. 
//                      光の入射位置と反射位置は同じものとして仮定します. 
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
    float3 BaseColor;
    float  Roughness;
    float3 Normal;
    float  Metalness;
    float  EmissiveIntensity;
    float3 EmissiveColor;
    int    lightModel;
};

struct DirectLight
{
    float3 Diffuse;
    float3 Specular;
    float3 Transmission;
};

//////////////////////////////////////////////////////////////////////////////////
//                             BRDF Implement
//////////////////////////////////////////////////////////////////////////////////
/****************************************************************************
*				  			Calculate_F0
****************************************************************************/
/* @fn        float3 Calculate_F0(float refractive_1, float refractive_2 = 1.0f)
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

float Fresnel_Schlick(float u, float F90, float F0 = 1.0f, float powValue = 5.0f)
{
    return F0 + (F90 - F0) * pow(1.0f - u, powValue); 
}

float3 Fresnel_Schlick(float u, float3 F90, float3 F0 = 1.0f, float powValue = 5.0f)
{
    return F0 + (F90 - F0) * pow(1.0f - u, powValue);
}

//////////////////////////////////////////////////////////////////////////////////
//                            Diffuse
//                   拡散反射のBRDF計算に使用します.
//      DiffuseBRDF_各使用モデル名の関数が並んでおり, Defaultは正規化ランバートです.
//      マクロでコンパイル時にどのモデルを採用するか決定できるようにしています. 
//      選択できるモデル: Normalized_Lambert, Disney, OnenNayar
//////////////////////////////////////////////////////////////////////////////////
/****************************************************************************
*				  		DiffuseBRDF_Normalized_Lambert
****************************************************************************/
/* @fn        float DiffuseBRDF_Normalized_Lambert(float3 cDiffuse)

*  @brief     Calcurate Lambert Diffuse

*  @param[in] void

*  @return    float
*****************************************************************************/
float DiffuseBRDF_Normalized_Lambert()
{
    return (1.0f / PI);
}

/****************************************************************************
*				  		DiffuseBRDF_Disney
****************************************************************************/
/* @fn        float DiffuseBRDF_Disney(const float normalDotEye, const float normalDotLight, const float lightDotHalf, const float roughness)

*  @brief     Calcurate BRDF Diffuse using the disney(burley) model.
*             [Burley 2012, "Physically-Based Shading at Disney"]
 
*  @param[in] const float normalDotEye   : normalize(surfaceNormal ・ toEye)
*  @param[in] const float normalDotLight : normalize(surfaceNormal ・ toLight)
*  @param[in] const float roughness

*  @return    float f_reflect_diffuse
*****************************************************************************/
float DiffuseBRDF_Disney(const float normalDotEye, const float normalDotLight, const float lightDotHalf, const float roughness)
{
    const float  F90          = 0.5f + 2.0f * roughness * pow(lightDotHalf, 2.0f); // 光が平行に入ってきたときの反射率
    const float lightScatter = Fresnel_Schlick(normalDotLight, F90, 1.0f);        // 平行に入ってきたときにも拡散の影響が出るようにしてる
    const float toEyeScatter = Fresnel_Schlick(normalDotEye, F90, 1.0f);
    return lightScatter * toEyeScatter * (1.0f / PI);
}

/****************************************************************************
*				  		DiffuseBRDF_OnenNayar
****************************************************************************/
/* @fn        float3 DiffuseBRDF_OnenNayar(const float normalDotEye, const float normalDotLight, const float eyeDotHalf, const float roughness)

*  @brief     Calcurate BRDF Diffuse using the onenNayar model.
*             [Gotanda 2012, "Beyond a Simple Physically Based Blinn-Phong Model in Real-Time""]
              観測方向と表面粗さによって拡散反射の見え方が変化する現象を再現してる
 
*  @param[in] const float normalDotEye   : normalize(surfaceNormal ・ toEye)
*  @param[in] const float normalDotLight : normalize(surfaceNormal ・ toLight)
*  @param[in] const float eyeDotHalf     : normalize(toEye ・ half)
*  @param[in] const float roughness

*  @return    float f_reflect_diffuse
*****************************************************************************/
float DiffuseBRDF_OnenNayar(const float normalDotEye, const float normalDotLight, const float eyeDotHalf, const float roughness)
{
    const float alpha         = roughness * roughness;
    const float alpha2        = alpha * alpha;
    const float toEyeDotLight = 2 * eyeDotHalf * eyeDotHalf - 1; // double angle identity
    const float cosri         = toEyeDotLight - normalDotEye * normalDotLight;
    const float c1 = 1.0f - 0.5f * alpha2 / (alpha2 + 0.33f);
    const float c2 = 0.45 * alpha2 / (alpha2 + 0.09) * cosri * (cosri >= 0 ? rcp(max(normalDotLight, normalDotEye)) : 1);
    
    return 1.0f / PI * (c1 + c2) * (1.0f + roughness * 0.5f);

}

/****************************************************************************
*				  		DiffuseBRDF_Gotanda
****************************************************************************/
/* @fn        float3 DiffuseBRDF_Gotanda(const float normalDotEye, const float normalDotLight, const float eyeDotHalf, const float roughness)

*  @brief     Calcurate BRDF Diffuse using the Gotanda model.
*             [Gotanda 2014, "Designing Reflectance Models for New Consoles"]
 
*  @param[in] const float normalDotEye   : normalize(surfaceNormal ・ toEye)
*  @param[in] const float normalDotLight : normalize(surfaceNormal ・ toLight)
*  @param[in] const float eyeDotHalf     : normalize(toEye ・ half)
*  @param[in] const float roughness

*  @return    float3 f_reflect_diffuse
*****************************************************************************/
float DiffuseBRDF_Gotanda(const float normalDotEye, const float normalDotLight, const float eyeDotHalf, const float roughness, const float F0)
{
    const float alpha       = roughness * roughness;
    const float alpha2      = alpha * alpha;
    const float eyeDotLight = 2 * eyeDotHalf * eyeDotHalf - 1; // 2倍角の公式
    const float cosri       = eyeDotLight - normalDotEye * normalDotLight;
    
    const float alpha2_13 = alpha2 + 1.36053f;
    const float Fr = (1 - (0.542026 * alpha2 + 0.303573f * alpha) / alpha2_13) * (1 - pow(1 - normalDotLight, 5.0f - 4.0f * alpha2) / alpha2_13) 
    * ((-0.733996f * alpha2 * alpha + 1.50912f * alpha2 - 1.16402f * alpha) 
    * pow(1.0f - normalDotEye, 1.0f + rcp(39.0f * alpha2 * alpha2 + 1.0f)) + 1.0f);
    
    const float Lm = (max(1 - 2 * alpha, 0) * (1 - pow(1 - normalDotLight, 5.0f)) + min(2 * alpha, 1)) * (1 - 0.5f * alpha * (normalDotLight - 1)) * normalDotLight;
    float Vd = (alpha2 / ((alpha2 + 0.09f) * (1.31072f + 0.995584f * normalDotEye))) * (1 - pow(1 - normalDotLight, (1 - 0.3726732f * normalDotEye * normalDotEye) / (0.188566f + 0.38841f * normalDotEye)));
    float Bp = cosri < 0 ? 1.4 * normalDotEye * normalDotLight * cosri : cosri;
    float Lr = (21.0 / 20.0) * (1 - F0) * (Fr * Lm + Vd + Bp);
    return Lr / PI;
}


//////////////////////////////////////////////////////////////////////////////////
//                            Specular
//                     鏡面反射のBRDF計算に使用します.
//            SpecularBRDF_各使用モデル名の関数が並んでいます. 
//            Default: F (Fresnel_Schlick), G (SmithJointApproximate), D(GGX)
//            MacroOption: 
//            F: None
//            G: Neumann, Kelmen, Sclick, Smith, SmithJointApproximante, SmithJoint, Schlick
//            D: Beckmann
//            Explain: 
//            F: 表面から反射してみる光の量が視野角に依存する. 
//            G: マイクロファセット同士が光の反射経路を遮蔽することにより, 失われてしまう光の反射成分を考慮するもの.
//            D: 物体表面のミクロレベルの各微小平面の法線がどれくらい指定の方向を向いているかという法線の分布を表す関数
//////////////////////////////////////////////////////////////////////////////////
/****************************************************************************
*				  			SpecularD_NDF_GGX
****************************************************************************/
/* @fn        float SpecularBRDF_D_NDF_GGX(float NormalDotHalf, float roughness)
*
*  @brief     Calculate Specular D (normal distribution fuction)
              Has long tale.
*
*  @param[in] const float NormalDotHalf
*  @param[in] const float roughness
*
*  @return    float (0.0f ～ 1.0f)
*****************************************************************************/
float SpecularBRDF_D_NDF_GGX(const float normalDotHalf, const float roughness)
{
    const float alpha  = roughness * roughness;
    const float alpha2 = alpha * alpha;                                       // roughnessの4乗
    const float NDotH2 = pow(normalDotHalf, 2);                               // surfaceNormal ・ half の2乗
    const float denominator = PI * pow((NDotH2 * (alpha2 - 1.0f) + 1.0f), 2); // 分母
    
    const float result = alpha2 / denominator;
    
    // 0.0f以上なのは, マクロ面とマイクロ面が同じ側にある方向に対してのみマイクロ面が見えることを保証するもの.
    // 1.0fよりは大きくしない. あくまで確率分布
    return saturate(result);
}

/****************************************************************************
*				  			SpecularD_NDF_Beckmann
****************************************************************************/
/* @fn        float SpecularBRDF_D_NDF_Beckmann(float NormalDotHalf, float roughness)
*
*  @brief     Calculate Specular D (normal distribution fuction)
              Beckmann distribution
*
*  @param[in] const float NormalDotHalf
*  @param[in] const float roughness
*
*  @return    float (0.0f ～ 1.0f)
*****************************************************************************/
float SpecularBRDF_D_NDF_Beckmann(const float normalDotHalf, const float roughness)
{
    const float roughness2 = roughness * roughness;
    const float NDotH2     = normalDotHalf * normalDotHalf;
    const float NDotH4     = NDotH2 * NDotH2; // cos項 (事前にNormalizeされていることが必要です)
    
    float result = 1.0f / (PI * roughness2 * NDotH4);
    result *= exp((NDotH2 - 1.0f) / (NDotH2 * roughness2));
    return saturate(result);
}

/****************************************************************************
*				  			Fresnel_Schlick
****************************************************************************/
/* @fn        float3 Fresnel_Schlick(float3 F0, float3 normal, float3 lightVector)

*  @brief     Calcurate Reflectance

*  @param[in] float3 F0 : ((n1 - n2) / (n1 + n2))^2 : n(refraction, default n2 = 1 (air))
*  @param[in] float3 normal 
*  @param[in] float3 lightVector

*  @return    float3
*****************************************************************************/
float3 SpecularBRDF_F_Schlick(const float3 toEye, const float3 halfVector, const float3 F90, const float3 F0 = 1.0f)
{
    const float normalDotLight = saturate(dot(toEye, halfVector));
    const float3 reflectance = F0 + (F90 - F0) * pow(1.0f - normalDotLight, 5.0f);
    return reflectance;
}
/****************************************************************************
*				  			SpecularBRDF_F_None
****************************************************************************/
/* @fn        SpecularBRDF_F_None()

*  @brief     Return (1.1.1)

*  @param[in] void

*  @return    float3 (1,1,1) 
*****************************************************************************/
float3 SpecularBRDF_F_None()
{
    return float3(1,1,1);
}

/****************************************************************************
*				  			SpecularBRDF_V_Neumann
****************************************************************************/
/* @fn        float SpecularBRDF_V_Neumann(const float normalDotEye, const float normalDotLight)

*  @brief     Calculate neumann model

*  @param[in] const float normalDotEye (saturate + normalized)
*  @param[in] const float normalDotLight (saturate + normalized)

*  @return    float
*****************************************************************************/
float SpecularBRDF_V_Neumann(const float normalDotEye, const float normalDotLight)
{
    return 1.0f / (4 * max(normalDotLight, normalDotEye));
}

float SpecularBRDF_V_Kelemen(const float eyeDotHalf)
{
    return rcp(4 * eyeDotHalf * eyeDotHalf + 1e-5);
}

float SpecularBRDF_V_Schlick(const float roughness, const float normalDotEye, const float normalDotLight)
{
    const float alpha         = roughness * roughness;
    const float k             = alpha * 0.5f;
    const float vSchlickEye   = normalDotEye * (1 - k) + k;
    const float vSchlickLight = normalDotLight * (1 - k) + k;
    return 0.25f / (vSchlickEye * vSchlickLight);
}

float SpecularBRDF_V_Smith(const float roughness, const float normalDotEye, const float normalDotLight)
{
    const float alpha       = roughness * roughness;
    const float alpha2      = alpha * alpha;
    const float vSmithEye   = normalDotEye   + sqrt(normalDotEye   * (normalDotEye   - normalDotEye   * alpha2) + alpha2);
    const float vSmithLight = normalDotLight + sqrt(normalDotLight * (normalDotLight - normalDotLight * alpha2) + alpha2);
    return rcp(vSmithEye * vSmithLight);
}

float SpecularBRDF_V_SmithJointApproximate(const float roughness, const float normalDotEye, const float normalDotLight)
{
    const float alpha       = roughness * roughness;
    const float vSmithEye   = normalDotLight * (normalDotEye   * (1 - alpha) + alpha);
    const float vSmithLight = normalDotEye   * (normalDotLight * (1 - alpha) + alpha);
    return 0.5f * saturate(rcp(vSmithEye + vSmithLight)); // 1を超えてくる可能性があったので, 念のため. (影が補色で表示されるバグがあった)
}

float SpecularBRDF_V_SmithJoint(const float roughness, const float normalDotEye, const float normalDotLight)
{
    const float alpha       = roughness * roughness;
    const float alpha2      = alpha * alpha;
    const float vSmithEye   = normalDotLight * sqrt(normalDotEye   * (normalDotEye   - normalDotEye   * alpha2) + alpha2);
    const float vSmithLight = normalDotEye   * sqrt(normalDotLight * (normalDotLight - normalDotLight * alpha2) + alpha2);
    return 0.5f * saturate(rcp(vSmithEye + vSmithLight)); // 1を超えてくる可能性があったので, 念のため
}

/****************************************************************************
*				  			Geometry_Smiths_Schlick_GGX
****************************************************************************/
/* @fn        float Geometry_Smiths_Schlick_GGX(float3 normal, float3 toEye, float roughness)
*  @brief     Calculate Specular G
*  @param[in] float3 normal
*  @param[in] float3 toEye 
*  @param[in] float roughness
*****************************************************************************/
float Geometry_Smiths_Schlick_GGX(float3 normal, float3 toEye, float roughness)
{
    const float k = pow((roughness + 1.0f), 2) / 8.0f;
    const float NDotToEye = max(0.0f, dot(normal, toEye));
    const float denominator = (NDotToEye * (1.0f - k) + k) + 0.0001f;
    
    return NDotToEye / denominator;

}

/****************************************************************************
*				  	Geometry_Smiths_Schlick_GGX_EnvironmentMap
****************************************************************************/
/* @fn        float Geometry_Smiths_Schlick_GGX_EnvironmentMap(float3 normal, float3 toEye, float roughness)
*  @brief     Calculate Specular G
*  @param[in] float3 normal
*  @param[in] float3 toEye 
*  @param[in] float roughness
*****************************************************************************/
float Geometry_Smiths_Schlick_GGX_EnvironmentMap(float3 normal, float3 toEye, float roughness)
{
    const float k = pow(roughness, 2) / 2.0f;
    const float NDotToEye = max(0.0f, dot(normal, toEye));
    const float denominator = (NDotToEye * (1.0f - k) + k) + 0.0001f;
    return NDotToEye / denominator;

}

/****************************************************************************
*				  		Masking_And_Shadowing_Function
****************************************************************************/
/* @fn        float Masking_And_Shadowing_Function(float3 normal, float3 toEye, float3 light, float roughness)
*  @brief     Masking, shadowing fuction
*  @param[in] float3 F0 : ((n1 - n2) / (n1 + n2))^2 : n(refraction, default n2 = 1 (air))
*  @param[in] float3 normal 
*  @param[in] float3 lightVector
*****************************************************************************/
float Masking_And_Shadowing_Function(float3 normal, float3 toEye, float3 light, float roughness)
{
    return Geometry_Smiths_Schlick_GGX(normal, toEye, roughness)
         * Geometry_Smiths_Schlick_GGX(normal, light, roughness);

}

/****************************************************************************
*				  		Masking_Abd_Shadowing_Function_EnvironmentMap
****************************************************************************/
/* @fn        float Masking_Abd_Shadowing_Function_EnvironmentMap(float3 normal, float toEye, float3 light, float roughness)
*  @brief     Masking, shadowing fuction
*  @param[in] float3 F0 : ((n1 - n2) / (n1 + n2))^2 : n(refraction, default n2 = 1 (air))
*  @param[in] float3 normal 
*  @param[in] float3 lightVector
*****************************************************************************/
float Masking_Abd_Shadowing_Function_EnvironmentMap(float3 normal, float toEye, float3 light, float roughness)
{
    return Geometry_Smiths_Schlick_GGX_EnvironmentMap(normal, toEye, roughness)
         * Geometry_Smiths_Schlick_GGX_EnvironmentMap(normal, light, roughness);
}

float3 ClearCoatTransmittance(float normalDotLight, float normalDotEye, float metallic, float3 baseColor)
{
    float3 transmittance = 1.0f.xxx;
    
    /*-------------------------------------------------------------------
	-            クリアコートの被覆率をメタルネスで定義 
	---------------------------------------------------------------------*/
    const float clearCoatCoverage = metallic;
    
    /*-------------------------------------------------------------------
	-            透過カラーの計算
	---------------------------------------------------------------------*/
    if (clearCoatCoverage > 0.0f)
    {
        const float layerThickness = 1.0f; // 正規化された厚みを仮定している. (これは後で修正案件)
        
        // 媒体内部の経路の長さは, 光の入射位置から表面の底面, 見ている人までの距離
        const float thinDistance   = layerThickness * (rcp(normalDotEye) + rcp(normalDotLight));
        
        // 反射色の定義
        const float3 transmittanceColor = DiffuseBRDF_Normalized_Lambert() * baseColor;
        
        // このため、消光は層厚を2回通過することで正規化される。
        const float3 extinctionCoefficient = -log(max(transmittanceColor, 0.0001f)) / (2.0f * layerThickness);
        
        // 光学的深度は, 上記でw計算されたthinDistanceに対して表か
        const float3 opticalDepth = extinctionCoefficient *  max(thinDistance - 2.0 * layerThickness, 0.0);
        
        transmittance = exp(-opticalDepth);
        transmittance = lerp(1.0, transmittance, clearCoatCoverage);
    }
    
    return transmittance;
}
//////////////////////////////////////////////////////////////////////////////////
//                            Luminance
//////////////////////////////////////////////////////////////////////////////////

/****************************************************************************
*				  	AccumlateSurfaceEnergy
****************************************************************************/
/* @class     float3 AccumlateSurfaceEnergy(in BRDFSurface surface, float3 light, float3 toEye)
*  @brief     Diffuse: Lambert Specular: Cook_Torrence_Model
*  @param[in] BRDFSurface surface
*  @param[in] float3 light
*  @param[in] float3 toEye
*****************************************************************************/
DirectLight AccumulateSurfaceEnergy(in BRDFSurface surface, float3 toLight, float3 toEye)
{
    /*-------------------------------------------------------------------
	-             Get each of vector for surface.  
	---------------------------------------------------------------------*/
    const float3 toEyeNormalize   = normalize(toEye);                             // 視線方向に向かう正規化ベクトル
    const float3 toLightNormalize = normalize(toLight);                           // 光源方向に向かう正規化ベクトル
    const float3 surfaceNormal    = normalize(surface.Normal);                    // マクロ表面の法線方向
    const float3 halfVector       = normalize(toEyeNormalize + toLightNormalize); // ハーフベクトル (視線とライト), 反射ベクトル = ハーフベクトルで最も光の反射が強い 
    const float  normalDotHalf    = saturate(dot(surfaceNormal, halfVector));     // 表面とハーフベクトル間の角度
    const float  normalDotToEye   = saturate(dot(surfaceNormal, toEyeNormalize)); // 表面と視線方向の角度
    const float  normalDotLight   = saturate(dot(surfaceNormal, toLight));        // 表面とライト方向の角度
    const float  lightDotHalf     = saturate(dot(toLight, halfVector));           // ライトとハーフベクトルの角度
    const float  eyeDotHalf       = saturate(dot(toEye, halfVector));             // 視線とハーフベクトルの角度
    
    /*-------------------------------------------------------------------
	-             Surface infomation
	---------------------------------------------------------------------*/
    const float  roughness = surface.Roughness;
    const float  metalness = surface.Metalness;
    //const float3 specular = lerp(0.08f * saturate(surface.SpecularIntensity.xxx), surface.BaseColor, metalness.xxx);
    const float3 F0        = lerp(float3(0.04f, 0.04f, 0.04f), surface.BaseColor, metalness);
    const float3 diffuse   = surface.BaseColor * (1.0f - metalness);
    
    /*-------------------------------------------------------------------
	-             Specular (Fresnel and Cook-Torrence model) 
	---------------------------------------------------------------------*/
    // F term
#ifdef USE_SPECULAR_F_NONE
    const float3 F  = SpecularBRDF_F_None();
#else
    const float3 F = SpecularBRDF_F_Schlick(toEyeNormalize, halfVector, F0);
#endif    
    
    // V(G) term
#ifdef USE_SPECULAR_V_NEUMANN
    const float V = SpecularBRDF_V_Neumann(normalDotToEye, normalDotLight); 
#elif  USE_SPECULAR_V_KELEMEN
    const float V = SpecularBRDF_V_Kelemen(eyeDotHalf);
#elif  USE_SPECULAR_V_SMITH
    const float V = SpecularBRDF_V_Smith(roughness, normalDotToEye, normalDotLight);
#elif  USE_SPECULAR_V_SCHLICK
    const float V = SpecularBRDF_V_Schlick(roughness, normalDotToEye, normalDotLight);
#elif  USE_SPECULAR_V_SMITHJOINT
    const float V = SpecularBRDF_V_SmithJoint(roughness, normalDotToEye, normalDotLight);
#else
    const float V = SpecularBRDF_V_SmithJointApproximate(roughness, normalDotToEye, normalDotLight);
#endif
    
    // D term
#ifdef USE_DIFFUSE_BECKMANN
    const float  D  = SpecularBRDF_D_NDF_Beckmann(normalDotHalf, roughness);
#else
    const float  D  = SpecularBRDF_D_NDF_GGX(normalDotHalf, roughness);
#endif

    //const float  denominator = (4.0f * normalDotToEye * normalDotLight) + 0.0001f;
    //const float3 Ls          = surface.Specular * numerator;
    const float3 Ls = ((D * V) * F) * normalDotLight;
     //const float3 cDiffuse = (1.0f - F) * (1.0f - metalness) * albedo;
    
    /*-------------------------------------------------------------------
	-             Diffuse項の輝度計算 (Lambert model) 
	---------------------------------------------------------------------*/
#ifdef USE_DIFFUSE_DISNEY
    const float Ld = DiffuseBRDF_Disney(normalDotToEye, normalDotLight, lightDotHalf, roughness);
#elif USE_DIFFUSE_ONEN_NAYAR
    const float Ld = DiffuseBRDF_OnenNayar(normalDotToEye, normalDotLight,eyeDotHalf, roughness);
#elif USE_DIFFUSE_GOTANDA
    const float Ld = DiffuseBRDF_Gotanda(normalDotToEye, normalDotLight,eyeDotHalf, roughness, F0);
#else   
    const float Ld = DiffuseBRDF_Normalized_Lambert() * normalDotLight;  
#endif
    
    /*-------------------------------------------------------------------
	-             Set directLight
	---------------------------------------------------------------------*/
    DirectLight directLight;
    directLight.Diffuse      = Ld * diffuse;
    directLight.Transmission = 0.0f;
    directLight.Specular     = Ls * metalness;
    
    return directLight; // Luminance emit + specular +  diffuse 
}

#endif