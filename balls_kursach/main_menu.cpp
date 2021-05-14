#include "game.h"
 
 
void menuRecords(SDL_Renderer* renderer)
{
	int marginX = SCREEN_HEIGHT * 0.2;
	int marginY = SCREEN_WIDTH * 0.1;

	SDL_SetRenderDrawColor(renderer, 65, 22, 194, 100);
	SDL_Rect rect = { marginX , marginY , SCREEN_WIDTH - 2 * marginX,SCREEN_HEIGHT - 2 * marginY };
	SDL_RenderFillRect(renderer, &rect);

	int padingX = (SCREEN_WIDTH - 2 * marginX) * 0.05;
	int padingY = (SCREEN_HEIGHT - 2 * marginY) * 0.05;

	int w, h;
	int x, y;


	TTF_SizeUTF8(FONT_L, u8"Leaderboard", &w, &h);

	x = SCREEN_WIDTH / 2;
	y = marginY + h / 2;

	font_drawText(renderer, FONT_L, { 253,255,103 }, x, y, u8"Leaderboard");

	int records[8];

	leaderboardRead(records);

	char score[10];
	for (int i = 0; i < 8; i++)
	{
		SDL_itoa(records[i], score, 10);
		y +=   h ;
		TTF_SizeUTF8(FONT_M, score, &w, &h);
		y += h / 2;
		font_drawText(renderer, FONT_M, { 255,255,255 }, x, y, score);
	}


	   

	TTF_SizeUTF8(FONT_L, "Continie", &w, &h);

	x = SCREEN_WIDTH / 2;
	y = SCREEN_HEIGHT - marginY - h / 2 - 5;

	rect = { x - w / 2 - 10 ,y - h / 2,w + 20,h };

	font_drawText(renderer, FONT_L, { 253,255,103 }, x, y, "Continue");


	SDL_RenderPresent(renderer);
	SDL_Event event;
	bool exit = false;
	while (!exit)
		while (SDL_PollEvent(&event)) {
			switch (event.type)
			{
			case SDL_QUIT:
				isExit = exit = true;
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT && (event.button.x >= rect.x && event.button.x <= rect.x + rect.w && event.button.y >= rect.y && event.button.y <= rect.y + rect.h))
					exit = true;
				break;

			}
		}
}


void menuSettings(SDL_Renderer* renderer)
{
	SDL_Event event;
	bool exit = false;
	while (!exit)
	{
	int marginX = SCREEN_HEIGHT * 0.1;
	int marginY = SCREEN_WIDTH * 0.1;

	SDL_SetRenderDrawColor(renderer, 65, 22, 194, 100);
	SDL_Rect rect = { marginX , marginY , SCREEN_WIDTH - 2 * marginX,SCREEN_HEIGHT - 2 * marginY };
	SDL_RenderFillRect(renderer, &rect);

	int padingX = (SCREEN_WIDTH - 2 * marginX) * 0.05;
	int padingY = (SCREEN_HEIGHT - 2 * marginY) * 0.05;

	int w, h;
	int x, y;
	char text[5];

	TTF_SizeUTF8(FONT_L, u8"Options", &w, &h);

	x = SCREEN_WIDTH / 2;
	y = marginY + h / 2;

	font_drawText(renderer, FONT_L, { 253,255,103 }, x, y, u8"Options");


	y += padingY + h / 2;
	TTF_SizeUTF8(FONT_L, "music volume", &w, &h);
	y += h / 2;
	x = marginX + w / 2 + padingX;
	font_drawText(renderer, FONT_L, { 255,255,255 }, x, y, "music volume");


	TTF_SizeUTF8(FONT_L, ">", &w, &h);
	x = SCREEN_WIDTH - w / 2 - padingX - marginX;
	font_drawText(renderer, FONT_L, { 253,255,103 }, x, y, ">");
	x -= w / 2;

	SDL_itoa(MUSIC_VOLUME, text, 10);
	TTF_SizeUTF8(FONT_L, text, &w, &h);
	x -= w / 2;
	font_drawText(renderer, FONT_L, { 255,255,255 }, x, y, text);

	x -= w / 2;
	TTF_SizeUTF8(FONT_L, "<", &w, &h);
	x -= w / 2;
	font_drawText(renderer, FONT_L, { 253,255,103 }, x, y, "<");




	y += padingY + h / 2;
	TTF_SizeUTF8(FONT_L, "sound volume", &w, &h);
	y += h / 2;
	x = marginX + w / 2 + padingX;
	font_drawText(renderer, FONT_L, { 255,255,255 }, x, y, "sound volume");

	TTF_SizeUTF8(FONT_L, ">", &w, &h);
	x = SCREEN_WIDTH - w / 2 - padingX - marginX;
	font_drawText(renderer, FONT_L, { 253,255,103 }, x, y, ">");
	x -= w / 2;

	SDL_itoa(SOUND_VOLUME, text, 10);
	TTF_SizeUTF8(FONT_L, text, &w, &h);
	x -= w / 2;
	font_drawText(renderer, FONT_L, { 255,255,255 }, x, y, text);

	x -= w / 2;
	TTF_SizeUTF8(FONT_L, "<", &w, &h);
	x -= w / 2;
	font_drawText(renderer, FONT_L, { 253,255,103 }, x, y, "<");


	TTF_SizeUTF8(FONT_L, "Continie", &w, &h);

	x = SCREEN_WIDTH / 2;
	y = SCREEN_HEIGHT - marginY - h / 2 - 5;

	rect = { x - w / 2 - 10 ,y - h / 2,w + 20,h };

	font_drawText(renderer, FONT_L, { 253,255,103 }, x, y, "Continue");


	SDL_RenderPresent(renderer);
	

	while (SDL_PollEvent(&event)) {
		switch (event.type)
		{
		case SDL_QUIT:
			isExit = exit = true;
			break;

		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_LEFT && (event.button.x >= rect.x && event.button.x <= rect.x + rect.w && event.button.y >= rect.y && event.button.y <= rect.y + rect.h))
				exit = true;
			break;

		}
	}
	}
}

 
void click_menu(SDL_Window* window, SDL_Renderer* renderer, int i) {

	switch (i)
	{
	case 0:
		//start geme
		start_game(window, renderer);
		break;
	case 1:
		//table of records
		menuRecords(renderer);
		break;
	case 2:
		// options
		menuSettings(renderer);
		break;
	default:
		break;
	}

}
 

