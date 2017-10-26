#include "Application.h"


extern ChatUISys	cUI;
extern BattleUISys	bUI;

Application* Application::app = nullptr;

Application* Application::GetInstance(sf::RenderWindow* _window)
{
	if (!app)
		app = new Application(_window);
	return app;
}

Application::Application():
	window(nullptr),
	msgQueue(nullptr),
	msgDeal(nullptr),
	msgRecv(nullptr)
{
	curState = UIState::SIGNIN;
	cli = Client::GetCliInstance();

	UI::init();
}

Application::Application(sf::RenderWindow* _window):
	window(_window),
	msgQueue(nullptr),
	msgDeal(nullptr),
	msgRecv(nullptr)
{
	curState = UIState::SIGNIN;
	cli = Client::GetCliInstance();

	UI::init();
}

void Application::display()
{
	switch (curState)
	{
	case SIGNIN:
		if (window)
		{
			window->setTitle("sign in");
			sf::Vector2u Vu(SIGN_X, SIGN_Y);
			window->setSize(Vu);
		}
		UI::signin(msgState, std::bind(&Application::signin, this, std::placeholders::_1, std::placeholders::_2, 
			std::placeholders::_3));
		break;
	case SIGNUP:
		if (window)
		{
			window->setTitle("sign up");
			sf::Vector2u Vu(SIGN_X, SIGN_Y);
			window->setSize(Vu);
		}
		UI::signup(msgState, std::bind(&Application::signup, this, std::placeholders::_1, std::placeholders::_2,
			std::placeholders::_3, std::placeholders::_4));
		break;
	case MAIN:
		if (window)
		{
			window->setTitle("mainUI");
			sf::Vector2u Vu(MAIN_X, MAIN_Y);
			window->setSize(Vu);
		}
		UI::mainUI(msgState, std::bind(&Application::mainUI, this, std::placeholders::_1, std::placeholders::_2), 
			std::bind(&Application::chatFunc, this, std::placeholders::_1), std::bind(&Application::usersFunc, this, std::placeholders::_1, std::placeholders::_2));
		break;
	case FIGHT:
		break;
	default:
		break;
	}
}

void Application::usersFunc(bool isNGetAllUsers, const char* user)
{
#ifdef DEBUG
	std::cout << "userFunc are called" << std::endl;
#endif // DEBUG
	std::string msg(msgState.uName);
	if (isNGetAllUsers)
	{
		if (user)
			msg += "_" + std::string(user);
		else
			return;
	}
	else
		msg += "_NONE_GetAllUsers";
	MsgSend(msg.c_str(), MessageType::TYPE_USER);
}

void Application::chatFunc(const char* text)
{
	if (text)
	{
#ifdef DEBUG
		std::cout << "chatFunc are called" << std::endl;
#endif // DEBUG
		std::vector<const char*> vec;
		vec.push_back(msgState.uName.c_str());
		vec.push_back(text);
		MsgSend(vec, MessageType::TYPE_CHAT);
	}
}

void Application::signin(const char* username, const char* password, bool isSignup)
{
	if (!isSignup)
	{
#ifdef DEBUG
		std::cout << username << std::endl;
		std::cout << password << std::endl;
#endif // DEBUG
		msgState.uName = std::string(username);

		std::vector<const char*> vec;
		vec.push_back(username);
		vec.push_back(password);
		MsgSend(vec, MessageType::TYPE_SIGNIN);
	}
	else
	{
		setCurState(UIState::SIGNUP);
		if (cli)
		{
			if (!cli->isConnect)
				setMsgState(MessageType::TYPE_SIGNUP, "Connect server failure,Server or \nClient error!");
			else
				setMsgState(MessageType::TYPE_SIGNUP, "Connect server successful!");
		}
	}
}

