//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIQuery.hpp
///             @brief  temp
///             @author toide
///             @date   2024/01/04 2:46:33
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_QUERY_HPP
#define RHI_QUERY_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class RHIDevice;
	class GPUBuffer;
	class RHIFence;
	class RHIQuery;

	/****************************************************************************
	*				  			   QueryLocation
	****************************************************************************/
	/* @class     QueryLocation
	*  @brief     Query�̌��ʂ��i�[���܂�
	*****************************************************************************/
	struct QueryResultLocation
	{
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		// @brief : �N�G�����i�[����q�[�v
		gu::SharedPointer<RHIQuery> Heap = nullptr;

		// @brief : �N�G���̔z��C���f�b�N�X
		gu::uint32 QueryID  = 0;

		// @brief : �N�G���̎��
		core::QueryHeapType Type = core::QueryHeapType::Occulusion;
		
		//// @brief : ���ʂ̃|�C���^
		//void* Target = nullptr;

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		QueryResultLocation() = default;

		QueryResultLocation(const gu::SharedPointer<RHIQuery>& queryHeap, const gu::uint32 id,
			const core::QueryHeapType type)
			: Heap(queryHeap), QueryID(id), Type(type)//, Target(target)
		{

		}

		~QueryResultLocation()
		{
			if (Heap) { Heap.Reset(); }
		}
	};

	/****************************************************************************
	*				  			   RHIQuery
	****************************************************************************/
	/* @class     RHIQuery
	*  @brief     Query
	*****************************************************************************/
	class RHIQuery : public gu::NonCopyable, public gu::EnableSharedFromThis<RHIQuery>
	{
	public:
		enum class ResetFlag
		{
			All,        // ���g������ (need reAllocate)
			OnlyOffset  // ���g�������Ȃ� (don't need reallocate)
		};

		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief :  Query�p�̊m�ۂ���Ă��Ȃ��̈��ResultLocation��Ԃ��܂�.
		*----------------------------------------------------------------------*/
		virtual QueryResultLocation Allocate() = 0;

		/*----------------------------------------------------------------------
		*  @brief :  Query�p�̎w�肳�ꂽIndex��������܂�.
		*----------------------------------------------------------------------*/
		virtual void Free(QueryResultLocation& location) = 0;

		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief :  �S�ẴN�G���q�[�v��64kB�̗̈���m�ۂ���悤�ɂ���.
		*----------------------------------------------------------------------*/
		static constexpr gu::uint32 MAX_HEAP_BYTE_SIZE = 65536;

		/*----------------------------------------------------------------------
		*  @brief :  ���ʂ��i�[����Readback buffer�̃|�C���^
		*----------------------------------------------------------------------*/
		gu::SharedPointer<GPUBuffer> GetResultBuffer() { return _resultBuffer; }

		/*----------------------------------------------------------------------
		*  @brief :  �N�G�����i�[����q�[�v�̈�̎�ނ�Ԃ�. 
		*----------------------------------------------------------------------*/
		QueryHeapType GetHeapType() const { return _queryHeapType; };

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		RHIQuery() = default;

		virtual ~RHIQuery() = default;

		explicit RHIQuery(const gu::SharedPointer<RHIDevice>& device, core::QueryHeapType heapType) : _device(device), _queryHeapType(heapType) {};

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		gu::SharedPointer<RHIDevice> _device = nullptr;

		gu::SharedPointer<GPUBuffer> _resultBuffer = nullptr;

		core::QueryHeapType _queryHeapType = core::QueryHeapType::Occulusion;
	};

}
#endif