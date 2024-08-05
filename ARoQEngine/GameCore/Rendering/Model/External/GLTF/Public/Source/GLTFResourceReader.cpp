//////////////////////////////////////////////////////////////////////////////////
//              @file   GLTFParser.cpp
///             @brief  GLTF File Parser
///             @author Partially edit by Toide
///             @date   2022_05_30
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/GLTF/Public/Include/GLTFResourceReader.hpp"
#include "GameCore/Rendering/Model/External/GLTF/Private/Include/GLTFConstants.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gltf;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
namespace
{
    template<typename T>
    std::vector<float> DecodeToFloats(const gltf::GLTFDocument& doc, const GLTFResourceReader& reader, const detail::asset::GLTFAccessor& accessor)
    {
        std::vector<T> rawData = reader.ReadBinaryData<T>(doc, accessor);

        std::vector<float> floatData;
        floatData.reserve(rawData.size());

        if (accessor.IsNormalized)
        {
            for (size_t i = 0; i < rawData.size(); ++i)
                floatData.push_back(detail::utils::ComponentToFloat(rawData[i]));
        }
        else
        {
            for (size_t i = 0; i < rawData.size(); ++i)
                floatData.push_back(static_cast<float>(rawData[i]));
        }

        return floatData;
    }

    bool ParseChunkType(const char* expectedChunkType, std::istream& stream)
    {
        using namespace detail::define;

        char buffer[GLB_CHUNK_TYPE_SIZE];
        stream.read(buffer, GLB_CHUNK_TYPE_SIZE);
        if (stream.fail())
        {
            throw detail::error::InvalidGLTFException("Cannot read the chunk type: " + std::string(expectedChunkType));
        }
        auto cmp = memcmp(&buffer[0], expectedChunkType, GLB_CHUNK_TYPE_SIZE);
        return cmp == 0;
    }

    std::string ReadJson(std::istream& stream, size_t jsonLength)
    {
        using namespace detail::define;

        stream.seekg(GLB_HEADER_BYTE_SIZE);
        std::string json(jsonLength, '\0');
        detail::StreamUtils::ReadBinary(stream, &json[0], jsonLength);
        if (stream.fail())
        {
            throw detail::error::InvalidGLTFException("Cannot read the json from the GLB file");
        }
        return json;
    }
}

/****************************************************************************
*                       ReadFloatData
****************************************************************************/
/* @fn        template<typename T> std::vector<T> GLTFResourceReader::ReadBinaryData(const GLTFDocument& document, const detail::asset::GLTFAccessor& accessor) const
*  @brief     Read float data. Return std::vector<float>
*  @param[in] const GLTFDocument& document
*  @param[in] const detail::asset::GLTFBufferView& bufferView
*  @return Å@Å@std::vector<float>
*****************************************************************************/
std::vector<float>   GLTFResourceReader::ReadFloatData(const GLTFDocument& document, const detail::asset::GLTFAccessor& accessor) const
{
	using namespace detail::asset;
	switch (accessor.ComponentDataType)
	{
	case ComponentType::Component_Byte:
		return DecodeToFloats<int8_t>(document, *this, accessor);

	case ComponentType::Component_Unsigned_Byte:
		return DecodeToFloats<uint8_t>(document, *this, accessor);

	case ComponentType::Component_Short:
		return DecodeToFloats<int16_t>(document, *this, accessor);

	case ComponentType::Component_Unsigned_Short:
		return DecodeToFloats<uint16_t>(document, *this, accessor);

	case ComponentType::Component_Float:
		return ReadBinaryData<float>(document, accessor);

	default:
		throw detail::error::GLTFException("Unsupported accessor ComponentType");
	}
}

#pragma region GLBResourceReader
GLBResourceReader::GLBResourceReader(std::shared_ptr<const detail::IStreamReader> streamReader, std::shared_ptr<std::istream> glbStream)
    : GLTFResourceReader(std::move(streamReader)), _buffer(std::move(glbStream)), _bufferOffset()
{
    Initialize();
}
GLBResourceReader::GLBResourceReader(std::unique_ptr<detail::IStreamReaderCache>  streamCache, std::shared_ptr<std::istream> glbStream)
    : GLTFResourceReader(std::move(streamCache)), _buffer(std::move(glbStream)), _bufferOffset()
{
    Initialize();
}
std::shared_ptr<std::istream> GLBResourceReader::GetBinaryStream(const detail::asset::GLTFBuffer& buffer) const
{
    std::shared_ptr<std::istream> stream;

    if (buffer.URI.empty() || buffer.URI == detail::define::EMPTY_URI)
    {
        stream = _buffer;
    }
    else
    {
        stream = GLTFResourceReader::GetBinaryStream(buffer);
    }

    return stream;
}
std::streampos GLBResourceReader::GetBinaryStreamPos(const detail::asset::GLTFBuffer& buffer) const
{
    std::streampos streamPos;

    if (buffer.URI.empty() || buffer.URI == detail::define::EMPTY_URI)
    {
        streamPos = _bufferOffset;
    }
    else
    {
        streamPos = GLTFResourceReader::GetBinaryStreamPos(buffer);
    }
    return streamPos;
}

