#include "UI.h"


ChatUISys	cUI;		// Group chat system UI
BattleUISys	bUI;		// User-user or Man-machine match system UI
FightUISys	fightUISys;	// User-user or Man-machine fight system UI

void UI::signup(MSG_STATE& msgState, std::function< void(const char*, const char*, 
	const char*, bool isBackup)> signupFunc)
{
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(500, 150));
	ImGui::Begin("sign in", 0, ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.40f, 0.35f, 0.60f, 0.70f);
	style.WindowRounding = 0.0f;
	ImVec2 itemSpacing = style.ItemSpacing;
	style.ItemSpacing = ImVec2(itemSpacing.x, 15.0f);
	style.FrameRounding = 5.0f;

	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(0, 150));
	ImGui::SetNextWindowSize(ImVec2(500, 350));
	ImGui::Begin("sign in2", 0, ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

	// ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.40f, 0.35f, 0.60f, 0.70f);

	ImGui::BeginChild("", ImVec2(100, 300));
	ImGui::EndChild();
	ImGui::SameLine();

	static char username[20] = { 0 };
	static char password[20] = { 0 };
	static char validate[20] = { 0 };

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
	ImGui::BeginGroup();
		ImGui::TextWrapped("");

		ImGui::TextWrapped("username:"); ImGui::SameLine();
		ImGui::PushItemWidth(180);

		ImGui::InputText("", username, sizeof(username));
		ImGui::TextWrapped("password:"); ImGui::SameLine(); ImGui::InputText("显示密码", password, sizeof(password),
			ImGuiInputTextFlags_Password | ImGuiInputTextFlags_CharsNoBlank);
		ImGui::SameLine(); showHelpMarker("(?)", password);

		ImGui::TextWrapped("validate:"); ImGui::SameLine(); ImGui::InputText("验证密码", validate, sizeof(validate),
			ImGuiInputTextFlags_Password | ImGuiInputTextFlags_CharsNoBlank);
		ImGui::SameLine(); showHelpMarker("(?)", validate);

		ImGui::PopItemWidth();

		ImGui::TextWrapped(""); ImGui::SameLine(70);

		ImGui::TextWrapped("");
		// ImGui::TextWrapped(""); ImGui::SameLine(70);
		if (ImGui::Button("sign up", ImVec2(85, 0)))
		{
			signupFunc(username, password, validate, false);
		}
		ImGui::SameLine(165);
		if (ImGui::Button("back up", ImVec2(85, 0)))
		{
			signupFunc(nullptr, nullptr, nullptr, true);
		}

		ImGui::TextWrapped("");
		showHelpMarker("tips(?)", "display the operator's state"); ImGui::SameLine();
		if (msgState.type == MessageType::TYPE_SIGNUP)
		{
			if (msgState.info)
				ImGui::TextColored(ImVec4(0.8f, 0.0f, 0.0f, 1.0f), msgState.info);
		}
	ImGui::EndGroup();
	ImGui::PopStyleVar();

	ImGui::End();
}

void UI::signin(MSG_STATE& msgState, std::function< void(const char*, const char*, bool isSignup)> signinFunc)
{
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(500, 150));
	ImGui::Begin("sign in", 0, ImGuiWindowFlags_NoMove | 
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
	
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.40f, 0.35f, 0.60f, 0.70f);
	style.WindowRounding = 0.0f;
	ImVec2 itemSpacing = style.ItemSpacing;
	style.ItemSpacing = ImVec2(itemSpacing.x, 15.0f);
	style.FrameRounding = 5.0f;
	
	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(0, 150));
	ImGui::SetNextWindowSize(ImVec2(500, 350));
	ImGui::Begin("sign in2", 0, ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

	// ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.40f, 0.35f, 0.60f, 0.70f);

	ImGui::BeginChild("", ImVec2(100, 300));
	ImGui::EndChild();
	ImGui::SameLine();

	static char username[20] = { 0 };
	static char password[20] = { 0 };
	static bool recordPasswd = false;
	static bool automaticLogin = false;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
	ImGui::BeginGroup();
		ImGui::TextWrapped("");

		ImGui::TextWrapped("username:"); ImGui::SameLine();
		ImGui::PushItemWidth(180);
		
		ImGui::InputText("", username, sizeof(username));
		ImGui::TextWrapped("password:"); ImGui::SameLine(); ImGui::InputText("显示密码", password, sizeof(password), 
			ImGuiInputTextFlags_Password | ImGuiInputTextFlags_CharsNoBlank);
		ImGui::SameLine(); showHelpMarker("(?)", password);
		ImGui::PopItemWidth();

		ImGui::TextWrapped(""); ImGui::SameLine(70);
		ImGui::Checkbox("RecordPasswd", &recordPasswd);

		ImGui::TextWrapped("");
		//ImGui::TextWrapped(""); ImGui::SameLine(70);
		if (ImGui::Button("sign in", ImVec2(85, 0)))
		{
			signinFunc(username, password, false);
		}
		ImGui::SameLine(165);
		if (ImGui::Button("sign up", ImVec2(85, 0)))
		{
			signinFunc(nullptr, nullptr, true);
		}

		ImGui::TextWrapped("");
		showHelpMarker("tips(?)", "display the operator's state"); ImGui::SameLine();
		if (msgState.type == MessageType::TYPE_SIGNIN)
		{
			if (msgState.info)
				ImGui::TextColored(ImVec4(0.8f, 0.0f, 0.0f, 1.0f), msgState.info);
		}
	ImGui::EndGroup();
	ImGui::PopStyleVar();

	ImGui::End();
}

