#pragma once

#include "TCPFactory.h"
#include "TCPComm.h"

namespace swktool 
{
	typedef TCPFactory<ATCPCommunication> ComFactory;

	class TCPServer {
		// IP address or Localhost
		std::string IPAddress;
		// Network communication port
		int Port;

		// Factory that is going to create a connection handler for us
		TCPFactory<ATCPCommunication>* pConnectFactory;

		// maximum connection system should allow at given time
		// we need to limite because each connection runs in separate thread
		int MaxConnectionCount;

		// Server Threading
		std::thread ServerThread;

		// Is server running?
		bool ServerThreadRunning = false;

	public:
		TCPServer(std::string sIPAddress, int iPort, ComFactory* pFactory, int nMaxThreadCount = 10) :
			IPAddress(sIPAddress), Port(iPort), pConnectFactory(pFactory), MaxConnectionCount(nMaxThreadCount)
		{
		}

		int Run();
		int RunInBackground();

	protected:
		static void ConnectionThreadHandler(SOCKET ConnectionSocket, ComFactory* pFactory);
		static void ServerThreadHandler(std::string IPAddress, int Port, int nMaxConnectionCount, TCPFactory<ATCPCommunication>* pFactory);
	};
} //  namespace swktool 