#include "MsgRecvThread.h"

MsgRecvThread::MsgRecvThread(Client* _cli, MsgQueue* _msg):
	msg(_msg),
	cli(_cli)
{}

void MsgRecvThread::run()
{
	char buffer[256] = { 0 };

	while (true)
	{
		if (!cli)
			break;

		int recvSize = cli->READ(buffer, sizeof(buffer));
		if (recvSize > 0)
		{
			// TCP粘包处理接收到的数据
			tcpDataSplit(buffer, recvSize);
		}
		else
		{
			std::cout << "连接异常..." << std::endl;
			DATA_PACKAGE dPackage;
			// ConsData("CONNECT ERROR!", MessageType::TYPE_SIGNOUT, dPackage);
			FilldPackage(MessageType::TYPE_SIGNOUT, SMessageTypeFlags_IsTheUserSingout, "CONNECT ERROR!", dPackage);
			if (msg)
				(*msg) << dPackage;
			break;
		}
	}
}

void MsgRecvThread::tcpDataSplit(const char* recvData, int recvSize)
{
	static char lastRecvData[MAX_DATA_SIZE * 4];
	static unsigned int currentSize = 0;
	static bool isFirst = true;

	if (isFirst)
	{
		memset(lastRecvData, 0, sizeof(lastRecvData));
		isFirst = false;
	}

	// 本次接收到的数据拼接到上次数据
	memcpy((char*)(lastRecvData + currentSize), recvData, recvSize);
	currentSize += recvSize;

	// 强制转换成DATA_HEADER类型的指针
	DATA_HEADER* hPtr = (DATA_HEADER*)lastRecvData;
	while (currentSize > sizeof(DATA_HEADER) &&
		currentSize >= (hPtr->dataSize + sizeof(DATA_HEADER)))
	{
		// 处理接收到的数据
		handleRecvData(hPtr);
		// 本次接受到的对象大小
		int recvObjectSize = sizeof(DATA_HEADER) + hPtr->dataSize;
		currentSize -= recvObjectSize;
		// 移动下一个对象
		hPtr = (DATA_HEADER*)((char*)hPtr + recvObjectSize);
	}

	// 剩余的对象未能组成一个对象的大小，需要保存起来
	if (lastRecvData != (char*)hPtr)
	{
		memmove(lastRecvData, (char*)hPtr, currentSize);
		memset((char*)(lastRecvData + currentSize), 0, sizeof(lastRecvData) - currentSize);
	}
}

void MsgRecvThread::handleRecvData(DATA_HEADER* pHeader)
{
	// 数据包
	DATA_PACKAGE* data = (DATA_PACKAGE*)pHeader;
	DATA_PACKAGE dataMsg;
	memcpy(&dataMsg, data, sizeof(DATA_PACKAGE));

	if (msg)
		(*msg) << dataMsg;
}