void UI::mainUI(MSG_STATE& msgState, std::function< void(unsigned char, bool)> mainUIFunc, std::function< void(const char*)> chatFunc, 
	std::function< void(bool, const char*)> usersFunc, std::function< void(bool, const char*)> matchFunc,
	std::function< void(int, const char*)> matchAgreeFunc)
{
	ImGui::SetNextWindowPos(ImVec2(-10, -10));
	ImGui::SetNextWindowSize(ImVec2(820, 160));
	ImGui::Begin("sign in", 0, ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.40f, 0.35f, 0.60f, 0.70f);
	style.WindowRounding = 0.0f;
	ImVec2 itemSpacing = style.ItemSpacing;
	style.ItemSpacing = ImVec2(itemSpacing.x, 15.0f);
	style.FrameRounding = 5.0f;

	ImGui::End();

	ImGui::SetNextWindowPos(ImVec2(0, 150));
	ImGui::SetNextWindowSize(ImVec2(800, 650));
	ImGui::Begin("sign in2", 0, ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);

	// ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.40f, 0.35f, 0.60f, 0.70f);

	ImGui::BeginChild("", ImVec2(100, 300));
	ImGui::EndChild();
	ImGui::SameLine();

	static bool show_app_about = false;
	static bool isExit = false;
	static bool show_chatUI = false;
	// static bool show_MatchUI = false;

	ImGui::BeginGroup();
		ImGui::TextWrapped(""); ImGui::SameLine(175);
		if (ImGui::Button("Man-machine battle", ImVec2(200, 0)))
		{
			mainUIFunc(1, false);
			fightUISys.Clear();
		}
		ImGui::TextWrapped(""); 
		ImGui::TextWrapped(""); ImGui::SameLine(175);
		if (ImGui::Button("User-user battle", ImVec2(200, 0)))
		{
			// show_MatchUI ^= true;
			mainUIFunc(2, false);
			ImGui::OpenPopup("UMatch");
			// For matchUI, to get user's information
			static bool isFirst = true;
			if (isFirst)
			{
				isFirst = false;
				usersFunc(false, nullptr);
			}
		}
		matchUI("UMatch", usersFunc , matchFunc);	// For User-user battle's match system
		ImGui::TextWrapped("");
		ImGui::TextWrapped(""); ImGui::SameLine(175);
		if (ImGui::Button("Group chat system", ImVec2(200, 0)))
		{
			show_chatUI ^= true;
			mainUIFunc(3, false);
		}
		ImGui::TextWrapped("");
		ImGui::TextWrapped(""); ImGui::SameLine(175);
		if (ImGui::Button("About ImGui-sfml game", ImVec2(200, 0)))
		{
			show_app_about ^= true;
			mainUIFunc(4, false);
		}
		ImGui::TextWrapped("");
		ImGui::TextWrapped(""); ImGui::SameLine(175);
		if (ImGui::Button("Exit", ImVec2(200, 0)))
		{
			isExit ^= true;
			mainUIFunc(5, false);
		}
		ImGui::TextWrapped("");
		ImGui::TextWrapped("");
		ImGui::TextWrapped(""); ImGui::SameLine(115);
		if (msgState.type == MessageType::TYPE_NONE)
		{
			if (msgState.info)
				showHelpMarker("tips(?)", msgState.info); 
			else
				showHelpMarker("tips(?)", "display the operator's state");
		}
		else
			showHelpMarker("tips(?)", "display the operator's state"); 
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.8f, 0.0f, 0.0f, 1.0f), "Choosing one of buttons from above and presssing!");
	ImGui::EndGroup();

	if (show_app_about)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
		// style.WindowRounding = 8.0f;
		ImGui::Begin("About ImGui-sfml game", &show_app_about, ImGuiWindowFlags_NoResize);
		ImGui::BulletText("ImGui:https://github.com/ocornut/imgui, An implementation method \nof ImGui(Dear ImGui)");
		ImGui::BulletText("ImGui-sfml:https://github.com/eliasdaler/imgui-sfml, this website \nis to introduce using sfml binding ImGui.");
		ImGui::BulletText("About this game:We will achieve WHITE && BLACK game by ImGui-sfml.\nhttps://github.com/SmallHedgehog/sfml_imgui_game");
		ImGui::End();
		ImGui::PopStyleVar();
	}

	if (isExit)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
		// style.WindowRounding = 8.0f;
		ImGui::Begin("IsExited?", &isExit, ImGuiWindowFlags_NoResize);
		ImGui::TextWrapped("");
		ImGui::TextWrapped("\tAre you really exit the game?");
		ImGui::TextWrapped("");
		ImGui::TextWrapped(""); ImGui::SameLine(50);
		if (ImGui::Button("YES", ImVec2(50, 0)))
		{
			mainUIFunc(5, true);
		}
		ImGui::SameLine(150);
		if (ImGui::Button("NO", ImVec2(50, 0)))
		{
			mainUIFunc(5, false);
			isExit ^= true;
		}
		ImGui::End();
		ImGui::PopStyleVar();
	}

	if (show_chatUI)
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.00f, 0.00f, 0.00f, 0.70f));
		// style.WindowRounding = 8.0f;
		chatUI(msgState, &show_chatUI, chatFunc, usersFunc);
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
	}

	if (bUI.GetIsShowMatchAUI())
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.20f, 0.05f, 0.20f, 0.70f));
		bUI.DrawMatchAgreeUI(matchAgreeFunc);
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
	}

	if (bUI.GetIsMatchAgree())
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.20f, 0.05f, 0.20f, 0.70f));
		ImGui::OpenPopup("reject");
		ImGui::SetNextWindowSize(ImVec2(200, 150));
		if (ImGui::BeginPopupModal("reject", 0, ImGuiWindowFlags_NoResize))
		{
			ImGui::NewLine();
			ImGui::NewLine(); ImGui::SameLine(20);
			ImGui::TextWrapped("Refused you request!!!");
			ImGui::NewLine();
			ImGui::NewLine();
			ImGui::SameLine(80);
			if (ImGui::Button("close"))
				bUI.SetIsMatchAgree(false);
			ImGui::EndPopup();
		}
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
	}

	ImGui::End();
}

void UI::FightUI(FightTypeFlags_ ftype, std::function< void(unsigned char, const char*)> fightFunc)
{
	// ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.250f, 0.10f, 0.250f, 1.0f));
	// Draw fight ui system
	switch (ftype)
	{
	case FightTypeFlags_UserToUser:
		fightUISys.fightFunc = fightFunc;
		fightUISys.DrawFightUserToUserUI(fightFunc);
		break;
	case FightTypeFlags_UserToMachine:
		fightUISys.DrawFightUserToMachineUI(fightFunc);
		break;
	}
	// ImGui::PopStyleVar();
	ImGui::PopStyleColor();
}

void UI::chatUI(MSG_STATE& msgState, bool* p_open, std::function< void(const char*)> chatFunc, 
	std::function< void(bool, const char*)> usersFunc)
{
	// static ChatUISys cUI;
	cUI.Draw(msgState, "Group chat system", p_open, chatFunc, usersFunc);
}

void UI::matchUI(const char* title, std::function< void(bool, const char*)> usersFunc, std::function< void(bool, const char*)> matchFunc)
{
	// Using BattleUISys class for matchUI
	bUI.DrawMatchUI(title, usersFunc, matchFunc);
}

void UI::init()
{
	// font
	// ImGui::GetIO().FontGlobalScale = 1.4f;
}

void UI::showHelpMarker(const char* desc, const char* text)
{
	ImGui::TextDisabled(desc);
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(450.0f);
		ImGui::TextUnformatted(text);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void UI::showHelpMarker(const char* desc, std::vector<std::string>& _val)
{
	static const char* type[5] = { "level", "exper", "wined", "losed", "escap" };

	ImGui::TextDisabled(desc);
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(450.0f);
		if (!_val.empty() && _val.size() == 6)
		{
			int i = 0;
			for (std::vector<std::string>::iterator it = _val.begin() + 1; it != _val.end(); ++it)
			{
				std::string item = std::string(type[i]) + ": " + (*it);
				ImGui::TextWrapped(item.c_str());
				++i;
			}
		}
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

//------------------------------------------------------------------------------------
// Group chat system
// ChatUISys include UsersUISys
//------------------------------------------------------------------------------------

/*
int ChatUISys::Stricmp(const char* str1, const char* str2)
{
	int sign;
	while ((sign = toupper(*str2) - toupper(*str1)) == 0 && *str1)
	{
		str1++;
		str2++;
	}
	return sign;
}

int ChatUISys::Strnicmp(const char* str1, const char* str2, int n)
{
	int sign;
	while (n > 0 && (sign = toupper(*str2) - toupper(*str1)) == 0 && *str1)
	{
		str1++;
		str2++;
		n--;
	}
	return sign;
}

char* ChatUISys::Strdup(const char* str)
{
	size_t len = strlen(str) + 1;
	void* buff = malloc(len);
	return (char*)memcpy(buff, (const void*)str, len);
}
*/

void ChatUISys::ParserText(std::vector<std::string>& vec, const char* text)
{
	std::string info(text);

	size_t last = 0;
	size_t index = info.find_first_of('_', last);
	if (index != std::string::npos)
	{
		vec.push_back(info.substr(last, index - last));
		last = index + 1;
	}

	vec.push_back(info.substr(last, info.size() - last));
}

void ChatUISys::Clear()
{
	for (int i = 0; i < Items.Size; ++i)
		free(Items[i]);
	for (int i = 0; i < Users.Size; ++i)
		free(Users[i]);
	Items.clear();
	Users.clear();
	Color.clear();
}

void ChatUISys::AddUser(const char* user)
{
	char buff[64] = { 0 };

	float time = ImGui::GetTime();
	sprintf(buff, "%s  %.2f", user, time);
	Users.push_back(Strdup(buff));
}

void ChatUISys::AddLog(const char color, const char* user, const char* fmt, ...)
{
	char buff[1024] = { 0 };

	va_list args;
	va_start(args, fmt);
	vsnprintf(buff, IM_ARRAYSIZE(buff), fmt, args);
	buff[IM_ARRAYSIZE(buff) - 1] = 0;
	va_end(args);
	Color.push_back(color);
	AddUser(user);
	Items.push_back(Strdup(buff));
	ScrollToBottom = true;
}

void ChatUISys::AddUserOfuUI(const char isOnline, const char* user)
{
	uUI.AddItem(isOnline, user);
}

void ChatUISys::SetItemOfuUI(const char isOnline, const char* user)
{
	uUI.SetItemIsOnline(isOnline, user);
}

void ChatUISys::Draw(MSG_STATE& msgState, const char* title, bool* p_open, std::function< void(const char*)> chatFunc,
	std::function< void(bool, const char*)> usersFunc)
{
	static char text[1024] = { 0 };
	static bool isShowUsers = false;
	static char* options[] = { "Enter", "Ctrl+Enter" };
	static int	optState = 0;

	ImGui::SetNextWindowSize(ImVec2(600, 800), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin(title, p_open))
	{
		ImGui::End();
		return;
	}
	
	bool isPressed = false;
	if (ImGui::Button("USERS", ImVec2(55, 0)))
	{
		isShowUsers ^= true;
		isPressed = true;
	}
	// Judge userUI whether open or not by per-frame, we will update user's online information
	if (isPressed && isShowUsers)
	{
		uUI.Clear();
		usersFunc(false, nullptr);
	}

	ImGui::SameLine(); UI::showHelpMarker("(?)", "display all users"); ImGui::SameLine();
	if (ImGui::Button("CLEAR", ImVec2(55, 0))) Clear();
	ImGui::SameLine(); UI::showHelpMarker("(?)", "clear the messages"); ImGui::SameLine();
	Filter.Draw("FIND");
	ImGui::SameLine(); UI::showHelpMarker("(?)", "find the messages");

	ImGui::Separator();

	ImGui::BeginChild("messages", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing() - ImGui::GetTextLineHeight() * 10), false,
		ImGuiWindowFlags_HorizontalScrollbar);
	if (ImGui::BeginPopupContextWindow())
	{
		if (ImGui::Selectable("clear")) Clear();
		ImGui::EndPopup();
	}
	
	for (int i = 0; i < Items.Size; ++i)
	{
		const char* item = Items[i];
		const char* user = Users[i];
		const char color = Color[i];
		if (!Filter.PassFilter(item) && !Filter.PassFilter(user))
			continue;

		// color
		ImVec4 col = ImVec4(0.4f, 1.0f, 0.4f, 1.0f);
		if (color == 0)
			col = ImVec4(0.4f, 0.4f, 1.0f, 1.0f);

		// user
		ImGui::PushStyleColor(ImGuiCol_Text, col);
		ImGui::TextWrapped(user);
		ImGui::PopStyleColor();

		// text
		ImGui::NewLine(); ImGui::SameLine(10);
		ImGui::TextWrapped(item);
	}

	if (ScrollToBottom)
		ImGui::SetScrollHere();
	ScrollToBottom = false;
	ImGui::EndChild();

	ImGui::Separator();

	if (ImGui::Button("image button"))
	{
		//
	}
	ImGui::SameLine();
	if (ImGui::Button("options"))
		ImGui::OpenPopup("send message options");
	if (ImGui::BeginPopup("send message options"))
	{
		for (int i = 0; i < IM_ARRAYSIZE(options); ++i)
		{
			if (ImGui::Selectable(options[i]))
				optState = i;
		}
		ImGui::EndPopup();
	}
	if (optState == 0)
		ImGui::InputTextMultiline("input_messages", text, IM_ARRAYSIZE(text), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 5), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AllowTabInput);
	else
		ImGui::InputTextMultiline("input_messages", text, IM_ARRAYSIZE(text), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 5), ImGuiInputTextFlags_AllowTabInput);
	ImGui::NewLine();
	ImGui::SameLine(ImGui::GetWindowWidth() / 5.0f * 4 + 9);
	if (ImGui::Button("close", ImVec2(60, 0)))
	{
		*p_open ^= true;
	}
	ImGui::SameLine();
	if (ImGui::Button("send", ImVec2(60, 0)))
	{
		char* input_end = text + strlen(text);
		while (input_end > text && input_end[-1] == ' ')
		{
			input_end--;
		}
		*input_end = 0;
		if (text[0])
		{
			AddLog(1, msgState.uName.c_str(), text);
			chatFunc(text);
		}
		strcpy(text, "");
	}
	// Draw Users
	if (isShowUsers)
	{
		uUI.Draw("Users", &isShowUsers, usersFunc);
	}

	ImGui::End();
}


