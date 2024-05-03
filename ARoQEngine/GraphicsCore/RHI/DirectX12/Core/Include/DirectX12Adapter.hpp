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
	/* @class     RHIDisplayAdapter
	*  @brief     論理デバイスに渡す物理デバイス(Apdapter)の設定, GPU情報を取得
	*****************************************************************************/
	class RHIDisplayAdapter : public core::RHIDisplayAdapter, public gu::EnableSharedFromThis<RHIDisplayAdapter>
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief 自身の物理デバイスに基づいて論理デバイスを生成し, そのSharedPointerを渡します.
		*  @param[in] void
		*  @return gu::SharedPointer<core::RHIDevice> 論理デバイスのポインタ
		*************************************************************************/
		gu::SharedPointer<core::RHIDevice> CreateDevice() override;

		/*!**********************************************************************
		*  @brief 物理デバイスの名前とスペックを出力に表示します@n
		*         基本的に実行時のログとして使用するものになります. @n
		*         ファイルや文字列に出力は行わないです.
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		virtual void PrintInfo() const override;

		#pragma endregion Public Function

		#pragma region Public Property
		/*!**********************************************************************
		*  @brief     アダプタに直接接続されている出力の数 (モニターなど)を返します.
		*  @param[in] void
		*  @return    gu::uint64 アダプタに直接されている出力の数
		*************************************************************************/
		virtual gu::uint64 GetOutputCount() const override;

		/*!**********************************************************************
		*  @brief  DirectX12で使用するIAdapterのComptrを返します.
		*  @param[in] void
		*  @return    物理デバイスのComポインタ
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

		#pragma region Protected Property
		/* @brief : DirectX12で使用するAdapterのポインタ*/
		AdapterComPtr _adapter = nullptr;		
		#pragma endregion Protected Property
	};
}

#endif