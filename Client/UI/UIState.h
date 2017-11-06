#pragma once

// UI size
#define SIGN_X			500
#define SIGN_Y			500
#define MAIN_X			800
#define MAIN_Y			800
#define FIGHT_X			(1370+50)
#define FIGHT_Y			(1000+50)

#define LINE_NUMS		17
#define LINE_SPACE		50
// Line position
#define BLINE_LEFT_X		(370+125)
#define BLINE_RIGHT_X		(370+125+750+50)
#define BLINE_UP_Y		125
#define BLINE_DOWN_Y		(125+750+50)

// UI type
enum UIState
{
	SIGNIN,
	SIGNUP,
	MAIN,
	FIGHT,			// fight to another user
	FIGHTTOMACHINE		// fight to machine
};

// In fight module
enum FightTypeFlags_
{
	FightTypeFlags_UserToMachine,	// Means user fight with machine
	FightTypeFlags_UserToUser	// Means user fight with another user
};
