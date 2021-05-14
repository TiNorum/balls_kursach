#include "game.h"

  
SDL_Renderer* renderer;
SDL_Texture* textureRedBall;
SDL_Texture* textureBludeBall;
SDL_Texture* textureGreenBall;
SDL_Texture* texturePurpleBall;
SDL_Texture* textureYellowBall;
SDL_Texture* textureLightBlueBall;
SDL_Texture* textureFon;

SDL_Rect fonPosition = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

GameField field;

 
int bubblesCleared = 0;
int ballShot = 0;
int largestGroup = 0;
bool isGame = false;



// подсчитываем количество соседних шариков одного цвета
int count_neighbours(ball_color color,int row, int index)
{
	if (field.rows[row].balls[index].checked) return 0;
	if (field.rows[row].balls[index].color != color) return 0;

	int k = 1;
	field.rows[row].balls[index].checked = true;

	int space = field.rows[row].balls[0].rect.x < field.ball_size / 2 ? 0 : 1;


	if (index > 0 )
		k += count_neighbours( color, row, index - 1);


	if (row > 0 && (index != 0 || space))
	{
		k += count_neighbours(color, row-1, index - 1 + space);
	}

	if (row > 0 && (index < maxBalls-1 || space))
	{
		k += count_neighbours(color, row - 1, index   + space);
	}


	if (index < maxBalls - 1)
		k += count_neighbours(color, row, index + 1);
	

	if (row < maxRows - 1   )
	{
		if (index != 0 || space)
			k += count_neighbours( color, row + 1, index - 1 + space);
		if (index < maxBalls - 1 || space)
			k += count_neighbours( color, row + 1, index + space);
	}
	
	return k;
}

// удаляем метки
void delet_checked()
{
	for (int i = 0; i < maxRows; i++)
		for (int j = 0; j < maxBalls; j++)
			field.rows[i].balls[j].checked = false;
}

void init_rows()
{ 
	int i = 0;

	for (; i < maxRows; i++)
	{
		for (int j = 0; j < maxBalls; j++)
		{
			field.rows[i].balls[j].rect.w = field.rows[i].balls[j].rect.h = field.ball_size;
			field.rows[i].balls[j].rect.x = (field.ball_size * j) + ((i % 2 == 0) ? field.ball_size / 2 : 0);
			field.rows[i].balls[j].rect.y = field.ball_size * i - field.ball_size *0.15*i;

			if (i < maxRows/3)
				field.rows[i].balls[j].color = ball_color(rand() % ball_color::AMOUNT);
			else
				field.rows[i].balls[j].color = ball_color::NONE;
		}
		
	}
	
}


void init_game( )
{
	init_rows();

	bubblesCleared = 0;
	ballShot = 0;
	largestGroup = 0;
	field.score = 0;
	field.current_ball.rect = { SCREEN_WIDTH / 2 - field.ball_size / 2, SCREEN_HEIGHT - field.ball_size - 10,field.ball_size,field.ball_size };
	field.current_ball.color = ball_color(rand() % ball_color::AMOUNT);
	field.isBallFlying = false;
	field.next_ball.rect = { SCREEN_WIDTH / 2 + field.ball_size, SCREEN_HEIGHT - field.ball_size / 2 - 10, field.ball_size/2, field.ball_size/2};
	field.next_ball.color = ball_color(rand() % ball_color::AMOUNT); 
	isGame = true;
}


SDL_Texture* get_ball_texture(  ball_color color)
{
	switch (color)
	{
	case RED:
		return textureRedBall;
	case YELLOW:
		return textureYellowBall;
	case BLUE:
		return textureBludeBall;
	case PURPLE:
		return texturePurpleBall;
	case GREEN:
		return textureGreenBall;
	case LIGHT_BLUE:
		return textureLightBlueBall;
	default:
		return 0;
	}
}

//помечаем шары, которые не висят в воздухе
void set_mark(  int row, int index)
{
	if (field.rows[row].balls[index].checked) return;
	if (field.rows[row].balls[index].color == ball_color::NONE) return;

	field.rows[row].balls[index].checked = true;

	int space = field.rows[row].balls[0].rect.x < field.ball_size / 2 ? 0 : 1;


	if (index > 0)
		set_mark(row, index - 1);


	if (row > 0 && (index != 0 || space))
	{
		set_mark(row - 1, index - 1 + space);
	}

	if (row > 0 && (index < maxBalls - 1 || space))
	{
		set_mark(row - 1, index + space);
	}


	if (index < maxBalls - 1)
		set_mark(row, index + 1);


	if (row < maxRows - 1)
	{
		if (index != 0 || space)
			set_mark( row + 1, index - 1 + space);
		if (index < maxBalls - 1 || space)
			set_mark(row + 1, index + space);
	}

}

