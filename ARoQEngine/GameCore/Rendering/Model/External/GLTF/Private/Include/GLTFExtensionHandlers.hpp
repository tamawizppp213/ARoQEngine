//////////////////////////////////////////////////////////////////////////////////
///             @file   GLTFExtensionHandler.hpp
///             @brief  Extension Handler Class:
///             @author // Copyright (c) Microsoft Corporation. All rights reserved.
///                        Licensed under the MIT License.
///                     Partially edit by Toide
///             @date   2022_05_22 
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GLTF_EXTENSION_HANDLERS_HPP
#define GLTF_EXTENSION_HANDLERS_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GLTFAsset.hpp"
#include "GLTFExtension.hpp"
#include <functional>
#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <utility>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Class
//////////////////////////////////////////////////////////////////////////////////
namespace gltf
{
	namespace detail
	{
		// type index type_infoを連想コンテナや非順序連想コンテナのインデックス型として使用するためのクラス
		using TypeKey = std::pair<std::type_index, std::type_index>;
        using NameKey = std::pair<std::string, std::type_index>;
        /****************************************************************************
        *                          MakeTypeKey
        *************************************************************************//**
        *  @fn        template<typename T1, typename T2> TypeKey MakeTypeKey()
        *  @brief     Return TypeKey
        *  @param[in] void
        *  @return 　　TypeKey
        *****************************************************************************/
        template<typename T1, typename T2>
        TypeKey MakeTypeKey() { return { typeid(T1), typeid(T2) }; }
        /****************************************************************************
        *                          MakeTypeKey
        *************************************************************************//**
        *  @fn        template<typename T> TypeKey MakeTypeKey()
        *  @brief     Return TypeKey
        *  @param[in] GLTFExtension
        *  @return 　　TypeKey
        *****************************************************************************/
        template<typename T>
        TypeKey MakeTypeKey(const GLTFExtension& extension) { return { typeid(extension), typeid(T) }; }
        /****************************************************************************
         *                          MakeTypeKey
         *************************************************************************//**
         *  @fn        TypeKey MakeTypeKey()
         *  @brief     Return TypeKey
         *  @param[in] GLTFExtension
         *  @param[in] GLTFProperty
         *  @return 　　TypeKey
         *****************************************************************************/
        TypeKey MakeTypeKey(const GLTFExtension& extension, const GLTFProperty& property);
        /****************************************************************************
        *                          MakeNameKey
        *************************************************************************//**
        *  @fn        template<typename T> NameKey MakeNameKey(const std::string& name, const GLTFProperty& property)
        *  @brief     Return NameKey
        *  @param[in] const std::string& name
        *  @return 　　NameKey
        *****************************************************************************/
        template<typename T>
        NameKey MakeNameKey(const std::string& name) { return{ name, typeid(T) }; }
        /****************************************************************************
        *                          MakeNameKey
        *************************************************************************//**
        *  @fn        NameKey MakeNameKey(const std::string& name, const GLTFProperty& property)
        *  @brief     Return NameKey
        *  @param[in] const std::string& name
        *  @param[in] const GLTFProperty& property
        *  @return 　　NameKey
        *****************************************************************************/
        NameKey MakeNameKey(const std::string& name, const GLTFProperty& property);

	}
    /****************************************************************************
    *				  			TemplateClass
    *************************************************************************//**
    *  @class     TemplateClass
    *  @brief     temp
    *****************************************************************************/
    template<typename TReturn, typename ...TArgs>
    class ExtensionHandlers
    {
    public:
        using Func = std::function<TReturn(std::add_lvalue_reference_t<const TArgs>...)>;
        /****************************************************************************
        **                Public Function
        *****************************************************************************/
        template<typename TExtenstion, typename Fn> 
        void AddHandler(const std::string& name, Fn fn) { AddHandler<TExtenstion, GLTFPropertyAll>(name, fn); }

