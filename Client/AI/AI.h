#pragma once

#include "../UI/UIState.h"
#include <queue>
#include <string.h>


/* AI algorithm */

// Score value
#define SCORE_WIN		 100000
#define SCORE_LIVEF		 10000
#define SCORE_D_LIVET		 5000
#define SCORE_DE_T_LIVE_T	 1000
#define SCORE_D_F		 500
#define SCORE_LOWD_F		 400
#define SCORE_LIVE_T		 100
#define SCORE_SKIP_LIVE_T	 90
#define SCORE_LIVE_TWO		 10
#define SCORE_LOW_LIVE_TWO 	 9
#define SCORE_D_T		 5
#define SCORE_D_TWO		 2
#define SCORE_NOTHREAT		 0

// Chess type
enum ChessTypeFlags_
{
	ChessTypeFlags_White = 1,
	ChessTypeFlags_Black = 2
};

enum DragTypeFlags_
{
	DragTypeFlags_Empty,
	DragTypeFlags_NotEmpty,

	DragTypeFlags_Enemy,
	DragTypeFlags_NEnemy
};

typedef struct Point
{
	int xPos;
	int yPos;

	Point() {};
	Point(int _xPos, int _yPos)
	{
		xPos = _xPos;
		yPos = _yPos;
	}
}Point;

class AI
{
public:
	AI();

	void AICalc(ChessTypeFlags_ cEnemyType, ChessTypeFlags_ cNEnemyType, char CLayout[][LINE_NUMS], const Point& cPos, Point& Pos);
	void AIInit();
	
	static bool JudgeIsWined(ChessTypeFlags_ cType, const Point& pos, char CLayout[][LINE_NUMS]);

private:
	void Calc(char CLayout[][LINE_NUMS], int ResSocre[][LINE_NUMS], ChessTypeFlags_ cMy, ChessTypeFlags_ cEnemy);
	int CalcScore(int xPos, int yPos, char CLayout[][LINE_NUMS], ChessTypeFlags_ cMy, ChessTypeFlags_ cEnemy);
	void CalcDragType(Point leftPos, Point rightPos, int dir, int offset,
		DragTypeFlags_& leftDragType, DragTypeFlags_& rightDragType, char CLayout[][LINE_NUMS], ChessTypeFlags_ cMy);

	// If not find, we will find the closest point of cPos by BFS
	void FindCloPosBFS(char CLayout[][LINE_NUMS], const Point& cPos, Point& Pos);

private:
	int Enemy[LINE_NUMS][LINE_NUMS];
	int MSelf[LINE_NUMS][LINE_NUMS];

	Point leftPoint;
	Point rightPoint;

	DragTypeFlags_ left;
	DragTypeFlags_ right;
};
