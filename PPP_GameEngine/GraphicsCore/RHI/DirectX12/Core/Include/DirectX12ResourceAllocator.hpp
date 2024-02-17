//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12ResourceAllocator.hpp
///             @brief  DirectX12Resource Allocator
///             @author Toide Yutaro
///             @date   2022_03_14 (OK)
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_RESOURCE_ALLOCATOR_HPP
#define DIRECTX12_RESOURCE_ALLOCATOR_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "DirectX12BaseStruct.hpp"
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include <queue>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace rhi::directX12
{
	/****************************************************************************
	*				  			ResourceAllocator
	*************************************************************************//**
	*  @class     ResourceAllocator
	*  @brief     CPU and GPU index management
	*****************************************************************************/
	class ResourceAllocator : public NonCopyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief :  �V����Resource�z��̃C���f�b�N�X�𔭍s���܂�
		/*----------------------------------------------------------------------*/
		inline UINT IssueID()
		{
			if ((UINT)_currentID + 1 > _maxDescriptorCount) { MessageBox(NULL, L"The number of IDs has exceeded the expected number. ", L"Warning", MB_ICONWARNING); }

			if (!_availableID.empty()) 
			{
				const auto id = _availableID.front();
				_availableID.pop();
				return id; 
			}

			_currentID++; return _currentID;
		}

		/*----------------------------------------------------------------------
		*  @brief :  ID�𖢎g�p��ԂɕύX���܂�
		/*----------------------------------------------------------------------*/
		inline void FreeID(const UINT id)
		{
			if (id >= _maxDescriptorCount) { OutputDebugStringA("Non available id"); return; }

			_availableID.push(id);
		}

		/*----------------------------------------------------------------------
		*  @brief :  �q�[�v�̒��g�͉������, ���̂܂�ID�݂̂�������Ԃɖ߂��܂�
		/*----------------------------------------------------------------------*/
		inline void ResetID(UINT offsetIndex = 0)
		{
			_currentID = INVALID_ID + offsetIndex;
		}

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief :  ���݂�Heap�̈�̔z��ɑ΂��Ăǂ̃C���f�b�N�X�ɓ����Ă��邩��Ԃ��܂�.
		/*----------------------------------------------------------------------*/
		inline UINT GetCurrentID() const { return _currentID; }

		/*----------------------------------------------------------------------
		*  @brief :  �f�B�X�N���v�^�q�[�v�̃o�C�g�T�C�Y��Ԃ��܂�.maxDescriptorViewCount * OneDescriptorSize
		/*----------------------------------------------------------------------*/
		inline UINT GetHeapSize() const { return _maxDescriptorCount * _descriptorSize; }
		
		/*----------------------------------------------------------------------
		*  @brief :  �ő��Descriptor Count��Ԃ��܂�.
		/*----------------------------------------------------------------------*/
		inline UINT GetMaxDescriptorCount() const { return _maxDescriptorCount; }

		/* @brief : Return DirectX12::CPU_DESCRIPTOR_HANDLE*/
		inline CPU_DESC_HANDLER GetCPUDescHandler(UINT offsetIndex = 0) const
		{
			if (offsetIndex > _maxDescriptorCount)
			{
				MessageBox(NULL, L"The number of IDs has exceeded the expected number. ", L"Warning", MB_ICONWARNING);
			}
			return CPU_DESC_HANDLER(_cpuHeapPtr, offsetIndex, _descriptorSize);
		}

		/* @brief : Return DirectX12::GPU_DESCRIPTOR_HANDLE*/
		inline GPU_DESC_HANDLER GetGPUDescHandler(UINT offsetIndex = 0) const
		{
			if (offsetIndex > _maxDescriptorCount)
			{
				MessageBox(NULL, L"The number of IDs has exceeded the expected number. ", L"Warning", MB_ICONWARNING);
			}
			return GPU_DESC_HANDLER(_gpuHeapPtr, offsetIndex, _descriptorSize);
		}

		inline void SetResourceAllocator(UINT maxDescriptorCount, UINT descriptorSize, D3D12_CPU_DESCRIPTOR_HANDLE cpuHeapPtr, D3D12_GPU_DESCRIPTOR_HANDLE gpuHeapPtr)
		{
			_maxDescriptorCount = maxDescriptorCount;
			_descriptorSize     = descriptorSize;
			_cpuHeapPtr         = cpuHeapPtr;
			_gpuHeapPtr         = gpuHeapPtr;
			_currentID          = INVALID_ID;
		}

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		ResourceAllocator() = default;

		ResourceAllocator(UINT maxDescriptorCount, UINT descriptorSize, D3D12_CPU_DESCRIPTOR_HANDLE cpuHeapPtr, D3D12_GPU_DESCRIPTOR_HANDLE gpuHeapPtr)
		{
			SetResourceAllocator(maxDescriptorCount, descriptorSize, cpuHeapPtr, gpuHeapPtr);
		}

	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		D3D12_CPU_DESCRIPTOR_HANDLE _cpuHeapPtr = D3D12_CPU_DESCRIPTOR_HANDLE();
		
		D3D12_GPU_DESCRIPTOR_HANDLE _gpuHeapPtr = D3D12_GPU_DESCRIPTOR_HANDLE(); // rtv and dsv are not used. 
		
		UINT                        _descriptorSize = 0;
		
		UINT                        _maxDescriptorCount = 0;
		
		int                         _currentID = INVALID_ID;

		std::queue<UINT> _availableID = {};

		static constexpr int INVALID_ID = -1;
	};
}

#endif