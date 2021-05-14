#ifndef font_h
#define font_h
#include <SDL.h>
#include <SDL_ttf.h>

#define FONT_TTF_NAME "resource\\Font\\Text.ttf"

 
extern TTF_Font* FONT_M;
extern TTF_Font* FONT_L;
 


void fontInit();
void font_drawText(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color, int cordX, int cordY,const char text[]);


#endif