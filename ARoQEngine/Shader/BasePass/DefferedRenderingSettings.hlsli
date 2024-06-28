//////////////////////////////////////////////////////////////////////////////////
///  @file   DefferedRenderingSettings.hpp
///  @brief  DefferedレンダリングのGBufferの設定など
///  @author toide
///  @date   2024/06/23 23:42:51
//////////////////////////////////////////////////////////////////////////////////
#ifndef DEFFERED_RENDERING_SETTINGS
#define DEFFERED_RENDERING_SETTINGS

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
/****************************************************************************
*				  		GBufferDesc
*************************************************************************//**
/*  @brief  Defferedレンダリングではfloat4の配列でMultiRenderTargetを使用するため, 細かい中身を定義した構造体です. 
*****************************************************************************/
struct GBufferDesc
{
    /*! @brief ベースカラー (0〜1)*/
    half3 Albedo;
    
    /*! @brief 拡散反射色 (0〜1)*/
    half3 Diffuse;
    
    /*! @brief 鏡面反射色 (0〜1)*/
    half3 Specular;
    
    /*! @brief 法線 (0〜1)*/
    half3 WorldNormal;
    
    /*! @brief 従法線 (0〜1)*/
    half3 WorldTangent;
    
    /*! @brief メタルネス (0〜1)*/
    half Metalness;
    
    /*! @brief Specularの強さ (0〜1)*/
    half SpecularPower;
    
};

#endif