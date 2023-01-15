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
#include "GameUtility/Math/Include/GMVertex.hpp"
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include <vector>
#include <memory>
#include <string>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class LowLevelGraphicsEngine;
namespace rhi::core
{
	struct GPUBufferMetaData;
	class  GPUBuffer;
}
//////////////////////////////////////////////////////////////////////////////////
//                         Mesh Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc
{
	struct PrimitiveMesh;
	/****************************************************************************
	*				  			Mesh
	*************************************************************************//**
	*  @class     Mesh
	*  @brief     Mesh
	*****************************************************************************/
	class Mesh : public NonCopyable
	{
		using LowLevelGraphicsEnginePtr = std::shared_ptr<LowLevelGraphicsEngine>;
		using VertexBufferPtr  = std::shared_ptr<rhi::core::GPUBuffer>;
		using IndexBufferPtr   = std::shared_ptr<rhi::core::GPUBuffer>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		virtual void Draw();

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		std::int32_t GetMaterialID() const { return _materialID; }

		void SetMaterialID(const std::int32_t id) { _materialID = id; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Mesh() = default;
		
		Mesh(const LowLevelGraphicsEnginePtr& engine, const PrimitiveMesh& mesh, const std::int32_t materialID = NEEDLESS, const std::wstring& addName = L"");

		Mesh(const LowLevelGraphicsEnginePtr& engine, 
			const rhi::core::GPUBufferMetaData& vertexInfo,
			const rhi::core::GPUBufferMetaData& indexInfo,
			const int32_t materialID = NEEDLESS,
			const std::wstring& addName = L"");

		virtual ~Mesh();

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void Prepare(const PrimitiveMesh& mesh, const std::wstring& name);
		
		void Prepare(const rhi::core::GPUBufferMetaData& vertexInfo, const rhi::core::GPUBufferMetaData& indexInfo, const std::wstring& name);

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		LowLevelGraphicsEnginePtr _engine = nullptr;

		std::vector<VertexBufferPtr> _vertexBuffers = {};
		IndexBufferPtr   _indexBuffer  = nullptr;
		std::int32_t     _materialID   = NEEDLESS;  // material ID in CBV Allocator
		std::uint64_t    _indexCount   = 0;

		static constexpr std::int32_t NEEDLESS = -1;
	};
}

#endif