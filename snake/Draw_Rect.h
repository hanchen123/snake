#pragma once

class Draw_Rect
{
public:
	Draw_Rect(SDL_Renderer* pass_renderer, int x, int y, int w = 10, int h = 10, Uint32 color = 0xffffff);
	~Draw_Rect(void);
	void Draw();

	void Setx(int x);
	void Sety(int y);
	int Getx(void);
	int Gety(void);

private: 
	SDL_Surface* surf;
	SDL_Texture* text;
	SDL_Rect rect;
	SDL_Renderer* renderer;
};

