//////////////////////////////////////////////////////////////////////////////////
///             @file   Material.hpp
///             @brief  Material
///             @author Copyright(c) Pocol. All right reserved.
///                     Partially edit by Toide Reference : DirectX12 é¿ëHÉKÉCÉh 
///             @date   2022_06_04
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef MATERIAL_HPP
#define MATERIAL_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "MaterialType.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
#include "GameUtility/Base/Include/GUString.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class LowLevelGraphicsEngine;
namespace rhi::core
{
	class RHIDescriptorHeap;
	class GPUBuffer;
	class GPUResourceView;
	class GPUResourceCache;
	class RHICommandList;
}
//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////

namespace engine
{
	enum class UsageTexture
	{
		Diffuse,
		Specular,
		Normal,
		CountOf
	};

	/****************************************************************************
	*				  			Material
	****************************************************************************/
	/* @class     Material
	*  @brief     Material Buffer
	*****************************************************************************/
	class Material : public gu::NonCopyable
	{
		using LowLevelGraphicsEnginePtr = gu::SharedPointer<LowLevelGraphicsEngine>;
		using GPUResourceViewPtr        = gu::SharedPointer<rhi::core::GPUResourceView>;
		using RHIDescriptorHeapPtr      = gu::SharedPointer<rhi::core::RHIDescriptorHeap>;
		using GPUResourceCachePtr       = gu::SharedPointer<rhi::core::GPUResourceCache>;
		
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		virtual void Bind(const gu::SharedPointer<rhi::core::RHICommandList>& graphicsCommandList, const std::uint32_t frameIndex, 
			const std::uint32_t materialID, // bind material constant buffer id
			const gu::DynamicArray<std::uint32_t>& textureIDs
		);

		void PackMaterial(const void* data);

		GPUResourceViewPtr LoadTexture(const gu::tstring& filePath, const UsageTexture textureType);

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		GPUResourceViewPtr GetMaterialView() const noexcept { return _materialBufferView; }

		GPUResourceViewPtr GetTexture(const UsageTexture usageTexture) const noexcept { return _textures[(int)usageTexture]; }

		void SetTexture(const GPUResourceViewPtr& texture, const UsageTexture textureType) { _textures[(int)textureType] = texture; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Material() = default;

		Material(const LowLevelGraphicsEnginePtr& engine, 
			const rhi::core::GPUBufferMetaData& bufferInfo, 
			const gu::tstring& addName = SP(""),
			const RHIDescriptorHeapPtr& customHeap = nullptr);

		~Material();

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void SetUpBuffer(const rhi::core::GPUBufferMetaData& bufferInfo, const gu::tstring& name);

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		LowLevelGraphicsEnginePtr _engine = nullptr;

		GPUResourceViewPtr   _materialBufferView = nullptr;

		GPUResourceViewPtr   _textures[(int)UsageTexture::CountOf] = {};

		RHIDescriptorHeapPtr _customHeap = nullptr;

	private:
		static std::uint64_t InstanceCount;

		static GPUResourceCachePtr ResourceCache;
	};
}
#endif