//удаляем шары, которые висят в воздухе
int delete_falling_ball()
{
	for (int i = 0; i < maxBalls; i++)
	{
		set_mark( 0, i);
	}

	int count = 0;
	for (int i = 0; i < maxRows; i++)
		for (int j = 0; j < maxBalls; j++)
		{
			if (!field.rows[i].balls[j].checked && field.rows[i].balls[j].color != ball_color::NONE)
			{
				field.rows[i].balls[j].color = ball_color::NONE;
				count++;
			}
			field.rows[i].balls[j].checked = false;
		}
	return count;
}


//проверяем шары на коллизию
bool MacroCollision(SDL_Rect obj1, SDL_Rect obj2) {
	 
	double   r, x1, x2, y1, y2;
	r = obj1.w / 2;
	x1 = obj1.x + obj1.w / 2;
	y1 = obj1.y + obj1.h / 2;
	
	x2 = obj2.x + obj2.w / 2;
	y2 = obj2.y + obj2.h / 2;

	return 4*r * r >= (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}


void add_row()
{
	BallsRow temp2, temp;

	temp2 = field.rows[0];

	for (int i = 1; i < maxRows-1; i++)
	{
		temp = field.rows[i];
		field.rows[i] = temp2;
		for (int j = 0; j < maxBalls; j++)
		{
			field.rows[i].balls[j].rect.y += field.ball_size - field.ball_size * 0.15;
		}
		temp2 = temp;
	}

	BallsRow row;

	for (int j = 0; j < maxBalls; j++)
	{
		row.balls[j].rect.w = row.balls[j].rect.h = field.ball_size;
		row.balls[j].rect.x = (field.ball_size * j) + (field.rows[1].balls[0].rect.x < field.ball_size/2 ? field.ball_size / 2:0);
		row.balls[j].rect.y = 0;
 
		row.balls[j].color = ball_color(rand() % ball_color::AMOUNT);
	 
	}

	field.rows[0] = row;
}

//удаляем шары отмеченные меткой(сбрасываем цвет)
void delete_balls( ball_color color, int row, int index)
{
	 
	if (field.rows[row].balls[index].color != color) return;

	field.rows[row].balls[index].color = ball_color::NONE;
 

	int space = field.rows[row].balls[0].rect.x < field.ball_size / 2 ? 0 : 1;


	if (index > 0)
		delete_balls( color, row, index - 1);


	if (row > 0 && (index != 0 || space))
	{
		 delete_balls( color, row - 1, index - 1 + space);
	}

	if (row > 0 && (index < maxBalls - 1 || space))
	{
		delete_balls( color, row - 1, index + space);
	}


	if (index < maxBalls - 1)
		delete_balls( color, row, index + 1);


	if (row < maxRows - 1)
	{
		if (index != 0 || space)
			delete_balls( color, row + 1, index - 1 + space);
		if (index < maxBalls - 1 || space)
			delete_balls( color, row + 1, index + space);
	}
	

}

//вызывается после остановки летящего шара 
void set_ball(int row, int col)
{
	//удаляем маркеры
	delet_checked();

	//задаем свободной ячейки цвет
	field.rows[row].balls[col].color = field.current_ball.color;

	//обновляем цвет и позицию след шарика
	field.current_ball.rect = { SCREEN_WIDTH / 2 - field.ball_size / 2, SCREEN_HEIGHT - field.ball_size - 10,field.ball_size,field.ball_size };
	field.current_ball.color = field.next_ball.color;
	field.next_ball.color = ball_color(rand() % ball_color::AMOUNT);



	//если соседних шариков одинакого цвета больше 2, то мы их лопаем
	int k = count_neighbours( field.rows[row].balls[col].color, row, col);
	if (largestGroup < k)
		largestGroup = k;

	if (k > 2)
	{
		delete_balls( field.rows[row].balls[col].color, row, col);
		k+=delete_falling_ball();

		bubblesCleared += k;
		field.score += k*5;
	}
	else
		field.addRow--;


	field.isBallFlying = false;
}

//двигаем шар и проверяем на коллизии 
void move_ball()
{
	if (field.current_ball.rect.y <= 0)
	{ 
		set_ball(0, (field.current_ball.rect.x + field.rows[0].balls[0].rect.x) / field.ball_size);
	 
		return;
	}

	for (int i = 0; i < maxRows; i++)
		for (int j = 0; j < maxBalls; j++)
			if (field.rows[i].balls[j].color != ball_color::NONE && MacroCollision(field.rows[i].balls[j].rect, field.current_ball.rect))
			{
				int  r,x1, x2, y1, y2,row,col;
				int space = field.rows[i].balls[0].rect.x < field.ball_size / 2 ? 1 : 0;

				x1 = field.current_ball.rect.x + field.current_ball.rect.w / 2;
				y1 = field.current_ball.rect.y + field.current_ball.rect.h / 2;

				x2 = field.rows[i].balls[j].rect.x + field.rows[i].balls[j].rect.w / 2;
				y2 = field.rows[i].balls[j].rect.y + field.rows[i].balls[j].rect.h / 2;
				r = field.current_ball.rect.w / 2;

				if (y2  + r > y1)
				{
					row = i;
					if (x2 > x1)
					{

						col = j - 1;
					}
					else
						col = j + 1;
				}
				else
				{
					row = i+1; 
					if (x2 > x1)
						col = j - space;
					else  
						col = j + 1 - space;
				}

				set_ball( row, col);

				return;
			}
			

	if (field.current_ball.rect.x <= 0 || field.current_ball.rect.x + field.current_ball.rect.w >= SCREEN_WIDTH)
		field.angleMoveBall = 3.14 - field.angleMoveBall;


	field.current_ball.rect.x += 10 * cos(field.angleMoveBall);
	field.current_ball.rect.y -= 10 * sin(field.angleMoveBall);
}

//проверяем на то, что нет шариков за границей
bool checkGameEnd()
{
	for (int i = maxRows - 1; i >= 0; i--)
	{
		for (int j = 0;  field.rows[i].balls[j].rect.y > SCREEN_HEIGHT * 0.7 && j < maxBalls; j++)
			if (field.rows[i].balls[j].color != ball_color::NONE)
			{
				return true;
			}
	}
	return false;
}
 

//диалог в конце игры, показывает счет и некоторые параметры
void gameMenu()
{
	int marginX = SCREEN_HEIGHT * 0.3;
	int marginY = SCREEN_WIDTH * 0.3;

	SDL_SetRenderDrawColor(renderer, 65, 22, 194, 100);
	SDL_Rect rect = { marginX , marginY , SCREEN_WIDTH - 2 * marginX,SCREEN_HEIGHT - 2 * marginY };

	SDL_RenderFillRect(renderer, &rect);

	int padingX = (SCREEN_WIDTH - 2 * marginX) * 0.05;
	int padingY = (SCREEN_HEIGHT - 2 * marginY) * 0.05;

	int w, h;
	int x, y;


	SDL_Rect rectContinie, rectRestart, rectExit;


	TTF_SizeUTF8(FONT_L, "Continie", &w, &h);
	x = SCREEN_WIDTH / 2;
	y = marginY + padingY + h / 2;
	rectContinie = { x - w / 2 - 10 ,y - h / 2,w + 20,h };
	font_drawText(renderer, FONT_L, { 253,255,103 }, x, y, "Continue");

	y += padingY + h / 2;

	TTF_SizeUTF8(FONT_L, "Restart", &w, &h);
	y += h / 2;
	rectRestart = { x - w / 2 - 10 ,y - h / 2,w + 20,h };
	font_drawText(renderer, FONT_L, { 253,255,103 }, x, y, "Restart");

	y += padingY + h / 2;

	TTF_SizeUTF8(FONT_L, "Exit", &w, &h);
	y += h / 2;
	rectExit = { x - w / 2 - 10 ,y - h / 2,w + 20,h };
	font_drawText(renderer, FONT_L, { 253,255,103 }, x, y, "Exit");



	SDL_RenderPresent(renderer);
	SDL_Event event;

	bool exit = false;
	while (!exit)
		while (SDL_PollEvent(&event)) {
			switch (event.type)
			{
			case SDL_QUIT:
				isExit = exit = true;
				isGame = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				//exit
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					if (event.button.x >= rectExit.x && event.button.x <= rectExit.x + rectExit.w && event.button.y >= rectExit.y && event.button.y <= rectExit.y + rectExit.h)
					{
						isGame = false;
						exit = true;
					}
					//restart
					if (event.button.x >= rectRestart.x && event.button.x <= rectRestart.x + rectRestart.w && event.button.y >= rectRestart.y && event.button.y <= rectRestart.y + rectRestart.h)
					{
						exit = true;
						init_game();
					}
					//continue
					if (event.button.x >= rectContinie.x && event.button.x <= rectContinie.x + rectContinie.w && event.button.y >= rectContinie.y && event.button.y <= rectContinie.y + rectContinie.h)
					{
						exit = true;
					}
				}
				break;
			case SDL_KEYDOWN:
				if (event.button.button == SDL_SCANCODE_ESCAPE)
				{
					exit = true;
				}
			}
		}

}


