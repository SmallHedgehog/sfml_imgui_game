#include "Chat.h"


void Chat::handleMessage(Socket* _clientSocket, const char* _msg, int size,
	MessageType _msgType)
{
	handleMessage_ChatInfo(_clientSocket, _msg, size);
}

void Chat::handleMessage_ChatInfo(Socket* _clientSocket, const char* _msg, int size)
{
	std::cout << "chat message handle" << std::endl;
	std::cout << _msg << std::endl;

	// Parser the _msg by '_'
	std::vector<std::string> vec;
	ParserMsg(vec, _msg);
	if (vec.size() > 1)
	{
		DATA_PACKAGE dPackage;
		dPackage.dataHeader.dataSize = sizeof(dPackage) - sizeof(DATA_HEADER);
		dPackage.dataHeader.msgType = MessageType::TYPE_CHAT;

		strcpy(dPackage.data, _msg);
		MultiMessageSend::messageSend(Cache::cSockets, _clientSocket, dPackage);
	}
}

void Chat::ParserMsg(std::vector<std::string>& vec, const char* _msg)
{
	std::string info(_msg);

	size_t last = 0;
	size_t index = info.find_first_of('_', last);
	if (index == std::string::npos)
	{
		vec.push_back(info);
		return;
	}
	vec.push_back(info.substr(last, index - last));
	vec.push_back(info.substr(index + 1, info.size() - (index + 1)));
}
