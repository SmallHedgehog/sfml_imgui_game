#include "clientThread.h"


clientThread::clientThread(Socket* _clientSocket, MsgQueue* _msg):
	clientSocket(_clientSocket), msgQueue(_msg)
{
	// do something
	isClosed = false;
}

void clientThread::run()
{
	char buffer[256] = { 0 };

	while (true)
	{
		if (!clientSocket || isClosed)
			break;

		int recvSize = clientSocket->READ(buffer, sizeof(buffer));
		if (recvSize > 0)
		{
			// 需要进行TCP粘包处理等
			tcpDataSplit(buffer, recvSize);
		}
		else
		{
			if (!isClosed)
			{
				std::cout << "客户端已经关闭" << std::endl;
				// 客户端连接关闭
				DATA_PACKAGE dataMsg;
				dataMsg.dataHeader.msgType = MessageType::TYPE_SIGNOUT;
				strcpy(dataMsg.data, "NONE");
				MESSAGE msg(clientSocket, dataMsg);
				if (msgQueue)
				{
					(*msgQueue) << msg;
				}
				clientSocket = nullptr;
				isClosed = true;
			}
			break;
		}
	}
}

void clientThread::handleRecvData(DATA_HEADER* pHeader)
{
	// 消息类型
	// MessageType msgType = pHeader->msgType;
	// 数据包
	DATA_PACKAGE* data = (DATA_PACKAGE*)pHeader;
	/*
	std::cout << data->dataHeader.msgType << std::endl;
	std::cout << data->dataHeader.dataSize << std::endl;
	std::cout << data->data << std::endl;
	std::cout << clientSocket->addr << std::endl;
	*/

	DATA_PACKAGE dataMsg;
	memcpy(&dataMsg, data, sizeof(DATA_PACKAGE));
	if (dataMsg.dataHeader.msgType == MessageType::TYPE_SIGNOUT)
	{
		// 登出消息
		isClosed = true;
	}

	// std::cout << dataMsg.dataHeader.dataSize << std::endl;
	// std::cout << dataMsg.data << std::endl;

	// 送往消息队列中的消息
	MESSAGE msg(clientSocket, dataMsg);
	if (msgQueue)
	{
		(*msgQueue) << msg;
	}
}

void clientThread::tcpDataSplit(const char* recvData, int recvSize)
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
