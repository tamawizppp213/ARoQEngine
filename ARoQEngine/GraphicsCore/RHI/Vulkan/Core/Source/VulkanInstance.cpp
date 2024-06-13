//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIInstance.hpp
///             @brief  Select device api (���̃G���W�����g�p���ŏ��ɌĂяo��.) 
///             @author Toide Yutaro
///             @date   2022_09_05
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanInstance.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanAdapter.hpp"
#include "GameUtility/File/Include/UnicodeUtility.hpp"
#include "GameUtility/Base/Include/GUAssert.hpp"
#include <sstream>
#include <Windows.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#pragma warning(disable: 26812 4100)
using namespace rhi;
using namespace rhi::vulkan;
using namespace gu;
#pragma comment(lib, "vulkan-1.lib")
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
namespace
{
#pragma region Create and Destroy Debug Messenger
	/****************************************************************************
	*				  			CreateDebugUtilsMessengerEXT
	****************************************************************************/
	/* @class     DestroyDebugUtilsMessengerEXT
	*  @brief     Define a callback to capture the messages
	*****************************************************************************/
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
	{
		auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
		}
		else
		{
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	/****************************************************************************
	*				  			DestroyDebugUtilsMessengerEXT
	****************************************************************************/
	/* @class     DestroyDebugUtilsMessengerEXT
	*  @brief     Call free debug messenger
	*****************************************************************************/
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
	{
		auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
		if (func != nullptr)
		{
			func(instance, debugMessenger, pAllocator);
		}
	}
#pragma endregion Create and Destroy Debug Messenger
#pragma region Debug Report Function
	/****************************************************************************
	*                     SkipErrorMessage
	****************************************************************************/
	/* @fn        gu::DynamicArray<VkExtensionProperties> RHIInstance::AcquireExtensionProperties()
	*  @brief     Return isSkip (true : skip, false: not skip)
	*  @param[in] void
	*  @return �@�@bool
	*****************************************************************************/
	bool SkipWarningMessage(VkDebugUtilsMessageSeverityFlagBitsEXT flags, VkObjectType objectType, const std::string& message)
	{
		// ignore verbose and info
		if (objectType != VK_OBJECT_TYPE_INSTANCE)                   { return false; }
		if (flags < VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) { return false; }

		// skip error message list
		static gu::DynamicArray<std::string> mutedWarnings = {
			"UNASSIGNED-CoreValidation-Shader-InconsistentSpirv",
			"VUID-vkCmdDrawIndexed-None-04007",
			"VUID-vkDestroyDevice-device-00378",
			"VUID-VkSubmitInfo-pWaitSemaphores-03243",
			"VUID-VkSubmitInfo-pSignalSemaphores-03244",
			"VUID-vkCmdPipelineBarrier-pDependencies-02285",
			"VUID-VkImageMemoryBarrier-oldLayout-01213",
			"VUID-vkCmdDrawIndexed-None-02721",
			"VUID-vkCmdDrawIndexed-None-02699",
			"VUID-vkCmdTraceRaysKHR-None-02699",
			"VUID-VkShaderModuleCreateInfo-pCode-04147",
		};

		// skip check
		for (auto& str : mutedWarnings)
		{
			if (message.find(str) != std::string::npos) { return true; }
		}
		
		return false; // not found skip 
	}

	/****************************************************************************
	*                     DebugReportCallback
	****************************************************************************/
	/* @fn        gu::DynamicArray<VkExtensionProperties> RHIInstance::AcquireExtensionProperties()
	*  @brief     Report debug error message. (�����͎~�߂Ȃ�. �񍐂̂�)
	*****************************************************************************/
	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,    // ���b�Z�[�W�̏d�v�x
		VkDebugUtilsMessageTypeFlagsEXT messageType, 
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, 
		void* pUserData)
	{
		/*-------------------------------------------------------------------
		-          Skip error message check
		---------------------------------------------------------------------*/
		if (SkipWarningMessage(messageSeverity, pCallbackData->pObjects->objectType, pCallbackData->pMessage)) { return VK_FALSE; }
		/*-------------------------------------------------------------------
		-			Report debug message
		---------------------------------------------------------------------*/
		std::stringstream buffer;
		buffer << "[validation layer] " << pCallbackData->pMessageIdName << " " <<  
			pCallbackData->pMessage << std::endl;
#ifdef _WIN32
		OutputDebugStringA(buffer.str().c_str());
#else
		printf("%s\n", buf.str().c_str());
#endif    
		return VK_FALSE;
	}
#pragma endregion Debug Report Function
}