//--------------------------------------------------------------------------------------
// UsersUISys
//--------------------------------------------------------------------------------------

void UsersUISys::Clear()
{
	for (int i = 0; i < Users.Size; ++i)
		free(Users[i]);
	Users.clear();
	IsOnline.clear();
}

void UsersUISys::SetItemIsOnline(const char isOnline, const char* user)
{
	for (int i = 0; i < Users.Size; ++i)
	{
		if (strcmp(Users[i], user) == 0)
		{
			if (IsOnline[i] != isOnline)
				IsOnline[i] = isOnline;
			return;
		}
	}
	// not find in Users, is a new user
	AddItem(isOnline, user);
}

void UsersUISys::AddItem(const char isOnline, const char* user)
{
	char buff[64] = { 0 };

	sprintf(buff, user);
	Users.push_back(ChatUISys::Strdup(buff));
	IsOnline.push_back(isOnline);
	ScrollToBottom = true;
}

void UsersUISys::Draw(const char* title, bool* p_open, std::function< void(bool, const char*)> usersFunc)
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15f, 0.10f, 0.20f, 0.70f));

	ImGui::SetNextWindowSize(ImVec2(220, 495));
	if (!ImGui::Begin(title, p_open, ImGuiWindowFlags_NoResize))
	{
		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
		return;
	}
	Filter.Draw("find", 80.0f);
	ImGui::SameLine(); UI::showHelpMarker("(?)", "find the user"); ImGui::SameLine();
	if (ImGui::Button("refresh"))
	{
		Clear();
		usersFunc(false, nullptr);
	}
	// ImGui::SameLine(); UI::showHelpMarker("(?)", "refresh the users");

	// ImGui::Separator();

	ImGui::BeginChild("AllUsers", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
	// Draw users
	for (int i = 0; i < Users.Size; ++i)
	{
		const char* user = Users[i];
		const char isOnline = IsOnline[i];
		if (!Filter.PassFilter(user))
			continue;
		// is onlined
		const char* IsOnlined = "[Ionline]";
		if (isOnline == '0')
			IsOnlined = "[Nonline]";

		ImGui::PushItemWidth(20);
		ImGui::TextUnformatted(IsOnlined);
		ImGui::PopItemWidth();
		ImGui::SameLine(); ImGui::TextUnformatted(user);
	}
	if (ScrollToBottom)
		ImGui::SetScrollHere();
	ScrollToBottom = false;
	ImGui::EndChild();

	ImGui::End();
	ImGui::PopStyleVar();
	ImGui::PopStyleColor();
}

void UsersUISys::Draw(const char* title, std::function< void(bool, const char*)> userFunc, std::function< void(bool, const char*)> matchFunc)
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.00f, 0.00f, 0.00f, 0.70f));

	ImGui::SetNextWindowSize(ImVec2(260, 495));
	if (ImGui::BeginPopupModal(title, NULL, ImGuiWindowFlags_NoResize))
	{
		Filter.Draw("find", 100.0f);
		ImGui::SameLine(); UI::showHelpMarker("(?)", "find the user"); ImGui::SameLine();
		if (ImGui::Button("refresh"))
		{
			/*
			Clear();
			userFunc(false, nullptr);
			*/
		}

		ImGui::BeginChild("AllUsersOfMatch", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing() - ImGui::GetTextLineHeight() / 2), 
			true, ImGuiWindowFlags_HorizontalScrollbar);
		// Draw users
		for (int i = 0; i < Users.Size; ++i)
		{
			const char* user = Users[i];
			const char isOnline = IsOnline[i];
			if (!Filter.PassFilter(user))
				continue;

			if (isOnline == '0')
				ImGui::TextUnformatted("NotOnline");
			else if (isOnline == '1')
			{
				static bool isButtonDown = false;
				std::string bID = "Match " + std::string(user);
				if (ImGui::Button(bID.c_str(), ImVec2(130, 0)))
				{
					// Match this user to fight
					matchFunc(true, user);
					isButtonDown = true;
				}
				std::string oId = std::string(user) + " Wait for match";
				if (MatchStateFlags_SUCCESS == bUI.GetMatchState() && isButtonDown)
				{
					ImGui::OpenPopup(oId.c_str());
					// Start timer
					// bUI.SetIsTimerComplete(false);
					static Timer timer(std::bind(&BattleUISys::SetIsTimerComplete, &bUI, std::placeholders::_1),
						std::bind(&BattleUISys::SetVClock, &bUI, std::placeholders::_1), std::bind(&BattleUISys::EndTimer, &bUI));
					bUI.SetIsEndTimer(false);
					timer.BEGIN_NegTimer(15, 0);
					isButtonDown = false;
				}
				ImGui::SetNextWindowSize(ImVec2(150, 120));
				if (ImGui::BeginPopupModal(oId.c_str(), NULL, ImGuiWindowFlags_NoResize))
				{
					// To show match wait ui
					bUI.DrawMatchWaitUI();
					if (bUI.GetIsTimerComplete())
					{
						matchFunc(false, bUI.GetMatUser());
						bUI.SetMatchState(MatchStateFlags_FAIL);
						bUI.SetIsTimerComplete(false);
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}
			}
			else if (isOnline == '2')
			{
				ImGui::TextUnformatted("ForUMatch");	// is matching user
			}
			else if (isOnline == '3')
			{
				ImGui::TextUnformatted("IsMatched");	// is matched
			}
			else
				ImGui::TextUnformatted("IYourself");	// is yourself
			ImGui::SameLine(150);
			std::string uID = std::string(user) + "'s infos";
			if (ImGui::Button(uID.c_str(), ImVec2(130, 0)))
			{
				// Get the user's information
				userFunc(true, user);
				ImGui::OpenPopup(user);
			}
			bool isClose = true;
			ImGui::SetNextWindowSize(ImVec2(300, 300));
			if (ImGui::BeginPopupModal(user, &isClose, ImGuiWindowFlags_NoResize))
			{
				// For show the user's information
				DrawForUserofInfo();
				ImGui::EndPopup();
			}
		}
		if (ScrollToBottom)
			ImGui::SetScrollHere();
		ScrollToBottom = false;
		ImGui::EndChild();

		ImGui::TextWrapped(""); ImGui::SameLine(30);
		if (ImGui::Button("OK", ImVec2(80, 0)))
			ImGui::CloseCurrentPopup();
		ImGui::SameLine(140);
		if (ImGui::Button("Cancel", ImVec2(80, 0)))
			ImGui::CloseCurrentPopup();

		ImGui::EndPopup();
	}

	ImGui::PopStyleVar();
	ImGui::PopStyleColor();
}

