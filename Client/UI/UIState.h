#pragma once

// UI size
#define SIGN_X			500
#define SIGN_Y			500
#define MAIN_X			800
#define MAIN_Y			800
#define FIGHT_X			1370
#define FIGHT_Y			1000

#define LINE_NUMS		16
#define LINE_SPACE		50
// Line position
#define BLINE_LEFT_X	(370+125)
#define BLINE_RIGHT_X	(370+125+750)
#define BLINE_UP_Y		125
#define BLINE_DOWN_Y	(125+750)

// UI type
enum UIState
{
	SIGNIN,
	SIGNUP,
	MAIN,
	FIGHT
};
