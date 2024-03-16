//////////////////////////////////////////////////////////////////////////////////
///             @file   IPAddress.hpp
///             @brief  IPAddress
///             @author Toide Yutaro
///             @date   2022_12_05
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef PACKET_QUEUE_HPP
#define PACKET_QUEUE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "MemoryStream.hpp"
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include <queue>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc
{
	class MemoryStream;
	
	/****************************************************************************
	*				  			 PacketQueue
	*************************************************************************//**
	*  @class     PacketQueue
	*  @brief     Packet buffer
	*****************************************************************************/
	class PacketQueue : public gu::NonCopyable
	{
		using DataSize = std::int32_t;
		struct PacketInfo
		{
			std::uint64_t Offset;
			std::uint64_t ByteSize;
		};
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Enqueue packet byte data to the memory stream.*/
		void Enqueue(const std::vector<std::uint8_t>& data, const std::uint64_t byteSize);

		/* @brief : Dequeue packet byte data from the memory stream. Return dataSize*/
		DataSize Dequeue(std::vector<std::uint8_t>& buffer, const std::uint64_t byteSize);

		
		void Clear() { _memoryStream.Clear(); }
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		PacketQueue();
		
		virtual ~PacketQueue();

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		MemoryStream _memoryStream;
		
		std::queue<PacketInfo> _offsetList = {};

		std::uint64_t _seekPosition = 0;

	};
}
#endif