#pragma region Constructor and Destructor
RHIInstance::RHIInstance(const core::RHIDebugCreateInfo& debugInfo) 
	:core::RHIInstance(debugInfo)
{
	/*-------------------------------------------------------------------
	-          Check validation support (in case enable CPU or GPU debugger)
	---------------------------------------------------------------------*/
	#ifdef _DEBUG
	if (_debugInfo.EnableCPUDebugger || _debugInfo.EnableGPUDebugger)
	{
		if (!CheckValidationLayerSupport())
		{
			throw std::runtime_error("validation layers requested, but not available");
		}
	}
	#endif
	
	/*-------------------------------------------------------------------
	-   ���݂�Vulkan API�̃o�[�W�������擾��, �G���W���̎g�p�\�ȃo�[�W�����ɂȂ��Ă��邩���m�F
	---------------------------------------------------------------------*/
	FindNewestVulkanAPIVersion();

	if (!CheckVulkanSupportInEngine()) { Confirmf(false, "Unsupported API version\n"); return; }

	/*-------------------------------------------------------------------
	-          Acquire Extension Infomation (for debugging layer)
	---------------------------------------------------------------------*/
	// const char*�ɂ���, �P���Ȓl�n�����o���Ȃ���������.
	const auto foundExtensions = AcquireExtensionList();

	// VKInstance create information�Ŏg�p���邽�߂Ƀf�[�^���^
	gu::DynamicArray<const char*> convertFoundExtensions(foundExtensions.Size());
	for (int i = 0; i < foundExtensions.Size(); ++i)
	{
		convertFoundExtensions[i] = foundExtensions[i].CString();
	}

	/*-------------------------------------------------------------------
	-               Set Application Infomation
	---------------------------------------------------------------------*/
	const VkApplicationInfo applicationInfo =
	{
		.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO, // structure type
		.pNext              = nullptr,
		.pApplicationName   = "Graphics Device Vulkan",           // application name
		.applicationVersion = VK_MAKE_VERSION(1,0,0),             // application version (���Ɏg�p���Ȃ�)
		.pEngineName        = EngineName,                         // engine name
		.engineVersion      = VK_MAKE_VERSION(1,0,0),             // engine version      (���Ɏg�p���Ȃ�)
		.apiVersion         = VK_MAKE_VERSION(_majorVersion, _minorVersion, _patchVersion), // newest version
	};

	/*-------------------------------------------------------------------
	-               Set VKInstance Create Infomation
	---------------------------------------------------------------------*/
	VkInstanceCreateInfo createInfo = 
	{
		.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,               // structure type
		.pNext                   = nullptr,
		.flags                   = 0,
		.pApplicationInfo        = &applicationInfo,                                     // application infomation
		.enabledLayerCount       = 0,
		.ppEnabledLayerNames     = nullptr,
		.enabledExtensionCount   = static_cast<UINT32>(convertFoundExtensions.Size()),   // extension count
		.ppEnabledExtensionNames = convertFoundExtensions.Data(),                        // extension name list
	}; 
	
#ifdef _DEBUG
	/*-------------------------------------------------------------------
	-       Layer : mesurement for the validation and performance
	---------------------------------------------------------------------*/
	if (debugInfo.EnableCPUDebugger || debugInfo.EnableGPUDebugger)
	{
		createInfo.enabledLayerCount   = static_cast<std::uint32_t>(_instanceLayers.Size());                 // enable layer count
		createInfo.ppEnabledLayerNames = _instanceLayers.Data();
	}
#endif

	/*-------------------------------------------------------------------
	-               Create VKInstance
	---------------------------------------------------------------------*/
	VkResult vkResult = vkCreateInstance(&createInfo, nullptr, &_instance);
	if (vkResult != VkResult::VK_SUCCESS)
	{
		Confirmf(false, "failed to create vulkan instance.\n");
	}

	/*-------------------------------------------------------------------
	-               Set up debugging layer
	---------------------------------------------------------------------*/
#ifdef _DEBUG
	if (!(debugInfo.EnableCPUDebugger || debugInfo.EnableGPUDebugger)) { return; }

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};
	PopulateDebugMessengerCreateInfo(debugCreateInfo);
	if (::CreateDebugUtilsMessengerEXT(_instance, &debugCreateInfo, nullptr, &_debugMessenger) != VK_SUCCESS)
	{
		Confirmf(false, "failed to set up debug messenger!");
	}
