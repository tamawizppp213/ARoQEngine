//////////////////////////////////////////////////////////////////////////////////
///             @file   GLTFStreamCacheLRU.hpp
///             @brief  Stream Cache LRU
///             @author Copyright (c) Microsoft Corporation. All rights reserved.Licensed under the MIT License.
///                     Partially edit by Toide
///             @date   2022_05_23  
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GLTF_STREAM_LRU_HPP
#define GLTF_STREAM_LRU_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GLTFStreamCache.hpp"
#include <list>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#pragma warning(disable: 4239)
//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gltf
{
	namespace detail
	{
		/****************************************************************************
		*				  			StreamCacheLRU
		*************************************************************************//**
		*  @class     StreamCacheLRU
		*  @brief     Least Recently Used eviction
		*****************************************************************************/
		template<typename TStream>
		class StreamCacheLRU : public IStreamCache<TStream>
		{
		public:

			/****************************************************************************
			**                Public Function
			*****************************************************************************/
			TStream Get(const std::string& uri) override
			{
				auto iterator = _cache.Get(uri);
				if (_cache.Size() != _cacheList.size()) { throw error::GLTFException("Suze mismatch between cache map and list"); }
				if (iterator != _cacheList.begin()) { _cacheList.splice(_cacheList.begin(), _cacheList, iterator); }
				return iterator->second;
			}
			TStream Set(const std::string& uri, TStream stream) override
			{
				if (_cache.Has(uri)) { _cacheList.erase(_cache.Get(uri)); }
				
				auto iterator = _cache.Set(uri, Update(uri, std::move(stream)));
				if (_cache.Size() != _cache.Size())
				{
					throw error::GLTFException("Size mismatch between cache map and list");
				}

				return iterator->second;
			}
			size_t Size() const { return _cache.Size(); }
			/****************************************************************************
			**                Public Property
			*****************************************************************************/

			const size_t CacheMaxSize;
			/****************************************************************************
			**                Constructor and Destructor
			*****************************************************************************/
			template<typename Function>
			StreamCacheLRU(Function functionGenerate, size_t cacheMaxSize = std::numeric_limits<size_t>::max()) :
				CacheMaxSize(cacheMaxSize),
				_cache([functionGenerate, this](const std::string& uri) { return Update(uri, functionGenerate(uri)); }),
				_cacheList()
			{
				if (CacheMaxSize == 0U) { throw error::GLTFException("LRU max cache size must be greater than zero."); }
			}
		private:
			typedef std::list<std::pair<std::string, TStream>> StreamCacheLRUList;
			/****************************************************************************
			**                Private Function
			*****************************************************************************/
			typename StreamCacheLRUList::iterator Update(const std::string& uri, TStream& stream)
			{
				_cacheList.emplace_front(uri, std::move(stream));
				if (_cache.Size() == CacheMaxSize)
				{
					_cache.Erase(_cacheList.back().first);
				}
				if (_cache.Size() > CacheMaxSize)
				{
					_cacheList.pop_back();
				}
				return _cacheList.begin();
			}
			/****************************************************************************
			**                Private Property
			*****************************************************************************/
			StreamCache<typename StreamCacheLRUList::iterator> _cache;
			StreamCacheLRUList _cacheList;
		};

		using StreamReaderCacheLRU = StreamCacheLRU<std::shared_ptr<std::istream>>;
		using StreamWriterCacheLRU = StreamCacheLRU<std::shared_ptr<std::ostream>>;
	}
}
#endif