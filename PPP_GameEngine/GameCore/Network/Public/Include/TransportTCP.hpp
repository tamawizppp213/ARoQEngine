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
		/****************************************************************************
		**                Public Member Variables
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
		**                Protected Member Variables
		*****************************************************************************/
	};
}
#endif