void UsersUISys::DrawForUserofInfo()
{
	static unsigned char sign = 0;

	if (ImGui::Button("Text", ImVec2(88, 0)))
		sign = 0;
	ImGui::SameLine();
	if (ImGui::Button("Histogram", ImVec2(88, 0)))
		sign = 1;
	ImGui::SameLine();
	if (ImGui::Button("PieChart", ImVec2(88, 0)))
		sign = 2;

	switch (sign)
	{
	case 0:
		// Only show user's information using normal text
		{
			ImGui::Columns(2, "UserInfoOfText");
			ImGui::Separator();
			ImGui::Text("type"); ImGui::NextColumn();
			ImGui::Text("value"); ImGui::NextColumn();
			ImGui::Separator();
			static char* type[5] = { "level", "exper", "wined", "losed", "escap" };
			// For get user's information
			UINT infos[5] = { 0 };
			bUI.GetUInfo(infos);
			/* static int selected = -1; */
			for (int i = 0; i < 5; ++i)
			{
				/*
				if (ImGui::Selectable(type[i], selected == i, ImGuiSelectableFlags_SpanAllColumns))
					selected = i;
				*/
				ImGui::Text(type[i]); ImGui::NextColumn();
				char buff[20] = { 0 };
				_itoa_s(infos[i], buff, sizeof(buff), 10);
				ImGui::Text(buff); ImGui::NextColumn();
			}
			ImGui::Columns(1);
			ImGui::Separator();
		}
		break;
	case 1:
		// Only show user's information using histogram
		ImGui::TextUnformatted("histogram");
		break;
	case 2:
		// Only show user's information using pie-chart
		ImGui::TextUnformatted("pie-chart");
		break;
	default:
		break;
	}
}

//------------------------------------------------------------------------------------
// User-user or Man-machine battle system.
// Each system of Battle system contains matchUI and fightUI.
//------------------------------------------------------------------------------------

void BattleUISys::DrawMatchWaitUI()
{
	char buff[8] = { 0 };
	_itoa_s(bUI.GetVClock(), buff, sizeof(buff), 10);

	ImGui::NewLine();
	ImGui::NewLine();
	// ImGui::NewLine();
	ImGui::SameLine(70); ImGui::TextUnformatted(buff);
}

void BattleUISys::DrawMatchAgreeUI(std::function< void(int, const char*)> matchAgreeFunc)
{
	matchWaitAgree = matchAgreeFunc;

	ImGui::OpenPopup("MatchAgree");
	ImGui::SetNextWindowSize(ImVec2(300, 300));
	if (ImGui::BeginPopupModal("MatchAgree", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
	{
		static Timer timer(std::bind(&BattleUISys::SetMatchATimerComplete, &bUI, std::placeholders::_1),
			std::bind(&BattleUISys::SetVClock, &bUI, std::placeholders::_1), std::bind(&BattleUISys::EndTimer, &bUI));
		if (timer.GetIsTimerEnd())
		{
			SetIsEndTimer(false);
			timer.BEGIN_NegTimer(10, 0);
		}
		std::string text("  ");
		text += std::string(matchSS.ReqUser) + " send battle requests to you, \nDo you agree with it?";
		ImGui::NewLine();
		ImGui::TextUnformatted(text.c_str());

		ImGui::NewLine();
		ImGui::NewLine();
		ImGui::NewLine();
		char buff[8] = { 0 };
		_itoa_s(bUI.GetVClock(), buff, sizeof(buff), 10);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.4f, 0.05f, 0.10f, 1.0f));
		ImGui::SameLine(140); ImGui::TextWrapped(buff);
		ImGui::PopStyleColor();

		ImGui::NewLine();
		ImGui::NewLine();
		ImGui::NewLine();
		// ImGui::NewLine();
		ImGui::SameLine(50);
		if (ImGui::Button("Agree", ImVec2(90, 0)))
		{
			matchAgreeFunc(1, bUI.GetMatUser());
			ImGui::CloseCurrentPopup();
			SetIsShowMatchAUI(false);
			// End timer
			timer.SetTimerEnd(true);
		}
		ImGui::SameLine(150);
		if (ImGui::Button("Reject", ImVec2(90, 0)))
		{
			matchAgreeFunc(2, bUI.GetMatUser());
			ImGui::CloseCurrentPopup();
			SetIsShowMatchAUI(false);
			// End timer
			timer.SetTimerEnd(true);
		}
		// Timer is complete
		if (isTimerAComplete)
		{
			matchAgreeFunc(3, bUI.GetMatUser());
			SetIsShowMatchAUI(false);
			SetMatchATimerComplete(false);
			ImGui::CloseCurrentPopup();
			// End timer
			timer.SetTimerEnd(true);
		}

		ImGui::EndPopup();
	}
}

void BattleUISys::GetUInfo(const char* user, UINT* infos)
{
	user = uInfo.uName;
	GetUInfo(infos);
}

void BattleUISys::GetUInfo(UINT* infos)
{
	infos[0] = uInfo.level;
	infos[1] = uInfo.exper;
	infos[2] = uInfo.wined;
	infos[3] = uInfo.losed;
	infos[4] = uInfo.escap;
}

void BattleUISys::SetUInfo(const char* user, const char* level, const char* exper, const char* wined, const char* losed,
	const char* escap)
{
	if (!userInfos.empty())
	{
		if (userInfos[0] == std::string(user))
		{
			userInfos.clear();
			userInfos.push_back(user);
			userInfos.push_back(level);
			userInfos.push_back(exper);
			userInfos.push_back(wined);
			userInfos.push_back(losed);
			userInfos.push_back(escap);
		}
	}
	if (!matchUserInfos.empty())
	{
		if (matchUserInfos[0] == std::string(user))
		{
			matchUserInfos.clear();
			matchUserInfos.push_back(user);
			matchUserInfos.push_back(level);
			matchUserInfos.push_back(exper);
			matchUserInfos.push_back(wined);
			matchUserInfos.push_back(losed);
			matchUserInfos.push_back(escap);
		}
	}

	int le = atoi(level);
	int ex = atoi(exper);
	int wi = atoi(wined);
	int lo = atoi(losed);
	int es = atoi(escap);

	SetUInfo(user, le, ex, wi, lo, es);
}

void BattleUISys::SetUInfo(const char* user, UINT level, UINT exper, UINT wined, UINT losed, UINT escap)
{
	memset(&uInfo, 0, sizeof(USERINFO));

	strcpy(uInfo.uName, user);
	uInfo.level = level;
	uInfo.exper = exper;
	uInfo.wined = wined;
	uInfo.losed = losed;
	uInfo.escap = escap;
}

