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
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class LowLevelGraphicsEngine;

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
	using PlatformApplicationPtr = std::shared_ptr<platform::core::PlatformApplication>;
	using CoreWindowPtr          = std::shared_ptr<platform::core::CoreWindow>;
	using LowLevelGraphicsEnginePtr = std::shared_ptr<LowLevelGraphicsEngine>;
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	void StartUp(const engine::setting::StartUpParameters& setting);



	void ShutDown();
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/
	LowLevelGraphicsEnginePtr GetLowLevelGraphics() const noexcept { return _graphicsEngine; }
	
	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	PPPEngine() = default;

	virtual ~PPPEngine() = default;

protected:
	/****************************************************************************
	**                Protected Function
	*****************************************************************************/

	/****************************************************************************
	**                Protected Member Variables
	*****************************************************************************/
	/* @brief : ウィンドウを管理するクラス*/
	PlatformApplicationPtr _platformApplication = nullptr;

	/* @brief : メインウィンドウ*/
	CoreWindowPtr _mainWindow = nullptr;

	/* @brief : 描画用の低レイヤー側のエンジン*/
	LowLevelGraphicsEnginePtr _graphicsEngine = nullptr;
};

#endif