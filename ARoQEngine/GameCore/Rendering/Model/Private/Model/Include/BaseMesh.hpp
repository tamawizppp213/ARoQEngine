//////////////////////////////////////////////////////////////////////////////////
///  @file   BaseMesh.hpp
///  @brief  メッシュの基底クラスです. GPUバッファの作成, メッシュを特定するための共通処理を提供します.
///  @author toide
///  @date   2024/07/27 22:46:33
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef BASE_MESH_HPP
#define BASE_MESH_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHITypeCore.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
#include "GameUtility/Base/Include/GUString.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class LowLevelGraphicsEngine;

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace engine
{
	/****************************************************************************
	*				  	BaseMeshSetting
	****************************************************************************/
	/* @brief  BaseMeshを作成する際の設定
	*****************************************************************************/
	struct BaseMeshSetting
	{
		/*! @brief VBがCPUアクセス可能か (デフォルトはfalse). このフラグにより, 動的にCPU側から頂点データを変更することが出来ます.*/
		bool IsCPUAccessibleVB = false;

		/*! @brief IBがCPUアクセス可能か (デフォルトはfalse). このフラグにより, 動的にCPU側からインデックスデータを変更することが出来ます.*/
		bool IsCPUAccessibleIB = false;

		/*! @brief 32bitのインデックスバッファか*/
		bool Is32BitIndex = false;
	};

	/****************************************************************************
	*				  			   BaseMesh
	****************************************************************************/
	/* @brief  メッシュの基底クラスです. GPUバッファの作成, メッシュを特定するための共通処理を提供します.
	*****************************************************************************/
	class IBaseMesh : public gu::NonCopyable
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     描画を行います. 
		*  @param[in] const RHICommandListPtr& コマンドリスト
		*  @return    void
		*************************************************************************/
		virtual void Draw(const RHICommandListPtr& commandList) = 0;

		#pragma endregion 

		#pragma region Public Property

		#pragma endregion 

		#pragma region Public Operator 

		#pragma endregion 

		#pragma region Public Constructor and Destructor
		/*! @brief デフォルトコンストラクタ*/
		IBaseMesh() = default;

		/*! @brief デストラクタ*/
		virtual ~IBaseMesh() = default;

		IBaseMesh(const gu::SharedPointer<LowLevelGraphicsEngine>& engine, const BaseMeshSetting& setting, const gu::tstring& name);

		#pragma endregion 

	protected:
		#pragma region Protected Constructor and Destructor

		#pragma endregion 

		#pragma region Protected Function
		/*!**********************************************************************
		*  @brief     頂点バッファとインデックスバッファを作成します.
		*  @param[in] const gu::uint32 VertexBufferのStride (1頂点に必要なバイトサイズ)
		*  @param[in] const gu::uint32 VertexBufferの頂点数
		*  @param[in] const gu::uint32 IndexBufferの個数
		*  @param[in] const void* VertexBufferに書き込み始める先頭ポインタ
		*  @param[in] const void* IndexBuffer書き込み始める先頭ポインタ
		*  @return    void
		*************************************************************************/
		void SetupVertexBufferAndIndexBuffer(const gu::uint32 vbStride, const gu::uint32 vbCount, const gu::uint32 ibCount, 
			const void* vbData, const void* ibData);

		/*!**********************************************************************
		*  @brief     頂点バッファを取得します. CPUアクセス可能の場合は現書き込みフレームのバッファを返します.
		*  @param[in] void
		*  @return    GPUBufferPtr
		*************************************************************************/
		GPUBufferPtr GetVertexBuffer() const noexcept;

		/*!**********************************************************************
		*  @brief     インデックスバッファを取得します. CPUアクセス可能の場合は現書き込みフレームのバッファを返します.
		*  @param[in] void
		*  @return    GPUBufferPtr
		*************************************************************************/
		GPUBufferPtr GetIndexBuffer() const noexcept;
		#pragma endregion 

		#pragma region Protected Property

		/*! @brief 描画用のグラフィクスエンジン*/
		gu::SharedPointer<LowLevelGraphicsEngine> _engine = nullptr;

		/*! @brief 名前*/
		gu::tstring _name = SP("");

		/*! @brief 頂点バッファ. CPUがアクセス可能の場合はフレーム分GPUバッファを消費します*/
		gu::DynamicArray<GPUBufferPtr> _vertexBuffer = {};

		/*! @brief インデックスバッファ. CPUがアクセス可能の場合は, フレーム分GPUバッファを消費します.*/
		gu::DynamicArray<GPUBufferPtr> _indexBuffer = {};

		/*! @brief 設定*/
		BaseMeshSetting _baseMeshSetting = {};

		#pragma endregion

	private:
		#pragma region Private Constructor and Destructor

		#pragma endregion

		#pragma region Private Function

		#pragma endregion 

		#pragma region Private Property

		#pragma endregion 

	};
}

#endif