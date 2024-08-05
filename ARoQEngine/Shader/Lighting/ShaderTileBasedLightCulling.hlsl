//////////////////////////////////////////////////////////////////////////////////
//              Title:  ShaderLightCulling.hlsl
//            Content:  Shader light culling 
//                      (point light and spot light support.)
//             Author:  Toide Yutaro 
//             Create:  2023_03_12
//////////////////////////////////////////////////////////////////////////////////
// reference: https://github.com/shoeisha-books/hlsl-grimoire-sample/blob/main/Sample_16_04/Sample_16_04/Assets/shader/lightCulling.fx
// reference: https://mobile.sites.google.com/site/monshonosuana/directx%E3%81%AE%E8%A9%B1/directx%E3%81%AE%E8%A9%B1-%E7%AC%AC125%E5%9B%9E

#ifndef SHADER_TILE_BASED_LIGHT_CULLING_HPP
#define SHADER_TILE_BASED_LIGHT_CULLING_HPP
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Lighting/ShaderLightType.hlsli"
#include "../Core/ShaderConstantBuffer3D.hlsli"

//////////////////////////////////////////////////////////////////////////////////
//                             Define
//////////////////////////////////////////////////////////////////////////////////
static const uint TILE_WIDTH  = 16;
static const uint TILE_HEIGHT = 16;
static const uint TILE_SIZE   = TILE_WIDTH * TILE_HEIGHT;

// Currently, the light culling supports only the point lights and spot lights.
Texture2D<float>         DepthTexture      : register(t0);
RWStructuredBuffer<uint> PointLightIndices : register(u0);
RWStructuredBuffer<uint> SpotLightIndices  : register(u1);

// group shared memory
groupshared uint MinDepth;
groupshared uint MaxDepth;
groupshared uint TilePointLightIndices[MAX_POINT_LIGHT];
groupshared uint TileSpotLightIndices[MAX_SPOT_LIGHT];
groupshared uint TileNumPointLights;
groupshared uint TileNumSpotLights;

//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////
/****************************************************************************
*				  		InitializeGroupMemory
****************************************************************************/
/* @class     void InitializeGroupMemory(uint groupIndex)

*  @brief     Initialize Group shared memories.

*  @param[in] const uint groupIndex

*  @return    void
*****************************************************************************/
void InitializeGroupMemory(const uint groupIndex)
{
    if (groupIndex != 0) { return; }

    TileNumPointLights = 0;
    TileNumSpotLights  = 0;
    MinDepth           = 0x7F7FFFFF; // max float value
    MaxDepth           = 0;
}

/****************************************************************************
*				  		ComputePositionInCamera
****************************************************************************/
/* @class     void ComputePositionInCamera(const uint2 globalCoords)
*
*  @brief     Calculate position in camera space.
*
*  @param[in] const uint2 global Coordinate
*
*  @return    float3 positionView
*****************************************************************************/
float3 ComputePositionInCamera(const uint2 globalCoordinate)
{
    float2 st = ((float2) globalCoordinate + 0.5) * rcp(RenderTargetSize);
    st = st * float2(2.0, -2.0) - float2(1.0, -1.0);
    
    float3 screenPosition;
    screenPosition.xy = st.xy;
    screenPosition.z = DepthTexture.Load(uint3(globalCoordinate, 0.0f));
    
    float4 cameraPosition = mul(InverseProjection, float4(screenPosition, 1.0f));
    return cameraPosition.xyz / cameraPosition.w;
}

