//////////////////////////////////////////////////////////////////////////////////
///  @file   RHIAdapter.hpp
///  @brief  �_���f�o�C�X�ɓn�������f�o�C�X(Apdapter)�̐ݒ�, GPU�����擾
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
	*  @brief     �_���f�o�C�X�ɓn�������f�o�C�X(Apdapter)�̐ݒ�, GPU�����擾
	*****************************************************************************/
	class RHIDisplayAdapter : public gu::NonCopyable
	{
	public:
		#pragma region Public Function
		/*!**********************************************************************
		*  @brief ���g�̕����f�o�C�X�Ɋ�Â��Ę_���f�o�C�X�𐶐���, ����SharedPointer��n���܂�. 
		*************************************************************************/
		virtual gu::SharedPointer<core::RHIDevice> CreateDevice() = 0;
		
		/*!**********************************************************************
		*  @brief �����f�o�C�X�̖��O�ƃX�y�b�N���o�͂ɕ\�����܂�@n
		*         ��{�I�Ɏ��s���̃��O�Ƃ��Ďg�p������̂ɂȂ�܂�. @n
		*         �t�@�C���╶����ɏo�͍͂s��Ȃ��ł�.
		*************************************************************************/
		virtual void PrintInfo() const = 0;

		#pragma endregion
		
		#pragma region Public Property
		/*!**********************************************************************
		*  @brief  �����f�o�C�X�ɐڑ�����Ă���o�͂̐� (���j�^�[�Ȃ�)��Ԃ��܂�.
		*************************************************************************/
		virtual gu::uint64 GetOutputCount() const = 0;

		/*!**********************************************************************
		*  @brief  Discrete GPU���ǂ����𔻒肵�܂�
		*  @note   �O��GPU(xGPU)��true�Ɣ��肳��, ����GPU(iGPU)�̂�false�Ɣ��肳��܂�.
		*************************************************************************/
		__forceinline bool IsDiscreteGPU() const { return _isDiscreteGPU; }

		/*!**********************************************************************
		*  @brief  ����GPU���ǂ����𔻒肵�܂�
		*************************************************************************/
		__forceinline bool IsUnifiedGPU() const { return !_isDiscreteGPU; }

		/*!**********************************************************************
		*  @brief  �����f�o�C�X�̖��O (GPU��)���擾���܂�
		*************************************************************************/
		__forceinline const gu::string& GetName() const { return _name; };

		/*!**********************************************************************
		*  @brief   �x���_�[ID�𒼐ڐ��l�Ƃ��ĕԂ��܂�. 
		*           �x���_�[ID�͕����f�o�C�X�̍쐬���(AMD, NVidia, Intel....)����肷�邱�Ƃ��o����ID 
		*  @note    �x���_�[ID��0�̏ꍇ��, ���蓖�Ă��Ă��Ȃ����Ƃ������܂�
		*************************************************************************/
		__forceinline const gu::uint32 GetVenderID() const { return _venderID; }
		
		/*!**********************************************************************
		*  @brief   �f�o�C�XID�𒼐ڐ��l�Ƃ��ĕԂ��܂�. 
		*************************************************************************/
		__forceinline const gu::uint32 GetDeviceID() const { return _deviceID; } 
		
		/*!**********************************************************************
		*  @brief   RHIInstance�̐��|�C���^��Ԃ��܂�.
		*************************************************************************/
		RHIInstance* GetInstance() const { return _instance.Get(); };
		
		/*!**********************************************************************
		*  @brief   NVidia���x���_�[���ǂ���
		*************************************************************************/
		bool IsAdapterNVIDIA() const { return _venderID == (gu::uint32)DisplayAdapterVendorType::Nvidia; }

		/*!**********************************************************************
		*  @brief   Intel���x���_�[���ǂ���
		*************************************************************************/
		bool IsAdapterIntel()  const { return _venderID == (gu::uint32)DisplayAdapterVendorType::Intel;}

		/*!**********************************************************************
		*  @brief   AMD���x���_�[���ǂ���
		*************************************************************************/
		bool IsAdapterAMD()    const { return _venderID == (gu::uint32)DisplayAdapterVendorType::Amd; }

		/*!**********************************************************************
		*  @brief   CPU�Ƌ��L����Ă��Ȃ���p�r�f�I�������̃o�C�g��
		*************************************************************************/
		gu::uint64 GetDedicatedVideoMemory() const { return _dedicatedVideoMemory; }

		/*!**********************************************************************
		*  @brief   CPU�Ƌ��L����Ă��Ȃ���p�V�X�e���������̃o�C�g��
		*************************************************************************/
		gu::uint64 GetDedicatedSystemMemory() const { return _dedicatedSystemMemory; }

		/*!**********************************************************************
		*  @brief   CPU�Ƌ��L����Ă���V�X�e���������̃o�C�g��
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

		/*! @brief Instance�|�C���^���g���ď�����*/
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
		/*! @brief : �����f�o�C�X�� */
		gu::string _name = "";

		/*! @brief : Instance handler*/
		gu::SharedPointer<RHIInstance> _instance = nullptr;

		/*! @brief CPU�Ƌ��L����Ă��Ȃ���p�r�f�I�������̃o�C�g��*/
		gu::uint64 _dedicatedVideoMemory = 0;

		/*! @brief CPU�Ƌ��L����Ă��Ȃ���p�V�X�e���������̃o�C�g��*/
		gu::uint64 _dedicatedSystemMemory = 0;

		/*! @brief CPU�Ƌ��L�����V�X�e���������̃o�C�g��*/
		gu::uint64 _sharedSystemMemory = 0;

		/*! @brief �����f�o�C�X���̂�����Ă����Ђ���肷�邽�߂�ID*/
		gu::uint32 _venderID = 0;

		/*! @brief   �n�[�h�E�F�A�f�o�C�X��PCI_ID��ACPI_ID. (ex. GeForce RTX 4090�ł�2684�������܂�.)
		    @details https://devicehunt.com/view/type/pci/vendor/10DE/device/2684 */
		gu::uint32 _deviceID = 0;

		/*! @brief      ����̃f�o�C�X(GeForce RTX 4090)�ł��قȂ郂�f������ʂ��邽�߂Ɏg�p
		    @attention  Vulkan�ł͎g�p�ł��܂���*/
		gu::uint32 _subSysID = 0;

		/*! @brief : true : dGPU, xGPU, false : iGPU*/
		bool _isDiscreteGPU = false;
	};
}

#endif