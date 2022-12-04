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
#include "GameCore/Network/Include/TransportTCP.hpp"
#include <stdexcept>
#include <string>
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

	/*-------------------------------------------------------------------
	-                      Make socket
	---------------------------------------------------------------------*/
	_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_socket == INVALID_SOCKET)
	{
		throw std::runtime_error("Failed to prepare socket. errorNo: " + std::to_string(WSAGetLastError()));
	}
}

TransportTCP::TransportTCP(const SOCKET socket, const std::string& transportName)
	:ITransport(), _socket(socket), _transportName(transportName)
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
/* @brief : Transport Connection (return true: Connection Success, false: Connection Fail)*/
bool TransportTCP::Connect(const std::string& address, const std::uint32_t port)
{
	return true;
}
/* @brief : Transport Disconnection*/
void TransportTCP::Disconnect()
{

}
#pragma endregion Public Function