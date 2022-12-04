//////////////////////////////////////////////////////////////////////////////////
///             @file   Socket.hpp
///             @brief  Socket
///             @author Toide Yutaro
///             @date   2022_12_04
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Network/Public/Include/Socket.hpp"
#include "GameCore/Network/Private/Include/NetworkErrorCode.hpp"
#include <stdexcept>
#include <string>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gc;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
Socket::Socket(const SocketType socketType, const ProtocolType protocolType)
	: _socketType(socketType) , _protocolType(protocolType)
{
	_socket = socket(AF_INET, static_cast<int>(socketType), static_cast<int>(protocolType));
	
	// error check
	if (_socket == INVALID_SOCKET)
	{
		throw std::runtime_error("Failed to prepare socket. errorNo: " + std::to_string(WSAGetLastError()));
	}
}

#pragma region Main Function
/****************************************************************************
*                     Connect
*************************************************************************//**
*  @fn        void Socket::Connect(const std::string& ipAddress, const std::uint32_t port)
*
*  @brief     Establish a connection to the specified server socket 
*
*  @param[in] const std::string&  ipAddress
*  @param[in] const std::uint32_t portNum
*
*  @return 　　void
*****************************************************************************/
bool Socket::Connect(const std::string& ipAddress, const std::uint32_t port)
{
	if (_socket == NULL) { return false; }

	/*-------------------------------------------------------------------
	-                  Set up server address (IPv4)
	---------------------------------------------------------------------*/
	SOCKADDR_IN serverAddress = {};
	serverAddress.sin_family           = AF_INET;
	serverAddress.sin_port             = htons(port); // host byte order -> network byte order
	serverAddress.sin_addr.S_un.S_addr = inet_addr(ipAddress.c_str());

	/*-------------------------------------------------------------------
	-                  Connection to server
	---------------------------------------------------------------------*/
	const int result = connect(_socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

	if (result == SOCKET_ERROR)
	{
		Close();
	}

	_connected = true;
	return _connected;
}

/****************************************************************************
*                     Close
*************************************************************************//**
*  @fn        void Socket::Close()
*
*  @brief     Close existing sockets
* 
*  @param[in] void
* 
*  @return 　　void
*****************************************************************************/
void Socket::Close()
{
	/*-------------------------------------------------------------------
	-                  Close Socket 
	---------------------------------------------------------------------*/
	const int result = closesocket(_socket);
	_socket = INVALID_SOCKET;

	/*-------------------------------------------------------------------
	-                  Show Error Log 
	---------------------------------------------------------------------*/
	if (result == SOCKET_ERROR)
	{
		NetworkException exception(result);
		exception.Log();
	}
}
#pragma endregion Main Function