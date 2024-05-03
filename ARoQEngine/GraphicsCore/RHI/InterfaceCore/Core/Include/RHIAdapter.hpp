//////////////////////////////////////////////////////////////////////////////////
///  @file   RHIAdapter.hpp
///  @brief  論理デバイスに渡す物理デバイス(Apdapter)の設定, GPU情報を取得
///  @author Toide Yutaro
///  @date   2024_03_29
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_ADAPTER_HPP
#define RHI_ADAPTER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "RHICommonState.hpp"
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Base/Include/GUAssert.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class RHIDevice;
	class RHIInstance;
	/****************************************************************************
	*				  			RHIDisplayAdapter
	*************************************************************************//**
	/* @class     RHIDisplayAdapter
	*  @brief     論理デバイスに渡す物理デバイス(Apdapter)の設定, GPU情報を取得
	*****************************************************************************/
	class RHIDisplayAdapter : public gu::NonCopyable
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief 自身の物理デバイスに基づいて論理デバイスを生成し, そのSharedPointerを渡します. 
		*************************************************************************/
		virtual gu::SharedPointer<core::RHIDevice> CreateDevice() = 0;
		
		/*!**********************************************************************
		*  @brief 物理デバイスの名前とスペックを出力に表示します@n
		*         基本的に実行時のログとして使用するものになります. @n
		*         ファイルや文字列に出力は行わないです.
		*************************************************************************/
		virtual void PrintInfo() const = 0;

		#pragma endregion
		
		#pragma region Public Property
		/*!**********************************************************************
		*  @brief  物理デバイスに接続されている出力の数 (モニターなど)を返します.
		*************************************************************************/
		virtual gu::uint64 GetOutputCount() const = 0;

		/*!**********************************************************************
		*  @brief  Discrete GPUかどうかを判定します
		*  @note   外部GPU(xGPU)はtrueと判定され, 統合GPU(iGPU)のみfalseと判定されます.
		*************************************************************************/
		__forceinline bool IsDiscreteGPU() const { return _isDiscreteGPU; }

		/*!**********************************************************************
		*  @brief  統合GPUかどうかを判定します
		*************************************************************************/
		__forceinline bool IsUnifiedGPU() const { return !_isDiscreteGPU; }

		/*!**********************************************************************
		*  @brief  物理デバイスの名前 (GPU名)を取得します
		*************************************************************************/
		__forceinline const gu::string& GetName() const { return _name; };

		/*!**********************************************************************
		*  @brief   ベンダーIDを直接数値として返します. 
		*           ベンダーIDは物理デバイスの作成会社(AMD, NVidia, Intel....)を特定することが出来るID 
		*  @note    ベンダーIDが0の場合は, 割り当てられていないことを示します
		*************************************************************************/
		__forceinline const gu::uint32 GetVenderID() const { return _venderID; }
		
		/*!**********************************************************************
		*  @brief   デバイスIDを直接数値として返します. 
		*************************************************************************/
		__forceinline const gu::uint32 GetDeviceID() const { return _deviceID; } 
		
		/*!**********************************************************************
		*  @brief   RHIInstanceの生ポインタを返します.
		*************************************************************************/
		RHIInstance* GetInstance() const { return _instance.Get(); };
		
		/*!**********************************************************************
		*  @brief   NVidiaがベンダーかどうか
		*************************************************************************/
		bool IsAdapterNVIDIA() const { return _venderID == (gu::uint32)DisplayAdapterVendorType::Nvidia; }

		/*!**********************************************************************
		*  @brief   Intelがベンダーかどうか
		*************************************************************************/
		bool IsAdapterIntel()  const { return _venderID == (gu::uint32)DisplayAdapterVendorType::Intel;}

		/*!**********************************************************************
		*  @brief   AMDがベンダーかどうか
		*************************************************************************/
		bool IsAdapterAMD()    const { return _venderID == (gu::uint32)DisplayAdapterVendorType::Amd; }

		/*!**********************************************************************
		*  @brief   CPUと共有されていない専用ビデオメモリのバイト数
		*************************************************************************/
		gu::uint64 GetDedicatedVideoMemory() const { return _dedicatedVideoMemory; }

		/*!**********************************************************************
		*  @brief   CPUと共有されていない専用システムメモリのバイト数
		*************************************************************************/
		gu::uint64 GetDedicatedSystemMemory() const { return _dedicatedSystemMemory; }

		/*!**********************************************************************
		*  @brief   CPUと共有されているシステムメモリのバイト数
		*************************************************************************/
		gu::uint64 GetSharedSystemMemory() const { return _sharedSystemMemory; }

		#pragma endregion


		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		/*! @brief Default constructor*/
		RHIDisplayAdapter() = default;

		/*! Destructor*/
		virtual ~RHIDisplayAdapter() { if (_instance) { _instance.Reset(); } }

		/*! @brief Instanceポインタを使って初期化*/
		RHIDisplayAdapter(const gu::SharedPointer<RHIInstance>& instance) : _instance(instance)
		{
			Check(_instance);
		}; 

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		/*! @brief : 物理デバイス名 */
		gu::string _name = "";

		/*! @brief : Instance handler*/
		gu::SharedPointer<RHIInstance> _instance = nullptr;

		/*! @brief CPUと共有されていない専用ビデオメモリのバイト数*/
		gu::uint64 _dedicatedVideoMemory = 0;

		/*! @brief CPUと共有されていない専用システムメモリのバイト数*/
		gu::uint64 _dedicatedSystemMemory = 0;

		/*! @brief CPUと共有されるシステムメモリのバイト数*/
		gu::uint64 _sharedSystemMemory = 0;

		/*! @brief 物理デバイス自体を作っている会社を特定するためのID*/
		gu::uint32 _venderID = 0;

		/*! @brief   ハードウェアデバイスのPCI_IDかACPI_ID. (ex. GeForce RTX 4090では2684を示します.)
		    @details https://devicehunt.com/view/type/pci/vendor/10DE/device/2684 */
		gu::uint32 _deviceID = 0;

		/*! @brief      同一のデバイス(GeForce RTX 4090)でも異なるモデルを区別するために使用
		    @attention  Vulkanでは使用できません*/
		gu::uint32 _subSysID = 0;

		/*! @brief : true : dGPU, xGPU, false : iGPU*/
		bool _isDiscreteGPU = false;
	};
}

#endif