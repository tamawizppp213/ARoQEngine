//////////////////////////////////////////////////////////////////////////////////
///             @file   Keyboard.cpp
///             @brief  Keyboard
///             @author Toide Yutaro
///             @date   2020_11_28
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Core/Include/Camera.hpp"
#include "GameUtility/Base/Include/Screen.hpp"
#include "GameUtility/Base/Include/GameTimer.hpp"
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUResourceView.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gm;
using namespace gc;
using namespace rhi;
using namespace rhi::core;
//////////////////////////////////////////////////////////////////////////////////
//							Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
Camera::Camera()
{

}

Camera::~Camera()
{
	if (_resourceView) { _resourceView.Reset(); }
	if (_sceneConstantBuffer) { _sceneConstantBuffer.Reset(); }
}

Camera::Camera(const LowLevelGraphicsEnginePtr& engine) : _engine(engine), _type(CameraType::Perspective)
{
	assert(("engine is nullptr", _engine));
	assert(("device is nullptr", engine->GetDevice()));

	/*-------------------------------------------------------------------
	-               Set Default lens
	---------------------------------------------------------------------*/
	const auto device = engine->GetDevice();

	SetLens(0.25f * GM_PI, Screen::GetAspectRatio(), 1.0f, 1000.0f);
	/*-------------------------------------------------------------------
	-               Create scece constant buffer and view
	---------------------------------------------------------------------*/
	GPUBufferMetaData metaData = GPUBufferMetaData::ConstantBuffer(sizeof(SceneConstants), 1);
	_sceneConstantBuffer = device->CreateBuffer(metaData, SP("SceneConstants"));

	_resourceView = device->CreateResourceView(ResourceViewType::ConstantBuffer, _sceneConstantBuffer, 0,0,nullptr);
}

Camera::Camera(const LowLevelGraphicsEnginePtr& engine, const PerspectiveInfo& info)
	: _engine(engine), _perspectiveInfo(info), _type(CameraType::Perspective)
{
	assert(("engine is nullptr", _engine));
	assert(("device is nullptr", _engine->GetDevice()));

	/*-------------------------------------------------------------------
	-               Set Perspective Lens
	---------------------------------------------------------------------*/
	const auto device = engine->GetDevice();
	SetLens(info.FovVertical, info.Aspect, info.NearZ, info.FarZ);

	/*-------------------------------------------------------------------
	-               Create scece constant buffer and view
	---------------------------------------------------------------------*/
	GPUBufferMetaData metaData = GPUBufferMetaData::ConstantBuffer(sizeof(SceneConstants), 1);
	_sceneConstantBuffer = device->CreateBuffer(metaData, L"SceneConstants");

	_resourceView = device->CreateResourceView(ResourceViewType::ConstantBuffer, _sceneConstantBuffer,0,0, nullptr);

}

Camera::Camera(const LowLevelGraphicsEnginePtr& engine, const OrthographicInfo& info)
	: _engine(engine), _orthographicInfo(info), _type(CameraType::Orthographic)
{
	assert(("engine is nullptr", _engine));
	assert(("device is nullptr", _engine->GetDevice()));

	/*-------------------------------------------------------------------
	-               Set Perspective Lens
	---------------------------------------------------------------------*/
	const auto device = engine->GetDevice();
	SetOrthoLens(info.Aspect * info.Height, info.Height, info.NearZ, info.FarZ);

	/*-------------------------------------------------------------------
	-               Create scece constant buffer and view
	---------------------------------------------------------------------*/
	GPUBufferMetaData metaData = GPUBufferMetaData::ConstantBuffer(sizeof(SceneConstants), 1);
	_sceneConstantBuffer = device->CreateBuffer(metaData, L"SceneConstants");

	_resourceView = device->CreateResourceView(ResourceViewType::ConstantBuffer, _sceneConstantBuffer, 0,0,nullptr);
}

#pragma endregion Constructor and Destructor

void Camera::Update(const GameTimerPtr& gameTimer)
{
	UpdateViewMatrix();
	UpdateSceneConstants(gameTimer);
}

