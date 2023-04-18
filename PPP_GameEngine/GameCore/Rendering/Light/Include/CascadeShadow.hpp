//////////////////////////////////////////////////////////////////////////////////
///             @file   CascadeShadow.hpp
///             @brief  CascadeShadow
///             @author Toide Yutaro
///             @date   2023_04_06
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef CASCADE_SHADOW_HPP
#define CASCADE_SHADOW_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHITypeCore.hpp"
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include <string>
#include <vector>
#include "GameUtility/Math/Include/GMMatrix.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class LowLevelGraphicsEngine;
class GameTimer;
namespace gc
{
	class Camera;
}
namespace gc::core
{
	class GameModel;
	class GameWorldInfo;
}
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc::rendering
{
	class ShadowMap;

	struct CascadeShadowDesc
	{
		float Near    = 200.0f;
		float Medium  = 500.0f;
		float Far     = 1000.0f;
		std::uint32_t MaxResolution = 1024;
		bool UseSoftShadow = true;
	};

	struct CascadeShadowInfo
	{
		gm::Float4x4 LVPC[3];
		int IsSoftShadow;
	};

	/****************************************************************************
	*				  			    Class
	*************************************************************************//**
	*  @class     CascadeShadow
	*  @brief     Cascade shadow map + soft shadow
	*****************************************************************************/
	class CascadeShadow : public NonCopyable
	{
	protected:
		using LowLevelGraphicsEnginePtr = std::shared_ptr<LowLevelGraphicsEngine>;
		using GameModelPtr              = std::shared_ptr<gc::core::GameModel>;
		using CameraPtr                 = std::shared_ptr<gc::Camera>;
		using ShadowMapPtr              = std::shared_ptr<ShadowMap>;
		using GameWorldInfoPtr          = std::shared_ptr<gc::core::GameWorldInfo>;

	public:

		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Directional Light��z�肵��Shadow�̕`��*/
		void Draw(const std::shared_ptr<GameTimer>& gameTimer, const gm::Float3& direction);

		void Add(const GameModelPtr& gameMode);


		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		ResourceViewPtr GetShadowInfoView() const noexcept { return _shadowInfoView; }



		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		CascadeShadow(const LowLevelGraphicsEnginePtr& engine, const CascadeShadowDesc& desc, const std::wstring& addName = L"");

		~CascadeShadow();

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void PrepareResourceView(const std::wstring& name);

		void Update(const std::shared_ptr<GameTimer>& gameTimer, const gm::Float3& direction);

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		LowLevelGraphicsEnginePtr _engine = nullptr;

		std::vector<GameModelPtr> _gameModels = {};

		// GPU binding resource
		ResourceLayoutPtr   _resourceLayout = nullptr;
		GraphicsPipelinePtr _pipeline       = nullptr;
		ResourceViewPtr     _shadowInfoView = nullptr;

		// Camera game world position
		CameraPtr        _lightCamera   = nullptr;

		// shadowMap
		std::vector<ShadowMapPtr> _shadowMaps = {};

		CascadeShadowDesc _shadowDesc = {};

		static constexpr size_t SHADOW_MAP_COUNT = 3;
	};
}
#endif