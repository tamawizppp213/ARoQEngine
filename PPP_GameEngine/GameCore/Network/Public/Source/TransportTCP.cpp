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
	/*-------------------------------------------------------------------
	-                 Enable to use socket check
	---------------------------------------------------------------------*/
	if (_socket != NULL) { return false; }

	
	return true;
}
/* @brief : Transport Disconnection*/
void TransportTCP::Disconnect()
{
	_isConnected = false;


}
#pragma endregion Public Function