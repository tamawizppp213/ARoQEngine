//////////////////////////////////////////////////////////////////////////////////
///             @file   Serializer.hpp
///             @brief  Serialize
///             @author Toide Yutaro
///             @date   2022_12_05
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Network/Private/Include/Serializer.hpp"
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gc;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
Serializer::Serializer()
{
	/*-------------------------------------------------------------------
	-                 Devide Endian
	---------------------------------------------------------------------*/
	_endian = BitConverter::IsLittleEndian() ? Endian::LittleEndian : Endian::BigEndian;
}

Serializer::~Serializer()
{
	Clear();
}
#pragma endregion Constructor and Destructor

#pragma region Main Function
/****************************************************************************
*                      Serialize
*************************************************************************//**
*  @fn        void Serializer::Serialize(const std::vector<std::uint8_t>& byte)
*
*  @brief     Serialize byteData -> Write memory buffer
*
*  @param[in] const std::vector<std::uint8_t>
*
*  @return    void
*****************************************************************************/
void Serializer::Serialize(const std::vector<std::uint8_t>& byte)
{
	_stream.Append(byte);
}

/****************************************************************************
*                      Deserialize
*************************************************************************//**
*  @fn        std::string Serializer::Deserialize(const std::uint64_t byteLength)
*
*  @brief     Read std::string data
*
*  @param[in] const std::uint64_t byteLength
*
*  @return    void
*****************************************************************************/
std::string Serializer::Deserialize(const std::uint64_t byteLength)
{
	const auto byte = _stream.Read(byteLength);

	return std::string(byte.begin(), byte.end());
}

/****************************************************************************
*                      Clear
*************************************************************************//**
*  @fn        void Serializer::Clear()
*
*  @brief     Clear memory stream
*
*  @param[in] void
*
*  @return    void
*****************************************************************************/
void Serializer::Clear()
{
	_stream.Clear();
}
#pragma endregion Main Function