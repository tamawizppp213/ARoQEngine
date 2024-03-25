//////////////////////////////////////////////////////////////////////////////////
///             @file   Network error code.hpp
///             @brief  Network error code
///             @author Toide Yutaro
///             @date   2022_12_04
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
//////////////////////////////////////////////////////////////////////////////////
///             @file   Serializer.hpp
///             @brief  Serialize
///             @author Toide Yutaro
///             @date   2022_12_05
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Network/Private/Include/PacketQueue.hpp"
#include <mutex>
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gc;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor 
PacketQueue::PacketQueue()
{
	
}

PacketQueue::~PacketQueue()
{
	_memoryStream.Clear();
}
#pragma endregion Constructor and Destructor

#pragma region Main Function 
/****************************************************************************
*                      Enqueue
*************************************************************************//**
*  @fn        void PacketQueue::Enqueue(const std::vector<std::uint8_t>& data)
*
*  @brief     Enqueue packet byte data to the memory stream.
*
*  @param[in] const std::vector<std::uint8_t>& dataByteSize
*  @param[in] const std::uint64_t byteSize
*
*  @return    void
*****************************************************************************/
void PacketQueue::Enqueue(const std::vector<std::uint8_t>& data, const std::uint64_t byteSize)
{
	std::mutex mutex;
	
	/*-------------------------------------------------------------------
	-                 Exclusive Control
	---------------------------------------------------------------------*/
	{
		// lock
		std::lock_guard lock(mutex);

		// save packet storage infomation
		_offsetList.push(PacketInfo{ .Offset = _seekPosition, .ByteSize = byteSize });

		// append memory stream buffer
		_memoryStream.Seek(_seekPosition, SeekOrigin::Begin);
		_memoryStream.Write(data, 0, byteSize);
		
		// proceed seek position
		_seekPosition += byteSize;

		// scoped unlock
	}

}

/****************************************************************************
*                      Dequeue
*************************************************************************//**
*  @fn        std::vector<std::uint8_t> PacketQueue::Dequeue(const std::uint64_t byteSize)
*
*  @brief     Dequeue packet byte data from the memory stream. (Return)
*
*  @param[in, out] std::vector<std::uint8_t>& buffer
*  @param[in] const std::uint64_t byteSize
*
*  @return    DataSize (std::int32_t)
*****************************************************************************/
PacketQueue::DataSize PacketQueue::Dequeue(std::vector<std::uint8_t>& buffer, const std::uint64_t byteSize)
{
	/*-------------------------------------------------------------------
	-                 Check OffsetList Count
	---------------------------------------------------------------------*/
	if (_offsetList.empty()) { return -1; }

	std::int32_t receiveSize = 0;
	/*-------------------------------------------------------------------
	-                 Exclusive Control
	---------------------------------------------------------------------*/
	std::mutex mutex;
	{
		// lock
		std::lock_guard lock(mutex);

		// decide read byte size
		const auto desiredReadByteSize = std::min(byteSize, _offsetList.front().ByteSize);

		// Acquire packet data
		_memoryStream.Seek(_offsetList.front().Offset, SeekOrigin::Begin);
		receiveSize = _memoryStream.Read(buffer, 0, desiredReadByteSize);

		// Delete the first element now that the queue data has been extracted 
		if (receiveSize > 0) { _offsetList.pop(); }

		// if read done, Memory stream buffer clear
		if (_memoryStream.DoneRead()) 
		{
			_memoryStream.Clear(); 
			_seekPosition = 0; 
		}

		// scoped unlock
	}

	return receiveSize;
}
#pragma endregion Main Function