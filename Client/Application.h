#pragma once

#include "UI/UI.h"
#include "Network/Client.h"
#include "Threads/MsgDealThread.h"
#include "Threads/MsgRecvThread.h"
#include "MsgQueue/MsgQueue.h"

#include <stdlib.h>


class Application
{
public:
	Application();
	Application(sf::RenderWindow* _window);
	~Application();
	static Application* GetInstance(sf::RenderWindow* _window);

	Application(const Application&) = delete;
	Application& operator = (const Application&) = delete;

public:
	void appInit(const char* addr, unsigned short port);
	void display();

	// handle or send message
	void ParserMsg(std::vector<std::string>& msgs, const char* msg, const char* delim);
	void ParserMsg2(std::vector<std::string>& msgs, const char* msg, const char* delim);
	void MsgSend(std::vector<const char*>& msgs, MessageType type);
	inline void MsgSend(const char* msgs, MessageType type) 
	{ 
		DATA_PACKAGE dPackage; 
		ConsData(msgs, type, dPackage); 
		if (cli)
			cli->write((char*)&dPackage, sizeof(dPackage));
	}
	inline void MsgSend(const char* msgs, MessageType type, SMessageTypeFlags_ sType)
	{
		DATA_PACKAGE dPackage;
		FilldPackage(type, sType, msgs, dPackage);
		if (cli)
			cli->write((char*)&dPackage, sizeof(dPackage));
	}

	// real-time update
	inline void setWindow(sf::RenderWindow* _window) 	 { window = _window; }
	inline void setCurState(UIState _state)			 { std::lock_guard<std::mutex> lock(_mutex); curState = _state; }
	void setMsgState(MessageType type, const char* text);

	// callback functions for UI
	void fight(unsigned char callbackType, const char* msg);
	void mainUI(unsigned char sign, bool isExited);
	void chatFunc(const char* text);
	void usersFunc(bool isNGetAllUsers, const char* user);
	void matchFunc(bool isMatch, const char* user);
	void matchWaitAgreeFunc(int flags, const char* matUser);
	void signup(const char* username, const char* password, const char* validate, bool isBackup);
	void signin(const char* username, const char* password, bool isSignup);
	void signout(const char* username);

private:
	UIState curState;
	MSG_STATE msgState;

	Client* cli;

	sf::RenderWindow* window;

	static Application* app;

	MsgQueue* msgQueue;
	MsgRecvThread* msgRecv;
	MsgDealThread* msgDeal;

	// 线程同步
	std::mutex _mutex;
	
private:
	void MsgHandleCenter(DATA_PACKAGE& dPackage);

	// message handle callback functions
	void SigninHandle(const char* msg);
	void SignupHandle(const char* msg);
	void SignoutHandle(const char* msg);
	void UserHandle(const char* msg);
	void MatchHandle(SMessageTypeFlags_ _stype, const char* msg);
	void FightHandle(SMessageTypeFlags_ _stype, const char* msg);
	void ChatHandle(const char* msg);
};

