#pragma once

#include <WinSock2.h>

class Socket
{
public:
	Socket(const Socket& _socket);
	Socket& operator=(Socket& _socket);
	virtual ~Socket();

	int read(void* buffer, unsigned int size);
	int READ(void* buffer, unsigned int size);
	int write(void* buffer, unsigned int size);
	void close();

	// ´æ´¢¿Í»§¶ËµØÖ·
	char* addr;

protected:
	friend class Server;

	Socket(SOCKET s);
	Socket();

	SOCKET _s;

	int* refCounter;

private:
	static void start();
	static void end();
	static int nofSockets;
};
