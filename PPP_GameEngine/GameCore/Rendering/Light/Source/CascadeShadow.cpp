//////////////////////////////////////////////////////////////////////////////////
//              @file   a.cpp
///             @brief  a
///             @author Toide Yutaro
///             @date   -
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/CascadeShadow.hpp"
#include "../Include/ShadowMap.hpp"
#include "../GameCore/Core/Include/Camera.hpp"
#include "../GameCore/Core/Include/GameWorldInfo.hpp"
#include "../../Model/Include/GameModel.hpp"
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIFrameBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUResourceView.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineState.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUPipelineFactory.hpp"
#include "GameUtility/Base/Include/Screen.hpp"
#include <iostream>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gc::core;
using namespace gc::rendering;
using namespace rhi::core;
using namespace gm;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
CascadeShadow::CascadeShadow(const LowLevelGraphicsEnginePtr& engine, const CascadeShadowDesc& desc, const std::wstring& addName)
	: _engine(engine), _shadowDesc(desc)
{
	/*-------------------------------------------------------------------
	-            Error log
	---------------------------------------------------------------------*/
	assert(("engine is nullptr", _engine));
	assert(("Near must not be less than 0"  , desc.Near   >= 0.0f));
	assert(("Medium must not be less than 0", desc.Medium >= 0.0f));
	assert(("Far must not be less than 0"   , desc.Far    >= 0.0f));
	assert(("The further away you go, the greater the value.", desc.Near < desc.Medium && desc.Medium < desc.Far));

	/*-------------------------------------------------------------------
	-            Set debug name
	---------------------------------------------------------------------*/
	std::wstring name = L""; if (name != L"") { name += addName; name += L"::"; }
	name += L"CascadeShadow::";

	/*-------------------------------------------------------------------
	-        Shadow map
	---------------------------------------------------------------------*/
	_lightCamera = std::make_shared<gc::Camera>(_engine);
	_shadowMaps.resize(SHADOW_MAP_COUNT);
	for (size_t i = 0; i < _shadowMaps.size(); ++i)
	{
		_shadowMaps[i] = std::make_shared<rendering::ShadowMap>(_engine, desc.MaxResolution / pow(2, i), desc.MaxResolution / pow(2,i));
	}

	PrepareResourceView (name);

}

CascadeShadow::~CascadeShadow()
{

}

#pragma endregion Constructor and Destructor

#pragma region Main Function
/****************************************************************************
*							Draw
*************************************************************************//**
*  @fn        void CascadeShadow::Draw()
* 
*  @brief     Draw shadow map and draw shadow
* 
*  @param[in] void
* 
*  @return 　　void
*****************************************************************************/
void CascadeShadow::Draw(const std::shared_ptr<GameTimer>& gameTimer,const gm::Float3& direction)
{
#ifdef _DEBUG
	assert(_engine);
#endif

	/*-------------------------------------------------------------------
	-               Update light camera
	---------------------------------------------------------------------*/
	Update(gameTimer, direction);

	/*-------------------------------------------------------------------
	-               Draw shadow maps of the each resolution
	---------------------------------------------------------------------*/
	for (const auto shadowMap : _shadowMaps)
	{
		// shadow map + gaussian blur
		shadowMap->Draw(_lightCamera->GetResourceView());
	}

}

/****************************************************************************
*							Add
*************************************************************************//**
*  @fn        void CascadeShadow::Add(const GameModelPtr& gameModel)
*
*  @brief     Add game models for the rendering shadow
*
*  @param[in] void
*
*  @return 　　void
*****************************************************************************/
void CascadeShadow::Add(const GameModelPtr& gameModel)
{
	if (!gameModel) { return; }

	for (const auto shadowMap : _shadowMaps)
	{
		shadowMap->Add(gameModel);
	}

	_gameModels.push_back(gameModel);
}
#pragma endregion Main Function

#pragma region SetUp Function
void CascadeShadow::PrepareResourceView(const std::wstring& name)
{
	const auto device = _engine->GetDevice();

	/*-------------------------------------------------------------------
	-        Shadow info
	---------------------------------------------------------------------*/
	{
		const CascadeShadowInfo shadowInfo =
		{
			{MatrixIdentityF(), MatrixIdentityF(), MatrixIdentityF()},
			_shadowDesc.UseSoftShadow
		};

		const auto metaData = GPUBufferMetaData::ConstantBuffer(sizeof(CascadeShadowInfo), 1, MemoryHeap::Upload, ResourceState::Common);
		const auto buffer   = device->CreateBuffer(metaData);
		buffer->SetName(name + L"ShadowInfo");
		buffer->Pack(&shadowInfo, nullptr);
		_shadowInfoView = device->CreateResourceView(ResourceViewType::Buffer, buffer, nullptr);
	}
}