        template <typename TExtension, typename TProperty, typename Fn>
        void AddHandler(const std::string& name, Fn fn)
        {
            static_assert(std::is_base_of<detail::GLTFExtension, TExtension>::value, "ExtensionHandlers::AddHandler: TExtension template parameter must derive from Extension");
            static_assert(std::is_base_of<detail::GLTFProperty, TProperty>::value, "ExtensionHandlers::AddHandler: TProperty template parameter must derive from glTFProperty");

            auto resultName = _nameToType.emplace(detail::MakeNameKey<TProperty>(name), typeid(TExtension));
            if (!resultName.second)
            {
                throw detail::error::GLTFException("A handler for the " + name + " extension already exists");
            }

            auto resultType = _typeToName.emplace(detail::MakeTypeKey<TExtension, TProperty>(), name);
            if (!resultType.second)
            {
                throw detail::error::GLTFException("A handler for the " + name + " extension already exists");
            }

            // Wrap the passed callable type Fn so that the handler is passed a reference to TExt rather than Extension
            auto fnConvert = [fn](std::add_lvalue_reference_t<const TArgs> ...args)
            {
                return fn(Convert<TExtension>(args)...);
            };

            auto resultHandler = _handlers.emplace(detail::MakeTypeKey<TExtension, TProperty>(), fnConvert);
            if (!resultHandler.second)
            {
                throw detail::error::GLTFException("A handler for the " + name + " extension already exists");
            }
        };

        template<typename TExtension> 
        bool HasHandler() const { return HasHandler<TExtension, GLTFPropertyAll>(); }
        template<typename TExtension, typename TProperty>
        bool HasHandler() const { return _typeToName.find(detail::MakeTypeKey<TExtension, TProperty>()) != _typeToName.end(); }
        bool HasHandler(const std::string& name) const { return _nameToType.find(detail::MakeNameKey<GLTFPropertyAll>(name)) != _nameToType.end(); }
        bool HasHandler(const std::string& name, const detail::GLTFProperty& property) const 
        {
            return _nameToType.find(detail::MakeNameKey(name, property)) != _nameToType.end();
        }
        /****************************************************************************
        **                Public Property
        *****************************************************************************/

        /****************************************************************************
        **                Constructor and Destructor
        *****************************************************************************/
    protected:
        /****************************************************************************
        **                Protected Define Struct
        *****************************************************************************/
        struct GLTFPropertyAll : detail::GLTFProperty
        {
            GLTFPropertyAll() = delete;
        };
        struct GLTFHash
        {
            template<typename T1, typename T2>
            size_t operator()(const std::pair<T1, T2>& key) const
            {
                // This method of combining hashes is from boost::hash_combine - https://www.boost.org/doc/libs/1_67_0/boost/container_hash/hash.hpp
                auto fnCombine = [](size_t& seed, size_t value)
                {
                    seed ^= value + 0x9e3779b9 + (seed << 6) + (seed >> 2);
                };

                std::hash<T1> hash1;
                std::hash<T2> hash2;

                size_t hash = {};

                fnCombine(hash, hash1(std::get<0>(key)));
                fnCombine(hash, hash2(std::get<1>(key)));

                return hash;
            }
        };
        /****************************************************************************
        **                Protected Function
        *****************************************************************************/
        TReturn Process(const detail::TypeKey& key, std::add_lvalue_reference_t<const TArgs> ...args) const
        {
            auto it = _handlers.find(key);
            if (it == _handlers.end()) { throw detail::error::GLTFException("No handler is registered for the specified type_index"); }
            return it->second(args...);
        }
        template<typename TExtension, typename TArg>
        static auto Convert(const TArg& arg) ->std::enable_if_t< std::is_base_of<detail::GLTFExtension, TArg>::value, const TExtension&>
        {
            return dynamic_cast<const TExtension&>(arg);
        }
        template<typename TExtension, typename TArg>
        static auto Convert(const TArg& arg) -> std::enable_if_t<!std::is_base_of<detail::GLTFExtension, TArg>::value, const TArg&>
        {
            return arg;
        }
        /****************************************************************************
        **                Protected Property
        *****************************************************************************/
        std::unordered_map<detail::TypeKey, Func, GLTFHash> _handlers;
        std::unordered_map<detail::TypeKey, std::string, GLTFHash>     _typeToName;
        std::unordered_map<detail::NameKey, std::type_index, GLTFHash> _nameToType;
    };
    
    struct ExtensionPair
    {
        std::string Name;
        std::string Value;
    };

    class GLTFDocument;

    class ExtensionSerializer final : public ExtensionHandlers<std::string, detail::GLTFExtension, GLTFDocument, ExtensionSerializer>
    {
    public:
        ExtensionPair Serialize(const detail::GLTFExtension& extension, const detail::GLTFProperty& property, const GLTFDocument document) const;
    };

    class ExtensionDeserializer final : public ExtensionHandlers<std::unique_ptr<detail::GLTFExtension>, std::string, ExtensionDeserializer>
    {
    public:
        std::unique_ptr<detail::GLTFExtension> DeSerialize(const ExtensionPair& extension, const detail::GLTFProperty& property) const;
    };
}
#endif