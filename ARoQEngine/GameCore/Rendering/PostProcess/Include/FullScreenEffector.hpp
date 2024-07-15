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
	class GPUBuffer;
	class GPUTexture;
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
		*  @brief     パイプラインステートを準備します.
		*  @param[in] const gu::tstring& デバッグ表示名
		*  @return    void
		*************************************************************************/
		virtual void PreparePipelineState(const gu::tstring& addName) = 0;

		virtual void PrepareResourceView() = 0;
		
		void PrepareVertexAndIndexBuffer(const gu::tstring& addName);
		
		/*!**********************************************************************
		*  @brief     デバッグ表示名を準備します. ただし, デフォルト値でFSEffector::が追加されます.
		*  @param[in] const gu::tstring& デバッグ表示名
		*  @return    void
		*************************************************************************/
		gu::tstring DefineDebugName(const gu::tstring& addName = SP(""))
		{
			gu::tstring name = SP(""); if (addName != SP("")) { name += addName; name += SP("::"); }
			name += SP("FSEffector::"); // 後に各リソース名称が追加されます.
			return name;
		}

		#pragma endregion

		#pragma region Protected Property

		/* @brief : frame resources*/
		gu::DynamicArray<VertexBufferPtr> _vertexBuffers = {};
		
		gu::DynamicArray<IndexBufferPtr>  _indexBuffers = {};
		
		PipelineStatePtr  _pipeline       = nullptr;
		
		ResourceLayoutPtr _resourceLayout = nullptr;
		
		gu::DynamicArray<ResourceViewPtr> _resourceViews = {};

		gu::uint32 _width  = 0;
		gu::uint32 _height = 0;

		/* @brief : device and command list*/
		LowLevelGraphicsEnginePtr _engine = nullptr;

		#pragma endregion
	};
}
#endif