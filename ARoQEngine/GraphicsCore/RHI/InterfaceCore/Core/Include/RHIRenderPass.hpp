//////////////////////////////////////////////////////////////////////////////////
///  @file   RHIRenderPass.hpp
///  @brief  Render Pass (for use vulkan)
///  @author Toide Yutaro
///  @date   2024_07_21
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_RENDER_PASS_HPP
#define RHI_RENDER_PASS_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
#include "GameUtility/Base/Include/GUOptional.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class RHIDevice;
	class RHIFrameBuffer;
	/****************************************************************************
	*				  			RHIRenderPass
	****************************************************************************/
	/* @brief 　レンダーターゲットとデプスステンシルのFrameBufferを書き込み開始時, 終了時の設定項目
	*****************************************************************************/
	class RHIRenderPass : public gu::NonCopyable
	{
	public:
		#pragma region Public Function

		#pragma endregion

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     レンダーターゲットとデプスステンシルそれぞれのクリアカラーを設定します. 
		*  @param[in] const gu::Optional<ClearValue>& color レンダーターゲットのクリアカラー
		*  @param[in] const gu::Optional<ClearValue>& depth デプスステンシルのクリアカラー
		*  @param[in] const gu::uint32 index レンダーターゲットのインデックス
		*  @return    void
		*************************************************************************/
		void SetClearValue(const gu::Optional<ClearValue>& color, const gu::Optional<ClearValue>& depth = {}, const gu::uint32 index = 0)
		{
			if (color.HasValue())
			{
				_colorClearValues[index] = color.Value();
			}
			_depthClearValue = depth; // optional
		}

		/*!**********************************************************************
		*  @brief     複数のレンダーターゲットとデプスステンシルそれぞれのクリアカラーを設定します.
		*  @param[in] const gu::DynamicArray<ClearValue>& colors レンダーターゲットのクリアカラー
		*  @param[in] const gu::Optional<ClearValue>&     depth デプスステンシルのクリアカラー
		*  @return    void
		*************************************************************************/
		void SetClearValue(const gu::DynamicArray<ClearValue>& colors, const gu::Optional<ClearValue>& depth = {})
		{
			_colorClearValues = colors;
			_depthClearValue  = depth;
		}

		/*!**********************************************************************
		*  @brief     色アタッチメントの数は、レンダーターゲットの数以上でなければならない. 深度ステンシルが存在する場合、深度アタッチメントをNULLにすることはできない。
		*  @param[in] void
		*  @return    bool
		*************************************************************************/
		bool Compatible(const gu::SharedPointer<RHIFrameBuffer>& frameBuffer) const;
	
		/*!**********************************************************************
		*  @brief     マルチサンプル数を取得します.
		*  @param[in] void
		*  @return    MultiSample マルチサンプル数
		*************************************************************************/
		__forceinline MultiSample GetMaxSample() const noexcept { return _maxSample; }
		
		/*!**********************************************************************
		*  @brief    　レンダーターゲット側のクリアカラーを取得します.
		*  @param[in] void
		*  @return    gu::DynamicArray<ClearValue> レンダーターゲットのクリアカラー
		*************************************************************************/
		__forceinline const gu::DynamicArray<ClearValue>& GetClearColor() const noexcept { return _colorClearValues; }
		
		/*!**********************************************************************
		*  @brief    　デプスステンシル側のクリアカラーを取得します.
		*  @param[in] void
		*  @return    gu::Optional<ClearColor> デプスステンシルのクリアカラー
		*************************************************************************/
		__forceinline gu::Optional<ClearValue> GetDepthClear() const noexcept { return _depthClearValue; }

		/*!**********************************************************************
		*  @brief     レンダーターゲット側のアタッチメントを取得します.
		*  @param[in] void
		*  @return    gu::Optional<Attachment> レンダーターゲットのアタッチメント
		*************************************************************************/
		__forceinline gu::Optional<Attachment> GetColorAttachment(const gu::uint32 index = 0) const { return _colorAttachments[index]; }
	
		/*!**********************************************************************
		*  @brief     デプスステンシル側のアタッチメントを取得します.
		*  @param[in] void
		*  @return    gu::Optional<Attachment> デプスステンシルのアタッチメント
		*************************************************************************/
		__forceinline gu::Optional<Attachment> GetDepthAttachment() const noexcept { return _depthAttachment; }
		
		/*!**********************************************************************
		*  @brief     レンダーターゲット側のアタッチメントの数を取得します.
		*  @param[in] void
		*  @return    gu::uint32 レンダーターゲットのアタッチメントの数
		*************************************************************************/
		__forceinline gu::uint32 GetColorAttachmentSize() const noexcept { return static_cast<gu::uint32>(_colorAttachments.Size()); }

		/*!**********************************************************************
		*  @brief     複数のレンダーターゲットとデプスステンシルそれぞれのクリアカラーを設定します.
		*  @param[in] const gu::tstring& デバッグ表示名
		*  @return    void
		*************************************************************************/
		virtual void SetName(const gu::tstring& name) = 0;

		#pragma endregion

		#pragma region Public Constructor and Destructor

		#pragma endregion
		
	protected:
		#pragma region Protected Constructor and Destructor
		/*! @brief デフォルトコンストラクタ*/
		RHIRenderPass() = default;

		/*! @brief デストラクタ*/
		virtual ~RHIRenderPass()
		{
			_depthClearValue.Reset();
			_depthAttachment.Reset();
			_colorClearValues.Clear(); _colorClearValues.ShrinkToFit();
			_colorAttachments.Clear(); _colorAttachments.ShrinkToFit();

			if (_device) { _device.Reset(); }
		}

		/*! @brief 各レンダーターゲットとデプスステンシルの初期化*/
		explicit RHIRenderPass(const gu::SharedPointer<RHIDevice>& device, const gu::DynamicArray<Attachment>& colors, const gu::Optional<Attachment>& depth = {});
		
		/*! @brief 一つのレンダーターゲットとデプスステンシルを使った初期化*/
		explicit RHIRenderPass(const gu::SharedPointer<RHIDevice>& device, const Attachment& color, const gu::Optional<Attachment>& depth = {});

		#pragma endregion

		#pragma region Protected Function

		#pragma endregion

		#pragma region Protected Property
		/*! @brief 論理デバイス*/
		gu::SharedPointer<RHIDevice> _device = nullptr;

		/* @brief レンダーターゲットの書き込み設定項目*/
		gu::DynamicArray<Attachment> _colorAttachments = {};

		/*! @brief レンダーターゲットのクリア時に初期化される色*/
		gu::DynamicArray<ClearValue> _colorClearValues = {};

		/*! @brief デプスステンシルの書き込み時設定項目*/
		gu::Optional<Attachment> _depthAttachment;

		/*! @brief デプスステンシルのクリア時に初期化される色*/
		gu::Optional<ClearValue> _depthClearValue;

		/*! @brief マルチサンプル数*/
		MultiSample _maxSample = MultiSample::Count1;
		#pragma endregion 
	};
}
#endif