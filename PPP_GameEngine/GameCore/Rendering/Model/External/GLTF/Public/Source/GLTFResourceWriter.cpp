//////////////////////////////////////////////////////////////////////////////////
//              @file   GLTFResourceWriter.cpp
///             @brief  GLTF Resource Writer
///             @author Partially edit by Toide
///             @date   2022_05_30
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/GLTF/Public/Include/GLTFResourceWriter.hpp"
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFStreamCacheLRU.hpp"
#include <sstream>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gltf;
using namespace gltf::detail;
using namespace gltf::detail::define;
#pragma warning(disable: 26815)
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
namespace
{
	uint32_t CalculatePadding(size_t byteLength)
	{
		const uint32_t alighmentSize = define::GLB_CHUNK_ALIGNMENT_SIZE;

		const auto padAlighment = byteLength % alighmentSize;
		const auto padding = padAlighment ? alighmentSize - padAlighment : 0U;

		return static_cast<uint32_t>(padding);
	}
}
#pragma region GLTFResourceWriter
GLTFResourceWriter::GLTFResourceWriter(std::shared_ptr<const detail::IStreamWriter> streamWriter)
	: GLTFResourceWriter(detail::MakeStreamWriterCache<StreamWriterCacheLRU>(std::move(streamWriter), 16U))
{

}
GLTFResourceWriter::GLTFResourceWriter(std::unique_ptr<detail::IStreamWriterCache> streamCache)
	: ResourceWriter(std::move(streamCache)), _streamOffsets()
{
	
}
std::string GLTFResourceWriter::GenerateBufferURI(const std::string& bufferID) const
{
	return _uriPrefix + bufferID + "." + gltf::detail::define::BUFFER_EXTENSION;
}
void GLTFResourceWriter::SetURIPrefix(std::string uriPrefix)
{
	_uriPrefix = std::move(uriPrefix);
}
std::ostream* GLTFResourceWriter::GetBufferStream(const std::string& bufferID)
{
    std::string uri = GenerateBufferURI(bufferID);
	return _streamWriterCache.get()->Get(uri).get();
}
std::streamoff GLTFResourceWriter::GetBufferOffset(const std::string& bufferID)
{
	return _streamOffsets[bufferID];
}
void GLTFResourceWriter::SetBufferOffset(const std::string& bufferID, std::streamoff offset)
{
	_streamOffsets[bufferID] = offset;
}

#pragma endregion GLTFResourceWriter
#pragma region GLBResourceWriter
GLBResourceWriter::GLBResourceWriter(std::shared_ptr<const detail::IStreamWriter> streamWriter)
	: GLBResourceWriter(std::move(streamWriter), std::make_unique<std::stringstream>())
{

}
GLBResourceWriter::GLBResourceWriter(std::shared_ptr<const detail::IStreamWriter> streamWriter, std::unique_ptr<std::iostream> tempBufferStream)
	: GLTFResourceWriter(std::move(streamWriter)), _stream(std::move(tempBufferStream))
{

}
GLBResourceWriter::GLBResourceWriter(std::unique_ptr<detail::IStreamWriterCache> streamCache)
	: GLBResourceWriter(std::move(streamCache), std::make_unique<std::stringstream>())
{

}
GLBResourceWriter::GLBResourceWriter(std::unique_ptr<detail::IStreamWriterCache> streamCache, std::unique_ptr<std::iostream> tempBufferStream)
	: GLTFResourceWriter(std::move(streamCache)), _stream(std::move(tempBufferStream))
{

}
void GLBResourceWriter::Flush(const std::string& manifest, const std::string& uri)
{
    uint32_t       jsonChunkLength   = static_cast<uint32_t>(manifest.length());
    const uint32_t jsonPaddingLength = ::CalculatePadding(jsonChunkLength);

    jsonChunkLength += jsonPaddingLength;

    uint32_t binaryChunkLength         = static_cast<uint32_t>(GetBufferOffset(GLB_BUFFER_ID));
    const uint32_t binaryPaddingLength = ::CalculatePadding(binaryChunkLength);

    binaryChunkLength += binaryPaddingLength;

    const uint32_t length = static_cast<uint32_t>(GLB_HEADER_BYTE_SIZE) // 12 bytes (GLB header) + 8 bytes (JSON header)
        + jsonChunkLength
        + static_cast<uint32_t>(sizeof(binaryChunkLength)) + GLB_CHUNK_TYPE_SIZE // 8 bytes (BIN header)
        + binaryChunkLength;

    auto stream = _streamWriterCache->Get(uri);

    // Write GLB header (12 bytes)
    StreamUtils::WriteBinary(*stream, GLB_HEADER_MAGIC_STRING, GLB_HEADER_MAGIC_STRING_SIZE);
    StreamUtils::WriteBinary(*stream, GLB_HEADER_VERSION_2);
    StreamUtils::WriteBinary(*stream, length);

    // Write JSON header (8 bytes)
    StreamUtils::WriteBinary(*stream, jsonChunkLength);
    StreamUtils::WriteBinary(*stream, GLB_CHUNK_TYPE_JSON, GLB_CHUNK_TYPE_SIZE);

    // Write JSON (indeterminate length)
    StreamUtils::WriteBinary(*stream, manifest);

    if (jsonPaddingLength > 0)
    {
        // GLB spec requires the JSON chunk to be padded with trailing space characters (0x20) to satisfy alignment requirements
        StreamUtils::WriteBinary(*stream, std::string(jsonPaddingLength, ' '));
    }

    // Write BIN header (8 bytes)
    StreamUtils::WriteBinary(*stream, binaryChunkLength);
    StreamUtils::WriteBinary(*stream, GLB_CHUNK_TYPE_BIN, GLB_CHUNK_TYPE_SIZE);

    // Write BIN contents (indeterminate length) - copy the temporary buffer's contents to the output stream
    if (binaryChunkLength > 0)
    {
        *stream << _stream->rdbuf();
    }

    if (binaryPaddingLength > 0)
    {
        // GLB spec requires the BIN chunk to be padded with trailing zeros (0x00) to satisfy alignment requirements
        StreamUtils::WriteBinary(*stream, std::vector<uint8_t>(binaryPaddingLength, 0));
    }
}
std::string   GLBResourceWriter::GenerateBufferURI(const std::string& bufferID) const
{
    std::string bufferUri;

    // Return an empty uri string when passed the GLB buffer id
    if (bufferID != GLB_BUFFER_ID)
    {
        bufferUri = GLTFResourceWriter::GenerateBufferURI(bufferID);
    }

    return bufferUri;
}
std::ostream* GLBResourceWriter::GetBufferStream(const std::string& bufferID)
{
    std::ostream* stream = _stream.get();

    if (bufferID != GLB_BUFFER_ID)
    {
        stream = GLTFResourceWriter::GetBufferStream(bufferID);
    }

    return stream;
}
#pragma endregion GLBResourceWriter