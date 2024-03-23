//////////////////////////////////////////////////////////////////////////////////
///             @file   GLTFResourceReader.hpp
///             @brief  Resource reader (GLTF and GLB)
///             @author Copyright (c) Microsoft Corporation. All rights reserved.Licensed under the MIT License.
///                     Partially edit by Toide
///             @date   2022_05_12  
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GLTF_RESOURCE_READER_HPP
#define GLTF_RESOURCE_READER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GLTFDocument.hpp"
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFIStreamReader.hpp"
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFResourceReaderUtils.hpp"
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFStreamCacheLRU.hpp"
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFStreamUtils.hpp"
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFSchema.hpp"
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFValidation.hpp"
#include <memory>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gltf
{
	/****************************************************************************
	*				  			GLTFResourceReader
	*************************************************************************//**
	*  @class     GLTFResourceReader
	*  @brief     Resource Reader
	*****************************************************************************/
	class GLTFResourceReader
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		std::vector<uint8_t> ReadBinaryData(const GLTFDocument& document, const detail::asset::GLTFImage& image) const
		{
			std::vector<uint8_t> data;
			std::string::const_iterator itBegin;
			std::string::const_iterator itEnd;

			/*-------------------------------------------------------------------
			-                        Get Data
			---------------------------------------------------------------------*/
			if (image.URI.empty())
			{
				if (image.BufferViewId.empty()) { throw detail::error::GLTFException("Invalid image, both uri and bufferView are unspecified"); }
				data = ReadBinaryData<uint8_t>(document, document.BufferViews.Get(image.BufferViewId));
			}
			else if (detail::utils::IsURIBase64(image.URI, itBegin, itEnd))
			{
				data = ReadBinaryDataURI<uint8_t>({ itBegin, itEnd });
			}
			else if (auto stream = _streamReaderCache->Get(image.URI))
			{
				data = detail::StreamUtils::ReadBinaryFull<uint8_t>(*stream);
			}
			else
			{
				throw detail::error::GLTFException("Unable to read image data");
			}
			return data;
		}
		template<typename T>
		std::vector<T>       ReadBinaryData(const GLTFDocument& document, const detail::asset::GLTFAccessor& accessor) const;
		template<typename T>
		std::vector<T>       ReadBinaryData(const GLTFDocument& document, const detail::asset::GLTFBufferView& bufferView) const;

		std::vector<float>   ReadFloatData(const GLTFDocument& document, const detail::asset::GLTFAccessor& accessor) const;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GLTFResourceReader(std::shared_ptr<const detail::IStreamReader> streamReader) : GLTFResourceReader(detail::MakeStreamReaderCache<detail::StreamReaderCacheLRU>(std::move(streamReader), 16U)){}
		GLTFResourceReader(std::unique_ptr<detail::IStreamReaderCache> streamCache) : _streamReaderCache(std::move(streamCache)){}
		GLTFResourceReader(GLTFResourceReader&&) = default;
		virtual ~GLTFResourceReader() = default;
	protected:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		template<typename T>
		std::vector<T> ReadAccessor(const GLTFDocument& document, const detail::asset::GLTFAccessor& accessor) const;

		template<typename T>
		std::vector<T> ReadSparceAcessor(const GLTFDocument& gltfDocument, const detail::asset::GLTFAccessor& accessor) const;

		virtual std::shared_ptr<std::istream> GetBinaryStream(const detail::asset::GLTFBuffer& buffer) const
		{
			if (buffer.URI.empty()) { throw detail::error::GLTFException("Buffer URI was not specified."); }
			return _streamReaderCache->Get(buffer.URI);
		}
		virtual std::streampos GetBinaryStreamPos(const detail::asset::GLTFBuffer&) const { return{}; }
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		void ReadBinaryDataURI(detail::utils::Base64StringView encodedData, detail::utils::Base64BufferView decodedData, const std::streamoff* offsetOverride = nullptr) const
		{
			size_t offsetAdjustment = 0;
			if (offsetOverride)
			{
				if (*offsetOverride < 0)
				{
					throw detail::error::GLTFException("Negative offsets are not supported");
				}

				auto byteCount = static_cast<size_t>(*offsetOverride);
				offsetAdjustment = detail::utils::ByteCountToCharCountReminder(byteCount);
				const size_t offsetBegin = detail::utils::ByteCountToCharCount(byteCount);

				if (offsetBegin < encodedData.GetCharCount())
				{
					encodedData.ItBegin += offsetBegin;
				}
				else
				{
					throw detail::error::GLTFException("Offset position as a base64 character index is outside the input range");
				}
			}

			auto offsetByteCount = decodedData.BufferByteLength + offsetAdjustment;
			size_t offsetEnd = detail::utils::ByteCountToCharCount(offsetByteCount);

			switch (detail::utils::ByteCountToCharCountReminder(offsetByteCount))
			{
			case 1U:
				offsetEnd += 2U;// Decode 2 more characters for 1 extra byte
				break;
			case 2U:
				offsetEnd += 3U;// Decode 3 more characters for 2 extra bytes
				break;
			}

			if (offsetEnd <= encodedData.GetCharCount())
			{
				encodedData.ItEnd = encodedData.ItBegin + offsetEnd;
			}
			else
			{
				throw detail::error::GLTFException("Offset position as a base64 character index is outside the input range");
			}

			Base64Decode(encodedData, decodedData, offsetAdjustment);
		}
		template<typename T>
		std::vector<T> ReadBinaryDataURI(detail::utils::Base64StringView encodedData, const std::streamoff* offsetOverride = nullptr, const size_t* componentCountOverride = nullptr) const
		{
			size_t componentCount;

			if (componentCountOverride) { componentCount = *componentCountOverride; }
			else { componentCount = encodedData.GetByteCount() / sizeof(T); }

			std::vector<T> decodedData(componentCount);
			ReadBinaryDataURI(encodedData, detail::utils::Base64BufferView(decodedData), offsetOverride);
			return decodedData;
		}

		template<typename T>
		std::vector<T> ReadBinaryData(const detail::asset::GLTFBuffer& buffer, std::streamoff offset, size_t componentCount) const;

		template<typename T>
		std::vector<T> ReadBinaryDataInterleaved(const detail::asset::GLTFBuffer& buffer, std::streamoff offset, size_t elementCount, uint8_t typeCount, size_t stride) const;
		
		template<typename T, typename I>
		void ReadSparseBinaryData(const GLTFDocument& gltfDocument, std::vector<T>& baseData, const detail::asset::GLTFAccessor& accessor) const;

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		std::unique_ptr < detail::IStreamReaderCache > _streamReaderCache;
	};

	/****************************************************************************
	*				  			GLBResourceReader
	*************************************************************************//**
	*  @class     GLBResourceReader
	*  @brief     GLB Resource read
	*****************************************************************************/
	class GLBResourceReader : public GLTFResourceReader
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		std::shared_ptr<std::istream> GetBinaryStream(const detail::asset::GLTFBuffer& buffer) const override;
		std::streampos                GetBinaryStreamPos(const detail::asset::GLTFBuffer& buffer) const override;

		const std::string& GetJson() const;
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GLBResourceReader(std::shared_ptr<const detail::IStreamReader> streamReader, std::shared_ptr<std::istream> glbStream);
		GLBResourceReader(std::unique_ptr<detail::IStreamReaderCache>  streamCache, std::shared_ptr<std::istream> glbStream);
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		void Initialize();

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		std::string _json;
		std::shared_ptr<std::istream> _buffer;
		std::streamoff                _bufferOffset;
	};

	
	/****************************************************************************
	*                       ReadBinaryData
	*************************************************************************//**
	*  @fn        template<typename T> std::vector<T> GLTFResourceReader::ReadBinaryData(const GLTFDocument& document, const detail::asset::GLTFAccessor& accessor) const
	*  @brief     Read binary function (accessor). Return std::vector<T>
	*  @param[in] const GLTFDocument& document
	*  @param[in] const detail::asset::GLTFAccessor& accessor
	*  @return 　　std::vector<T>
	*****************************************************************************/
	template<typename T>
	std::vector<T>       GLTFResourceReader::ReadBinaryData(const GLTFDocument& document, const detail::asset::GLTFAccessor& accessor) const
	{
		using namespace detail::asset;

		bool isValid;
		
		switch (accessor.ComponentDataType)
		{
			case ComponentType::Component_Byte:           isValid = std::is_same<T, int8_t>::value; break;
			case ComponentType::Component_Unsigned_Byte : isValid = std::is_same<T, uint8_t>::value; break;
			case ComponentType::Component_Short         : isValid = std::is_same<T, int16_t>::value; break;
			case ComponentType::Component_Unsigned_Short: isValid = std::is_same<T, uint16_t>::value; break;
			case ComponentType::Component_Unsigned_Int  : isValid = std::is_same<T, uint32_t>::value; break;
			case ComponentType::Component_Float         : isValid = std::is_same<T, float>::value; break;
			default:
				throw detail::error::GLTFException("ReadAccessorData: Template type T does not match accessor ComponentType");
		}

		if (!isValid) { throw detail::error::GLTFException("ReadAccessorData: Template type T does not match accessor Component Type"); }
		
		detail::validation::ValidateAccessor(document, accessor);
		if (accessor.Sparse.Count > 0U) { return ReadSparceAcessor<T>(document, accessor); }

		return ReadAccessor<T>(document, accessor);
	}
	/****************************************************************************
	*                       ReadBinaryData
	*************************************************************************//**
	*  @fn        template<typename T> std::vector<T> GLTFResourceReader::ReadBinaryData(const GLTFDocument& document, const detail::asset::GLTFBufferView& bufferView) const
	*  @brief     Read binary function (bufferView). Return std::vector<T>
	*  @param[in] const GLTFDocument& document
	*  @param[in] const detail::asset::GLTFBufferView& bufferView
	*  @return 　　std::vector<T>
	*****************************************************************************/
	template<typename T>
	std::vector<T>       GLTFResourceReader::ReadBinaryData(const GLTFDocument& document, const detail::asset::GLTFBufferView& bufferView) const
	{
		using namespace detail::asset;

		const GLTFBuffer& buffer = document.Buffers.Get(bufferView.BufferID);
		detail::validation::ValidateBufferView(bufferView, buffer);
		
		auto count = bufferView.ByteLength / sizeof(T);
		Check(bufferView.ByteLength % sizeof(T) == 0);
		return ReadBinaryData<T>(buffer, bufferView.ByteOffset, count);
	}
	
	/****************************************************************************
	*                       ReadAccessor
	*************************************************************************//**
	*  @fn        template<typename T> std::vector<T> GLTFResourceReader::ReadAccessor(const GLTFDocument& document, const detail::asset::GLTFAccessor& accessor) const
	*  @brief     Read accessor data. Return std::vector<float>
	*  @param[in] const GLTFDocument& document
	*  @param[in] const detail::asset::GLTFAccessor& accessor
	*  @return 　　std::vector<T>
	*****************************************************************************/
	template<typename T>
	std::vector<T> GLTFResourceReader::ReadAccessor(const GLTFDocument& document, const detail::asset::GLTFAccessor& accessor) const
	{
		using namespace detail::asset;
		const auto typeCount   = GLTFAccessor::GetTypeCount(accessor.AccessorDataType);
		const auto elementSize = sizeof(T) * typeCount;

		std::vector<T> data;
		/*-------------------------------------------------------------------
		-                       Get Buffer Data
		---------------------------------------------------------------------*/
		const GLTFBufferView& bufferView = document.BufferViews.Get(accessor.BufferViewID);
		const GLTFBuffer&     buffer     = document.Buffers    .Get(bufferView.BufferID);
		const size_t          offset     = accessor.ByteOffset + bufferView.ByteOffset;

		/*-------------------------------------------------------------------
		-                      Read Buffer Data
		---------------------------------------------------------------------*/
		if (!bufferView.ByteStride || bufferView.ByteStride.Get() == elementSize)
		{
			data = ReadBinaryData<T>(buffer, offset, accessor.Count * typeCount);
		}
		else
		{
			data = ReadBinaryDataInterleaved<T>(buffer, offset, accessor.Count, typeCount, bufferView.ByteStride.Get());
		}
		return data;
	}
	/****************************************************************************
	*                       ReadSparceAcessor
	*************************************************************************//**
	*  @fn        std::vector<T> GLTFResourceReader::ReadSparceAcessor(const GLTFDocument& document, const detail::asset::GLTFAccessor& accessor) const
	*  @brief     Read accessor data. Return std::vector<float>
	*  @param[in] const GLTFDocument& document
	*  @param[in] const detail::asset::GLTFAccessor& accessor
	*  @return 　　std::vector<T>
	*****************************************************************************/
	template<typename T>
	std::vector<T> GLTFResourceReader::ReadSparceAcessor(const GLTFDocument& document, const detail::asset::GLTFAccessor& accessor) const
	{
		using namespace detail::asset;
		const auto typeCount   = GLTFAccessor::GetTypeCount(accessor.AccessorDataType);
		const auto elementSize = sizeof(T) * typeCount;

		/*-------------------------------------------------------------------
		-                        Read data
		---------------------------------------------------------------------*/
		std::vector<T> baseData;
		if (accessor.BufferViewID.empty()) { baseData.resize(typeCount * accessor.Count, T()); }
		else
		{
			const GLTFBufferView& bufferView = document.BufferViews.Get(accessor.BufferViewID);
			const GLTFBuffer&     buffer     = document.Buffers    .Get(bufferView.BufferID);
			const size_t          offset     = accessor.ByteOffset + bufferView.ByteOffset;
			if (!bufferView.ByteStride || bufferView.ByteStride.Get() == elementSize)
			{
				baseData = ReadBinaryData<T>(buffer, offset, accessor.Count * typeCount);
			}
			else
			{
				baseData = ReadBinaryDataInterleaved<T>(buffer, offset, accessor.Count, typeCount, bufferView.ByteStride.Get());
			}
		}

		/*-------------------------------------------------------------------
		-                        Read sparce data
		---------------------------------------------------------------------*/
		switch (accessor.Sparse.IndicesComponentType)
		{
			case ComponentType::Component_Unsigned_Byte : ReadSparseBinaryData<T, uint8_t>(document, baseData, accessor); break;
			case ComponentType::Component_Unsigned_Short: ReadSparseBinaryData<T, uint16_t>(document, baseData, accessor); break;
			case ComponentType::Component_Unsigned_Int  : ReadSparseBinaryData<T, uint32_t>(document, baseData, accessor); break;
			default:
				throw detail::error::GLTFException("Unsupported sparce indices ComponentType");
			
		}
		return baseData;
	}
	/****************************************************************************
	*                       ReadBinaryDataURi
	*************************************************************************//**
	*  @fn        void GLTFResourceReader::ReadBinaryDataURI(detail::utils::Base64StringView encodedData, detail::utils::Base64BufferView decodedData, const std::streamoff* offsetOverride) const
	*  @brief     Read binary data uri 
	*  @param[in] detail::utils::Base64StringView
	*  @param[in] detail::utils::Base64BufferView decodedData, 
	*  @param[in] const std::streamoff* offsetOverride
	*  @return 　　void
	*****************************************************************************/
	
	/****************************************************************************
	*                       ReadBinaryDataURI
	*************************************************************************//**
	*  @fn        std::vector<T> GLTFResourceReader::ReadBinaryDataURI(detail::utils::Base64StringView encodedData, const std::streamoff* offsetOverride, const size_t* componentCountOverride) const
	*  @brief     Read binary data uri
	*  @param[in] detail::utils::Base64StringView
	*  @param[in] const std::streamoff* offsetOverride, 
	*  @param[in] const size_t* componentCountOverride
	*  @return 　　std::vector<T>
	*****************************************************************************/
	
	/****************************************************************************
	*                       ReadBinaryData
	*************************************************************************//**
	*  @fn        std::vector<T> GLTFResourceReader::ReadBinaryData(const detail::asset::GLTFBuffer& buffer, std::streamoff offset, size_t componentCount) const
	*  @brief     Read binary data
	*  @param[in] const detail::asset::GLTFBuffer& buffer, 
	*  @param[in,out] std::streamoff offset, 
	*  @param[in]     size_t componentCount
	*  @return 　　std::vector<T>
	*****************************************************************************/
	template<typename T>
	std::vector<T> GLTFResourceReader::ReadBinaryData(const detail::asset::GLTFBuffer& buffer, std::streamoff offset, size_t componentCount) const
	{
		std::vector<T> data;
		std::string::const_iterator itBegin;
		std::string::const_iterator itEnd;

		if (detail::utils::IsURIBase64(buffer.URI, itBegin, itEnd))
		{
			data = ReadBinaryDataURI<T>({ itBegin, itEnd }, &offset, &componentCount);
		}
		else
		{
			data.resize(componentCount);
			auto bufferStream    = GetBinaryStream(buffer);
			auto bufferStreamPos = GetBinaryStreamPos(buffer);

			bufferStream->seekg(bufferStreamPos);
			bufferStream->seekg(offset, std::ios_base::cur);

			detail::StreamUtils::ReadBinary(*bufferStream, reinterpret_cast<char*>(data.data()), componentCount * sizeof(T));
		}

		return data;
	}
	/****************************************************************************
	*                       ReadBinaryDataInterleaved
	*************************************************************************//**
	*  @fn        std::vector<T> GLTFResourceReader::ReadBinaryDataInterleaved(const detail::asset::GLTFBuffer& buffer, std::streamoff offset, size_t elementCount, uint8_t typeCount, size_t stride) const
	*  @brief     Read binary data interleaved  
	*  @param[in] const detail::asset::GLTFBuffer& buffer,
	*  @param[in,out] std::streamoff offset,
	*  @param[in] size_t elementCount, 
	*  @param[in] uint8_t typeCount, 
	*  @param[in] size_t stride
	*  @return 　　std::vector<T>
	*****************************************************************************/
	template<typename T>
	std::vector<T> GLTFResourceReader::ReadBinaryDataInterleaved(const detail::asset::GLTFBuffer& buffer, std::streamoff offset, size_t elementCount, uint8_t typeCount, size_t stride) const
	{
		using namespace detail::asset;

		const size_t elementSize    = sizeof(T) * typeCount;
		const size_t componentCount = elementCount * typeCount;

		std::vector<T> data(componentCount);

		std::string::const_iterator itBegin;
		std::string::const_iterator itEnd;

		if (detail::utils::IsURIBase64(buffer.URI, itBegin, itEnd))
		{
			detail::utils::Base64StringView encodedData(itBegin, itEnd);

			for (size_t componentsRead = 0U; componentsRead < componentCount; componentsRead += typeCount, offset += stride)
			{
				ReadBinaryDataURI(encodedData, detail::utils::Base64BufferView(data.data() + componentsRead, elementSize), &offset);
			}
		}
		else
		{
			auto bufferStream    = GetBinaryStream(buffer);
			auto bufferStreamPos = GetBinaryStreamPos(buffer) + offset;

			for (size_t componentsRead = 0U; componentsRead < componentCount; componentsRead += typeCount)
			{
				bufferStream->seekg(bufferStreamPos);
				bufferStreamPos += stride;

				detail::StreamUtils::ReadBinary(*bufferStream, reinterpret_cast<char*>(data.data() + componentsRead), elementSize);
			}
		}

		return data;
	}
	/****************************************************************************
	*                       ReadBinaryDataInterleaved
	*************************************************************************//**
	*  @fn        template<typename T, typename I>
				　　void GLTFResourceReader::ReadSparseBinaryData(const GLTFDocument& gltfDocument, std::vector<T>& baseData, const detail::asset::GLTFAccessor& accessor) const
	*  @brief     Read binary data interleaved
	*  @param[in] const GLTFDocument& gltfDocument, 
	*  @param[in] std::vector<T>& baseData,
	*  @param[in] const detail::asset::GLTFAccessor& accessor
	*  @return 　　void
	*****************************************************************************/
	template<typename T, typename I>
	void GLTFResourceReader::ReadSparseBinaryData(const GLTFDocument& gltfDocument, std::vector<T>& baseData, const detail::asset::GLTFAccessor& accessor) const
	{
		using namespace detail::asset;

		const auto typeCount   = GLTFAccessor::GetTypeCount(accessor.AccessorDataType);
		const auto elementSize = sizeof(T) * typeCount;

		const size_t count = accessor.Sparse.Count;

		const GLTFBufferView& indicesBufferView = gltfDocument.BufferViews.Get(accessor.Sparse.IndicesBufferViewID);
		const GLTFBuffer&     indicesBuffer     = gltfDocument.Buffers.Get(indicesBufferView.BufferID);
		const size_t          indicesOffset     = accessor.Sparse.IndicesByteOffset + indicesBufferView.ByteOffset;

		const GLTFBufferView& valuesBufferView = gltfDocument.BufferViews.Get(accessor.Sparse.ValuesBufferViewID);
		const GLTFBuffer&     valuesBuffer     = gltfDocument.Buffers.Get(valuesBufferView.BufferID);
		const size_t          valuesOffset     = accessor.Sparse.ValuesByteOffset + valuesBufferView.ByteOffset;

		std::vector<I> indices;

		if (!indicesBufferView.ByteStride || indicesBufferView.ByteStride.Get() == sizeof(I))
		{
			indices = ReadBinaryData<I>(indicesBuffer, indicesOffset, count);
		}
		else
		{
			indices = ReadBinaryDataInterleaved<I>(indicesBuffer, indicesOffset, count, 1U, indicesBufferView.ByteStride.Get());
		}

		std::vector<T> values;

		if (!valuesBufferView.ByteStride || valuesBufferView.ByteStride.Get() == elementSize)
		{
			values = ReadBinaryData<T>(valuesBuffer, valuesOffset, count * typeCount);
		}
		else
		{
			values = ReadBinaryDataInterleaved<T>(valuesBuffer, valuesOffset, count, typeCount, valuesBufferView.ByteStride.Get());
		}

		for (size_t i = 0; i < indices.size(); i++)
		{
			Check(baseData.size() == accessor.Count * typeCount);
			static_assert(sizeof(I) <= sizeof(size_t), "sizeof(I) < sizeof(size_t)");
			if (0 <= indices[i] && static_cast<size_t>(indices[i]) < accessor.Count)
			{
				for (size_t j = 0; j < typeCount; j++)
				{
					baseData[indices[i] * typeCount + j] = values[i * typeCount + j];
				}
			}
		}
	}
}

#endif