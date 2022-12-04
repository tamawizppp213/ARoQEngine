//////////////////////////////////////////////////////////////////////////////////
///             @file   ITransport.hpp
///             @brief  Transport interface class (TCP or UDP)
///             @author Toide Yutaro
///             @date   2022_12_04
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Network/Public/Include/ITransport.hpp"
#include "GameCore/Network/Private/Include/NetworkErrorCode.hpp"
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
ITransport::ITransport()
{
	/*-------------------------------------------------------------------
	-                      Initialize
	---------------------------------------------------------------------*/
	WSADATA wsaData = {};

	int error = WSAStartup(MAKEWORD(2, 2), &wsaData);

	/*-------------------------------------------------------------------
	-                      Error check
	---------------------------------------------------------------------*/
	if (error != 0)
	{
		NetworkException exception(error);
		exception.Log();
	}
}

ITransport::~ITransport()
{
	/*-------------------------------------------------------------------
	-                      Clean up wsa
	---------------------------------------------------------------------*/
	WSACleanup();
}
#pragma endregion Constructor and Destrctor