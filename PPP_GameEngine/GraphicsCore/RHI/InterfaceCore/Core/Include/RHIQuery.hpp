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
#include "GameUtility/Base/Include/ClassUtility.hpp"
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
	*************************************************************************//**
	*  @class     QueryLocation
	*  @brief     Queryの結果を格納します
	*****************************************************************************/
	struct QueryResultLocation
	{
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		// @brief : クエリを格納するヒープ
		gu::SharedPointer<RHIQuery> Heap = nullptr;

		// @brief : クエリの配列インデックス
		gu::uint32 QueryID  = 0;

		// @brief : クエリの種類
		core::QueryHeapType Type = core::QueryHeapType::Occulusion;
		
		//// @brief : 結果のポインタ
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
	*************************************************************************//**
	*  @class     RHIQuery
	*  @brief     Query
	*****************************************************************************/
	class RHIQuery : public NonCopyable, public gu::EnableSharedFromThis<RHIQuery>
	{
	public:
		enum class ResetFlag
		{
			All,        // 中身を消す (need reAllocate)
			OnlyOffset  // 中身を消さない (don't need reallocate)
		};

		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief :  Query用の確保されていない領域のResultLocationを返します.
		/*----------------------------------------------------------------------*/
		virtual QueryResultLocation Allocate() = 0;

		/*----------------------------------------------------------------------
		*  @brief :  Query用の指定されたIndexを解放します.
		/*----------------------------------------------------------------------*/
		virtual void Free(QueryResultLocation& location) = 0;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief :  全てのクエリヒープは64kBの領域を確保するようにする.
		/*----------------------------------------------------------------------*/
		static constexpr gu::uint32 MAX_HEAP_BYTE_SIZE = 65536;

		/*----------------------------------------------------------------------
		*  @brief :  結果を格納するReadback bufferのポインタ
		/*----------------------------------------------------------------------*/
		gu::SharedPointer<GPUBuffer> GetResultBuffer() { return _resultBuffer; }

		/*----------------------------------------------------------------------
		*  @brief :  クエリを格納するヒープ領域の種類を返す. 
		/*----------------------------------------------------------------------*/
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
		**                Protected Member Variables
		*****************************************************************************/
		gu::SharedPointer<RHIDevice> _device = nullptr;

		gu::SharedPointer<GPUBuffer> _resultBuffer = nullptr;

		core::QueryHeapType _queryHeapType = core::QueryHeapType::Occulusion;
	};

}
#endif