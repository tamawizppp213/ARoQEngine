//////////////////////////////////////////////////////////////////////////////////
///             @file   Mesh.hpp
///             @brief  Mesh 
///             @author Copyright(c) Pocol. All right reserved.
///                     Partially edit by Toide Reference : DirectX12 実践ガイド 
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
	/* @brief  メッシュを描画するためのクラス.
	*****************************************************************************/
	class Mesh : public gu::NonCopyable
	{
		using LowLevelGraphicsEnginePtr = gu::SharedPointer<LowLevelGraphicsEngine>;
		using MaterialPtr               = gu::SharedPointer<Material>;
	public:
		#pragma region Public Function
		
		virtual void Draw(const RHICommandListPtr& graphicsCommandList, const gu::uint32 frameIndex);

		#pragma endregion

		#pragma region Public Property 
		const gu::DynamicArray<GPUBufferPtr>& GetVertexBuffers() const noexcept { return _vertexBuffers; }

		GPUBufferPtr GetIndexBuffer() const noexcept { return _indexBuffer; }

		MaterialPtr GetMaterial() const noexcept { return _material; }

		void SetMaterial(const MaterialPtr& material) { _material = material; }
		#pragma endregion

		#pragma region Public Constructor and Destructor
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
			const gu::DynamicArray<GPUBufferPtr>& vertexBuffers,
			const GPUBufferPtr& indexBuffer,
			const gu::uint64 indexCount = 0,
			const gu::uint32 indexOffset = 0,
			const MaterialPtr& material = nullptr);

		virtual ~Mesh();
		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor
		#pragma endregion

		#pragma region Protected Function
		void Prepare(const PrimitiveMesh& mesh, const gu::tstring& name);
		
		void Prepare(const rhi::core::GPUBufferMetaData& vertexInfo, const rhi::core::GPUBufferMetaData& indexInfo, const gu::tstring& name);

		#pragma endregion
		#pragma region Protected Property
		LowLevelGraphicsEnginePtr _engine = nullptr;

		/* @brief : frame count size vertex buffer*/
		gu::DynamicArray<GPUBufferPtr> _vertexBuffers = {};

		/* @brief : index data buffer*/
		GPUBufferPtr _indexBuffer  = nullptr;

		/* @brief : If you use material, you should set this variable.*/
		MaterialPtr _material = nullptr;

		gu::uint64 _indexCount  = 0; // index bufferのCountはIndexBuffer全体のCount数だが, こっちはMaterialに対応したIndexCount
		gu::uint32 _indexOffset = 0;

		bool _hasCreatedNewBuffer = false;

		#pragma endregion

	};
}

#endif