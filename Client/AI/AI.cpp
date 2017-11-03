#include "AI.h"


AI::AI() :
	left(DragTypeFlags_NotEmpty), right(DragTypeFlags_NotEmpty)
{
	memset(Enemy, 0, sizeof(Enemy));
	memset(MSelf, 0, sizeof(MSelf));
}

void AI::AIInit()
{
	left = DragTypeFlags_NotEmpty;
	right = DragTypeFlags_NotEmpty;
	memset(Enemy, 0, sizeof(Enemy));
	memset(MSelf, 0, sizeof(MSelf));
}

/* CLayout[i][j] == 0 stand for empty, 1 is white, 2 is black*/
void AI::AICalc(ChessTypeFlags_ cEnemyType, ChessTypeFlags_ cNEnemyType, char CLayout[][LINE_NUMS], const Point& cPos, Point& Pos)
{
	AIInit();

	// Enemy(judge by ChessTypeFlags_ cEnemyType)
	Calc(CLayout, Enemy, cEnemyType, cNEnemyType);
	// MSelf
	Calc(CLayout, MSelf, cNEnemyType, cEnemyType);

	int MSelfScore = 0;
	Point MSelfPos;
	for (int i = 0; i < LINE_NUMS; ++i)
	{
		for (int j = 0; j < LINE_NUMS; ++j)
		{
			if (CLayout[i][j] != 0)
				continue;
			if (MSelf[i][j] >= MSelfScore)
			{
				MSelfScore = MSelf[i][j];
				MSelfPos.xPos = i;
				MSelfPos.yPos = j;
			}
		}
	}

	int EnemyScore = 0;
	Point EnemyPos;
	for (int i = 0; i < LINE_NUMS; ++i)
	{
		for (int j = 0; j < LINE_NUMS; ++j)
		{
			if (CLayout[i][j] != 0)
				continue;
			if (Enemy[i][j] >= EnemyScore)
			{
				EnemyScore = Enemy[i][j];
				EnemyPos.xPos = i;
				EnemyPos.yPos = j;
			}
		}
	}

	if (MSelfScore == 0 && EnemyScore == 0)
	{
		// closest point of cPos
		FindCloPosBFS(CLayout, cPos, Pos);
	}
	else
	{
		if (MSelfScore > EnemyScore)
			Pos = MSelfPos;
		else
			Pos = EnemyPos;
	}
}

void AI::FindCloPosBFS(char CLayout[][LINE_NUMS], const Point& cPos, Point& Pos)
{
	static const Point Dir[] = {
		{0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}
	};

	bool isVisited[LINE_NUMS][LINE_NUMS];
	memset(isVisited, false, sizeof(isVisited));
	std::queue<Point> Q;
	Q.push(cPos);
	isVisited[cPos.xPos][cPos.yPos] = true;
	Point pos, newPos;
	while (!Q.empty())
	{
		pos = Q.front();
		Q.pop();
		for (int i = 0; i < 8; ++i)
		{
			newPos.xPos = pos.xPos + Dir[i].xPos;
			newPos.yPos = pos.yPos + Dir[i].yPos;
			if (newPos.xPos < 0 || newPos.xPos >= LINE_NUMS || newPos.yPos < 0 || newPos.yPos >= LINE_NUMS)
				continue;
			else
			{
				if (isVisited[newPos.xPos][newPos.yPos])
					continue;
				if (CLayout[newPos.xPos][newPos.yPos] == char(0))
				{
					Pos = newPos;
					return;
				}
				else
				{
					isVisited[newPos.xPos][newPos.yPos] = true;
					Q.push(newPos);
				}
			}
		}
	}
}

void AI::Calc(char CLayout[][LINE_NUMS], int ResSocre[][LINE_NUMS], ChessTypeFlags_ cMy, ChessTypeFlags_ cEnemy)
{
	for (int i = 0; i < LINE_NUMS; ++i)
	{
		for (int j = 0; j < LINE_NUMS; ++j)
		{
			if (CLayout[i][j] != 0)
				continue;
			ResSocre[i][j] = CalcScore(i, j, CLayout, cMy, cEnemy);
		}
	}
}