void BattleUISys::SetUAMInfos(unsigned char type, const char* username)
{
	if (type == 0)
	{
		userInfos.clear();
		userInfos.push_back(username);
	}
	else
	{
		matchUserInfos.clear();
		matchUserInfos.push_back(username);
	}
}

//-----------------------------------------------------------------------------------------------------------------------------
// In order to make code clearer, we will achieve fight system in another class(FightUISys), BattleUISys Only achieve match
// function actually. FightUISys contains chat and fight model.
// The chat model is to achieve communicate with two users, the fight model is to achieve game function.
//-----------------------------------------------------------------------------------------------------------------------------

void UChat::Clear()
{
	for (int i = 0; i < Items.Size; ++i)
		free(Items[i]);
	for (int i = 0; i < Items.Size; ++i)
		free(Users[i]);
	Items.clear();
	Type.clear();
	ScrollToBottom = true;
}

void UChat::AddItem(char type, const char* fmt, const char* user)
{
	std::lock_guard<std::mutex> lock(_mutex);

	char buff[1024] = { 0 };
	sprintf_s(buff, 1024, fmt);
	Items.push_back(ChatUISys::Strdup(buff));
	Type.push_back(type);
	memset(buff, 0, sizeof(buff));
	sprintf_s(buff, 1024, user);
	Users.push_back(ChatUISys::Strdup(buff));
	ScrollToBottom = true;
}

void UChat::DrawUChatOfUM(const char* title)
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowTitleAlign = ImVec2(0.50f, 0.50f);

	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSize(ImVec2(370.0f, 1050.0f));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.20f, 0.05f, 0.20f, 0.80f));
	if (!ImGui::Begin(title, 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
	{
		ImGui::End();
		return;
	}
	DrawCommonModule(title);
	ImGui::Separator();

	// Input text
	ImGui::PushItemWidth(285);
	if (ImGui::InputText("", InputBuf, IM_ARRAYSIZE(InputBuf), ImGuiInputTextFlags_EnterReturnsTrue))
	{
		char* input_end = InputBuf + strlen(InputBuf);
		while (input_end > InputBuf && input_end[-1] == ' ')
		{
			input_end--;
		}
		*input_end = 0;
		if (InputBuf[0])
		{
			std::string curTime = GetCurTime();
			std::string UserATime = bUI.GetUersInfos()[0] + "\t" + curTime;
			AddItem(1, InputBuf, UserATime.c_str());
			std::string robotATime = "robot\t" + curTime;
			AddItem(0, "I am a robot!", robotATime.c_str());
		}
		strcpy(InputBuf, "");
	}
	ImGui::PopItemWidth();
	ImGui::SameLine();
	if (ImGui::Button("Send", ImVec2(60, 0))) {}

	ImGui::End();
	ImGui::PopStyleColor();
}

void UChat::DrawUChatOfUU(const char* title, std::function< void(unsigned char, const char*)> chatFunc)
{
	ImGuiStyle& style = ImGui::GetStyle();
	style.WindowTitleAlign = ImVec2(0.50f, 0.50f);

	ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetNextWindowSize(ImVec2(370.0f, 1050.0f));
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.20f, 0.05f, 0.20f, 0.80f));
	if (!ImGui::Begin(title, 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
	{
		ImGui::End();
		return;
	}
	DrawCommonModule(title);
	ImGui::Separator();

	// Input text
	ImGui::PushItemWidth(285);
	if (ImGui::InputText("", InputBuf, IM_ARRAYSIZE(InputBuf), ImGuiInputTextFlags_EnterReturnsTrue))
	{
		char* input_end = InputBuf + strlen(InputBuf);
		while (input_end > InputBuf && input_end[-1] == ' ')
		{
			input_end--;
		}
		*input_end = 0;
		if (InputBuf[0])
		{
			std::string UserATime = bUI.GetUersInfos()[0] + "\t" + GetCurTime();
			AddItem(1, InputBuf, UserATime.c_str());
			chatFunc(2, InputBuf);
		}
		strcpy(InputBuf, "");
	}
	ImGui::PopItemWidth();
	ImGui::SameLine();
	if (ImGui::Button("Send", ImVec2(60, 0))) {}

	ImGui::End();
	ImGui::PopStyleColor();
}

void UChat::DrawCommonModule(const char* title)
{
	if (ImGui::Button("Clear", ImVec2(60, 0))) Clear();
	ImGui::SameLine();
	Filter.Draw("Find", 250);
	ImGui::Separator();

	ImGui::BeginChild("Chat content", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing() * 2), false, ImGuiWindowFlags_HorizontalScrollbar);
	if (ImGui::BeginPopupContextWindow())
	{
		if (ImGui::Selectable("Clear")) Clear();
		ImGui::EndPopup();
	}
	// Draw text
	for (int i = 0; i < Items.Size; ++i)
	{
		const char* item = Items[i];
		const char	type = Type[i];
		const char* user = Users[i];
		if (!Filter.PassFilter(item))
			continue;
		// color
		ImVec4 col = ImVec4(0.4f, 1.0f, 0.4f, 1.0f);
		if (type == 0)
			col = ImVec4(0.4f, 0.4f, 1.0f, 1.0f);
		// user
		ImGui::PushStyleColor(ImGuiCol_Text, col);
		ImGui::TextWrapped(user);
		ImGui::PopStyleColor();
		// text
		ImGui::NewLine(); ImGui::SameLine(10);
		ImGui::TextWrapped(item);
	}
	if (ScrollToBottom)
		ImGui::SetScrollHere();
	ScrollToBottom = false;
	ImGui::EndChild();
}