/****************************************************************************
*                       RotateRoll
*************************************************************************//**
*  @fn        void Camera::RotateRoll(float angle)
* 
*  @brief     Rotate Roll
* 
*  @param[in] float angle

*  @return 　　void
*****************************************************************************/
void Camera::RotateRoll(float angle)
{
	// Rotate up and right vector about the look vector.
	Matrix4 rotate = RotationAxis(_look, angle);

	_up    = TransformNormal(Vector3(_up), rotate).ToFloat3();
	_right = TransformNormal(Vector3(_right), rotate).ToFloat3();

	_viewDirty = true;
}
/****************************************************************************
*                       RotatePitch
*************************************************************************//**
*  @fn        void Camera::RotatePitch(float angle)
* 
*  @brief     Rotate Pitch
* 
*  @param[in] float angle
* 
*  @return 　　void
*****************************************************************************/
void Camera::RotatePitch(float angle)
{
	// Rotate up and look vector about the right vector.
	Matrix4 rotate = RotationAxis(Vector3(_right), angle);

	_up   = TransformNormal(Vector3(_up), rotate).ToFloat3();
	_look = TransformNormal(Vector3(_look), rotate).ToFloat3();

	_viewDirty = true;
}

/****************************************************************************
*                       RotateYaw
*************************************************************************//**
*  @fn        void Camera::RotateYaw(float angle)
* 
*  @brief     Rotate Yaw
* 
*  @param[in] float angle
* 
*  @return 　　void
*****************************************************************************/
void Camera::RotateYaw(float angle)
{
	// Rotate right and look vector about the up vector.
	Matrix4 rotate = RotationAxis(Vector3(_up), angle);

	_right = TransformNormal(Vector3(_right), rotate).ToFloat3();
	_look  = TransformNormal(Vector3(_look), rotate).ToFloat3();

	_viewDirty = true;
}

/****************************************************************************
*                       RotateWorldX
*************************************************************************//**
*  @fn        void Camera::RotateWorldX(float angle)
*  @brief     Rotate X
*  @param[in] float angle
*  @return 　　void
*****************************************************************************/
void Camera::RotateWorldX(float angle)
{
	// Rotate the basis vectors about the world y-axis.

	Matrix4 R = RotationX(angle);

	_right = TransformNormal(Vector3(_right), R).ToFloat3();
	_up    = TransformNormal(Vector3(_up), R).ToFloat3();
	_look  = TransformNormal(Vector3(_look), R).ToFloat3();

	_viewDirty = true;
}
/****************************************************************************
*                       RotateWorldY
*************************************************************************//**
*  @fn        void Camera::RotateWorldY(float angle)
*  @brief     Rotate Y
*  @param[in] float angle
*  @return 　　void
*****************************************************************************/
void Camera::RotateWorldY(float angle)
{
	// Rotate the basis vectors about the world y-axis.

	Matrix4 R = RotationY(angle);

	_right     = TransformNormal(_right, R).ToFloat3();
	_up        = TransformNormal(_up, R).ToFloat3();
	_look      = TransformNormal(Vector3(_look), R).ToFloat3();
	_viewDirty = true;
}

/****************************************************************************
*                       RotateWorldZ
*************************************************************************//**
*  @fn        void Camera::RotateWorldZ(float angle)
*  @brief     Rotate Z
*  @param[in] float angle
*  @return 　　void
*****************************************************************************/
void Camera::RotateWorldZ(float angle)
{
	// Rotate the basis vectors about the world y-axis.

	Matrix4 R = RotationZ(angle);

	_right = TransformNormal(Vector3(_right), R).ToFloat3();
	_up    = TransformNormal(Vector3(_up), R).ToFloat3();
	_look  = TransformNormal(Vector3(_look), R).ToFloat3();

	_viewDirty = true;
}