int AI::CalcScore(int xPos, int yPos, char CLayout[][LINE_NUMS], ChessTypeFlags_ cMy, ChessTypeFlags_ cEnemy)
{
	static const Point Directions[][4] = {
		{ { 0, -1 },{ 0, -2 },{ 0, -3 },{ 0, -4 } },
		{ { 1, -1 },{ 2, -2 },{ 3, -3 },{ 4, -4 } },
		{ { 1,  0 },{ 2,  0 },{ 3,  0 },{ 4,  0 } },
		{ { 1,  1 },{ 2,  2 },{ 3,  3 },{ 4,  4 } },
		{ { 0,  1 },{ 0,  2 },{ 0,  3 },{ 0,  4 } },
		{ { -1, 1 },{ -2, 2 },{ -3, 3 },{ -4, 4 } },
		{ { -1, 0 },{ -2, 0 },{ -3, 0 },{ -4, 0 } },
		{ { -1,-1 },{ -2,-2 },{ -3,-3 },{ -4,-4 } }
	};

	int Score = 0;
	for (int i = 0; i < 4; ++i)
	{
		int Count = 0;
		for (int j = 0; j < 4; ++j)
		{
			int newXPos = xPos + Directions[i][j].xPos;
			int newYPos = yPos + Directions[i][j].yPos;
			leftPoint.xPos = newXPos; leftPoint.yPos = newYPos;
			if (newXPos < 0 || newXPos >= LINE_NUMS ||
				newYPos < 0 || newYPos >= LINE_NUMS)
			{
				left = DragTypeFlags_NotEmpty;
				break;
			}
			if (CLayout[newXPos][newYPos] == char(cMy))
				Count++;
			else if (CLayout[newXPos][newYPos] == char(0))
			{
				left = DragTypeFlags_Empty;
				break;
			}
			else
			{
				left = DragTypeFlags_NotEmpty;
				break;
			}
		}
		for (int j = 0; j < 4; ++j)
		{
			int newXPos = xPos + Directions[i + 4][j].xPos;
			int newYPos = yPos + Directions[i + 4][j].yPos;
			rightPoint.xPos = newXPos; rightPoint.yPos = newYPos;
			if (newXPos < 0 || newXPos >= LINE_NUMS ||
				newYPos < 0 || newYPos >= LINE_NUMS)
			{
				right = DragTypeFlags_NotEmpty;
				break;
			}
			if (CLayout[newXPos][newYPos] == char(cMy))
				Count++;
			else if (CLayout[newXPos][newYPos] == char(0))
			{
				right = DragTypeFlags_Empty;
				break;
			}
			else
			{
				right = DragTypeFlags_NotEmpty;
				break;
			}
		}

		Count++;
		if (Count >= 5)
			return SCORE_WIN;
		else
		{
			switch (Count)
			{
			case 4:
				if (left == DragTypeFlags_Empty && right == DragTypeFlags_Empty)
				{
					if (Score < int(SCORE_LIVEF))
						Score = SCORE_LIVEF;
				}
				else if (left == DragTypeFlags_Empty || right == DragTypeFlags_Empty)
				{
					if (Score < int(SCORE_D_F))
						Score = SCORE_D_F;
				}
				break;
			case 3:
			{
				DragTypeFlags_ llDragType, rrDragType;
				CalcDragType(leftPoint, rightPoint, i, 1, llDragType, rrDragType, CLayout, cMy);

				if (left == DragTypeFlags_Empty && right == DragTypeFlags_Empty)
				{
					if (llDragType == DragTypeFlags_Enemy && rrDragType == DragTypeFlags_Enemy)
					{
						if (Score < int(SCORE_D_T))
							Score = SCORE_D_T;
					}
					else if (llDragType == DragTypeFlags_NEnemy || rrDragType == DragTypeFlags_NEnemy)
					{
						if (Score < int(SCORE_LOWD_F))
							Score = SCORE_LOWD_F;
					}
					else if (llDragType == DragTypeFlags_Empty || rrDragType == DragTypeFlags_Empty)
					{
						if (Score < int(SCORE_LIVE_T))
							Score = SCORE_LIVE_T;
					}
				}
				else if (left == DragTypeFlags_Empty || right == DragTypeFlags_Empty)
				{
					if (llDragType == DragTypeFlags_Enemy)
					{
						if (rrDragType == DragTypeFlags_Empty)
						{
							if (Score < int(SCORE_D_T))
								Score = SCORE_D_T;
						}
						if (rrDragType == DragTypeFlags_NEnemy)
						{
							if (Score < int(SCORE_LOWD_F))
								Score = SCORE_LOWD_F;
						}
					}
					if (rrDragType == DragTypeFlags_Enemy)
					{
						if (llDragType == DragTypeFlags_Empty)
						{
							if (Score < int(SCORE_D_T))
								Score = SCORE_D_T;
						}
						if (llDragType == DragTypeFlags_NEnemy)
						{
							if (Score < int(SCORE_LOWD_F))
								Score = SCORE_LOWD_F;
						}
					}
				}
			}
			break;
			case 2:
			{
				DragTypeFlags_ llDragType, rrDragType, lllDragType, rrrDragType;
				CalcDragType(leftPoint, rightPoint, i, 1, llDragType, rrDragType, CLayout, cMy);
				CalcDragType(leftPoint, rightPoint, i, 2, lllDragType, rrrDragType, CLayout, cMy);

				if (left == DragTypeFlags_Empty && right == DragTypeFlags_Empty)
				{
					if ((rrDragType == DragTypeFlags_Empty && rrrDragType == DragTypeFlags_NEnemy) ||
						(llDragType == DragTypeFlags_Empty && lllDragType == DragTypeFlags_NEnemy))
					{
						if (Score < int(SCORE_D_T))
							Score = SCORE_D_T;
					}
					else if (llDragType == DragTypeFlags_Empty && rrDragType == DragTypeFlags_Empty)
					{
						if (Score < int(SCORE_LIVE_TWO))
							Score = SCORE_LIVE_TWO;
					}

					if ((rrDragType == DragTypeFlags_NEnemy && rrrDragType == DragTypeFlags_Enemy) ||
						(llDragType == DragTypeFlags_NEnemy && lllDragType == DragTypeFlags_Enemy))
					{
						if (Score < int(SCORE_D_T))
							Score = SCORE_D_T;
					}

					if ((rrDragType == DragTypeFlags_NEnemy && rrrDragType == DragTypeFlags_NEnemy) ||
						(llDragType == DragTypeFlags_NEnemy && lllDragType == DragTypeFlags_NEnemy))
					{
						if (Score < int(SCORE_D_F))
							Score = SCORE_D_F;
					}

					if ((rrDragType == DragTypeFlags_NEnemy && rrrDragType == DragTypeFlags_Empty) ||
						(llDragType == DragTypeFlags_NEnemy && lllDragType == DragTypeFlags_Empty))
					{
						if (Score < int(SCORE_SKIP_LIVE_T))
							Score = SCORE_SKIP_LIVE_T;
					}
				}
				else if (left == DragTypeFlags_Empty || right == DragTypeFlags_Empty)
				{
					if (left == DragTypeFlags_Enemy)
					{
						if (rrDragType == DragTypeFlags_Enemy || rrrDragType == DragTypeFlags_Enemy) {}
						else if (rrDragType == DragTypeFlags_Empty && rrrDragType == DragTypeFlags_Empty)
						{
							if (Score < int(SCORE_D_TWO))
								Score = SCORE_D_TWO;
						}
						else if (rrDragType == DragTypeFlags_NEnemy && rrrDragType == DragTypeFlags_NEnemy)
						{
							if (Score < int(SCORE_D_F))
								Score = SCORE_D_F;
						}
						else if (rrDragType == DragTypeFlags_NEnemy || rrrDragType == DragTypeFlags_NEnemy)
						{
							if (Score < int(SCORE_D_T))
								Score = SCORE_D_T;
						}
					}
					if (right == DragTypeFlags_Enemy)
					{
						if (llDragType == DragTypeFlags_Enemy || lllDragType == DragTypeFlags_Enemy) {}
						else if (llDragType == DragTypeFlags_Empty && lllDragType == DragTypeFlags_Empty)
						{
							if (Score < int(SCORE_D_TWO))
								Score = SCORE_D_TWO;
						}
						else if (llDragType == DragTypeFlags_NEnemy && lllDragType == DragTypeFlags_NEnemy)
						{
							if (Score < int(SCORE_D_F))
								Score = SCORE_D_F;
						}
						else if (llDragType == DragTypeFlags_NEnemy || lllDragType == DragTypeFlags_NEnemy)
						{
							if (Score < int(SCORE_D_T))
								Score = SCORE_D_T;
						}
					}
				}
			}
			break;
			case 1:
			{
				DragTypeFlags_ llDragType, rrDragType, lllDragType, rrrDragType, llllDragType, rrrrDragType;
				CalcDragType(leftPoint, rightPoint, i, 1, llDragType, rrDragType, CLayout, cMy);
				CalcDragType(leftPoint, rightPoint, i, 2, lllDragType, rrrDragType, CLayout, cMy);
				CalcDragType(leftPoint, rightPoint, i, 3, llllDragType, rrrrDragType, CLayout, cMy);

				if (left == DragTypeFlags_Empty && llDragType == DragTypeFlags_NEnemy && lllDragType == DragTypeFlags_NEnemy &&
					llllDragType == DragTypeFlags_NEnemy)
				{
					if (Score < int(SCORE_LOWD_F))
						Score = SCORE_LOWD_F;
				}
				if (right == DragTypeFlags_Empty && rrDragType == DragTypeFlags_NEnemy && rrrDragType == DragTypeFlags_NEnemy &&
					rrrrDragType == DragTypeFlags_NEnemy)
				{
					if (Score < int(SCORE_LOWD_F))
						Score = SCORE_LOWD_F;
				}

				if (left == DragTypeFlags_Empty && llDragType == DragTypeFlags_NEnemy && lllDragType == DragTypeFlags_NEnemy &&
					llllDragType == DragTypeFlags_Empty && right == DragTypeFlags_Empty)
				{
					if (Score < int(SCORE_SKIP_LIVE_T))
						Score = SCORE_SKIP_LIVE_T;
				}
				if (right == DragTypeFlags_Empty && rrDragType == DragTypeFlags_NEnemy && rrrDragType == DragTypeFlags_NEnemy &&
					rrrrDragType == DragTypeFlags_Empty && left == DragTypeFlags_Empty)
				{
					if (Score < int(SCORE_SKIP_LIVE_T))
						Score = SCORE_SKIP_LIVE_T;
				}

				if (left == DragTypeFlags_Empty && llDragType == DragTypeFlags_NEnemy && lllDragType == DragTypeFlags_NEnemy &&
					llllDragType == DragTypeFlags_Enemy && right == DragTypeFlags_Empty)
				{
					if (Score < int(SCORE_D_T))
						Score = SCORE_D_T;
				}
				if (right == DragTypeFlags_Empty && rrDragType == DragTypeFlags_NEnemy && rrrDragType == DragTypeFlags_NEnemy &&
					rrrrDragType == DragTypeFlags_Enemy && left == DragTypeFlags_Empty)
				{
					if (Score < int(SCORE_D_T))
						Score = SCORE_D_T;
				}

				if (left == DragTypeFlags_Empty && llDragType == DragTypeFlags_Empty && lllDragType == DragTypeFlags_NEnemy &&
					llllDragType == DragTypeFlags_NEnemy)
				{
					if (Score < int(SCORE_D_T))
						Score = SCORE_D_T;
				}
				if (right == DragTypeFlags_Empty && rrDragType == DragTypeFlags_Empty && rrrDragType == DragTypeFlags_NEnemy &&
					rrrrDragType == DragTypeFlags_NEnemy)
				{
					if (Score < int(SCORE_D_T))
						Score = SCORE_D_T;
				}

				if (left == DragTypeFlags_Empty && llDragType == DragTypeFlags_NEnemy && lllDragType == DragTypeFlags_Empty &&
					llllDragType == DragTypeFlags_NEnemy)
				{
					if (Score < int(SCORE_D_T))
						Score = SCORE_D_T;
				}
				if (right == DragTypeFlags_Empty && rrDragType == DragTypeFlags_NEnemy && rrrDragType == DragTypeFlags_Empty &&
					rrrrDragType == DragTypeFlags_NEnemy)
				{
					if (Score < int(SCORE_D_T))
						Score = SCORE_D_T;
				}

				if (left == DragTypeFlags_Empty && llDragType == DragTypeFlags_NEnemy && lllDragType == DragTypeFlags_Empty &&
					llllDragType == DragTypeFlags_Empty && right == DragTypeFlags_Empty)
				{
					if (Score < int(SCORE_LOW_LIVE_TWO))
						Score = SCORE_LOW_LIVE_TWO;
				}
				if (right == DragTypeFlags_Empty && rrDragType == DragTypeFlags_NEnemy && rrrDragType == DragTypeFlags_Empty &&
					rrrrDragType == DragTypeFlags_Empty && left == DragTypeFlags_Empty)
				{
					if (Score < int(SCORE_LOW_LIVE_TWO))
						Score = SCORE_LOW_LIVE_TWO;
				}

				if (left == DragTypeFlags_Empty && llDragType == DragTypeFlags_Empty && lllDragType == DragTypeFlags_NEnemy &&
					llllDragType == DragTypeFlags_Empty && right == DragTypeFlags_Empty)
				{
					if (Score < int(SCORE_LOW_LIVE_TWO))
						Score = SCORE_LOW_LIVE_TWO;
				}
				if (right == DragTypeFlags_Empty && rrDragType == DragTypeFlags_Empty && rrrDragType == DragTypeFlags_NEnemy &&
					rrrrDragType == DragTypeFlags_Empty && left == DragTypeFlags_Empty)
				{
					if (Score < int(SCORE_LOW_LIVE_TWO))
						Score = SCORE_LOW_LIVE_TWO;
				}
			}
			break;
			}
		}
	}

	return Score;
}

