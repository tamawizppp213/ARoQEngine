//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIResourceLayout.hpp
///             @brief  Resource Layout (DirectX12 :: Root Signature) 
///             @author Toide Yutaro
///             @date   2022_07_08
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_RESOURCE_LAYOUT_HPP
#define RHI_RESOURCE_LAYOUT_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIResourceLayoutElement.hpp"
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include <vector>
#include <optional>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////]
namespace rhi::core
{
	class RHIDevice;

	/****************************************************************************
	*				  			RHIResourceLayoutDesc
	*************************************************************************//**
	*  @class     RHIResourceLayoutDesc
	*  @brief     Resource layout descriptor (set up how to bind gpu resource)
	*****************************************************************************/
	struct RHIResourceLayoutDesc
	{
		std::vector<core::ResourceLayoutElement> Elements = {};
		std::vector<core::SamplerLayoutElement>  Samplers = {};
		std::optional<core::Constant32Bits>      Constant32Bits = std::nullopt;
		RootSignatureType ResourceLayoutType = RootSignatureType::Rasterize;
		bool UseDirectlyIndexedResourceHeap  = false;
		bool UseDirectlyIndexedSamplerHeap   = false;
		bool UseIAInputLayout                = true;

		RHIResourceLayoutDesc(
			const std::vector<core::ResourceLayoutElement>& elements = {},
			const std::vector<core::SamplerLayoutElement>& samplers = {},
			const std::optional<core::Constant32Bits>& constant32Bits = std::nullopt,
			const RootSignatureType resourceLayoutType = RootSignatureType::Rasterize,
			const bool useDirectlyIndexedResourceHeap = false,
			const bool useDirectlyIndexedSamplerHeap = false,
			const bool useIAInputLayout = true
		) : Elements(elements), Samplers(samplers), Constant32Bits(constant32Bits),
			ResourceLayoutType(resourceLayoutType),
			UseDirectlyIndexedResourceHeap(useDirectlyIndexedResourceHeap),
			UseDirectlyIndexedSamplerHeap(useDirectlyIndexedSamplerHeap),
			UseIAInputLayout(useIAInputLayout)
		{
		};

		~RHIResourceLayoutDesc()
		{
			Elements.clear(); Elements.shrink_to_fit();
			Samplers.clear(); Samplers.shrink_to_fit();
		}
	};

	/****************************************************************************
	*				  			RHIResourceLayout
	*************************************************************************//**
	*  @class     RHIResourceLayout
	*  @brief     Resource layout (set up how to bind gpu resource)
	*****************************************************************************/
	class RHIResourceLayout : public NonCopyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief :  Return gpu resource shader binding element
		/*----------------------------------------------------------------------*/
		const ResourceLayoutElement&  GetResourceElement(const size_t index) const { return _desc.Elements[index]; }
		
		/*----------------------------------------------------------------------
		*  @brief :  Return shader layout element of the sampler state
		/*----------------------------------------------------------------------*/
		const SamplerLayoutElement&   GetSamplerElement (const size_t index) const { return _desc.Samplers[index]; }
		
		/*----------------------------------------------------------------------
		*  @brief :  Return Constant32Bits data
		/*----------------------------------------------------------------------*/
		std::optional<Constant32Bits> GetConstant32Bits() const noexcept { return _desc.Constant32Bits; }

		/*----------------------------------------------------------------------
		*  @brief :  Return All gpu resource shader binding elements
		/*----------------------------------------------------------------------*/
		const std::vector<ResourceLayoutElement>& GetResourceElements() const{ return _desc.Elements; }
		
		/*----------------------------------------------------------------------
		*  @brief :  Return all sampler state shader binding elements
		/*----------------------------------------------------------------------*/
		const std::vector<SamplerLayoutElement>&  GetSamplerElements () const{ return _desc.Samplers; }
		
		virtual void SetName(const gu::tstring& name) = 0;
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIResourceLayout() = default;

		virtual ~RHIResourceLayout()
		{
			if (_device) { _device.Reset(); }
		}

		explicit RHIResourceLayout(const gu::SharedPointer<RHIDevice>& device, const RHIResourceLayoutDesc& desc)
			: _device(device), _desc(desc)
		{
		};

		explicit RHIResourceLayout(
			const gu::SharedPointer<RHIDevice>& device,
			const std::vector<core::ResourceLayoutElement>& elements = {},
			const std::vector<core::SamplerLayoutElement>&  samplers = {},
			const std::optional<core::Constant32Bits>& constant32Bits = std::nullopt
		) : _device(device)
		{
			_desc.Elements       = elements;
			_desc.Samplers       = samplers;
			_desc.Constant32Bits = constant32Bits;
		};
		
		explicit RHIResourceLayout(
			const gu::SharedPointer<RHIDevice>& device,
			const core::ResourceLayoutElement& element,
			const core::SamplerLayoutElement& sampler,
			const std::optional<core::Constant32Bits>& constant32Bits = std::nullopt
		) : _device(device)
		{
			_desc.Elements = { element };
			_desc.Samplers = { sampler };
			_desc.Constant32Bits = { constant32Bits };
		}

		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		gu::SharedPointer<RHIDevice> _device = nullptr;
		
		RHIResourceLayoutDesc _desc = {};
	};
}
#endif