//////////////////////////////////////////////////////////////////////////////////
///             @file   GLTFIStreamCache.hpp
///             @brief  Stream Cache Interface
///             @author Copyright (c) Microsoft Corporation. All rights reserved.Licensed under the MIT License.
///                     Partially edit by Toide
///             @date   2022_05_23  
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GLTF_ISTREAM_CACHE_HPP
#define GLTF_ISTREAM_CACHE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include <istream>
#include "GameUtility/Base/Include/GUSharedPointer.hpp"
#include <ostream>
#include <string>

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
		template<typename TStream>
		class IStreamCache
		{
		public:
			virtual ~IStreamCache() = default;
			virtual TStream Get(const std::string& uri) = 0;
			virtual TStream Set(const std::string& uri, TStream stream) = 0;
		};
		using IStreamReaderCache = IStreamCache<std::shared_ptr<std::istream>>;
		using IStreamWriterCache = IStreamCache<std::shared_ptr<std::ostream>>;
	}
}
#endif