void Application::signup(const char* username, const char* password, const char* validate, bool isBackup)
{
	if (!isBackup)
	{
		if (cli)
		{
			if (cli->isConnect)
			{
				// ÑéÖ¤ÃÜÂë
				if (strcmp(password, validate) == 0)
				{
					// ×¢²á
				}
				else
					setMsgState(MessageType::TYPE_SIGNUP, "Validate password is not identical!");
			}
		}
	}
	else
	{
		setCurState(UIState::SIGNIN);
		if (cli)
		{
			if (!cli->isConnect)
				setMsgState(MessageType::TYPE_SIGNIN, "Connect server failure,Server or \nClient error!");
			else
				setMsgState(MessageType::TYPE_SIGNIN, "Connect server successful!");
		}
	}
}

void Application::mainUI(unsigned char sign, bool isExited)
{
	switch (sign)
	{
	case 1:
#ifdef DEBUG
		std::cout << "Choose Man-machine battle button" << std::endl;
#endif // DEBUG
		setMsgState(MessageType::TYPE_NONE, "You have choosed (Man-machine battle) button!");
		break;
	case 2:
#ifdef DEBUG
		std::cout << "Choose User-user battle button" << std::endl;
#endif // DEBUG
		setMsgState(MessageType::TYPE_NONE, "You have choosed (User-user battle) button!");
		break;
	case 3:
#ifdef DEBUG
		std::cout << "Choose Group chat system button" << std::endl;
#endif // DEBUG
		setMsgState(MessageType::TYPE_NONE, "You have choosed (Group chat system) button!");
		break;
	case 4:
#ifdef DEBUG
		std::cout << "Choose About ImGui-sfml game button" << std::endl;
#endif // DEBUG
		setMsgState(MessageType::TYPE_NONE, "You have choosed (About ImGui-sfml game) button!");
		break;
	case 5:
#ifdef DEBUG
		std::cout << "Choose Exit button" << std::endl;
#endif // DEBUG
		if (isExited)
		{
			std::vector<const char*> vec;
			vec.push_back(msgState.uName.c_str());
			MsgSend(vec, MessageType::TYPE_SIGNOUT);
			
			// Exit system
			exit(0);
		}
		else
			setMsgState(MessageType::TYPE_NONE, "You have choosed (Exit) button!");
		break;
	}
}

void Application::setWindow(sf::RenderWindow* _window)
{
	window = _window;
}

void Application::setCurState(UIState _state)
{
	curState = _state;
}

void Application::setMsgState(MessageType type, const char* text)
{
	std::lock_guard<std::mutex> lock(_mutex);

	msgState.type = type;
	memset(msgState.info, 0, sizeof(msgState.info));
	strcpy(msgState.info, text);
}

void Application::MsgHandleCenter(DATA_PACKAGE& dPackage)
{
#ifdef DEBUG
	std::cout << "MsgHandleCenter" << std::endl;
#endif // DEBUG

	MessageType msgType = dPackage.dataHeader.msgType;
	switch (msgType)
	{
	case TYPE_SIGNIN:
		SigninHandle(dPackage.data);
		break;
	case TYPE_SIGNUP:
		break;
	case TYPE_USER:
		UserHandle(dPackage.data);
		break;
	case TYPE_MATCH:
		break;
	case TYPE_FIGHT:
		break;
	case TYPE_SIGNOUT:
		break;
	case TYPE_CHAT:
		ChatHandle(dPackage.data);
		break;
	case TYPE_NONE:
		break;
	default:
		break;
	}
}

void Application::UserHandle(const char* msg)
{
#ifdef DEBUG
	std::cout << msg << std::endl;
#endif // DEBUG
	std::vector<std::string> msgs;
	ParserMsg(msgs, msg, "_");

	if (!msgs.empty())
	{
		if (msgs[0] == "GetAllUsers")
		{
			cUI.ClearOfuUI();
			bUI.Clear();
			if ((msgs.size() % 2) == 1)
			{
				bool isFind = false;
				for (int i = 1; i < msgs.size(); i += 2)
				{
					const char* isOnline = msgs[i + 1].c_str();
					if (isOnline)
					{
						cUI.AddUserOfuUI(isOnline[0], msgs[i].c_str());
						if (!isFind)
						{
							if (strcmp(msgState.uName.c_str(), msgs[i].c_str()) == 0)
							{
								isFind = true;
								bUI.AddUserOfuUI('2', msgs[i].c_str());		/* It is for find yourself */
							}
							else
								bUI.AddUserOfuUI(isOnline[0], msgs[i].c_str());
						}
						else
							bUI.AddUserOfuUI(isOnline[0], msgs[i].c_str());
					}
				}
			}
		}
		else if (msgs[0] == "SingleUser")
		{
			// do something
		}
	}
}

