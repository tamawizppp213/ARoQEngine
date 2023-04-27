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
		/* @brief: Return gpu resource shader binding element*/
		const ResourceLayoutElement&  GetResourceElement(const size_t index) const { return _elements[index]; }
		
		/* @brief : Return shader layout element of the sampler state*/
		const SamplerLayoutElement&   GetSamplerElement (const size_t index) const { return _samplers[index]; }
		
		/* @brief : Return Constant32Bits data*/
		std::optional<Constant32Bits> GetConstant32Bits() const noexcept { return _constant32Bits; }

		/* @brief : Return All gpu resource shader binding elements*/
		const std::vector<ResourceLayoutElement>& GetResourceElements() const{ return _elements; }
		
		/* @brief : Return all sampler state shader binding elements*/
		const std::vector<SamplerLayoutElement>&  GetSamplerElements () const{ return _samplers; }
		
		virtual void SetName(const std::wstring& name) = 0;
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
			_elements.clear(); _elements.shrink_to_fit();
			_samplers.clear(); _samplers.shrink_to_fit();

		}

		explicit RHIResourceLayout(
			const std::shared_ptr<RHIDevice>& device,
			const std::vector<core::ResourceLayoutElement>& elements = {},
			const std::vector<core::SamplerLayoutElement>&  samplers = {},
			const std::optional<core::Constant32Bits>& constant32Bits = std::nullopt
		) : _device(device), _elements(elements), _samplers(samplers), _constant32Bits(constant32Bits){ };
		
		explicit RHIResourceLayout(
			const std::shared_ptr<RHIDevice>& device, 
			const core::ResourceLayoutElement& element, 
			const core::SamplerLayoutElement& sampler,
			const std::optional<core::Constant32Bits>& constant32Bits = std::nullopt
		) : _device(device), _elements({ element }), _samplers({sampler}), _constant32Bits(constant32Bits) { }

		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::shared_ptr<RHIDevice> _device = nullptr;
		std::vector<core::ResourceLayoutElement> _elements       = {};
		std::vector<core::SamplerLayoutElement>  _samplers       = {};
		std::optional<core::Constant32Bits>      _constant32Bits = std::nullopt;
	};
}
#endif