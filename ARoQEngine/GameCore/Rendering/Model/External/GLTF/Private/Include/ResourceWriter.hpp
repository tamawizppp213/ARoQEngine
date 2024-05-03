//////////////////////////////////////////////////////////////////////////////////
///             @file   GLTFResourceWriter.hpp
///             @brief  GLTF Resource Writer
///             @author Copyright (c) Microsoft Corporation. All rights reserved.Licensed under the MIT License.
///                     Partially edit by Toide
///             @date   2022_05_23  
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RESOURCE_WRITER_HPP
#define RESOURCE_WRITER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/GLTF/Public/Include/GLTFDocument.hpp"
#include "GLTFStreamCache.hpp"
#include "GLTFStreamUtils.hpp"
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
		*				  			ResourcceWriter
		*************************************************************************//**
		*  @class     ResourceWriter
		*  @brief     Resource Write
		*****************************************************************************/
		class ResourceWriter
		{
		public:
			/****************************************************************************
			**                Public Function
			*****************************************************************************/
			virtual std::string GenerateBufferURI(const std::string& bufferID) const = 0;
			
			void Write(const asset::GLTFBufferView& bufferView, const void* data);
			void Write(const asset::GLTFBufferView& bufferView, const void* data, const asset::GLTFAccessor& accessor);
			template<typename T>
			void Write(const asset::GLTFBufferView& bufferView, const std::vector<T>& data);
			template<typename T>
			void Write(const asset::GLTFBufferView& bufferView, const std::vector<T>& data, const asset::GLTFAccessor& accessor);

			void WriteExternal(const std::string& uri, const void* data, size_t byteLength) const;
			void WriteExternal(const std::string& uri, const std::string& data) const;
			template<typename T>
			void WriteExternal(const std::string& uri, const std::vector<T>& data) const { WriteExternal(uri, data.data(), data.size() * sizeof(T)); }
			/****************************************************************************
			**                Public Property
			*****************************************************************************/

			/****************************************************************************
			**                Constructor and Destructor
			*****************************************************************************/
			virtual ~ResourceWriter() = default;

		protected:
			/****************************************************************************
			**                Protected Function
			*****************************************************************************/
			ResourceWriter(std::unique_ptr<IStreamWriterCache> streamWriter);

			virtual std::ostream*  GetBufferStream(const std::string& bufferID) = 0;
			virtual std::streamoff GetBufferOffset(const std::string& bufferID) = 0;
			virtual void SetBufferOffset(const std::string& bufferID, const std::streamoff offset) = 0;

			/****************************************************************************
			**                Protected Property
			*****************************************************************************/
			std::unique_ptr<IStreamWriterCache> _streamWriterCache = nullptr;

		private:
			void WriteImpl(const asset::GLTFBufferView& bufferView, const void* data, std::streamoff totalOffset, size_t totalByteLength);
		};
		
		template<typename T>
		void ResourceWriter::Write(const asset::GLTFBufferView& bufferView, const std::vector<T>& data)
		{
			const auto byteLength = data.size() * sizeof(T);
			if (byteLength != bufferView.ByteLength)
			{
				throw error::InvalidGLTFException("The given vector size in bytes doesn't equal bufferView,byteLength");

			}
			Write(bufferView, data.data());
		}
		template<typename T>
		void ResourceWriter::Write(const asset::GLTFBufferView& bufferView, const std::vector<T>& data, const asset::GLTFAccessor& accessor)
		{
			const auto byteLength = data.size() * sizeof(T);
			if (byteLength != accessor.GetByteLength())
			{
				throw error::InvalidGLTFException("The given vector size in bytes doesn't equal the accessor's byte length");
			}

			Write(bufferView, data.data(), accessor);
		}
	}
}
#endif