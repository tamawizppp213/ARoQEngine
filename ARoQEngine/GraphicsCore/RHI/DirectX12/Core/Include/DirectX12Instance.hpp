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
	/*!**********************************************************************
	*  @brief  GPUがクラッシュした際のデバッグの設定
	*************************************************************************/
	enum class GPUCrashDebuggingMode
	{
		None = 0x0,
		Dred = 0x1, //!<デバイスが削除された際のデバッグ情報
		All = Dred
	};

	/****************************************************************************
	*				  			RHIInstance
	****************************************************************************/
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
		*  @note      DirectX12では外部GPU, ディスクリートGPU, 統合GPUの順に選択されます.
		*  @param[in] void
		*  @return gu::SharedPointer<RHIDisplayAdapter> DisplayAdapterのポインタ
		*************************************************************************/
		virtual gu::SharedPointer<core::RHIDisplayAdapter> SearchHighPerformanceAdapter() override;
		
		/*!**********************************************************************
		*  @brief     最も性能が低い(消費電力が低い)物理デバイスを自動で選定します. 高い順にiGPU (integrated GPU), dGPU(discrete GPU), xGPU(外部GPU)の順に優先されます
		*  @note      DirectX12では統合GPU, ディスクリートGPU, 外部GPUの順に選択されます.　@n
		*  @param[in] void
		*  @return gu::SharedPointer<RHIDisplayAdapter> DisplayAdapterのポインタ
		*************************************************************************/
		virtual gu::SharedPointer<core::RHIDisplayAdapter> SearchMinimumPowerAdapter ()override;
		
		/*!**********************************************************************
		*  @brief     全ての利用可能な物理デバイスを配列の形で返します.
		*  @attention この関数は, 配列の順番が特に指定がありません. 
		*  @param[in] void
		*  @return    gu::DynamicArray<gu::SharedPointer<RHIDisplayAdapter>> : 物理デバイスの配列
		*************************************************************************/
		virtual gu::DynamicArray<gu::SharedPointer<core::RHIDisplayAdapter>> EnumrateAdapters() const override;
		
		/*!**********************************************************************
		*  @brief     出力欄に全ての物理デバイスを記入します
		*  @param[in] void
		*  @return    void
		*************************************************************************/
		void LogAdapters() const override;

		#pragma endregion Public Function

		#pragma region Public Member Function
		/*!**********************************************************************
		*  @brief     Factoryポインタを返します
		*************************************************************************/
		FactoryComPtr GetFactory() const noexcept { return _factory; }

		/*!**********************************************************************
		*  @brief     GPUがクラッシュした際のデバッグの設定
		*************************************************************************/
		GPUCrashDebuggingMode GetGPUCrashDebuggingMode() const { return _gpuCrashDebuggingMode; }

		#if D3D12_CORE_ENABLED
		/*!**********************************************************************
		*  @brief     DirectXAgilitySDKがロード済みかを返します
		*************************************************************************/
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
		
		/*! @brief デストラクタ*/
		~RHIInstance();

		#pragma endregion

	protected:
		#pragma region Protected Function
		/*!**********************************************************************
		*  @brief    DirectX12におけるCPUのデバッグ機能をOnにする
		*  @note     デバッグモードにしか使用できません
		*************************************************************************/
		void EnabledDebugLayer(); // debug mode only use

		/*!**********************************************************************
		*  @brief     GPUベースのバリデーション
		*  @note      FPSに強く影響を与えることから, Release時には使用しないようにしてください
		*  @details   以下のことが行えます. @n
		*             1. 初期化されていない、または互換性のないディスクリプタをシェーダで使用する @n
		*             2. 削除されたResourceを参照するディスクリプタをシェーダで使用する @n
		*             3. 昇格したリソースの状態とリソースの状態の減衰の検証 @n
		*             4. シェーダーにおけるディスクリプタ・ヒープの終端を超えたインデックス付け @n
		*             5. 互換性のない状態でシェーダーがリソースにアクセスした @n
		*             6. 初期化されていない、または互換性のないサンプラーをシェーダーで使用する @n
		*************************************************************************/
		void EnabledShaderBasedValidation(); // It has a significant impact on the frame rate.
		
		/*!**********************************************************************
		*  @brief     Deviceが削除されたときの情報を記述する(DRED)などの有効化
		*************************************************************************/
		void EnabledGPUClashDebuggingModes(); 

		/*!**********************************************************************
		*  @brief     高性能または最小電力を示すAdapterを選択（（高）xGPU、dGPU iGPU（低）
		*  @param[in] const DXGI_GPU_PREFERENCE 選択するGPUの種類
		*  @return    gu::SharedPointer<RHIDisplayAdapter> : 物理デバイスのポインタ
		*************************************************************************/
		gu::SharedPointer<core::RHIDisplayAdapter> SearchAdapter(const DXGI_GPU_PREFERENCE preference) const;

		#pragma endregion

		#pragma region Protected Property
		/*! @brief コマンドラインで指定されている場合, WARPデバイスで作るようにする
		*   @note  コマンドラインでは"warp"コマンドを打ってください*/
		static bool SHOULD_CREATE_WITH_WARP;

		/*! @brief コマンドラインで指定されている場合, ソフトウェアレンダリングを許可する
		*   @note  コマンドラインでは, "allowSoftwareRenderingを打ってください"*/
		static bool ALLOW_SOFTWARE_RENDERING;

		/*! @brief Factoryのポインタ*/
		FactoryComPtr _factory = nullptr;

		/* @brief GPUがクラッシュした際、予期しないデバイス削除エラーが検出された後に原因を解析できるような機能を追加するか*/
		bool _useDRED        = false;

		/* @brief BreadcrumbContextを使用するか*/
		bool _useDREDContext = false;

		/* @brief 軽量版のDREDを使用するか*/
		bool _useLightWeightDRED = false;

		GPUCrashDebuggingMode _gpuCrashDebuggingMode = GPUCrashDebuggingMode::None;
		#pragma endregion

	};
}

#endif