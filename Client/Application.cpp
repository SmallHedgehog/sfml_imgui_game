#include "Application.h"


extern ChatUISys	cUI;
extern BattleUISys	bUI;
extern FightUISys	fightUISys;

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
			window->setSize(sf::Vector2u(SIGN_X, SIGN_Y));
		}
		UI::signin(msgState, std::bind(&Application::signin, this, std::placeholders::_1, std::placeholders::_2, 
			std::placeholders::_3));
		break;
	case SIGNUP:
		if (window)
		{
			window->setTitle("sign up");
			window->setSize(sf::Vector2u(SIGN_X, SIGN_Y));
		}
		UI::signup(msgState, std::bind(&Application::signup, this, std::placeholders::_1, std::placeholders::_2,
			std::placeholders::_3, std::placeholders::_4));
		break;
	case MAIN:
		if (window)
		{
			window->setTitle("mainUI");
			window->setSize(sf::Vector2u(MAIN_X, MAIN_Y));
		}
		UI::mainUI(msgState, std::bind(&Application::mainUI, this, std::placeholders::_1, std::placeholders::_2), 
			std::bind(&Application::chatFunc, this, std::placeholders::_1), std::bind(&Application::usersFunc, this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&Application::matchFunc, this, std::placeholders::_1, std::placeholders::_2),
			std::bind(&Application::matchWaitAgreeFunc, this, std::placeholders::_1, std::placeholders::_2));
		break;
	case FIGHT:
		if (window)
		{
			window->setTitle("Fight system(User-User)");
			window->setSize(sf::Vector2u(FIGHT_X, FIGHT_Y));
		}
		UI::FightUI(FightTypeFlags_UserToUser, std::bind(&Application::fight, this, std::placeholders::_1, std::placeholders::_2));
		break;
	case FIGHTTOMACHINE:
		if (window)
		{
			window->setTitle("Fight system(User-Machine)");
			window->setSize(sf::Vector2u(FIGHT_X, FIGHT_Y));
		}
		fightUISys.SetCType(ChessTypeFlags_White, ChessTypeFlags_Black);
		UI::FightUI(FightTypeFlags_UserToMachine, std::bind(&Application::fight, this, std::placeholders::_1, std::placeholders::_2));
		break;
	default:
		break;
	}
}

void Application::matchFunc(bool isMatch, const char* user)
{
#ifdef DEBUG
	if (user)
		std::cout << "matchFunc are called, match " << user << std::endl;
	else
		std::cout << "matchFunc are called" << std::endl;
#endif // DEBUG
	if (isMatch)
	{
		std::string msg(msgState.uName);
		msg += "_" + std::string(user);
		MsgSend(msg.c_str(), MessageType::TYPE_MATCH);
	}
	else
	{
		std::string msg = "COMPLETE_" + msgState.uName + "_" + std::string(user);
		MsgSend(msg.c_str(), MessageType::TYPE_MATCH);
	}
}