#endif
}

RHIInstance::~RHIInstance()
{
#ifdef _DEBUG
	printf("Destroy RHI Instance : (Vulkan)\n");
#endif
	if (_debugMessenger){DestroyDebugUtilsMessengerEXT(_instance, _debugMessenger, nullptr);}
	
	if (_instance) 
	{
		vkDestroyInstance(_instance, nullptr); 
	}

	_instanceLayers.Clear();
	_instanceLayers.ShrinkToFit();
}

#pragma endregion Constructor and Destructor
#pragma region Adapter Function
/****************************************************************************
*                     EnumrateAdapters
****************************************************************************/
/* @brief     Return all availablle adapter lists
*  @param[in] void
*  @return �@�@gu::DynamicArray<gu::SharedPointer<core::RHIAdapter>> 
*****************************************************************************/
gu::DynamicArray<gu::SharedPointer<core::RHIDisplayAdapter>> RHIInstance::EnumrateAdapters() const
{
	gu::DynamicArray<gu::SharedPointer<core::RHIDisplayAdapter>> adapterLists = {};

	/*-------------------------------------------------------------------
	-               Acquire physical devices
	---------------------------------------------------------------------*/
	const auto& devices = EnumratePhysicalDevices();

	/*-------------------------------------------------------------------
	-               Acquire Vulkan Adapter List
	---------------------------------------------------------------------*/
	for (const auto& device : devices)
	{
		adapterLists.Push(gu::MakeShared<vulkan::RHIDisplayAdapter>(SharedFromThis(), device));
	}
	return adapterLists;
}

/****************************************************************************
*                     SearchHighPerformanceAdapter
****************************************************************************/
/* @brief     (Supported GPU: NVidia, AMD, Intel) VideoMemory�̑������̂���
*              (High) xGPU, dGPU iGPU (Low) selected
*
*  @param[in] void
*
*  @return �@�@gu::SharedPointer<core::RHIDisplayAdapter>
*****************************************************************************/
gu::SharedPointer<core::RHIDisplayAdapter> RHIInstance::SearchHighPerformanceAdapter()
{
	const VkPhysicalDeviceType types[] =
	{
		VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU,
		VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU,
		VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU,
		VK_PHYSICAL_DEVICE_TYPE_CPU
	};

	for (auto type : types)
	{
		const auto adapter = SearchAdapter(type);
		if(adapter)
		{
			return adapter;
		}
	}

	return nullptr;
}

/****************************************************************************
*                     SearchMinimumPowerAdapter
****************************************************************************/
/* @brief     (Supported GPU: NVidia, AMD, Intel) VideoMemory�̏��Ȃ����̂���
*             (Low) iGPU, dGPU xGPU (High)
*
*  @param[in] void
*
*  @return �@�@gu::SharedPointer<core::RHIDisplayAdapter>
*****************************************************************************/
gu::SharedPointer<core::RHIDisplayAdapter> RHIInstance::SearchMinimumPowerAdapter()
{
	const VkPhysicalDeviceType types[] =
	{
		VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU,
		VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU,
		VK_PHYSICAL_DEVICE_TYPE_CPU
	};

	for (auto type : types)
	{
		const auto adapter = SearchAdapter(type);
		if (adapter)
		{
			return adapter;
		}
	}

	return nullptr;
}