void FightUISys::DrawFightUserToMachineUI(std::function< void(unsigned char, const char*)> fightFunc)
{
	chatSys.DrawUChatOfUM("Chat module(User-Machine)");

	ImGui::SetNextWindowPos(ImVec2(370.0f, 0.0f));
	ImGui::SetNextWindowSize(ImVec2(1050.0f, 1050.0f));
	if (!ImGui::Begin("Fight module(User-User)", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoTitleBar))
	{
		ImGui::End();
		return;
	}

	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
	ImGui::NewLine(); ImGui::NewLine(); ImGui::SameLine(520);
	ImGui::TextWrapped("[robot]"); ImGui::NewLine(); ImGui::SameLine(520);
	UI::showHelpMarker("User's infos(?)", "I am a robot!"); ImGui::NewLine();
	for (int i = 1; i < 25; ++i)
		ImGui::NewLine();
	ImGui::NewLine(); ImGui::SameLine(35);
	if (!isStartGame)
	{
		if (ImGui::Button("start", ImVec2(60, 0)))
		{
			isStartGame = true;
			isCanDrag	= true;
			SetIsTrunsYou(true);
		}
	}
	else
	{
		if (ImGui::Button("reset", ImVec2(60, 0)))
			ReSet();
	}
	ImGui::NewLine(); ImGui::SameLine(35);
	if (ImGui::Button("back", ImVec2(60, 0)))
	{
		//
	}
	ImGui::NewLine(); ImGui::NewLine(); ImGui::SameLine(35);
	if (ImGui::Button("exit", ImVec2(60, 0)))
	{
		isCanDrag = false;
		ImGui::OpenPopup("IsExited??");
	}
	ImGui::SetNextWindowSize(ImVec2(255.0f, 160.0f));
	if (ImGui::BeginPopupModal("IsExited??", 0, ImGuiWindowFlags_NoResize))
	{
		ImGui::NewLine();
		ImGui::TextWrapped("\tAre you really exit the game?");
		ImGui::NewLine(); ImGui::NewLine(); ImGui::SameLine(50);
		isCanDrag = false;
		if (ImGui::Button("YES", ImVec2(50, 0)))
		{
			if (isStartGame)
				isCanDrag = true;
			fightFunc(1, nullptr);
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine(150);
		if (ImGui::Button("NO", ImVec2(50, 0)))
		{
			if (isStartGame)
				isCanDrag = true;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	std::vector<std::string> userInfos = bUI.GetUersInfos();
	ImGui::NewLine(); ImGui::NewLine(); ImGui::NewLine(); ImGui::SameLine(520);
	if (userInfos.empty())
		ImGui::TextWrapped("[stranger]");
	else
	{
		std::string user = "[" + userInfos[0] + "]";
		ImGui::TextWrapped(user.c_str());
	}
	ImGui::NewLine();
	if (isStartGame && isCanDrag && GetIsTrunsYou())
	{
		static Timer timer(std::bind(&FightUISys::IsTimerComplete, &fightUISys, std::placeholders::_1), std::bind(&FightUISys::SetWaitValue, &fightUISys, std::placeholders::_1),
			std::bind(&FightUISys::EndTimer, &fightUISys));
		if (timer.GetIsTimerEnd())
		{
			SetIsEndTimer(false);
			timer.BEGIN_NegTimer(15, 0);
		}

		char buff[8] = { 0 };
		_itoa_s(fightUISys.GetWaitValue(), buff, sizeof(buff), 10);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.65f, 0.70f, 1.0f));
		std::string text = "Wait: " + std::string(buff) + "s";
		ImGui::SameLine(320); ImGui::TextWrapped(text.c_str());
		ImGui::PopStyleColor();
	}
	ImGui::SameLine(520); UI::showHelpMarker("User's inofs(?)", userInfos);
	ImGui::PopStyleVar();

	DrawFightCommonUI(draw_list);

	ImGui::End();

	if (isStartGame && IsWin)
		DrawWinOrLose(true);
	else if (isStartGame && IsFail)
		DrawWinOrLose(false);

	if (isStartGame && isCanDrag && !GetIsTrunsYou())
	{
		if (countChess >= (LINE_NUMS * LINE_NUMS))
			isCanDrag = false;
		else
		{
			static AI ai;
			Point pos;
			ai.AICalc(ChessTypeFlags_White, ChessTypeFlags_Black, cLayout, Point((chesses.back().cPos.x - BLINE_LEFT_X) / LINE_SPACE,
				(chesses.back().cPos.y - BLINE_UP_Y) / LINE_SPACE), pos);
			AddChess(CHESS(2, ImVec2(BLINE_LEFT_X + pos.xPos * LINE_SPACE, BLINE_UP_Y + pos.yPos * LINE_SPACE)));
			cLayout[pos.xPos][pos.yPos] = 2;
			AddChessCount();
			SetIsTrunsYou(true);
			if (AI::JudgeIsWined(ChessTypeFlags_Black, Point(pos.xPos, pos.yPos), cLayout))
				IsFail = true;
		}
	}
}

void FightUISys::DrawFightUserToUserUI(std::function< void(unsigned char, const char*)> fightFunc)
{
	chatSys.DrawUChatOfUU("Chat module(User-User)", fightFunc);

	ImGui::SetNextWindowPos(ImVec2(370.0f, 0.0f));
	ImGui::SetNextWindowSize(ImVec2(1050.0f, 1050.0f));
	if (!ImGui::Begin("Fight module(User-User)", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoTitleBar))
	{
		ImGui::End();
		return;
	}

	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
	ImGui::NewLine(); ImGui::NewLine(); ImGui::SameLine(520);
	std::vector<std::string> matchUserInfos = bUI.GetMatchUserInfos();
	if (matchUserInfos.empty())
		ImGui::TextWrapped("[stranger]");
	else
	{
		std::string user = "[" + matchUserInfos[0] + "]";
		ImGui::TextWrapped(user.c_str());
	}
	ImGui::NewLine();
	if (isStartGame && isCanDrag && !GetIsTrunsYou())
	{
		static Timer timer(std::bind(&FightUISys::IsTimerCompleteUU, &fightUISys, std::placeholders::_1), std::bind(&FightUISys::SetWaitValue, &fightUISys, std::placeholders::_1),
			std::bind(&FightUISys::EndTimer, &fightUISys));
		if (timer.GetIsTimerEnd())
		{
			SetIsEndTimer(false);
			timer.BEGIN_NegTimer(15, 0);
		}

		char buff[8] = { 0 };
		_itoa_s(fightUISys.GetWaitValue(), buff, sizeof(buff), 10);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.65f, 0.70f, 1.0f));
		std::string text = "Wait: " + std::string(buff) + "s";
		ImGui::SameLine(320); ImGui::TextWrapped(text.c_str());
		ImGui::PopStyleColor();
	}
	ImGui::SameLine(520); UI::showHelpMarker("User's infos(?)", matchUserInfos); ImGui::NewLine();
	for (int i = 1; i < 25; ++i)
		ImGui::NewLine();
	ImGui::NewLine(); ImGui::SameLine(35);
	if (!isStartGame)
	{
		if (ImGui::Button("start", ImVec2(60, 0))) {}
	}
	else
	{
		if (ImGui::Button("reset", ImVec2(60, 0)))
			ReSet();
	}
	ImGui::NewLine(); ImGui::SameLine(35);
	if (ImGui::Button("back", ImVec2(60, 0))) {}
	ImGui::NewLine(); ImGui::NewLine(); ImGui::SameLine(35);
	if (ImGui::Button("exit", ImVec2(60, 0)))
	{
		ImGui::OpenPopup("IsExited??");
	}
	ImGui::SetNextWindowSize(ImVec2(255.0f, 160.0f));
	if (ImGui::BeginPopupModal("IsExited??", 0, ImGuiWindowFlags_NoResize))
	{
		isCanDrag = false;
		ImGui::NewLine();
		ImGui::TextWrapped("\tAre you really exit the game?");
		ImGui::NewLine(); ImGui::NewLine(); ImGui::SameLine(50);
		if (ImGui::Button("YES", ImVec2(50, 0)))
		{
			fightUISys.Clear();
			fightFunc(4, nullptr);
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine(150);
		if (ImGui::Button("NO", ImVec2(50, 0)))
		{
			isCanDrag = true;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	std::vector<std::string> userInfos = bUI.GetUersInfos();
	ImGui::NewLine(); ImGui::NewLine(); ImGui::NewLine(); ImGui::SameLine(520);
	if (userInfos.empty())
		ImGui::TextWrapped("[stranger]");
	else
	{
		std::string user = "[" + userInfos[0] + "]";
		ImGui::TextWrapped(user.c_str());
	}
	ImGui::NewLine();
	if (isStartGame && isCanDrag && GetIsTrunsYou())
	{
		static Timer timer(std::bind(&FightUISys::IsTimerCompleteUU, &fightUISys, std::placeholders::_1), std::bind(&FightUISys::SetWaitValue, &fightUISys, std::placeholders::_1),
			std::bind(&FightUISys::EndTimer, &fightUISys));
		if (timer.GetIsTimerEnd())
		{
			SetIsEndTimer(false);
			timer.BEGIN_NegTimer(15, 0);
		}

		char buff[8] = { 0 };
		_itoa_s(fightUISys.GetWaitValue(), buff, sizeof(buff), 10);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.65f, 0.70f, 1.0f));
		std::string text = "Wait: " + std::string(buff) + "s";
		ImGui::SameLine(320); ImGui::TextWrapped(text.c_str());
		ImGui::PopStyleColor();
	}
	ImGui::SameLine(520); UI::showHelpMarker("User's inofs(?)", userInfos);
	ImGui::PopStyleVar();

	DrawFightCommonUI(draw_list, fightFunc);

	ImGui::End();

	if (isStartGame && IsWin)
		DrawWinOrLose(true);
	else if (isStartGame && IsFail)
		DrawWinOrLose(false);

	if (isStartGame && IsMatchedUserExit)
		DrawExitUI(fightFunc);
}

void FightUISys::IsTimerComplete(bool _v)
{
	if (_v)
	{
		static AI ai;
		Point pos;
		ai.AICalc(ChessTypeFlags_Black, ChessTypeFlags_White, cLayout, Point((chesses.back().cPos.x - BLINE_LEFT_X) / LINE_SPACE,
			(chesses.back().cPos.y - BLINE_UP_Y) / LINE_SPACE), pos);
		AddChess(CHESS(1, ImVec2(BLINE_LEFT_X + pos.xPos * LINE_SPACE, BLINE_UP_Y + pos.yPos * LINE_SPACE)));
		cLayout[pos.xPos][pos.yPos] = 1;
		AddChessCount();
		SetIsTrunsYou(false);
		if (AI::JudgeIsWined(ChessTypeFlags_White, Point(pos.xPos, pos.yPos), cLayout))
			IsWin = true;
	}
}

