//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIResourceLayout.hpp
///             @brief  Resource Layout (DirectX12 :: Root Signature) 
///             @author Toide Yutaro
///             @date   2022_07_08
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_RESOURCE_LAYOUT_ELEMENT_HPP
#define RHI_RESOURCE_LAYOUT_ELEMENT_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include <memory>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////]
namespace rhi::core
{
	class GPUSampler;
	/****************************************************************************
	*				  			ResourceLayoutElement
	*************************************************************************//**
	*  @class     ResourceLayoutElement
	*  @brief     Layout element
	*****************************************************************************/
	struct ResourceLayoutElement
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		//ShaderVisibility   Visibility     = ShaderVisibility::All;
		DescriptorHeapType DescriptorType = DescriptorHeapType::CBV;
		size_t             Binding        = 0; // or shader resister
		size_t             RegisterSpace  = 0;

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		ResourceLayoutElement() = default;

		explicit ResourceLayoutElement(const core::DescriptorHeapType type, const size_t binding = 0, const size_t registerSpace = 0)
			: Binding(binding), RegisterSpace(registerSpace), DescriptorType(type)
		{
		};
	};

	/****************************************************************************
	*				  			SamplerLayoutElement
	*************************************************************************//**
	*  @class     SamplerLayoutElement
	*  @brief     Sampler Layout element
	*****************************************************************************/
	struct SamplerLayoutElement
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		ShaderVisibility Visibility     = ShaderVisibility::All;
		size_t           Binding        = 0;
		size_t           RegisterSpace  = 0;
		std::shared_ptr<GPUSampler> Sampler;

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		SamplerLayoutElement() = default;

		explicit SamplerLayoutElement(const std::shared_ptr<GPUSampler>& sampler, const size_t binding = 0, const size_t space = 0, const ShaderVisibility visibility = ShaderVisibility::All)
			: Visibility(visibility), Binding(binding), RegisterSpace(space), Sampler(sampler)
		{
		};
	};

	/****************************************************************************
	*				  			InputLayoutElement
	*************************************************************************//**
	*  @class     InputLayoutElement
	*  @brief     Input Layout element
	*****************************************************************************/
	struct InputLayoutElement
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		InputFormat Format       = InputFormat::Unknown;
		std::string SemanticName = "";
		size_t      Slot         = 0;
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		InputLayoutElement() = default;

		~InputLayoutElement() = default;

		explicit InputLayoutElement(const std::string& name, const InputFormat format, const size_t slot = 0) : Format(format), SemanticName(name), Slot(slot) {};
	};

	struct Value32Bit
	{
		union
		{
			std::uint32_t UInt;
			std::int32_t  Int;
			float         Float;
		};
		Value32Bit() = default;

		explicit Value32Bit(const std::uint32_t uint32) :UInt(uint32){};

		explicit Value32Bit(const std::int32_t int32) : Int(int32){}

		explicit Value32Bit(const float fValue) : Float(fValue) {};
	};

	/****************************************************************************
	*				  			SamplerLayoutElement
	*************************************************************************//**
	*  @class     SamplerLayoutElement
	*  @brief     Sampler Layout element
	*****************************************************************************/
	struct Constant32Bits
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		ShaderVisibility Visibility = ShaderVisibility::All;
		size_t           Binding        = 0;
		size_t           RegisterSpace  = 0;
		size_t           Count          = 1;

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Constant32Bits() = default;

		explicit Constant32Bits(const size_t count, const size_t binding = 0, const size_t space = 0, const ShaderVisibility visibility = ShaderVisibility::All)
			: Visibility(visibility), Binding(binding), RegisterSpace(space), Count(count)
		{
		};
	};
}
#endif