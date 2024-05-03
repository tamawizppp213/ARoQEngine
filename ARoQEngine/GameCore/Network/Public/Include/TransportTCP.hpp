//////////////////////////////////////////////////////////////////////////////////
///             @file   TransportTCP.hpp
///             @brief  Winsock2 TCP Module
///             @author Toide Yutaro
///             @date   2022_12_04
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef TRANSPORT_TCP_HPP
#define TRANSPORT_TCP_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "ITransport.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc
{

	/****************************************************************************
	*				  			 TransportTCP
	*************************************************************************//**
	*  @class     TransportTCP
	*  @brief     TCP Connection Class (Winsock (windows only))
	*****************************************************************************/
	class TransportTCP : public ITransport
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Transmission processing on the communication thread side.*/
		void SendPacket() override;

		/* @brief : Transmission processing on the communication thread side.*/
		void ReceivePacket() override;

		/* @brief : Transport Connection (return true: Connection Success, false: Connection Fail)*/
		bool Connect(const IPAddress& address, const std::uint32_t port) override;
		
		/* @brief : Transport Disconnection*/
		void Disconnect() override;

		/* @brief : Enqueue send packet queue*/
		void PackSendQueue(const std::vector<std::uint8_t>& data, const std::uint64_t size);

		/* @ brief : Dequeue receive packet queue*/
		std::int32_t UnpackReceiveQueue(std::vector<std::uint8_t>& buffer, const std::uint64_t size);
		/****************************************************************************
		**                Public Property
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		TransportTCP();

		~TransportTCP();

		TransportTCP(const SocketPtr& socket, const std::string& transportName);
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Property
		*****************************************************************************/
	};
}
#endif