//////////////////////////////////////////////////////////////////////////////////
///             @file   SharedPointerReference.hpp
///             @brief  temp
///             @author toide
///             @date   2023/11/08 23:48:04
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_SHARED_REFERENCER_HPP
#define GU_SHARED_REFERENCER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUType.hpp"
#include <atomic>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
namespace gu
{
	enum class SharedPointerThreadMode : uint8
	{
		NotThreadSafe = 0,
		ThreadSafe = 1
	};
}

#define SHARED_POINTER_DEFAULT_THREAD_MODE (SharedPointerThreadMode::NotThreadSafe)
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gu
{

	namespace details
	{
		/****************************************************************************
		*				  			   SharedPointerReference
		*************************************************************************//**
		*  @class     SharedPointerReference
		*  @brief     Shared pointer�̎Q�ƃJ�E���g��ێ����邽�߂̃N���X
		*****************************************************************************/
		template<SharedPointerThreadMode Mode = SHARED_POINTER_DEFAULT_THREAD_MODE>
		class SharedReferencer
		{
		public:
			/****************************************************************************
			**                Public Function
			*****************************************************************************/
			/*----------------------------------------------------------------------
			*  @brief : Increment the reference count.
			/*----------------------------------------------------------------------*/
			void Add();

			/*----------------------------------------------------------------------
			*  @brief : Decrement the reference count.
			/*----------------------------------------------------------------------*/
			void Release();

			/****************************************************************************
			**                Public Member Variables
			*****************************************************************************/
			/*----------------------------------------------------------------------
			*  @brief : Return the reference count in the shared pointer.
			/*----------------------------------------------------------------------*/
			inline int32 GetReferenceCount() const { return _referenceCount; }

			/*----------------------------------------------------------------------
			*  @brief : Check if this class has the only one reference (true : Unique, false, some references)
			/*----------------------------------------------------------------------*/
			bool IsUnique() const;

			/*----------------------------------------------------------------------
			*  @brief : Return the Reference count is under 0;
			/*----------------------------------------------------------------------*/
			inline bool EnableDelete() const { return _referenceCount <= 0; }

			/****************************************************************************
			**                Constructor and Destructor
			*****************************************************************************/
			SharedReferencer() = default;

			virtual ~SharedReferencer() = default;

		protected:
			/****************************************************************************
			**                Protected Function
			*****************************************************************************/

			/****************************************************************************
			**                Protected Member Variables
			*****************************************************************************/
			// Threadsafe�Ȃ�Atomic<int32>, ����ȊO��int32
			std::conditional_t<Mode == SharedPointerThreadMode::ThreadSafe, 
				std::atomic<int32>, int32> _referenceCount = 1;
		};


#pragma region SharedReferencer Implement
		/*----------------------------------------------------------------------
		*  @brief : �Q�ƃJ�E���g�𑝂₷
		/*----------------------------------------------------------------------*/
		template<SharedPointerThreadMode Mode>
		void SharedReferencer<Mode>::Add()
		{
			if constexpr (Mode == SharedPointerThreadMode::ThreadSafe)
			{
				_referenceCount.fetch_add(1, std::memory_order_relaxed);
			}
			else
			{
				++_referenceCount;
			}
		}

		/*----------------------------------------------------------------------
		*  @brief : �Q�ƃJ�E���g�����炷
		/*----------------------------------------------------------------------*/
		template<SharedPointerThreadMode Mode>
		void SharedReferencer<Mode>::Release()
		{
			if (_referenceCount <= 0) { return; }

			if constexpr (Mode == SharedPointerThreadMode::ThreadSafe)
			{
				_referenceCount.fetch_sub(1, std::memory_order_acq_rel);
			}
			else
			{
				--_referenceCount;
			}
		}

		/*----------------------------------------------------------------------
		*  @brief : �ق��̎Q�Ƃ������Ă��Ȃ������m�F����
		/*----------------------------------------------------------------------*/
		template<SharedPointerThreadMode Mode>
		bool SharedReferencer<Mode>::IsUnique() const
		{
			if constexpr (Mode == SharedPointerThreadMode::ThreadSafe)
			{
				return _referenceCount.load(std::memory_order_acquire);
			}
			else
			{
				return _referenceCount == 1;
			}
		}
#pragma endregion SharedReferencer Implement
	}
}
#endif