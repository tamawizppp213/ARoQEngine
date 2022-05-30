//////////////////////////////////////////////////////////////////////////////////
///             @file   GLTFResourceWriter.hpp
///             @brief  Resource Writer (GLTF and GLB)
///             @author Copyright (c) Microsoft Corporation. All rights reserved.Licensed under the MIT License.
///                     Partially edit by Toide
///             @date   2022_05_12
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GLTF_RESOURCE_WRITER_HPP
#define GLTF_RESOURCE_WRITER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/ResourceWriter.hpp"
#include <memory>
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
		class IStreamWriter;
	}
	
	/****************************************************************************
	*				  			GLTFResourceWriter
	*************************************************************************//**
	*  @class     GLTFResourceWriter
	*  @brief     Resource write
	*****************************************************************************/
	class GLTFResourceWriter : public detail::ResourceWriter
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		std::string GenerateBufferURI(const std::string& bufferID) const override;
		void SetURIPrefix(std::string uriPrefix);
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GLTFResourceWriter(std::shared_ptr<const detail::IStreamWriter> streamWriter);
		GLTFResourceWriter(std::unique_ptr<detail::IStreamWriterCache> streamCache);

	protected:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		std::ostream*  GetBufferStream(const std::string& bufferID) override;
		std::streamoff GetBufferOffset(const std::string& bufferID) override;
		void           SetBufferOffset(const std::string& bufferID, std::streamoff offset) override;

	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		std::string _uriPrefix;
		std::unordered_map<std::string, std::streamoff> _streamOffsets;
	};

	/****************************************************************************
	*				  			GLTFResourceWriter
	*************************************************************************//**
	*  @class     GLTFResourceWriter
	*  @brief     Resource write
	*****************************************************************************/
	class GLBResourceWriter : public GLTFResourceWriter
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Flush(const std::string& manifest, const std::string& uri);
		std::string   GenerateBufferURI(const std::string& bufferID) const override;
		std::ostream* GetBufferStream  (const std::string& bufferID) override;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GLBResourceWriter(std::shared_ptr<const detail::IStreamWriter> streamWriter);
		GLBResourceWriter(std::shared_ptr<const detail::IStreamWriter> streamWriter, std::unique_ptr<std::iostream> tempBufferStream);
		GLBResourceWriter(std::unique_ptr<detail::IStreamWriterCache> streamCache);
		GLBResourceWriter(std::unique_ptr<detail::IStreamWriterCache> streamCache, std::unique_ptr<std::iostream> tempBufferStream);
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		std::shared_ptr<std::iostream> _stream;
		
	};
}
#endif