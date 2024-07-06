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
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
#include "GameUtility/Base/Include/GUOptional.hpp"
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
	****************************************************************************/
	/* @class     RHIResourceLayoutDesc
	*  @brief     Resource layout descriptor (set up how to bind gpu resource)
	*****************************************************************************/
	struct RHIResourceLayoutDesc
	{
		/*! @brief Descriptor Tableに登録される各要素*/
		gu::DynamicArray<core::ResourceLayoutElement> Elements = {};

		/*! @brief 動的サンプラー*/
		gu::DynamicArray<core::SamplerLayoutElement>  Samplers = {};
		
		/*! @brief 32bitの定数*/
		gu::Optional<core::Constant32Bits> Constant32Bits = {};

		/*! @brief RootSignatureの種類を指定*/
		RootSignatureType ResourceLayoutType = RootSignatureType::Rasterize;

		/*! @brief リソースに直接Indexで指定できるようにするか*/
		bool UseDirectlyIndexedResourceHeap  = false;

		/*! @brief 動的サンプラーに直接Indexで指定するようにするか*/
		bool UseDirectlyIndexedSamplerHeap   = false;

		/*! @brief Input Assemblyの入力レイアウトをシェーダーステージに採用するか*/
		bool UseIAInputLayout = true;

		RHIResourceLayoutDesc(
			const gu::DynamicArray<core::ResourceLayoutElement>& elements = {},
			const gu::DynamicArray<core::SamplerLayoutElement>& samplers = {},
			const gu::Optional<core::Constant32Bits>& constant32Bits = {},
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
			Elements.Clear(); Elements.ShrinkToFit();
			Samplers.Clear(); Samplers.ShrinkToFit();
		}
	};

	/****************************************************************************
	*				  			RHIResourceLayout
	****************************************************************************/
	/* @class     RHIResourceLayout
	*  @brief     Resource layout (set up how to bind gpu resource)
	*****************************************************************************/
	class RHIResourceLayout : public gu::NonCopyable
	{
	public:
		#pragma region Public Function
		#pragma endregion

		#pragma region Public Property
		/*----------------------------------------------------------------------
		*  @brief :  Return gpu resource shader binding element
		*----------------------------------------------------------------------*/
		const ResourceLayoutElement&  GetResourceElement(const size_t index) const { return _desc.Elements[index]; }
		
		/*----------------------------------------------------------------------
		*  @brief :  Return shader layout element of the sampler state
		*----------------------------------------------------------------------*/
		const SamplerLayoutElement&   GetSamplerElement (const size_t index) const { return _desc.Samplers[index]; }
		
		/*----------------------------------------------------------------------
		*  @brief :  Return Constant32Bits data
		*----------------------------------------------------------------------*/
		gu::Optional<Constant32Bits> GetConstant32Bits() const noexcept { return _desc.Constant32Bits; }

		/*----------------------------------------------------------------------
		*  @brief :  Return All gpu resource shader binding elements
		*----------------------------------------------------------------------*/
		const gu::DynamicArray<ResourceLayoutElement>& GetResourceElements() const{ return _desc.Elements; }
		
		/*----------------------------------------------------------------------
		*  @brief :  Return all sampler state shader binding elements
		*----------------------------------------------------------------------*/
		const gu::DynamicArray<SamplerLayoutElement>&  GetSamplerElements () const{ return _desc.Samplers; }
		
		virtual void SetName(const gu::tstring& name) = 0;
		
		#pragma endregion

		#pragma region Public Constructor and Destructor
		#pragma endregion

	protected:
		#pragma region Protected Constructor and Destructor
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
			const gu::DynamicArray<core::ResourceLayoutElement>& elements = {},
			const gu::DynamicArray<core::SamplerLayoutElement>&  samplers = {},
			const gu::Optional<core::Constant32Bits>& constant32Bits = {}
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
			const gu::Optional<core::Constant32Bits>& constant32Bits = {}
		) : _device(device)
		{
			_desc.Elements = { element };
			_desc.Samplers = { sampler };
			_desc.Constant32Bits = { constant32Bits };
		}

		#pragma endregion

		#pragma region Protected Function
		#pragma endregion

		#pragma region Protected Property
		/*! @brief 論理デバイス*/
		gu::SharedPointer<RHIDevice> _device = nullptr;
		
		RHIResourceLayoutDesc _desc = {};
		#pragma endregion
	};
}
#endif