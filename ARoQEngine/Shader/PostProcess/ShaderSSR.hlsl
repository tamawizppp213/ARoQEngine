//////////////////////////////////////////////////////////////////////////////////
//              Title:  ShadeSSR.hlsl
//            Content:  Screen space reflection shader
//                      https://zenn.dev/mebiusbox/articles/43ecf1bb12831c 
//                      https://tips.hecomi.com/entry/2016/04/04/022550
//             Author:  Toide Yutaro
//             Create:  2023_02_23
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_SSR_HLSL
#define SHADER_SSR_HLSL
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Core/ShaderConstantBuffer3D.hlsli"
#include "../Core/ShaderVertexType.hlsli"
#define VertexIn  VSInputVertex
#define VertexOut VSOutputVertex
//////////////////////////////////////////////////////////////////////////////////
//                             Define
/////////////////////////////////////////////////////////////////////////////////
SamplerState SamplerPointClamp : register(s1);
SamplerState SamplerDepth       : register(s2);

// Textyre
Texture2D<float4> BaseColorTexture : register(t0);
Texture2D<float4> NormalTexture : register(t1);  // ���󃏁[���h�@���ɂȂ��Ă���̂�, �C���K�v
Texture2D<float>  DepthTexture  : register(t2);  // NDC��Ԃ̐[�x�}�b�v

cbuffer SSRSetting : register(b1)
{
    int   MaxRayMarchIteration;     // ���C�}�[�`���O���s���ő�̎��s�� (�f�t�H���g��16)
    int   MaxBinarySearchIteration; // �񕪒T�����s�����s��
    float MaxRayDistance;           // ���C�̍ő勗��
    float RayThicknessInViewSpace;  // ���C�̌���
    float MaxFadeDistance;
};

struct PSInput
{
    float4 Position    : SV_Position;
    float3 NearPlaneViewPosition : POSITION;
    float2 UV          : TEXCOORD;
};

//////////////////////////////////////////////////////////////////////////////////
//                            Implement
//////////////////////////////////////////////////////////////////////////////////
/* @brief : NDC��Ԃ̐[�x��View��Ԃɕ�������*/
float NDCDepthToViewDepth(const float z_ndc);
/* @brief : ���˃x�N�g�������ƂɃV�[���̏Փ˔�����s��.*/
bool ExecuteRayMarchInCameraSpace(const float3 rayOrigin, const float3 rayDirection, out float3 hitRayPosition, out float2 hitUV);
/* @brief : ���C�̏Փ˔���*/
bool IntersectThicknessRay(const float z, const float2 uv);
/* @brief : �A���t�@�l�̎擾*/
float CalculateAlpha(const bool isIntersect, const float2 uv, const float3 hitRayPosition, const float3 rayOrigin);

PSInput VSMain(VertexIn vertexIn)
{
    PSInput result;
    result.Position = vertexIn.Position;
    result.UV       = vertexIn.UV;
    
    // Transform rectangle corners to view space near plane
    const float4 position = mul(InverseProjection, vertexIn.Position);
    result.NearPlaneViewPosition = position.xyz / position.w;
    return result;
}

