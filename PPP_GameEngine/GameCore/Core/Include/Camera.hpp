//////////////////////////////////////////////////////////////////////////////////
///             @file   Camera.hpp
///             @brief  Camera Class
///             @author Toide Yutaro
///             @date   2020_11_
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef CAMERA_HPP
#define CAMERA_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include "GameUtility/Math/Include/GMMatrix.hpp"
#include "GameUtility/Base/Include/GUMemory.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class GameTimer;
class LowLevelGraphicsEngine;
namespace rhi::core
{
	class GPUBuffer;
	class GPUResourceView;
}
//////////////////////////////////////////////////////////////////////////////////
//								Camera 
//////////////////////////////////////////////////////////////////////////////////
namespace gc
{

	/****************************************************************************
	*				  			PerspectiveInfo
	*************************************************************************//**
	*  @struct    PerspectiveInfo
	*  @brief     Perspective camera
	*****************************************************************************/
	struct PerspectiveInfo
	{
		float NearZ;
		float FarZ;
		float Aspect;
		float FovVertical; // angle
		float NearWindowHeight;
		float FarWindowHeight;
	};

	/****************************************************************************
	*				  			OrthographicInfo
	*************************************************************************//**
	*  @struct    OrthographicInfo
	*  @brief     Orthographic camera
	*****************************************************************************/
	struct OrthographicInfo
	{
		float NearZ;
		float FarZ;
		float Height;
		float Aspect;
	};

	enum class CameraType
	{
		Perspective,
		Orthographic,
	};

	/****************************************************************************
	*				  			Camera
	*************************************************************************//**
	*  @class     Camera
	*  @brief     Camera
	*****************************************************************************/
	class Camera : public Copyable
	{
		struct SceneConstants
		{
			gm::Float4x4 View                    = gm::MatrixIdentityF();
			gm::Float4x4 InverseView             = gm::MatrixIdentityF();
			gm::Float4x4 Projection              = gm::MatrixIdentityF();
			gm::Float4x4 InverseProjection       = gm::MatrixIdentityF();
			gm::Float4x4 ViewProjection          = gm::MatrixIdentityF();
			gm::Float4x4 InverseViewProjection   = gm::MatrixIdentityF();
			gm::Float3   EyePosition             = { 0.0f, 0.0f, 0.0f };
			float        cbPerObjectPad1         = 0.0f;
			gm::Float2   RenderTargetSize        = { 0.0f, 0.0f };
			gm::Float2   InverseRenderTargetSize = { 0.0f, 0.0f };

			float  NearZ = 0.0f;
			float  FarZ = 0.0f;
			float  TotalTime = 0.0f;
			float  DeltaTime = 0.0f;
		};

		using LowLevelGraphicsEnginePtr = gu::SharedPointer<LowLevelGraphicsEngine>;
		using SceneConstantBufferPtr    = gu::SharedPointer<rhi::core::GPUBuffer>;
		using GPUResourceViewPtr        = gu::SharedPointer<rhi::core::GPUResourceView>;
		using GameTimerPtr              = gu::SharedPointer<GameTimer>;

	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Update view materix and scene constants buffer.
		            Please call at a frame*/
		void Update(const GameTimerPtr& gameTimer);

		// Define camera space via LookAt parameters.
		void LookAt(gm::Vector3 position, gm::Vector3 target, gm::Vector3 worldUp);
		
		void LookAt(const gm::Float3& position, const gm::Float3& target, const gm::Float3& up);

		// Camera Motion
		void Strafe(float distance);
		void Walk  (float distance);

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		SceneConstantBufferPtr GetSceneBuffer() const { return _sceneConstantBuffer; }
		
		GPUResourceViewPtr GetResourceView() const { return _resourceView; }
		
		/*-------------------------------------------------------------------
		-    World Camera Position
		---------------------------------------------------------------------*/
		gm::Vector3 GetPosition()   const;
		gm::Float3  GetPosition3f() const;
		
		void SetPosition(const gm::Float3& position);
		void SetPosition(float x, float y, float z);

