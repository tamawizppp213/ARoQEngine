//////////////////////////////////////////////////////////////////////////////////
//              Title:  ShaderZPrepass.hlsl
//            Content:  不透明オブジェクトの深度値(0～1)をテクスチャに描画します
//             Author:  Toide Yutaro
//             Create:  2023_10_05
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_ZPREPASS_HLSL
#define SHADER_ZPREPASS_HLSL
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Core/ShaderVertexType.hlsli"
#include "../Core/ShaderConstantBuffer3D.hlsli"

//////////////////////////////////////////////////////////////////////////////////
//                             Define
/////////////////////////////////////////////////////////////////////////////////
struct PSIn
{
    float4 Position : SV_POSITION;
    float3 Depth    : TEXCOORD0;	
};

//////////////////////////////////////////////////////////////////////////////////
//                            Implement
//////////////////////////////////////////////////////////////////////////////////
float LinearizeDepth(const float depth, const float near, const float far)
{
    return (2.0f * near) / (far + near - depth * (far - near));
}

PSIn VSMain( VSInputSkinVertex vertexIn)
{
	PSIn result;
	
	/*-------------------------------------------------------------------
	-        transform to world space 
	---------------------------------------------------------------------*/
	const float4 positionWorld      = mul(World, vertexIn.Position);      // モデルのローカル座標系 -> ワールド座標系に変換
    const float4 positionView       = mul(View, positionWorld);           // ワールド座標系       -> ビュー座標系
    result.Position = mul(Projection, positionView);                      // ビュー座標系     -> スクリーン座標系
		
    result.Depth.x = result.Position.z / result.Position.w;       // 非線形な深度状態の正規化(0～1)    : Raw
    result.Depth.y = positionView.z;                              // カメラからのワールド座標としての距離 : Eye
    result.Depth.z = LinearizeDepth(result.Depth.x, NearZ, FarZ); // 線形な深度状態の正規化(0～1)      : Linear01
	return result;
}

float4 PSMain(PSIn psIn) : SV_TARGET
{
	// Raw, Eye, Linear01の順番で入っています. 
	// ただし, 描画負荷の関係で現状R32_Floatで動作させているので, y,z,w成分は無視されます. 治ったらR32G32B32A32で動かしてください.
	return float4(psIn.Depth.x, psIn.Depth.y, psIn.Depth.z, 1.0f);
}
#endif