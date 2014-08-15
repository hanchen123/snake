#pragma once
#include "StdAfx.h"
#include <vector>
#include <cstdlib>
#include <time.h>
#include "SDL_Setup.h"
#include "Draw_Rect.h"

enum
{
	Up,
	Down,
	Left,
	Right,
	Quit
};

class Game
{
public:
	Game(int screen_w, int screen_h);
	~Game(void);
	void start(void);
	void draw_rect(int x, int y, Uint32 color, int w=10, int h=10);
	void cast_food(void);
	bool collision(void);
	void move(void);
	void score(void);

private:
	int points;
	int delay;
	int screen_w;
	int screen_h;
	int direction;
	bool get;	

	bool cheat;

	Draw_Rect* food;
	std::vector<Draw_Rect*> edge;
	std::vector<Draw_Rect*> snake;

	Mix_Chunk* effect;
	Mix_Music* music;

	SDL_Surface* surf;
	SDL_Texture* text;
	SDL_Rect rect;

	SDL_Setup* sdl_setup;
};