void AI::CalcDragType(Point leftPos, Point rightPos, int dir, int offset, DragTypeFlags_& leftDragType, DragTypeFlags_& rightDragType,
	char CLayout[][LINE_NUMS], ChessTypeFlags_ cMy)
{
	switch (dir)
	{
	case 0:
		leftPos.yPos -= offset;
		rightPos.yPos += offset;
		break;
	case 1:
		leftPos.xPos += offset;
		leftPos.yPos -= offset;
		rightPos.xPos -= offset;
		rightPos.yPos += offset;
		break;
	case 2:
		leftPos.xPos += offset;
		rightPos.xPos -= offset;
		break;
	case 3:
		leftPos.xPos += offset;
		leftPos.yPos += offset;
		rightPos.xPos -= offset;
		rightPos.yPos -= offset;
		break;
	}
	if (leftPos.xPos < 0 || leftPos.xPos >= LINE_NUMS || leftPos.yPos < 0 || leftPos.yPos >= LINE_NUMS)
		leftDragType = DragTypeFlags_Enemy;
	else
	{
		if (CLayout[leftPoint.xPos][leftPoint.yPos] == char(0))
			leftDragType = DragTypeFlags_Empty;
		else if (CLayout[leftPoint.xPos][leftPoint.yPos] == char(cMy))
			leftDragType = DragTypeFlags_NEnemy;
		else
			leftDragType = DragTypeFlags_Enemy;
	}
	if (rightPos.xPos < 0 || rightPos.xPos >= LINE_NUMS || rightPos.yPos < 0 || rightPos.yPos >= LINE_NUMS)
		rightDragType = DragTypeFlags_Enemy;
	else
	{
		if (CLayout[rightPoint.xPos][rightPoint.yPos] == char(0))
			rightDragType = DragTypeFlags_Empty;
		else if (CLayout[rightPoint.xPos][rightPoint.yPos] == char(cMy))
			rightDragType = DragTypeFlags_NEnemy;
		else
			rightDragType = DragTypeFlags_Enemy;
	}
}
