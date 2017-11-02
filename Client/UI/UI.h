#pragma once

#include "imgui.h"
#include "imgui-SFML.h"
#include "UIState.h"
#include "../MessageType.h"
#include "../Timer/Timer.h"

#include <iostream>
#include <sstream>
#include <functional>
#include <vector>
#include <mutex>
#include <stdlib.h>
#include <cmath>
#include <SFML/Graphics.hpp>

#define IM_ARRAYSIZE(_ARR)	((int)(sizeof(_ARR)/sizeof(*_ARR)))


class UI
{
public:
	static void init();
	static void showHelpMarker(const char* desc, const char* text);
	static void signin(MSG_STATE& msgState, std::function< void(const char*, const char*, bool isSignup)> signinFunc);
	static void signup(MSG_STATE& msgState, std::function< void(const char*, const char*, const char*, bool isBackup)> signupFunc);
	static void mainUI(MSG_STATE& msgState, std::function< void(unsigned char, bool)> mainUIFunc, 
		std::function< void(const char*)> chatFunc, std::function< void(bool, const char*)> usersFunc,
		std::function< void(bool, const char*)> matchFunc, std::function< void(int, const char*)> matchAgreeFunc);
	static void FightUI();

private:
	static void chatUI(MSG_STATE& msgState, bool* p_open, std::function< void(const char*)> chatFunc,
		std::function< void(bool, const char*)> usersFunc);
	static void matchUI(const char* title, std::function< void(bool, const char*)> usersFunc, std::function< void(bool, const char*)> matchFunc);
};

//-----------------------------------------------------------------------------------------------------------------------------
// For Group chat system, it contians userUI(UsersUISys) and chat UI(ChatUISys).
// We can achieve userUI in Group chat system and chatUI in User-user or Man-machine battle system by UsersUISys class.
//-----------------------------------------------------------------------------------------------------------------------------

class UsersUISys
{
	friend class ChatUISys;
public:
	void Clear();
	void Draw(const char* title, bool* p_open, std::function< void(bool, const char*)> usersFunc);
	// For matchUI in BattleUISys(User-user or Man-machine battle system)
	void Draw(const char* title, std::function< void(bool, const char*)> userFunc, std::function< void(bool, const char*)> matchFunc);
	void SetItemIsOnline(const char isOnline, const char* user);
	void AddItem(const char isOnline, const char* user);

private:
	// For show user's information
	void DrawForUserofInfo();

private:
	ImVector<char*>	Users;
	ImVector<char>	IsOnline;

	ImGuiTextFilter	Filter;
	bool		ScrollToBottom;
};


class ChatUISys
{
public:
	void Clear();
	void AddLog(const char color, const char* user, const char* fmt, ...);
	void AddUser(const char* user);
	void AddUserOfuUI(const char isOnline, const char* user);
	void SetItemOfuUI(const char isOnline, const char* user);
	void ClearOfuUI() { uUI.Clear(); }
	void Draw(MSG_STATE& msgState, const char* title, bool* p_open, std::function< void(const char*)> chatFunc,
		std::function< void(bool, const char*)> usersFunc);

	static inline int	Stricmp(const char* str1, const char* str2)			{ int sign; while ((sign = toupper(*str2) - toupper(*str1)) == 0 && *str1){ str1++; str2++;} return sign; }
	static inline int	Strnicmp(const char* str1, const char* str2, int n) 		{ int sign; while (n > 0 && (sign = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; n--; } return sign; }
	static inline char* Strdup(const char* str)						{ size_t len = strlen(str) + 1; void* buff = malloc(len); return (char*)memcpy(buff, (const void*)str, len); }
	static inline void	ParserText(std::vector<std::string>& vec, const char* text);

private:
	ImVector<char*> Items;
	ImVector<char*> Users;
	ImVector<char>	Color;

	ImGuiTextFilter	Filter;
	bool		ScrollToBottom;

	UsersUISys	uUI;
};

//-----------------------------------------------------------------------------------------------------------------------------
// For User-user or Man-machine battle system, each contains matchUI and fightUI.
// In User-user battle system, we achieve one user match another user, but in Man-machine system, we achieve one user match
// machine(easy, median, difficult machine etc.)
// The matchUI are achieved by UserUISys, the fightUI contains user-user chat system and fight system.
//-----------------------------------------------------------------------------------------------------------------------------

typedef unsigned int UINT;

/* User's fight information structure */
typedef struct
{
	char uName[20];
	UINT level;
	UINT exper;
	UINT wined;
	UINT losed;
	UINT escap;
}USERINFO;

/* User's match state */
enum MatchStateFlags_
{
	MatchStateFlags_SUCCESS,
	MatchStateFlags_FAIL,
	MatchStateFlags_AGREE,
	MatchStateFlags_REJECT,
	MatchStateFlags_NONE
};

/* User's match information structure */
typedef struct MatchStateSct
{
	char ReqUser[20];
	char MatUser[20];
	MatchStateFlags_ MSFlags;
	MatchStateSct() :MSFlags(MatchStateFlags_NONE) {}
}MatchStateSct;

class BattleUISys
{
public:
	BattleUISys() :isTimerComplete(false), isTimerAComplete(false), isMatchAgree(false), isEndTimer(false) {}
	// For matchUI
	inline void DrawMatchUI(const char* title, std::function< void(bool, const char*)> userFunc, 
		std::function< void(bool, const char*)> matchFunc)		{ matchUISys.Draw(title, userFunc, matchFunc); }
	inline void AddUserOfuUI(const char isOnline, const char* user)		{ matchUISys.AddItem(isOnline, user); }
	inline void SetItemOfuUI(const char isOnline, const char* user)		{ matchUISys.SetItemIsOnline(isOnline, user); }
	inline void Clear()							{ matchUISys.Clear(); }
	// For match wait UI by vClock
	void DrawMatchWaitUI();
	void DrawMatchAgreeUI(std::function< void(int, const char*)> matchAgreeFunc);


