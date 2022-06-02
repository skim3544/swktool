#include "pch.h"
//#include <thread>
//#include "TCPClient.h"
//#include "ThreadPool.h"
//
//using namespace swktool;
//
///// <summary>
/////  Server connection is blocking
/////  This call allows the blocking server to run in background in separate thread
/////  only need to call once per server as you do not want multiple servers running in separate thread on the same port
///// </summary>
///// <returns></returns>
//int TCPClient::RunInBackground()
//{
//	if (ServerThreadRunning == false) {
//		ServerThread = std::thread(ClientThreadHandler, IPAddress, Port, MaxConnectionCount, pConnectFactory);
//		ServerThreadRunning = true;
//		return -1;
//	}
//
//	return 0;
//}
//
///// <summary>
///// This is blocking call
///// the system goes into TCP/IP listening mode for the IP,Port and does not return
///// </summary>
///// <returns></returns>
//int TCPClient::Run()
//{
//	ClientThreadHandler(IPAddress, Port, MaxConnectionCount, pConnectFactory);
//	return 0;
//}
//
//
//void TCPClient::ClientThreadHandler(std::string IPAddress, int Port, int nMaxConnectionCount, ComFactory* pFactory)
//{
//	WSADATA wsaData;
//
//	int iResult;
//	struct addrinfo* result = NULL,
//		* ptr = NULL,
//		hints;
//
//
//	// Initialize Winsock
//	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
//	if (iResult != 0) {
//		printf("WSAStartup failed with error: %d\n", iResult);
//		return;
//	}
//
//	ZeroMemory(&hints, sizeof(hints));
//	hints.ai_family = AF_UNSPEC;
//	hints.ai_socktype = SOCK_STREAM;
//	hints.ai_protocol = IPPROTO_TCP;
//
//
//	// Resolve the server address and port
//	std::string sPort = std::to_string(Port);
//	iResult = getaddrinfo(IPAddress.c_str(), sPort.c_str(), &hints, &result);
//	if (iResult != 0) {
//		printf("getaddrinfo failed with error: %d\n", iResult);
//		WSACleanup();
//		return;
//	}
//
//	SOCKET ConnectSocket = INVALID_SOCKET;
//	// Create a thread pool that limits concurrent connections
//	//swktool::ThreadPool ConnectionThreadPool(nMaxConnectionCount);;
//
//
//
//
//	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
//		// Create a SOCKET for connecting to server
//		ConnectSocket = socket(hints.ai_family, hints.ai_socktype,
//			hints.ai_protocol);
//		if (ConnectSocket == INVALID_SOCKET) {
//			printf("socket failed with error: %ld\n", WSAGetLastError());
//			WSACleanup();
//			return;
//		}
//
//		// Connect to server.
//		//iResult = connect(ConnectSocket, hints.ai_addr, (int)hints.ai_addrlen);
//		//iResult = connect(ConnectSocket, "127.0.0.l", (int)hints.ai_addrlen);
//		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
//		if (iResult == SOCKET_ERROR) {
//			closesocket(ConnectSocket);
//			ConnectSocket = INVALID_SOCKET;
//		}
//	}
//		
//	ConnectionThreadHandler(ConnectSocket, pFactory);
//	// make a separate thread handle the connection request
//	//std::thread ConnectionThread = std::thread(ConnectionThreadHandler, ConnectSocket, pFactory);
//	//std::thread NewConnectionThread = std::thread(ConnectionThread, ClientSocket);
//	//ConnectionThreadPool.AddThread(std::move(NewConnectionThread));
//
//	
//
//	freeaddrinfo(result);
//	/*
//	iResult = send(ConnectSocket, SendStr.c_str(), SendStr.length(), 0);
//	if (iResult == SOCKET_ERROR) {
//		;
//		DebugText("send failed with error:", std::to_string(WSAGetLastError()));
//		closesocket(ConnectSocket);
//		WSACleanup();
//		return "";;
//	}
//
//	std::string output;
//	char buffer[20];
//	buffer[0] = 0;
//
//
//	if (ReceiveRespose)
//	{
//		ZeroMemory(&buffer, sizeof(buffer));
//
//		// Receive until the peer closes the connection
//		do {
//			iResult = recv(ConnectSocket, buffer, sizeof(buffer), 0);
//			if (iResult > 0)
//			{
//				DebugText("Bytes received:", std::to_string(iResult));
//				break;
//			}
//		} while (true);
//
//		DebugText("Receiving: ", buffer);
//	}
//
//	output = buffer;
//	*/
//	closesocket(ConnectSocket);
//	WSACleanup();
//
//
//	return;
//}
//
//
///// <summary>
///// Connection multi-thread handler
///// Calls factory object to get the connection handler,
///// then ask it to Process the session
///// </summary>
///// <param name="ConnectionSocket"></param>
///// <param name="pFactory"></param>
//void TCPClient::ConnectionThreadHandler(SOCKET ConnectionSocket, ComFactory* pFactory)
//{
//	// Create a client connection object that would handle this request
//	std::unique_ptr<ATCPCommunication> Connect = pFactory->Create(ConnectionSocket);
//
//	// Process the communication
//	if (Connect != nullptr) {
//		Connect->ProcessConnectSession(ConnectionSocket);
//	}
//}
