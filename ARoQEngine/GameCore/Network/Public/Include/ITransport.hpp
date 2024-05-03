//////////////////////////////////////////////////////////////////////////////////
///             @file   TransportTCP.hpp
///             @brief  Winsock2 TCP Module
///             @author Toide Yutaro
///             @date   2022_12_04
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef ITRANSPORT_HPP
#define ITRANSPORT_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "IPAddress.hpp"
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include <WinSock2.h>
#include <vector>
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc
{
	class PacketQueue;
	class Socket;
	/****************************************************************************
	*				  			 TransportTCP
	*************************************************************************//**
	*  @class     TransportTCP
	*  @brief     TCP Connection Class (Winsock (windows only))
	*****************************************************************************/
	class ITransport: public gu::NonCopyable
	{
	protected :
		using PacketQueuePtr = gu::SharedPointer<PacketQueue>;
		using SocketPtr      = gu::SharedPointer<Socket>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Transmission processing on the communication thread side.*/
		virtual void SendPacket() = 0;
		
		/* @brief : Transmission processing on the communication thread side.*/
		virtual void ReceivePacket() = 0;

		/* @brief : Transport Connection (return true: Connection Success, false: Connection Fail)*/
		virtual bool Connect(const IPAddress& address, const std::uint32_t port) = 0;
		
		/* @brief : Transport Disconnection*/
		virtual void Disconnect() = 0;
		/****************************************************************************
		**                Public Property
		*****************************************************************************/
		bool IsConnected() const { return _isConnected; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		ITransport();

		ITransport(const SocketPtr& socket, const std::string& transportName) : _socket(socket), _transportName(transportName) {};
		
		virtual ~ITransport();
		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
		/* @brief: Connection Flags*/
		bool _isConnected = false;

		// @brief :Packet queue
		PacketQueuePtr _sendQueue    = nullptr;  // Send
		PacketQueuePtr _receiveQueue = nullptr;  // Receive

		/* @brief: Communication socket*/
		SocketPtr _socket = nullptr;

		/* @brief : Transport name*/
		std::string _transportName = "";

		/* @brief : Maximum size of packets to send and receive.
		            This size is decided by MTU configuration. (MTU: Maximum data size that can be sent at a time )
					Ethernet maximum MTS is 1500 bytes.*/
		static constexpr std::uint32_t MaxPacketSize = 1400; 
	};
}
#endif