void Application::matchWaitAgreeFunc(int flags, const char* matUser)
{
#ifdef DEBUG
	std::cout << "matchWaitAgreeFunc are called" << std::endl;
#endif // DEBUG
	std::string text;
	switch (flags)
	{
	case 1:		// match AGREE
		bUI.SetUAMInfos(1, bUI.GetMatUser());
		usersFunc(true, bUI.GetMatUser());
		text = "AGREE_" + msgState.uName + "_" + std::string(matUser);
		bUI.SetMatchState(MatchStateFlags_AGREE);
		bUI.SetItemOfuUI('3', matUser);
		setCurState(UIState::FIGHT);
		// Set MyChess type and EnemyChess type
		fightUISys.SetCType(ChessTypeFlags_Black, ChessTypeFlags_White);
		fightUISys.Clear();
		fightUISys.SetRespInitState();
		break;
	case 2:		// match REJECT
		text = "REJECT_" + msgState.uName + "_" + std::string(matUser);
		bUI.SetItemOfuUI('1', matUser);
		bUI.SetMatchState(MatchStateFlags_REJECT);
		break;
	case 3:		// match OVERTIME
		text = "OVERTIME_" + msgState.uName + "_" + std::string(matUser);
		bUI.SetItemOfuUI('1', matUser);
		bUI.SetMatchState(MatchStateFlags_NONE);
		break;
	}
	bUI.SetIsEndTimer(true);
	// bUI.SetIsShowMatchAUI(false);
	MsgSend(text.c_str(), MessageType::TYPE_MATCH);
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

void Application::signout(const char* username)
{
#ifdef DEBUG
	std::cout << "singout: " << username << std::endl;
#endif // DEBUG
	std::string text(username);
	text += "_NORMAL";
	MsgSend(text.c_str(), MessageType::TYPE_SIGNOUT);
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
				// Validate username
				std::string uname(username);
				for (int i = 0; i < uname.size(); ++i)
				{
					if (uname[i] == '_')
					{
						setMsgState(MessageType::TYPE_SIGNUP, "Validate username is not identical, Not contain '_'!");
						return;
					}
				}
				// Validate password
				if (strcmp(password, validate) == 0)
				{
					// loginup
					uname += "_" + std::string(password);
					MsgSend(uname.c_str(), MessageType::TYPE_SIGNUP);
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

void Application::fight(unsigned char callbackType, const char* msg)
{
#ifdef DEBUG
	std::cout << "Fight callback function are called" << std::endl;
#endif // DEBUG
	switch (callbackType)
	{
	case 1:			// Exit fight module to mainUI
		if (curState == UIState::FIGHTTOMACHINE)
			setCurState(UIState::MAIN);
		else
		{
			//
		}
		break;
	case 2:			// Send chat message
		{
			std::string SendMsg = std::string(bUI.GetReqUser()) + "/" + std::string(bUI.GetMatUser()) + "_" + std::string(msg);
			MsgSend(SendMsg.c_str(), MessageType::TYPE_FIGHT, SMessageTypeFlags_FightChatMsg);
		}
		break;
	case 3:			// Send chess position
		{
			std::string SendMsg = std::string(bUI.GetReqUser()) + "_" + std::string(bUI.GetMatUser()) + "_" + std::string(msg);
			MsgSend(SendMsg.c_str(), MessageType::TYPE_FIGHT, SMessageTypeFlags_FightChessPosMsg);
		}
		break;
	case 4:			// Exit message
		{
			setCurState(UIState::MAIN);
			std::string SendMsg = std::string(bUI.GetReqUser()) + "_" + std::string(bUI.GetMatUser());
			MsgSend(SendMsg.c_str(), MessageType::TYPE_FIGHT, SMessageTypeFlags_FightExitMsg);
		}
		break;
	case 5:
		setCurState(UIState::MAIN);
		break;
	case 6:			// Victory message
		{
			std::string SendMsg = std::string(bUI.GetReqUser()) + "_" + std::string(bUI.GetMatUser());
			MsgSend(SendMsg.c_str(), MessageType::TYPE_FIGHT, SMessageTypeFlags_FightFailure);
		}
		break;
	case 7:			// Failure message
		{
			std::string SendMsg = std::string(bUI.GetReqUser()) + "_" + std::string(bUI.GetMatUser());
			MsgSend(SendMsg.c_str(), MessageType::TYPE_FIGHT, SMessageTypeFlags_FightVictory);
		}
		break;
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
		setCurState(UIState::FIGHTTOMACHINE);
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
			std::string text(msgState.uName.c_str());
			text += "_NORMAL";
			MsgSend(text.c_str(), MessageType::TYPE_SIGNOUT);

			// Exit system
			exit(0);
		}
		else
			setMsgState(MessageType::TYPE_NONE, "You have choosed (Exit) button!");
		break;
	}
}

/*
void Application::setWindow(sf::RenderWindow* _window)
{
	window = _window;
}

void Application::setCurState(UIState _state)
{
	curState = _state;
}
*/

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
		SignupHandle(dPackage.data);
		break;
	case TYPE_USER:
		UserHandle(dPackage.data);
		break;
	case TYPE_MATCH:
		MatchHandle(dPackage.dataHeader.smsgType, dPackage.data);
		break;
	case TYPE_FIGHT:
		FightHandle(dPackage.dataHeader.smsgType, dPackage.data);
		break;
	case TYPE_SIGNOUT:
		SignoutHandle(dPackage.data);
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

void Application::SignupHandle(const char* msg)
{
#ifdef DEBUG
	std::cout << "Signup message handle: " << msg << std::endl;
#endif // DEBUG
	std::vector<std::string> msgs;
	ParserMsg(msgs, msg, "_");

	int msgsSize = msgs.size();
	if (msgsSize == 1)
	{
		bUI.SetItemOfuUI('0', msgs[0].c_str());
		cUI.SetItemOfuUI('0', msgs[0].c_str());
	}
	else if (msgsSize == 2)
	{
		if (msgs[0] == "SUCCESS")
		{
			setCurState(UIState::SIGNIN);
			setMsgState(MessageType::TYPE_SIGNIN, msgs[1].c_str());
		}
		else
		{
			setCurState(UIState::SIGNUP);
			setMsgState(MessageType::TYPE_SIGNUP, msgs[1].c_str());
		}
		
	}
}

void Application::SignoutHandle(const char* msg)
{
	if (msg && (strcmp(msg, "CONNECT ERROR!") != 0))
	{
		bUI.SetItemOfuUI('0', msg);
		cUI.SetItemOfuUI('0', msg);
	}
}

void Application::FightHandle(SMessageTypeFlags_ _stype, const char* msg)
{
#ifdef DEBUG
	std::cout << "Fight message handle: " << msg << std::endl;
#endif // DEBUG
	switch (_stype)
	{
	case SMessageTypeFlags_FightChatMsg:
		{
			std::vector<std::string> msgs;
			ParserMsg2(msgs, msg, "_");
			if (msgs.size() == 2)
			{
				std::vector<std::string> tUsers;
				ParserMsg2(tUsers, msgs[0].c_str(), "/");
				if (tUsers.size() == 2)
				{
					tUsers[0] += "\t" + GetCurTime();
					fightUISys.AddChatMsg(0, msgs[1].c_str(), tUsers[0].c_str());
				}
			}
		}
		break;
	case SMessageTypeFlags_FightChessPosMsg:
		{
			std::vector<std::string> msgs;
			ParserMsg(msgs, msg, "_");
			if (msgs.size() == 4)
			{
				int col = atoi(msgs[2].c_str());
				int row = atoi(msgs[3].c_str());
				fightUISys.AddEnemyChess(col, row);
			}
		}
		break;
	case SMessageTypeFlags_FightExitMsg:
		{
			std::vector<std::string> msgs;
			ParserMsg(msgs, msg, "_");
			if (msgs.size() == 2)
				fightUISys.SetMatchUserExit(true);
		}
		break;
	case SMessageTypeFlags_FightUserToOnline:
		{
			std::vector<std::string> msgs;
			ParserMsg(msgs, msg, "_");
			if (msgs.size() == 2)
			{
				if (msgs[0] == msgState.uName)
				{
					bUI.SetMatchSS(MatchStateFlags_NONE);
					bUI.SetIsEndTimer(false);
					bUI.SetIsTimerComplete(false);
					bUI.SetItemOfuUI('1', msgs[1].c_str());
				}
				else
				{
					if (msgs[1] == msgState.uName)
					{
						bUI.SetMatchSS(MatchStateFlags_NONE);
						bUI.SetIsEndTimer(false);
						bUI.SetIsTimerComplete(false);
						bUI.SetItemOfuUI('1', msgs[0].c_str());
					}
					else
					{
						if (strcmp(msgs[0].c_str(), "") != 0)
							bUI.SetItemOfuUI('1', msgs[0].c_str());
						if (strcmp(msgs[1].c_str(), "") != 0)
							bUI.SetItemOfuUI('1', msgs[1].c_str());
					}
				}
			}
		}
		break;
	default:
		break;
	}
}

void Application::MatchHandle(SMessageTypeFlags_ _stype, const char* msg)
{
#ifdef DEBUG
	std::cout << "Match message handle: " << msg << std::endl;
#endif // DEBUG
	std::vector<std::string> msgs;
	ParserMsg(msgs, msg, "_");

	if (msgs.size() == 2)
	{
		switch (_stype)
		{
		case SMessageTypeFlags_UserToMatch_SUC:		/* Get the match message success (from this user to the matched user) */
			bUI.SetMatchSS(MatchStateFlags_SUCCESS, msgs[0].c_str(), msgs[1].c_str());
			break;
		case SMessageTypeFlags_UserToMatch_FAI:		/* Get the match message error. (from this user to the matched user) */
			bUI.SetMatchSS(MatchStateFlags_FAIL);
			if (msgState.uName != msgs[0] && strcmp(msgs[0].c_str(), "") != 0)
				bUI.SetItemOfuUI('1', msgs[0].c_str());
			if (msgState.uName != msgs[1] && strcmp(msgs[1].c_str(), "") != 0)
				bUI.SetItemOfuUI('1', msgs[1].c_str());
			break;
		case SMessageTypeFlags_UserBeMatched:		/* The matched user gets this message */
			{
				MatchStateFlags_ type = bUI.GetMatchState();
				if (type != MatchStateFlags_SUCCESS && type != MatchStateFlags_AGREE)
				{
					bUI.SetMatchSS(MatchStateFlags_NONE, msgs[0].c_str(), msgs[1].c_str());
					bUI.SetIsShowMatchAUI(true);
				}
				else
					bUI.SetIsShowMatchAUI(false);
			}
			break;
		case SMessageTypeFlags_UserIsForMatching:	/* Stand for the user is matching the another user */
			if (msgState.uName != msgs[0].c_str() && strcmp(msgs[0].c_str(), "") != 0)
				bUI.SetItemOfuUI('2', msgs[0].c_str());
			if (msgState.uName != msgs[1].c_str() && strcmp(msgs[1].c_str(), "") != 0)
				bUI.SetItemOfuUI('2', msgs[1].c_str());
			break;
		case SMessageTypeFlags_UserToOnline:		/* Change user's state from matching to online */
			{
				if (msgState.uName != msgs[0])
					bUI.SetItemOfuUI('1', msgs[0].c_str());
			}
			break;
		case SMessageTypeFlags_RejectOrOvertime:	/* the matched user reject to the match message or overtime */
			if (msgState.uName != msgs[0] && strcmp(msgs[0].c_str(), "") != 0)
				bUI.SetItemOfuUI('1', msgs[0].c_str());
			if (msgState.uName != msgs[1] && strcmp(msgs[1].c_str(), "") != 0)
				bUI.SetItemOfuUI('1', msgs[1].c_str());
			if (bUI.GetMatchState() == MatchStateFlags_SUCCESS)
			{
				bUI.SetIsEndTimer(true);
				bUI.SetIsTimerComplete(true);
				bUI.SetMatchSS(MatchStateFlags_REJECT);
				bUI.SetIsMatchAgree(true);
			}
			break;
		case SMessageTypeFlags_MatchedUserAgree:	/* the matched user agree with the match message */
			bUI.SetUAMInfos(1, bUI.GetMatUser());
			usersFunc(true, bUI.GetMatUser());
			if (msgState.uName != msgs[0] && strcmp(msgs[0].c_str(), "") != 0)
				bUI.SetItemOfuUI('3', msgs[0].c_str());
			if (msgState.uName != msgs[1] && strcmp(msgs[1].c_str(), "") != 0)
				bUI.SetItemOfuUI('3', msgs[1].c_str());
			if (bUI.GetMatchState() == MatchStateFlags_SUCCESS)
			{
				bUI.SetIsEndTimer(true);
				bUI.SetIsTimerComplete(true);
				bUI.SetMatchState(MatchStateFlags_AGREE);
				setCurState(UIState::FIGHT);
				// Set MyChess type and EnemyChess type
				fightUISys.SetCType(ChessTypeFlags_White, ChessTypeFlags_Black);
				fightUISys.Clear();
				fightUISys.SetReqInitState();
			}
			break;
		default:
			break;
		}
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
								bUI.AddUserOfuUI('4', msgs[i].c_str());		/* It is for find yourself */
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
			if (msgs.size() != 7)
				return;
			else
				bUI.SetUInfo(msgs[1].c_str(), msgs[2].c_str(), msgs[3].c_str(), msgs[4].c_str(), msgs[5].c_str(), msgs[6].c_str());
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
			// Get the signin user's infos
			bUI.SetUAMInfos(0, msgState.uName.c_str());
			usersFunc(true, msgState.uName.c_str());
		}
		else
		{
			setCurState(UIState::SIGNIN);
			setMsgState(MessageType::TYPE_SIGNIN, msgs[1].c_str());
		}
	}
	else if (msgs_num == 1)
	{
#ifdef DEBUG
		std::cout << "the user " << msgs[0].c_str() << " online" << std::endl;
#endif // DEBUGG
		cUI.SetItemOfuUI('1', msgs[0].c_str());
		bUI.SetItemOfuUI('1', msgs[0].c_str());
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

/*
void Application::MsgSend(const char* msgs, MessageType type)
{
	DATA_PACKAGE dPackage;
	ConsData(msgs, type, dPackage);
	if (cli)
		cli->write((char*)&dPackage, sizeof(dPackage));
}
*/

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
