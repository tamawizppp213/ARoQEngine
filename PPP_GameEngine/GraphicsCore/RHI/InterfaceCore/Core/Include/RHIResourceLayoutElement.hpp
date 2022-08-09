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
		ShaderVisibility Visibility     = ShaderVisibility::All;
		DescriptorType   DescriptorType = DescriptorType::Buffer;
		size_t           ShaderRegister = 0;
		size_t           RegisterSpace  = 0;

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		ResourceLayoutElement() = default;
		explicit ResourceLayoutElement(const core::DescriptorType type, const size_t shaderRegister = 0, const size_t registerSpace = 0, const ShaderVisibility visibility = ShaderVisibility::All)
			: Visibility(visibility), ShaderRegister(shaderRegister), RegisterSpace(registerSpace), DescriptorType(type)
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
		size_t           ShaderRegister = 0;
		size_t           RegisterSpace  = 0;
		std::shared_ptr<GPUSampler> Sampler;

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		SamplerLayoutElement() = default;
		explicit SamplerLayoutElement(const std::shared_ptr<GPUSampler>& sampler, const size_t shaderRegister = 0, const size_t space = 0, const ShaderVisibility visibility = ShaderVisibility::All)
			: Visibility(visibility), ShaderRegister(shaderRegister), RegisterSpace(space), Sampler(sampler)
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
		PixelFormat Format = PixelFormat::Unknown;
		std::string Name = "";
		size_t      Slot = 0;
		InputLayoutElement() = default;
		~InputLayoutElement() = default;
		explicit InputLayoutElement(const std::string& name, const PixelFormat format, const size_t slot = 0) : Format(format), Name(name), Slot(slot) {};
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
		size_t           ShaderRegister = 0;
		size_t           RegisterSpace  = 0;
		size_t           Count          = 1;

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Constant32Bits() = default;
		explicit Constant32Bits(const size_t count, const size_t shaderRegister = 0, const size_t space = 0, const ShaderVisibility visibility = ShaderVisibility::All)
			: Visibility(visibility), ShaderRegister(shaderRegister), RegisterSpace(space), Count(count)
		{
		};
	};
}
#endif