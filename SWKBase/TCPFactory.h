#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <memory>


namespace swktool {	

	/// <summary>
	/// TCp/IP abstract factory
	/// creates object to handle work, but must require SOCKET for TCP/IP communication
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<class T>
	class TCPFactory {
	public:
		virtual std::unique_ptr<T> Create(SOCKET ConnectSocker) = 0;
	};
}