//////////////////////////////////////////////////////////////////////////////////
///             @file   TransportTCP.hpp
///             @brief  Winsock2 TCP Module
///             @author Toide Yutaro
///             @date   2022_12_04
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef NETWORK_DEFINE_HPP
#define NETWORK_DEFINE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc
{
	enum class SocketType
	{
		Unknown         = -1,
		Stream          = 1, // Supports reliable, bidirectional, connection-based byte streams without data replication and boundary maintenance.
		Datagram        = 2, // It supports datagrams. This is a connectionless, fixed (usually short) maximum length, unreliable message
		Raw             = 3,
		RDM             = 4,
		SequencedPacket = 5
	};

	enum class ProtocolType
	{
        Unknown = -1,
		IPv4    = 4,
        TCP     = 6,
		UDP     = 17,
		IDP     = 22,
		IPv6    = 41
	};

	enum class ShutdownType
	{
		Receive = 0,
		Send    = 1,
		Both    = 2,
	};

	enum class Endian
	{
		BigEndian    = 0,
		LittleEndian = 1
	};
}

#endif