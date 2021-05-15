#pragma once
#include "game_properties.h"
#include "font.h"

enum ball_color
{
	RED,
	YELLOW,
	BLUE,
	PURPLE,
	GREEN,
	LIGHT_BLUE,
	AMOUNT,
	NONE
};


struct Ball
{
	ball_color color;
	SDL_Rect rect;
	bool checked = false;
};


struct BallsRow
{
	bool start_space;
	Ball balls[maxBalls];

};


struct GameField
{
	BallsRow  rows[maxRows];
	int addRow = 4;			//количество промахов до новой строки шариков
	Ball current_ball;
	Ball next_ball;
	int score = 0;
	bool isBallFlying = false;
	int ball_size;
	double angle = 90 * 3.14 / 180; //угол в радианах
	double angleMoveBall;
 
};