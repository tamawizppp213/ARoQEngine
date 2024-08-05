//////////////////////////////////////////////////////////////////////////////////
///             @file   BitConverter.hpp
///             @brief  Default type -> byte vector array class
///             @author Toide Yutaro
///             @date   2022_12_05
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef BIT_CONVERTER_HPP
#define BIT_CONVERTER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <cassert>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

/****************************************************************************
*				  			 BitConverter
****************************************************************************/
/* @class     BitConverter
*  @brief     Convert default type into  byte vector array
*****************************************************************************/
class BitConverter
{

public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	/* @brief : Convert bytes array*/
	template<typename T> requires std::is_integral_v<T> || std::is_floating_point_v<T>
	static std::vector<std::uint8_t> GetBytes(const T value);

	/* @brief : Convert byte array data into T value*/
	template<typename T> requires std::is_integral_v<T> || std::is_floating_point_v<T>
	static T GetValue(const std::vector<std::uint8_t>& inputs, const std::uint32_t startIndex = 0);
	/****************************************************************************
	**                Public Property
	*****************************************************************************/
	static bool IsLittleEndian() { return _isLittleEndian; }
	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	BitConverter()  = delete; // Not create instance

	~BitConverter() = default;
protected:
	/****************************************************************************
	**                Protected Function
	*****************************************************************************/

	/****************************************************************************
	**                Protected Property
	*****************************************************************************/
	static bool _isLittleEndian;
};

/****************************************************************************
*                     GetBytes
****************************************************************************/
/* @fn        template<typename T> std::vector<std::uint8_t> BitConverter::GetBytes(const T value, bool isLittleEndian)
*
*  @brief     Convert Default type input bytes array
*
*  @param[in] const T value
*
*  @return    std::vector<std::uint8_t>
*****************************************************************************/
template<typename T> requires std::is_integral_v<T> || std::is_floating_point_v<T>
std::vector<std::uint8_t> BitConverter::GetBytes(const T value)
{
	/*-------------------------------------------------------------------
	-                Set up byte array size
	---------------------------------------------------------------------*/
	const auto byteSize = sizeof(T);

	std::vector<std::uint8_t> result(byteSize);

	/*-------------------------------------------------------------------
	-                Convert T -> std::vector<std::uint8_t> 
	---------------------------------------------------------------------*/
	const std::uint64_t oneByteSize = 8;
	const std::uint8_t  bitMask     = 0xff;

	// Little endian set up
	if (_isLittleEndian)
	{
		for (std::uint64_t i = 0; i < byteSize; ++i)
		{
			const auto temp = value >> oneByteSize * i;
			
			result[i] = temp & bitMask;
		}
	}
	// Big endian set up
	else
	{
		for (std::uint64_t i = 0; i < byteSize; ++i)
		{
			const auto shiftCount = (byteSize - 1 - i);
			const auto temp       = value >> oneByteSize * shiftCount;
			
			result[i] = temp & bitMask;
		}
	}

	return result;
}

/****************************************************************************
*                     GetValue
****************************************************************************/
/* @fn        template<typename T> T BitConverter::GetValue(const std::vector<std::uint8_t>& inputs, const std::uint32_t startOffset, bool isLittleEndian)
*
*  @brief     Convert byte array data into T value
*
*  @param[in] const std::vector<std::uint8_t>& inputs
*  @oaram[in] const std::uint32_t startOffset
*
*  @return    std::vector<std::uint8_t>
*****************************************************************************/
template<typename T> requires std::is_integral_v<T> || std::is_floating_point_v<T>
T BitConverter::GetValue(const std::vector<std::uint8_t>& inputs, const std::uint32_t startOffset)
{
	/*-------------------------------------------------------------------
	-                Error check
	---------------------------------------------------------------------*/
	assert(!inputs.empty());

	assert(startOffset < inputs.size() - sizeof(T));

	/*-------------------------------------------------------------------
	-                Convert std::vector<std::uint8_t> -> T
	---------------------------------------------------------------------*/
	const std::uint64_t oneByteSize = 8;
	
	T result = 0;

	if (_isLittleEndian)
	{
		for (std::uint64_t i = 0; i < byteSize; ++i)
		{
			const std::uint8_t byteData = inputs[startOffset + i];
			
			result += byteData << oneByteSize * i;
		}
	}
	else
	{
		for (std::uint64_t i = 0; i < byteSize; ++i)
		{
			const std::uint8_t byteData = inputs[startOffset + byteSize - 1 - i];
			result += byteData << oneByteSize * i;
		}
	}
}
#endif