//////////////////////////////////////////////////////////////////////////////////
///  @file   ShaderScreenPass.hlsl
///  @brief  四角形のスクリーンを表示する共通シェーダー
///  @author toide
///  @date   2024/07/15 1:52:22
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_SCREEN_PASS_HLSL
#define SHADER_SCREEN_PASS_HLSL

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Define
//////////////////////////////////////////////////////////////////////////////////
struct VSInputPositionTexture
{
    float2 Position : POSITION;
    float2 UV       : TEXCOORD0;
};

struct PSInputPositionTexture
{
    float4 Position : SV_POSITION;
    float2 UV       : TEXCOORD0;
};

//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////

/*!**********************************************************************
*  @brief     頂点シェーダを実行
*  @param[in] const VSInput input
*  @return    float4
*************************************************************************/
PSInputPositionTexture MainVS(VSInputPositionTexture input)
{
    // 最初からNDC座標系で代入することを想定しています. そのためそのまま返します
    PSInputPositionTexture output = (PSInputPositionTexture) 0;
    output.Position = float4(input.Position.xy, 0.0f, 1.0f);
    output.UV       = input.UV;
    return output;
}

#endif 
