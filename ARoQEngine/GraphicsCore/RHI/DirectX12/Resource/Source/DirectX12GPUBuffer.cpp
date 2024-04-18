//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12GPUBuffer.hpp
///             @brief  GPU Buffer 
///             @author Toide Yutaro
///             @date   2022_07_08
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/DirectX12GPUBuffer.hpp"
#include "../../Core/Include/DirectX12Debug.hpp"
#include "../../Core/Include/DirectX12EnumConverter.hpp"
#include "../../Core/Include/DirectX12Device.hpp"
#include "../../Core/Include/DirectX12CommandList.hpp"
#include "../../Core/Include/DirectX12BaseStruct.hpp"
#include "GameUtility/Base/Include/GUAssert.hpp"
#include "GameUtility/Memory/Include/GUMemory.hpp"
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUBarrierBatcher.hpp"
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;
using namespace gu;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
GPUBuffer::GPUBuffer(const gu::SharedPointer<core::RHIDevice>& device, const core::GPUBufferMetaData& metaData, const gu::tstring& name)
	:core::GPUBuffer(device, metaData, name)
{
	using enum core::BufferCreateFlags;

	const auto rhiDevice = static_cast<directX12::RHIDevice*>(device.Get());

	/*-------------------------------------------------------------------
	-           Set heap property
	---------------------------------------------------------------------*/
	const D3D12_HEAP_PROPERTIES heapProp
	{
		.Type                 = EnumConverter::Convert(_metaData.HeapType), 
		.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
		.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN,
		.CreationNodeMask     = rhiDevice->GetGPUMask().Value(),
		.VisibleNodeMask      = rhiDevice->GetGPUMask().Value()
	};

	/*-------------------------------------------------------------------
	-           Set resource desc
	---------------------------------------------------------------------*/
	const auto usage     = metaData.Usage;
	const auto isDynamic = gu::HasAnyFlags(usage, core::BufferCreateFlags::AnyDynamic);

	Check(isDynamic ? _metaData.HeapType != core::MemoryHeap::Default : true);

	const D3D12_RESOURCE_DESC resourceDesc = 
	{
		.Dimension          = D3D12_RESOURCE_DIMENSION_BUFFER,
		.Alignment          = metaData.Stride > 0 && 
		                      (gu::HasAnyFlags(usage, StructuredBuffer) ||
							  !gu::HasAnyFlags(usage, ByteAddress | DrawIndirect)) ? (uint64)0: (uint64)4,
		.Width              = static_cast<uint64>(GetTotalByteSize()),
		.Height             = 1, // For 1D buffer
		.DepthOrArraySize   = 1, // For 1D buffer
		.MipLevels          = 1,
		.Format             = DXGI_FORMAT_UNKNOWN,
		.SampleDesc         = {1, 0},
		.Layout             = D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
		.Flags              = EnumConverter::Convert(metaData.Usage)
	};

	/*-------------------------------------------------------------------
	-           Create
	---------------------------------------------------------------------*/
	const auto dxDevice = static_cast<rhi::directX12::RHIDevice*>(_device.Get());
	ThrowIfFailed( dxDevice->CreateCommittedResource(_resource, resourceDesc, heapProp, EnumConverter::Convert(metaData.State)));

	_resource->SetName(name.CString());
}

GPUBuffer::~GPUBuffer()
{
	if (_intermediateBuffer) { _intermediateBuffer.Reset(); }
	if (_resource)           { _resource.Reset(); }
}

