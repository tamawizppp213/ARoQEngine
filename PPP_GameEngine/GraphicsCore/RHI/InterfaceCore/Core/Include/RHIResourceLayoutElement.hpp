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
#include "GameUtility/Base/Include/GUSmartPointer.hpp"

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
		static constexpr gu::int32 GLOBAL_ROOT_SIGNATURE_REGISTER_SPACE = 1;
		static constexpr gu::int32 LOCAL_ROOT_SIGNATURE_REGISTER_SPACE  = 2;

		static ResourceLayoutElement GlobalRootSignature(const core::DescriptorHeapType type, const size_t binding = 0, const size_t registerSpace = GLOBAL_ROOT_SIGNATURE_REGISTER_SPACE)
		{
			return ResourceLayoutElement(type, binding, registerSpace, rhi::core::ShaderVisibleFlag::All);
		}

		static ResourceLayoutElement LocalRootSignature(const core::DescriptorHeapType type, const size_t binding = 0, const size_t registerSpace = LOCAL_ROOT_SIGNATURE_REGISTER_SPACE)
		{
			return ResourceLayoutElement(type, binding, registerSpace, rhi::core::ShaderVisibleFlag::All);
		}

		static ResourceLayoutElement GraphicsRootSignature(const core::DescriptorHeapType type, const size_t binding, const size_t registerSpace = 0, const core::ShaderVisibleFlag flag = core::ShaderVisibleFlag::All)
		{
			return ResourceLayoutElement(type, binding, registerSpace, flag);
		}

		static ResourceLayoutElement ComputeRootSignature(const core::DescriptorHeapType type, const size_t binding, const size_t registerSpace = 0, const core::ShaderVisibleFlag flag = core::ShaderVisibleFlag::All)
		{
			return ResourceLayoutElement(type, binding, registerSpace, flag);
		}

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		ShaderVisibleFlag  Visibility     = ShaderVisibleFlag::All;    // Specify the shaders that can access the contents of the root signature slot.
		DescriptorHeapType DescriptorType = DescriptorHeapType::CBV;  // Descriptor type
		size_t             Binding        = 0;                        // binding or shader resister. In case of srv, 3 is mapped in register(t3) of HLSL.   
		size_t             RegisterSpace  = 0;                        // register space. normally this value is set 0. Multiple descriptor arrays of unknown size can be prevented from overlapping
		                                                              // register space how to: https://learn.microsoft.com/ja-jp/windows/win32/direct3d12/resource-binding-in-hlsl

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		ResourceLayoutElement() = default;

		explicit ResourceLayoutElement(const core::DescriptorHeapType type, const size_t binding = 0, const size_t registerSpace = 0, const ShaderVisibleFlag visibility = ShaderVisibleFlag::All)
			: Binding(binding), RegisterSpace(registerSpace), DescriptorType(type), Visibility(visibility)
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
		ShaderVisibleFlag Visibility     = ShaderVisibleFlag::All;
		size_t           Binding        = 0;
		size_t           RegisterSpace  = 0;
		gu::SharedPointer<GPUSampler> Sampler;

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		SamplerLayoutElement() = default;

		explicit SamplerLayoutElement(const gu::SharedPointer<GPUSampler>& sampler, const size_t binding = 0, const size_t space = 0, const ShaderVisibleFlag visibility = ShaderVisibleFlag::All)
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
		InputFormat         Format         = InputFormat::Unknown;
		InputClassification Classification = InputClassification::PerVertex;
		size_t              Slot           = 0;
		std::string         SemanticName   = "";

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		InputLayoutElement() = default;

		~InputLayoutElement() = default;

		explicit InputLayoutElement(const std::string& name, const InputFormat format, const InputClassification classification = InputClassification::PerVertex,  const size_t slot = 0) : Format(format), SemanticName(name), Classification(classification), Slot(slot) {};
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
		ShaderVisibleFlag Visibility     = ShaderVisibleFlag::All;
		size_t           Binding        = 0; // リソースレイアウト上でバインドするIndex
		size_t           RegisterSpace  = 0; // レジスタ空間
		size_t           Count          = 1; // 32 bitの定数を割り当てる数

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Constant32Bits() = default;

		explicit Constant32Bits
		(
			const size_t count, 
			const size_t binding = 0, 
			const size_t space   = 0, 
			const ShaderVisibleFlag visibility = ShaderVisibleFlag::All)
			: Visibility(visibility), Binding(binding), RegisterSpace(space), Count(count)
		{
		};
	};
}
#endif