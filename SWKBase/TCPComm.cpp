#include "pch.h"
#include "TCPComm.h"
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace swktool;

int ATCPCommunication::Send(SOCKET ConnectSocket, TBuffer& Buf) {
	int TotalSent = 0;
	auto TotalToSend = Buf.Size();

	// loop around until everything we have gets ent
	while (TotalSent < TotalToSend)
	{
		const char* p = (const char*)(&Buf[TotalSent]);
		auto loadSize = (int)Buf.Size() - TotalSent;
		auto SizeSent = send(ConnectSocket, (const char*)p, loadSize, 0);
		TotalSent = TotalSent + SizeSent;
	}

	return TotalSent;
}



int ATCPCommunication::Send(SOCKET ConnectSocket, char buffer[], std::size_t TotalToSend)
{
	// Debug	
//const int BufferSize = 1;
	const int BufferSize = 4096;

	size_t TotalSent = 0;
	size_t Remaining = TotalToSend;
	
	char SendBuffer[BufferSize];
	memset(SendBuffer, 0, sizeof(SendBuffer));

	while (TotalSent < TotalToSend) {
		int LoadSize = std::min<int>((int)Remaining, sizeof(SendBuffer));

		// copy to the send buffer
		memcpy(&SendBuffer[0], &buffer[TotalSent], LoadSize);

		// send
		int SizeSent = send(ConnectSocket, &SendBuffer[0], LoadSize, 0);

		if (SizeSent > 0) {
			TotalSent = TotalSent + SizeSent;

			Remaining = Remaining - SizeSent;
		}
		else {
			break;
		}
	}

	return (int)TotalSent;

}


int ATCPCommunication::Send(SOCKET ConnectSocket, std::fstream& of, std::size_t TotalToSend) {
	// Debug	
//const int BufferSize = 1;
	const int BufferSize = 4096;

	size_t TotalSent = 0;
	size_t Remaining = TotalToSend;

	char buffer[BufferSize];
	memset(buffer, 0, sizeof(buffer));

	while (TotalSent < TotalToSend) {
		int LoadSize = std::min<int>((int)TotalToSend, sizeof(buffer));
		of.read(&buffer[0], LoadSize);

		// if good read
		if (of) {
			// send
			Send(ConnectSocket, buffer, LoadSize);
			TotalSent = TotalSent + LoadSize;
			Remaining = Remaining - LoadSize;
		}
		else {
			// IO Error
			break;
		}
	}

	return (int)TotalSent;
}


int ATCPCommunication::Send(SOCKET ConnectSocket, std::string s) {
	size_t TotalSent = 0;
	while (TotalSent < s.length()) {
		auto LoadSize = s.length() - TotalSent;

		// get the raw memory pointer, convert to vector
		const unsigned char* raw_memory =
			reinterpret_cast<const unsigned char*>(&s[TotalSent]);

		auto SizeSent = send(ConnectSocket, (char*)raw_memory, (int)LoadSize, 0);
		TotalSent = TotalSent + SizeSent;
	}
	return (int)TotalSent;
}



int ATCPCommunication::Recv(SOCKET ConnectSocket, TBuffer& Buf, std::size_t TotalToReceive) {
	// Debug	
	//const int BufferSize = 1;
	const int BufferSize = 4096;

	size_t TotalReceived = 0;
	size_t Remaning = TotalToReceive;

	// use internal 4K buffer
	char Buffer[BufferSize];
	memset(Buffer, 0, sizeof(Buffer));

	while (TotalReceived < TotalToReceive) {
		// load size is either full size of the buffer or bytes remaining
		int  LoadSize = min((int)Remaning, sizeof(Buffer));
		int SizeReceived = recv(ConnectSocket, &Buffer[0], LoadSize, 0);
		if (SizeReceived > 0) {
			TotalReceived = TotalReceived + SizeReceived;
			Remaning = Remaning - SizeReceived;

			Buf.PutArray(&Buffer[0], SizeReceived);
		}
		else {
			// receive failure
			break;
		}
	}

	return (int)TotalReceived;
}


int ATCPCommunication::Recv(SOCKET ConnectSocket, char Buf[], std::size_t TotalToReceive)
{
	//const int BufferSize = 1;
	const int BufferSize = 4096;

	// zero out buffer
	memset(&Buf[0], 0, TotalToReceive);

	// nothing receied to start
	size_t TotalReceived = 0;

	// remining bytes to receove
	size_t Remaning = TotalToReceive;

	// use internal 4K buffer
	char Buffer[BufferSize];
	memset(Buffer, 0, sizeof(Buffer));

	while (TotalReceived < TotalToReceive) {

		// load size is either full size of the buffer or bytes remaining
		int LoadSize = min((int)Remaning, sizeof(Buffer));

		// receive the data
		int SizeReceived = recv(ConnectSocket, &Buffer[0], LoadSize, 0);
		if (SizeReceived > 0) {
			memcpy(&Buf[TotalReceived], &Buffer[0], SizeReceived);

			TotalReceived = TotalReceived + SizeReceived;
			Remaning = TotalToReceive - SizeReceived;
		}
		else {
			// receive failure
			break;
		}
	}

	return (int)TotalReceived;
}


int ATCPCommunication::Recv(SOCKET ConnectSocket, std::fstream& oFile, std::size_t TotalToReceive)
{
	//const int BufferSize = 1;
	const int BufferSize = 4096;

	// nothing receied to start
	size_t TotalReceived = 0;

	// remining bytes to receove
	size_t Remaning = TotalToReceive;

	// use internal 4K buffer
	char Buffer[BufferSize];
	memset(Buffer, 0, sizeof(Buffer));


	while (TotalReceived < TotalToReceive) {

		// load size is either full size of the buffer or bytes remaining
		int LoadSize = min((int)Remaning, sizeof(Buffer));

		// receive the data
		int SizeReceived = recv(ConnectSocket, &Buffer[0], LoadSize, 0);
		if (SizeReceived > 0) {	
			// flush to file
			oFile.write(&Buffer[0], SizeReceived);

			TotalReceived = TotalReceived + SizeReceived;
			Remaning = TotalToReceive - SizeReceived;
		}
		else {
			// receive failure
			break;
		}
	}

	return (int)TotalReceived;

}

// Process request, make sure the socket gets disconnected at end
int ATCPCommunication::ProcessConnectSession(SOCKET ConnectSocket)
{
	// Process the request
	int retval = ProcessRequest(ConnectSocket);

	// force closing of socket after process
	closesocket(ConnectSocket);

	ConnectSocket = INVALID_SOCKET;

	return retval;
}


int ATCPCommunication::ProcessRequest(SOCKET ConnectSocket)
{
	return 0;
}
