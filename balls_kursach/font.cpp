#include <SDL.h>
#include <SDL_ttf.h>
#include "font.h"
 
 
int sizeM = 35;
int sizeL = 70;
 
 
TTF_Font* FONT_M;
TTF_Font* FONT_L;
 

void fontInit() {
    FONT_M = TTF_OpenFont(FONT_TTF_NAME, sizeM);
    FONT_L = TTF_OpenFont(FONT_TTF_NAME, sizeL);
}

void font_drawText(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color, int cordX, int cordY, const char text[]) {
    int w, h;
    TTF_SizeUTF8(font, text, &w, &h);
    SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, text, color);

    SDL_Rect rect = { cordX - w / 2, cordY - h / 2, w, h };
    SDL_Texture* Texture_font = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_RenderCopy(renderer, Texture_font, NULL, &rect);
    SDL_DestroyTexture(Texture_font);
    SDL_FreeSurface(textSurface);
}