/****************************************************************************
*                       PSMain
****************************************************************************/
/* @fn        float4 PSMain(VertexOut input) : SV_TARGET

*  @brief     ScreenSpacerReflection post effect

*  @param[in] VertexOut input

*  @return �@�@float4
*****************************************************************************/
float4 ExecuteSSR(PSInput input) : SV_TARGET
{
    /*-------------------------------------------------------------------
	-        ���˃x�N�g�������߂�.
	---------------------------------------------------------------------*/
    // �[�x�}�b�v����NDC��Ԃɂ����邱�̃s�N�Z����z���W���擾����
    // �[�x�}�b�v�̒�`��zPrepass.hlsl���Q�Ƃ��Ă�������, 
    const float ndcDepth         = DepthTexture.Sample(SamplerDepth, input.UV).r; 
    
    // �r���[��Ԃɂ�����[�x (�J���� -> ���_�܂ł̐[�x)
    const float viewSurfaceDepth = NDCDepthToViewDepth(ndcDepth);
    
    // �[�x�l�����Ƃ�, View��Ԃɂ�����T���v�����O�ʒu���č\������.
    const float  distanceRatio       = (viewSurfaceDepth / input.NearPlaneViewPosition.z); // 
    const float3 viewSurfacePosition = distanceRatio * input.NearPlaneViewPosition; // NearPlane�͒P�ʃx�N�g���I�Ȉ���.
    
    // ���[���h�@������r���[�@�����擾����
    const float3 worldSurfaceNormal = (NormalTexture.Sample(SamplerPointClamp, input.UV).xyz * 2) - 1;
    const float3 viewSurfaceNormal = normalize(mul((float3x3)View, worldSurfaceNormal));
    
    // ���C�����ʒu
    const float3 rayOrigin    = viewSurfacePosition;
    
    // �J�������_���畨���\�ʂɓ��˂�, �����\�ʂ̖@���ɑ΂��Ĕ��˂��s�� 
    const float3 rayDirection = reflect(normalize(viewSurfacePosition), viewSurfaceNormal);
    
    /*-------------------------------------------------------------------
	-        �V�[���Ƃ̏Փ˔�����s��
	---------------------------------------------------------------------*/
    float2 hitRayUV;
    float3 hitRayPosition;
    const bool isIntersect = ExecuteRayMarchInCameraSpace(rayOrigin, rayDirection, hitRayPosition, hitRayUV);
    
     /*-------------------------------------------------------------------
	-        ��ʂ̊O���ɂȂ�قǉ�ʔ����Ɍ������ă��C�g���[�V���O���s����悤�ɂȂ�, 
     �@�@�@�@ ���������ʂɂȂ�Ȃ��̂�, ���˂��O���ɍs���قǔ��˂��キ���� (�r�l�b�g�Ɠ����悤�Ȍ����ł�)
	---------------------------------------------------------------------*/
    const float alpha = CalculateAlpha(isIntersect, hitRayUV, hitRayPosition, rayOrigin);
    
    const float3 beforeColor = BaseColorTexture.Sample(SamplerLinearWrap, input.UV).rgb;
    const float3 afterColor  = BaseColorTexture.Sample(SamplerLinearWrap, hitRayUV).rgb;
    const float3 color = lerp(beforeColor, afterColor, alpha);
    return float4(color,1.0f);
}

/****************************************************************************
*				  			NDCDepthToViewDepth
****************************************************************************/
/* @fn        float NDCDepthToViewDepth(const float z_ndc)

*  @brief     Convert ndc Depth to view space depth

*  @param[in] const float z depth in ndc space.

*  @return    float
*****************************************************************************/
float NDCDepthToViewDepth(const float z_ndc)
{
    // invert the calculation from NDC space to view space for the z-zoordinate. 
    // We have that z_ndc = A + B / viewZ, where gProj[2,2] = A and gProj[3,2] = B. 
    return Projection[3][2] / (z_ndc - Projection[2][2]);
}

