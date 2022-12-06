//////////////////////////////////////////////////////////////////////////////////
///             @file   Network error code.hpp
///             @brief  Network error code
///             @author Toide Yutaro
///             @date   2022_12_04
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
MemoryStream::MemoryStream(const std::vector<std::uint8_t>& array) : _stream(array)
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
void MemoryStream::Seek(const std::int64_t offset, SeekOrigin origin)
{
	switch (origin)
	{
		case SeekOrigin::Begin: { _position = 0 + offset; break; }
		case SeekOrigin::End:   { _position = _stream.size() - 1 - offset; }
		default:                { _position += offset; }
	}

	if (_position >= _stream.size()) { throw std::runtime_error("Exceed max stream size"); }
}

void MemoryStream::Write(const std::vector<std::uint8_t>& buffer, const std::uint64_t offset, const std::uint64_t count)
{
	if (offset + count >= _stream.size()) { throw std::runtime_error("Exceed max stream size"); }

	std::memcpy(_stream.data(), &buffer[offset], count);
}

void MemoryStream::WriteByte(const std::uint8_t byte)
{
	_stream.push_back(byte);
}

void MemoryStream::Read(std::vector<std::uint8_t>& buffer, const std::uint64_t offset, const std::uint64_t count)
{

}
#pragma endregion Main Function