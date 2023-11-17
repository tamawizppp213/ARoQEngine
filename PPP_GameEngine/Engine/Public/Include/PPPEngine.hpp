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
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include "GameUtility/Base/Include/GUSharedPointer.hpp"
#include <memory>
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
*  @brief     各分野のEngineをまとめたクラス
*****************************************************************************/
class PPPEngine : public NonCopyable
{
protected:
	using PlatformApplicationPtr    = gu::SharedPointer<platform::core::PlatformApplication>;
	using CoreWindowPtr             = gu::SharedPointer<platform::core::CoreWindow>;
	using PlatformCommandPtr        = gu::SharedPointer<platform::core::PlatformCommand>;
	using EngineThreadManagerPtr    = std::shared_ptr<engine::core::EngineThreadManager>;
	using GameTimerPtr              = std::shared_ptr<GameTimer>;
	using LowLevelGraphicsEnginePtr = std::shared_ptr<LowLevelGraphicsEngine>;
	using RenderPipelinePtr         = std::shared_ptr<gc::IRenderPipeline>;
	using ResourceViewPtr           = std::shared_ptr<rhi::core::GPUResourceView>;
	using CameraPtr                 = std::shared_ptr<gc::Camera>;
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	void StartUp(const engine::setting::StartUpParameters& setting);

	void ExecuteMainThread();
	void ExecuteUpdateThread();
	void ExecuteRenderThread();

	void ShutDown();
	/****************************************************************************
	**                Public Member Variables
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
	**                Protected Member Variables
	*****************************************************************************/
#pragma region Variables
#pragma region Common 
	/* @brief : ウィンドウを管理するクラス*/
	gu::SharedPointer<platform::core::PlatformApplication>  _platformApplication = nullptr;

	/* @brief : メインウィンドウ*/
	CoreWindowPtr _mainWindow = nullptr;

	/* @brief : プラットフォームのコマンドをまとめたクラス*/
	PlatformCommandPtr _platformCommand = nullptr;

	/* @brief : エンジンのスレッドを管理するクラス*/
	EngineThreadManagerPtr _engineThreadManager = nullptr;

	/* @brief : 全てのスレッドが停止要求を行ったか*/
	std::atomic_bool _isStoppedAllThreads = false;

	bool _hasShutdown = false;
#pragma endregion      Common

#pragma region Main Thread
	/* @brief : メインスレッドのタイマー*/
	GameTimerPtr _mainThreadTimer = nullptr;

	/* @brief : ポーズ処理を走らせるか*/
	bool _isApplicationPaused = false;
#pragma endregion Main Thread

#pragma region Render Thread
	/* @brief : 描画用の低レイヤー側のエンジン*/
	LowLevelGraphicsEnginePtr _graphicsEngine = nullptr;

	/* @brief : 描画パイプライン. デフォルトではURPを使用します.*/
	RenderPipelinePtr _renderPipeline = nullptr;

#pragma endregion Render Thread
#pragma endregion Variables
};

#endif