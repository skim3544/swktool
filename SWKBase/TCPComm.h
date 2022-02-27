#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <thread>

#include <memory>
#include <string>
#include <iostream>
#include <ostream>

#include "TBuffer.h"

namespace swktool
{ 
	/// <summary>
	/// TCP Communication object
	/// This object handles both Server communication (Accept, receive, send response)
	/// and client communication (Connect, send request, receive request)
	/// by overriding ProcessRequest function and implement the communication in specific order
	/// </summary>
	class ATCPCommunication {
	protected:
		int Send(SOCKET ConnectSocket, char buffer[], std::size_t Size);
		int Send(SOCKET ConnectSocket, std::string s);		
		int Send(SOCKET ConnectSocket, TBuffer& Buf);
		int Send(SOCKET ConnectSocket, std::fstream& of, std::size_t TotalToSend);

		int Recv(SOCKET ConnectSocket, char buffer[], std::size_t Size);
		int Recv(SOCKET ConnectSocket, TBuffer& Buf, std::size_t Size);		
		int Recv(SOCKET ConnectSocket, std::fstream& of, std::size_t TotalToReceive);

		int RecvChar(SOCKET ConnectSocket, char& C)
		{						
			char buf[1] = { 0 };
			//buf[0] = 0;
			int ReadSize = Recv(ConnectSocket, &buf[0], 1);
			if (ReadSize == 1) {
				C = buf[0];
			}
			return ReadSize;
		}

		int SendChar(SOCKET ConnectSocket, char& C)
		{
			char buf[1] = { };
			buf[0] = C;
			int SendSize = Send(ConnectSocket, &buf[0], 1);

			return SendSize;
		}



	public:
		virtual int ProcessConnectSession(SOCKET ConnectSocket);
		virtual int ProcessRequest(SOCKET ConnectSocket);
	};


}