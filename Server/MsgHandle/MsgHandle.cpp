#include "MsgHandle.h"


MsgHandle::MsgHandle()
{
	// 登录、注册
	logicHandles[SIGN] = new Login();

	logicHandles[USER] = new User();
	logicHandles[FIGHT] = new Fight();
	logicHandles[MATCH] = new Match();

	// 初始化数据库连接对象
	db = DBManager::getInstance();
	db->init("120.25.163.214", "root", "123456");
}

void MsgHandle::handleCliConn_MsgRecv(MESSAGE& _msg)
{
	// 消息类型
	MessageType msgType = _msg.data.dataHeader.msgType;

	switch (msgType)
	{
	case MessageType::TYPE_SIGNIN:		// 登录消息处理
		logicHandles[SIGN]->handleMessage(_msg.cSocket, _msg.data.data, _msg.data.dataHeader.dataSize,
			MessageType::TYPE_SIGNIN);
		break;
	case MessageType::TYPE_SIGNUP:		// 注册消息处理
		logicHandles[SIGN]->handleMessage(_msg.cSocket, _msg.data.data, _msg.data.dataHeader.dataSize,
			MessageType::TYPE_SIGNUP);
		break;
	case MessageType::TYPE_SIGNOUT:		// 登出消息处理
		logicHandles[SIGN]->handleMessage(_msg.cSocket, _msg.data.data, _msg.data.dataHeader.dataSize,
			MessageType::TYPE_SIGNOUT);
		handleCliClose(_msg.cSocket, "");
		break;
	case MessageType::TYPE_USER:		// 用户消息处理
		logicHandles[USER]->handleMessage(_msg.cSocket, _msg.data.data, _msg.data.dataHeader.dataSize);
		break;
	case MessageType::TYPE_FIGHT:		// 交战消息处理
		logicHandles[FIGHT]->handleMessage(_msg.cSocket, _msg.data.data, _msg.data.dataHeader.dataSize);
		break;
	case MessageType::TYPE_MATCH:		// 匹配消息处理
		logicHandles[MATCH]->handleMessage(_msg.cSocket, _msg.data.data, _msg.data.dataHeader.dataSize);
		break;
	default:
		break;
	}
}

void MsgHandle::handleCliClose(Socket* _clientSocket, const char* _error)
{
	// 客户端连接断开时，释放客户端sockets、客户端线程
	releaseResource(_clientSocket);
}

void MsgHandle::releaseResource(Socket* _clientSocket)
{
/*必须要先删除处理客户端的线程，然后再删客户端Socket*/
	// 删除客户端关闭时处理客户端的线程
	for (std::list<virtualThread*>::iterator it = Cache::cThreads.begin(); it != Cache::cThreads.end(); )
	{
		clientThread* cThread = dynamic_cast<clientThread*>((*it));
		if (cThread)
		{
			Socket* cSocket = cThread->get_cSocket();
			if (&(*cSocket) == &(*(_clientSocket)))
			{
				cThread->join();
				delete (*it);
				it = Cache::cThreads.erase(it);
			}
			else
				++it;
		}
		else
			++it;
	}
	// 删除客户端关闭时的客户端Socket
	for (std::list<Socket*>::iterator it = Cache::cSockets.begin(); it != Cache::cSockets.end(); )
	{
		if (&(*(*it)) == &(*(_clientSocket)))
		{
			delete (*it);
			it = Cache::cSockets.erase(it);
		}
		else
			++it;
	}
}

MsgHandle::~MsgHandle()
{
	for (int i = 0; i < (int)SUM_LOGICTYPE; ++i)
	{
		if (logicHandles[i])
			delete logicHandles[i];
	}

	if (db)
		delete db;
}
