#include "User.h"


void User::handleMessage(Socket* _clientSocket, const char* _msg, int size,
	MessageType _msgType)
{
	handleMessage_UserInfo(_clientSocket, _msg, size);
}

void User::handleMessage_UserInfo(Socket* _clientSocket, const char* _msg, int size)
{
	std::cout << "Get user's information or Get all users" << std::endl;

	// Parser the message
	std::vector<std::string> msgs;
	parserMessage(msgs, _msg);

	DATA_PACKAGE dPackage;
	dPackage.dataHeader.dataSize = sizeof(dPackage) - sizeof(DATA_HEADER);
	dPackage.dataHeader.msgType = MessageType::TYPE_USER;

	int msgs_nums = msgs.size();
	bool isGetAllUsers = false;
	if (msgs_nums != 2)
	{
		if ((msgs_nums == 3) && (msgs[2] == "GetAllUsers"))
		{
			// Get all users and get user's is online or offline
			std::vector<std::string> allUsers;
			if (UserInfo::getAllUsers(allUsers))
			{
				isGetAllUsers = true;
				sendData(_clientSocket, allUsers);
			}
			else
				strcpy(dPackage.data, "ERROR_GET ALL USERS ERROR!");
		}
		else
			strcpy(dPackage.data, "PARAMETOR_USER MESSAGE PARAMETER ERROR!");
	}
	else
	{
		// Get user's information
		std::vector<std::string> userinfos;
		if (UserInfo::findUserInfo(msgs[1], userinfos))
		{
			std::string infos = "SingleUser_";
			for (int i = 0; i < userinfos.size(); ++i)
			{
				if (i == userinfos.size() - 1)
					infos += userinfos[i];
				else
					infos += userinfos[i] + "_";
			}
			strcpy(dPackage.data, infos.c_str());
		}
		else
		{
			strcpy(dPackage.data, "ERROR_GET THIS USER'S DATA ERROR!");
		}
	}

	if (!isGetAllUsers)
	{
		if (_clientSocket)
			_clientSocket->write((char*)&dPackage, sizeof(DATA_PACKAGE));
	}
}

void User::sendData(Socket* _clientSocket, std::vector<std::string>& datas)
{
	if (_clientSocket)
	{
		DATA_PACKAGE dPackage;
		dPackage.dataHeader.dataSize = sizeof(dPackage) - sizeof(DATA_HEADER);
		dPackage.dataHeader.msgType = MessageType::TYPE_USER;

		int Groups = datas.size() / 50;
		int Remain = datas.size() % 50;
		// Group numbers
		for (int i = 0; i < Groups; ++i)
		{
			memset(dPackage.data, 0, sizeof(dPackage.data));
			std::string data = "GetAllUsers_";
			for (int j = 0; j < 50; ++j)
			{
				if (j + 1 == 50)	// last
					data += datas[i * 50 + j];
				else
					data += datas[i * 50 + j] + "_";
			}
			strcpy(dPackage.data, data.c_str());

			_clientSocket->write((char*)&dPackage, sizeof(DATA_PACKAGE));
		}

		if (Remain != 0)
		{
			memset(dPackage.data, 0, sizeof(dPackage.data));
			std::string data = "GetAllUsers_";
			for (int i = Groups * 50; i < Groups * 50 + Remain; ++i)
			{
				if ((i + 1) == (Groups * 50 + Remain))	// last
					data += datas[i];
				else
					data += datas[i] + "_";
			}
			strcpy(dPackage.data, data.c_str());

			_clientSocket->write((char*)&dPackage, sizeof(DATA_PACKAGE));
		}
	}
}
