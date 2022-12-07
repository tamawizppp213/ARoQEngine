//////////////////////////////////////////////////////////////////////////////////
///             @file   MemoryStream.hpp
///             @brief  byte memory stream
///             @author Toide Yutaro
///             @date   2022_12_05
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef MEMORY_STREAM_HPP
#define MEMORY_STREAM_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/File/Include/BitConverter.hpp"
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc
{
	enum class SeekOrigin
	{
		Begin, 
		Current, 
		End
	};

	/****************************************************************************
	*				  			 MemoryStream
	*************************************************************************//**
	*  @class     MemoryStream
	*  @brief     Memory Stream Buffer
	*****************************************************************************/
	class MemoryStream
	{

	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Sets the position in the current stream to the specified value.*/
		void Seek(const std::int64_t offset, const SeekOrigin origin);

		/* @ brief : Writes a block of bytes to the current stream using data read from the buffer.*/
		void Write(const std::vector<std::uint8_t>& buffer, const std::uint64_t offset, const std::uint64_t count);

		/* @brief : Writes 1 byte to the current position in the current stream.*/
		void AppendByte(const std::uint8_t byte);

		/* @brief : Append */
		void Append(const std::vector<std::uint8_t>& buffer);

		/* @brief : Read byte array and proceed byte indexer
		            Not taking endian into account*/
		std::vector<std::uint8_t> Read(const std::uint64_t count);

		/* @brief : Read value and proceed byte indexer (+= sizeof(T))
		            Taking endian into account.*/
		template<typename T> requires std::is_integral_v<T> || std::is_floating_point_v<T>
		T Read(const bool isLittleEndian);

		/* Clear buffer*/
		void Clear();
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		const std::vector<std::uint8_t>& Get() const { return _stream; }
		
		std::uint64_t GetBufferSize() const { return _stream.size(); }

		void SetBufferSize(const std::uint64_t byteLength) { _stream.resize(byteLength); };
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		MemoryStream() = default;

		MemoryStream(const std::vector<std::uint8_t>& array);

		~MemoryStream();
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
	
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::vector<std::uint8_t> _stream = {}; // actual stream buffer to store buffer

		std::uint64_t _position = 0; // current pointing position in the array
	};

#pragma region Template Implement
	template<typename T> requires std::is_integral_v<T> || std::is_floating_point_v<T>
	T MemoryStream::Read(const bool isLittleEndian)
	{
		T result = BitConverter::GetValue(_stream, _position, isLittleEndian);
		_position += result;
		return result;
	}

#pragma endregion Template Implement
}
#endif