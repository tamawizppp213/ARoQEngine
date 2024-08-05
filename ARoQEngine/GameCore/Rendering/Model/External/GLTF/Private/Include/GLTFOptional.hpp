//////////////////////////////////////////////////////////////////////////////////
///             @file   GLTFProperty.hpp
///             @brief  Property 
///             @author Copyright (c) Microsoft Corporation. All rights reserved.
///                     Licensed under the MIT License. 
///             @date   2022_05_15  
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GLTF_OPTIONAL_HPP
#define GLTF_OPTIONAL_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFExceptions.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gltf
{
	namespace detail
	{
		/****************************************************************************
		*				  			GLTFOptional
		****************************************************************************/
		/* @class     GLTFOptional
		*  @brief     Optiona
		*****************************************************************************/
		template<typename T>
		class Optional final
		{
		public:
			/****************************************************************************
			**                Public Function
			*****************************************************************************/
			bool HasValue() const noexcept { return _isConstructed; }
			void Reset();
			void Swap(Optional& other) { Swap(*this, other); }
			static void Swap(Optional& lhs, Optional& rhs);
			/****************************************************************************
			**                Public Property
			*****************************************************************************/
			T& Get();
			const T& Get() const;

			/****************************************************************************
			**                Constructor and Destructor
			*****************************************************************************/
			Optional()                    : _isConstructed(false) {};
			Optional(T&& t)               : _isConstructed(true ) { new (this->GetStorage()) T(std::move(t)); }
			Optional(const T& t)          : _isConstructed(true ) { new (this->GetStorage()) T(t); }
			Optional(Optional<T>&& other) noexcept : _isConstructed(false) { if (other._isConstructed) { Swap(other); } }
			Optional(const Optional& other) :_isConstructed(other._isConstructed){ if (other._isConstructed) { new (this->GetStorage()) T(other.Get()); }}
			~Optional() { Reset(); }

			Optional& operator=(Optional&& other) { if (this != &other) { Optional(std::move(other)).Swap(*this); } return *this; }
			Optional& operator=(const Optional& other)
			{
				if (this != &other) { Optional(other).Swap(*this);}
				return *this;
			}
			Optional& operator=(T&& t)
			{
				if (_isConstructed) { Get() = std::move(t); }
				else
				{
					new (this->GetStorage()) T(std::move(t));
					_isConstructed = true;
				}
				return *this;
			}
			Optional& operator=(const T& t)
			{
				if (_isConstructed) { Get() = t; }
				else
				{
					new (this->GetStorage()) T(t);
					_isConstructed = true;
				}
				return *this;
			}
			explicit operator bool() const noexcept { return HasValue(); }
		private:
			/****************************************************************************
			**                Private Function
			*****************************************************************************/
			void*       GetStorage()       { return _storage; }
			const void* GetStorage() const { return _storage; }

			/****************************************************************************
			**                Private Property
			*****************************************************************************/
			alignas(alignof(T))unsigned char _storage[sizeof(T)] = {0};
			bool _isConstructed;
		};
		
		template<typename T> T& Optional<T>::Get()
		{
			if (_isConstructed) { return *static_cast<T*>(this->GetStorage()); }
			else { throw error::GLTFException("Optional has no value"); }
		}
		template<typename T> const T& Optional<T>::Get() const
		{
			if (_isConstructed) { return *static_cast<const T*>(this->GetStorage()); }
			else { throw error::GLTFException("Optional has no value"); }
		}
		template<typename T> inline void Optional<T>::Reset()
		{
			if (_isConstructed) { Get().~T(); }
			_isConstructed = false;
		}
		template<typename T> inline void Optional<T>::Swap(Optional& lhs, Optional& rhs)
		{
			if (lhs && rhs)
			{
				std::swap(lhs.Get(), rhs.Get());
			}
			else if (lhs)
			{
				new (rhs.GetStorage()) T(std::move(lhs.Get()));
				rhs._isConstructed = true;
				lhs.Reset();
			}
			else if (rhs)
			{
				new (lhs.GetStorage()) T(std::move(rhs.Get()));
				lhs._isConstructed = true;
				rhs.Reset();
			}
		}
		template<typename T>
		bool operator==(const Optional<T>& lhs, const Optional<T>& rhs)
		{
			return (!lhs && !rhs) || ((lhs && rhs) && (lhs.Get() == rhs.Get()));
		}

		template<typename T>
		bool operator!=(const Optional<T>& lhs, const Optional<T>& rhs)
		{
			return !(lhs == rhs);
		}
	}
}

#endif