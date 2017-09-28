#pragma once

#include "Socket.h"


class Server : public Socket
{
public:
	Server(unsigned short port, unsigned int connections);

	Socket* Accept();
};