/****************************************************************************
*						UpdateLightCamera
*************************************************************************//**
*  @fn        void CascadeShadow::Update()
*
*  @brief     Move the directional light camera
*
*  @param[in] const std::shared_ptr<GameTimer>& gameTimer
*
*  @return 　　void
*****************************************************************************/
void CascadeShadow::Update(const std::shared_ptr<GameTimer>& gameTimer, const gm::Float3& direction)
{
	/*-------------------------------------------------------------------
	-              Update the light camera
	-              Calculate the light view projection matrix.
	---------------------------------------------------------------------*/
	// camera update (とりあえずDirectional Light 想定で行ってます)
	const auto cameraPosition = Float3(-direction.x, -direction.y, -direction.z);
	_lightCamera->SetOrthoLens(Screen::GetScreenWidth(), Screen::GetScreenHeight(), _lightCamera->GetNearZ(), _lightCamera->GetFarZ());
	_lightCamera->LookAt(cameraPosition, Float3(0, 0, 0), _lightCamera->GetUp3f());
	_lightCamera->Update(gameTimer);

	// Acquire the camera forward, right, and up vector
	const auto lvpMatrix = _lightCamera->GetViewMatrix() * _lightCamera->GetProjectionMatrix();
	const auto forward = _lightCamera->GetLook();
	const auto right   = _lightCamera->GetRight();
	const auto up      = _lightCamera->GetUp();

	const float depthList[] = { _shadowDesc.Near, _shadowDesc.Medium, _shadowDesc.Far };
	Matrix4 lvpcMatrices[SHADOW_MAP_COUNT];

	/*-------------------------------------------------------------------
	-              Calculate the each frustum vertex 
	---------------------------------------------------------------------*/
	float nearDepth = _lightCamera->GetNearZ();
	for (int areaNo = 0; areaNo < SHADOW_MAP_COUNT; ++areaNo)
	{
		const auto frustum = _lightCamera->GetFrustum();

		// acquire the camera information
		const auto nearY        = tanf(frustum.FovVertical) * nearDepth;
		const auto nearX        = nearY * frustum.Aspect;
		const auto farY         = tanf(frustum.FovVertical) * depthList[areaNo];
		const auto farX         = farY * frustum.Aspect;
		const auto nearPosition = _lightCamera->GetPosition() + forward * nearDepth;         // the rectangle center position of the near surface
		const auto farPosition  = _lightCamera->GetPosition() + forward * depthList[areaNo]; // the rectangle center position of the far  surface.

		// calculate the 8 vertex position list.
		gm::Float3 vertices[8];
		vertices[0] = (nearPosition + up * (+nearY) + right * (+nearX)).ToFloat3(); // near right upper
		vertices[1] = (nearPosition + up * (+nearY) + right * (-nearX)).ToFloat3(); // near  left upper
		vertices[2] = (nearPosition + up * (-nearY) + right * (+nearX)).ToFloat3(); // near right lower
		vertices[3] = (nearPosition + up * (-nearY) + right * (-nearX)).ToFloat3(); // near  left lower
		vertices[4] = (farPosition  + up * (+farY ) + right * (+farX )).ToFloat3(); // far  right upper
		vertices[5] = (farPosition  + up * (+farY ) + right * (-farX )).ToFloat3(); // far   left upper
		vertices[6] = (farPosition  + up * (-farY ) + right * (+farX )).ToFloat3(); // far  right lower
		vertices[7] = (farPosition  + up * (-farY ) + right * (-farX )).ToFloat3(); // far   left lower
		
		// covert the world space to the light view projection matrix
		gm::Vector3 vMax, vMin;
		vMax = gm::Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
		vMin = gm::Vector3(FLT_MAX, FLT_MAX, FLT_MAX);
		for (auto& vertex : vertices)
		{
			vertex = (lvpMatrix * vertex).GetXYZ().ToFloat3();

			// calculate the AABB min max range.
			gm::Max(vMax, vertex);
			gm::Min(vMin, vertex);
		}

		// calculate the crop matrix
		// The crop matrix is the matrix to pack the range from -1 to 1.
		const float xScale = 2.0f / (vMax.GetX() - vMin.GetX());
		const float yScale = 2.0f / (vMax.GetY() - vMin.GetY());
		const float xOffset = (vMax.GetX() + vMin.GetX()) * (-0.5f) * xScale;
		const float yOffset = (vMax.GetY() + vMin.GetY()) * (-0.5f) * yScale;
		
		auto clopMatrix = MatrixIdentity();
		clopMatrix.GetX().SetX(xScale);
		clopMatrix.GetY().SetY(yScale);
		clopMatrix.GetW().SetX(xOffset);
		clopMatrix.GetW().SetY(yOffset);

		// Calculate the LVPC matrix
		lvpcMatrices[areaNo] = lvpMatrix * clopMatrix;

		// update the near depth
		nearDepth = depthList[areaNo];
	}

	/*-------------------------------------------------------------------
	-              Update lvpc matrix buffer
	---------------------------------------------------------------------*/
	CascadeShadowInfo shadowInfo;
	shadowInfo.IsSoftShadow = _shadowDesc.UseSoftShadow;
	for (int i = 0; i < _countof(shadowInfo.LVPC); ++i)
	{
		shadowInfo.LVPC[i] = lvpcMatrices[i].ToFloat4x4();
	}

	_shadowInfoView->GetBuffer()->Update(&shadowInfo, 1);
}
#pragma endregion SetUp Function