//////////////////////////////////////////////////////////////////////////////////
///             @file   Mesh.hpp
///             @brief  Mesh 
///             @author Copyright(c) Pocol. All right reserved.
///                     Partially edit by Toide Reference : DirectX12 é¿ëHÉKÉCÉh 
///             @date   2022_06_02
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef MESH_HPP
#define MESH_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHITypeCore.hpp"
#include "GameUtility/Math/Include/GMVertex.hpp"
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Base/Include/GUString.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class LowLevelGraphicsEngine;

//////////////////////////////////////////////////////////////////////////////////
//                         Mesh Class
//////////////////////////////////////////////////////////////////////////////////
namespace engine
{
	struct PrimitiveMesh;
	class  Material;
	/****************************************************************************
	*				  			Mesh
	****************************************************************************/
	/* @class     Mesh
	*  @brief     Mesh
	*****************************************************************************/
	class Mesh : public gu::NonCopyable
	{
		using LowLevelGraphicsEnginePtr = gu::SharedPointer<LowLevelGraphicsEngine>;
		using VertexBufferPtr  = GPUBufferPtr;
		using IndexBufferPtr   = GPUBufferPtr;
		using MaterialPtr      = gu::SharedPointer<Material>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		virtual void Draw(const gu::SharedPointer<rhi::core::RHICommandList>& graphicsCommandList, 
			const std::uint32_t frameIndex);

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		const gu::DynamicArray<VertexBufferPtr>& GetVertexBuffers() const noexcept { return _vertexBuffers; }

		IndexBufferPtr GetIndexBuffer() const noexcept { return _indexBuffer; }

		MaterialPtr GetMaterial() const noexcept { return _material; }

		void SetMaterial(const MaterialPtr& material) { _material = material; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Mesh() = default;
		
		Mesh(const LowLevelGraphicsEnginePtr& engine, const PrimitiveMesh& mesh, const MaterialPtr& material = nullptr, const gu::tstring& addName = SP(""));

		/* @brief : This constructor is used when creating new vb and ib*/
		Mesh(const LowLevelGraphicsEnginePtr& engine, 
			const rhi::core::GPUBufferMetaData& vertexInfo,
			const rhi::core::GPUBufferMetaData& indexInfo,
			const MaterialPtr& material = nullptr,
			const gu::tstring& addName = SP(""));

		// @brief : This constructor is used, when sharing one VB and Index and separating each material. (e.g. Model drawing)
		Mesh(const LowLevelGraphicsEnginePtr& engine,
			const gu::DynamicArray<VertexBufferPtr>& vertexBuffers,
			const IndexBufferPtr& indexBuffer,
			const std::uint64_t indexCount = 0,
			const std::uint32_t indexOffset = 0,
			const MaterialPtr& material = nullptr);

		virtual ~Mesh();

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void Prepare(const PrimitiveMesh& mesh, const gu::tstring& name);
		
		void Prepare(const rhi::core::GPUBufferMetaData& vertexInfo, const rhi::core::GPUBufferMetaData& indexInfo, const gu::tstring& name);


		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		LowLevelGraphicsEnginePtr _engine = nullptr;

		/* @brief : frame count size vertex buffer*/
		gu::DynamicArray<VertexBufferPtr> _vertexBuffers = {};

		/* @brief : index data buffer*/
		IndexBufferPtr _indexBuffer  = nullptr;

		/* @brief : If you use material, you should set this variable.*/
		MaterialPtr _material = nullptr;

		std::uint64_t _indexCount  = 0; // index bufferÇÃCountÇÕIndexBufferëSëÃÇÃCountêîÇæÇ™, Ç±Ç¡ÇøÇÕMaterialÇ…ëŒâûÇµÇΩIndexCount
		std::uint32_t _indexOffset = 0;

		bool _hasCreatedNewBuffer = false;

	};
}

#endif