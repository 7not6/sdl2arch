#include "common.h"
#include "font10x16.h"

extern SDL_Renderer* renderer;

static char drawTextBuffer[MAX_LINE_LENGTH];
static SDL_Texture *fontTexture;

void initFonts(void)
{
	SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, "init font10x16 texture" );	
	//fontTexture =  IMG_LoadTexture(renderer, "./font10x16.png");	
	SDL_Surface *tmp = NULL;
	tmp = SDL_CreateRGBSurfaceFrom((void *)font10x16, 160, 256, 32,160*4, 0xff000000, 0x00ff0000, 0x0000ff00,0x000000ff);
	
	if(NULL == tmp){
		fprintf(stderr, "Error SDL_CreateRGBSurface tmp: %s", SDL_GetError());	
		goto quit;
	}
		
	fontTexture = SDL_CreateTextureFromSurface(renderer, tmp);
	
	SDL_FreeSurface(tmp);
	
	quit:

	if (!fontTexture)printf("create font failed\n");
}

void deinitFonts(void)
{
	if (fontTexture){
		//printf("destroy font\n");
		SDL_DestroyTexture(fontTexture);
		
	}
}

void drawChar(int x, int y, int r, int g, int b, char c)
{
	SDL_Rect rect;
	
	rect.w = GLYPH_WIDTH;
	rect.h = GLYPH_HEIGHT;
	
	SDL_SetTextureColorMod(fontTexture, r, g, b);
	
	int cy=c/GLYPH_HEIGHT;
	int cx=c-cy*GLYPH_HEIGHT;
		
	rect.y = (cy*GLYPH_HEIGHT);
	rect.x = (cx*GLYPH_WIDTH);
					
	blitRect(fontTexture, &rect, x, y);
}

void drawCharScale(int x, int y, int r, int g, int b, char c,int scale)
{
	SDL_Rect rect;
	
	rect.w = GLYPH_WIDTH;
	rect.h = GLYPH_HEIGHT;
	
	SDL_SetTextureColorMod(fontTexture, r, g, b);
	
	int cy=c/GLYPH_HEIGHT;
	int cx=c-cy*GLYPH_HEIGHT;
		
	rect.y = (cy*GLYPH_HEIGHT);
	rect.x = (cx*GLYPH_WIDTH);
					
	blitRectScale(fontTexture, &rect, x, y,scale);
}

void drawText(int x, int y, int r, int g, int b, int align,char *format, ...)
{
	int i, len, c;
	SDL_Rect rect;
	va_list args;
	
	memset(&drawTextBuffer, '\0', sizeof(drawTextBuffer));

	va_start(args, format);
	vsprintf(drawTextBuffer, format, args);
	va_end(args);
	
	len = strlen(drawTextBuffer);
	
	switch (align)
	{
		case TEXT_RIGHT:
			x -= (len * GLYPH_WIDTH);
			break;
			
		case TEXT_CENTER:
			x -= (len * GLYPH_WIDTH) / 2;
			break;
	}
	
	rect.w = GLYPH_WIDTH;
	rect.h = GLYPH_HEIGHT;
	rect.y = 0;
	
	SDL_SetTextureColorMod(fontTexture, r, g, b);
		
	for (i = 0 ; i < len ; i++)
	{
		c = drawTextBuffer[i];
		
		int cy=c/GLYPH_HEIGHT;
		int cx=c-cy*GLYPH_HEIGHT;
		
		rect.y = (cy*GLYPH_HEIGHT);
		rect.x = (cx*GLYPH_WIDTH);
					
		blitRect(fontTexture, &rect, x, y);
		
		x += GLYPH_WIDTH;

	}
	
}

void drawTextScale(int x, int y, int r, int g, int b, int align,int scale,char *format, ...)
{
	int i, len, c;
	SDL_Rect rect;
	va_list args;
	
	memset(&drawTextBuffer, '\0', sizeof(drawTextBuffer));

	va_start(args, format);
	vsprintf(drawTextBuffer, format, args);
	va_end(args);
	
	len = strlen(drawTextBuffer);
	
	switch (align)
	{
		case TEXT_RIGHT:
			x -= (len * GLYPH_WIDTH);
			break;
			
		case TEXT_CENTER:
			x -= (len * GLYPH_WIDTH) / 2;
			break;
	}
	
	rect.w = GLYPH_WIDTH;
	rect.h = GLYPH_HEIGHT;
	rect.y = 0;
	
	SDL_SetTextureColorMod(fontTexture, r, g, b);
		
	for (i = 0 ; i < len ; i++)
	{
		c = drawTextBuffer[i];
		
		int cy=c/GLYPH_HEIGHT;
		int cx=c-cy*GLYPH_HEIGHT;
		
		rect.y = (cy*GLYPH_HEIGHT);
		rect.x = (cx*GLYPH_WIDTH);					
		
		blitRectScale(fontTexture, &rect, x, y,scale);
		x += GLYPH_WIDTH*scale;

	}
	
}
