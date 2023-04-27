//////////////////////////////////////////////////////////////////////////////////
///             @file   GPUBlendState.hpp
///             @brief  Blend State
///             @author Toide Yutaro
///             @date   2022_06_28
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GPU_BLEND_STATE_HPP
#define GPU_BLEND_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GPUState.hpp"
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class RHIDevice;
	enum class DefaultBlendStateType
	{
		NoColorWrite,
		OverWrite,
		AlphaBlend,
		CountOfBlendStateType
	};
	/****************************************************************************
	*				  			RHIPipelineState
	*************************************************************************//**
	*  @class     RHIPipelineState
	*  @brief     PipelineState
	*****************************************************************************/
	class GPUBlendState : public GPUState
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		const core::BlendProperty& GetProperty(const size_t index = 0) { return _blendProperties[index]; }

		bool UseAlphaToCoverage() const { return _blendProperties[0].AlphaToConverageEnable; }
	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUBlendState() = default;

		virtual ~GPUBlendState();

		explicit GPUBlendState(const std::shared_ptr<RHIDevice>& device, const std::vector<BlendProperty>& properties) : GPUState(device), _blendProperties(properties), _isIndependentBlendEnable(true) {};
		
		explicit GPUBlendState(const std::shared_ptr<RHIDevice>& device, const BlendProperty& blendProperty) : GPUState(device)
		{
			_blendProperties.push_back(blendProperty);
			_isIndependentBlendEnable = false;
		}

		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		/* @brief : Blend State Properties : (Render target Ç≈ã§í ÇÃèÍçáÇÕàÍÇ¬ÇæÇØégópÇµ, isIndependentBlendEnableÇfalseÇ…ê›íË)*/
		std::vector<core::BlendProperty> _blendProperties;
		
		/* @brief : is all render target configuration the same?  */
		bool _isIndependentBlendEnable = false;
	};

}
#endif