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
		case WSAEINPROGRESS    : throw std::runtime_error("Progress blocking operation, or service provider is processing callback function.");
		case WSAENOTSOCK       : throw std::runtime_error("Descriptor is not socket.");
		case WSAENETDOWN       : throw std::runtime_error("Network subsystem failed");
		case WSAEPROCLIM       : throw std::runtime_error("The maximum number of processes that winsock can handle has been reached.");
		case WSASYSNOTREADY    : throw std::runtime_error("Network subSystems are not ready for connecting to the network.");
		case WSAVERNOTSUPPORTED: throw std::runtime_error("Requested Winsock version is not supported.");
		case WSANOTINITIALISED : throw std::runtime_error("Before this function can be used, the WSAStartup must be called");
		default:
			throw std::runtime_error("Unknown error");
	}
}