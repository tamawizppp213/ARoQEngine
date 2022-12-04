//////////////////////////////////////////////////////////////////////////////////
///             @file   ITransport.hpp
///             @brief  Transport interface class (TCP or UDP)
///             @author Toide Yutaro
///             @date   2022_12_04
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Network/Include/ITransport.hpp"
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

	int error = WSAStartup(MAKEWORD(2, 0), &wsaData);

	/*-------------------------------------------------------------------
	-                      Error check
	---------------------------------------------------------------------*/
	if (error != 0)
	{
		switch (error)
		{
			case WSASYSNOTREADY    : throw std::runtime_error("Network subSystems are not ready for connecting to the network.");
			case WSAVERNOTSUPPORTED: throw std::runtime_error("Requested Winsock version is not supported.");
			case WSAEINPROGRESS    : throw std::runtime_error("Progress blocking operation, or service provider is processing callback function.");
			case WSAEPROCLIM       : throw std::runtime_error("The maximum number of processes that winsock can handle has been reached.");
			case WSAEFAULT         : throw std::runtime_error("Unvalid WSAData pointer");
			default:
				throw std::runtime_error("Unknown error");
		}
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