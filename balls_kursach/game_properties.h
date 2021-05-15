#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "SDL_mixer.h"
using namespace std;

static int SCREEN_WIDTH = 900;
static int SCREEN_HEIGHT = 900;

static bool isExit = false;

static const int maxBalls = 11;
static const int maxRows = 12;



void start_game(SDL_Window* window, SDL_Renderer* renderer);