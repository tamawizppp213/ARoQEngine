//////////////////////////////////////////////////////////////////////////////////
///             @file   MemoryStream.hpp
///             @brief  byte memory stream
///             @author Toide Yutaro
///             @date   2022_12_05
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Network/Private/Include/MemoryStream.hpp"
#include <stdexcept>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gc;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor 
MemoryStream::MemoryStream(const std::vector<std::byte>& array) : _stream(array)
{

}

MemoryStream::~MemoryStream()
{
	if (!_stream.empty())
	{
		_stream.clear(); _stream.shrink_to_fit();
	}
}
#pragma endregion Constructor and Destructor

#pragma region Main Function
/****************************************************************************
*                     Seek
*************************************************************************//**
*  @fn        void MemoryStream::Seek(const std::int64_t offset, SeekOrigin origin)
*
*  @brief     Sets the read position in the current stream to the specified value
*
*  @param[in] const std::int64_t offset
*  @param[in] const SeekOrigin origin
*
*  @return    void
*****************************************************************************/
void MemoryStream::Seek(const std::int64_t offset, const SeekOrigin origin)
{
	switch (origin)
	{
		case SeekOrigin::Begin: { _position = 0 + offset; break; }
		case SeekOrigin::End:   { _position = _stream.size() - 1 - offset; }
		default:                { _position += offset; }
	}

	if (_position >= _stream.size()) { throw std::runtime_error("Exceed max stream size"); }
}

/****************************************************************************
*                     Write
*************************************************************************//**
*  @fn        void MemoryStream::Write(const std::vector<std::byte>& buffer, const std::uint64_t offset, const std::uint64_t count)
*
*  @brief     Write to memory streamBuffer.
*
*  @param[in] const std::vector<std::byte>& inputBuffer
*  @param[in] const std::uint64_t read start offset index
*  @pamra[in] const std::uint64_t byteCount
*
*  @return    void
*****************************************************************************/
void MemoryStream::Write(const std::vector<std::byte>& buffer, const std::uint64_t offset, const std::uint64_t count)
{
	if (offset + count >= _stream.size()) { throw std::runtime_error("Exceed max stream size"); }

	std::memcpy(_stream.data(), &buffer[offset], count);
}

/****************************************************************************
*                     AppendByte
*************************************************************************//**
*  @fn        void MemoryStream::AppendByte(const std::byte byte)
*
*  @brief     Push back byte
*
*  @param[in] const std::byte byte
*
*  @return    void
*****************************************************************************/
void MemoryStream::AppendByte(const std::byte byte)
{
	_stream.push_back(byte);
}

/****************************************************************************
*                     Read
*************************************************************************//**
*  @fn        std::vector<std::byte> MemoryStream::Read(const std::uint64_t count)
*
*  @brief     Read byte array and proceed byte indexer
		      Not taking endian into account
*
*  @param[in] const std::uint64_t arrayCount (= byteLength) 
*
*  @return    std::vector<std::byte>
*****************************************************************************/
std::vector<std::byte> MemoryStream::Read(const std::uint64_t count)
{
	std::vector<std::byte> result(count);

	// memory copy 
	std::memcpy(result.data(), &_stream[_position], count);

	// proceed seekPosition by byteLength
	_position += count;

	return result;
}
#pragma endregion Main Function