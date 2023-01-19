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
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include <memory>
#include <string>

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
}
//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////

namespace gc::core
{

	/****************************************************************************
	*				  			Material
	*************************************************************************//**
	*  @class     Material
	*  @brief     Material Buffer
	*****************************************************************************/
	class Material : public NonCopyable
	{
		using LowLevelGraphicsEnginePtr = std::shared_ptr<LowLevelGraphicsEngine>;
		using GPUBufferPtr              = std::shared_ptr<rhi::core::GPUBuffer>;
		using GPUResourceViewPtr        = std::shared_ptr<rhi::core::GPUResourceView>;
		using RHIDescriptorHeapPtr      = std::shared_ptr<rhi::core::RHIDescriptorHeap>;
		using GPUResourceCachePtr       = std::shared_ptr<rhi::core::GPUResourceCache>;
		
	public:
		enum class UsageTexture
		{
			Diffuse,
			Specular,
			Normal,
			CountOf
		};
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void PackMaterial(const void* data);

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		GPUResourceViewPtr GetMaterialView() const noexcept { return _materialBufferView; }

		GPUResourceViewPtr GetTexture(const UsageTexture usageTexture) const noexcept { return _textures[(int)usageTexture]; }

		void SetTexture(const GPUResourceViewPtr& texture, const UsageTexture textureType) { _textures[(int)textureType] = texture; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Material(const LowLevelGraphicsEnginePtr& engine, const rhi::core::GPUBufferMetaData& bufferInfo, const std::wstring& addName = L"",
			const RHIDescriptorHeapPtr& customHeap = nullptr);

		~Material();

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void SetUpBuffer(const rhi::core::GPUBufferMetaData& bufferInfo, const std::wstring& name);

		/****************************************************************************
		**                Protected Member Variables
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