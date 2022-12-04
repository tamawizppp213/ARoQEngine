//////////////////////////////////////////////////////////////////////////////////
///             @file   Socket.hpp
///             @brief  Socket
///             @author Toide Yutaro
///             @date   2022_12_04
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SOCKET_HPP
#define SOCKET_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include <WinSock2.h>
#include <string>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
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

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
	/****************************************************************************
	*				  			    Socket
	*************************************************************************//**
	*  @class     Socket
	*  @brief     socket
	*****************************************************************************/
	class Socket
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		// @brief : Connect to server. Use client socket
		bool Connect(const std::string& ipAddress, const std::uint32_t port);

		/* @brief : Close socket*/
		void Close();
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		bool         Connected      () const { return _connected; }
		SOCKET       GetSocket      () const { return _socket; }
		SocketType   GetSocketType  () const { return _socketType; }
		ProtocolType GetProtocolType() const { return _protocolType; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Socket() = default;
		~Socket() = default;
		Socket(const SocketType socketType, const ProtocolType protocolType);
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		SOCKET       _socket       = INVALID_SOCKET;
		SocketType   _socketType   = SocketType::Unknown;
		ProtocolType _protocolType = ProtocolType::Unknown;
		bool         _connected    = false;
	};
}

#endif