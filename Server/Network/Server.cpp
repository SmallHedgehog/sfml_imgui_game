#include "Server.h"


Server::Server(unsigned short port, unsigned int connections)
{
	sockaddr_in sa;
	memset(&sa, 0, sizeof(sa));

	sa.sin_family = PF_INET;
	sa.sin_port = htons(port);
	sa.sin_addr.S_un.S_addr = INADDR_ANY;

	_s = socket(AF_INET, SOCK_STREAM, 0);
	if (_s == INVALID_SOCKET)
	{
		throw "SERVER: invalid socket!";
	}

	// bind
	if (bind(_s, (sockaddr*)&sa, sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		closesocket(_s);
		throw "SERVER: bind error";
	}

	// listen
	listen(_s, connections);
}

Socket* Server::Accept()
{
	sockaddr_in addr;
	int addr_size = sizeof(addr);
	SOCKET client_socket = accept(_s, (sockaddr*)&addr, &addr_size);
	if (client_socket == INVALID_SOCKET)
	{
		int res = WSAGetLastError();
		if (res == WSAEWOULDBLOCK)		// 非阻塞方式且无连接可供接受
		{
			return 0;
		}
		else
		{
			throw "SERVER: accept error!";
		}
	}

	Socket* s = new Socket(client_socket);
	if (s)
		s->addr = inet_ntoa(addr.sin_addr);

	return s;
}
