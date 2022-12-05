//////////////////////////////////////////////////////////////////////////////////
///             @file   IPAddress.hpp
///             @brief  IPAddress
///             @author Toide Yutaro
///             @date   2022_12_05
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Network/Private/Include/IPAddress.hpp"
#include <WinSock2.h>
#include <stdexcept>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gc;
#pragma warning(disable : 4996)
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
IPAddress::IPAddress(const std::string& ipAddress)
{
	_ipAddress = inet_addr(ipAddress.c_str());
}

IPAddress::IPAddress(const std::uint64_t ipAddress)
{
	_ipAddress = ipAddress;
}
#pragma endregion Constructor and Destrucor

IPAddress IPAddress::GetUniqueIPAddress(const UniqueIPAddressType type)
{
	switch (type)
	{
		case UniqueIPAddressType::BroadCast  : return IPAddress(INADDR_BROADCAST);
		case UniqueIPAddressType::Any        : return IPAddress(INADDR_ANY);
		case UniqueIPAddressType::None       : return IPAddress(INADDR_NONE);
		case UniqueIPAddressType::LoopBack   : return IPAddress(INADDR_LOOPBACK);
		default:
			throw std::runtime_error("not support ip address type");
	}
}

