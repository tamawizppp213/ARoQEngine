//////////////////////////////////////////////////////////////////////////////////
///             @file   GameWorldInfo.hpp
///             @brief  Game object information to exist in the game world. (World position etc...)
///             @author Toide Yutaro
///             @date   2023_02_03
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GAME_WORLD_INFO_HPP
#define GAME_WORLD_INFO_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUBuffer.hpp"
#include "GameUtility/Base/Include/HLSLUtility.hpp"
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include <vector>
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class LowLevelGraphicsEngine;

namespace rhi::core
{
	class GPUResourceView;
	class RHICommandList;
}
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc::core
{
	struct GameWorldConstant
	{
		hlsl::matrix World; // Scale * Rotation * Translation
#ifdef _DEBUG
		hlsl::float4 DebugColor = gm::Float4(1.0f,1.0f,1.0f,1.0f);
#endif
	};

	/****************************************************************************
	*				  		GameWorldInfo
	****************************************************************************/
	/* @class     GameWorldInfo
	*  @brief     GameWorld constant buffer (This class can be used for both individual drawings and instancing drawings. )
	*             When you would like to change the contents in the buffer, you should use GetBuffer function.
	*****************************************************************************/
	class GameWorldInfo : public gu::NonCopyable
	{
		using LowLevelGraphicsEnginePtr = gu::SharedPointer<LowLevelGraphicsEngine>;
		using ConstantBufferPtr         = gu::SharedPointer<rhi::core::GPUBuffer>;
		using ConstantBufferViewPtr     = gu::SharedPointer<rhi::core::GPUResourceView>;
		using GraphicsCommandListPtr    = gu::SharedPointer<rhi::core::RHICommandList>;

	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Bind constant buffer view. index : root descriptor table id.*/
		void Bind(const GraphicsCommandListPtr& commandList, const std::uint32_t index);

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		/* @brief : Return max instance count for instance drawing. */
		std::uint64_t GetMaxInsntanceCount() const { return _maxInstanceCount; }

		/* @brief : Return gpu buffer pointer*/
		ConstantBufferPtr GetBuffer() const noexcept { return _gameWorldConstants; }

		/* @brief: Return constant buffer view including the Game world constants.*/
		ConstantBufferViewPtr GetBufferView() const noexcept { return _resourceView; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GameWorldInfo(const LowLevelGraphicsEnginePtr& engine, const std::uint64_t maxInstanceCount = 1);

		virtual ~GameWorldInfo();
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		LowLevelGraphicsEnginePtr _engine = nullptr;

		/* @brief : Constant buffer used for holding the Game world constants*/
		ConstantBufferPtr _gameWorldConstants = nullptr;

		/* @brief : Constant buffer view including the Game world constants. */
		ConstantBufferViewPtr _resourceView = nullptr;
		
		/* @brief : For instancing drawing. max Instance count*/
		std::uint64_t _maxInstanceCount = 1;
	};
}
#endif