/****************************************************************************
*				  		GetTileFrustumPlane
****************************************************************************/
/* @class     void GetTileFrustumPlane(out float4 frustumPlanes[6], const uint3 groupID)
*
*  @brief     Calculate the visual frustum plane for eachTile. 
*             カメラからの一個一個のタイルが視錐台になっており, これらとライトとの衝突判定を行う. 
*
*  @param[out] float4 frustumPlanes[6]
*  @param[in]  groupID == tileID
*
*  @return     void
*****************************************************************************/
void GetTileFrustumPlane(out float4 frustumPlanes[6], const uint3 groupID)
{
    // Convert float value (tile's min and max depth)
    const float minTileZ = asfloat(MinDepth);
    const float maxTileZ = asfloat(MaxDepth);
    
    const float2 tileScale = RenderTargetSize * rcp(float(2 * TILE_WIDTH)); // rcp (1/(2 * width))
    const float2 tileBias  = tileScale - float2(groupID.xy);
    
    const float4 c1 = float4(Projection._11 * tileScale.x, 0.0  , tileBias.x, 0.0);
    const float4 c2 = float4(0.0f, -Projection._22 * tileScale.y, tileBias.y, 0.0);
    const float4 c4 = float4(0.0f, 0.0f, 1.0f, 0.0f);
    
    frustumPlanes[0] = c4 - c1; // right
    frustumPlanes[1] = c1;      // left
    frustumPlanes[2] = c4 - c2;  // top
    frustumPlanes[3] = c2;       // bottom
    frustumPlanes[4] = float4(0.0f, 0.0f,  1.0f, -minTileZ);
    frustumPlanes[5] = float4(0.0f, 0.0f, -1.0f,  maxTileZ);

    // 法線が正規化されていない4麺だけ正規化する.
    [unroll]
    for(uint i = 0; i < 4; ++i)
    {
        frustumPlanes[i] *= rcp(length(frustumPlanes[i].xyz));
    }
}

void CreatePointLightIndexArrayInTile(const uint groupIndex, const float4 frustumPlanes[6])
{
    for (uint lightIndex = groupIndex; lightIndex < MAX_POINT_LIGHT; lightIndex += TILE_SIZE)
    {
        PointLight light = PointLights[lightIndex];
        
        /*-------------------------------------------------------------------
	    -        In case of light unused, nothing to do.
	    ---------------------------------------------------------------------*/
        if (!light.IsUse){continue;}

        /*-------------------------------------------------------------------
	    -        Collision detection between frustum and point light.
	    ---------------------------------------------------------------------*/
        bool inFrustum = true;
        
        const float4 positionWorld = float4(mul(World, float4(light.Position, 1.0f)));
        const float4 positionView  = float4(mul(View, positionWorld).xyz, 1.0f);     // 内積用にfloat4にしてる
        for(uint i = 0; i < 6; ++i)
        {
            const float d = dot(frustumPlanes[i], positionView);
            
            // Collision detection using the distance of the light and that of the frustum plane
            inFrustum = inFrustum && (d >= -light.Range);
        }
        
        if (!inFrustum){continue;}
        uint listIndex = 0;
        InterlockedAdd(TileNumPointLights, 1, listIndex);
        TilePointLightIndices[listIndex] = lightIndex;
    }

}

void CreateSpotLightIndexArrayInTile(const uint groupIndex, const float4 frustumPlanes[6])
{
    for (uint lightIndex = groupIndex; lightIndex < MAX_SPOT_LIGHT; lightIndex += TILE_SIZE)
    {
        SpotLight light = SpotLights[lightIndex];
        
        /*-------------------------------------------------------------------
	    -        In case of light unused, nothing to do.
	    ---------------------------------------------------------------------*/
        if (!light.IsUse){continue;}

        /*-------------------------------------------------------------------
	    -        Collision detection between frustum and point light.
	    ---------------------------------------------------------------------*/
        bool inFrustum = true;
        
        const float4 positionView   = float4(mul(View, float4(light.Position, 1.0f)).xyz, 1.0f);     // 内積用にfloat4にしてる
        const float3 directionView  = mul(View, float4(light.Direction, 1.0f)).xyz;
        for(uint i = 0; i < 6; ++i)
        {
            const float d = dot(frustumPlanes[i], positionView);
            
            if (d < 0.0f)
            {
                float t = dot(frustumPlanes[i].xyz, directionView);
                inFrustum = t >= 0.0f;
            }
            
            // Collision detection using the distance of the light and that of the frustum plane
            inFrustum = inFrustum && (d >= -light.Range);
        }
        
        if (!inFrustum){continue;}
        uint listIndex = 0;
        InterlockedAdd(TileNumSpotLights, 1, listIndex);
        TileSpotLightIndices[listIndex] = lightIndex;
    }
}
    
