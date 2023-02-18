//////////////////////////////////////////////////////////////////////////////////
///             @file   BasePassLightCulling.hpp
///             @brief  BasePass Light Culling (scene point light and spot light)
///             @author Toide Yutaro
///             @date   2022_02_14 
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef BASE_PASS_LIGHT_CULLING_HPP
#define BASE_PASS_LIGHT_CULLING_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include <memory>
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class LowLevelGraphicsEngine;
namespace rhi::core
{
	class GPUBuffer;
	class GPUResourceView;
	class RHIResourceLayout;
	class GPUComputePipelineState;
}

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc::basepass
{
	class ZPrepass;
	
	enum CullingLightType
	{
		PointLight,
		SpotLight,
		// ëºÇÃCullingLightÇí«â¡Ç∑ÇÈèÍçáÇÕ, ResourceLayoutÇ‡í«â¡ÇµÇƒÇ≠ÇæÇ≥Ç¢.
		CountOf
	};

	struct LightCullingDesc
	{
		std::uint64_t LightCounts[CullingLightType::CountOf]={0};
		std::shared_ptr<ZPrepass> ZPrepass = nullptr;
	};

	/****************************************************************************
	*				  		LightCulling 
	*************************************************************************//**
	*  @class     LightCulling
	*  @brief     Spot light and point light culling
	*****************************************************************************/
	class LightCulling : public NonCopyable
	{
	protected:
		using LowLevelGraphicsEnginePtr = std::shared_ptr<LowLevelGraphicsEngine>;
		using ZPrepassPtr               = std::shared_ptr<ZPrepass>;
		using BufferPtr                 = std::shared_ptr<rhi::core::GPUBuffer>;
		using ResourceViewPtr           = std::shared_ptr<rhi::core::GPUResourceView>;
		using ResourceLayoutPtr         = std::shared_ptr<rhi::core::RHIResourceLayout>;
		using PipelineStatePtr          = std::shared_ptr<rhi::core::GPUComputePipelineState>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Execute light culling. (on execution compute shader pipeline)
		            Scene : camera view constant buffer view
					light : scene light constant buffer view*/
		void Execute(const ResourceViewPtr& scene, const ResourceViewPtr& light);

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		ResourceViewPtr GetLightIDList(const CullingLightType type) { return _lightIDLists[type]; }
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		LightCulling(const LowLevelGraphicsEnginePtr& engine, const LightCullingDesc& desc);

		~LightCulling();

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void PrepareBuffer(const LightCullingDesc& desc);

		void PreparePipelineState();

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		/* @brief : 0 -> PointLight, 1 -> SpotLight, ç°å„í«â¡Ç≥ÇÍÇÈâ¬î\ê´Ç†ÇË*/
		std::vector<ResourceViewPtr> _lightIDLists = {};

		LowLevelGraphicsEnginePtr _engine = nullptr;

		ZPrepassPtr _zprepass = nullptr;

		ResourceLayoutPtr _resourceLayout = nullptr;

		PipelineStatePtr _pipeline = nullptr;

		std::uint32_t _width  = 0;
		std::uint32_t _height = 0;

		static constexpr std::uint64_t TILE_WIDTH  = 16;
		static constexpr std::uint64_t TILE_HEIGHT = 16;
	};
}
#endif