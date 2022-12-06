#//////////////////////////////////////////////////////////////////////////////////
///             @file   MemoryStream.hpp
///             @brief  memory stream
///             @author Toide Yutaro
///             @date   2022_12_05
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef MEMORY_STREAM_HPP
#define MEMORY_STREAM_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
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
		void Seek(const std::int64_t offset, SeekOrigin origin);

		/* @ brief : Writes a block of bytes to the current stream using data read from the buffer.*/
		void Write(const std::vector<std::uint8_t>& buffer, const std::uint64_t offset, const std::uint64_t count);

		/* @brief : Writes 1 byte to the current position in the current stream.*/
		void WriteByte(const std::uint8_t byte);

		/* @brief : Read byte to write buffer vector*/
		void Read(std::vector<std::uint8_t>& buffer, const std::uint64_t offset, const std::uint64_t count);
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		const std::vector<std::uint8_t>& Get() const { return _stream; }
		
		void SetByteLength(std::uint64_t byteLength) { _stream.resize(byteLength); };
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
}
#endif