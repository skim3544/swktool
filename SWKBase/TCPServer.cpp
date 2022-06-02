#include "pch.h"
//#include "TCPServer.h"
//#include "ThreadPool.h"
//
//#pragma comment (lib, "Ws2_32.lib")
//
//
//#define DEFAULT_BUFLEN 4096
//
//
//
//namespace swktool {
//
//    /// <summary>
//    ///  Server connection is blocking
//    ///  This call allows the blocking server to run in background in separate thread
//    ///  only need to call once per server as you do not want multiple servers running in separate thread on the same port
//    /// </summary>
//    /// <returns></returns>
//    int TCPServer::RunInBackground() 
//    {   
//        if (ServerThreadRunning == false) {
//            ServerThread = std::thread(ServerThreadHandler, IPAddress, Port, MaxConnectionCount, pConnectFactory);
//            ServerThreadRunning = true;
//            return -1;
//        }
//
//        return 0;
//    }
//
//	/// <summary>
//	/// This is blocking call
//    /// the system goes into TCP/IP listening mode for the IP,Port and does not return
//	/// </summary>
//	/// <returns></returns>
//	int TCPServer::Run()
//	{
//        ServerThreadHandler(IPAddress, Port, MaxConnectionCount, pConnectFactory);
//        return 0;
//	}
//
//
//    /// <summary>
//    /// Static server thread handler
//    /// serverThreadHandler can run on main thread or separate thread
//    /// </summary>
//    /// <param name="IPAddress"></param>
//    /// <param name="Port"></param>
//    /// <param name="nMaxConnectionCount"></param>
//    /// <param name="pClientFactory"></param>
//    void TCPServer::ServerThreadHandler(std::string IPAddress, int Port, int nMaxConnectionCount, ComFactory* pClientFactory)
//    {        
//        // Microsoft TCP/IP server example code, modified the client connection
//        WSADATA wsaData;
//        int iResult;
//
//        SOCKET ListenSocket = INVALID_SOCKET;
//        SOCKET ClientSocket = INVALID_SOCKET;
//
//        struct addrinfo* result = NULL;
//        struct addrinfo hints;
//
//        //   int iSendResult;
//        //   char recvbuf[DEFAULT_BUFLEN];
//        int recvbuflen = DEFAULT_BUFLEN;
//
//        // Initialize Winsock
//        iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
//        if (iResult != 0) {
//            printf("WSAStartup failed with error: %d\n", iResult);
//            return;
//        }
//
//        ZeroMemory(&hints, sizeof(hints));
//        hints.ai_family = AF_INET;
//        hints.ai_socktype = SOCK_STREAM;
//        hints.ai_protocol = IPPROTO_TCP;
//        hints.ai_flags = AI_PASSIVE;
//
//        // Resolve the server address and port
//        std::string sPort = std::to_string(Port);
//        iResult = getaddrinfo(IPAddress.c_str(), sPort.c_str(), &hints, &result);
//        if (iResult != 0) {
//            printf("getaddrinfo failed with error: %d\n", iResult);
//            WSACleanup();
//            return;
//        }
//
//        // Create a SOCKET for connecting to server
//        ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
//        if (ListenSocket == INVALID_SOCKET) {
//            printf("socket failed with error: %ld\n", WSAGetLastError());
//            freeaddrinfo(result);
//            WSACleanup();
//            return;
//        }
//
//        // Setup the TCP listening socket
//        iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
//        if (iResult == SOCKET_ERROR) {
//            printf("bind failed with error: %d\n", WSAGetLastError());
//            freeaddrinfo(result);
//            closesocket(ListenSocket);
//            WSACleanup();
//            return;
//        }
//
//        freeaddrinfo(result);
//
//        iResult = listen(ListenSocket, SOMAXCONN);
//        if (iResult == SOCKET_ERROR) {
//            printf("listen failed with error: %d\n", WSAGetLastError());
//            closesocket(ListenSocket);
//            WSACleanup();
//            return;
//        }
//
//
//        // Create a thread pool that limits concurrent connections
//        swktool::ThreadPool ConnectionThreadPool(nMaxConnectionCount);;
//
//        // Receive until the peer shuts down the connection
//        while (true) {
//
//            // Accept a client socket
//            ClientSocket = accept(ListenSocket, NULL, NULL);
//            if (ClientSocket == INVALID_SOCKET) {
//                printf("accept failed with error: %d\n", WSAGetLastError());
//                closesocket(ListenSocket);
//                WSACleanup();
//                return;
//            }
//
//
//            // make a separate thread handle the connection request
//            std::thread NewConnectionThread = std::thread(ConnectionThreadHandler, ClientSocket, pClientFactory);
//            //std::thread NewConnectionThread = std::thread(ConnectionThread, ClientSocket);
//            ConnectionThreadPool.AddThread(std::move(NewConnectionThread));
//        };
//
//
//        // No longer need server socket
//        closesocket(ListenSocket);
//
//
//
//        // shutdown the connection since we're done
//        iResult = shutdown(ClientSocket, SD_SEND);
//        if (iResult == SOCKET_ERROR) {
//            printf("shutdown failed with error: %d\n", WSAGetLastError());
//            closesocket(ClientSocket);
//            WSACleanup();
//            return ;
//        }
//
//        // cleanup
//        closesocket(ClientSocket);
//        WSACleanup();
//
//    }
//
//
//    /// <summary>
//    /// Connection multi-thread handler
//    /// Calls factory object to get the connection handler,
//    /// then ask it to Process the session
//    /// </summary>
//    /// <param name="ConnectionSocket"></param>
//    /// <param name="pFactory"></param>
//    void TCPServer::ConnectionThreadHandler(SOCKET ConnectionSocket, ComFactory* pFactory)
//    {
//        // Create a client connection object that would handle this request
//        std::unique_ptr<ATCPCommunication> Connect = pFactory->Create(ConnectionSocket);
//
//        // Process the communication
//        if (Connect != nullptr) {            
//            Connect->ProcessConnectSession(ConnectionSocket);
//        }        
//    }
//}
//