#pragma region Public Function
/****************************************************************************
*                     Upload
*************************************************************************//**
*  @brief     GPU�Ƀ�������z�u���܂�. �Z�ʂ������悤��byte�P�ʂŎw�肵�܂�.
*
*  @param[in] const void* : GPU�ɃA�b�v���[�h����CPU���̃������z��
*  @param[in] const gu::uint64 �������̊m�ۂ���o�C�g�T�C�Y
*  @param[in] const gu::uint64 ���������m�ۂ��鏉���I�t�Z�b�g [byte]
*  @param[in] const gu::SharedPointer<RHICommandList> Graphics��Copy�̃R�}���h���X�g
*  @param[in] const bool �蓮�Ń}�b�v���s����
*
*  @return �@�@void
*****************************************************************************/
void GPUBuffer::UploadByte(const void* data, const gu::uint64 allocateByteSize, const gu::uint64 offsetByte, const gu::SharedPointer<core::RHICommandList>& commandList, const bool useMapManually)
{
	const auto rhiDevice = gu::StaticPointerCast<rhi::directX12::RHIDevice>(_device);
	const auto dxDevice  = rhiDevice->GetDevice();

	const auto bufferSize = _metaData.GetTotalByte();
	Check(allocateByteSize + offsetByte <= bufferSize);

	if (_metaData.IsCPUAccessible() && HasAnyFlags(_metaData.Usage, core::BufferCreateFlags::AnyDynamic))
	{
		if (useMapManually)
		{
			Memory::Copy(&_mappedData[offsetByte], data, allocateByteSize);
		}
		else
		{
			Map();
			Memory::Copy(&_mappedData[offsetByte], data, allocateByteSize);
			Unmap();
		}
	}
	else
	{
		/*-------------------------------------------------------------------
		-         CPU�������̃f�[�^���f�t�H���g�̃o�b�t�@�ɃR�s�[����B
		-         ���̎�, �R�s�[�p�̒��ԃo�b�t�@���o�R���čs��.
		---------------------------------------------------------------------*/
		// ���ԃo�b�t�@�Ŏg�p����GPU�q�[�v�̐ݒ�
		const D3D12_HEAP_PROPERTIES uploadHeapProp
		{
			.Type                 = D3D12_HEAP_TYPE_UPLOAD,
			.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY_UNKNOWN,
			.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN,
			.CreationNodeMask     = rhiDevice->GetGPUMask().Value(),
			.VisibleNodeMask      = rhiDevice->GetGPUMask().Value()
		};

		// upload buffer�Ŏg�p����descriptor
		const D3D12_RESOURCE_DESC uploadBufferDesc =
		{
			.Dimension        = D3D12_RESOURCE_DIMENSION::D3D12_RESOURCE_DIMENSION_BUFFER,
			.Alignment        = 0,
			.Width            = allocateByteSize,
			.Height           = 1, // For 1D buffer
			.DepthOrArraySize = 1, // For 1D buffer
			.MipLevels        = 1,
			.Format           = DXGI_FORMAT_UNKNOWN,
			.SampleDesc       = {1, 0},
			.Layout           = D3D12_TEXTURE_LAYOUT_ROW_MAJOR,
			.Flags            = D3D12_RESOURCE_FLAG_NONE
		};

		D3D12_SUBRESOURCE_DATA subresourceData =
		{
			.pData      = data,
			.RowPitch   = static_cast<LONG_PTR>(allocateByteSize),
			.SlicePitch = static_cast<LONG_PTR>(allocateByteSize)  // �����1�����o�b�t�@�̂���, ���Ɏg�p����Ȃ�. 
		};

		// CreateCommittedResource -> Heap�m�� + ���ۂ�GPU�Ƀ������̊m�ۂ𗼕�����Ă����.
		ThrowIfFailed(rhiDevice->CreateCommittedResource
		(
			_intermediateBuffer, uploadBufferDesc, uploadHeapProp, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr)
		);

		_intermediateBuffer->SetName(L"Intermediate Buffer");

		/*-------------------------------------------------------------------
		-      Schedule to copy the data to the default buffer resource
		---------------------------------------------------------------------*/
		const auto dxCommandList = gu::StaticPointerCast<directX12::RHICommandList>(commandList)->GetCommandList();
		const auto beforeState   = EnumConverter::Convert(_metaData.State);
		const auto before        = GPUBarrier::CreateTransition(_resource.Get(), beforeState, D3D12_RESOURCE_STATE_COPY_DEST);

		dxCommandList->ResourceBarrier(1, &before);
		UpdateSubresources<1>(dxCommandList.Get(), _resource.Get(), _intermediateBuffer.Get(), offsetByte, 0, 1, &subresourceData);

		auto after = GPUBarrier::CreateTransition(_resource.Get(), D3D12_RESOURCE_STATE_COPY_DEST, beforeState);
		dxCommandList->ResourceBarrier(1, &after);
	}
}

/****************************************************************************
*                     Map
*************************************************************************//**
/* @brief     �蓮��CPU����GPU�Ƀf�[�^���A�b�v���[�h���鏀���Ƃ��Ďg�p���܂�.
* 
*  @param[in] void
* 
*  @return �@�@void
*****************************************************************************/
void GPUBuffer::Map()
{
	Check(_metaData.IsCPUAccessible());

	ThrowIfFailed(_resource->Map(0, nullptr, reinterpret_cast<void**>(&_mappedData)));

	_useCPUMapped = true;
}


/****************************************************************************
*                     UploadIndex
*************************************************************************//**
/* @brief     �z��̗v�f���w�肷��C���f�b�N�X���g����CPU����GPU�Ƀ�������z�u���܂�.
* 
*  @param[in] const void* : GPU�ɃA�b�v���[�h������CPU���̃������z��
*  @param[in] const gu::uint64 : �z��̗v�f��
*  @param[in] const gu::uint64 : ���������m�ۂ��鏉���C���f�b�N�X
*  @param[in] const gu::SharedPointer<RHICommandList> Graphics��Copy�̃R�}���h���X�g
*  @param[in] const bool �蓮�Ń}�b�v���s����
* 
*  @return �@�@void
*****************************************************************************/
void GPUBuffer::UploadIndex(const void* data, const gu::uint64 elementCount, const gu::uint64 offsetIndex, const gu::SharedPointer<core::RHICommandList>& commandList, const bool useMapManually)
{
	Check(_useCPUMapped);
	Check(elementCount + offsetIndex <= _metaData.Count);
	UploadByte(data, elementCount * _metaData.Stride, offsetIndex * _metaData.Stride, commandList, useMapManually);
}

/****************************************************************************
*                     Unmap
*************************************************************************//**
/* @brief     CPU����GPU�Ƀf�[�^���A�b�v���[�h����̂��~�߂�ꍇ�Ɏg�p���܂�.
* 
*  @param[in] void
* 
*  @return �@�@const gu::tstring �f�o�b�O�\����
*****************************************************************************/
void GPUBuffer::Unmap()
{
	_resource->Unmap(0, nullptr);
	_useCPUMapped = false;
}

/****************************************************************************
*                    SetName
*************************************************************************//**
/* @brief     �f�o�b�O�\������ݒ肵�܂�.
*
*  @param[in] void
*
*  @return �@�@void
*****************************************************************************/
void GPUBuffer::SetName(const gu::tstring& name)
{
	ThrowIfFailed(_resource->SetName(name.CString()));
}

#pragma endregion Public Function