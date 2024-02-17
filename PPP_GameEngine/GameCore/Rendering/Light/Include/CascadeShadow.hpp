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
#include "GameUtility/Base/Include/GUString.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
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
		using LowLevelGraphicsEnginePtr = gu::SharedPointer<LowLevelGraphicsEngine>;
		using GameModelPtr              = gu::SharedPointer<gc::core::GameModel>;
		using CameraPtr                 = gu::SharedPointer<gc::Camera>;
		using ShadowMapPtr              = gu::SharedPointer<ShadowMap>;
		using GameWorldInfoPtr          = gu::SharedPointer<gc::core::GameWorldInfo>;

	public:

		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Directional Light‚ð‘z’è‚µ‚½Shadow‚Ì•`‰æ*/
		void Draw(const gu::SharedPointer<GameTimer>& gameTimer, const gm::Float3& direction);

		void Add(const GameModelPtr& gameMode);


		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		ResourceViewPtr GetShadowInfoView() const noexcept { return _shadowInfoView; }



		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		CascadeShadow(const LowLevelGraphicsEnginePtr& engine, const CascadeShadowDesc& desc, const gu::tstring& addName = SP(""));

		~CascadeShadow();

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void PrepareResourceView(const gu::tstring& name);

		void Update(const gu::SharedPointer<GameTimer>& gameTimer, const gm::Float3& direction);

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		LowLevelGraphicsEnginePtr _engine = nullptr;

		gu::DynamicArray<GameModelPtr> _gameModels = {};

		// GPU binding resource
		ResourceLayoutPtr   _resourceLayout = nullptr;
		GraphicsPipelinePtr _pipeline       = nullptr;
		ResourceViewPtr     _shadowInfoView = nullptr;

		// Camera game world position
		CameraPtr        _lightCamera   = nullptr;

		// shadowMap
		gu::DynamicArray<ShadowMapPtr> _shadowMaps = {};

		CascadeShadowDesc _shadowDesc = {};

		static constexpr size_t SHADOW_MAP_COUNT = 3;
	};
}
#endif