/****************************************************************************
*                     SearchAdapter
****************************************************************************/
/* @brief     �����\�܂��͍ŏ��d�͂�����Adapter��I���i�i���jxGPU�AdGPU iGPU�i��j
*
*  @param[in] const DXGI_GPU_PREFERENCE preference (high performance or minimum power)
*
*  @return �@�@gu::SharedPointer<core::RHIDisplayAdapter>
*****************************************************************************/
gu::SharedPointer<core::RHIDisplayAdapter> RHIInstance::SearchAdapter(const VkPhysicalDeviceType deviceType)
{
	/*-------------------------------------------------------------------
	-         �����f�o�C�X����ʂ�擾����.
	---------------------------------------------------------------------*/
	const auto& physicalDevices = EnumratePhysicalDevices();
	if (physicalDevices.Size() == 0)
	{
		_RPT0(_CRT_WARN, "SearchAdapter could not find a compatible Vulkan device or driver\n");
		_RPT0(_CRT_WARN, "Make sure your video card supports Vulkan and try updating your video driver to a more recent version (proceed with any pending reboots)\n");
		return nullptr; 
	}

	// �K�v�ƂȂ���̂��Ȃ���΍ŏ��Ɍ�����Adapter��n��. 
	for (int i = 0; i < physicalDevices.Size(); ++i)
	{
		/*-------------------------------------------------------------------
		-               Get properties
		---------------------------------------------------------------------*/
		VkPhysicalDeviceProperties properties = {};
		vkGetPhysicalDeviceProperties(physicalDevices[i], &properties);
		/*-------------------------------------------------------------------
		-               Target device type check
		---------------------------------------------------------------------*/
		if (properties.deviceType == deviceType)
		{
			return gu::MakeShared<vulkan::RHIDisplayAdapter>(SharedFromThis(), physicalDevices[i]);
		}
	}

	return nullptr;
}

/****************************************************************************
*                     LogAdapters
****************************************************************************/
/* @brief     Show all available adapter information using Output debug string
*  @param[in] void
*  @return �@�@void
*****************************************************************************/
void RHIInstance::LogAdapters() const
{
	auto adapterList = EnumrateAdapters();
	for (auto& adapter : adapterList)
	{
		adapter->PrintInfo();
	}
}

#pragma endregion Adapter Function
#pragma region Set Up Function

#pragma endregion  Set Up  Function
#pragma region Debug Function
/****************************************************************************
*                     CheckValidationLayerSupport
****************************************************************************/
/* @fn        bool RHIDevice::CheckValidationLayerSupport()
*  @brief     Validation layer support check
*  @param[in] void
*  @return �@�@bool
*****************************************************************************/
bool RHIInstance::CheckValidationLayerSupport()
{
	/*-------------------------------------------------------------------
	-                  Get Available Layers
	---------------------------------------------------------------------*/
	const auto availableLayers = GetInstanceLayers();

	/*-------------------------------------------------------------------
	-                  Search Available Layers
	---------------------------------------------------------------------*/
	// 1.1.106.0 �̃o�[�W��������P��̃��C�������ŋ����C���[�̑S�Ă̌��؋@�\���g�p�\
	if constexpr (VK_HEADER_VERSION_COMPLETE >= VK_MAKE_VERSION(1, 1, 106)) 
	{
		_instanceLayers.Push("VK_LAYER_KHRONOS_validation"); // normally use
	}
	else
	{
		_instanceLayers.Push("VK_LAYER_LUNARG_standard_validation");
	}

	_instanceLayers.Push("VK_LAYER_KHRONOS_shader_object");
	//_instanceLayers.Push( "VK_LAYER_LUNARG_api_dump");

	/* This implementation is used because it is not used in release build.*/
	for (const char* layerName : _instanceLayers)
	{
		bool layerFound = false;
		for (const auto& availableLayer : availableLayers)
		{
			if (strcmp(layerName, availableLayer.layerName) == 0)
			{
				layerFound = true;
				break;
			}

		}
		if (!layerFound) { return false; }
	}
	return true;
}

gu::DynamicArray<VkLayerProperties> RHIInstance::GetInstanceLayers() const
{
	std::uint32_t layerCount = 0;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	gu::DynamicArray<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.Data());

	return availableLayers;
}