//диалог в конце игры, показывает счет и некоторые параметры
void gameEndDialog()
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


	TTF_SizeUTF8(FONT_L, u8"Game over!", &w, &h);

	x = SCREEN_WIDTH / 2;
	y = marginY + h / 2;

	font_drawText(renderer, FONT_L, { 253,255,103 }, x, y, u8"Game over!");

	char score[10];
	SDL_itoa(field.score, score, 10);

	y += padingY + h / 2;
	TTF_SizeUTF8(FONT_L, score, &w, &h);
	y += h / 2;
	font_drawText(renderer, FONT_L, { 255,255,255 }, x, y, score);



	y += padingY + h / 2;
	TTF_SizeUTF8(FONT_L, "Bubble cleared", &w, &h);
	y += h / 2;
	x = marginX + w / 2 + padingX;
	font_drawText(renderer, FONT_L, { 255,255,255 }, x, y, "Bubble cleared");

	SDL_itoa(bubblesCleared, score, 10);
	TTF_SizeUTF8(FONT_L, score, &w, &h);
	x = SCREEN_WIDTH - w / 2 - padingX - marginX;
	font_drawText(renderer, FONT_L, { 255,255,255 }, x, y, score);

	y += padingY + h / 2;
	TTF_SizeUTF8(FONT_L, "Ball shot", &w, &h);
	y += h / 2;
	x = marginX + w / 2 + padingX;
	font_drawText(renderer, FONT_L, { 255,255,255 }, x, y, "Ball shot");

	SDL_itoa(ballShot, score, 10);
	TTF_SizeUTF8(FONT_L, score, &w, &h);
	x = SCREEN_WIDTH - w / 2 - padingX - marginX;
	font_drawText(renderer, FONT_L, { 255,255,255 }, x, y, score);

	y += padingY + h / 2;
	TTF_SizeUTF8(FONT_L, "Largest group", &w, &h);
	y += h / 2;
	x = marginX + w / 2 + padingX;
	font_drawText(renderer, FONT_L, { 255,255,255 }, x, y, "Largest group");

	SDL_itoa(largestGroup, score, 10);
	TTF_SizeUTF8(FONT_L, score, &w, &h);
	x = SCREEN_WIDTH - w / 2 - padingX - marginX;
	font_drawText(renderer, FONT_L, { 255,255,255 }, x, y, score);


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


