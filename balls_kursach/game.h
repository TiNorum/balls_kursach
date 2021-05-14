#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "SDL_mixer.h"
#include "font.h" 
#include "records.h"


static int SCREEN_WIDTH = 900;
static int SCREEN_HEIGHT = 900;
static int MUSIC_VOLUME = 100;
static int SOUND_VOLUME = 100;
static int VOLUME = 100;

static bool isExit = false;



const int maxBalls = 15;
const int maxRows = 15;


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

struct Balls; 

void start_game(SDL_Window* window, SDL_Renderer* render);