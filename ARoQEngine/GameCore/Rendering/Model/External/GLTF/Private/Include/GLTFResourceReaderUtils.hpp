//////////////////////////////////////////////////////////////////////////////////
///             @file   GLTFAnimationUtils.hpp
///             @brief  Animation Utils
///             @author Copyright (c) Microsoft Corporation. All rights reserved.Licensed under the MIT License.
///                     Partially edit by Toide
///             @date   2022_05_23  
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GLTF_RESOURCE_READER_UTILS_HPP
#define GLTF_RESOURCE_READER_UTILS_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GLTFExceptions.hpp"
#include <string>
#include <vector>
#include <cmath>

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

		namespace utils
		{
			constexpr char CHARACTER_SET[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
			constexpr size_t CharCountToByteCount        (size_t charCount) { return (charCount * 3U) / 4U; }
			constexpr size_t ByteCountToCharCount        (size_t byteCount) { return (byteCount / 3U) * 4U; }
			constexpr size_t ByteCountToCharCountReminder(size_t byteCount) { return byteCount % 3U; }

			/****************************************************************************
			*				  			Base64StringView
			****************************************************************************/
			/* @struct    Base64StringView
			*  @brief     String view
			*****************************************************************************/
			struct Base64StringView
			{
			public:
				/****************************************************************************
				**                Public Function
				*****************************************************************************/
				size_t GetCharCount() const { return std::distance(ItBegin, ItEnd); }
				size_t GetByteCount() const { return CharCountToByteCount(GetCharCount()); }
				std::string::const_iterator begin() const { return ItBegin; }
				std::string::const_iterator end  () const { return ItEnd; }

				/****************************************************************************
				**                Public Property
				*****************************************************************************/
				std::string::const_iterator ItBegin;
				std::string::const_iterator ItEnd;
				/****************************************************************************
				**                Constructor and Destructor
				*****************************************************************************/
				Base64StringView(std::string::const_iterator begin, std::string::const_iterator end) : ItBegin(begin), ItEnd(end)
				{
					while ((ItBegin != ItEnd) && (*(ItEnd - 1) == '=')) { --ItEnd; }
				}
				explicit Base64StringView(const std::string& string) : Base64StringView(string.begin(), string.end()){}

			};

			/****************************************************************************
			*				  			Base64BufferView
			****************************************************************************/
			/* @struct    Base64BufferView
			*  @brief     intended to be used as a pass-by-value function parameter
			*****************************************************************************/
			struct Base64BufferView
			{
			public:
				/****************************************************************************
				**                Public Function
				*****************************************************************************/
				
				/****************************************************************************
				**                Public Property
				*****************************************************************************/
				void* const Buffer;
				const size_t BufferByteLength;
				/****************************************************************************
				**                Constructor and Destructor
				*****************************************************************************/
				Base64BufferView(void* buffer, size_t bufferByteLength) : Buffer(buffer), BufferByteLength(bufferByteLength) {};
				template<typename T>
				explicit Base64BufferView(std::vector<T>& decodedDataBuffer) : Base64BufferView(decodedDataBuffer.data(), decodedDataBuffer.size() * sizeof(T))
				{
				}

			};
			/****************************************************************************
			*                       GetDecodeTable
			****************************************************************************/
			/* @fn        inline std::vector<uint8_t> GetDecodeTable()
			*  @brief     Return decode char table
			*  @param[in] void
			*  @return 　　std::vector<uint8_t>
			*****************************************************************************/
			inline std::vector<uint8_t> GetDecodeTable()
			{
				std::vector<uint8_t> decodeTable(128, std::numeric_limits<uint8_t>::max());
				static constexpr size_t characterSetCount = std::extent<decltype(CHARACTER_SET)>::value - 1U;

				for (size_t i = 0; i < characterSetCount; ++i)
				{
					decodeTable[static_cast<size_t>(CHARACTER_SET[i])] = static_cast<uint8_t>(i);
				}

				return decodeTable;
			}
			/****************************************************************************
			*                       Base64Decode
			****************************************************************************/
			/* @fn        inline void Base64Decode(Base64StringView encodedData, Base64BufferView decodedData, size_t byteToSkip)
			*  @brief     Base64Decode
			*  @param[in] Base64StringView encodeData
			*  @param[in] Base64BufferView decodeData
			*  @param[in] size_t byteToSkip
			*  @return 　　std::vector<uint8_t>
			*****************************************************************************/
			inline void Base64Decode(Base64StringView encodedData, Base64BufferView decodedData, size_t bytesToSkip)
			{
				if (encodedData.GetByteCount() != (decodedData.BufferByteLength + bytesToSkip))
				{
					throw detail::error::GLTFException("The specified decode buffer's size is incorrect");
				}

				static const std::vector<uint8_t> decodeTable = GetDecodeTable();

				uint32_t block          = 0U;
				uint32_t blockBits      = 0U;
				uint8_t* decodedBytePtr = static_cast<uint8_t*>(decodedData.Buffer);

				for (const auto encodedChar : encodedData)
				{
					// For platforms where char is unsigned(encodedChar < 0) results in a tautology warning.
					// Only do that test on platforms where char is signed.
					if ((std::numeric_limits<char>::is_signed && (static_cast<signed char>(encodedChar) < 0)) || (static_cast<size_t>(encodedChar) >= decodeTable.size()))
					{
						throw error::GLTFException("Invalid base64 character");
					}

					const auto decodedChar = decodeTable[encodedChar];

					if (decodedChar == std::numeric_limits<uint8_t>::max())
					{
						throw error::GLTFException("Invalid base64 character");
					}

					// Each character of a base64 string encodes 6 bits of data so left shift any remaining
					// bits to accomodate another character's worth of data before performing a bitwise OR
					block <<= 6U;
					block |= decodedChar & 0x3F;

					// Keep track of how many bits of the 'block' variable are currently used
					blockBits += 6U;

					// If there are 8 or more bits stored in 'block' then write a single byte to the output buffer
					if (blockBits >= 8U)
					{
						blockBits -= 8U;

						if (bytesToSkip > 0)
						{
							bytesToSkip--;
						}
						else
						{
							// Right shift the decoded data stored in 'block' so that the byte of
							// data to be written to the output buffer occupies the low-order byte
							*(decodedBytePtr++) = (block >> blockBits) & 0xFF;
						}

						// Generate a bitmask that discards only the byte just written to the output buffer during the bitwise AND
						block &= (1 << blockBits) - 1;
					}

				}
			}
			inline std::vector<uint8_t> Base64Decode(const Base64StringView& encodedData)
			{
				std::vector<uint8_t> decodedDataBuffer(encodedData.GetByteCount());

				Base64BufferView decodedData(decodedDataBuffer);
				Base64Decode(encodedData, decodedData, 0);

				return decodedDataBuffer;

			}
			inline std::vector<uint8_t> Base64Decode(const std::string& encodedData)
			{
				return Base64Decode(Base64StringView(encodedData));
			}
			inline bool IsURIBase64(const std::string& uri, std::string::const_iterator& begin, std::string::const_iterator& end)
			{
				static const char dataPrefix[]     = "data:";
				constexpr size_t  dataPrefixLength = std::extent<decltype(dataPrefix)>::value - 1;

				static const char base64Indicator[]    = ";base64,";
				constexpr size_t base64IndicatorLength = std::extent<decltype(base64Indicator)>::value - 1;

				if (uri.compare(0, dataPrefixLength, dataPrefix) == 0)
				{
					const size_t findIndex = uri.find(base64Indicator);

					if (findIndex != std::string::npos)
					{
						begin = uri.begin() + findIndex + base64IndicatorLength;
						end   = uri.end();

						return true;
					}
				}

				return false;
			}
			inline bool IsURIBase64(const std::string& uri)
			{
				std::string::const_iterator itBegin;
				std::string::const_iterator itEnd;
				return IsURIBase64(uri, itBegin, itEnd);
			}

			// Conversions of normalized component types to/from floats are explicitly defined in the 2.0 spec
			inline float ComponentToFloat(const float w) { return w; }
			inline float ComponentToFloat(const int8_t w) { return std::max(static_cast<float>(w) / 127.0f, -1.0f); }
			inline float ComponentToFloat(const uint8_t w) { return static_cast<float>(w) / 255.0f; }
			inline float ComponentToFloat(const int16_t w) { return std::max(static_cast<float>(w) / 32767.0f, -1.0f); }
			inline float ComponentToFloat(const uint16_t w) { return static_cast<float>(w) / 65535.0f; }

			template<typename T>
			inline T FloatToComponent(const float f)
			{
				static_assert(std::is_same<float, T>::value, "Microsoft::glTF::FloatToComponent: expecting float template type");
				return f;
			}
			template<> inline int8_t   FloatToComponent<int8_t>  (const float f) { return static_cast<int8_t>(std::round(f * 127.0f)); }
			template<> inline uint8_t  FloatToComponent<uint8_t> (const float f) { return static_cast<uint8_t>(std::round(f * 255.0f)); }
			template<> inline int16_t  FloatToComponent<int16_t> (const float f) { return static_cast<int16_t>(std::round(f * 32767.0f)); }
			template<> inline uint16_t FloatToComponent<uint16_t>(const float f) { return static_cast<uint16_t>(std::round(f * 65535.0f)); }
		}
	}
}
#endif