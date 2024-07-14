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
	/* @brief  Resource layout descriptor (set up how to bind gpu resource)
	*****************************************************************************/
	struct RHIResourceLayoutDesc
	{
		/*! @brief Descriptor Table�ɓo�^�����e�v�f*/
		gu::DynamicArray<core::ResourceLayoutElement> Elements = {};

		/*! @brief ���I�T���v���[*/
		gu::DynamicArray<core::SamplerLayoutElement>  Samplers = {};
		
		/*! @brief 32bit�̒萔*/
		gu::Optional<core::Constant32Bits> Constant32Bits = {};

		/*! @brief RootSignature�̎�ނ��w��*/
		RootSignatureType ResourceLayoutType = RootSignatureType::Rasterize;

		/*! @brief ���\�[�X�ɒ���Index�Ŏw��ł���悤�ɂ��邩*/
		bool UseDirectlyIndexedResourceHeap  = false;

		/*! @brief ���I�T���v���[�ɒ���Index�Ŏw�肷��悤�ɂ��邩*/
		bool UseDirectlyIndexedSamplerHeap   = false;

		/*! @brief Input Assembly�̓��̓��C�A�E�g���V�F�[�_�[�X�e�[�W�ɍ̗p���邩*/
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
	/*  @brief  GPU�̃��\�[�X�̃o�C���h���@��ݒ肷�郊�\�[�X���C�A�E�g
	*****************************************************************************/
	class RHIResourceLayout : public gu::NonCopyable
	{
	public:
		#pragma region Public Function
		#pragma endregion

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     GPU���\�[�X�̃V�F�[�_�[�o�C���f�B���O�v�f��Ԃ��܂�
		*  @param[in] void
		*  @return    const ResourceLayoutElement&
		*************************************************************************/
		const ResourceLayoutElement&  GetResourceElement(const gu::uint8 index) const { return _desc.Elements[index]; }
		
		/*!**********************************************************************
		*  @brief     SamplerLayoutElement��Ԃ��܂�
		*  @param[in] void
		*  @return    const SamplerLayoutElement& 
		*************************************************************************/
		const SamplerLayoutElement& GetSamplerElement (const gu::uint8 index) const { return _desc.Samplers[index]; }
		
		/*!**********************************************************************
		*  @brief     Constant32Bits��Ԃ��܂�
		*  @param[in] void
		*  @return    gu::Optional<Constant32Bits>
		*************************************************************************/
		gu::Optional<Constant32Bits> GetConstant32Bits() const noexcept { return _desc.Constant32Bits; }

		/*!**********************************************************************
		*  @brief     �S�Ă�ResourceLayoutElement��Ԃ��܂�
		*  @param[in] void
		*  @return    gu::DynamicArray<ResourceLayoutElement>
		*************************************************************************/
		const gu::DynamicArray<ResourceLayoutElement>& GetResourceElements() const{ return _desc.Elements; }
		
		/*!**********************************************************************
		*  @brief     �T���v���[�X�e�[�g�̃V�F�[�_�[�o�C���f�B���O�v�f��Ԃ��܂�
		*  @param[in] void 
		*  @return    gu::DynamicArray<SamplerLayoutElement>& : �T���v���[�X�e�[�g�̃V�F�[�_�[�o�C���f�B���O�v�f
		*************************************************************************/
		const gu::DynamicArray<SamplerLayoutElement>&  GetSamplerElements () const{ return _desc.Samplers; }
		
		/*!**********************************************************************
		*  @brief     �f�o�b�O�\������ݒ肵�܂�
		*  @param[in] const gu::tstring& name : �f�o�b�O�\����
		*  @return    void
		*************************************************************************/
		virtual void SetName(const gu::tstring& name) = 0;
		
		#pragma endregion

		#pragma region Public Constructor and Destructor
		#pragma endregion

	protected:
		#pragma region Protected Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		RHIResourceLayout() = default;

		/*! @brief �f�X�g���N�^*/
		virtual ~RHIResourceLayout()
		{
			if (_device) { _device.Reset(); }
		}

		/*! @brief Descriptor�ō쐬*/
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
		/*! @brief �_���f�o�C�X*/
		gu::SharedPointer<RHIDevice> _device = nullptr;
		
		/*! @brief ResourceLayout��*/
		RHIResourceLayoutDesc _desc = {};
		#pragma endregion
	};
}
#endif