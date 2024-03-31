//////////////////////////////////////////////////////////////////////////////////
///  @file   DirectX12RHIInstance.hpp
///  @brief  GraphicsAPIを選択し, 以降でGraphicsAPIに対応するRenderResourceを呼び出すためのクラス@n
///          そのほか, 物理デバイスの選択やCPU, GPUデバッガの有無なども登録することが出来ます.@n 
/// 　　　　　　DirectX12AgilitySDKのバージョンを最新版にするためには, cpp上のD3D12SDKVersionを設定しておく必要があります
///  @author Toide Yutaro
///  @date   2022_09_05
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_RHI_INSTANCE_HPP
#define DIRECTX12_RHI_INSTANCE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIInstance.hpp"
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
	*				  			RHIInstance
	*************************************************************************//**
	/* @class  RHIInstance
	*  @brief  GraphicsAPIを選択し, 以降でGraphicsAPIに対応するRenderResourceを呼び出すためのクラス@n
    *          そのほか, 物理デバイスの選択やCPU, GPUデバッガの有無なども登録することが出来ます.@n 
    * 　　　　　　DirectX12AgilitySDKのバージョンを最新版にするためには, cpp上のD3D12SDKVersionを設定しておく必要があります
	*****************************************************************************/
	class RHIInstance : public core::RHIInstance, public gu::EnableSharedFromThis<RHIInstance>
	{
	public:
		#pragma region Public Function

		/*!**********************************************************************
		*  @brief     最も性能が高い物理デバイスを自動で選定します. 高い順にxGPU(外部GPU), dGPU(discrete GPU), iGPU (integrated GPU)の順に優先されます
		*  @note      DirectX12では外部GPU, ディスクリートGPU, 統合GPUの順に選択されます.　@n
		*  @return gu::SharedPointer<RHIDisplayAdapter> DisplayAdapterのポインタ
		*************************************************************************/
		virtual gu::SharedPointer<core::RHIDisplayAdapter> SearchHighPerformanceAdapter() override;
		
		/*!**********************************************************************
		*  @brief     最も性能が低い(消費電力が低い)物理デバイスを自動で選定します. 高い順にiGPU (integrated GPU), dGPU(discrete GPU), xGPU(外部GPU)の順に優先されます
		*  @note      DirectX12では統合GPU, ディスクリートGPU, 外部GPUの順に選択されます.　@n
		*  @return gu::SharedPointer<RHIDisplayAdapter> DisplayAdapterのポインタ
		*************************************************************************/
		virtual gu::SharedPointer<core::RHIDisplayAdapter> SearchMinimumPowerAdapter ()override;
		
		/*!**********************************************************************
		*  @brief     全ての利用可能な物理デバイスを配列の形で返します.
		* @attention この関数は, 配列の順番が特に指定がありません. 
		*  @return    gu::DynamicArray<gu::SharedPointer<RHIDisplayAdapter>> : 物理デバイスの配列
		*************************************************************************/
		virtual gu::DynamicArray<gu::SharedPointer<core::RHIDisplayAdapter>> EnumrateAdapters() override;
		
		/*!**********************************************************************
		*  @brief     出力欄に全ての物理デバイスを記入します
		*************************************************************************/
		void LogAdapters() override;

		#pragma endregion Public Function

		#pragma region Public Member Function
		/*!**********************************************************************
		*  @brief     Factoryポインタを返します
		*************************************************************************/
		FactoryComPtr GetFactory() const noexcept { return _factory; }

		/*!**********************************************************************
		*  @brief     DirectXAgilitySDKがロード済みかを返します
		*************************************************************************/
		#if D3D12_CORE_ENABLED
		bool HasLoadedDirectXAgilitySDK() const;
		#endif

		/*IDREDSettings*  GetDREDSettings () const noexcept { return _useDRED ? _dredSettings.Get() : nullptr; }
		IDREDSettings1* GetDREDSettings1() const noexcept { return _useDREDContext ? _dredSettings1.Get() : nullptr; }*/
		
		#pragma endregion

		#pragma region Public Constructor and Destructor

		/*! @brief : デフォルトコンストラクタ*/
		RHIInstance() = default;

		/*! @brief : Debuggerを使うコンストラクタ*/
		RHIInstance(const core::RHIDebugCreateInfo& debugCreateInfo); 
		
		~RHIInstance();

		#pragma endregion

	protected:
		#pragma region Protected Function
		/*! @brief : CPU debugger*/
		void EnabledDebugLayer();            // debug mode only use

		/* @brief : GPU debugger*/
		void EnabledShaderBasedValidation(); // It has a significant impact on the frame rate.
		
		/* @brief : Acquire the debug information when the Device is removed.*/
		void EnabledGPUClashDebuggingModes(); 

		/*!**********************************************************************
		*  @brief     高性能または最小電力を示すAdapterを選択（（高）xGPU、dGPU iGPU（低）
		*  @return    gu::SharedPointer<RHIDisplayAdapter> : 物理デバイスのポインタ
		*************************************************************************/
		gu::SharedPointer<core::RHIDisplayAdapter> SearchAdapter(const DXGI_GPU_PREFERENCE preference) const;

		#pragma endregion

		#pragma region Protected Member Variables
		/*! @brief コマンドラインで指定されている場合, WARPデバイスで作るようにする
		*   @note  コマンドラインでは"warp"コマンドを打ってください*/
		static bool SHOULD_CREATE_WITH_WARP;

		/*! @brief コマンドラインで指定されている場合, ソフトウェアレンダリングを許可する
		*   @note  コマンドラインでは, "allowSoftwareRenderingを打ってください"*/
		static bool ALLOW_SOFTWARE_RENDERING;

		FactoryComPtr _factory = nullptr;

		bool _useDRED        = false;
		bool _useDREDContext = false;
		bool _useLightWeightDRED = false;

		#pragma endregion

	};
}

#endif