void drawMainMenu(SDL_Window* window)
{
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_Texture* textureStartGame;
	SDL_Texture* textureExit;
	SDL_Texture* textureOptions;
	SDL_Texture* textureRecords;
	SDL_Texture* textureFon;

	//<<------------------------Загружаем текстуры кнопок-------------------------
	SDL_Surface* obj_img = IMG_Load("resource\\textures\\records.bmp");
	SDL_SetColorKey(obj_img, SDL_TRUE, SDL_MapRGB(obj_img->format, 255, 255, 255));
	textureRecords = SDL_CreateTextureFromSurface(renderer, obj_img);

	obj_img = IMG_Load("resource\\textures\\options.bmp");
	SDL_SetColorKey(obj_img, SDL_TRUE, SDL_MapRGB(obj_img->format, 255, 255, 255));
	textureOptions = SDL_CreateTextureFromSurface(renderer, obj_img);

	obj_img = IMG_Load("resource\\textures\\start_game.bmp");
	SDL_SetColorKey(obj_img, SDL_TRUE, SDL_MapRGB(obj_img->format, 255, 255, 255));
	textureStartGame = SDL_CreateTextureFromSurface(renderer, obj_img);
	 
	obj_img = IMG_Load("resource\\textures\\exit.bmp");
	SDL_SetColorKey(obj_img, SDL_TRUE, SDL_MapRGB(obj_img->format, 255, 255, 255));
	textureExit = SDL_CreateTextureFromSurface(renderer, obj_img);
	
	obj_img = IMG_Load("resource\\textures\\fon.png");
	SDL_SetColorKey(obj_img, SDL_TRUE, SDL_MapRGB(obj_img->format, 255, 255, 255));
	textureFon = SDL_CreateTextureFromSurface(renderer, obj_img);
	
	SDL_FreeSurface(obj_img);
	//--------------------------------------------------------------------------->>


	
	//<<---------------------расчитываем размер и расположение кнопок----------------

	int height = SCREEN_HEIGHT * 0.1; //высота кнопки
	int width = SCREEN_WIDTH * 0.5;//ширина кнопки

	SDL_Rect rects_textures[4];
	SDL_Rect fon_rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	for (int i = 0; i < 4; i++)
	{
		rects_textures[i] = { int(SCREEN_WIDTH * 0.25), int(SCREEN_HEIGHT * 0.3 )+ height * i, width, height };
	}

	//--------------------------------------------------------------------------->>

	 
	SDL_Event event;

	 	 
	while (!isExit)
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
		while (SDL_PollEvent(&event)) {
			switch (event.type)
			{
			case SDL_QUIT:
				isExit = true;
				break;
			case SDL_MOUSEBUTTONDOWN:
				//отлавливаем нажатие и смотрим - не нажмал ли пользователь на кнопки в меню
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					for (int i = 0; i < 4; i++)
						if (rects_textures[i].x <= event.button.x && rects_textures[i].x + rects_textures[i].w >= event.button.x && rects_textures[i].y <= event.button.y && rects_textures[i].y + rects_textures[i].h >= event.button.y)
						{
							//если нажали на выход, то просто выходим из приложения
							if (i == 3)
							{
								isExit = true;

							}
							// если нажатие на другие кнопки, то переходим к ним
							else
								click_menu(window,renderer, i);
						}
				}
				 
				break;
			default:
				break;
			}
		}

		SDL_RenderClear(renderer);

		SDL_RenderCopy(renderer, textureFon, NULL, &fon_rect);
		SDL_RenderCopy(renderer, textureStartGame, NULL, &rects_textures[0]);
		SDL_RenderCopy(renderer, textureRecords, NULL, &rects_textures[1]);
		SDL_RenderCopy(renderer, textureOptions, NULL, &rects_textures[2]);
		SDL_RenderCopy(renderer, textureExit, NULL, &rects_textures[3]);

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyTexture(textureStartGame);
	SDL_DestroyTexture(textureRecords);
	SDL_DestroyTexture(textureOptions);
	SDL_DestroyTexture(textureExit);
	SDL_DestroyRenderer(renderer);
}
 

int main(int argc, char** argv)
{
	//инициализируем всякую дребедень свзянную с сдл
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
		return 3;
	}

	int flags = IMG_INIT_PNG;
	if (!(IMG_Init(flags) & flags)) {
		std::cout << "Can't init image: " << IMG_GetError() << std::endl;
		return false;
	}
	TTF_Init();
	fontInit();
	Mix_Init(0);

	SDL_Window* window = SDL_CreateWindow("Smarty bubbles", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);
	Mix_Volume(-1, 70);

	//вызываем функцию отрисовки меню
	drawMainMenu(window);


	Mix_CloseAudio();
	TTF_Quit();
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

