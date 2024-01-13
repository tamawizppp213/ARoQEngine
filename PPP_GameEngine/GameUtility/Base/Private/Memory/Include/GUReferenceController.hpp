//////////////////////////////////////////////////////////////////////////////////
///             @file   GUReferenceController.hpp
///             @brief  temp
///             @author toide
///             @date   2023/11/14 0:26:26
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_REFERENCE_CONTROLLER_HPP
#define GU_REFERENCE_CONTROLLER_HPP
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GUReferenceControllerBase.hpp"
#include "GUSharedDeleter.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gu
{
	namespace details
	{
		/****************************************************************************
		*				  			 ReferenceController
		*************************************************************************//**
		*  @class     ReferenceController
		*  @brief     参照カウントを調整し, Resourceも破棄できるクラスです
		*****************************************************************************/
		template<class ElementType, class Deleter = DefaultDeleter<ElementType>, SharedPointerThreadMode Mode = SHARED_POINTER_DEFAULT_THREAD_MODE>
		class ReferenceController : public ReferenceControllerBase<Mode>
		{
		public:
			/****************************************************************************
			**                Public Function
			*****************************************************************************/

			/****************************************************************************
			**                Public Member Variables
			*****************************************************************************/

			/****************************************************************************
			**                Constructor and Destructor
			*****************************************************************************/
			ReferenceController() = default;

			explicit ReferenceController(ElementType* element)
				: _element(element), _deleter(Deleter()){ };

			ReferenceController(ElementType* element, Deleter&& deleter) 
				: _element(element), _deleter(deleter){ };

			~ReferenceController() = default;

		protected:
			/****************************************************************************
			**                Protected Function
			*****************************************************************************/
			__forceinline void Dispose() override
			{
				_deleter(_element);
			}

			__forceinline void DeleteThis() override
			{
				delete this;
			}

			/****************************************************************************
			**                Protected Member Variables
			*****************************************************************************/
			ElementType* _element = nullptr;
			Deleter _deleter;
		};
	}
}
#endif