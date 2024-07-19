//////////////////////////////////////////////////////////////////////////////////
///  @file   FullScreenEffector.hpp
///  @brief  このクラスは主にポスト・エフェクトのために使われる @n
///          フルスクリーンのポリゴンを描画するためのインターフェース.
///  @author Toide Yutaro
///  @date   2024_07_15
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef FULL_SCREEN_EFFECTOR_HPP
#define FULL_SCREEN_EFFECTOR_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
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
	class RHIResourceLayout;
	class GPUResourceView;
	class GPUGraphicsPipelineState;
	class GPUComputePipelineState;
	class GPUBuffer;
	class GPUTexture;
	class RHIRenderPass;
	class GPUShaderState;
}
//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace engine
{
	/****************************************************************************
	*				  			IFullScreenEffector
	****************************************************************************/
	/* @brief  フルスクリーンのポリゴンを描画するためのインターフェース. 
	*****************************************************************************/
	class IFullScreenEffector : public gu::NonCopyable
	{
	protected:
		using VertexBufferPtr           = gu::SharedPointer<rhi::core::GPUBuffer>;
		using IndexBufferPtr            = gu::SharedPointer<rhi::core::GPUBuffer>;
		using ResourceLayoutPtr         = gu::SharedPointer<rhi::core::RHIResourceLayout>;
		using ResourceViewPtr           = gu::SharedPointer<rhi::core::GPUResourceView>;
		using PipelineStatePtr          = gu::SharedPointer<rhi::core::GPUGraphicsPipelineState>;
		using ComputePipelineStatePtr   = gu::SharedPointer<rhi::core::GPUComputePipelineState>;
		using LowLevelGraphicsEnginePtr = gu::SharedPointer<LowLevelGraphicsEngine>;

	public:
		#pragma region Public Function

		/*!**********************************************************************
		*  @brief     画面のサイズを変更します. 
		*  @param[in] const gu::uint32 新しい幅
		*  @param[in] const gu::uint32 新しい高さ
		*  @return    void
		*************************************************************************/
		virtual void OnResize(const gu::uint32 newWidth, const gu::uint32 newHeight) = 0;

		/*!**********************************************************************
		*  @brief     コマンドリストに描画内容を登録します
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		virtual void Draw() = 0;

		#pragma endregion

	    #pragma region Public Property

		#pragma endregion

	protected:
		#pragma region Protected Constructor and Destructor 

		/*! @brief デフォルトコンストラクタ*/
		IFullScreenEffector();

		/*! @brief デストラクタ*/
		virtual ~IFullScreenEffector();

		/*! @brief グラフィクスエンジンを使って初期化*/
		IFullScreenEffector(const LowLevelGraphicsEnginePtr& engine);

		#pragma endregion

		#pragma region Protected Function
		/*!**********************************************************************
		*  @brief     Resize可能かを判定します
		*  @param[in] const gu::uint32 幅
		*  @param[in] const gu::uint32 高さ
		*  @return    bool
		*************************************************************************/
		__forceinline bool EnableResize(const gu::uint32 newWidth, const gu::uint32 newHeight) const 
		{
			return newWidth != _width || newHeight != _height;
		}

		/*!**********************************************************************
		*  @brief     FullScreenStateの基本のグラフィクスパイプラインの設定を行います.
		*  @param[in] const gu::SharedPointer<rhi::core::RHIRenderPass>& レンダーパス
		*  @param[in] const gu::SharedPointer<rhi::core::RHIResourceLayout>& リソースレイアウト
		*  @param[in] const gu::SharedPointer<rhi::core::GPUShaderState>& 頂点シェーダ
		*  @param[in] const gu::SharedPointer<rhi::core::GPUShaderState>& ピクセルシェーダ
		*  @return    void
		*************************************************************************/
		PipelineStatePtr CreateDefaultFullScreenGraphicsPipelineState(
			const gu::SharedPointer<rhi::core::RHIRenderPass>& renderPass, 
			const gu::SharedPointer<rhi::core::RHIResourceLayout>& resourceLayout,
			const gu::SharedPointer<rhi::core::GPUShaderState>& vs,
			const gu::SharedPointer<rhi::core::GPUShaderState>& ps) const;

		/*!**********************************************************************
		*  @brief     FullScreenStateの基本のComputeパイプラインの設定を行います.
		*  @param[in] const gu::SharedPointer<rhi::core::RHIResourceLayout>& リソースレイアウト
		*  @param[in] const gu::SharedPointer<rhi::core::GPUShaderState>& コンピュートシェーダー
		*  @return    void
		*************************************************************************/
		ComputePipelineStatePtr CreateDefaultFullScreenComputePipelineState(
			const gu::SharedPointer<rhi::core::RHIResourceLayout>& resourceLayout,
			const gu::SharedPointer<rhi::core::GPUShaderState>& cs);

		/*!**********************************************************************
		*  @brief     パイプラインステートを準備します.
		*  @param[in] const gu::tstring& デバッグ表示名
		*  @return    void
		*************************************************************************/
		virtual void PreparePipelineState(const gu::tstring& addName) = 0;

		/*!**********************************************************************
		*  @brief     頂点バッファとインデックスバッファを準備します.
		*  @param[in] const gu::tstring& デバッグ表示名
		*  @return    void
		*************************************************************************/
		void PrepareVertexAndIndexBuffer(const gu::tstring& addName);
		
		/*!**********************************************************************
		*  @brief     デバッグ表示名を準備します. ただし, デフォルト値でFSEffector::が追加されます.
		*  @param[in] const gu::tstring 加えたい文字列
		*  @param[in] const gu::tstring 現在の文字列
		*  @return    gu::tstring
		*************************************************************************/
		gu::tstring CombineDebugName(const gu::tstring& addName, const gu::tstring& original) const;

		#pragma endregion

		#pragma region Protected Property

		/* @brief : 長方形の頂点バッファ*/
		gu::DynamicArray<VertexBufferPtr> _vertexBuffers = {};
		
		/*! @brief 長方形のインデックスバッファ*/
		IndexBufferPtr  _indexBuffer = nullptr;
		
		/*! @brief マクロの使用 (カンマ区切りで)*/
		gu::DynamicArray<gu::tstring> _useMacros = {};

		/*! @brief グラフィックスパイプライン*/
		PipelineStatePtr  _pipeline       = nullptr;

		/*! @brief コンピュートパイプライン*/
		ComputePipelineStatePtr _computePipeline = nullptr;

		/*! @brief GPUのリソースのバインド方法を設定するリソースレイアウト*/
		ResourceLayoutPtr _resourceLayout = nullptr;
		
		/*! @brief GPUのリソースビュー*/
		gu::DynamicArray<ResourceViewPtr> _resourceViews = {};

		/* @brief : 幅*/
		gu::uint32 _width  = 0;

		/* @brief : 高さ*/
		gu::uint32 _height = 0;

		/* @brief : device and command list*/
		LowLevelGraphicsEnginePtr _engine = nullptr;

		#pragma endregion
	};
}
#endif