void initTextures()
{
	//<<------------------------Загружаем текстуры------------------------------
	SDL_Surface* obj_img = IMG_Load("resource\\textures\\red.png");
	SDL_SetColorKey(obj_img, SDL_TRUE, SDL_MapRGB(obj_img->format, 255, 255, 255));
	textureRedBall = SDL_CreateTextureFromSurface(renderer, obj_img);

	obj_img = IMG_Load("resource\\textures\\yellow.png");
	SDL_SetColorKey(obj_img, SDL_TRUE, SDL_MapRGB(obj_img->format, 255, 255, 255));
	textureYellowBall = SDL_CreateTextureFromSurface(renderer, obj_img);

	obj_img = IMG_Load("resource\\textures\\purple.png");
	SDL_SetColorKey(obj_img, SDL_TRUE, SDL_MapRGB(obj_img->format, 255, 255, 255));
	texturePurpleBall = SDL_CreateTextureFromSurface(renderer, obj_img);

	obj_img = IMG_Load("resource\\textures\\orange.png");
	SDL_SetColorKey(obj_img, SDL_TRUE, SDL_MapRGB(obj_img->format, 255, 255, 255));
	textureLightBlueBall = SDL_CreateTextureFromSurface(renderer, obj_img);

	obj_img = IMG_Load("resource\\textures\\green.png");
	SDL_SetColorKey(obj_img, SDL_TRUE, SDL_MapRGB(obj_img->format, 255, 255, 255));
	textureGreenBall = SDL_CreateTextureFromSurface(renderer, obj_img);

	obj_img = IMG_Load("resource\\textures\\blue.png");
	SDL_SetColorKey(obj_img, SDL_TRUE, SDL_MapRGB(obj_img->format, 255, 255, 255));
	textureBludeBall = SDL_CreateTextureFromSurface(renderer, obj_img);

	obj_img = IMG_Load("resource\\textures\\fon.png");
	SDL_SetColorKey(obj_img, SDL_TRUE, SDL_MapRGB(obj_img->format, 255, 255, 255));
	textureFon = SDL_CreateTextureFromSurface(renderer, obj_img);

	SDL_FreeSurface(obj_img);
	//--------------------------------------------------------------------------->>
}


