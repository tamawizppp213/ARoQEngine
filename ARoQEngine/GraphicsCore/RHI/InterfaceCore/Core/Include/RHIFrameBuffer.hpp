//////////////////////////////////////////////////////////////////////////////////
///  @file   RHIFrameBuffer.hpp
///  @brief  レンダーターゲットとデプスステンシルのテクスチャやResourceViewを使用するクラスです.
///  @author Toide Yutaro
///  @date   2024_07_21
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_FRAME_BUFFER_HPP
#define RHI_FRAME_BUFFER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////

namespace rhi::core
{
	class RHIDevice;
	class GPUTexture;
	class RHIRenderPass;
	class GPUResourceView;
	/****************************************************************************
	*				  			RHIFrameBuffer
	****************************************************************************/
	/* @brief   レンダーターゲットとデプスステンシルのテクスチャやResourceViewを使用するクラスです.
	*****************************************************************************/
	class RHIFrameBuffer : public gu::NonCopyable
	{
	protected:
		using TexturePtr      = gu::SharedPointer<GPUTexture>;
		using ResourceViewPtr = gu::SharedPointer<GPUResourceView>;
	public:
		#pragma region Public Function
		#pragma endregion

		#pragma region Public Property

		/*!**********************************************************************
		*  @brief     レンダーターゲットの描画枚数を指定します. 
		*  @param[in] void
		*  @return    gu::uint32 レンダーターゲットの枚数
		*************************************************************************/
		__forceinline gu::uint32 GetRenderTargetSize() const { return static_cast<gu::uint32>(_renderTargets.Size()); };

		/*!**********************************************************************
		*  @brief     レンダーターゲットのポインタを返します.
		*  @param[in] const gu::uint32 レンダーターゲットのインデックス
		*  @return    TexturePtr レンダーターゲットのポインタ
		*************************************************************************/
		__forceinline TexturePtr GetRenderTarget(const gu::uint32 index = 0) const { return _renderTargets[index]; };

		/*!**********************************************************************
		*  @brief     全てのレンダーターゲット配列を返します.
		*  @param[in] void
		*  @return    gu::DynamicArray<TexturePtr>& レンダーターゲットの配列
		*************************************************************************/
		__forceinline const gu::DynamicArray<TexturePtr>& GetRenderTargets() const { return _renderTargets; }

		/*!**********************************************************************
		*  @brief     DepthStencilのポインタを返します.
		*  @param[in] void
		*  @return    TexturePtr DepthStencilのポインタ
		*************************************************************************/
		__forceinline TexturePtr GetDepthStencil() const noexcept { return _depthStencil; };

		/*!**********************************************************************
		*  @brief     レンダーターゲットが設定しているビューポートを取得します. 
		*  @param[in] const gu::uint32 index レンダーターゲットのインデックス
		*  @return    Viewport ビューポート
		*************************************************************************/
		Viewport    GetFullViewport(const gu::uint32 index = 0) const noexcept;

		/*!**********************************************************************
		*  @brief     レンダーターゲットが設定しているシザー矩形を取得します.
		*  @param[in] const gu::uint32 index レンダーターゲットのインデックス
		*  @return    ScissorRect シザー矩形
		*************************************************************************/
		ScissorRect GetFullScissorRect(const gu::uint32 index = 0) const noexcept;

		/*!**********************************************************************
		*  @brief     レンダーターゲットが設定しているRender Target View配列を取得します.
		*  @param[in] void
		*  @return    const gu::DynamicArray<ResourceViewPtr>& RTV配列
		*************************************************************************/
		__forceinline const gu::DynamicArray<ResourceViewPtr>& GetRenderTargetViews() const { return _renderTargetViews; }

		/*!**********************************************************************
		*  @brief     レンダーターゲットが設定しているRender Target Viewを取得します.
		*  @param[in] void
		*  @return    const gu::DynamicArray<ResourceViewPtr>& RTV配列
		*************************************************************************/
		__forceinline ResourceViewPtr GetRenderTargetView(const gu::uint32 index = 0) const { return _renderTargetViews[index]; }

		/*!**********************************************************************
		*  @brief     レンダーターゲットが設定しているShader Resource View (テクスチャとして使用する場合などに取得)を取得します.
		*  @param[in] const gu::uint32 index レンダーターゲットのインデックス
		*  @return    ResourceViewPtr Shader Resource View
		*************************************************************************/
		__forceinline ResourceViewPtr GetRenderTargetSRV(const gu::uint32 index = 0) const noexcept { return _renderTargetSRVs[index]; }