	// For uInfo
	void GetUInfo(UINT* infos);
	void GetUInfo(const char* user, UINT* infos);
	void SetUInfo(const char* user, UINT level = 0, UINT exper = 0, UINT wined = 0, UINT losed = 0, UINT escap = 0);
	void SetUInfo(const char* user, const char* level, const char* exper, const char* wined, const char* losed,
		const char* escap);

	// For matchSS
	inline void SetMatchSS(MatchStateFlags_ msFlags, const char* _reqUser, const char* _matUser) { memset(&matchSS, 0, sizeof(matchSS)); matchSS.MSFlags = msFlags; strcpy(matchSS.ReqUser, _reqUser); strcpy(matchSS.MatUser, _matUser);}
	inline void SetMatchSS(MatchStateFlags_ flags)		{ memset(&matchSS, 0, sizeof(matchSS)); matchSS.MSFlags = flags; }
	inline const char* GetReqUser() const			{ return matchSS.ReqUser; }
	inline const char* GetMatUser() const			{ return matchSS.MatUser; }
	inline MatchStateFlags_ GetMatchState() const		{ return matchSS.MSFlags; }
	inline void SetMatchState(MatchStateFlags_ _t)		{ matchSS.MSFlags = _t; }
	inline void SetVClock(int _vClock)			{ std::lock_guard<std::mutex> lock(_mutex); vClock = _vClock; }
	inline const int GetVClock() const			{ return vClock; }
	inline void SetIsTimerComplete(bool _v)			{ std::lock_guard<std::mutex> lock(_mutex); isTimerComplete = _v; }
	inline bool GetIsTimerComplete() const			{ return isTimerComplete; }
	inline bool EndTimer()					{ return isEndTimer; }
	inline void SetIsEndTimer(bool _v)			{ std::lock_guard<std::mutex> lock(_mutex); isEndTimer = _v; }
	inline void SetMatchATimerComplete(bool _v)		{ std::lock_guard<std::mutex> lock(_mutex); isTimerAComplete = _v; }

	// For match agree UI
	inline void SetIsMatchAgree(bool _v)			{ std::lock_guard<std::mutex> lock(_mutex); isMatchAgree = _v; }
	inline bool GetIsMatchAgree() const			{ return isMatchAgree; }
	inline void SetIsShowMatchAUI(bool _v)			{ std::lock_guard<std::mutex> lock(_mutex); isShowMatchAUI = _v; }
	inline bool GetIsShowMatchAUI() const			{ return isShowMatchAUI; }

private:
	UsersUISys	matchUISys;
	USERINFO	uInfo;
	MatchStateSct	matchSS;

	// For match wait UI, we will show the value of clock(from vClock to 0 step by 1)
	int		vClock;
	// To sync the vClock
	std::mutex	_mutex;
	// If the match wait UI is closed, we should close the match wait UI
	bool		isTimerComplete;
	bool		isEndTimer;

	// For match agree UI
	bool		isShowMatchAUI;
	bool		isTimerAComplete;
	bool		isMatchAgree;
	std::function< void(int, const char*)> matchWaitAgree;
};


//-----------------------------------------------------------------------------------------------------------------------------
// In order to make code clearer, we will achieve fight system in another class(FightUISys), BattleUISys Only achieve match
// function actually. FightUISys contains chat and fight model.
// The chat model is to achieve communicate with two users, the fight model is to achieve game function.
//-----------------------------------------------------------------------------------------------------------------------------

class UChat
{
private:
	char		InputBuf[256];
	ImGuiTextFilter	Filter;
	ImVector<char*>	Items;
	ImVector<char*> Users;
	ImVector<char>	Type;
	bool		ScrollToBottom;

public:
	UChat()		{ Clear(); memset(InputBuf, 0, sizeof(InputBuf)); }
	~UChat()	{ Clear(); }

	void Clear();
	void AddItem(char type, const char* fmt, const char* user);
	void Draw(const char* title);
};

typedef struct CHESS
{
	char	ctype;	// Chess type
	ImVec2	cPos;	// Chess position

	CHESS(char _ctype, const ImVec2& _cPos)
	{
		ctype = _ctype;
		cPos  = _cPos;
	}
}CHESS;

class FightUISys
{
public:
	FightUISys(BattleUISys& _battleUISys) :battleUISys(_battleUISys) {}
	void DrawFightUI();

private:
	BattleUISys&	battleUISys;
	UChat		chatSys;
	
	ImVector<CHESS> chesses;
};
