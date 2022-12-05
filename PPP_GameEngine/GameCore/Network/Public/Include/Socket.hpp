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
#include "../../Private/Include/NetworkDefine.hpp"
#include <WinSock2.h>
#include <string>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
	

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc
{
	class IPAddress;
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
		/* @brief : Accepts connection requests from TCP clients. Return new Socket*/
		Socket Accept();

		/* @brief : Associate a local address with a socket.*/
		void Bind(const IPAddress& ipAddress, const std::uint16_t port);
		
		/* @brief : Close socket*/
		void Close();

		// @brief : Connect to server. Use client socket
		bool Connect(const IPAddress& ipAddress, const std::uint16_t port);

		/* @brief : Ready to wait for connection requestes form TCP clients */
		void Listen(const std::int32_t backlog);

		/* @brief : Close the status of socket. */
		void Poll(const std::int32_t waitMicroSeconds = 10000); // 10000 : MaxWaitTime 

		/* @brief : Shutdown socket*/
		void Shutdown(const ShutdownType type);
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

		Socket(SOCKET socket, const SocketType socketType, const ProtocolType protocolType);
		
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