/****************************************************************************
*				  			ExecuteRayMarchInCameraSpace
****************************************************************************/
/* @fn        bool ExecuteRayMarchInCameraSpace(const float3 rayOrigin, const float3 rayDirection)

*  @brief     �J������Ԃł̃��C�}�[�`���O�̎��s

*  @param[in] const float3 rayOrigin (�r���[��Ԃł̌��_)
*  @param[in] const float3 rayDirection (�r���[��Ԃł̃��C�̕���)

*  @return    bool
*****************************************************************************/
bool ExecuteRayMarchInCameraSpace(const float3 rayOrigin, const float3 rayDirection, out float3 hitRayPosition, out float2 hitRayUV)
{
    /*-------------------------------------------------------------------
	-        1�C�e���[�V�������Ƃ�depth�̐[�x������ݒ肷��
	---------------------------------------------------------------------*/
    // clipping�̈�͈͓̔�
    const float3 rayEnd       = rayOrigin + rayDirection * MaxRayDistance;
    float3 rayStepDelta = (rayEnd - rayOrigin) / MaxRayMarchIteration;
    
    /*-------------------------------------------------------------------
	-        ���C�}�[�`���O�̎��s (�e�߂ɒT��)
	---------------------------------------------------------------------*/
    float3 rayPosition = rayOrigin;
    bool   isIntersect = false;
    
    for (int i = 0; i < MaxRayMarchIteration; ++i)
    {
        // �Փ˂��Ă�����I��
        if (isIntersect){ break; }

        /*-------------------------------------------------------------------
	    -        �J�n�_���甽�˃x�N�g����1�X�e�b�v���Ƃɐi�߂��ʒu�ɂ�����[�x�����v�Z��, �Փ˔�����s��
	    ---------------------------------------------------------------------*/
        // ���C��i�߂�
        rayPosition += rayStepDelta;
        
        // NDC��Ԃɕϊ���, RayPosition��UV���W���擾����
        const float4 ndcRayPosition = mul(Projection, float4(rayPosition, 1.0f));
        hitRayUV = rayPosition.xy * 0.5f + 0.5f; // -1�`1 -> 0�`1
        
        // �Փ˔���
        isIntersect = IntersectThicknessRay(rayPosition.z, hitRayUV);
    }
    
    /*-------------------------------------------------------------------
	-        �i������̂���, �񕪒T�����s�����ڍׂ�
	---------------------------------------------------------------------*/
    if (isIntersect)
    {
        rayPosition  -= rayStepDelta;             // ��O�ɖ߂�
        rayStepDelta /= (float)MaxBinarySearchIteration; // ����ɃX�e�b�v�����ׂ����ݒ肷��
        
        float strideOrigin = (float) MaxBinarySearchIteration * 0.5f;
        float rayStride    = strideOrigin;
        
        for (int i = 0; i < MaxBinarySearchIteration; ++i)
        {
            rayPosition += rayStepDelta * rayStride; // ���������i�߂�
            
            // NDC��Ԃɕϊ���, RayPosition��UV���W���擾����
            const float4 ndcRayPosition = mul(Projection, float4(rayPosition, 1.0f));
            hitRayUV = rayPosition.xy * 0.5f + 0.5f; // -1�`1 -> 0�`1
            
            // ������΂�蔽�˖ʂɋ߂�����, ������Ȃ���΂�蔽�˖ʂɉ������ɐi�߂�
            strideOrigin *= 0.5f;
            rayStride    = IntersectThicknessRay(rayPosition.z, hitRayUV) ? -strideOrigin : strideOrigin;
        }

    }
    
    hitRayPosition = rayPosition;
    return isIntersect;
}

/****************************************************************************
*				  			CalculateAlpha
****************************************************************************/
/* @fn        float CalculateAlpha(const bool isIntersect, const float2 hitRayUV, const float3 hitRayPosition, const float3 rayOrigin)

*  @brief     ��ʂ̊O���ɂȂ�قǉ�ʊO�Ɍ������ă��C�g���[�V���O���s���, ���������ʂɂȂ�Ȃ�����, ��ʂ̊O���ɍs���ق�SSR�ɂ�锽�˂��キ����

*  @param[in] const bool isIntersect : �Փ˔��肪�������Ă��邩
   @param[in] const float2 hitRayUV  : hit���Ă��郌�C��UV�l
   @param[in] const float3 hitRayPosition : hit����Ray��View���W
   @param[in] const float3 rayOrigin : ray�̌��_

*  @return    float alpha
*****************************************************************************/
float CalculateAlpha(const bool isIntersect, const float2 hitRayUV, const float3 hitRayPosition, const float3 rayOrigin)
{
    if(!isIntersect){ return 0.0f; }
    
    float alpha = 1.0f;
    
    /*-------------------------------------------------------------------
	-        �����t�F�[�h
	---------------------------------------------------------------------*/
    const float rayDistance = length(hitRayPosition - rayOrigin);
    alpha *= saturate(1.0f - rayDistance / MaxFadeDistance);
    
    /*-------------------------------------------------------------------
	-        �X�N���[���̃G�b�W�t�F�[�h
	---------------------------------------------------------------------*/
    const float2 uvCenter   = float2(0.5f, 0.5f);
    
    float2 uvDistance = 2.0f * length(hitRayUV - 0.5f); 
    uvDistance *= uvDistance;
    
    const float screenEdgeFactor = max(0.0f, 1.0f - screenEdgeFactor);
    alpha *= screenEdgeFactor;
    
    return alpha;
}

bool IntersectThicknessRay(const float viewZ, const float2 uv)
{
    const float ndcSceneDepth  = DepthTexture.Sample(SamplerDepth, uv).r; // zBuffer���ŕ`�悳��Ă���I�u�W�F�N�g�̐[�x���
    const float viewSceneDepth = NDCDepthToViewDepth(ndcSceneDepth);      
    const float deltaViewZ     = viewZ - viewSceneDepth; 
    return deltaViewZ < 0.0f && deltaViewZ > -RayThicknessInViewSpace; 
}
#endif