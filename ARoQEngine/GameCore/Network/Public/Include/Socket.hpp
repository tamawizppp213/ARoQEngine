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
#include <vector>
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
	****************************************************************************/
	/* @class     Socket
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
		bool Poll(const SelectMode selectMode, const std::int32_t waitMicroSeconds = 10000); // 10000 : MaxWaitTime 

		/* @brief Receive data on the connected socket*/
		std::vector<std::uint8_t> Receive(const std::uint64_t byteSize, const SocketFlags socketFlags = SocketFlags::None);
		
		std::int32_t Receive(std::vector<std::uint8_t>& buffer, const std::uint64_t offset, const std::uint64_t size, const SocketFlags socketFlags = SocketFlags::None);

		/* @brief : Send data on the connected socket*/
		void Send(const std::vector<std::uint8_t>& buffer, const std::uint64_t offset, const std::uint64_t size, const SocketFlags socketFlags = SocketFlags::None);
		
		/* @brief : Shutdown socket*/
		void Shutdown(const ShutdownType type);
		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		/* @brief : Done connect to server socket. */
		bool         Connected      () const { return _connected; }

		/* @brief : Get winsock2 socket*/
		SOCKET       GetSocket      () const { return _socket; }

		/* @brief : Socket type */
		SocketType   GetSocketType  () const { return _socketType; }

		/* @brief Protocol type*/
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
		**                Private Property
		*****************************************************************************/
		SOCKET       _socket       = INVALID_SOCKET;
		SocketType   _socketType   = SocketType::Unknown;
		ProtocolType _protocolType = ProtocolType::Unknown;
		bool         _connected    = false;
	};
}

#endif