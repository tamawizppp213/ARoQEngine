//////////////////////////////////////////////////////////////////////////////////
/// @file   RHIInstance.hpp
/// @brief  GraphicsAPI��I����, �ȍ~��GraphicsAPI�ɑΉ�����RenderResource���Ăяo�����߂̃N���X@n
///         ���̂ق�, �����f�o�C�X�̑I����CPU, GPU�f�o�b�K�̗L���Ȃǂ��o�^���邱�Ƃ��o���܂�.@n 
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
	*   @brief     GraphicsAPI��I����, �ȍ~��GraphicsAPI�ɑΉ�����RenderResource���Ăяo�����߂̃N���X@n
	*              ���̂ق�, �����f�o�C�X�̑I����CPU, GPU�f�o�b�K�̗L���Ȃǂ��o�^���邱�Ƃ��o���܂�.@n
	*****************************************************************************/
	class RHIInstance : public core::RHIInstance, public gu::EnableSharedFromThis<RHIInstance>
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief   �S�Ă̗��p�\�ȕ����f�o�C�X��z��̌`�ŕԂ��܂�.
		*  @return  gu::DynamicArray<gu::SharedPointer<RHIDisplayAdapter>> : �����f�o�C�X�̔z��
		*************************************************************************/
		virtual gu::DynamicArray<gu::SharedPointer<core::RHIDisplayAdapter>> EnumrateAdapters() const override;
		
		/*!**********************************************************************
		*  @brief     �ł����\�����������f�o�C�X�������őI�肵�܂�. ��������xGPU(�O��GPU), dGPU(discrete GPU), iGPU (integrated GPU)�̏��ɗD�悳��܂�
		*  @note      Vulkan�ł͂܂��O��GPU���Ή��ł��Ă��炸, �f�B�X�N���[�gGPU, integrated GPU (�ŏ��ɑI�����ꂽGPU)�̏��ɑI������܂�@n
		*  @return gu::SharedPointer<RHIDisplayAdapter> DisplayAdapter�̃|�C���^
		*************************************************************************/
		virtual gu::SharedPointer<core::RHIDisplayAdapter> SearchHighPerformanceAdapter() override;
		
		/*!**********************************************************************
		*  @brief   �ł����\���Ⴂ(����d�͂��Ⴂ)�����f�o�C�X�������őI�肵�܂�. ��������iGPU (integrated GPU), dGPU(discrete GPU), xGPU(�O��GPU)�̏��ɗD�悳��܂�
		*  @note    Vulkan�ł͂܂��O��GPU���Ή��ł��Ă��炸, integrated GPU, �f�B�X�N���[�gGPU (�ŏ��ɑI�����ꂽGPU)�̏��ɑI������܂�
		*  @return gu::SharedPointer<RHIDisplayAdapter> DisplayAdapter�̃|�C���^
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
		*  @brief   Vulkan��Instance��Ԃ��܂�/
		*************************************************************************/
		VkInstance GetVkInstance() const { return _instance; }

		gu::DynamicArray<VkLayerProperties> GetInstanceLayers() const;
 
	    /*!**********************************************************************
		*  @brief   VulkanAPI�̃o�[�W�������w�肵���o�[�W�����ȏォ�𔻒肵�܂�.
		*************************************************************************/
		bool MeetRequiredVersion(const gu::uint32 major, const gu::uint32 minor, const gu::uint32 patch = 0) const
		{
			return VK_MAKE_VERSION(_majorVersion, _minorVersion, _patchVersion) >= VK_MAKE_VERSION(major, minor, patch);
		}

		/*!**********************************************************************
		*  @brief   VulkanAPI�̃o�[�W�������擾���܂�.
		*************************************************************************/
		gu::uint32 GetVkAPIVersion() const { return VK_MAKE_VERSION(_majorVersion, _minorVersion, _patchVersion); }

		/*!**********************************************************************
		*  @brief   VulkanAPI�̃��W���[�o�[�W�������擾���܂�.
		*************************************************************************/
		gu::uint32 GetVkMajorVersion() const { return _majorVersion; }

		/*!**********************************************************************
		*  @brief   VulkanAPI�̃��W���[�o�[�W�������擾���܂�.
		*************************************************************************/
		gu::uint32 GetVkMinorVersion() const { return _minorVersion; }

		#pragma endregion

		#pragma region Public Constructor and Destructor
		
		/*! @brief Default Constructor */
		RHIInstance() = default;

		/*! @brief Destructor */
		~RHIInstance();

		/*! @brief Debugger���g���R���X�g���N�^*/
		RHIInstance(const core::RHIDebugCreateInfo& debugCreateInfo);

		#pragma endregion

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		bool CheckValidationLayerSupport(); // check enable cpu and gpu debugger
		
		/*!**********************************************************************
		*  @brief     DeviceType�Ɋ�Â������f�o�C�X��I��
		*  @return    gu::SharedPointer<RHIDisplayAdapter> : �����f�o�C�X�̃|�C���^
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
		/*! @brief Vulkan API�̃��W���[�o�[�W�����ł�. */
		gu::uint32 _majorVersion = 0;

		/*! @brief Vulkan API�̃}�C�i�[�o�[�W�����ł�*/
		gu::uint32 _minorVersion = 0;

		/*! @brief Vulkan API�̃p�b�`�o�[�W�����ł�.*/
		gu::uint32 _patchVersion = 0;

		#pragma endregion Protected Property
	
	private:
		#pragma region Private Function
		/*!**********************************************************************
		*  @brief   �ŐV��Vulkan API�̃o�[�W������ݒ肵�܂�.
		*************************************************************************/
		void FindNewestVulkanAPIVersion();

		/*!**********************************************************************
		*  @brief  Engine�̏�Ԃɂ������g���@�\�𒲂ׂ܂�
		*************************************************************************/
		bool CheckVulkanSupportInEngine() const;

		/*!**********************************************************************
		*  @brief   VkInstance���T�|�[�g���Ă���g���@�\���X�g��string�z��ŕԂ��܂�. 
		*************************************************************************/
		gu::DynamicArray<gu::string> AcquireExtensionList() const;

		/*!**********************************************************************
		*  @brief  �����f�o�C�X�̃|�C���^��S�ă��X�g�����ēn���܂�. 
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