const std::string& GLBResourceReader::GetJson() const
{
    return _json;
}

void GLBResourceReader::Initialize()
{
    // Get the length of the stream before reading anything, to validate against later
    // NOTE: The approach used below with seekg to the end and then tellg may be problematic since
    // seekg is not guaranteed to give the number of bytes from the start of the file:
    // see http://stackoverflow.com/a/22986486. This is used elsewhere in the code though,
    // and seems to Do The Right Thing as of right now. TODO: discuss fixing this problem.
    const auto curPos = _buffer->tellg();
    _buffer->seekg(0, std::ios::end);
    uint32_t trueStreamLength = static_cast<uint32_t>(_buffer->tellg()); // in bytes (i.e. sizeof(char))
    _buffer->seekg(curPos); // reset the stream pointer where it was

    char magic[detail::define::GLB_HEADER_MAGIC_STRING_SIZE];
    _buffer->read(magic, sizeof(magic));
    if (_buffer->fail())
    {
        throw detail::error::InvalidGLTFException("Cannot read the magic number");
    }

    const uint32_t version = detail::StreamUtils::ReadBinary<uint32_t>(*_buffer);
    const uint32_t length  = detail::StreamUtils::ReadBinary<uint32_t>(*_buffer);

    // Verify that the length we just read actually matches the length of the stream
    if (trueStreamLength != length)
    {
        throw detail::error::InvalidGLTFException("File-reported file length does not match actual file length");
    }

    const uint32_t jsonChunkLength = detail::StreamUtils::ReadBinary<uint32_t>(*_buffer);

    if (!ParseChunkType(detail::define::GLB_CHUNK_TYPE_JSON, *_buffer))
    {
        throw detail::error::InvalidGLTFException("JSON chunk should appear first");
    }

    // validate header
    if (strncmp(magic, detail::define::GLB_HEADER_MAGIC_STRING, detail::define::GLB_HEADER_MAGIC_STRING_SIZE) != 0)
    {
        throw detail::error::InvalidGLTFException("Cannot find GLB magic bytes");
    }

    if (version != detail::define::GLB_HEADER_VERSION_2)
    {
        throw detail::error::InvalidGLTFException("Unsupported GLB Version: " + std::to_string(version));
    }

    // Length has been validated as the actual length of the file, but make sure to include the header bytes in this check
    if (length < (detail::define::GLB_HEADER_BYTE_SIZE + jsonChunkLength))
    {
        throw detail::error::InvalidGLTFException("File length " + std::to_string(length) + " less than content length " + std::to_string(jsonChunkLength) +
            " plus header length " + std::to_string(detail::define::GLB_HEADER_BYTE_SIZE));
    }

    _json = ReadJson(*_buffer, jsonChunkLength);

    // If length is exactly equal to the json chunk length, plus the header, it means there is no binary buffer chunk
    if (length == (detail::define::GLB_HEADER_BYTE_SIZE + jsonChunkLength))
    {
        return;
    }

    // Read the length of the binary buffer chunk
    const uint32_t bufferChunkLength = detail::StreamUtils::ReadBinary<uint32_t>(*_buffer);

    if (!ParseChunkType(detail::define::GLB_CHUNK_TYPE_BIN, *_buffer))
    {
        throw detail::error::InvalidGLTFException("Binary chunk should appear second");
    }

    // Verify that the sum of the sizes of the chunks (plus the headers) matches the size of the file
    const uint32_t chunkSizeSum = detail::define::GLB_HEADER_BYTE_SIZE + jsonChunkLength + static_cast<uint32_t>(sizeof(bufferChunkLength)) + detail::define::GLB_CHUNK_TYPE_SIZE + bufferChunkLength;

    if (chunkSizeSum != length)
    {
        throw detail::error::InvalidGLTFException("File length does not match sum of length of component chunks");
    }

    _bufferOffset = _buffer->tellg();
}
#pragma endregion GLBResourceReader