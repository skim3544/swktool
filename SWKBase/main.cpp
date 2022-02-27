#include "pch.h"
#include <iostream>
#include <ostream>
#include <thread>
#include <chrono>

#include "DateTime.h"
#include "ThreadPool.h"
#include "TCPServer.h"
#include "TCPClient.h"
#include "TBuffer.h"

using namespace std;

void Test() {
	for (int i = 0; i < 100; i++) {
		cout << "Working..." << endl;

		this_thread::sleep_for(chrono::milliseconds(500));
	}
}

using namespace swktool;

class ConnectionClient : public ATCPCommunication {
public:
	ConnectionClient()
	{
		;
	}

	virtual ~ConnectionClient() { ;  }


	virtual int ProcessRequest(SOCKET ConnectSocket) { 
		char C = '\0';
		TBuffer Buf;
		//int Recv(SOCKET ConnectSocket, TBuffer & Buf, std::size_t Size);
		int r = Recv(ConnectSocket, Buf, 11);
		
		int s = Send(ConnectSocket, "Good bye World");

		closesocket(ConnectSocket);
		return r;
	}
};


class ConnectClient : public ATCPCommunication {
public:
	ConnectClient()
	{
		;
	}

	virtual ~ConnectClient() { ; }


	virtual int ProcessRequest(SOCKET ConnectSocket) {
		TBuffer Buff;
		
		Buff.PutString("Hello World"); 
		int Size = Send(ConnectSocket, Buff);
		
		char buffer[1024];		
		int r = Recv(ConnectSocket, buffer, 14);

		closesocket(ConnectSocket);
		return Size;
	}
};


class ConnectionFactory : public TCPFactory<ATCPCommunication> {
public:
	ConnectionFactory() { ; }

	virtual std::unique_ptr<ATCPCommunication> Create(SOCKET ConnectSocker)
	{
		return std::make_unique<ConnectionClient>();
	}
};

class ClientConnectionFactory : public TCPFactory<ATCPCommunication> {
public:
	ClientConnectionFactory() { ; }

	virtual std::unique_ptr<ATCPCommunication> Create(SOCKET ConnectSocker)
	{
		return std::make_unique<ConnectClient>();
	}
};


int main()
{
	swktool::ThreadPool TP(10);
	std::unique_ptr<TCPFactory<ATCPCommunication>> oConnectionFactory = std::make_unique<ConnectionFactory>();

	std::unique_ptr<TCPFactory<ATCPCommunication>> oClientFactory = std::make_unique<ClientConnectionFactory>();

	swktool::TCPServer Svr("Localhost", 5200, oConnectionFactory.get());
	Svr.RunInBackground();


	swktool::TCPClient Client("Localhost", 5200, oClientFactory.get(), 10);
	Client.RunInBackground();


	while (true) {

		
		
		

		
		//ThreadContext t(Test);
		//thread t(Test);
		//TP.AddThread(move(t));

		;		

		this_thread::sleep_for(chrono::milliseconds(20000));
	}


	/*
	DateTime date1(2000, 1, 1, 11, 50, 53);

	DateTime Now1 = DateTime::Now();
	DateTime Now2 = DateTime::Now();

	string sTime = Now1.ToString("%x %X");

	bool b = DateTime::IsLeapYear(1900);
	b = DateTime::IsLeapYear(2000);
	b = DateTime::IsLeapYear(2004);

	string LongDateStr = Now1.ToLongDateString();
	string LongTimeStr = Now1.ToLongTimeString();

	string ShortDateStr = Now1.ToShortDateString();
	string ShortTimeStr = Now1.ToShortTimeString();

	std::cout << "Hello World!\n" << Now1 << " " + sTime << endl;
	std::cout << LongDateStr << " " << LongTimeStr << endl;
	std::cout << ShortDateStr << " " << ShortTimeStr << endl;
	{
		DateTime Yesterday = Now1.AddDays(-1);
		ShortDateStr = Yesterday.ToShortDateString();
		ShortTimeStr = Yesterday.ToShortTimeString();
		std::cout << ShortDateStr << " " << ShortTimeStr << endl;
	}

	if (Now1 == Now2) cout << "Same Day" << endl;

	DateTime NextDay = Now1.AddDays(1);
	DateTime BeforeDay = Now1.AddDays(-1);
	if (NextDay == Now2) cout << "Same Day2" << endl;
	if (NextDay > Now2) cout << "Later Day2" << endl;
	if (NextDay < Now2) cout << "Before Day2" << endl;

	if (BeforeDay == Now2) cout << "Same Day3" << endl;
	if (BeforeDay > Now2) cout << "Later Day3" << endl;
	if (BeforeDay < Now2) cout << "Before Day3" << endl;
	*/
}