/****************************************************************************
*                       SetLens
*************************************************************************//**
*  @fn        void Camera::SetLens(const float fovVertical, const float aspect, const float nearZ, const float farZ)
* 
*  @brief     Set perspective lens
* 
*  @param[in] const float fovVertical
*  @param[in] const float aspect
*  @param[in] const float nearZ
*  @param[in] coknst float farZ
* 
*  @return 　　void
*****************************************************************************/
void Camera::SetLens(const float fovVertical, const float aspect, const float nearZ, const float farZ)
{
	assert(("nearZ must be greater than or equal to 0.0f. " ,nearZ >= 0.0f));
	assert(("farZ  must be greater than or equal to 0.0f "  ,farZ >= 0.0f));
	assert(("farZ must be greater than nearZ", farZ > nearZ));
	assert(("aspect must be greater than 0.0f", aspect > 0.0f));

	_type = CameraType::Perspective;

	_perspectiveInfo.FovVertical = fovVertical;
	_perspectiveInfo.Aspect      = aspect;
	_perspectiveInfo.NearZ       = nearZ;
	_perspectiveInfo.FarZ        = farZ;

	_perspectiveInfo.NearWindowHeight = 2.0f * _perspectiveInfo.NearZ * tanf(0.5f * _perspectiveInfo.FovVertical);
	_perspectiveInfo.FarWindowHeight  = 2.0f * _perspectiveInfo.FarZ  * tanf(0.5f * _perspectiveInfo.FovVertical);

	// Perspective Field of View Left-Handed
	Matrix4 P = PerspectiveFovLH(_perspectiveInfo.FovVertical, _perspectiveInfo.Aspect, _perspectiveInfo.NearZ, _perspectiveInfo.FarZ);
	_proj = P.ToFloat4x4();
}

/****************************************************************************
*                       SetOrthoLens
*************************************************************************//**
*  @fn        void Camera::SetOrthoLens(const float width, const float height, const float nearZ, const float farZ)
* 
*  @brief     Set Ortho lens
* 
*  @param[in] const float width
*  @param[in] const float height
*  @param[in] const float nearZ
*  @param[in] const float farZ
* 
*  @return 　　void
*****************************************************************************/
void Camera::SetOrthoLens(const float width, const float height, const float nearZ, const float farZ)
{
	assert(("nearZ must be greater than or equal to 0.0f. " ,nearZ >= 0.0f));
	assert(("farZ  must be greater than or equal to 0.0f "  ,farZ >= 0.0f));
	assert(("farZ must be greater than nearZ" , farZ > nearZ));
	assert(("width must be greater than 0.0f" , width > 0.0f));
	assert(("height must be greater than 0.0f", height > 0.0f));

	_type = CameraType::Orthographic;

	_orthographicInfo.NearZ  = nearZ;
	_orthographicInfo.FarZ   = farZ;
	_orthographicInfo.Aspect = height / width;
	
	// Set orthographics matrix
	Matrix4 P = OrthographicLH(width, height, nearZ, farZ);
	_proj     = P.ToFloat4x4();
}

/****************************************************************************
*                       SetZRange
*************************************************************************//**
*  @fn        void Camera::SetZRange(float nearZ, float farZ)
* 
*  @brief     Set ZRange in the perspective or orthographic camera
* 
*  @param[in] const float nearZ
* 
*  @param[in] const float farZ
* 
*  @return 　　void
*****************************************************************************/
void Camera::SetZRange(const float nearZ, const float farZ)
{
	if (_type == CameraType::Perspective)
	{
		_perspectiveInfo.NearZ = nearZ;
		_perspectiveInfo.FarZ  = farZ;
	}
	else
	{
		_orthographicInfo.NearZ = nearZ;
		_orthographicInfo.FarZ  = farZ;
	}
}

