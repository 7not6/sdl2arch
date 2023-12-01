#include "common.h"

SDL_Renderer* renderer = NULL;


void prepareScene(void)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
}

void presentScene(void)
{
	SDL_RenderPresent(renderer);
}

void blit(SDL_Texture *texture, int x, int y)
{
	SDL_Rect dest;
	
	dest.x = x;
	dest.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);
	
	SDL_RenderCopy(renderer, texture, NULL, &dest);
}

void blitRect(SDL_Texture *texture, SDL_Rect *src, int x, int y)
{
	SDL_Rect dest;
	
	dest.x = x;
	dest.y = y;
	dest.w = src->w;
	dest.h = src->h;
	
	SDL_RenderCopy(renderer, texture, src, &dest);
}

void blitRectScale(SDL_Texture *texture, SDL_Rect *src, int x, int y,int scale)
{
	SDL_Rect dest;
	
	dest.x = x;
	dest.y = y;
	dest.w = src->w*scale;
	dest.h = src->h*scale;
	
	SDL_RenderCopy(renderer, texture, src, &dest);
}

