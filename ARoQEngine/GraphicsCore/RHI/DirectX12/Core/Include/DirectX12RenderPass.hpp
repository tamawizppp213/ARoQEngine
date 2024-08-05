//////////////////////////////////////////////////////////////////////////////////
///  @file   DirectX12RenderPass.hpp
///  @brief  レンダーターゲットとデプスステンシルにおけるFrameBufferの書き込み開始時, 終了時の設定項目
///  @author Toide Yutaro
///  @date   2024_07_21
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_RENDER_PASS_HPP
#define DIRECTX12_RENDER_PASS_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIRenderPass.hpp"
#include "DirectX12Core.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::directX12
{

	/****************************************************************************
	*				  			RHIRenderPass
	****************************************************************************/
	/* @brief     レンダーターゲットとデプスステンシルにおけるFrameBufferの書き込み開始時, 終了時の設定項目
	*****************************************************************************/
	class RHIRenderPass : public  rhi::core::RHIRenderPass
	{
	public:
		#pragma region Public Function

		#pragma endregion

		#pragma region Public Property
		
		/*!**********************************************************************
		*  @brief     複数のレンダーターゲットとデプスステンシルそれぞれのクリアカラーを設定します.
		*  @param[in] const gu::tstring& デバッグ表示名
		*  @return    void
		*************************************************************************/
		virtual void SetName(const gu::tstring& name) override { _name = name; };

		#pragma endregion

		#pragma region Public Constructor and Destructor
		/*! @brief デフォルトコンストラクタ*/
		RHIRenderPass() = default;

		/*! @brief デストラクタ*/
		~RHIRenderPass() = default;

		/*! @brief 各レンダーターゲットとデプスステンシルの初期化*/
		explicit RHIRenderPass(const gu::SharedPointer<core::RHIDevice>& device, const gu::DynamicArray<core::Attachment>& colors, const gu::Optional<core::Attachment>& depth = {}) : 
			core::RHIRenderPass(device, colors, depth){}

		/*! @brief 一つのレンダーターゲットとデプスステンシルを使った初期化*/
		explicit RHIRenderPass(const gu::SharedPointer<core::RHIDevice>& device, const core::Attachment& color, const gu::Optional<core::Attachment>& depth = {}) :
			core::RHIRenderPass(device, color, depth){};

		#pragma endregion

	protected:
		#pragma region Protected Function

		#pragma endregion

		#pragma region Protected Property
		gu::tstring _name = L"";
		#pragma endregion
	};
}
#endif