void WriteLightIndexInTileToList(const uint groupIndex, const float2 frameUV)
{
    const uint numCellx  = (RenderTargetSize.x + TILE_WIDTH - 1) / TILE_WIDTH;
    const uint tileIndex = floor(frameUV.x / TILE_WIDTH) + floor(frameUV.y / TILE_HEIGHT) * numCellx;
    
    /*-------------------------------------------------------------------
	-        Output Point light index list
	---------------------------------------------------------------------*/
    const uint pointLightStart = MAX_POINT_LIGHT * tileIndex;
    for(uint pIndex = groupIndex; pIndex < TileNumPointLights; pIndex += TILE_SIZE)
    {
        PointLightIndices[pointLightStart + pIndex] = TilePointLightIndices[pIndex];
    }
    if ((groupIndex == 0) && TileNumPointLights < MAX_POINT_LIGHT)
    {
        PointLightIndices[pointLightStart + TileNumPointLights] = 0xffffffff;
    }
    
    /*-------------------------------------------------------------------
	-        Output Spot light index list
	---------------------------------------------------------------------*/
    const uint spotLightStart = MAX_POINT_LIGHT * tileIndex;
    for(uint sIndex = groupIndex; sIndex < TileNumSpotLights; sIndex += TILE_SIZE)
    {
        SpotLightIndices[pointLightStart + pIndex] = TileSpotLightIndices[pIndex];
    }
    if ((groupIndex == 0) && TileNumSpotLights < MAX_SPOT_LIGHT)
    {
        SpotLightIndices[spotLightStart + TileNumSpotLights] = 0xffffffff;
    }
}


// 各引数の説明: https://learn.microsoft.com/ja-jp/windows/win32/direct3dhlsl/sv-groupid
[numthreads(TILE_WIDTH, TILE_HEIGHT, 1)]
void LightCulling( const uint3 groupID          : SV_GroupID,            // Dispatchによって宣言された範囲内でのグループスレッドのID (各TileIDに相当する)
                   const uint3 dispatchThreadID : SV_DispatchThreadID,   // threadIDをグループスレッドから順に数え上げときのスレッドID (groupID * numThreadCount + threadIDInGroup)
                   const uint3 threadIDInGroup  : SV_GroupThreadID,      // グループスレッド内で並列に実行されるThreadのID
                   const uint  groupIndex       : SV_GroupIndex          // groupThreadID.z * TILE_WIDTH * TILE_HEIGHT + gropuThreadID.y * TILE_WIDTH + groupThreadID.x
)
{
    // Call at once InitializeGroupMemory.
    InitializeGroupMemory(groupIndex);
    
    /*-------------------------------------------------------------------
	-        Get uv and position in view
	---------------------------------------------------------------------*/
    const uint2 frameUV       = dispatchThreadID.xy;
    const float3 viewPosition = ComputePositionInCamera(frameUV);
    
    /*-------------------------------------------------------------------
	-        Get MaxDepth and MinDepth
	---------------------------------------------------------------------*/
    GroupMemoryBarrierWithGroupSync();
    if(threadIDInGroup.x <= RenderTargetSize.x && threadIDInGroup.y <= RenderTargetSize.y)
    {
        InterlockedMin(MinDepth, asuint(viewPosition.z));
        InterlockedMax(MaxDepth, asuint(viewPosition.z));
    }
    GroupMemoryBarrierWithGroupSync();
    
    /*-------------------------------------------------------------------
	-        Create Tile Index array list (point light and spot light)
	---------------------------------------------------------------------*/
    if (threadIDInGroup.x <= RenderTargetSize.x && threadIDInGroup.y <= RenderTargetSize.y)
    {
        // Get six planes coefficients in plane equation, which constructs frustum.
        float4 frustumPlanes[6];
        GetTileFrustumPlane(frustumPlanes, groupID);
        
        // CollisionTest each light.
        CreatePointLightIndexArrayInTile(groupIndex, frustumPlanes);
        CreateSpotLightIndexArrayInTile (groupIndex, frustumPlanes);
    }
    // Execute all thread collisition test
    GroupMemoryBarrierWithGroupSync();
    
    /*-------------------------------------------------------------------
	-        Output light list.
	---------------------------------------------------------------------*/
    if (threadIDInGroup.x <= RenderTargetSize.x && threadIDInGroup.y <= RenderTargetSize.y)
    {
        WriteLightIndexInTileToList(groupIndex, frameUV);
    }

}
#endif