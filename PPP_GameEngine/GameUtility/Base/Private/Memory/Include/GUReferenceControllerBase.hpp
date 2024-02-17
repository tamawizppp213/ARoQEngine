//////////////////////////////////////////////////////////////////////////////////
///             @file   GUReferenceControllerBase.hpp
///             @brief  参照カウントを加算, 減算を行い, 0になったら破棄するクラスです.
///             @author toide
///             @date   2023/11/08 23:48:04
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_REFERENCE_CONTROLLER_BASE_HPP
#define GU_REFERENCE_CONTROLLER_BASE_HPP

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

#define SHARED_POINTER_DEFAULT_THREAD_MODE (SharedPointerThreadMode::ThreadSafe)
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gu
{

	namespace details
	{
		/****************************************************************************
		*				  			  ReferenceControllerBase
		*************************************************************************//**
		*  @class     ReferenceControllerBase
		*  @brief     参照カウントを保持するための基底クラス
		*****************************************************************************/
		template<SharedPointerThreadMode Mode = SHARED_POINTER_DEFAULT_THREAD_MODE>
		class ReferenceControllerBase
		{
		private:
			using ReferenceCountType = std::conditional_t<Mode == SharedPointerThreadMode::ThreadSafe, std::atomic<int32>, int32>;

		public:
			/****************************************************************************
			**                Public Function
			*****************************************************************************/
			/*----------------------------------------------------------------------
			*  @brief : Increment the shared reference count.
			/*----------------------------------------------------------------------*/
			__forceinline void AddSharedReference();

			/*----------------------------------------------------------------------
			*  @brief : Increment the weak + shared reference count.
			/*----------------------------------------------------------------------*/
			__forceinline void AddObserverReference();

			/*----------------------------------------------------------------------
			*  @brief : Decrement the shared reference count.
			/*----------------------------------------------------------------------*/
			__forceinline void ReleaseSharedReference();

			/*----------------------------------------------------------------------
			*  @brief : Decrement the weak + shared reference count.
			/*----------------------------------------------------------------------*/
			__forceinline void ReleaseObserverReference();

			/****************************************************************************
			**                Public Member Variables
			*****************************************************************************/
			/*----------------------------------------------------------------------
			*  @brief : Return the reference count in the shared pointer.
			/*----------------------------------------------------------------------*/
			__forceinline int32 GetSharedReferenceCount() const 
			{
				if constexpr (Mode == SharedPointerThreadMode::ThreadSafe)
				{
					return _sharedReferenceCount.load(std::memory_order_relaxed);
				}
				else
				{
					return _sharedReferenceCount;
				}
			}

			/*----------------------------------------------------------------------
			*  @brief : Return the total of observer reference count in the shared and weak pointer.
			/*----------------------------------------------------------------------*/
			__forceinline int32 GetObserverReferenceCount() const 
			{
				if constexpr (Mode == SharedPointerThreadMode::ThreadSafe)
				{
					return _observerReferenceCount.load(std::memory_order_relaxed);
				}
				else
				{
					return _observerReferenceCount;
				}
			}

			/*----------------------------------------------------------------------
			*  @brief : Check if this class has the only one reference (true : Unique, false, some references)
			/*----------------------------------------------------------------------*/
			__forceinline bool IsUnique() const;

			/*----------------------------------------------------------------------
			*  @brief : Return the Reference count is under 0;
			/*----------------------------------------------------------------------*/
			__forceinline bool EnableDelete() const { return _sharedReferenceCount <= 0; }

			/****************************************************************************
			**                Constructor and Destructor
			*****************************************************************************/
			ReferenceControllerBase() = default;

			virtual ~ReferenceControllerBase() {};

			ReferenceControllerBase           (const ReferenceControllerBase&) = delete;
			ReferenceControllerBase& operator=(const ReferenceControllerBase&) = delete;

		protected:
			/****************************************************************************
			**                Protected Function
			*****************************************************************************/
			/*----------------------------------------------------------------------
			*  @brief : Destroy ElementType*(template) resource
			/*----------------------------------------------------------------------*/
			virtual void Dispose() = 0;

			/*----------------------------------------------------------------------
			*  @brief : Destroy pointer
			/*----------------------------------------------------------------------*/
			virtual void DeleteThis() = 0;

			/****************************************************************************
			**                Protected Member Variables
			*****************************************************************************/
			// ThreadsafeならAtomic<int32>, それ以外はint32
			ReferenceCountType _sharedReferenceCount   = 1;    // shared pointer count
			ReferenceCountType _observerReferenceCount = 1; // weak + shared pointer count
		};

		/*----------------------------------------------------------------------
		*  @brief : 参照カウントを増やす
		/*----------------------------------------------------------------------*/
		template<SharedPointerThreadMode Mode>
		__forceinline void ReferenceControllerBase<Mode>::AddSharedReference()
		{
			if constexpr (Mode == SharedPointerThreadMode::ThreadSafe)
			{
				_sharedReferenceCount.fetch_add(1, std::memory_order_relaxed);
			}
			else
			{
				++_sharedReferenceCount;
			}
		}

		/*----------------------------------------------------------------------
		*  @brief : 参照カウントを増やす
		/*----------------------------------------------------------------------*/
		template<SharedPointerThreadMode Mode>
		__forceinline void ReferenceControllerBase<Mode>::AddObserverReference()
		{
			if constexpr (Mode == SharedPointerThreadMode::ThreadSafe)
			{
				_observerReferenceCount.fetch_add(1, std::memory_order_relaxed);
			}
			else
			{
				++_observerReferenceCount;
			}
		}

		/*----------------------------------------------------------------------
		*  @brief : 参照カウントを減らす
		/*----------------------------------------------------------------------*/
		template<SharedPointerThreadMode Mode>
		void ReferenceControllerBase<Mode>::ReleaseSharedReference()
		{
			if (_sharedReferenceCount <= 0) { return; }

			if constexpr (Mode == SharedPointerThreadMode::ThreadSafe)
			{
				_sharedReferenceCount.fetch_sub(1, std::memory_order_acq_rel);
			}
			else
			{
				--_sharedReferenceCount;
			}

			if (_sharedReferenceCount == 0)
			{
				Dispose();
			}
		}

		/*----------------------------------------------------------------------
		*  @brief : 参照カウントを減らす 
		/*----------------------------------------------------------------------*/
		template<SharedPointerThreadMode Mode>
		void ReferenceControllerBase<Mode>::ReleaseObserverReference()
		{
			if (_observerReferenceCount <= 0) { return; }

			if constexpr (Mode == SharedPointerThreadMode::ThreadSafe)
			{
				_observerReferenceCount.fetch_sub(1, std::memory_order_acq_rel);
			}
			else
			{
				--_observerReferenceCount;
			}
		}

		/*----------------------------------------------------------------------
		*  @brief : ほかの参照を持っていないかを確認する
		/*----------------------------------------------------------------------*/
		template<SharedPointerThreadMode Mode>
		bool ReferenceControllerBase<Mode>::IsUnique() const
		{
			if constexpr (Mode == SharedPointerThreadMode::ThreadSafe)
			{
				return _sharedReferenceCount.load(std::memory_order_acquire);
			}
			else
			{
				return _sharedReferenceCount == 1;
			}
		}
#pragma endregion SharedReferencer Implement
	}
}
#endif