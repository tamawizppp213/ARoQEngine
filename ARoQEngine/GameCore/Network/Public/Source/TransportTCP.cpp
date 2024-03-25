//////////////////////////////////////////////////////////////////////////////////
///             @file   TransportTCP.hpp
///             @brief  Winsock2 TCP Module
///                     When the partner connects by calling the Connect() function, each other is ready to send and receive data
///             @author Toide Yutaro
///             @date   2022_12_04
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Network/Public/Include/TransportTCP.hpp"
#include "GameCore/Network/Public/Include/Socket.hpp"
#include "GameCore/Network/Private/Include/PacketQueue.hpp"
#include "GameCore/Network/Private/Include/NetworkDefine.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#pragma comment(lib, "ws2_32.lib")
using namespace gc;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
TransportTCP::TransportTCP() : ITransport()
{

	
}

TransportTCP::TransportTCP(const SocketPtr& socket, const std::string& transportName)
	:ITransport(socket, transportName)
{

}
TransportTCP::~TransportTCP()
{
	/*-------------------------------------------------------------------
	-                      Clean up wsa 
	---------------------------------------------------------------------*/
	WSACleanup();
}
#pragma endregion Constructor and Destructor

#pragma region Public Function 
/****************************************************************************
*                      SendPacket
*************************************************************************//**
*  @fn        void TransportTCP::SendPacket()
*
*  @brief     Send packet
*
*  @param[in] void
*
*  @return    void
*****************************************************************************/
void TransportTCP::SendPacket()
{
	if (!_socket) { return; }

	/*-------------------------------------------------------------------
	-                      Polling (Non wait time)
	---------------------------------------------------------------------*/
	_socket->Poll(SelectWrite, 0);

	/*-------------------------------------------------------------------
	-              Push packet byte data from packet queue
	---------------------------------------------------------------------*/
	// Prepare max packet size buffer
	std::vector<std::uint8_t> buffer(MaxPacketSize);

	// send packet data
	int32_t sendSize = _sendQueue->Dequeue(buffer, buffer.size()); // Buffer size and sendSize may differ
	int32_t sum      = sendSize;

	while (sendSize > 0)
	{
		// update next sendqueue's offset and size
		sendSize = _sendQueue->Dequeue(buffer, buffer.size());
		sum     += sendSize > 0 ? sendSize : 0;
	}

	/*-------------------------------------------------------------------
	-                      Send packet data
	---------------------------------------------------------------------*/
	_socket->Send(buffer, 0, sum, SocketFlags::None);
}

/****************************************************************************
*                      ReceivePacket
*************************************************************************//**
*  @fn        void TransportTCP::ReceivePacket()
*
*  @brief     Transmission processing on the communication thread side.
*
*  @param[in] void
*
*  @return    void
*****************************************************************************/
void TransportTCP::ReceivePacket()
{
	if (!_socket) { return; }

	/*-------------------------------------------------------------------
	-                      Polling (Non wait time)
	---------------------------------------------------------------------*/
	while (_socket->Poll(SelectRead, 0))
	{
		std::vector<std::uint8_t> buffer(MaxPacketSize);

		const auto receiveSize = _socket->Receive(buffer, 0, buffer.size(), SocketFlags::None);
		
		// If result == 0, Disconnection from a communication partner
		if (receiveSize == 0)
		{
			OutputDebugStringA("[TCP] Disconnect receive from other");
			Disconnect();
			break;
		}
		if (receiveSize > 0)
		{
			_receiveQueue->Enqueue(buffer, receiveSize);
			break;
		}
	}
}

/****************************************************************************
*                      Connect
*************************************************************************//**
*  @fn        bool TransportTCP::Connect(const IPAddress& address, const std::uint32_t port)
*
*  @brief     Transport Connection (return true: Connection Success, false: Connection Fail)
*
*  @param[in] const IPAddress& address
*  @param[in] const std::uint32_t port
*
*  @return    bool result : (true) connection succuess (false) connection failed
*****************************************************************************/
bool TransportTCP::Connect(const IPAddress& address, const std::uint32_t port)
{
	/*-------------------------------------------------------------------
	-                 Enable to use socket check
	---------------------------------------------------------------------*/
	if (!_socket) { OutputDebugStringA("Connection failed");  return false; }

	/*-------------------------------------------------------------------
	-                 Create new socket 
	---------------------------------------------------------------------*/
	_socket = gu::MakeShared<Socket>(SocketType::Stream, ProtocolType::TCP);
	
	/*-------------------------------------------------------------------
	-                 Connect
	---------------------------------------------------------------------*/
	_socket->Connect(address, (std::uint16_t)port);
	_isConnected = true;
	OutputDebugStringA("Connection Success");

	// Todo : 接続結果をイベント通知できるようにする.

	return _isConnected;
}
/****************************************************************************
*                      Disonnect
*************************************************************************//**
*  @fn        void TransportTCP::Disconnect()
*
*  @brief     Transport Disconnection
*
*  @param[in] void
*
*  @return    void
*****************************************************************************/
void TransportTCP::Disconnect()
{
	_isConnected = false;

	/*-------------------------------------------------------------------
	-                 Disconnect
	---------------------------------------------------------------------*/
	if (!_socket) { return; }

	// close socket
	_socket->Shutdown(ShutdownType::Both);
	_socket->Close();
	_socket.Reset();

	// Todo : 切断結果を通知します.
}

void TransportTCP::PackSendQueue(const std::vector<std::uint8_t>& data, const std::uint64_t size)
{
	_sendQueue->Enqueue(data, size);
}

std::int32_t TransportTCP::UnpackReceiveQueue(std::vector<std::uint8_t>& buffer, const std::uint64_t size)
{
	return _receiveQueue->Dequeue(buffer, size);
}
#pragma endregion Public Function