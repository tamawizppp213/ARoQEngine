//////////////////////////////////////////////////////////////////////////////////
///             @file   GLTFProperty.hpp
///             @brief  Property 
///             @author Copyright (c) Microsoft Corporation. All rights reserved.
///                     Licensed under the MIT License. 
///             @date   2022_05_15  
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GLTF_PROPERTY_HPP
#define GLTF_PROPERTY_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFConstants.hpp"
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFExtension.hpp"
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFExceptions.hpp"
#include <unordered_map>
#include <string>
#include <vector>
#include <memory>
#include <typeindex>

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
		*				  			GLTFProperty
		*************************************************************************//**
		*  @struct     GLTFProperty
		*  @brief      Property (has extension and extra)
		*****************************************************************************/
		struct GLTFProperty
		{
		public:
			/****************************************************************************
			**                Public Function
			*****************************************************************************/
			/*---------------------------------------------------------------------------
							Getter Extension
			-----------------------------------------------------------------------------*/
			template<typename T> const T& GetExtension() const;
			template<typename T>       T& GetExtension();
			std::vector<std::reference_wrapper<GLTFExtension>> GetExtensions() const;
			/*---------------------------------------------------------------------------
							Setter Extension
			-----------------------------------------------------------------------------*/
			template<typename TExtension, typename ...TArgs>
			void SetExtension(TArgs&& ...args) { SetExtension(std::make_unique<TExtension>(std::forward<TArgs>(args)...)); }
			void SetExtension(std::unique_ptr<GLTFExtension>&& extenison);
			/*---------------------------------------------------------------------------
							Has Extension
			-----------------------------------------------------------------------------*/
			template<typename T>
			bool HasExtension()                                    const { return _registeredExtensions.find(typeid(T)) != _registeredExtensions.end(); };
			bool HasUnregisteredExtension(const std::string& name) const { return Extensions.find(name) != Extensions.end(); }
			/*---------------------------------------------------------------------------
							Remove Extension
			-----------------------------------------------------------------------------*/
			template<typename T>
			void RemoveExtension() { return _registeredExtensions.erase(typeid(T)); }
			/****************************************************************************
			**                Public Member Variables
			*****************************************************************************/
			std::unordered_map<std::string, std::string> Extensions;
			std::string                                  Extras;
			/****************************************************************************
			**                Constructor and Destructor
			*****************************************************************************/
			virtual ~GLTFProperty();

		protected:
			/****************************************************************************
			**                Protected Function
			*****************************************************************************/
			static bool Equals(const GLTFProperty& lhs, const GLTFProperty& rhs);
			/****************************************************************************
			**                Protected Member Variables
			*****************************************************************************/

			/****************************************************************************
			**                Constructor and Destructor
			*****************************************************************************/
			GLTFProperty() = default;
			GLTFProperty(const GLTFProperty& other);
			GLTFProperty& operator=(const GLTFProperty& other);
		private:
			std::unordered_map<std::type_index, std::unique_ptr<GLTFExtension>> _registeredExtensions;
		};

		/****************************************************************************
		*				  			TemplateStruct
		*************************************************************************//**
		*  @struct     TemplateStruct
		*  @brief     temp
		*****************************************************************************/
		struct GLTFChildOfRootProperty : GLTFProperty
		{
		public:
			/****************************************************************************
			**                Public Function
			*****************************************************************************/

			/****************************************************************************
			**                Public Member Variables
			*****************************************************************************/
			std::string ID;
			std::string Name;
			/****************************************************************************
			**                Constructor and Destructor
			*****************************************************************************/
		protected:
			/****************************************************************************
			**                Protected Function
			*****************************************************************************/
			static bool Equals(const GLTFChildOfRootProperty& lhs, const GLTFChildOfRootProperty& rhs)
			{
				return lhs.ID == rhs.ID && lhs.Name == rhs.Name && GLTFProperty::Equals(lhs, rhs);
			}
			GLTFChildOfRootProperty() = default;
			GLTFChildOfRootProperty(std::string id, std::string name) : ID(std::move(id)), Name(std::move(name)){}
			/****************************************************************************
			**                Protected Member Variables
			*****************************************************************************/
		};
#pragma region Inline Function
#pragma region GLTFProperty
		/****************************************************************************
		*							GetExtension
		*************************************************************************//**
		*  @fn        template<typename T> inline const T& GLTFProperty::GetExtension() const
		*  @brief     Find Extension from unordered map.
		*  @param[in] void
		*  @return 　　const T& extension
		*****************************************************************************/
		template<typename T> inline const T& GLTFProperty::GetExtension() const
		{
			auto iterator = _registeredExtensions.find(typeid(T));
			if (iterator != _registeredExtensions.end())
			{
				return static_cast<T&>(*iterator->second.get());
			}
			throw gltf::detail::error::GLTFException(std::string("Could not find extensions: ") + typeid(T).name());
		}
		/****************************************************************************
		*							GetExtension
		*************************************************************************//**
		*  @fn        template<typename T> inline T& GLTFProperty::GetExtension()
		*  @brief     Find Extension from unordered map.
		*  @param[in] void
		*  @return 　　const T& extension
		*****************************************************************************/
		template<typename T> inline T& GLTFProperty::GetExtension()
		{
			auto iterator = _registeredExtensions.find(typeid(T));
			if (iterator != _registeredExtensions.end())
			{
				return static_cast<T&>(*iterator->second.get());
			}
			throw gltf::detail::error::GLTFException(std::string("Could not find extensions: ") + typeid(T).name());
		}
		/****************************************************************************
		*							SetExtension
		*************************************************************************//**
		*  @fn        inline void GLTFProperty::SetExtension(std::unique_ptr<GLTFExtension>&& extension)
		*  @brief     Set extension
		*  @param[in] std::unique_ptr<GLTFExtension>&& extension
		*  @return 　　const T& extension
		*****************************************************************************/
		inline void GLTFProperty::SetExtension(std::unique_ptr<GLTFExtension>&& extension)
		{
			const auto& typeExpr = *extension;
			const auto& typeInfo = typeid(typeExpr);
			_registeredExtensions.emplace(typeInfo, std::move(extension));
		}
#pragma endregion GLTFProperty
#pragma endregion Inline Function
	}
}
#endif