#pragma once

/*
继承AbstractHandle抽象类，实现
（1）接收客户端连接、收到客户端消息；
（2）客户端连接关闭的消息；
等抽象处理方法。
*/

#include "AbstractHandle.h"
#include "../MessageType.h"
#include "../Threads/virtualThread.h"
#include "../Threads/clientThread.h"
#include "DataAccessObject/DBManager.h"
#include "Logic/Login.h"
#include "Logic/Fight.h"
#include "Logic/Match.h"
#include "Logic/User.h"
#include "Logic/AbstractLogic.h"


class MsgHandle : public AbstractHandle
{
public:
	MsgHandle();
	virtual ~MsgHandle();

public:
	// 客户端连接、收到客户端消息
	void handleCliConn_MsgRecv(MESSAGE& _msg);

	// 客户端连接关闭
	void handleCliClose(Socket* _clientSocket, const char* _error);

	// 客户端连接断开时，释放客户端sockets、客户端线程
	void releaseResource(Socket* _clientSocket);

public:
	enum logicType
	{
		SIGN,		// 登录、注册
		USER,
		FIGHT,
		MATCH,
		SUM_LOGICTYPE
	};

private:
	// 逻辑处理
	AbstractLogic* logicHandles[SUM_LOGICTYPE];

	// 所有客户端sockets
	// std::list<Socket*>& cSockets;

	// 所有处理客户端的线程
	// std::list<virtualThread*>& cThreads;

	// 数据库连接对象
	DBManager* db;
};
