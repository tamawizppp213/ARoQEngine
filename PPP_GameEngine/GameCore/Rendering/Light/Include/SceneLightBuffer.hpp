//////////////////////////////////////////////////////////////////////////////////
///             @file   SceneLightBuffer.hpp
///             @brief  Frame resource: scene light management
///             @author Toide Yutaro
///             @date   2022_07_19
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SCENE_LIGHT_BUFFER_HPP
#define SCENE_LIGHT_BUFFER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUBuffer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUResourceView.hpp"
#include "GameUtility/Base/Include/Screen.hpp"
#include "LightType.hpp"
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include <memory>
#include <vector>
#include <numeric>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc::rendering
{
	using namespace rhi::core;

	/****************************************************************************
	*				  			   SceneLightBuffer
	*************************************************************************//**
	*  @class     SceneLightBuffer
	*  @brief     Scene light buffer
	*****************************************************************************/
	template<typename TLight> requires std::is_base_of_v<LightData, TLight>
	class SceneLightBuffer : public NonCopyable
	{
	protected:
		using LowLevelGraphicsEnginePtr = std::shared_ptr<LowLevelGraphicsEngine>;
		using BufferPtr                 = std::shared_ptr<rhi::core::GPUBuffer>;
		using ResourceViewPtr           = std::shared_ptr<rhi::core::GPUResourceView>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Bind between the resource layout and GPU resource.*/
		void BindLightData
		(
			const std::shared_ptr<rhi::core::RHICommandList>& graphicsCommandList,
			const std::uint32_t bindID
		);

		/* @brief : Bind light ID for light culling. UAV list
		            This variable is basically limited by the Point lights and spot lights*/
		void BindHitLightIDLists
		(
			const std::shared_ptr<rhi::core::RHICommandList>& commandList,
			const std::uint32_t bindID
		);

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/* @brief : return settable light data count. */
		std::uint64_t GetMaxCount() const { return _lights.size(); }

		/* @brief : return the max count among set light data.*/
		std::uint32_t GetUsingMaxCount() const { return _usingMaxCount; }

		/* @brief : return the light data*/
		void SetLight(const std::uint32_t index, const TLight& light);

		TLight& GetLight(const std::uint32_t index) { return _lights.at(index); }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		SceneLightBuffer
		(
			const LowLevelGraphicsEnginePtr& engine, 
			const size_t count,
			const bool useLightIDList = false // Use for Tile-based rendering. this variable need std::uint32_t * TILE * ScreenWidth * ScreenHeight bytesize 
		);

		~SceneLightBuffer()
		{
			_lights   .clear(); _lights   .shrink_to_fit();
			_updateIDs.clear(); _updateIDs.shrink_to_fit();
		}
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void UpdateLightData();

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		LowLevelGraphicsEnginePtr _engine = nullptr;

		/* A list summarizing the Index of Light that HIt in each tile,*/
		ResourceViewPtr _hitLightIDListsInTile = nullptr;
		std::vector<std::int32_t> _updateIDs = {};

		/* Light GPU data*/
		ResourceViewPtr _lightDataView    = nullptr;

		/* Light CPU data*/
		std::vector<TLight> _lights = {};

		/* Config*/
		std::uint32_t _usingMaxCount = 0;

		bool _needUpdate = false;


		// for tile based rendering. (This variable is basically limited by the Point light and spot light);
		static constexpr std::uint32_t TILE_LENGTH = 16;
	};

#pragma region Implement
	template<typename TLight> requires std::is_base_of_v<LightData, TLight>
	void SceneLightBuffer<TLight>::BindLightData
	(
		const std::shared_ptr<rhi::core::RHICommandList>& commandList,
		const std::uint32_t bindID)
	{
		/*-------------------------------------------------------------------
		-             Copy the cpu light data to the gpu
		---------------------------------------------------------------------*/
		if (_needUpdate) { UpdateLightData();}

		_lightDataView->Bind(commandList, bindID);
	}

	/****************************************************************************
	*                       BindHitLightIDLists
	*************************************************************************//**
	*  @fn        template<typename TLight> requires std::is_base_of_v<LightData, TLight>
				  void SceneLightBuffer<TLight>::BindHitLightIDListss

	*  @brief     Bind UAV about the HitLightIDLists In Tile.
	*
	*  @param[in] const std::shared_ptr<rhi::core::RHICommandList>& graphics or compute commandList
	*
	*  @return 　　void
	*****************************************************************************/
	template<typename TLight> requires std::is_base_of_v<LightData, TLight>
	void SceneLightBuffer<TLight>::BindHitLightIDLists
	(
		const std::shared_ptr<rhi::core::RHICommandList>& commandList,
		const std::uint32_t bindID)
	{
		if (!_hitLightIDListsInTile) { return; }
		_hitLightIDListsInTile->Bind(commandList, bindID);
	}

	/****************************************************************************
	*                       SetLight
	*************************************************************************//**
	*  @fn        template<typename TLight> requires std::is_base_of_v<LightData, TLight>
				  void SceneLightBuffer<TLight>::SetLight(const std::uint32_t index, const TLight& light)

	*  @brief     Set light and prepare copy to the GPU.
	*
	*  @param[in] const std::uint32_t lightIndex
	*  @param[in] const TLight& light
	*
	*  @return 　　void
	*****************************************************************************/
	template<typename TLight> requires std::is_base_of_v<LightData, TLight>
	void SceneLightBuffer<TLight>::SetLight(const std::uint32_t index, const TLight& light)
	{
#ifdef _DEBUG
		assert(index < GetMaxCount());
#endif

		_lights   [index] = light;
		_updateIDs.push_back(index);
		_usingMaxCount = (std::max)(index, _usingMaxCount);
		_needUpdate       = true;
		
	}

	/****************************************************************************
	*                       UpdateLightData
	*************************************************************************//**
	*  @fn        template<typename TLight> requires std::is_base_of_v<LightData, TLight>
				  void SceneLightBuffer<TLight>::UpdateLightData()

	*  @brief     Copy the CPU light data to the GPU buffer
	*
	*  @param[in] void
	*
	*  @return 　　void
	*****************************************************************************/
	template<typename TLight> requires std::is_base_of_v<LightData, TLight>
	void SceneLightBuffer<TLight>::UpdateLightData()
	{
		if (!_needUpdate) { return; }

		// get light buffer from the resource view
		const auto lightBuffer = _lightDataView->GetBuffer();

		// clear
		lightBuffer->CopyStart();
		for (std::uint32_t i = 0; i < (std::uint32_t)_updateIDs.size(); ++i)
		{
			lightBuffer->CopyData(&_lights[i], _updateIDs[i]);
		}
		lightBuffer->CopyEnd();

		// clear update IDs
		_updateIDs.clear();
		_needUpdate = false;
	}

	/*-------------------------------------------------------------------
	-              Constructor
	---------------------------------------------------------------------*/
	template<typename TLight> requires std::is_base_of_v<LightData, TLight>
	SceneLightBuffer<TLight>::SceneLightBuffer(
		const LowLevelGraphicsEnginePtr& engine,
		const size_t count,
		const bool useLightIDList) : _engine(engine)
	{
#ifdef _DEBUG
		assert(_engine);
#endif
		const auto device = _engine->GetDevice();

		_lights.resize(count);

		/*-------------------------------------------------------------------
		-              Create Light Data View
		---------------------------------------------------------------------*/
		{
			const auto bufferInfo = GPUBufferMetaData::ConstantBuffer(sizeof(TLight), count);
			const auto buffer     = device->CreateBuffer(bufferInfo, L"LightData");

			_lightDataView = device->CreateResourceView(ResourceViewType::ConstantBuffer, buffer, nullptr);
		}

		/*-------------------------------------------------------------------
		-              Create Light IndexList
		---------------------------------------------------------------------*/
		if(useLightIDList)
		{
			const int tileCount = static_cast<int>((Screen::GetScreenWidth() / TILE_LENGTH) * (Screen::GetScreenHeight()/ TILE_LENGTH));

			// set up light ID buffer ()
			auto bufferInfo          = GPUBufferMetaData::DefaultBuffer(sizeof(std::int32_t), count * tileCount);
			bufferInfo.ResourceUsage = ResourceUsage::UnorderedAccess;
			const auto buffer        = device->CreateBuffer(bufferInfo, L"LightID");

			// Create light id uav resource view
			_hitLightIDListsInTile = device->CreateResourceView(ResourceViewType::RWStructuredBuffer, buffer, nullptr);
		}
	}
}

#pragma endregion Implement
#endif