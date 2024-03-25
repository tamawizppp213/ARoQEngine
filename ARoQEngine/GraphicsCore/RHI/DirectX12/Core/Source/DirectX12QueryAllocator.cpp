//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12QueryAllocator.cpp
///             @brief  temp
///             @author toide
///             @date   2024/01/05 21:00:13
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/DirectX12QueryAllocator.hpp"
#include <crtdbg.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::directX12;
using namespace rhi;

//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor 
QueryAllocator::QueryAllocator(const gu::uint32 maxQueryCount)
	: _maxQueryCount(maxQueryCount), _currentID(INVALID_ID)
{

}
#pragma endregion Constructor and Destructor

#pragma region Main Function
/****************************************************************************
*                       IssueID
*************************************************************************//**
*  @fn        gu::uint32 QueryAllocator::IssueID()
*
*  @brief     �V�����N�G���z��̃C���f�b�N�X�𔭍s���܂�
*
*  @param[in] void
*
*  @return �@�@gu::uint32 new id
*****************************************************************************/
gu::uint32 QueryAllocator::IssueID()
{
	// �A���P�[�^�����t�ɂȂ����珉��ID�Ƀ��Z�b�g����
	// �����Query�����t���[���Ă΂�邽��, �����ɒ��g�����t�ɂȂ邽�߂ł���. (�����O�o�b�t�@�̂悤�Ȏd�g�݂ɂ���)
	if (_currentID + 1 > _maxQueryCount)
	{
		ResetID();
		_availableID.Clear();
	}

	// ���g�p���X�g�ɂ��܂��Ă���΂�������g�p����
	if (!_availableID.IsEmpty())
	{
		const auto id = _availableID.Front();
		_availableID.Pop();
		return id;
	}

	_currentID++;
	return _currentID;
}

/****************************************************************************
*                       FreeID
*************************************************************************//**
*  @fn        void QueryAllocator::FreeID(const gu::uint32 id)
*
*  @brief     ID�𖢎g�p��ԂɕύX���܂�
*
*  @param[in] const gu::uint32 id
*
*  @return �@�@void
*****************************************************************************/
void QueryAllocator::FreeID(const gu::uint32 id)
{
	if (id >= _maxQueryCount)
	{
		_RPT0(_CRT_WARN, "The number of IDs has exceeded the expected number\n");
	}

	_availableID.Push(id);
}

/****************************************************************************
*                       ResetID
*************************************************************************//**
*  @fn        void QueryAllocator::RestID(const gu::uint32 offsetIndex)
*
*  @brief     �q�[�v�̒��g�͉������, ���̂܂�ID�݂̂�������Ԃɖ߂��܂�
*
*  @param[in] const gu::uint32 offsetIndex
*
*  @return �@�@void
*****************************************************************************/
void QueryAllocator::ResetID(const gu::uint32 offsetIndex)
{
	_currentID = INVALID_ID + offsetIndex;
}
#pragma endregion Main Function