/****************************************************************************
*                     FillFilteredNameArray
****************************************************************************/
/* @fn        VkResult RHIInstance::FillFilteredNameArray(gu::DynamicArray<std::string>& used,
			  const gu::DynamicArray<VkLayerProperties>& properties,
		      const gu::DynamicArray<Entry>& requestedLayers)

*  @brief     used: layer property name vector list. 
* 
*  @param[out] gu::DynamicArray<std::string>& used : layer property name vector.
*  @param[in]  gu::DynamicArray<VkLayerProperties>properties : VkLayerProperty
*  @param[in]  gu::DynamicArray<Entry>& requestedLayer
* 
*  @return �@�@VkResult
*****************************************************************************/
VkResult RHIInstance::FillFilteredNameArray(gu::DynamicArray<gu::string>& used,
	const gu::DynamicArray<VkLayerProperties>& properties,
	const gu::DynamicArray<Entry>& requestedLayers)
{
	/* This implementation is used because it is not used in release build.*/
	// requested layer�̒�����, properties�̖��O��������.
	for (const auto& layer : requestedLayers)
	{
		bool layerFound = false;
		for (const auto& property : properties)
		{
			if (strcmp(layer.Name.CString(), property.layerName) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if (layerFound)
		{
			used.Push(layer.Name);
		}
		else if (layer.Optional == false)
		{
			return VK_ERROR_LAYER_NOT_PRESENT;
		}
	}

	return VK_SUCCESS;
}

/****************************************************************************
*                     FillFilteredNameArray
****************************************************************************/
/* @fn        VkResult RHIInstance::FillFilteredNameArray(gu::DynamicArray<std::string>& used,
			  const gu::DynamicArray<VkLayerProperties>& properties,
			  const gu::DynamicArray<Entry>& requestedLayers)

*  @brief     used: layer property name vector list.
*
*  @param[out] gu::DynamicArray<std::string>& used : layer property name vector.
*  @param[in]  gu::DynamicArray<VkLayerProperties>properties : VkLayerProperty
*  @param[in]  gu::DynamicArray<Entry>& requestedLayer
*  @param[out] gu::DynamicArray<void*>& featureStructs
*
*  @return �@�@VkResult
*****************************************************************************/
VkResult RHIInstance::FillFilteredNameArray(gu::DynamicArray<gu::string>& used,
	const gu::DynamicArray<VkExtensionProperties>& properties,
	const gu::DynamicArray<Entry>& requested,
	gu::DynamicArray<void*>& featureStructs)
{
	for (const auto& itr : requested)
	{
		bool found = false;
		for (const auto& property : properties)
		{
			// �Ώۂ̖��O�ƃX�y�b�N����v���邩�𒲂ׂ�.
			if (strcmp(itr.Name.CString(), property.extensionName) == 0 &&
				itr.Version == 0 || itr.Version == property.specVersion)
			{
				found = true;
				break;
			}
		}

		// feature struct push back
		if (found)
		{
			used.Push(itr.Name);
			if (itr.FeatureStruct)
			{
				featureStructs.Push(itr.FeatureStruct);
			}
		}
		else if (!itr.Optional)
		{
			return VK_ERROR_EXTENSION_NOT_PRESENT;
		}
	}

	return VK_SUCCESS;
}

/****************************************************************************
*                     PopulateDebugMessengerCreateInfo
****************************************************************************/
/* @fn        void RHIInstance::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
*  @brief     Set up callback debug report 
*  @param[out] VkDebugReportCallbackCreateInfoEXT& createInfo
*  @return �@�@void
*****************************************************************************/
void RHIInstance::PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
	createInfo = {};
	createInfo.sType           = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

	// ���b�Z�[�W�̏d�v��: �f�f���b�Z�[�W, �A�v���P�[�V�����̃o�O�ł���\������������ɑ΂��郁�b�Z�[�W, �����ŃN���b�V���ɂȂ��铮��ɂ��Ẵ��b�Z�[�W���o�͂���
	createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
	
	// �d�l�ɂ��p�t�H�[�}���X�ɂ��֌W�̂Ȃ��C�x���g������, �d�l�Ɉᔽ�����������s�����ꍇ, VUlkan�̍œK�łȂ��g���������Ă���\���̂�����̂��
	createInfo.messageType     = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
	
	// �R�[���o�b�N���s���֐�.
	createInfo.pfnUserCallback = DebugCallback;
}
#pragma endregion   Debug   Function

#pragma region Private Function
/****************************************************************************
*                     FindNewestVulkanAPIVersion
****************************************************************************/
/* @brief     �ŐV��Vulkan API�̃o�[�W������ݒ肵�܂�.
*
*  @param[in] void
*
*  @return    void
*****************************************************************************/
void RHIInstance::FindNewestVulkanAPIVersion()
{
	/*-------------------------------------------------------------------
	-        ���݂�Vulkan API�̃o�[�W�������擾����
	---------------------------------------------------------------------*/
	// �C���X�^���X�쐬�O��, �C���X�^���X���x���̃o�[�W������T���Ă���
	uint32 instanceVersion = VK_API_VERSION_1_0;

	auto EnumrateInstanceVersion  = PFN_vkEnumerateInstanceVersion(vkGetInstanceProcAddr(VK_NULL_HANDLE, "vkEnumerateInstanceVersion"));
	if (EnumrateInstanceVersion)
	{
		const auto instanceVersionResult = EnumrateInstanceVersion(&instanceVersion);
		
		if (instanceVersionResult != VK_SUCCESS)
		{
			Confirmf(false, "Failed to EnumrateInstanceVersion\n");
		}
	}

	// �ŐV��Vulkan API�̃o�[�W�������擾����.
	_majorVersion = VK_VERSION_MAJOR(instanceVersion);
	_minorVersion = VK_VERSION_MINOR(instanceVersion);
	_patchVersion = VK_VERSION_PATCH(instanceVersion);
}

/****************************************************************************
*                     Engine�̏�Ԃɂ������g���@�\�𒲂ׂ܂�
****************************************************************************/
/* @brief     Engine�̏�Ԃɂ������g���@�\�𒲂ׂ܂�
*
*  @param[in] void
*
*  @return    bool �T�|�[�g����Ă��邩
*****************************************************************************/
bool RHIInstance::CheckVulkanSupportInEngine() const
{
	/*-------------------------------------------------------------------
	-          Acquire Extension Infomation (for debugging layer)
	---------------------------------------------------------------------*/
	const auto foundExtensions = AcquireExtensionList();

	/*-------------------------------------------------------------------
	-   Vulkan1.0�̃o�[�W�����ł���Ȃ�, VK_KHR_get_physical_device_properties2���K�v
	---------------------------------------------------------------------*/
	if (!MeetRequiredVersion(1, 1, 0))
	{
		for (uint64 i = 0; i < foundExtensions.Size(); ++i)
		{
			if (foundExtensions[i] == VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME)
			{
				return true;
			}
		}

		return false;
	}

	return true;
}
/****************************************************************************
*                     AcquireExtensionProperties
****************************************************************************/
/* @fn        gu::DynamicArray<gu::string> AcquireExtensionList() const;
* 
*  @brief     VkInstance���T�|�[�g���Ă���g���@�\���X�g��string�z��ŕԂ��܂�
* 
*  @param[in] void
* 
*  @return �@�@gu::DynamicArray<gu::string>
*****************************************************************************/
gu::DynamicArray<gu::string> RHIInstance::AcquireExtensionList() const
{
	gu::DynamicArray<VkExtensionProperties> properties;

	/*-------------------------------------------------------------------
	-              Acquire the extension properties
	---------------------------------------------------------------------*/
	uint32 count = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &count, nullptr);           // acqure property count
	properties.Resize(count);                                                   // set buffer region 
	vkEnumerateInstanceExtensionProperties(nullptr, &count, properties.Data()); // acquire property Data

	/*-------------------------------------------------------------------
	-               Discrete GPU check
	---------------------------------------------------------------------*/
	gu::DynamicArray<gu::string> foundExtensions(count);

	for (uint64 i = 0; i < count; ++i)
	{
		gu::string name = gu::string(properties[i].extensionName);
		foundExtensions[i] = (name);
	}

	return foundExtensions;
}

/****************************************************************************
*                     EnumratePhysicalDevices
****************************************************************************/
/* @fn        gu::DynamicArray<VkPhysicalDevice> RHIInstance::EnumratePhysicalDevices() const
* 
*  @brief     �����f�o�C�X�̃|�C���^��S�ă��X�g�����ēn���܂�.
* 
*  @param[in] void
* 
*  @return �@�@gu::DynamicArray<VkPhysicalDevice>
*****************************************************************************/
gu::DynamicArray<VkPhysicalDevice> RHIInstance::EnumratePhysicalDevices() const
{
	uint32 deviceCount = 0;
	vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr);

	/*error check*/
	if (deviceCount == 0) { Confirmf(false, "failed to find GPUs with Vulkan support."); }

	gu::DynamicArray<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(_instance, &deviceCount, devices.Data());

	return devices;
}
#pragma endregion Private Function