/****************************************************************************
*                       LookAt
*************************************************************************//**
*  @fn        vvoid Camera::LookAt(Vector3 position, Vector3 target, Vector3 worldUp)
*  @brief     LookAt Camera
*  @param[in] Vector3 cameraPosition (world coordinate)
*  @param[in] Vector3 targetPosition (world coordinate)
*  @param[in] Vector3 upVector (0,1,0) default
*  @return 　　void
*****************************************************************************/
void Camera::LookAt(Vector3 position, Vector3 target, Vector3 worldUp)
{
	Vector3 look  = Normalize(target - position); // diff
	Vector3 right = Normalize(Cross(worldUp, look));
	Vector3 up    = Cross(look, right);

	_position = position.ToFloat3();
	_look     = look.ToFloat3();
	_right    = right.ToFloat3();
	_up       = up.ToFloat3();

	_viewDirty = true;

}
/****************************************************************************
*                       LookAt
*************************************************************************//**
*  @fn        void Camera::LookAt(const Float3& position, const Float3& target, const Float3& up)
*  @brief     LookAt Camera
*  @param[in] Float3 cameraPosition (world coordinate)
*  @param[in] Float3 targetPosition (world coordinate)
*  @param[in] Float3 upVector (0,1,0) default
*  @return 　　void
*****************************************************************************/
void Camera::LookAt(const Float3& position, const Float3& target, const Float3& up)
{
	Vector3 P = Vector3(position);
	Vector3 T = Vector3(target);
	Vector3 U = Vector3(up);

	LookAt(P, T, U);

	_viewDirty = true;
}

/****************************************************************************
*                       Strafe
*************************************************************************//**
*  @fn        void Camera::Strafe(float distance)
*  @brief     Strafe camera
*  @param[in] float distance
*  @return 　　void
*****************************************************************************/
void Camera::Strafe(float distance)
{
	/*-------------------------------------------------------------------
	-                  position += d * _right
	---------------------------------------------------------------------*/
	Vector3 s = ReplicateVector3(distance);
	Vector3 r = Vector3(_right);
	Vector3 p = Vector3(_position);
	_position = (s * r + p).ToFloat3();

	_viewDirty = true;
}
/****************************************************************************
*                       Walk
*************************************************************************//**
*  @fn        void Camera::Walk(float distance)
*  @brief     Walk camera
*  @param[in] float distance
*  @return 　　void
*****************************************************************************/
void Camera::Walk(float distance)
{
	/*-------------------------------------------------------------------
	-                   _position += d*_look
	---------------------------------------------------------------------*/
	Vector3 s = ReplicateVector3(distance);
	Vector3 l = Vector3(_look);
	Vector3 p = Vector3(_position);
	_position = (s * l + p).ToFloat3();

	_viewDirty = true;
}

/****************************************************************************
*                       UpdateViewMatrix
*************************************************************************//**
*  @fn        void Camera::UpdateViewMatrix()
* 
*  @brief     Update camera view matrix
* 
*  @param[in] void
* 
*  @return 　　void
*****************************************************************************/
void Camera::UpdateViewMatrix()
{
	if (!_viewDirty) { return; }
	
	auto right    = Vector3(_right);
	auto up       = Vector3(_up);
	auto look     = Vector3(_look);
	auto position = Vector3(_position);

	/*-------------------------------------------------------------------
	-     Keep camera's axes orthogonal to each other and of unit length.
	---------------------------------------------------------------------*/
	look = Normalize(look);
	up   = Normalize(Cross(look, right));

	/*-------------------------------------------------------------------
	-     U, L already ortho-normal, so no need to normalize cross product.
	---------------------------------------------------------------------*/
	right = Cross(up, look);

	/*-------------------------------------------------------------------
	-                 Fill in the view matrix entries
	---------------------------------------------------------------------*/
	float x = -Dot(position, right);
	float y = -Dot(position, up);
	float z = -Dot(position, look);

	_right = right.ToFloat3();
	_up    = up.ToFloat3();
	_look  = look.ToFloat3();

	_view(0, 0) = _right.x;
	_view(1, 0) = _right.y;
	_view(2, 0) = _right.z;
	_view(3, 0) = x;

	_view(0, 1) = _up.x;
	_view(1, 1) = _up.y;
	_view(2, 1) = _up.z;
	_view(3, 1) = y;

	_view(0, 2) = _look.x;
	_view(1, 2) = _look.y;
	_view(2, 2) = _look.z;
	_view(3, 2) = z;

	_view(0, 3) = 0.0f;
	_view(1, 3) = 0.0f;
	_view(2, 3) = 0.0f;
	_view(3, 3) = 1.0f;

	_viewDirty = false;
	
}

#pragma region Property

Vector3 Camera::GetPosition() const
{
	return Vector3(_position);
}

