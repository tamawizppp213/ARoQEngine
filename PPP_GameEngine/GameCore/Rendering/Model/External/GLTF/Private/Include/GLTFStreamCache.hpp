//////////////////////////////////////////////////////////////////////////////////
///             @file   GLTFStreamCache.hpp
///             @brief  StreamCache
///             @author Copyright (c) Microsoft Corporation. All rights reserved.Licensed under the MIT License.
///                     Partially edit by Toide
///             @date   2022_05_23  
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GLTF_STREAM_CACHE_HPP
#define GLTF_STREAM_CACHE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GLTFExceptions.hpp"
#include "GLTFIStreamCache.hpp"
#include "GLTFIStreamReader.hpp"
#include "GLTFIStreamWriter.hpp"
#include <unordered_map>
#include <functional>
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
		*				  			TemplateClass
		*************************************************************************//**
		*  @class     TemplateClass
		*  @brief     temp
		*****************************************************************************/
		template<typename TStream>
		class StreamCache : public IStreamCache<TStream>
		{
		public:
			/****************************************************************************
			**                Public Function
			*****************************************************************************/
			bool Has(const std::string& uri) { return _cacheMap.find(uri) != _cacheMap.end(); }
			void Erase(const std::string& uri)
			{
				auto iterator = _cacheMap.find(uri);
				if (iterator != _cacheMap.end()) { _cacheMap.erase(iterator); }
				else { throw error::GLTFException("Passed key doesn't exist in the stream cache"); }
			}
			size_t Size() const { return _cacheMap.size(); }
			/****************************************************************************
			**                Public Member Variables
			*****************************************************************************/
			TStream Get(const std::string& uri) override
			{
				auto iterator = _cacheMap.find(uri);
				if (iterator == _cacheMap.end()) { return Set(uri, _cacheFunction(uri)); }
				else { return iterator->second; }
			}

			TStream Set(const std::string& uri, TStream stream) override { return _cacheMap[uri] = std::move(stream); }

			/****************************************************************************
			**                Constructor and Destructor
			*****************************************************************************/
			template<typename Function>
			StreamCache(Function functionGenerate) : _cacheMap(), _cacheFunction(functionGenerate)
			{

			}
		protected:
			/****************************************************************************
			**                Protected Function
			*****************************************************************************/

			/****************************************************************************
			**                Protected Member Variables
			*****************************************************************************/
			std::unordered_map<std::string, TStream> _cacheMap;
		private:
			/****************************************************************************
			**                Private Function
			*****************************************************************************/

			/****************************************************************************
			**                Private Member Variables
			*****************************************************************************/
			std::function<TStream(const std::string&)> _cacheFunction;
		};

		using StreamReaderCache = StreamCache<std::shared_ptr<std::istream>>;
		using StreamWriterCache = StreamCache<std::shared_ptr<std::ostream>>;

		template<typename TStreamCache, typename ...TArgs>
		std::unique_ptr<TStreamCache> MakeStreamReaderCache(std::shared_ptr<const IStreamReader> streamReader, TArgs&& ...args)
		{
			static_assert(std::is_base_of<IStreamReaderCache, TStreamCache>::value, "Template parameter TStreamCache must derive from IStreamReaderCache");

			return std::make_unique<TStreamCache>([streamReaderLocal = std::move(streamReader)](const std::string& uri)
			{
				return streamReaderLocal->GetInputStream(uri);
			}, std::forward<TArgs>(args)...);
		}

		template<typename TStreamCache, typename ...TArgs>
		std::unique_ptr<TStreamCache> MakeStreamWriterCache(std::shared_ptr<const IStreamWriter> streamWriter, TArgs&& ...args)
		{
			static_assert(std::is_base_of<IStreamWriterCache, TStreamCache>::value, "Template parameter TStreamCache must derive from IStreamWriterCache");

			return std::make_unique<TStreamCache>([streamWriterLocal = std::move(streamWriter)](const std::string& uri)
			{
				return streamWriterLocal->GetOutputStream(uri);
			}, std::forward<TArgs>(args)...);
		}
	}
}

#endif