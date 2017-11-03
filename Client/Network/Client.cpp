#include "Client.h"

Client* Client::cliInstance = nullptr;

Client* Client::GetCliInstance()
{
	if (cliInstance)
		return cliInstance;
	cliInstance = new Client();

	return cliInstance;
}

Client::Client():
	isConnect(false)
{
	_s = socket(AF_INET, SOCK_STREAM, 0);
	if (_s == INVALID_SOCKET) throw "CLIENT: invalid socket!";
}

bool Client::Connect(const char* addr, unsigned short port)
{
	sockaddr_in sa;
	memset(&sa, 0, sizeof(sa));

	sa.sin_family = PF_INET;
	sa.sin_port = htons(port);
	sa.sin_addr.S_un.S_addr = inet_addr(addr);

	// 尝试与服务器进行连接
	if (SOCKET_ERROR == connect(_s, (const sockaddr*)&sa, sizeof(sockaddr_in)))
	{
		closesocket(_s);
		isConnect = false;
		return false;
	}
	isConnect = true;

	return true;
}
