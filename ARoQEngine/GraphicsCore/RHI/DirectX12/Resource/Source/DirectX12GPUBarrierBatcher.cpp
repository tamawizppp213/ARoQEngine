//////////////////////////////////////////////////////////////////////////////////
/// @file   DirectX12GPUBarrierBatcher.cpp
/// @brief  temp
/// @author toide
///�@@date   2024/04/14 0:22:20
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUBarrierBatcher.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12CommandList.hpp"
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUBuffer.hpp"
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUTexture.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12EnumConverter.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::directX12;
using namespace rhi;
//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor 

#pragma endregion Constructor and Destructor

#pragma region Main Function
/****************************************************************************
*                     PushTransitionBarrier
****************************************************************************/
/* @brief     �P�Ƃ�GPU���\�[�X�̏�ԑJ�ڂ������܂�. @n
*             ���\�[�X�̎g�������ς��^�C�~���O�ŌĂяo���܂�. 
*
*  @param[in] gu::SharedPointer<GPUResource>& GPU���\�[�X (���ꂾ��GPUResource��������̂͌�Ń��\�[�X�̑J�ڌ�̏�Ԃ�������K�v�����邽��)
*  @param[in] const D3D12_RESOURCE_STATES �J�ڌ�̃��\�[�X���
*  @param[in] const gu::uint32 �T�u���\�[�X�������C���f�b�N�X (�f�t�H���g�͑S�ẴT�u���\�[�X�������܂�)
*
*  @return �@�@bool Barrier���o�^���ꂽ��true
*****************************************************************************/
bool GPUBarrierBatcher::PushTransitionBarrier(const gu::SharedPointer<core::GPUResource>& resource, const core::ResourceState after, const gu::uint32 subresource)
{
	const auto dxResource    = resource->IsBuffer() ? gu::StaticPointerCast<directX12::GPUBuffer> (resource)->GetResourcePtr():
			                                          gu::StaticPointerCast<directX12::GPUTexture>(resource)->GetResourcePtr();
	const auto dxBeforeState = EnumConverter::Convert(resource->GetResourceState());
	const auto dxAfterState  = EnumConverter::Convert(after);

	/*-------------------------------------------------------------------
	-         �Ӗ��̂Ȃ��J�ڂ����s����Ă��邩���m�F����.
	---------------------------------------------------------------------*/
	Check(dxBeforeState != dxAfterState);
	if (_barriers.Size() > 0)
	{
		// �Ӗ��̂Ȃ��J�� (�P���ɑJ�ڂ��s���Ė߂��ĂƂ����悤�ȏ�)���s���Ă��邩���m�F���܂�
		// �������ʂ����ꍇ�͐V�K�ɒǉ�������, �Ō�̗v�f���폜���܂�. 
		// �������ׂ̊ϓ_�ōŌゾ���s���Ă��܂�. 
		
		const auto last = _barriers.Back();
		if(last.Type      == D3D12_RESOURCE_BARRIER_TYPE_TRANSITION && 
			dxResource    == last.Transition.pResource   && 
			subresource   == last.Transition.Subresource && 
			dxBeforeState == last.Transition.StateAfter  && 
			dxAfterState  == last.Transition.StateBefore)
		{
			_barriers.Pop();
			_afterTransitions.Pop();
			return false;
		}
	}

	/*-------------------------------------------------------------------
	-         �V�K�̃o���A���쐬����. 
	---------------------------------------------------------------------*/
	_barriers.Push(gu::type::Forward<GPUBarrier>(GPUBarrier::CreateTransition(dxResource,  dxBeforeState, dxAfterState, subresource)));
	_afterTransitions.Push({ after, resource });
	return true;
}

/****************************************************************************
*                     PushAliasingBarrier
****************************************************************************/
/* @brief     Unordered access view��p�̏�ԃo���A @n
*             UAV�̓ǂݏ������ɂق���UAV���Ώۃ��\�[�X��ǂݏ�������`��R�}���h�̎��s��h�����Ƃ�ړI�Ƃ��܂� @n
*
*  @param[in]  ID3D12Resource* Unordered access view������GPU���\�[�X
*
*  @return �@�@void
*****************************************************************************/
void GPUBarrierBatcher::PushAliasingBarrier(ID3D12Resource* beforeResource, ID3D12Resource* afterResource)
{
	_barriers.Push(gu::type::Forward<GPUBarrier>(GPUBarrier::CreateAliasing(beforeResource, afterResource)));
}

/****************************************************************************
*                     PushUAVBarrier
****************************************************************************/
/* @brief     �����������̈�Ƀ}�b�s���O���ꂽ������GPU���\�[�X�ɑ΂�, �g�p���郊�\�[�X���̂�؂�ւ���ۂɎg�p���܂�. @n
*          �����Ɏg�p���Ȃ����Ƃ��S�ۂ���Ă��郊�\�[�X�̃�������ߖ񂷂邱�Ƃ��\�ƂȂ�܂�. @n
*          ������, �{�֐����g�p����ꍇ��,CreateCommittedResource�ł͖���, CreatePlacedResource���g�p�������@�Ńq�[�v�̊m�ۂ��s���Ă�������.@n
*          (�ʁX�̃q�[�v������Ă��܂����ƂɂȂ�, �����������̈������Ȃ��Ȃ邽��.)
*
*  @param[in] ID3D12Resource* �؂�ւ���O�Ɏg�p���Ă���GPU���\�[�X
*  @param[in] ID3D12Resource* �؂�ւ�����Ɏg�p����\���GPU���\�[�X
*
*  @return �@�@void
*****************************************************************************/
void GPUBarrierBatcher::PushUAVBarrier(ID3D12Resource* resource)
{
	_barriers.Push(gu::type::Forward<GPUBarrier>(GPUBarrier::CreateUAV(resource)));
}

/****************************************************************************
*                     Flush
****************************************************************************/
/* @brief     ���ɑ��݂��Ă���Heap���ɂ܂��}�b�v�܂ł͍s��Ȃ��\��ς݂̃��\�[�X���쐬
*
*  @param[in] ICommandList* �`�施�߂����s����R�}���h���X�g
*
*  @return �@�@void
*****************************************************************************/
void GPUBarrierBatcher::Flush(ICommandList* commandList)
{
	if (_barriers.Size() == 0) { return; }

	commandList->ResourceBarrier(static_cast<gu::uint32>(_barriers.Size()), _barriers.Data());

	/*-------------------------------------------------------------------
	-      Barrier�����s���Ȃ��ƑJ�ڂ����Ƃ݂Ȃ��Ȃ�. 
	      ������, GPU��CPU��ResourceState����������Ă���킯�ł͂Ȃ�����, CPU�ł͕ύX����������GPU�ł͂܂��Ƃ����󋵂���������\��������܂�. 
	---------------------------------------------------------------------*/
	for (gu::uint64 i = 0; i < _afterTransitions.Size(); ++i)
	{
		_afterTransitions[i].Value->SetResourceState(_afterTransitions[i].Key);
	}

	/*-------------------------------------------------------------------
	-      ���߂��񂾃o�b�t�@�̃T�C�Y��0�ɂ���. (�������͍������̂��߂ɏ��������Ȃ�)
	---------------------------------------------------------------------*/
	_barriers.Clear();
	_afterTransitions.Clear();
}
#pragma endregion Main Function