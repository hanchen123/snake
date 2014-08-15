#include "StdAfx.h"
#include "Draw_Rect.h"


Draw_Rect::Draw_Rect(SDL_Renderer* pass_renderer, int x, int y, int w, int h, Uint32 color)
{
	renderer = pass_renderer;

	surf = NULL;
	surf = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
	if (surf  == NULL) {
		std::cout << "Surface load errer" << std::endl;
	}

	SDL_FillRect(surf, NULL, color);

	text = NULL;
	text = SDL_CreateTextureFromSurface(renderer, surf);
	if (text == NULL) {
		std::cout << "Texture load errer" << std::endl;
	}

	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
}

Draw_Rect::~Draw_Rect(void)
{
	SDL_FreeSurface(surf);
	SDL_DestroyTexture(text);
}

void Draw_Rect::Draw(void)
{
	SDL_RenderCopy(renderer, text, NULL, &rect);
}

void Draw_Rect::Setx(int x) 
{
	rect.x = x;
}

void Draw_Rect::Sety(int y) 
{
	rect.y = y;
}

int Draw_Rect::Getx()
{
	return rect.x;
}

int Draw_Rect::Gety()
{
	return rect.y;
}