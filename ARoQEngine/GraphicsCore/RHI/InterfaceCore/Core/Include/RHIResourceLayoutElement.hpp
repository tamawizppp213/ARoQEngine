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
	/* @brief    ���_�f�[�^�̓��͂ɂ�����, �X�̃f�[�^���ǂ̂悤�ȍ\���ł��邩���w�肵�܂�. @n
	*            ��̓I�ɂ�, POSITION, 0, R32G32B32 (32bit��float��x,y,z��ݒ�), PerVertex�Ȃǂ̂悤�Ɏg�p���܂�.
	*****************************************************************************/
	struct InputLayoutElement
	{
	public:
		#pragma region Public Function
		#pragma endregion

		#pragma region Public Member Variables
		/*! @brief �V�F�[�_�[�p�C�v���C���ɑ΂��ăp�����[�^�̎g�p�ړI�Ɋւ������`�B���邽�߂̕�����ł�. 
		           Semantic���͈ȉ��̎Q�l������������������. https://learn.microsoft.com/ja-jp/windows/win32/direct3dhlsl/dx-graphics-hlsl-semantics*/
		gu::string SemanticName = "";

		/*! @brief �����Z�}���e�B�b�N�����g�p����ꍇ�ɂ����Č݂��̃Z�}���e�B�b�N����ʂ���ꍇ�Ɏg�p���܂�. */
		gu::uint8  SemanticIndex = 0;

		/*! @brief ���̓X���b�g�ԍ����w�肵�܂�. ���ݗL���l��0�`15�ł�. �����̒��_�o�b�t�@���K�v�Ȏ��Ɏw�肵�Ă�������. 
		    https://learn.microsoft.com/ja-jp/windows/win32/api/d3d12/ns-d3d12-d3d12_input_element_desc*/
		gu::uint8 Slot = 0;

		/*! @brief ��v�f�̍\���f�[�^�`����ݒ肵�܂�. �����̃f�[�^�`�����K�v�Ȃ̂ł����, DynamicArray��InputLayoutElement�𕡐��ݒ肵�܂�*/
		PixelFormat Format = PixelFormat::Unknown;

		/*! @brief ���̓��C�A�E�g�����_���Ƃɐݒ肳�����̂�, �C���X�^���X���Ƃɐݒ肳�����̂����w�肵�܂�*/
		InputClassification Classification = InputClassification::PerVertex;

		#pragma endregion
		
		#pragma region Public Constructor and Destructor
		/*! @brief �f�t�H���g�R���X�g���N�^*/
		InputLayoutElement() = default;

		/*! @brief �f�X�g���N�^*/
		~InputLayoutElement() = default;

		/*! @brief ���̓��C�A�E�g�̎�ނ��`����R���X�g���N�^�ł�.*/
		explicit InputLayoutElement(const gu::string& name, const PixelFormat format, const InputClassification classification = InputClassification::PerVertex, const gu::uint8 semanticIndex = 0, const gu::uint8 slot = 0) : Format(format), SemanticName(name), Classification(classification), SemanticIndex(semanticIndex), Slot(slot) {};
		#pragma endregion 
	};

	struct Value32Bit
	{
		union
		{
			gu::uint32 UInt;
			gu::int32  Int;
			float         Float;
		};
		Value32Bit() = default;

		explicit Value32Bit(const gu::uint32 uint32) :UInt(uint32){};

		explicit Value32Bit(const gu::int32 int32) : Int(int32){}

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
		size_t           Binding        = 0; // ���\�[�X���C�A�E�g��Ńo�C���h����Index
		size_t           RegisterSpace  = 0; // ���W�X�^���
		size_t           Count          = 1; // 32 bit�̒萔�����蓖�Ă鐔

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