#include "Application.h"


Application::Application(unsigned short port, unsigned int connections) :
	server(nullptr), msg(nullptr)
{
	msg = new MsgQueue();
	if (!msg)
	{
		throw "APPLICATION: create MsgQueue error!";
	}

	msgHandle = new MsgHandle();
	if (!msgHandle)
	{
		throw "APPLICATION: create MsgHandle error!";
	}

	server = new Server(port, connections);
	if (!server)
	{
		throw "APPLICATION: create server error!";
	}
}

void Application::run()
{
	// 创建消息处理线程，用于监听消息队列中的消息
	msgHandleThread* msgThread = new msgHandleThread(msg, msgHandle);
	msgThread->start();
	Cache::cThreads.push_back(msgThread);
	while (true)
	{
		// 阻塞处理来自客户端的连接
		std::cout << "接收来自客户端的连接" << std::endl;
		Socket* s = server->Accept();
		if (s)
		{
			Cache::cSockets.push_back(s);
			// 创建客户端线程，处理来自客户端的连接
			clientThread* cThread = new clientThread(s, msg);
			cThread->start();
			Cache::cThreads.push_back(cThread);
		}
	}

	for (std::list<virtualThread*>::iterator it = Cache::cThreads.begin(); it != Cache::cThreads.end(); ++it)
	{
		if (*it)
			(*it)->join();
	}
}

Application::~Application()
{
	if (server)
		delete server;

	if (msgHandle)
		delete msgHandle;

	if (msg)
		delete msg;
	// 客户端处理线程
	for (std::list<virtualThread*>::iterator it = Cache::cThreads.begin(); it != Cache::cThreads.end(); ++it)
	{
		if (*it)
			delete (*it);
	}
	Cache::cThreads.clear();

	// 客户端连接Socket
	for (std::list<Socket*>::iterator it = Cache::cSockets.begin(); it != Cache::cSockets.end(); ++it)
	{
		if (*it)
			delete (*it);
	}
	Cache::cSockets.clear();
}