		/*!**********************************************************************
		*  @brief     レンダーターゲットが設定しているUnordered Access View (テクスチャの読み書きを行う場合などに取得)を取得します.
		*  @param[in] const gu::uint32 index レンダーターゲットのインデックス
		*  @return    ResourceViewPtr Unordered Access View
		*************************************************************************/
		__forceinline ResourceViewPtr GetRenderTargetUAV(const gu::uint32 index = 0) const noexcept { return _renderTargetUAVs[index]; }

		/*!**********************************************************************
		*  @brief     デプスステンシルが設定しているDSVを取得します.
		*  @param[in] void
		*  @return    ResourceViewPtr DSV
		*************************************************************************/
		__forceinline ResourceViewPtr GetDepthStencilView() const noexcept { return _depthStencilView; }

		/*!**********************************************************************
		*  @brief     デプスステンシルが設定しているShader Resource Viewを取得します.
		*  @param[in] void
		*  @return    ResourceViewPtr SRV
		*************************************************************************/
		__forceinline ResourceViewPtr GetDepthStencilSRV() const noexcept { return _depthStencilSRV; }

		/*!**********************************************************************
		*  @brief     RenderTargetを設定します. 
		*  @param[in] const gu::DynamicArray<TexturePtr>& textures レンダーターゲットの配列
		*  @return    void
		*************************************************************************/
		void SetRenderTargets(const gu::DynamicArray<TexturePtr>& textures);

		/*!**********************************************************************
		*  @brief     RenderTargetを設定します.
		*  @param[in] const TexturePtr& texture レンダーターゲット
		*  @param[in] const gu::uint32 index レンダーターゲットのインデックス
		*  @return    void
		*************************************************************************/
		void SetRenderTarget(const TexturePtr& texture, const gu::uint32 index = 0);

		/*!**********************************************************************
		*  @brief     デプスステンシルを設定します.
		*  @param[in] const TexturePtr& texture デプスステンシル
		*  @return    void
		*************************************************************************/
		void SetDepthStencil(const TexturePtr& texture);

		#pragma endregion

		#pragma region Public Constructor and Destructor
		#pragma endregion


	protected:
		#pragma region Protected Constructor and Destructor
		/*! @brief デフォルトコンストラクタ*/
		RHIFrameBuffer() = default;

		/*! @brief デストラクタ*/
		virtual ~RHIFrameBuffer();

		/*! @brief 論理デバイス, レンダーパス, 複数のレンダーターゲット, デプスステンシルを使って初期化*/
		explicit RHIFrameBuffer(const gu::SharedPointer<RHIDevice>& device, const gu::SharedPointer<core::RHIRenderPass>& renderPass, 
			const gu::DynamicArray<gu::SharedPointer<GPUTexture>>& renderTargets, const gu::SharedPointer<GPUTexture>& depthStencil = nullptr);

		/*! @brief 論理デバイス, レンダーパス, レンダーターゲット, デプスステンシルを使って初期化*/
		explicit RHIFrameBuffer(const gu::SharedPointer<RHIDevice>& device, const gu::SharedPointer<core::RHIRenderPass>& renderPass, 
			const gu::SharedPointer<GPUTexture>& renderTarget, const gu::SharedPointer<GPUTexture>& depthStencil = nullptr);
		#pragma endregion 

		#pragma region Protected Function
		#pragma endregion

		#pragma region Protected Property
		/*! @brief 論理デバイス*/
		gu::SharedPointer<RHIDevice>     _device     = nullptr;

		/*! @brief レンダーパス*/
		gu::SharedPointer<RHIRenderPass> _renderPass = nullptr;

		/*! @brief レンダーターゲット*/
		gu::DynamicArray<TexturePtr>      _renderTargets     = {nullptr};

		/*! @brief レンダーターゲットビュー*/
		gu::DynamicArray<ResourceViewPtr> _renderTargetViews = {nullptr};

		/*! @brief レンダーターゲットシェーダーリソースビュー*/
		gu::DynamicArray<ResourceViewPtr> _renderTargetSRVs = { nullptr };

		/*! @brief レンダーターゲットアンオーダーアクセスビュー*/
		gu::DynamicArray<ResourceViewPtr> _renderTargetUAVs = { nullptr };

		/*! @brief デプスステンシル*/
		TexturePtr      _depthStencil     = nullptr;

		/*! @brief デプスステンシルビュー*/
		ResourceViewPtr _depthStencilView = nullptr;

		/*! @brief デプスステンシルシェーダーリソースビュー*/
		ResourceViewPtr _depthStencilSRV  = nullptr;
		#pragma endregion

	private:
		#pragma region Private Function
		void CheckResourceFormat();
		#pragma endregion
	};
}
#endif