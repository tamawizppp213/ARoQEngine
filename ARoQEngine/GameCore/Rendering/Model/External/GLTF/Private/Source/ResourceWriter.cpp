//////////////////////////////////////////////////////////////////////////////////
//              @file   GLTFSchema.cpp
///             @brief  GLTF Schema
///             @author // Copyright (c) Microsoft Corporation. All rights reserved. Licensed under the MIT License.
///             @date   2022_05_22
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/ResourceWriter.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gltf;
using namespace gltf::detail;
using namespace gltf::detail::asset;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
ResourceWriter::ResourceWriter(std::unique_ptr<IStreamWriterCache> streamWriterCache) : _streamWriterCache(std::move(streamWriterCache))
{

}

void ResourceWriter::Write(const asset::GLTFBufferView& bufferView, const void* data)
{
	WriteImpl(bufferView, data, bufferView.ByteOffset, bufferView.ByteLength);
}

void ResourceWriter::Write(const asset::GLTFBufferView& bufferView, const void* data, const asset::GLTFAccessor& accessor)
{
	if (accessor.BufferViewID != bufferView.ID)
	{
		throw error::InvalidGLTFException("accessor.BufferViewID doesn't match bufferView.ID");
	}

	const auto componentTypeSize = GLTFAccessor::GetComponentTypeSize(accessor.ComponentDataType);
	// From the glTF 2.0 spec: the offset of an accessor into a bufferView (accessor.byteOffset) must be a multiple of the size of the accessor's component type
	if (accessor.ByteOffset % componentTypeSize)
	{
		throw error::InvalidGLTFException("accessor.ByteOffset must be a multiple of the accessor's component type size");
	}

	// From the glTF 2.0 spec: the offset of an accessor into a buffer (accessor.byteOffset + bufferView.byteOffset) must be a multiple of the size of the accessor's component type
	if ((accessor.ByteOffset + bufferView.ByteOffset) % componentTypeSize)
	{
		throw error::InvalidGLTFException("accessor.byteOffset + bufferView.byteOffset must be a multiple of the accessor's component type size");
	}

	const auto accessorByteLength = accessor.GetByteLength();
	// Ensure there is enough room in the BufferView for the accessor's data
	if (bufferView.ByteLength < accessor.ByteOffset + accessorByteLength)
	{
		throw error::InvalidGLTFException("accessor offset and byte length exceed the buffer view's byte length");
	}

	WriteImpl(bufferView, data, bufferView.ByteOffset + accessor.ByteOffset, accessorByteLength);
}

void ResourceWriter::WriteExternal(const std::string& uri, const void* data, size_t byteLength) const
{
	if (auto stream = _streamWriterCache->Get(uri))
	{
		StreamUtils::WriteBinary(*stream, data, byteLength);
	}
}

void ResourceWriter::WriteExternal(const std::string& uri, const std::string& data) const 
{
	WriteExternal(uri, data.c_str(), data.length());
}

void ResourceWriter::WriteImpl(const GLTFBufferView& bufferView, const void* data, std::streamoff totalOffset, size_t totalByteLength)
{
	if (auto bufferStream = GetBufferStream(bufferView.BufferID))
	{
		const auto bufferOffset = GetBufferOffset(bufferView.BufferID);

		if (totalOffset < bufferOffset)
		{
			throw error::InvalidGLTFException("Stream 'put' pointer is already ahead of specified offset");
		}
		else if (totalOffset > bufferOffset)
		{
			const auto padSize = static_cast<size_t>(totalOffset - bufferOffset);
			const auto padData = std::make_unique<char[]>(padSize);

			StreamUtils::WriteBinary(*bufferStream, padData.get(), padSize);
		}

		SetBufferOffset(bufferView.BufferID, totalOffset);

		if (StreamUtils::WriteBinary(*bufferStream, data, totalByteLength) != totalByteLength)
		{
			throw error::InvalidGLTFException("An unexpected number of bytes were output to the stream");
		}

		SetBufferOffset(bufferView.BufferID, totalOffset + totalByteLength);
	}
}