void FightUISys::IsTimerCompleteUU(bool _v)
{
	if (_v && GetIsTrunsYou())
	{
		static AI ai;
		Point pos;
		ai.AICalc(cEnemyType, cMyselfType, cLayout, Point((chesses.back().cPos.x - BLINE_LEFT_X) / LINE_SPACE,
			(chesses.back().cPos.y - BLINE_UP_Y) / LINE_SPACE), pos);
		std::stringstream str;
		str << pos.xPos << "_" << pos.yPos;
		fightFunc(3, str.str().c_str());
		AddChess(CHESS(char(cMyselfType), ImVec2(BLINE_LEFT_X + pos.xPos * LINE_SPACE, BLINE_UP_Y + pos.yPos * LINE_SPACE)));
		cLayout[pos.xPos][pos.yPos] = char(cMyselfType);
		AddChessCount();
		SetIsTrunsYou(false);
		if (AI::JudgeIsWined(cMyselfType, Point(pos.xPos, pos.yPos), cLayout))
		{
			IsWin = true;
			fightFunc(6, nullptr);
		}
	}
}

void FightUISys::DrawWinOrLose(bool isWin)
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
	if (isCanDrag)
		ImGui::OpenPopup("GameOver");
	isCanDrag = false;
	SetIsEndTimer(true);
	bool isShow = true;
	ImGui::SetNextWindowSize(ImVec2(150.0f, 100.0f));
	if (ImGui::BeginPopupModal("GameOver", &isShow, ImGuiWindowFlags_NoResize))
	{
		ImGui::NewLine(); ImGui::NewLine(); ImGui::SameLine(40);
		if (isWin)
			ImGui::TextWrapped("Victory!!!");
		else
			ImGui::TextWrapped("Failure!!!");
		ImGui::EndPopup();
	}
	ImGui::PopStyleVar();
}

