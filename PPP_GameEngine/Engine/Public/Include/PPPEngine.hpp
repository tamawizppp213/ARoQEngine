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
*  @brief     �e�����Engine���܂Ƃ߂��N���X
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
	/* @brief : �E�B���h�E���Ǘ�����N���X*/
	PlatformApplicationPtr _platformApplication = nullptr;

	/* @brief : ���C���E�B���h�E*/
	CoreWindowPtr _mainWindow = nullptr;

	/* @brief : �`��p�̒჌�C���[���̃G���W��*/
	LowLevelGraphicsEnginePtr _graphicsEngine = nullptr;
};

#endif