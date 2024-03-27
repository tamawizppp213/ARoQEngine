//////////////////////////////////////////////////////////////////////////////////
/// @file   RHIInstance.hpp
/// @brief  GraphicsAPIを選択し, 以降でGraphicsAPIに対応するRenderResourceを呼び出すためのクラス@n
///         そのほか, 物理デバイスの選択やCPU, GPUデバッガの有無なども登録することが出来ます.@n 
/// @author Toide Yutaro
/// @date   2024_03_27
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_INSTANCE_HPP
#define RHI_INSTANCE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "RHICommonState.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class RHIDisplayAdapter;
	/****************************************************************************
	*				  			RHIInstance
	*************************************************************************//**
	/*  @class     RHIInstance
	*   @brief     GraphicsAPIを選択し, 以降でGraphicsAPIに対応するRenderResourceを呼び出すためのクラス@n
	*              そのほか, 物理デバイスの選択やCPU, GPUデバッガの有無なども登録することが出来ます.@n 
	*****************************************************************************/
	class RHIInstance : public gu::NonCopyable
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief     手動でGraphics APIを選択します. @n
		*             そのほかCPU, GPUデバッガを使用するかも選択できますが, リリースモードでは使用することが出来ません
		*  @param[in] const core::GraphicsAPI グラフィクスAPI
		*  @param[in] const RHIInstanceCreateInfo デバッガの指定用構造体
		*  @return gu::SharedPointer<RHIInstance> RHIInstanceのポインタ
		*************************************************************************/
		static gu::SharedPointer<RHIInstance> CreateInstance(const core::GraphicsAPI graphicsAPI, const RHIDebugCreateInfo& debugCreateInfo);
		
		/*!**********************************************************************
		*  @brief     プラットフォームに合わせて自動でGraphics APIを選択します. @n
		*             そのほかCPU, GPUデバッガを使用するかも選択できますが, リリースモードでは使用することが出来ません
		*  @param[in] const RHIDebugCreateInfo& debugCreateInfo
		*  @return gu::SharedPointer<RHIInstance> RHIInstanceのポインタ
		*************************************************************************/
		static gu::SharedPointer<RHIInstance> CreateInstance(const RHIDebugCreateInfo& debugCreateInfo);

		/*!**********************************************************************
		*  @brief     最も性能が高い物理デバイスを自動で選定します. 高い順にxGPU(外部GPU), dGPU(discrete GPU), iGPU (integrated GPU)の順に優先されます
		*  @note      DirectX12では外部GPU, ディスクリートGPU, 統合GPUの順に選択されます.　@n 
		*             Vulkanではまだ外部GPUが対応できておらず, ディスクリートGPU, integrated GPU (最初に選択されたGPU)の順に選択されます@n
		*  @return gu::SharedPointer<RHIDisplayAdapter> DisplayAdapterのポインタ
		*************************************************************************/
		virtual gu::SharedPointer<RHIDisplayAdapter> SearchHighPerformanceAdapter() = 0;
		
		/*!**********************************************************************
		*  @brief     最も性能が低い(消費電力が低い)物理デバイスを自動で選定します. 高い順にiGPU (integrated GPU), dGPU(discrete GPU), xGPU(外部GPU)の順に優先されます
		*  @note      DirectX12では統合GPU, ディスクリートGPU, 外部GPUの順に選択されます.　@n
		*             Vulkanではまだ外部GPUが対応できておらず, integrated GPU, ディスクリートGPU (最初に選択されたGPU)の順に選択されます
		*  @return gu::SharedPointer<RHIDisplayAdapter> DisplayAdapterのポインタ
		*************************************************************************/
		virtual gu::SharedPointer<RHIDisplayAdapter> SearchMinimumPowerAdapter() = 0;
		
		/*!**********************************************************************
		*  @brief     全ての利用可能な物理デバイスを配列の形で返します.
		*  @return    gu::DynamicArray<gu::SharedPointer<RHIDisplayAdapter>> : 物理デバイスの配列
		*************************************************************************/
		virtual gu::DynamicArray<gu::SharedPointer<RHIDisplayAdapter>> EnumrateAdapters() = 0;
		
		/*!**********************************************************************
		*  @brief     出力欄に全ての物理デバイスを記入します
		*************************************************************************/
		virtual void LogAdapters() = 0;

		#pragma endregion Public Function

		#pragma region Public Member Function
		/*!**********************************************************************
		*  @brief    エンジンの名前を取得します
		*************************************************************************/
		const char* GetEngineName() const { return EngineName; }

		/*!**********************************************************************
		*  @brief    GPUのブレークポイントを設定できるかを返します.
		*************************************************************************/
		bool UseGPUDebugBreak() const { return _debugInfo.EnableGPUDebugBreak; }

		#pragma endregion Public Member Function

		#pragma region Public Constructor and Destructor

		#pragma endregion

	protected:
		#pragma region Protected Constructor and Destructor
		
		/*! @brief : デフォルトコンストラクタ*/
		RHIInstance() = default;

		/*! @brief : Debuggerを使うコンストラクタ*/
		RHIInstance(const core::RHIDebugCreateInfo& debugCreateInfo) : _debugInfo(debugCreateInfo) {};

		virtual ~RHIInstance() = default;

		#pragma endregion

		#pragma region Protected Member Variables

		/* @ Debuggerを指定するコンストラクタ*/
		core::RHIDebugCreateInfo _debugInfo;

		/*! @brief : エンジン名*/
		const char* EngineName = "ARoQ Engine";

		#pragma endregion 
	};
	
}

#endif