void draw_field()
{
	SDL_Event event;
	char score[20];

	int w, h;

	while (isGame)
	{
		while (SDL_PollEvent(&event)) {
			switch (event.type)
			{
			case SDL_QUIT:
				isExit = true;
				isGame = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				//Если игрок нажал на ЛКМ и в это время не летит шар
				if (event.button.button == SDL_BUTTON_LEFT && !field.isBallFlying)
				{
					ballShot++;
					field.isBallFlying = true;			//запускаем в полет шар
					field.angleMoveBall = field.angle;  //запоминам угол под которым полетел шар
				}

				break;
			case SDL_MOUSEMOTION:

				double x, y;
				double temp;

				x = event.button.x - SCREEN_WIDTH / 2;
				y = -(event.button.y - SCREEN_HEIGHT);

				//находим угол между  осью от центра шара и курсора
				temp = acos(x / sqrt(x * x + y * y));

				if (temp * 180 / 3.14 >= 15 && temp * 180 / 3.14 <= 165)
					field.angle = temp;

				break;
			case SDL_KEYDOWN:
				if (event.button.button == SDL_SCANCODE_ESCAPE)
				{
					gameMenu();

				}
			default:
				break;
			}
		}

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
		SDL_RenderClear(renderer);

		SDL_RenderCopy(renderer, textureFon, NULL, &fonPosition);


		for (int i = 0; i < maxRows; i++)
			for (int j = 0; j < maxBalls; j++)
				SDL_RenderCopy(renderer, get_ball_texture(field.rows[i].balls[j].color), NULL, &field.rows[i].balls[j].rect);


		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 0);

		SDL_RenderDrawLine(renderer, SCREEN_WIDTH / 2, SCREEN_HEIGHT - field.current_ball.rect.h / 2,
			SCREEN_WIDTH / 2 + 200 * cos(field.angle), SCREEN_HEIGHT - field.current_ball.rect.h / 2 - 200 * sin(field.angle));


		if (field.isBallFlying)
			move_ball();

		if (field.addRow == 0)
		{
			add_row();
			field.addRow = 4;
		}

		SDL_RenderCopy(renderer, get_ball_texture(field.current_ball.color), NULL, &field.current_ball.rect);
		SDL_RenderCopy(renderer, get_ball_texture(field.next_ball.color), NULL, &field.next_ball.rect);


		//отрисовываем счет
		SDL_itoa(field.score, score, 10);
		char text_score[20] = "score: ";
		strcat_s(text_score, score);
		TTF_SizeUTF8(FONT_L, text_score, &w, &h);
		font_drawText(renderer, FONT_L, { 255,255,255 }, 10 + w / 2, SCREEN_HEIGHT - h / 2 - 10, text_score);


		if (checkGameEnd())
		{
			SDL_Rect line = { 0,SCREEN_HEIGHT * 0.7 , SCREEN_WIDTH, 10 };
			SDL_RenderFillRect(renderer, &line);
			SDL_RenderPresent(renderer);
			int records[8];
			leaderboardRead(records);
			leaderboardUpdate(records, field.score);
			SDL_Delay(1500);
			isGame = false;
			gameEndDialog();
		}

		SDL_RenderPresent(renderer);
		SDL_Delay(10);
	}
}


void start_game(SDL_Window* window, SDL_Renderer*  render)
{
	renderer = render;

	srand(time(0));

	field.ball_size = int (SCREEN_WIDTH / (maxBalls + 0.5));

	initTextures();
	 
	init_game();
	fontInit();

	draw_field();


	SDL_DestroyTexture(textureBludeBall);
	SDL_DestroyTexture(textureGreenBall);
	SDL_DestroyTexture(textureLightBlueBall);
	SDL_DestroyTexture(texturePurpleBall);
	SDL_DestroyTexture(textureRedBall);
	SDL_DestroyTexture(textureYellowBall);
 }