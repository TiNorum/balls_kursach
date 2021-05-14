#include "game_properties.h"
 
 
void click_menu(SDL_Window* window,SDL_Renderer* renderer, int i) {

	switch (i)
	{
	case 0:
		//start geme
		start_game(window, renderer);
		break;
	case 1:
		//table of records
	case 2:
		// options


	default:
		break;
	}

}


void draw_menu(SDL_Window* window)
{
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_Texture* textureStartGame;
	SDL_Texture* textureExit;
	SDL_Texture* textureOptions;
	SDL_Texture* textureRecords;

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
	
	SDL_FreeSurface(obj_img);
	//--------------------------------------------------------------------------->>


	
	//<<---------------------расчитываем размер и расположение кнопок----------------

	int height = SCREEN_HEIGHT * 0.1; //высота кнопки
	int width = SCREEN_WIDTH * 0.5;//ширина кнопки

	SDL_Rect rects_textures[4];

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
	Mix_Init(0);

	SDL_Window* window = SDL_CreateWindow("Smarty bubbles", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024);
	Mix_Volume(-1, 70);

	//вызываем функцию отрисовки меню
	draw_menu(window);


	Mix_CloseAudio();
	TTF_Quit();
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}