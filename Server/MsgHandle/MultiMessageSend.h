#pragma once

#include <list>
#include "../Network/Socket.h"
#include "../MessageType.h"


/* 用于消息群发 */

class MultiMessageSend
{
public:
	static void messageSend(std::list<Socket*>& _clientSockets, Socket* filter, DATA_PACKAGE& _data)
	{
		for (std::list<Socket*>::iterator it = _clientSockets.begin(); it != _clientSockets.end(); ++it)
		{
			if ((*it) && 
				&(*(*it)) != &(*(filter)))
			{
				(*it)->write((char*)&_data, sizeof(DATA_PACKAGE));
			}
		}
	}

	static void messageSend(std::list<Socket*>& _clientSockets, DATA_PACKAGE& _data)
	{
		for (std::list<Socket*>::iterator it = _clientSockets.begin(); it != _clientSockets.end(); ++it)
		{
			if ((*it))
			{
				(*it)->write((char*)&_data, sizeof(DATA_PACKAGE));
			}
		}
	}
};