		/*-------------------------------------------------------------------
		-   Rotate the camera
		---------------------------------------------------------------------*/
		void RotateRoll(float angle);
		void RotatePitch(float angle);
		void RotateYaw(float angle);
		void RotateWorldX(float angle);
		void RotateWorldY(float angle);
		void RotateWorldZ(float angle);

		/*-------------------------------------------------------------------
		-   Get camera basis vectors
		---------------------------------------------------------------------*/
		gm::Vector3 GetRight()   const;
		gm::Float3  GetRight3f() const;
		gm::Vector3 GetUp()      const;
		gm::Float3  GetUp3f()    const;
		gm::Vector3 GetLook()    const;
		gm::Float3  GetLook3f()  const;

		/*-------------------------------------------------------------------
		-   Get frustum properties
		---------------------------------------------------------------------*/
		PerspectiveInfo GetPerspectiveInfo() const { return _perspectiveInfo; }
		OrthographicInfo GetOrthographicsInfo() const { return _orthographicInfo; }
		float GetNearZ()         const;
		float GetFarZ()          const;
		float GetAspect()        const;
		float GetFovVertical()   const;
		float GetFovHorizontal() const;

		/*-------------------------------------------------------------------
		-   Get near and far plane dimensions in view space coordinates
		---------------------------------------------------------------------*/
		float GetNearWindowWidth()  const;
		float GetNearWindowHeight() const;
		float GetFarWindowWidth()   const;
		float GetFarWindowHeight()  const;

		/*-------------------------------------------------------------------
		-               Get View / Projection Matrix
		---------------------------------------------------------------------*/
		gm::Matrix4   GetViewMatrix()           const;
		gm::Matrix4   GetProjectionMatrix()     const;
		gm::Float4x4  GetViewMatrix4x4f()       const;
		gm::Float4x4  GetProjectionMatrix4x4f() const;

		/*-------------------------------------------------------------------
		-               Set Frustum lens
		---------------------------------------------------------------------*/
		/* @brief : Set camera (perspective or orthographic) lens*/
		void SetLens     (const float fovVertical, const float aspect, const float nearZ, const float farZ);
		void SetOrthoLens(const float width      , const float height, const float nearZ, const float farZ);
		
		/* @brief : Set near and far depth in the perspective or orthographic camera*/
		void SetZRange(const float nearZ, const float farZ);

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Camera();

		explicit Camera(const LowLevelGraphicsEnginePtr& engine);

		Camera(const LowLevelGraphicsEnginePtr& engine, const PerspectiveInfo& info);

		Camera(const LowLevelGraphicsEnginePtr& engine, const OrthographicInfo& info);
		
		~Camera();
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void UpdateSceneConstants(const GameTimerPtr& gameTimer);
		// After modifying camera position / orientation, call to rebuild the matrix.
		void UpdateViewMatrix();
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/* @brief : Use perspective camera or orthographic camera*/
		CameraType _type = CameraType::Perspective;
		
		/* @brief : Camera world position*/
		gm::Float3 _position = { 0.0f, 0.0f, 0.0f };

		/* @brief : Camera orientation*/
		gm::Float3 _right    = { 1.0f, 0.0f, 0.0f };
		gm::Float3 _up       = { 0.0f, 1.0f, 0.0f };
		gm::Float3 _look     = { 0.0f, 0.0f, 1.0f };

		/* @brief : Camera view and projection matrix*/
		gm::Float4x4 _view = gm::MatrixIdentityF(); // world space -> camera's origin view space
		gm::Float4x4 _proj = gm::MatrixIdentityF(); // camera view space -> clip space (x, y : -1Å`1, z : 0Å`1)

		/* @brief : Camera lens information*/
		PerspectiveInfo  _perspectiveInfo = {};
		OrthographicInfo _orthographicInfo = {};

		// camera supdate flag
		bool _viewDirty = true;

		/* @brief : GPU resources*/
		SceneConstantBufferPtr _sceneConstantBuffer = nullptr;
		GPUResourceViewPtr     _resourceView        = nullptr;

		LowLevelGraphicsEnginePtr _engine;
	};
}
#endif
