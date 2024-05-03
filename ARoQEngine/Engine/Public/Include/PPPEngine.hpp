//////////////////////////////////////////////////////////////////////////////////
///             @file   PPPEngine.hpp
///             @brief  Engine
///             @author Toide Yutaro
///             @date   2023_09_24
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef PPP_ENGINE_HPP
#define PPP_ENGINE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "EngineProfiles.hpp"
#include "EngineThreadManager.hpp"
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class LowLevelGraphicsEngine;
class GameTimer;
namespace rhi::core
{
	class GPUResourceView;
}
namespace gc
{
	class IRenderPipeline;
	class Camera;
}
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

/****************************************************************************
*				  			    PPPEngine
*************************************************************************//**
*  @class     PPPEngine
*  @brief     �e�����Engine���܂Ƃ߂��N���X
*****************************************************************************/
class PPPEngine : public gu::NonCopyable
{
protected:
	using PlatformApplicationPtr    = gu::SharedPointer<platform::core::PlatformApplication>;
	using CoreWindowPtr             = gu::SharedPointer<platform::core::CoreWindow>;
	using PlatformCommandPtr        = gu::SharedPointer<platform::core::PlatformCommand>;
	using EngineThreadManagerPtr    = gu::SharedPointer<engine::core::EngineThreadManager>;
	using GameTimerPtr              = gu::SharedPointer<GameTimer>;
	using LowLevelGraphicsEnginePtr = gu::SharedPointer<LowLevelGraphicsEngine>;
	using RenderPipelinePtr         = gu::SharedPointer<gc::IRenderPipeline>;
	using ResourceViewPtr           = gu::SharedPointer<rhi::core::GPUResourceView>;
	using CameraPtr                 = gu::SharedPointer<gc::Camera>;
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	void StartUp(const engine::setting::StartUpParameters& setting);

	void Run();

	void ExecuteMainThread();
	void ExecuteUpdateThread();
	void ExecuteRenderThread();

	void ShutDown();
	/****************************************************************************
	**                Public Property
	*****************************************************************************/
	LowLevelGraphicsEnginePtr GetLowLevelGraphics() const noexcept { return _graphicsEngine; }
	
	gu::SharedPointer<platform::core::PlatformApplication> GetPlatformApplication() const noexcept { return _platformApplication; }

	CoreWindowPtr GetWindow() const noexcept { return _mainWindow; }

	PlatformCommandPtr GetPlatformCommand() const noexcept { return _platformCommand; }

	GameTimerPtr GetMainThreadTimer() const noexcept { return _mainThreadTimer; }

	RenderPipelinePtr GetRenderPipeline() const noexcept { return _renderPipeline; }

	void SetRenderingPipeline(const RenderPipelinePtr& pipeline) { _renderPipeline = pipeline; }

	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	PPPEngine() = default;

	virtual ~PPPEngine();

protected:
	/****************************************************************************
	**                Protected Function
	*****************************************************************************/

	/****************************************************************************
	**                Protected Property
	*****************************************************************************/
#pragma region Variables
#pragma region Common 
	// @ brief : �����ݒ�
	engine::setting::StartUpParameters StartUpParameter = {};

	/* @brief : �E�B���h�E���Ǘ�����N���X*/
	gu::SharedPointer<platform::core::PlatformApplication>  _platformApplication = nullptr;

	/* @brief : ���C���E�B���h�E*/
	CoreWindowPtr _mainWindow = nullptr;

	/* @brief : �v���b�g�t�H�[���̃R�}���h���܂Ƃ߂��N���X*/
	PlatformCommandPtr _platformCommand = nullptr;

	/* @brief : �G���W���̃X���b�h���Ǘ�����N���X*/
	EngineThreadManagerPtr _engineThreadManager = nullptr;

	/* @brief : �S�ẴX���b�h����~�v�����s������*/
	std::atomic_bool _isStoppedAllThreads = false;

	bool _hasShutdown = false;
#pragma endregion      Common

#pragma region Main Thread
	/* @brief : ���C���X���b�h�̃^�C�}�[*/
	GameTimerPtr _mainThreadTimer = nullptr;

	/* @brief : �|�[�Y�����𑖂点�邩*/
	bool _isApplicationPaused = false;
#pragma endregion Main Thread

#pragma region Render Thread
	// @brief : �`��X���b�h�̃Q�[���^�C�}�[
	GameTimerPtr _renderThreadTimer = nullptr;

	/* @brief : �`��p�̒჌�C���[���̃G���W��*/
	LowLevelGraphicsEnginePtr _graphicsEngine = nullptr;

	/* @brief : �`��p�C�v���C��. �f�t�H���g�ł�URP���g�p���܂�.*/
	RenderPipelinePtr _renderPipeline = nullptr;

#pragma endregion Render Thread
#pragma endregion Variables
};

#endif