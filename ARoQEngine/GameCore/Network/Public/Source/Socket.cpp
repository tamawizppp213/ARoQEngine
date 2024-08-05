//////////////////////////////////////////////////////////////////////////////////
///             @file   Socket.hpp
///             @brief  Socket
///             @author Toide Yutaro
///             @date   2022_12_04
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Network/Public/Include/Socket.hpp"
#include "GameCore/Network/Public/Include/IPAddress.hpp"
#include "GameCore/Network/Private/Include/NetworkErrorCode.hpp"
#include <stdexcept>
#include <string>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gc;
#pragma warning(disable : 4996)

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
Socket::Socket(const SocketType socketType, const ProtocolType protocolType)
	: _socketType(socketType) , _protocolType(protocolType)
{
	_socket = socket(AF_INET, static_cast<int>(socketType), static_cast<int>(protocolType));
	
	// error check
	if (_socket == INVALID_SOCKET)
	{
		throw std::runtime_error("Failed to prepare socket. errorNo: " + std::to_string(WSAGetLastError()));
	}
}

Socket::Socket(const SOCKET socket, const SocketType socketType, const ProtocolType protocolType)
	: _socket(socket), _socketType(socketType), _protocolType(protocolType)
{

}

#pragma region Main Function
/****************************************************************************
*                     Connect
****************************************************************************/
/* @fn        void Socket::Connect(const std::string& ipAddress, const std::uint32_t port)
*
*  @brief     Establish a connection to the specified server socket 
*
*  @param[in] const IPAddress&  ipAddress
*  @param[in] const std::uint16_t portNum
*
*  @return    void
*****************************************************************************/
bool Socket::Connect(const IPAddress& ipAddress, const std::uint16_t port)
{
	if (_socket == NULL) { return false; }

	/*-------------------------------------------------------------------
	-                  Set up server address (IPv4)
	---------------------------------------------------------------------*/
	SOCKADDR_IN serverAddress = {};
	serverAddress.sin_family           = AF_INET; // 後で変えるかも
	serverAddress.sin_port             = htons(port); // host byte order -> network byte order
	serverAddress.sin_addr.S_un.S_addr = static_cast<ULONG>(ipAddress.Get()); // string -> 32bit binary

	/*-------------------------------------------------------------------
	-                  Connection to server
	---------------------------------------------------------------------*/
	const int result = connect(_socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
	
	if (result == SOCKET_ERROR)
	{
		Close();
		NetworkException exception(result);
		exception.Log();
	}
	
	OutputDebugStringA("Connect");
	_connected = true;
	return _connected;
}

/****************************************************************************
*                     Accept
****************************************************************************/
/* @fn        Socket Socket::Accept()
*
*  @brief     Accepts connection requests from TCP clients. Return new Socket
*
*  @param[in] void
*
*  @return    Socket newSocket
*****************************************************************************/
Socket Socket::Accept()
{
	if (_socket == NULL) { throw std::runtime_error("Null socket"); }

	/*-------------------------------------------------------------------
	-                  client address (IPv4)
	---------------------------------------------------------------------*/
	SOCKADDR_IN clientAddress = {};
	
	/*-------------------------------------------------------------------
	-                  Connection to server
	---------------------------------------------------------------------*/
	int byteLength   = static_cast<int>(sizeof(clientAddress));
	SOCKET newSocket = accept(_socket, (struct sockaddr*)&clientAddress, &byteLength);

	/*-------------------------------------------------------------------
	-                  Show Debug Client Connect Log
	---------------------------------------------------------------------*/
#ifdef _DEBUG
	std::string addressInfo = inet_ntoa(clientAddress.sin_addr);
	std::string portInfo    = std::to_string(ntohs(clientAddress.sin_port));
	OutputDebugStringA("Accepted connection from ");
	OutputDebugStringA(addressInfo.c_str());
	OutputDebugStringA(", port = ");
	OutputDebugStringA(portInfo.c_str());
	OutputDebugStringA("\n");
#endif

	return Socket(newSocket, _socketType, _protocolType);
}

/****************************************************************************
*                     Bind
****************************************************************************/
/* @fn        void Socket::Bind(const std::string& ipAddress, const std::uint32_t port)
*
*  @brief     Associate a local address with a socket.
*
*  @param[in] const IPAddress& ipAddress
*  @param[in] const std::uint16_t port
*
*  @return    void
*****************************************************************************/
void Socket::Bind(const IPAddress& ipAddress, const std::uint16_t port)
{
	if (_socket == NULL) { return; }

	/*-------------------------------------------------------------------
	-                  Set up server address (IPv4)
	---------------------------------------------------------------------*/
	SOCKADDR_IN address = {};
	address.sin_family           = AF_INET;
	address.sin_port             = htons(port); // host byte order -> network byte order
	address.sin_addr.S_un.S_addr = static_cast<ULONG>(ipAddress.Get()); // string -> 32bit binary

	/*-------------------------------------------------------------------
	-                   Bind
	---------------------------------------------------------------------*/
	const int result = bind(_socket, (struct sockaddr*)&address, static_cast<int>(sizeof(address)));

	if (result == SOCKET_ERROR)
	{
		NetworkException exception(result);
		exception.Log();
	}
}

/****************************************************************************
*                     Listen
****************************************************************************/
/* @fn        void Socket::Listen(const std::int32_t backlog)
*
*  @brief     Ready to wait for connection requestes form TCP clients
*
*  @param[in] const std::int32_t backlog (Maximum length of queue of pending connections.)
*
*  @return    void
*****************************************************************************/
void Socket::Listen(const std::int32_t backlog)
{
	if (_socket == INVALID_SOCKET) { return; }

	/*-------------------------------------------------------------------
	-                Wait for connection
	---------------------------------------------------------------------*/
	const int result = listen(_socket, backlog);

	/*-------------------------------------------------------------------
	-                Show error log
	---------------------------------------------------------------------*/
	if (result == SOCKET_ERROR)
	{
		NetworkException exception(result);
		exception.Log();
	}
}

/****************************************************************************
*                     Close
****************************************************************************/
/* @fn        void Socket::Close()
*
*  @brief     Close existing sockets
* 
*  @param[in] void
* 
*  @return    void
*****************************************************************************/
void Socket::Close()
{
	/*-------------------------------------------------------------------
	-                  Close Socket 
	---------------------------------------------------------------------*/
	const int result = closesocket(_socket);
	_socket = INVALID_SOCKET;

	/*-------------------------------------------------------------------
	-                  Show Error Log 
	---------------------------------------------------------------------*/
	if (result == SOCKET_ERROR)
	{
		NetworkException exception(result);
		exception.Log();
	}
}

/****************************************************************************
*                     Poll
****************************************************************************/
/* @fn        void Socket::Poll(const std::int32_t waitMicroSeconds)
*
*  @brief     Close the status of socket. 
*
*  @param[in] const SelectMode selectMode
*  @param[in] const std::int32_t waitMicroSeconds. (this parameter sets INT_MAX, Choice Blocking mode) Max Blocking = 10s
*
*  @return    void
*****************************************************************************/
bool Socket::Poll(const SelectMode selectMode, const std::int32_t waitMicroSeconds)
{
	fd_set setSocket = {};

	/*-------------------------------------------------------------------
	-                 Initialize
	---------------------------------------------------------------------*/
	FD_ZERO(&setSocket);

	/*-------------------------------------------------------------------
	-                 Regist read socket
	---------------------------------------------------------------------*/
	FD_SET(_socket, &setSocket);

	/*-------------------------------------------------------------------
	-                 Set up time
	---------------------------------------------------------------------*/
	const std::int32_t maxTime = 10000;
	timeval waitTime = {};
	waitTime.tv_sec  = 0;
	waitTime.tv_usec = waitMicroSeconds <= maxTime ? waitMicroSeconds : maxTime;

	/*-------------------------------------------------------------------
	-                 Polling
	---------------------------------------------------------------------*/
	const int result = select(0,
		SelectMode::SelectRead  == selectMode ? &setSocket : NULL,
		SelectMode::SelectWrite == selectMode ? &setSocket : NULL,
		SelectMode::SelectError == selectMode ? &setSocket : NULL,
		&waitTime);

	if (result == 0)
	{
		OutputDebugStringA("Time out");
	}

	return result > 0;
}

/****************************************************************************
*                     Receive
****************************************************************************/
/* @fn        std::vector<std::uint8_t> Socket::Receive(const std::uint64_t byteSize, const SocketFlags socketFlags)
*
*  @brief     Receive data from the connected socket
*
*  @param[in] const std::uint64_t receiveSize
*  @param[in] const SocketFlags socketFlags
*
*  @return    std::vector<std::uint8_t> byteData
*****************************************************************************/
std::vector<std::uint8_t> Socket::Receive(const std::uint64_t byteSize, const SocketFlags socketFlags)
{
	std::vector<std::uint8_t> buffer(byteSize);

	/*-------------------------------------------------------------------
	-            Receive
	---------------------------------------------------------------------*/
	const int result = recv(_socket, (char*)buffer.data(), (int)byteSize, (int)socketFlags);

	if (result == -1)
	{
		NetworkException exception(result);
		exception.Log();
	}

	return buffer;
}

/****************************************************************************
*                     Receive
****************************************************************************/
/* @fn        void Socket::Receive(std::vector<std::uint8_t>& buffer, const std::uint64_t offset, const std::uint64_t count, const SocketFlags socketFlags = SocketFlags::None)
*
*  @brief     Receive data from the connected socket
*
*  @param[in,out] std::vector<std::uint8_t>& buffer
*  @param[in] const std::uint64_t receiveSize
*  @param[in] const SocketFlags socketFlags
*
*  @return    std::int32_t result (-1 : error, 0 : timeout, other : byte size)
*****************************************************************************/
std::int32_t Socket::Receive(std::vector<std::uint8_t>& buffer, const std::uint64_t offset, const std::uint64_t count, const SocketFlags socketFlags)
{
	/*-------------------------------------------------------------------
	-             Exceed buffer size check
	---------------------------------------------------------------------*/
	if (offset + count > buffer.size()) { return -1; }

	/*-------------------------------------------------------------------
	-            Receive
	---------------------------------------------------------------------*/
	const int result = recv(_socket, (char*)(&buffer[offset]), (int)count, (int)socketFlags);

	if (result == -1)
	{
		NetworkException exception(result);
		exception.Log();
	}

	return result;
}

/****************************************************************************
*                     Send
****************************************************************************/
/* @fn        void Socket::Send(const std::vector<std::uint8_t>& buffer, const std::uint64_t offset, const std::uint64_t size, const SocketFlags socketFlags)
*
*  @brief     Send data on the connected socket
*
*  @param[in] const std::vector<std::uint8_t>& buffer
*  @param[in] const std::uint64_t offset
*  @param[in] const std::uint64_t sendSize
*  @param[in] const SocketFlags socketFlags
*
*  @return    void
*****************************************************************************/
void Socket::Send(const std::vector<std::uint8_t>& buffer, const std::uint64_t offset, const std::uint64_t size, const SocketFlags socketFlags)
{
	/*-------------------------------------------------------------------
	-             Exceed buffer size check
	---------------------------------------------------------------------*/
	if (offset + size > buffer.size()) { return; }

	/*-------------------------------------------------------------------
	-             Send 
	---------------------------------------------------------------------*/
	const int result = send(_socket, (char*)(&buffer[offset]), (int)size, (int)socketFlags); // キャストが心配


	if (result == -1)
	{
		NetworkException exception(result);
		exception.Log();
	}
}

/****************************************************************************
*                     Shutdown
****************************************************************************/
/* @fn        void Socket::Shutdown()
*
*  @brief     Disconnects the currently established communication session
*
*  @param[in] const ShutdownType
*
*  @return    void
*****************************************************************************/
void Socket::Shutdown(const ShutdownType type)
{
	/*-------------------------------------------------------------------
	-                  Shutdown 
	---------------------------------------------------------------------*/
	const int result = shutdown(_socket, static_cast<int>(type));

	if (result == SOCKET_ERROR)
	{
		Close();

		NetworkException exception(result);
		exception.Log();
	}
}
#pragma endregion Main Function