Float3 Camera::GetPosition3f() const
{
	return this->_position;
}

void Camera::SetPosition(const Float3& position)
{
	_position = position;
	_viewDirty = true;
}

void Camera::SetPosition(float x, float y, float z)
{
	_position = Float3(x, y, z);
	_viewDirty = true;
}


Vector3 Camera::GetRight() const
{
	return Vector3(_right);
}

Float3 Camera::GetRight3f() const
{
	return _right;
}

Vector3 Camera::GetUp() const
{
	return Vector3(_up);
}

Float3 Camera::GetUp3f() const
{
	return _up;
}

Vector3 Camera::GetLook() const
{
	return Vector3(_look);
}

Float3 Camera::GetLook3f() const
{
	return _look;
}

float Camera::GetNearZ() const
{
	return _perspectiveInfo.NearZ;
}

float Camera::GetFarZ() const
{
	return _perspectiveInfo.FarZ;
}

float Camera::GetAspect() const
{
	return _perspectiveInfo.Aspect;
}

float Camera::GetFovVertical() const
{
	return _perspectiveInfo.FovVertical;
}

float Camera::GetFovHorizontal() const
{
	float halfWidth = 0.5f * GetNearWindowWidth();
	return 2.0f * atanf(halfWidth / _perspectiveInfo.NearZ);
}

float Camera::GetNearWindowWidth() const
{
	return _perspectiveInfo.Aspect * _perspectiveInfo.NearWindowHeight;
}

float Camera::GetNearWindowHeight() const
{
	return _perspectiveInfo.NearWindowHeight;
}

float Camera::GetFarWindowWidth() const
{
	return _perspectiveInfo.Aspect * _perspectiveInfo.FarWindowHeight;
}

float Camera::GetFarWindowHeight() const
{
	return _perspectiveInfo.FarWindowHeight;
}

Matrix4 Camera::GetViewMatrix() const
{
	return Matrix4(_view);
}

Matrix4 Camera::GetProjectionMatrix() const
{
	return Matrix4(_proj);
}

Float4x4 Camera::GetViewMatrix4x4f() const
{
	return _view;
}

Float4x4 Camera::GetProjectionMatrix4x4f() const
{
	return _proj;
}
#pragma endregion Property

#pragma region Protected Function
void Camera::UpdateSceneConstants(const GameTimerPtr& gameTimer)
{
	SceneConstants scene;

	Matrix4 view                      = GetViewMatrix();
	Matrix4 projection                = GetProjectionMatrix();
	Matrix4 viewProjection            = view * projection; 

	Vector4 viewDeterminant           = Determinant(view);
	Vector4 projectionDeterminant     = Determinant(projection);
	Vector4 viewProjectionDeterminant = Determinant(viewProjection);

	Matrix4 inverseView               = Inverse(viewDeterminant          , view);
	Matrix4 inverseProjection         = Inverse(projectionDeterminant    , projection);
	Matrix4 inverseViewProjection     = Inverse(viewProjectionDeterminant, viewProjection);
	// note: Texture and shadow related features will be added later.

	scene.View                  = view.ToFloat4x4();
	scene.InverseView           = inverseView.ToFloat4x4();
	scene.Projection            = projection .ToFloat4x4();
	scene.InverseProjection     = inverseProjection.ToFloat4x4();
	scene.ViewProjection        = viewProjection.ToFloat4x4();
	scene.InverseViewProjection = inverseViewProjection.ToFloat4x4();

	scene.EyePosition             = GetPosition3f();
	scene.RenderTargetSize        = Float2((float)Screen::GetScreenWidth(), (float)Screen::GetScreenHeight());
	scene.InverseRenderTargetSize = Float2(1.0f / Screen::GetScreenWidth(), 1.0f / Screen::GetScreenHeight());
	scene.NearZ                   = GetNearZ();
	scene.FarZ                    = GetFarZ();
	scene.TotalTime               = gameTimer->TotalTime();
	scene.DeltaTime               = gameTimer->DeltaTime();

	_sceneConstantBuffer->Pack(&scene, nullptr); 
}
#pragma endregion Protected Function