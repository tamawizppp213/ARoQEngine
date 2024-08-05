//////////////////////////////////////////////////////////////////////////////////
/// @file   RHIInstance.hpp
/// @brief  GraphicsAPIを選択し, 以降でGraphicsAPIに対応するRenderResourceを呼び出すためのクラス@n
///         そのほか, 物理デバイスの選択やCPU, GPUデバッガの有無なども登録することが出来ます.@n 
/// @author Toide Yutaro
/// @date   2024_03_27
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VULKAN_RHI_INSTANCE_HPP
#define VULKAN_RHI_INSTANCE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIInstance.hpp"
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
#include "VulkanHelper.hpp"
#include "GameUtility/Base/Include/GUType.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define#include "GameUtility/Container/Include/GUDynamicArray.hpp"
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::vulkan
{
	/****************************************************************************
	*				  			RHIInstance
	****************************************************************************/
	/* @class     RHIInstance
	*   @brief     GraphicsAPIを選択し, 以降でGraphicsAPIに対応するRenderResourceを呼び出すためのクラス@n
	*              そのほか, 物理デバイスの選択やCPU, GPUデバッガの有無なども登録することが出来ます.@n
	*****************************************************************************/
	class RHIInstance : public core::RHIInstance, public gu::EnableSharedFromThis<RHIInstance>
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief   全ての利用可能な物理デバイスを配列の形で返します.
		*  @return  gu::DynamicArray<gu::SharedPointer<RHIDisplayAdapter>> : 物理デバイスの配列
		*************************************************************************/
		virtual gu::DynamicArray<gu::SharedPointer<core::RHIDisplayAdapter>> EnumrateAdapters() const override;
		
		/*!**********************************************************************
		*  @brief     最も性能が高い物理デバイスを自動で選定します. 高い順にxGPU(外部GPU), dGPU(discrete GPU), iGPU (integrated GPU)の順に優先されます
		*  @note      Vulkanではまだ外部GPUが対応できておらず, ディスクリートGPU, integrated GPU (最初に選択されたGPU)の順に選択されます@n
		*  @return gu::SharedPointer<RHIDisplayAdapter> DisplayAdapterのポインタ
		*************************************************************************/
		virtual gu::SharedPointer<core::RHIDisplayAdapter> SearchHighPerformanceAdapter() override;
		
		/*!**********************************************************************
		*  @brief   最も性能が低い(消費電力が低い)物理デバイスを自動で選定します. 高い順にiGPU (integrated GPU), dGPU(discrete GPU), xGPU(外部GPU)の順に優先されます
		*  @note    Vulkanではまだ外部GPUが対応できておらず, integrated GPU, ディスクリートGPU (最初に選択されたGPU)の順に選択されます
		*  @return gu::SharedPointer<RHIDisplayAdapter> DisplayAdapterのポインタ
		*************************************************************************/
		virtual gu::SharedPointer<core::RHIDisplayAdapter> SearchMinimumPowerAdapter() override;
		
		/* OutputDebugString : adapter list*/
		virtual void LogAdapters() const override;
		#pragma endregion Public Function

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		#pragma region Public Property

		/*!**********************************************************************
		*  @brief   VulkanのInstanceを返します/
		*************************************************************************/
		VkInstance GetVkInstance() const { return _instance; }

		gu::DynamicArray<VkLayerProperties> GetInstanceLayers() const;
 
	    /*!**********************************************************************
		*  @brief   VulkanAPIのバージョンが指定したバージョン以上かを判定します.
		*************************************************************************/
		bool MeetRequiredVersion(const gu::uint32 major, const gu::uint32 minor, const gu::uint32 patch = 0) const
		{
			return VK_MAKE_VERSION(_majorVersion, _minorVersion, _patchVersion) >= VK_MAKE_VERSION(major, minor, patch);
		}

		/*!**********************************************************************
		*  @brief   VulkanAPIのバージョンを取得します.
		*************************************************************************/
		gu::uint32 GetVkAPIVersion() const { return VK_MAKE_VERSION(_majorVersion, _minorVersion, _patchVersion); }

		/*!**********************************************************************
		*  @brief   VulkanAPIのメジャーバージョンを取得します.
		*************************************************************************/
		gu::uint32 GetVkMajorVersion() const { return _majorVersion; }

		/*!**********************************************************************
		*  @brief   VulkanAPIのメジャーバージョンを取得します.
		*************************************************************************/
		gu::uint32 GetVkMinorVersion() const { return _minorVersion; }

		#pragma endregion

		#pragma region Public Constructor and Destructor
		
		/*! @brief Default Constructor */
		RHIInstance() = default;

		/*! @brief Destructor */
		~RHIInstance();

		/*! @brief Debuggerを使うコンストラクタ*/
		RHIInstance(const core::RHIDebugCreateInfo& debugCreateInfo);

		#pragma endregion

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		bool CheckValidationLayerSupport(); // check enable cpu and gpu debugger
		
		/*!**********************************************************************
		*  @brief     DeviceTypeに基づく物理デバイスを選択
		*  @return    gu::SharedPointer<RHIDisplayAdapter> : 物理デバイスのポインタ
		*************************************************************************/
		gu::SharedPointer<core::RHIDisplayAdapter> SearchAdapter(const VkPhysicalDeviceType deviceType);

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		#pragma region Protected Property
		
		VkInstance               _instance         = nullptr;
		VkDebugUtilsMessengerEXT _debugMessenger   = nullptr;
		gu::DynamicArray<const char*> _instanceLayers   = {};

		// current version
		/*! @brief Vulkan APIのメジャーバージョンです. */
		gu::uint32 _majorVersion = 0;

		/*! @brief Vulkan APIのマイナーバージョンです*/
		gu::uint32 _minorVersion = 0;

		/*! @brief Vulkan APIのパッチバージョンです.*/
		gu::uint32 _patchVersion = 0;

		#pragma endregion Protected Property
	
	private:
		#pragma region Private Function
		/*!**********************************************************************
		*  @brief   最新のVulkan APIのバージョンを設定します.
		*************************************************************************/
		void FindNewestVulkanAPIVersion();

		/*!**********************************************************************
		*  @brief  Engineの状態にあった拡張機能を調べます
		*************************************************************************/
		bool CheckVulkanSupportInEngine() const;

		/*!**********************************************************************
		*  @brief   VkInstanceがサポートしている拡張機能リストをstring配列で返します. 
		*************************************************************************/
		gu::DynamicArray<gu::string> AcquireExtensionList() const;

		/*!**********************************************************************
		*  @brief  物理デバイスのポインタを全てリスト化して渡します. 
		*************************************************************************/
		gu::DynamicArray<VkPhysicalDevice> EnumratePhysicalDevices() const;

		// push back name array list
		VkResult FillFilteredNameArray(gu::DynamicArray<gu::string>& used, 
			const gu::DynamicArray<VkLayerProperties>& properties, 
			const gu::DynamicArray<Entry>& requestedLayers);

		VkResult FillFilteredNameArray(gu::DynamicArray<gu::string>& used,
			const gu::DynamicArray<VkExtensionProperties>& properties,
			const gu::DynamicArray<Entry>& requested,
			gu::DynamicArray<void*>& featureStructs);

		// debugging
		void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

		#pragma endregion Private Function
	};
}

#endif