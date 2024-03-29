//////////////////////////////////////////////////////////////////////////////////
///  @file   DirectX12Adapter.hpp
///  @brief  論理デバイスに渡す物理デバイス(Apdapter)の設定, GPU情報を取得
///  @author Toide Yutaro
///  @date   2024_03_29
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_RHI_ADAPTER_HPP
#define DIRECTX12_RHI_ADAPTER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIAdapter.hpp"
#include "DirectX12Core.hpp"
#include <dxgi1_6.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::directX12
{
	/****************************************************************************
	*				  			RHIDisplayAdapter
	*************************************************************************//**
	*  @class     RHIDisplayAdapter
	*  @brief     論理デバイスに渡す物理デバイス(Apdapter)の設定, GPU情報を取得
	*****************************************************************************/
	class RHIDisplayAdapter : public core::RHIDisplayAdapter, public gu::EnableSharedFromThis<RHIDisplayAdapter>
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief 自身の物理デバイスに基づいて論理デバイスを生成し, そのSharedPointerを渡します.
		*************************************************************************/
		gu::SharedPointer<core::RHIDevice> CreateDevice() override;

		/*!**********************************************************************
		*  @brief 物理デバイスの名前とスペックを出力に表示します@n
		*         基本的に実行時のログとして使用するものになります. @n
		*         ファイルや文字列に出力は行わないです.
		*************************************************************************/
		virtual void PrintInfo() const override;

		#pragma endregion Public Function

		#pragma region Public Member Variables
		/*!**********************************************************************
		*  @brief  アダプタに直接接続されている出力の数 (モニターなど)を返します.
		*************************************************************************/
		virtual gu::uint64 GetOutputCount() const override;

		/*!**********************************************************************
		*  @brief  DirectX12で使用するIAdapterのComptrを返します.
		*************************************************************************/
		AdapterComPtr GetAdapter() { return _adapter; }

		#pragma endregion

		#pragma region Public Constructor and Destructor
		
		/*! @brief Default Constructor*/
		RHIDisplayAdapter(const gu::SharedPointer<core::RHIInstance>& instance, const AdapterComPtr& adapter);

		/*! @brief Destructor */
		~RHIDisplayAdapter();

		#pragma endregion 

	protected:
		#pragma region Protected Function

		#pragma endregion

		#pragma region Protected Member Variables
		/* @brief : DirectX12で使用するAdapterのポインタ*/
		AdapterComPtr _adapter = nullptr;		
		#pragma endregion Protected Member Variables
	};
}

#endif