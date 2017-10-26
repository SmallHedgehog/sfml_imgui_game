#pragma once

#include <string>

#define DEBUG 1

// 定义消息类型
enum MessageType
{
	TYPE_SIGNIN,				// 登录消息类型
	TYPE_SIGNUP,				// 注册消息类型
	TYPE_SIGNOUT,				// 登出消息类型
	TYPE_USER,					// 用户消息类型
	TYPE_MATCH,					// 匹配消息类型
	TYPE_FIGHT,					// 交战消息类型
	TYPE_CHAT,					// Chat message
	TYPE_NONE
};


// 定义数据类型

#define MAX_DATA_SIZE	1024	// max data size

/* 数据包头定义 */
typedef struct data_header
{
	MessageType msgType;		// 消息类型
	unsigned int dataSize;		// 数据包中真实数据的长度

	data_header& operator = (const data_header& _data)
	{
		if (this != &_data)
		{
			msgType = _data.msgType;
			dataSize = _data.dataSize;
		}
		return *this;
	}
}DATA_HEADER;

/* 数据包类型定义 */
typedef struct data_package
{
	DATA_HEADER dataHeader;		// 数据包头
	char data[MAX_DATA_SIZE];	// 真实数据

	data_package& operator = (const data_package& _data)
	{
		if (this != &_data)
		{
			dataHeader = _data.dataHeader;
			memcpy(data, _data.data, sizeof(data));
		}
		return *this;
	}
}DATA_PACKAGE;

/* 消息状态更新 */
typedef struct msg_state
{
	// username
	std::string uName;

	// real-time message
	MessageType type;
	char info[128];

	msg_state()
	{
		type = MessageType::TYPE_SIGNIN;
		// info = nullptr;
	}
}MSG_STATE;

static void ConsData(const char* _msg, MessageType type, DATA_PACKAGE& dPackage)
{
	dPackage.dataHeader.msgType = type;
	dPackage.dataHeader.dataSize = sizeof(dPackage) - sizeof(DATA_HEADER);

	strcpy(dPackage.data, _msg);
}