void Application::ChatHandle(const char* msg)
{
	std::vector<std::string> msgs;
	ParserMsg2(msgs, msg, "_");
	if (msgs.size() > 1)
		cUI.AddLog(0, msgs[0].c_str(), msgs[1].c_str());
}

void Application::SigninHandle(const char* msg)
{
	std::vector<std::string> msgs;
	ParserMsg(msgs, msg, "_");

	int msgs_num = msgs.size();
	if (msgs_num == 2)
	{
		if (msgs[0] == "SUCCESS")
		{
#ifdef DEBUG
			std::cout << "signin success!" << std::endl;
#endif // DEBUG
			setCurState(UIState::MAIN);
			setMsgState(MessageType::TYPE_NONE, "Choosing one of buttons from above and presssing!");
			// Update user's online state
			// cUI.SetItemOfuUI('1', msgState.uName.c_str());
			// bUI.SetItemOfuUI('2', msgState.uName.c_str());
		}
		else
		{
			setCurState(UIState::SIGNIN);
			setMsgState(MessageType::TYPE_NONE, msgs[1].c_str());
		}
	}
	else if (msgs_num == 1)
	{
#ifdef DEBUG
		std::cout << "the user " << msgs[0].c_str() << " online" << std::endl;
#endif // DEBUGG
	}
}

void Application::appInit(const char* addr, unsigned short port)
{
	if (cli)
	{
		msgState.type = MessageType::TYPE_SIGNIN;
		memset(msgState.info, 0, sizeof(msgState.info));
		if (cli->Connect(addr, port))
			strcpy(msgState.info, "Connect server successful!");
		else
			strcpy(msgState.info, "Connect server failure,Server or \nClient error!");
	}

	msgQueue = new MsgQueue();
	msgRecv = new MsgRecvThread(cli, msgQueue);
	msgDeal = new MsgDealThread(msgQueue, std::bind(&Application::MsgHandleCenter, this, std::placeholders::_1));

	if (msgRecv) msgRecv->start();
	if (msgDeal) msgDeal->start();
}

void Application::MsgSend(std::vector<const char*>& msgs, MessageType type)
{
	std::string msg;
	for (int i = 0; i < msgs.size(); ++i)
	{
		if ((i + 1) != msgs.size())
			msg += std::string(msgs[i]) + "_";
		else
			msg += std::string(msgs[i]);
	}
	
	MsgSend(msg.c_str(), type);
}

void Application::MsgSend(const char* msgs, MessageType type)
{
	DATA_PACKAGE dPackage;
	ConsData(msgs, type, dPackage);
	if (cli)
		cli->write((char*)&dPackage, sizeof(dPackage));
}

void Application::ParserMsg(std::vector<std::string>& msgs, const char* msg, const char* delim)
{
	std::string message(msg);

	size_t last = 0;
	size_t index = message.find_first_of(delim, last);
	while (index != std::string::npos)
	{
		msgs.push_back(message.substr(last, index - last));
		last = index + 1;
		index = message.find_first_of(delim, last);
	}

	if (index - last > 0)
	{
		msgs.push_back(message.substr(last, index - last));
	}
}

void Application::ParserMsg2(std::vector<std::string>& msgs, const char* msg, const char* delim)
{
	std::string info(msg);

	size_t last = 0;
	size_t index = info.find_first_of(delim, last);
	if (index == std::string::npos)
	{
		msgs.push_back(info);
		return;
	}
	msgs.push_back(info.substr(last, index - last));
	msgs.push_back(info.substr(index + 1, info.size() - (index + 1)));
}

Application::~Application()
{
	if (cli) delete cli;
	
	if (msgRecv)
		msgRecv->join();
	if (msgDeal)
		msgDeal->join();

	if (msgRecv) delete msgRecv;
	if (msgDeal) delete msgDeal;
	if (msgQueue) delete msgQueue;
}
