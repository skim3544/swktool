#pragma once
#include <string>

#include "TCPServer.h"

namespace swktool 
{
	class TCPClient {
		// IP address or Localhost
		std::string IPAddress;
		// Network communication port
		int Port;

		// Factory that is going to create a connection handler for us
		ComFactory* pConnectFactory;

		// maximum connection system should allow at given time
		// we need to limite because each connection runs in separate thread
		int MaxConnectionCount;

		std::thread ServerThread;
		bool ServerThreadRunning = false;

	public:
		TCPClient(std::string sIPAddress, int iPort, TCPFactory<ATCPCommunication>* pFactory, int nMaxThreadCount = 10) :
			IPAddress(sIPAddress), Port(iPort), pConnectFactory(pFactory), MaxConnectionCount(nMaxThreadCount)
		{
		}

		int Run();
		int RunInBackground();

	protected:
		static void ClientThreadHandler(std::string IPAddress, int Port, int nMaxConnectionCount, ComFactory* pFactory);
		static void ConnectionThreadHandler(SOCKET ConnectionSocket, ComFactory* pFactory);
	};
}