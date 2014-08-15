#include "stdafx.h"
#include "Game.h"

Game::Game(int pass_w, int pass_h)
{
	screen_w = pass_w;
	screen_h = pass_h;

	sdl_setup = new SDL_Setup(screen_w, screen_h);

	Mix_OpenAudio(22040, MIX_DEFAULT_FORMAT, 2, 4096);
	music = Mix_LoadMUS("music/bg.wav");
	effect = Mix_LoadWAV("music/hit.wav");
	Mix_PlayMusic(music, -1);

	//put the edges
	edge.push_back(new Draw_Rect(sdl_setup->Get_Renderer(), 0, 0, screen_w, 10, 0xff0000));
	edge.push_back(new Draw_Rect(sdl_setup->Get_Renderer(), 0, 0, 10, screen_h - 40, 0xff0000));
	edge.push_back(new Draw_Rect(sdl_setup->Get_Renderer(), 0, screen_h - 50, screen_w, 10, 0xff0000));
	edge.push_back(new Draw_Rect(sdl_setup->Get_Renderer(), screen_w - 10, 0, 10, screen_h - 40, 0xff0000));

	//put the snake
	for(int i = 0; i < 5; i++)
		snake.push_back(new Draw_Rect(sdl_setup->Get_Renderer(), (40 + i)*10, 100, 10 , 10, 0x00ff00));

	points = 0;
	delay = 100;
	get = false;
	cheat = false;
	direction = Left;
	srand(time(0));
	cast_food();
}


Game::~Game(void)
{
	for(int i = 0; i < edge.size(); i++)
		delete edge[i];
	edge.clear();
	for(int i = 0; i < snake.size(); i++)
		delete snake[i];
	snake.clear();
	delete food;

	SDL_FreeSurface(surf);
	SDL_DestroyTexture(text);

	Mix_FreeChunk(effect);
	Mix_FreeMusic(music);
	Mix_CloseAudio();

	delete sdl_setup;
}

void Game::cast_food(void)
{
	while(1)
	{
		int pos_x = rand() % screen_w/10 + 1;
		int pos_y = rand() % screen_h/10 + 1;
		for(int i = 0; i < snake.size(); i++)
			if(snake[i]->Getx() == pos_x && snake[i]->Gety() == pos_y)
				continue;
		if(pos_x >= screen_w/10 - 2 || pos_y >= screen_h/10 - 5)
			continue;
		food = new Draw_Rect(sdl_setup->Get_Renderer(), pos_x*10, pos_y*10, 10 , 10, 0xff00ff);
		break;
	}
}

void Game::move(void)
{

		if(sdl_setup->Get_Main()->type == SDL_KEYDOWN)
		{
			switch(sdl_setup->Get_Main()->key.keysym.sym)
			{
				case SDLK_a:
					if(direction != Right)
						direction = Left;
					break;

				case SDLK_w:
					if(direction != Down)
						direction = Up;
					break;

				case SDLK_s:
					if(direction != Up)
						direction = Down;
					break;

				case SDLK_d:
					if(direction != Left)
						direction = Right;
					break;

				case SDLK_y:
					cheat = true;
					break;

				case SDLK_ESCAPE:
					direction = Quit;
					break;

				default:
					break;
			}
		}
		else if(sdl_setup->Get_Main()->type == SDL_QUIT)
			direction = Quit;

	if(!get)
	{
		delete snake[snake.size()-1];
		snake.pop_back();
	}
	if(direction == Left)
		snake.insert(snake.begin(), new Draw_Rect(sdl_setup->Get_Renderer(), snake[0]->Getx() - 10, snake[0]->Gety(), 10, 10, 0x00ff00));
	else if(direction == Right)
		snake.insert(snake.begin(), new Draw_Rect(sdl_setup->Get_Renderer(), snake[0]->Getx() + 10, snake[0]->Gety(), 10, 10, 0x00ff00));
	else if(direction == Up)
		snake.insert(snake.begin(), new Draw_Rect(sdl_setup->Get_Renderer(), snake[0]->Getx(), snake[0]->Gety() - 10, 10, 10, 0x00ff00));
	else if(direction	== Down)
		snake.insert(snake.begin(), new Draw_Rect(sdl_setup->Get_Renderer(), snake[0]->Getx(), snake[0]->Gety() + 10, 10, 10, 0x00ff00));
}

bool Game::collision(void)
{
	if(snake[0]->Getx() <= 10 || snake[0]->Getx() >= screen_w - 10 || snake[0]->Gety() <= 10 || snake[0]->Gety() >= screen_h - 50)
		return 1;

	for(int i = 2;i < snake.size(); i++)
		if(snake[0]->Getx() == snake[i]->Getx() && snake[0]->Gety() == snake[i]->Gety())
			return true;
	//get food
	if(snake[0]->Getx() == food->Getx() && snake[0]->Gety() == food->Gety())
	{
		Mix_PlayChannel(-1, effect, 0);
		get = true;
		delete food;
		cast_food();
		if (points <= 99980)
			points += 10;
		if((points % 100) == 0 && delay > 0)
			delay -= 10;
	}
	else
		get = false;	
	return false;	
}

void Game::score(void)
{
	char score[6];
	sprintf(score, "%d", points);
	SDL_Color color = {255,255,255};
	surf = TTF_RenderText_Blended(sdl_setup->Get_Font(), score, color);
	text = SDL_CreateTextureFromSurface(sdl_setup->Get_Renderer(), surf);
	int text_w = 0;
	int text_h = 0;
	SDL_QueryTexture(text, NULL, NULL, &text_w, &text_h);
	rect.x = 0;
	rect.y = screen_h - 40;
	rect.w = text_w;
	rect.h = text_h;
	SDL_RenderCopy(sdl_setup->Get_Renderer(), text, NULL, &rect);
}

void Game::start(void)
{
	while(sdl_setup->Get_Main()->type != SDL_QUIT) 
	{	
		sdl_setup->Start();
		if(cheat)
		{	
			if(points <= 98990)
				points += 1000;
			cheat = false;
		}

		if(collision())
		{
			std::cout << "Game over" << std::endl;
			break;
		}

		score();
		food->Draw();
		move();

        if(direction == Quit)
			break;
		for(int i = 0; i < edge.size(); i++)
			edge[i]->Draw();
		for(int i = 0;i < snake.size(); i++)
			snake[i]->Draw();
		SDL_Delay(delay);
		sdl_setup->End();
	}
}