void FightUISys::DrawExitUI(std::function< void(unsigned char, const char*)> fightFunc)
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
	if (isCanDrag)
		ImGui::OpenPopup("UserExit");
	isCanDrag = false;
	SetIsEndTimer(true);
	ImGui::SetNextWindowSize(ImVec2(200.0f, 150.0f));
	if (ImGui::BeginPopupModal("UserExit", 0, ImGuiWindowFlags_NoResize))
	{
		ImGui::NewLine(); ImGui::NewLine(); ImGui::SameLine(10);
		std::string text = std::string(bUI.GetMatUser()) + " have exited!";
		ImGui::TextWrapped(text.c_str());
		ImGui::NewLine();
		ImGui::NewLine(); ImGui::SameLine(80.0f);
		if (ImGui::Button("OK", ImVec2(50.0f, 0)))
		{
			fightUISys.Clear();
			fightFunc(5, nullptr);
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	ImGui::PopStyleVar();
}

void FightUISys::AddEnemyChess(int col, int row)
{
	if (col >= 0 && col < LINE_NUMS && row >= 0 && row < LINE_NUMS)
	{
		if (!GetIsTrunsYou())
		{
			SetIsEndTimer(true);
			SetIsTrunsYou(true);
			cLayout[col][row] = char(cEnemyType);
			AddChessCount();
			AddChess(CHESS(char(cEnemyType), ImVec2(BLINE_LEFT_X + col * LINE_SPACE, BLINE_UP_Y + row * LINE_SPACE)));
			if (AI::JudgeIsWined(cEnemyType, Point(col, row), cLayout))
			{
				IsFail = true;
				fightFunc(7, nullptr);
			}
		}
	}
}

void FightUISys::DrawFightCommonUI(ImDrawList* Draw_list, std::function< void(unsigned char, const char*)> fightFunc)
{
	DrawFightCommonUIOfCommon(Draw_list);

	if (isCanDrag)
	{
		// Mouse position
		ImVec2 MousePos = ImVec2(ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
		if ((MousePos.x >= (BLINE_LEFT_X - LINE_SPACE / 2)) && (MousePos.x <= (BLINE_RIGHT_X + LINE_SPACE / 2)) &&
			(MousePos.y >= (BLINE_UP_Y - LINE_SPACE / 2)) && (MousePos.y <= (BLINE_DOWN_Y + LINE_SPACE / 2)))
		{
			int col = abs(MousePos.x - BLINE_LEFT_X) / LINE_SPACE;
			int row = abs(MousePos.y - BLINE_UP_Y) / LINE_SPACE;
			int colPos = BLINE_LEFT_X + col * LINE_SPACE;
			int rowPos = BLINE_UP_Y + row * LINE_SPACE;
			if (MousePos.x > (colPos + LINE_SPACE / 2))
				col += 1;
			if (MousePos.x < (colPos - LINE_SPACE / 2))
				col -= 1;
			if (MousePos.y >(rowPos + LINE_SPACE / 2))
				row += 1;
			if (MousePos.y < (rowPos - LINE_SPACE / 2))
				row -= 1;

			// Draw tips
			int left_x = (BLINE_LEFT_X + col * LINE_SPACE) - LINE_SPACE / 2;
			int right_x = (BLINE_LEFT_X + col * LINE_SPACE) + LINE_SPACE / 2;
			int up_y = (BLINE_UP_Y + row * LINE_SPACE) - LINE_SPACE / 2;
			int down_y = (BLINE_UP_Y + row * LINE_SPACE) + LINE_SPACE / 2;
			Draw_list->AddLine(ImVec2(left_x, up_y), ImVec2(left_x + 12, up_y), ImColor(ImVec4(0.50f, 0.20f, 0.20f, 1.0f)));
			Draw_list->AddLine(ImVec2(left_x, up_y), ImVec2(left_x, up_y + 12), ImColor(ImVec4(0.50f, 0.20f, 0.20f, 1.0f)));
			Draw_list->AddLine(ImVec2(right_x, up_y), ImVec2(right_x - 12, up_y), ImColor(ImVec4(0.50f, 0.20f, 0.20f, 1.0f)));
			Draw_list->AddLine(ImVec2(right_x, up_y), ImVec2(right_x, up_y + 12), ImColor(ImVec4(0.50f, 0.20f, 0.20f, 1.0f)));
			Draw_list->AddLine(ImVec2(left_x, down_y), ImVec2(left_x, down_y - 12), ImColor(ImVec4(0.50f, 0.20f, 0.20f, 1.0f)));
			Draw_list->AddLine(ImVec2(left_x, down_y), ImVec2(left_x + 12, down_y), ImColor(ImVec4(0.50f, 0.20f, 0.20f, 1.0f)));
			Draw_list->AddLine(ImVec2(right_x, down_y), ImVec2(right_x, down_y - 12), ImColor(ImVec4(0.50f, 0.20f, 0.20f, 1.0f)));
			Draw_list->AddLine(ImVec2(right_x, down_y), ImVec2(right_x - 12, down_y), ImColor(ImVec4(0.50f, 0.20f, 0.20f, 1.0f)));

			// Draw white Chess
			if (ImGui::IsMouseClicked(0))
			{
				if (isTrunsYou)
				{
					if (cLayout[col][row] == char(0))
					{
						std::stringstream str;
						str << col << "_" << row;
						fightFunc(3, str.str().c_str());
						SetIsEndTimer(true);
						SetIsTrunsYou(false);
						cLayout[col][row] = char(cMyselfType);
						AddChessCount();
						AddChess(CHESS(char(cMyselfType), ImVec2(BLINE_LEFT_X + col * LINE_SPACE, BLINE_UP_Y + row * LINE_SPACE)));
						if (AI::JudgeIsWined(cMyselfType, Point(col, row), cLayout))
						{
							IsWin = true;
							fightFunc(6, nullptr);
						}
					}
				}
			}

		}
	}
}

void FightUISys::DrawFightCommonUI(ImDrawList* Draw_list)
{
	DrawFightCommonUIOfCommon(Draw_list);

	if (isCanDrag)
	{
		// Mouse position
		ImVec2 MousePos = ImVec2(ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
		if ((MousePos.x >= (BLINE_LEFT_X - LINE_SPACE / 2)) && (MousePos.x <= (BLINE_RIGHT_X + LINE_SPACE / 2)) &&
			(MousePos.y >= (BLINE_UP_Y - LINE_SPACE / 2)) && (MousePos.y <= (BLINE_DOWN_Y + LINE_SPACE / 2)))
		{
			int col = abs(MousePos.x - BLINE_LEFT_X) / LINE_SPACE;
			int row = abs(MousePos.y - BLINE_UP_Y) / LINE_SPACE;
			int colPos = BLINE_LEFT_X + col * LINE_SPACE;
			int rowPos = BLINE_UP_Y + row * LINE_SPACE;
			if (MousePos.x > (colPos + LINE_SPACE / 2))
				col += 1;
			if (MousePos.x < (colPos - LINE_SPACE / 2))
				col -= 1;
			if (MousePos.y >(rowPos + LINE_SPACE / 2))
				row += 1;
			if (MousePos.y < (rowPos - LINE_SPACE / 2))
				row -= 1;

			// Draw tips
			int left_x = (BLINE_LEFT_X + col * LINE_SPACE) - LINE_SPACE / 2;
			int right_x = (BLINE_LEFT_X + col * LINE_SPACE) + LINE_SPACE / 2;
			int up_y = (BLINE_UP_Y + row * LINE_SPACE) - LINE_SPACE / 2;
			int down_y = (BLINE_UP_Y + row * LINE_SPACE) + LINE_SPACE / 2;
			Draw_list->AddLine(ImVec2(left_x, up_y), ImVec2(left_x + 12, up_y), ImColor(ImVec4(0.50f, 0.20f, 0.20f, 1.0f)));
			Draw_list->AddLine(ImVec2(left_x, up_y), ImVec2(left_x, up_y + 12), ImColor(ImVec4(0.50f, 0.20f, 0.20f, 1.0f)));
			Draw_list->AddLine(ImVec2(right_x, up_y), ImVec2(right_x - 12, up_y), ImColor(ImVec4(0.50f, 0.20f, 0.20f, 1.0f)));
			Draw_list->AddLine(ImVec2(right_x, up_y), ImVec2(right_x, up_y + 12), ImColor(ImVec4(0.50f, 0.20f, 0.20f, 1.0f)));
			Draw_list->AddLine(ImVec2(left_x, down_y), ImVec2(left_x, down_y - 12), ImColor(ImVec4(0.50f, 0.20f, 0.20f, 1.0f)));
			Draw_list->AddLine(ImVec2(left_x, down_y), ImVec2(left_x + 12, down_y), ImColor(ImVec4(0.50f, 0.20f, 0.20f, 1.0f)));
			Draw_list->AddLine(ImVec2(right_x, down_y), ImVec2(right_x, down_y - 12), ImColor(ImVec4(0.50f, 0.20f, 0.20f, 1.0f)));
			Draw_list->AddLine(ImVec2(right_x, down_y), ImVec2(right_x - 12, down_y), ImColor(ImVec4(0.50f, 0.20f, 0.20f, 1.0f)));

			// Draw white Chess
			if (ImGui::IsMouseClicked(0))
			{
				if (isTrunsYou)
				{
					if (cLayout[col][row] == char(0))
					{
						SetIsEndTimer(true);
						SetIsTrunsYou(false);
						cLayout[col][row] = 1;
						AddChessCount();
						AddChess(CHESS(1, ImVec2(BLINE_LEFT_X + col * LINE_SPACE, BLINE_UP_Y + row * LINE_SPACE)));
						if (AI::JudgeIsWined(ChessTypeFlags_White, Point(col, row), cLayout))
							IsWin = true;
					}
				}
			}

		}
	}
}

void FightUISys::DrawFightCommonUIOfCommon(ImDrawList* Draw_list)
{
	Draw_list->AddRectFilled(ImVec2(790, 15), ImVec2(790 + 80, 15 + 80), ImColor(ImVec4(0.60f, 0.20f, 0.4f, 1.0f)), 10.0f);
	Draw_list->AddRectFilled(ImVec2(790, 955), ImVec2(790 + 80, 955 + 80), ImColor(ImVec4(0.50f, 0.30f, 0.45f, 1.0f)), 10.0f);
	if (isStartGame)
	{
		if (GetIsTrunsYou())
			Draw_list->AddRect(ImVec2(790, 955), ImVec2(790 + 80, 955 + 80), ImColor(ImVec4(0.60f, 0.60f, 0.60f, 1.0f)), 10.0f);
		else
			Draw_list->AddRect(ImVec2(790, 15), ImVec2(790 + 80, 15 + 80), ImColor(ImVec4(0.60f, 0.60f, 0.60f, 1.0f)), 10.0f);
	}

	// Draw grid
	for (int i = 0; i < LINE_NUMS; ++i)
	{
		// Draw x
		Draw_list->AddLine(ImVec2(BLINE_LEFT_X, BLINE_UP_Y + LINE_SPACE * i), ImVec2(BLINE_RIGHT_X, BLINE_UP_Y + LINE_SPACE * i),
			ImColor(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)));
		// Draw y
		Draw_list->AddLine(ImVec2(BLINE_LEFT_X + LINE_SPACE * i, BLINE_UP_Y), ImVec2(BLINE_LEFT_X + LINE_SPACE * i, BLINE_DOWN_Y),
			ImColor(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)));
	}
	// Draw five black point
	Draw_list->AddCircleFilled(ImVec2(BLINE_LEFT_X + LINE_SPACE * 4, BLINE_UP_Y + LINE_SPACE * 4), 4.0f, ImColor(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), 60);
	Draw_list->AddCircleFilled(ImVec2(BLINE_RIGHT_X - LINE_SPACE * 4, BLINE_UP_Y + LINE_SPACE * 4), 4.0f, ImColor(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), 60);
	Draw_list->AddCircleFilled(ImVec2(BLINE_LEFT_X + LINE_SPACE * 8, BLINE_UP_Y + LINE_SPACE * 8), 4.0f, ImColor(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), 60);
	Draw_list->AddCircleFilled(ImVec2(BLINE_LEFT_X + LINE_SPACE * 4, BLINE_DOWN_Y - LINE_SPACE * 4), 4.0f, ImColor(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), 60);
	Draw_list->AddCircleFilled(ImVec2(BLINE_RIGHT_X - LINE_SPACE * 4, BLINE_DOWN_Y - LINE_SPACE * 4), 4.0f, ImColor(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), 60);

	// Draw chesses
	for (int i = 0; i < chesses.Size; ++i)
	{
		if (chesses[i].ctype == 1)
			Draw_list->AddCircleFilled(chesses[i].cPos, 18.0f, ImColor(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), 60);
		else
			Draw_list->AddCircleFilled(chesses[i].cPos, 18.0f, ImColor(ImVec4(0.0f, 0.0f, 0.0f, 1.0f)), 60);
	}

	// Draw last chess
	if (!chesses.empty())
	{
		int left_x = chesses.back().cPos.x - LINE_SPACE / 2;
		int right_x = chesses.back().cPos.x + LINE_SPACE / 2;
		int up_y = chesses.back().cPos.y - LINE_SPACE / 2;
		int down_y = chesses.back().cPos.y + LINE_SPACE / 2;
		Draw_list->AddLine(ImVec2(left_x, up_y), ImVec2(left_x + 12, up_y), ImColor(ImVec4(0.50f, 0.20f, 0.20f, 1.0f)));
		Draw_list->AddLine(ImVec2(left_x, up_y), ImVec2(left_x, up_y + 12), ImColor(ImVec4(0.50f, 0.20f, 0.20f, 1.0f)));
		Draw_list->AddLine(ImVec2(right_x, up_y), ImVec2(right_x - 12, up_y), ImColor(ImVec4(0.50f, 0.20f, 0.20f, 1.0f)));
		Draw_list->AddLine(ImVec2(right_x, up_y), ImVec2(right_x, up_y + 12), ImColor(ImVec4(0.50f, 0.20f, 0.20f, 1.0f)));
		Draw_list->AddLine(ImVec2(left_x, down_y), ImVec2(left_x, down_y - 12), ImColor(ImVec4(0.50f, 0.20f, 0.20f, 1.0f)));
		Draw_list->AddLine(ImVec2(left_x, down_y), ImVec2(left_x + 12, down_y), ImColor(ImVec4(0.50f, 0.20f, 0.20f, 1.0f)));
		Draw_list->AddLine(ImVec2(right_x, down_y), ImVec2(right_x, down_y - 12), ImColor(ImVec4(0.50f, 0.20f, 0.20f, 1.0f)));
		Draw_list->AddLine(ImVec2(right_x, down_y), ImVec2(right_x - 12, down_y), ImColor(ImVec4(0.50f, 0.20f, 0.20f, 1.0f)));
	}
}
