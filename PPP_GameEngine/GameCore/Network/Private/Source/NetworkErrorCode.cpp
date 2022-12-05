//////////////////////////////////////////////////////////////////////////////////
///             @file   Network error code.hpp
///             @brief  Network error code
///             @author Toide Yutaro
///             @date   2022_12_04
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
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
NetworkException::NetworkException(const std::int32_t errorCode)
	: _errorCode(errorCode)
{
	if (_errorCode == INVALID_ID)
	{
		// ì¡íËÇÃÉGÉâÅ[î‘çÜÇéÊìæÇ∑ÇÈ
		_errorCode = WSAGetLastError();
	}
}
void NetworkException::Log()
{
	if (_errorCode == 0) { return; }

	switch (_errorCode)
	{
		case WSAEINTR          : throw std::runtime_error("(Blocking) Windows Socket 1.1 calls have been canceled via WSACancelBlockingCall.");
		case WSAEFAULT         : throw std::runtime_error("Unvalid WSAData pointer");
		case WSAEINVAL         : throw std::runtime_error("Invalid argument");
		case WSAEMFILE         : throw std::runtime_error("There are no more socket descriptors available");
		case WSAEINPROGRESS    : throw std::runtime_error("Progress blocking operation, or service provider is processing callback function.");
		case WSAENOTSOCK       : throw std::runtime_error("Descriptor is not socket.");
		case WSAEOPNOTSUPP     : throw std::runtime_error("Not support listen");
		case WSAEADDRINUSE     : throw std::runtime_error("The local address of the socket is already in use and the socket is not marked for address reuse with SO_REUSEADDR");
		case WSAEADDRNOTAVAIL  : throw std::runtime_error("The address specified in the name parameter is not a valid IP address for this computer.");
		case WSAENETDOWN       : throw std::runtime_error("Network subsystem failed");
		case WSAECONNABORTED   : throw std::runtime_error("The virtual communication channel has terminated due to a timeout or other error. The application should close the socket since it can no longer be used");
		case WSAECONNRESET     : throw std::runtime_error("The virtual communication channel has been reset because the remote side has performed a HARD CLOSE or ABORTIVE CLOSE. The application should close the socket since it can no longer be used.");
		case WSAENOBUFS        : throw std::runtime_error("There are no buffer region");
		case WSAEISCONN        : throw std::runtime_error("Already connect socket");
		case WSAEPROCLIM       : throw std::runtime_error("The maximum number of processes that winsock can handle has been reached.");
		case WSASYSNOTREADY    : throw std::runtime_error("Network subSystems are not ready for connecting to the network.");
		case WSAVERNOTSUPPORTED: throw std::runtime_error("Requested Winsock version is not supported.");
		case WSANOTINITIALISED : throw std::runtime_error("Before this function can be used, the WSAStartup must be called");
		default:
			throw std::runtime_error("Unknown error");
	}
}